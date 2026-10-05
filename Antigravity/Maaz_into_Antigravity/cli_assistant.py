"""
Interactive CLI Assistant for Qwen Desktop Controller
Allows controlling Windows applications via natural language terminal interface.
"""

import sys
import os

# Fix Windows console UTF-8 encoding
if sys.platform == "win32":
    try:
        sys.stdout.reconfigure(encoding="utf-8")
        sys.stderr.reconfigure(encoding="utf-8")
        sys.stdin.reconfigure(encoding="utf-8")
        # Enable ANSI color sequences on Windows
        os.system("")
    except Exception:
        pass

from qwen_agent import QwenDesktopAgent

# Terminal ANSI Color codes
CYAN = "\033[96m"
GREEN = "\033[92m"
YELLOW = "\033[93m"
MAGENTA = "\033[95m"
BLUE = "\033[94m"
RED = "\033[91m"
BOLD = "\033[1m"
DIM = "\033[2m"
RESET = "\033[0m"


def print_banner():
    banner = f"""
{CYAN}{BOLD}========================================================================{RESET}
{MAGENTA}{BOLD}   🤖 Qwen Windows Desktop Assistant (Model: qwen3:1.7b via Ollama){RESET}
{CYAN}{BOLD}========================================================================{RESET}
{DIM}Controls any application or component on your system using natural language.{RESET}
{YELLOW}Examples:{RESET}
  • "Open Notepad and Calculator"
  • "Launch Chrome and search for local news"
  • "Open Visual Studio Code"
  • "What applications do I have installed for programming?"
  • "Close Calculator"
  • "Open Windows Settings"

{DIM}Special commands: /apps [filter], /running, /clear, /help, /exit{RESET}
{CYAN}------------------------------------------------------------------------{RESET}
"""
    print(banner)


def handle_special_command(cmd: str, agent: QwenDesktopAgent) -> bool:
    parts = cmd.strip().split(maxsplit=1)
    keyword = parts[0].lower()
    arg = parts[1] if len(parts) > 1 else ""

    if keyword in ("/exit", "/quit", "exit", "quit"):
        print(f"\n{YELLOW}Goodbye! Exiting Qwen Assistant.{RESET}")
        sys.exit(0)
    elif keyword in ("/help", "/h"):
        print(f"\n{BOLD}Available Commands:{RESET}")
        print(f"  {CYAN}/apps [filter]{RESET}   - Search and list installed applications")
        print(f"  {CYAN}/running{RESET}         - Show currently active processes")
        print(f"  {CYAN}/open <name>{RESET}     - Directly launch an app by name")
        print(f"  {CYAN}/close <name>{RESET}    - Directly close an app by name")
        print(f"  {CYAN}/clear{RESET}           - Clear screen and reset conversation context")
        print(f"  {CYAN}/exit{RESET}            - Exit assistant")
        print(f"  Or simply type natural English prompts!\n")
        return True
    elif keyword == "/apps":
        print(f"\n{DIM}Searching installed applications...{RESET}")
        apps = agent.resolver.list_apps(arg, limit=30)
        print(f"{GREEN}Installed applications ({len(apps)} found):{RESET}")
        for a in apps:
            print(f"  • {BOLD}{a['name']}{RESET} {DIM}({a['type']}){RESET}")
        print()
        return True
    elif keyword == "/running":
        print(f"\n{DIM}Fetching running processes...{RESET}")
        running = agent.resolver.list_running_apps()
        print(f"{GREEN}Active Applications:{RESET}")
        for p in running[:15]:
            print(f"  • {BOLD}{p['name']}{RESET} (PID: {p['pid']}, RAM: {p['memory_mb']} MB)")
        print()
        return True
    elif keyword == "/open":
        if not arg:
            print(f"{RED}Please specify an application name to open.{RESET}")
        else:
            success, msg = agent.resolver.launch(arg)
            print(f"{GREEN if success else RED}{msg}{RESET}")
        return True
    elif keyword == "/close":
        if not arg:
            print(f"{RED}Please specify an application name to close.{RESET}")
        else:
            success, msg = agent.resolver.close(arg)
            print(f"{GREEN if success else RED}{msg}{RESET}")
        return True
    elif keyword in ("/clear", "/cls"):
        os.system("cls" if os.name == "nt" else "clear")
        agent.reset()
        print_banner()
        print(f"{GREEN}Conversation context reset.{RESET}\n")
        return True
    return False


def run_prompt(agent: QwenDesktopAgent, user_prompt: str):
    print(f"\n{DIM}Qwen is thinking and processing...{RESET}")
    try:
        res = agent.chat(user_prompt)

        # Print executed tool calls
        if res.get("tool_calls"):
            for tc in res["tool_calls"]:
                tool_name = tc["name"]
                args = tc["arguments"]
                result = tc["result"]
                args_str = ", ".join(f"{k}='{v}'" for k, v in args.items())
                print(f"{YELLOW}⚡ Tool Executed:{RESET} {CYAN}{tool_name}({args_str}){RESET}")
                print(f"   {GREEN}↳ {result}{RESET}")

        # Print response
        response_text = res.get("response", "").strip()
        if response_text:
            print(f"\n{BOLD}🤖 Qwen:{RESET} {response_text}\n")
        elif not res.get("tool_calls"):
            print(f"\n{BOLD}🤖 Qwen:{RESET} (No output)\n")

    except Exception as e:
        print(f"\n{RED}Error communicating with Qwen/Ollama: {str(e)}{RESET}\n")


def main():
    agent = QwenDesktopAgent()

    # If prompt given via CLI argument: e.g. python cli_assistant.py "open notepad"
    if len(sys.argv) > 1:
        prompt = " ".join(sys.argv[1:])
        print(f"{CYAN}Processing command:{RESET} {prompt}")
        run_prompt(agent, prompt)
        return

    print_banner()

    while True:
        try:
            user_input = input(f"{BOLD}{GREEN}You > {RESET}").strip()
            if not user_input:
                continue

            if handle_special_command(user_input, agent):
                continue

            run_prompt(agent, user_input)

        except (KeyboardInterrupt, EOFError):
            print(f"\n{YELLOW}Session ended. Goodbye!{RESET}")
            break


if __name__ == "__main__":
    main()
