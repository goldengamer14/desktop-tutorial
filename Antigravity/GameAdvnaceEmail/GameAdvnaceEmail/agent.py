import os
import sys
import json
import asyncio
import threading
import http.server
import socketserver

from dotenv import load_dotenv
from groq import Groq
from playwright.sync_api import sync_playwright

from mcp import ClientSession, StdioServerParameters
from mcp.client.stdio import stdio_client


# ============================================================
# CONFIG
# ============================================================

load_dotenv()

client = Groq(
    api_key=os.getenv("GROQ_API_KEY")
)

MODEL = "openai/gpt-oss-120b"

GAME_DIR = "game"

PORT = 8000

ALLOWED_FILES = {
    "index.html",
    "style.css",
    "game.js"
}

os.makedirs(
    GAME_DIR,
    exist_ok=True
)


# ============================================================
# HELPERS
# ============================================================

def valid_file(filename):

    return filename in ALLOWED_FILES


# ============================================================
# FILE TOOLS
# ============================================================

def create_file(filename, content):

    if not valid_file(filename):

        return {
            "error":
            "Only index.html, style.css and game.js are allowed"
        }

    path = os.path.join(
        GAME_DIR,
        filename
    )

    with open(
        path,
        "w",
        encoding="utf-8"
    ) as f:

        f.write(content)

    return {
        "success": True,
        "message": f"{filename} created"
    }


def read_file(filename):

    if not valid_file(filename):

        return {
            "error": "Invalid file"
        }

    path = os.path.join(
        GAME_DIR,
        filename
    )

    if not os.path.exists(path):

        return {
            "error":
            f"{filename} does not exist"
        }

    with open(
        path,
        "r",
        encoding="utf-8"
    ) as f:

        return {
            "success": True,
            "content": f.read()
        }


def edit_file(
    filename,
    old_text,
    new_text
):

    if not valid_file(filename):

        return {
            "error": "Invalid file"
        }

    path = os.path.join(
        GAME_DIR,
        filename
    )

    if not os.path.exists(path):

        return {
            "error":
            f"{filename} does not exist"
        }

    with open(
        path,
        "r",
        encoding="utf-8"
    ) as f:

        content = f.read()

    if old_text not in content:

        return {
            "error":
            "The text to replace was not found"
        }

    content = content.replace(
        old_text,
        new_text,
        1
    )

    with open(
        path,
        "w",
        encoding="utf-8"
    ) as f:

        f.write(content)

    return {
        "success": True,
        "message": f"{filename} edited"
    }


def delete_file(filename):

    if not valid_file(filename):

        return {
            "error": "Invalid file"
        }

    path = os.path.join(
        GAME_DIR,
        filename
    )

    if os.path.exists(path):

        os.remove(path)

    return {
        "success": True,
        "message": f"{filename} deleted"
    }


# ============================================================
# BROWSER SERVER
# ============================================================

server = None


def start_server():

    global server

    handler = lambda *args, **kwargs: (
        http.server.SimpleHTTPRequestHandler(
            *args,
            directory=".",
            **kwargs
        )
    )

    server = socketserver.TCPServer(
        (
            "127.0.0.1",
            PORT
        ),
        handler
    )

    thread = threading.Thread(
        target=server.serve_forever,
        daemon=True
    )

    thread.start()


# ============================================================
# RUN GAME
# ============================================================

last_errors = []


def run_game():

    global last_errors

    errors = []

    with sync_playwright() as p:

        browser = p.chromium.launch(
            headless=True
        )

        page = browser.new_page()

        # ----------------------------------------------------
        # JavaScript runtime errors
        # ----------------------------------------------------

        page.on(
            "pageerror",
            lambda error:
            errors.append(
                f"JavaScript error: {error}"
            )
        )

        # ----------------------------------------------------
        # Console errors
        # ----------------------------------------------------

        def console_handler(msg):

            if msg.type == "error":

                errors.append(
                    f"Console error: {msg.text}"
                )

        page.on(
            "console",
            console_handler
        )

        try:

            page.goto(
                f"http://127.0.0.1:{PORT}/game/index.html",
                wait_until="networkidle",
                timeout=10000
            )

            page.wait_for_timeout(
                1000
            )

        except Exception as e:

            errors.append(
                f"Browser error: {e}"
            )

        browser.close()

    last_errors = errors

    return {
        "success": len(errors) == 0,
        "errors": errors
    }


def get_errors():

    return {
        "success": len(last_errors) == 0,
        "errors": last_errors
    }


# ============================================================
# MCP EMAIL
# ============================================================

async def send_game_email(
    recipient_email,
    description,
    subject="Your Browser Game — Game Development Agent"
):
    """
    Start email_mcp.py as an MCP stdio server and
    call the send_game_package MCP tool.
    """

    server_params = StdioServerParameters(
        command=sys.executable,
        args=[
            os.path.join(
                os.path.dirname(
                    os.path.abspath(__file__)
                ),
                "email_mcp.py"
            )
        ],
    )

    async with stdio_client(
        server_params
    ) as (
        read_stream,
        write_stream
    ):

        async with ClientSession(
            read_stream,
            write_stream
        ) as session:

            await session.initialize()

            result = await session.call_tool(
                "send_game_package",
                {
                    "recipient_email":
                        recipient_email,

                    "description":
                        description,

                    "subject":
                        subject,
                }
            )

            return result


def send_game_email_sync(
    recipient_email,
    description,
    subject="Your Browser Game — Game Development Agent"
):
    """
    Synchronous wrapper used by the Groq tool executor.
    """

    try:

        result = asyncio.run(
            send_game_email(
                recipient_email=recipient_email,
                description=description,
                subject=subject
            )
        )

        return {
            "success": True,
            "mcp_result": str(result)
        }

    except Exception as e:

        return {
            "success": False,
            "error": str(e)
        }


# ============================================================
# TOOL DEFINITIONS FOR GROQ
# ============================================================

TOOLS = [

    # --------------------------------------------------------
    # CREATE FILE
    # --------------------------------------------------------

    {
        "type": "function",

        "function": {

            "name": "create_file",

            "description":
                "Create or completely overwrite one allowed game file.",

            "parameters": {

                "type": "object",

                "properties": {

                    "filename": {

                        "type": "string",

                        "enum": [
                            "index.html",
                            "style.css",
                            "game.js"
                        ]
                    },

                    "content": {

                        "type": "string"
                    }
                },

                "required": [
                    "filename",
                    "content"
                ]
            }
        }
    },


    # --------------------------------------------------------
    # READ FILE
    # --------------------------------------------------------

    {
        "type": "function",

        "function": {

            "name": "read_file",

            "description":
                "Read one allowed game file.",

            "parameters": {

                "type": "object",

                "properties": {

                    "filename": {

                        "type": "string",

                        "enum": [
                            "index.html",
                            "style.css",
                            "game.js"
                        ]
                    }
                },

                "required": [
                    "filename"
                ]
            }
        }
    },


    # --------------------------------------------------------
    # EDIT FILE
    # --------------------------------------------------------

    {
        "type": "function",

        "function": {

            "name": "edit_file",

            "description":
                "Replace one exact piece of text inside a game file.",

            "parameters": {

                "type": "object",

                "properties": {

                    "filename": {

                        "type": "string",

                        "enum": [
                            "index.html",
                            "style.css",
                            "game.js"
                        ]
                    },

                    "old_text": {

                        "type": "string"
                    },

                    "new_text": {

                        "type": "string"
                    }
                },

                "required": [
                    "filename",
                    "old_text",
                    "new_text"
                ]
            }
        }
    },


    # --------------------------------------------------------
    # DELETE FILE
    # --------------------------------------------------------

    {
        "type": "function",

        "function": {

            "name": "delete_file",

            "description":
                "Delete one allowed game file.",

            "parameters": {

                "type": "object",

                "properties": {

                    "filename": {

                        "type": "string",

                        "enum": [
                            "index.html",
                            "style.css",
                            "game.js"
                        ]
                    }
                },

                "required": [
                    "filename"
                ]
            }
        }
    },


    # --------------------------------------------------------
    # RUN GAME
    # --------------------------------------------------------

    {
        "type": "function",

        "function": {

            "name": "run_game",

            "description":
                "Open the game in a headless browser and detect runtime/browser errors.",

            "parameters": {

                "type": "object",

                "properties": {}
            }
        }
    },


    # --------------------------------------------------------
    # GET ERRORS
    # --------------------------------------------------------

    {
        "type": "function",

        "function": {

            "name": "get_errors",

            "description":
                "Return errors from the most recent game run.",

            "parameters": {

                "type": "object",

                "properties": {}
            }
        }
    },


    # --------------------------------------------------------
    # SEND GAME PACKAGE
    # --------------------------------------------------------

    {
        "type": "function",

        "function": {

            "name": "send_game_package",

            "description": """
Email the completed browser game to the email address
specified by the user.

ONLY call this after the game has been successfully
created and tested.

The MCP server will create a ZIP containing ONLY:

game/index.html
game/style.css
game/game.js

The MCP server will then email the ZIP.

The recipient email MUST come from the user's request.

Never invent an email address.

Do not call this tool before testing the game.
""",

            "parameters": {

                "type": "object",

                "properties": {

                    "recipient_email": {

                        "type": "string",

                        "description":
                            "The email address explicitly provided by the user."
                    },

                    "description": {

                        "type": "string",

                        "description":
                            "A concise description of the completed game."
                    },

                    "subject": {

                        "type": "string",

                        "description":
                            "Optional email subject."
                    }
                },

                "required": [
                    "recipient_email",
                    "description"
                ]
            }
        }
    }

]


# ============================================================
# TOOL EXECUTOR
# ============================================================

def execute_tool(
    name,
    args
):

    if name == "create_file":

        return create_file(
            args["filename"],
            args["content"]
        )


    if name == "read_file":

        return read_file(
            args["filename"]
        )


    if name == "edit_file":

        return edit_file(
            args["filename"],
            args["old_text"],
            args["new_text"]
        )


    if name == "delete_file":

        return delete_file(
            args["filename"]
        )


    if name == "run_game":

        return run_game()


    if name == "get_errors":

        return get_errors()


    if name == "send_game_package":

        return send_game_email_sync(
            recipient_email=
                args["recipient_email"],

            description=
                args["description"],

            subject=
                args.get(
                    "subject",
                    "Your Browser Game — Game Development Agent"
                )
        )


    return {
        "error":
        f"Unknown tool: {name}"
    }


# ============================================================
# AGENT PROMPT
# ============================================================

SYSTEM_PROMPT = """

You are an autonomous browser-game coding agent.

Your job is to create the game requested by the user.

You can ONLY use these three game files:

- index.html
- style.css
- game.js

You have tools for:

- creating files
- reading files
- editing files
- deleting files
- running the game
- checking errors
- emailing the completed game through MCP


WORKFLOW:

1. Understand the user's game request.

2. If the user requested an email, identify the
   recipient email address from the user's request.

3. Create the required game files.

4. Run the game.

5. Inspect the errors.

6. Read the relevant file.

7. Edit the file to fix the problem.

8. Run the game again.

9. Repeat until the game works.

10. ONLY AFTER the game has been successfully tested,
    call send_game_package if the user requested email.

11. Give the MCP tool the exact email address requested
    by the user.

12. Give the MCP tool a concise description of the
    game that was actually built.


EMAIL RULES:

- The recipient email must come from the user's request.
- Never invent an email address.
- Never use an email address from .env.
- SMTP credentials are handled by email_mcp.py.
- Do not ask for SMTP credentials.
- Do not manually create the ZIP.
- The MCP server creates the ZIP.
- The ZIP contains ONLY:

  game/index.html
  game/style.css
  game/game.js

- Do not call send_game_package before successful testing.


IMPORTANT:

- Do not create any other game files.
- Do not use markdown as a response when a tool is needed.
- Always use tools for file operations.
- Test the game before finishing.
- Keep the game simple enough to run in a browser.
"""


# ============================================================
# AGENT LOOP
# ============================================================

def run_agent(user_request):

    messages = [

        {
            "role": "system",
            "content": SYSTEM_PROMPT
        },

        {
            "role": "user",
            "content": user_request
        }

    ]


    for step in range(25):

        print(
            "\n" + "=" * 60
        )

        print(
            f"AGENT STEP {step + 1}"
        )

        print(
            "=" * 60
        )


        response = client.chat.completions.create(

            model=MODEL,

            messages=messages,

            tools=TOOLS,

            tool_choice="auto",

            parallel_tool_calls=False,

            temperature=0.2,

            max_completion_tokens=4000
        )


        message = response.choices[0].message


        # ----------------------------------------------------
        # NO TOOL CALL
        # ----------------------------------------------------

        if not message.tool_calls:

            print("\nFINAL:")

            print(
                message.content or "Done."
            )

            break


        # ----------------------------------------------------
        # ADD ASSISTANT MESSAGE
        # ----------------------------------------------------

        messages.append({

            "role": "assistant",

            "content":
                message.content or "",

            "tool_calls": [

                {

                    "id": tc.id,

                    "type": "function",

                    "function": {

                        "name":
                            tc.function.name,

                        "arguments":
                            tc.function.arguments
                    }
                }

                for tc in message.tool_calls

            ]
        })


        # ----------------------------------------------------
        # EXECUTE TOOLS
        # ----------------------------------------------------

        for tc in message.tool_calls:

            name = tc.function.name


            try:

                args = json.loads(
                    tc.function.arguments
                )

            except json.JSONDecodeError:

                result = {
                    "error":
                    "Invalid tool arguments"
                }

                messages.append({

                    "role": "tool",

                    "tool_call_id":
                        tc.id,

                    "name":
                        name,

                    "content":
                        json.dumps(result)
                })

                continue


            print(
                f"\n🔧 TOOL: {name}"
            )

            print(
                "ARGS:",
                args
            )


            result = execute_tool(
                name,
                args
            )


            print(
                "RESULT:",
                result
            )


            messages.append({

                "role": "tool",

                "tool_call_id":
                    tc.id,

                "name":
                    name,

                "content":
                    json.dumps(result)
            })


# ============================================================
# MAIN
# ============================================================

if __name__ == "__main__":

    if not os.getenv(
        "GROQ_API_KEY"
    ):

        raise RuntimeError(
            "GROQ_API_KEY missing. "
            "Put it in .env"
        )


    start_server()


    print(
        "\n🎮 GAME CODING AGENT"
    )

    print(
        "===================="
    )


    request = input(
        "\nWhat game should I create?\n> "
    )


    run_agent(
        request
    )