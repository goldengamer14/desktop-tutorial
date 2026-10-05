"""
Web Server for Qwen Desktop Controller
Lightweight, zero-external-dependency HTTP server with REST API & Web Dashboard.
"""

import os
import sys
import json
import mimetypes
import webbrowser
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs

# Fix Windows console UTF-8 encoding
if sys.platform == "win32":
    try:
        sys.stdout.reconfigure(encoding="utf-8")
        sys.stderr.reconfigure(encoding="utf-8")
    except Exception:
        pass

from qwen_agent import QwenDesktopAgent

STATIC_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "static")
PORT = 8088

agent = QwenDesktopAgent()


class QwenRequestHandler(BaseHTTPRequestHandler):
    def send_json(self, data: dict, status: int = 200):
        body = json.dumps(data).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(body)

    def do_OPTIONS(self):
        self.send_response(200)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()

    def do_GET(self):
        parsed = urlparse(self.path)
        path = parsed.path

        # REST API endpoints
        if path == "/api/status":
            self.send_json({
                "status": "ok",
                "model": agent.model,
                "total_apps": len(agent.resolver.start_apps) + len(agent.resolver.registry_apps),
            })
            return

        elif path == "/api/apps":
            query_params = parse_qs(parsed.query)
            filter_text = query_params.get("q", [""])[0]
            apps = agent.resolver.list_apps(filter_text=filter_text, limit=120)
            self.send_json({"apps": apps})
            return

        elif path == "/api/running":
            procs = agent.resolver.list_running_apps()
            self.send_json({"running": procs})
            return

        # Static file serving
        if path in ("/", "/index.html"):
            file_path = os.path.join(STATIC_DIR, "index.html")
        elif path.startswith("/static/"):
            rel_path = path.removeprefix("/static/").replace("/", os.sep)
            file_path = os.path.join(STATIC_DIR, rel_path)
        else:
            file_path = os.path.join(STATIC_DIR, path.lstrip("/").replace("/", os.sep))

        if os.path.isfile(file_path):
            ctype, _ = mimetypes.guess_type(file_path)
            if not ctype:
                ctype = "application/octet-stream"
            try:
                with open(file_path, "rb") as f:
                    content = f.read()
                self.send_response(200)
                self.send_header("Content-Type", ctype)
                self.send_header("Content-Length", str(len(content)))
                self.end_headers()
                self.wfile.write(content)
            except Exception as e:
                self.send_error(500, f"Error reading file: {e}")
        else:
            self.send_error(404, "File Not Found")

    def do_POST(self):
        parsed = urlparse(self.path)
        path = parsed.path
        content_len = int(self.headers.get("Content-Length", 0))
        post_data = self.rfile.read(content_len)

        try:
            body = json.loads(post_data.decode("utf-8")) if post_data else {}
        except Exception:
            body = {}

        if path == "/api/chat":
            prompt = body.get("prompt", "").strip()
            if not prompt:
                self.send_json({"error": "Empty prompt provided."}, status=400)
                return

            try:
                result = agent.chat(prompt)
                self.send_json(result)
            except Exception as e:
                self.send_json({"error": str(e)}, status=500)
            return

        elif path == "/api/launch":
            app_name = body.get("app_name", "").strip()
            args = body.get("args")
            success, msg = agent.resolver.launch(app_name, args=args)
            self.send_json({"success": success, "message": msg})
            return

        elif path == "/api/close":
            app_name = body.get("app_name", "").strip()
            success, msg = agent.resolver.close(app_name)
            self.send_json({"success": success, "message": msg})
            return

        elif path == "/api/clear":
            agent.reset()
            self.send_json({"success": True})
            return

        self.send_error(404, "Endpoint not found")

    def log_message(self, format, *args):
        # Concise logging
        print(f"[WebUI] {self.address_string()} - {format % args}")


def start_server(port=PORT, open_browser=True):
    server = HTTPServer(("127.0.0.1", port), QwenRequestHandler)
    url = f"http://127.0.0.1:{port}"
    print(f"\n=======================================================")
    print(f"🚀 Qwen Desktop Controller Web UI running at:")
    print(f"   {url}")
    print(f"   Model: {agent.model} (via Ollama)")
    print(f"=======================================================\n")
    if open_browser:
        webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopping web server...")
        server.server_close()


if __name__ == "__main__":
    open_in_browser = "--no-browser" not in sys.argv
    start_server(open_browser=open_in_browser)
