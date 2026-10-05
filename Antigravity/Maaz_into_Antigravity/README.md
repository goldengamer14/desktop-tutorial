# Qwen Windows Desktop Controller

Autonomous system controller empowering your local **`qwen3:1.7b`** model (via Ollama) to discover, launch, and manage any Windows application on your machine using natural language.

---

## Features

- **Universal Application Resolution**:
  - **UWP / Microsoft Store Apps**: Discovers modern Windows apps (Calculator, Camera, Clock, Photos, Settings, etc.) via `shell:AppsFolder`.
  - **Win32 & Desktop Programs**: Scans Windows Registry `App Paths`, Start Menu shortcuts (`.lnk`), and system PATH (Chrome, Edge, VS Code, Office, 7-Zip, CodeBlocks, etc.).
  - **Fuzzy & Substring Matching**: Resolves variations like "7zip", "codeblocks", "calc", "visual studio code".
- **Ollama Native Tool Calling**:
  - Connects directly to local Ollama (`http://127.0.0.1:11434`) using model `qwen3:1.7b`.
  - Qwen automatically invokes tools:
    - `open_application(app_name, args)`
    - `close_application(app_name)`
    - `list_installed_applications(search)`
    - `list_running_applications()`
    - `open_url(url)`
- **Two Flexible Interfaces**:
  1. **Interactive CLI Assistant** (`cli_assistant.py` or double-click `start_cli.bat`):
     - Fast, terminal-based REPL with colorized status badges and instant feedback.
     - Single-command execution support (e.g. `python cli_assistant.py "open notepad"`).
  2. **Glassmorphic Web Dashboard** (`web_server.py` or double-click `start_web.bat`):
     - Real-time chat interface with model reasoning/thinking visualization.
     - Quick Launch Dock for popular applications.
     - Searchable browser of all 110+ detected apps with 1-click launch buttons.
     - Active process monitor with RAM usage and 1-click termination.

---

## Quick Start

### 1. Interactive Terminal Assistant
Double click `start_cli.bat` or run:
```bash
python cli_assistant.py
```

Try commands like:
- `"Open Notepad and Calculator"`
- `"Launch Google Chrome and search for machine learning"`
- `"Open Visual Studio Code"`
- `"What apps do I have for coding?"`
- `"Close Calculator"`
- `"Open Windows Settings"`

### 2. Web Dashboard
Double click `start_web.bat` or run:
```bash
python web_server.py
```
Then visit: `http://127.0.0.1:8088` in your browser.

### 3. One-Shot Command Execution
```bash
python cli_assistant.py "open calculator"
python cli_assistant.py "close calculator"
```

---

## Architecture

```
User Prompt (Natural Language)
               │
               ▼
   Ollama (qwen3:1.7b)
               │ (Tool Call: open_application)
               ▼
    QwenDesktopAgent (Python)
               │
               ▼
       AppResolver
    ┌──────────┴──────────────────────────┐
    ▼                                     ▼
Registry / Start Menu / UWP          Windows Shell Execution
(110+ applications detected)         (os.startfile / subprocess)
```
