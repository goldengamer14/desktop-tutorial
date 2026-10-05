"""
Windows Application Resolver & Launcher
Detects, resolves, and launches any installed application or Windows component.
"""

import os
import sys
import re
import csv
import io
import json
import shutil
import difflib
import winreg
import subprocess
import psutil
from typing import Dict, List, Tuple, Optional, Any

# Common Windows built-ins and aliases
KNOWN_ALIASES = {
    "notepad": "notepad.exe",
    "notes": "notepad.exe",
    "calc": "calc.exe",
    "calculator": "calc.exe",
    "paint": "mspaint.exe",
    "mspaint": "mspaint.exe",
    "cmd": "cmd.exe",
    "command prompt": "cmd.exe",
    "terminal": "wt.exe",
    "windows terminal": "wt.exe",
    "powershell": "powershell.exe",
    "powershell core": "pwsh.exe",
    "explorer": "explorer.exe",
    "file explorer": "explorer.exe",
    "files": "explorer.exe",
    "task manager": "taskmgr.exe",
    "taskmgr": "taskmgr.exe",
    "control panel": "control.exe",
    "control": "control.exe",
    "settings": "ms-settings:",
    "windows settings": "ms-settings:",
    "chrome": "chrome.exe",
    "google chrome": "chrome.exe",
    "edge": "msedge.exe",
    "microsoft edge": "msedge.exe",
    "firefox": "firefox.exe",
    "mozilla firefox": "firefox.exe",
    "word": "winword.exe",
    "ms word": "winword.exe",
    "excel": "excel.exe",
    "ms excel": "excel.exe",
    "powerpoint": "powerpnt.exe",
    "ppt": "powerpnt.exe",
    "code": "code",
    "vscode": "code",
    "vs code": "code",
    "visual studio code": "code",
    "snipping tool": "snippingtool.exe",
    "snip": "snippingtool.exe",
    "camera": "microsoft.windows.camera:",
    "clock": "ms-clock:",
    "alarms": "ms-clock:",
    "regedit": "regedit.exe",
    "registry editor": "regedit.exe",
    "device manager": "devmgmt.msc",
    "disk management": "diskmgmt.msc",
    "services": "services.msc",
}


def normalize_string(s: str) -> str:
    """Removes non-alphanumeric characters and converts to lowercase for fuzzy matching."""
    return re.sub(r"[^a-z0-9]", "", s.lower())


class AppResolver:
    def __init__(self, cache_file: Optional[str] = None):
        self.cache_file = cache_file or os.path.join(
            os.path.dirname(os.path.abspath(__file__)), "apps_cache.json"
        )
        self.start_apps: Dict[str, str] = {}  # Display Name -> AppID
        self.registry_apps: Dict[str, str] = {}  # Executable name -> Full Path
        self.shortcut_apps: Dict[str, str] = {}  # Display Name -> .lnk Path
        self.load_cache_or_scan()

    def scan_all(self):
        """Scans all sources of applications on the Windows system."""
        self._scan_start_apps()
        self._scan_registry_apps()
        self._scan_shortcuts()
        self.save_cache()

    def _scan_start_apps(self):
        """Queries Get-StartApps via PowerShell for all registered modern and classic apps."""
        try:
            cmd = [
                "powershell",
                "-NoProfile",
                "-NonInteractive",
                "-Command",
                "Get-StartApps | ConvertTo-Csv -NoTypeInformation",
            ]
            proc = subprocess.run(
                cmd, capture_output=True, text=True, encoding="utf-8", timeout=15
            )
            if proc.returncode == 0:
                reader = csv.DictReader(io.StringIO(proc.stdout))
                for row in reader:
                    name = row.get("Name", "").strip()
                    app_id = row.get("AppID", "").strip()
                    if name and app_id:
                        self.start_apps[name] = app_id
        except Exception as e:
            print(f"[AppResolver] Warning scanning StartApps: {e}", file=sys.stderr)

    def _scan_registry_apps(self):
        """Scans Windows Registry App Paths for registered executable paths."""
        reg_keys = [
            (
                winreg.HKEY_LOCAL_MACHINE,
                r"SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths",
            ),
            (
                winreg.HKEY_CURRENT_USER,
                r"Software\Microsoft\Windows\CurrentVersion\App Paths",
            ),
        ]
        for root, subkey in reg_keys:
            try:
                with winreg.OpenKey(root, subkey) as key:
                    num_subkeys = winreg.QueryInfoKey(key)[0]
                    for i in range(num_subkeys):
                        try:
                            app_name = winreg.EnumKey(key, i)
                            with winreg.OpenKey(key, app_name) as sub:
                                path_val, _ = winreg.QueryValueEx(sub, "")
                                if path_val and os.path.exists(path_val):
                                    clean_name = (
                                        app_name.lower().removesuffix(".exe").strip()
                                    )
                                    self.registry_apps[clean_name] = path_val
                        except Exception:
                            continue
            except Exception:
                continue

    def _scan_shortcuts(self):
        """Scans Start Menu folders for .lnk shortcut files."""
        dirs = [
            os.path.join(
                os.environ.get("PROGRAMDATA", ""),
                r"Microsoft\Windows\Start Menu\Programs",
            ),
            os.path.join(
                os.environ.get("APPDATA", ""),
                r"Microsoft\Windows\Start Menu\Programs",
            ),
        ]
        for d in dirs:
            if os.path.exists(d):
                for root, _, files in os.walk(d):
                    for f in files:
                        if f.lower().endswith(".lnk"):
                            name = os.path.splitext(f)[0].strip()
                            self.shortcut_apps[name] = os.path.join(root, f)

    def save_cache(self):
        """Saves scanned application data to a JSON cache."""
        data = {
            "start_apps": self.start_apps,
            "registry_apps": self.registry_apps,
            "shortcut_apps": self.shortcut_apps,
        }
        try:
            with open(self.cache_file, "w", encoding="utf-8") as f:
                json.dump(data, f, indent=2)
        except Exception as e:
            print(f"[AppResolver] Failed to write cache: {e}", file=sys.stderr)

    def load_cache_or_scan(self):
        """Loads from cache if available; otherwise scans."""
        if os.path.exists(self.cache_file):
            try:
                with open(self.cache_file, "r", encoding="utf-8") as f:
                    data = json.load(f)
                    self.start_apps = data.get("start_apps", {})
                    self.registry_apps = data.get("registry_apps", {})
                    self.shortcut_apps = data.get("shortcut_apps", {})
                if self.start_apps or self.registry_apps or self.shortcut_apps:
                    return
            except Exception:
                pass
        self.scan_all()

    def resolve(self, query: str) -> Optional[Dict[str, Any]]:
        """
        Resolves a user-provided application query into a launchable target.
        Returns a dictionary with:
          - 'name': Friendly display name
          - 'target': Launch target (shell:AppsFolder, path, URI, or command)
          - 'type': 'start_app' | 'registry' | 'shortcut' | 'alias' | 'path' | 'uri'
        """
        q = query.strip()
        if not q:
            return None

        q_lower = q.lower()
        q_norm = normalize_string(q)

        # 1. Known Aliases
        if q_lower in KNOWN_ALIASES:
            alias_target = KNOWN_ALIASES[q_lower]
            return {
                "name": q.title(),
                "target": alias_target,
                "type": "alias",
            }

        # Check if query matches alias key with normalization
        for alias_key, target in KNOWN_ALIASES.items():
            if normalize_string(alias_key) == q_norm:
                return {
                    "name": alias_key.title(),
                    "target": target,
                    "type": "alias",
                }

        # 2. Check direct URI scheme (e.g. ms-settings:, calc:)
        if ":" in q and not os.path.exists(q) and not (len(q) > 2 and q[1] == ":"):
            return {
                "name": q,
                "target": q,
                "type": "uri",
            }

        # 3. Check StartApps exact normalized match
        for name, app_id in self.start_apps.items():
            if normalize_string(name) == q_norm:
                return {
                    "name": name,
                    "target": f"shell:AppsFolder\\{app_id}",
                    "type": "start_app",
                }

        # 4. Check Registry Apps exact normalized match
        for clean_name, path in self.registry_apps.items():
            if normalize_string(clean_name) == q_norm:
                return {
                    "name": clean_name.title(),
                    "target": path,
                    "type": "registry",
                }

        # 5. Check Shortcut Apps exact normalized match
        for name, path in self.shortcut_apps.items():
            if normalize_string(name) == q_norm:
                return {
                    "name": name,
                    "target": path,
                    "type": "shortcut",
                }

        # 6. Check Substring / Word Match in StartApps
        for name, app_id in self.start_apps.items():
            name_norm = normalize_string(name)
            if q_norm in name_norm or name_norm in q_norm:
                return {
                    "name": name,
                    "target": f"shell:AppsFolder\\{app_id}",
                    "type": "start_app",
                }

        # 7. Check Substring in Registry Apps
        for clean_name, path in self.registry_apps.items():
            reg_norm = normalize_string(clean_name)
            if q_norm in reg_norm or reg_norm in q_norm:
                return {
                    "name": clean_name.title(),
                    "target": path,
                    "type": "registry",
                }

        # 8. Check Substring in Shortcuts
        for name, path in self.shortcut_apps.items():
            sc_norm = normalize_string(name)
            if q_norm in sc_norm or sc_norm in q_norm:
                return {
                    "name": name,
                    "target": path,
                    "type": "shortcut",
                }

        # 9. Fuzzy matching across StartApps
        candidates = list(self.start_apps.keys())
        matches = difflib.get_close_matches(q_lower, [c.lower() for c in candidates], n=1, cutoff=0.55)
        if matches:
            matched_name = next(c for c in candidates if c.lower() == matches[0])
            return {
                "name": matched_name,
                "target": f"shell:AppsFolder\\{self.start_apps[matched_name]}",
                "type": "start_app",
            }

        # 10. Check if executable exists in system PATH
        path_binary = shutil.which(q) or shutil.which(f"{q}.exe")
        if path_binary:
            return {
                "name": os.path.basename(path_binary),
                "target": path_binary,
                "type": "path",
            }

        # 11. Direct file path
        if os.path.exists(q):
            return {
                "name": os.path.basename(q),
                "target": os.path.abspath(q),
                "type": "path",
            }

        return None

    def launch(self, query: str, args: Optional[str] = None) -> Tuple[bool, str]:
        """
        Resolves and launches the requested application.
        Returns (success: bool, message: str).
        """
        resolution = self.resolve(query)
        if not resolution:
            return False, f"Could not find any application matching '{query}' on this system."

        target = resolution["target"]
        name = resolution["name"]

        try:
            # If arguments are provided and target is an executable or alias
            if args:
                if target.startswith("shell:AppsFolder\\") or target.endswith(":"):
                    # Launch via startfile then let OS handle URI
                    os.startfile(target)
                    return True, f"Launched '{name}' ({target}) with default parameters."
                else:
                    cmd = f'"{target}" {args}'
                    subprocess.Popen(cmd, shell=True)
                    return True, f"Launched '{name}' with arguments: {args}"

            # Standard launch
            if target.startswith("shell:AppsFolder\\") or target.endswith(":") or os.path.exists(target):
                os.startfile(target)
                return True, f"Successfully launched '{name}' ({target})."
            else:
                # Fallback to subprocess / os.system
                subprocess.Popen(target, shell=True)
                return True, f"Successfully launched '{name}' via command '{target}'."

        except Exception as e:
            return False, f"Failed to launch '{name}' ({target}): {str(e)}"

    def close(self, query: str) -> Tuple[bool, str]:
        """
        Finds and closes running processes matching the query.
        Returns (success: bool, message: str).
        """
        q_norm = normalize_string(query)
        closed_count = 0
        closed_names = []

        # Find matching processes
        for p in psutil.process_iter(attrs=["pid", "name"]):
            try:
                pname = p.info["name"] or ""
                pname_clean = normalize_string(pname.removesuffix(".exe"))
                if q_norm in pname_clean or pname_clean in q_norm:
                    p.terminate()
                    closed_count += 1
                    closed_names.append(f"{pname} (PID {p.info['pid']})")
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                continue

        if closed_count > 0:
            return True, f"Closed {closed_count} running process(es): {', '.join(closed_names)}"
        else:
            return False, f"No running processes found matching '{query}'."

    def list_apps(self, filter_text: str = "", limit: int = 50) -> List[Dict[str, str]]:
        """Returns a list of installed applications matching an optional filter."""
        results = []
        seen_names = set()

        q_norm = normalize_string(filter_text) if filter_text else ""

        # Collect from StartApps
        for name, app_id in self.start_apps.items():
            if not q_norm or q_norm in normalize_string(name):
                if name.lower() not in seen_names:
                    seen_names.add(name.lower())
                    results.append({"name": name, "type": "Start App", "target": app_id})

        # Collect from Registry
        for clean_name, path in self.registry_apps.items():
            if not q_norm or q_norm in normalize_string(clean_name):
                if clean_name.lower() not in seen_names:
                    seen_names.add(clean_name.lower())
                    results.append({"name": clean_name.title(), "type": "Win32 App", "target": path})

        # Collect from Shortcuts
        for name, path in self.shortcut_apps.items():
            if not q_norm or q_norm in normalize_string(name):
                if name.lower() not in seen_names:
                    seen_names.add(name.lower())
                    results.append({"name": name, "type": "Shortcut", "target": path})

        return results[:limit]

    def list_running_apps(self) -> List[Dict[str, Any]]:
        """Returns running processes that have typical application characteristics."""
        running = []
        for p in psutil.process_iter(attrs=["pid", "name", "memory_info", "create_time"]):
            try:
                name = p.info["name"]
                # Filter out obvious system background workers to keep list clean
                if name.lower() in ("system", "registry", "smss.exe", "csrss.exe", "wininit.exe", "services.exe", "lsass.exe", "svchost.exe", "fontdrvhost.exe", "dwm.exe"):
                    continue
                mem_mb = round(p.info["memory_info"].rss / (1024 * 1024), 1) if p.info.get("memory_info") else 0
                running.append({
                    "pid": p.info["pid"],
                    "name": name,
                    "memory_mb": mem_mb,
                })
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                continue

        # Sort by memory usage descending
        running.sort(key=lambda x: x["memory_mb"], reverse=True)
        return running[:30]
