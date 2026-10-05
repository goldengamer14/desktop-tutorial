import os
import smtplib
from email.message import EmailMessage

from dotenv import load_dotenv
from mcp.server.mcpserver import MCPServer


# ============================================================
# CONFIG
# ============================================================

load_dotenv()


mcp = MCPServer(
    "game-email-mcp",
)


# ============================================================
# SEND EMAIL
# ============================================================

def send_email(
    recipient_email: str,
    description: str,
    subject: str,
):
    """
    Send a game completion email.

    No ZIP or files are attached.
    """

    # --------------------------------------------------------
    # SMTP CONFIG
    # --------------------------------------------------------

    smtp_host = os.getenv(
        "SMTP_HOST",
        "smtp.gmail.com",
    )

    smtp_port = int(
        os.getenv(
            "SMTP_PORT",
            "465",
        )
    )

    smtp_username = os.getenv(
        "SMTP_USERNAME"
    )

    smtp_password = os.getenv(
        "SMTP_PASSWORD"
    )

    from_name = os.getenv(
        "EMAIL_FROM_NAME",
        "Game Development Agent",
    )

    # --------------------------------------------------------
    # VALIDATION
    # --------------------------------------------------------

    if not smtp_username:
        raise RuntimeError(
            "Missing SMTP_USERNAME in .env"
        )

    if not smtp_password:
        raise RuntimeError(
            "Missing SMTP_PASSWORD in .env"
        )

    if not recipient_email:
        raise RuntimeError(
            "Missing recipient_email"
        )

    # --------------------------------------------------------
    # CREATE EMAIL
    # --------------------------------------------------------

    message = EmailMessage()

    message["From"] = (
        f"{from_name} <{smtp_username}>"
    )

    message["To"] = recipient_email

    message["Subject"] = subject

    message.set_content(
        f"""
🎮 YOUR GAME IS READY!

The Game Development Agent has successfully
created and tested your browser game.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

GAME DESCRIPTION
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

{description}

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

GAME FILES CREATED
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✓ index.html
✓ style.css
✓ game.js

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TECHNOLOGY
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

HTML
CSS
JavaScript

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

STATUS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✓ Game generated
✓ Game tested
✓ Game completed successfully

The game is ready to run locally.

Open the `index.html` file in a browser
to play the game.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Built autonomously by the
Game Development Agent 🤖

        """.strip()
    )

    # --------------------------------------------------------
    # SEND
    # --------------------------------------------------------

    if smtp_port == 465:

        with smtplib.SMTP_SSL(
            smtp_host,
            smtp_port,
            timeout=30,
        ) as smtp:

            smtp.login(
                smtp_username,
                smtp_password,
            )

            smtp.send_message(
                message
            )

    else:

        with smtplib.SMTP(
            smtp_host,
            smtp_port,
            timeout=30,
        ) as smtp:

            smtp.ehlo()

            smtp.starttls()

            smtp.ehlo()

            smtp.login(
                smtp_username,
                smtp_password,
            )

            smtp.send_message(
                message
            )


# ============================================================
# MCP TOOL
# ============================================================

@mcp.tool()
def send_game_package(
    recipient_email: str,
    description: str,
    subject: str = "🎮 Your Browser Game Is Ready",
) -> dict:
    """
    Send the completed game information by email.

    NO ZIP FILE IS CREATED.
    NO FILES ARE ATTACHED.

    The email contains:
    - Game description
    - Files created
    - Technology used
    - Completion status
    - Instructions to run the game
    """

    try:

        send_email(
            recipient_email=recipient_email,
            description=description,
            subject=subject,
        )

        return {
            "success": True,
            "message": (
                "Game completion email sent successfully."
            ),
            "recipient": recipient_email,
            "attachment": None,
        }

    except Exception as error:

        return {
            "success": False,
            "error": str(error),
        }


# ============================================================
# START MCP SERVER
# ============================================================

if __name__ == "__main__":

    mcp.run()