"""
Qwen Desktop Agent
Integrates Ollama's qwen3:1.7b model with local Windows tools.
"""

import os
import sys
import json
import webbrowser
from typing import List, Dict, Any, Generator, Tuple
import ollama
from app_resolver import AppResolver

DEFAULT_MODEL = "qwen3:1.7b"

SYSTEM_PROMPT = """You are an intelligent Windows Desktop Assistant powered by Qwen.
You have the autonomous ability to control the host Windows system using your available tools.

When the user asks you to:
1. Open, start, launch, or run any application, tool, setting, or program (e.g. Notepad, Chrome, Calculator, VS Code, Task Manager, Settings, CodeBlocks, etc.), you MUST call the `open_application` tool.
2. Close, kill, or terminate an application, you MUST call the `close_application` tool.
3. Check or find what applications are installed, you MUST call the `list_installed_applications` tool.
4. Check what applications are currently running, you MUST call the `list_running_applications` tool.
5. Open any website or URL, you MUST call the `open_url` tool.

Guidelines:
- Always call the appropriate tool when an action is requested.
- If the user asks you to open multiple apps (e.g. "open notepad and calculator"), call the tool for each app.
- Provide a clear, polite, and concise confirmation response after executing the action.
"""


class QwenDesktopAgent:
    def __init__(self, model: str = DEFAULT_MODEL, host: str = "http://127.0.0.1:11434"):
        self.model = model
        self.host = host
        self.client = ollama.Client(host=self.host)
        self.resolver = AppResolver()
        self.messages: List[Dict[str, Any]] = [
            {"role": "system", "content": SYSTEM_PROMPT}
        ]
        self._init_tools()

    def _init_tools(self):
        """Prepares python functions for Ollama tool calling."""
        resolver = self.resolver

        def open_application(app_name: str, args: str = "") -> str:
            """Opens an application, program, or Windows tool on the system.
            Args:
                app_name: The name of the application to open (e.g. 'Notepad', 'Calculator', 'Chrome', 'Edge', 'VS Code', 'Settings', 'Excel').
                args: Optional arguments or file path to pass to the application.
            """
            success, msg = resolver.launch(app_name, args=args if args else None)
            return msg

        def close_application(app_name: str) -> str:
            """Closes or terminates a running application.
            Args:
                app_name: The name of the application to close (e.g. 'Notepad', 'Calculator', 'Chrome').
            """
            success, msg = resolver.close(app_name)
            return msg

        def list_installed_applications(search: str = "") -> str:
            """Searches and lists installed applications on the computer.
            Args:
                search: Optional search keyword to filter applications (e.g. 'code', 'player', 'office').
            """
            apps = resolver.list_apps(search, limit=25)
            if not apps:
                return f"No installed applications found matching '{search}'."
            lines = [f"- {a['name']} ({a['type']})" for a in apps]
            return f"Found {len(apps)} installed application(s):\n" + "\n".join(lines)

        def list_running_applications() -> str:
            """Lists active applications currently running on the computer."""
            running = resolver.list_running_apps()
            if not running:
                return "No user applications currently active."
            lines = [f"- {p['name']} (PID: {p['pid']}, RAM: {p['memory_mb']} MB)" for p in running[:15]]
            return "Currently running applications:\n" + "\n".join(lines)

        def open_url(url: str) -> str:
            """Opens a website or web link in the default browser.
            Args:
                url: The web URL to open (e.g. 'https://www.google.com' or 'youtube.com').
            """
            target = url.strip()
            if not (target.startswith("http://") or target.startswith("https://")):
                target = f"https://{target}"
            webbrowser.open(target)
            return f"Opened {target} in default web browser."

        self.tool_functions = {
            "open_application": open_application,
            "close_application": close_application,
            "list_installed_applications": list_installed_applications,
            "list_running_applications": list_running_applications,
            "open_url": open_url,
        }
        self.tools = list(self.tool_functions.values())

    def reset(self):
        """Clears conversation history while keeping system prompt."""
        self.messages = [{"role": "system", "content": SYSTEM_PROMPT}]

    def chat(self, user_input: str) -> Dict[str, Any]:
        """
        Sends a user prompt to Qwen, executes any tool calls,
        and returns the final response and tool execution details.
        """
        self.messages.append({"role": "user", "content": user_input})
        executed_tools = []

        # Send initial request to Ollama with tools
        response = self.client.chat(
            model=self.model,
            messages=self.messages,
            tools=self.tools,
        )

        message = response.message
        self.messages.append(message)

        # Process tool calls if any
        if message.tool_calls:
            for tool_call in message.tool_calls:
                func_name = tool_call.function.name
                func_args = tool_call.function.arguments or {}
                
                tool_fn = self.tool_functions.get(func_name)
                if tool_fn:
                    try:
                        tool_result = tool_fn(**func_args)
                    except Exception as ex:
                        tool_result = f"Error executing {func_name}: {str(ex)}"
                else:
                    tool_result = f"Error: Tool '{func_name}' is not recognized."

                executed_tools.append({
                    "name": func_name,
                    "arguments": func_args,
                    "result": tool_result,
                })

                # Append tool result to history
                self.messages.append({
                    "role": "tool",
                    "content": tool_result,
                })

            # Get final response from Qwen incorporating tool results
            final_response = self.client.chat(
                model=self.model,
                messages=self.messages,
            )
            final_message = final_response.message
            self.messages.append(final_message)

            return {
                "response": final_message.content,
                "tool_calls": executed_tools,
                "thinking": getattr(message, "thinking", None),
            }

        return {
            "response": message.content,
            "tool_calls": [],
            "thinking": getattr(message, "thinking", None),
        }
