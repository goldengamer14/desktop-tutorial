import json
import requests


# ============================================================
# CONFIG
# ============================================================

OLLAMA_URL = "http://localhost:11434/api/chat"
MODEL = "qwen3:8b"


# ============================================================
# PLAN SCHEMA
# ============================================================

PLAN_SCHEMA = {
    "type": "object",
    "properties": {
        "goal": {
            "type": "string"
        },
        "approach": {
            "type": "string"
        },
        "steps": {
            "type": "array",
            "items": {
                "type": "object",
                "properties": {
                    "step": {
                        "type": "integer"
                    },
                    "task": {
                        "type": "string"
                    },
                    "files": {
                        "type": "array",
                        "items": {
                            "type": "string"
                        }
                    }
                },
                "required": ["step", "task", "files"]
            }
        },
        "verification": {
            "type": "array",
            "items": {
                "type": "string"
            }
        }
    },
    "required": [
        "goal",
        "approach",
        "steps",
        "verification"
    ]
}


# ============================================================
# SYSTEM PROMPT
# ============================================================

SYSTEM_PROMPT = """
You are a planning agent for a simple browser game coding agent.

Your job is NOT to write code.

Your job is to create a short, practical implementation plan
for the coding agent.

The coding agent can ONLY work with these files:

- index.html
- style.css
- game.js

Create a plan that is:
- simple
- realistic
- executable by another coding agent
- focused on the user's request
- no unnecessary technologies
- no unnecessary files

The plan should contain:
1. The main goal
2. A short implementation approach
3. Ordered implementation steps
4. Which game files each step affects
5. How the result should be verified

Never write actual source code.
"""


# ============================================================
# PLAN GENERATOR
# ============================================================

def create_plan(user_request, feedback=None):

    prompt = f"""
User request:

{user_request}
"""

    if feedback:
        prompt += f"""

The human reviewed the previous plan and gave this feedback:

{feedback}

Regenerate the plan according to this feedback.
"""

    payload = {
        "model": MODEL,
        "messages": [
            {
                "role": "system",
                "content": SYSTEM_PROMPT
            },
            {
                "role": "user",
                "content": prompt
            }
        ],
        "stream": False,
        "think": False,
        "format": PLAN_SCHEMA,
        "options": {
            "temperature": 0.2
        }
    }

    try:
        response = requests.post(
            OLLAMA_URL,
            json=payload,
            timeout=300
        )

        response.raise_for_status()

    except requests.exceptions.ConnectionError:
        raise RuntimeError(
            "Could not connect to Ollama. "
            "Make sure Ollama is running."
        )

    except requests.exceptions.Timeout:
        raise RuntimeError(
            "Ollama took too long to respond."
        )

    except requests.exceptions.RequestException as e:
        raise RuntimeError(f"Ollama request failed: {e}")

    data = response.json()

    content = data["message"]["content"]

    try:
        plan = json.loads(content)
    except json.JSONDecodeError:
        raise RuntimeError(
            "Qwen returned invalid JSON.\n\n"
            f"Raw response:\n{content}"
        )

    return plan


# ============================================================
# DISPLAY PLAN
# ============================================================

def print_plan(plan):

    print("\n")
    print("=" * 70)
    print("🧠 GENERATED PLAN")
    print("=" * 70)

    print("\nGOAL")
    print(plan["goal"])

    print("\nAPPROACH")
    print(plan["approach"])

    print("\nSTEPS")

    for step in plan["steps"]:
        print(
            f"\n{step['step']}. {step['task']}"
        )

        if step["files"]:
            print(
                "   Files: "
                + ", ".join(step["files"])
            )

    print("\nVERIFICATION")

    for item in plan["verification"]:
        print(f"✓ {item}")

    print("\n" + "=" * 70)


# ============================================================
# TEST
# ============================================================

if __name__ == "__main__":

    request = input(
        "\nWhat do you want to build?\n> "
    )

    plan = create_plan(request)

    print_plan(plan)