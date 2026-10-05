import json

from planning_agent import create_plan, print_plan
from agent import run_agent, start_server


# ============================================================
# ORCHESTRATOR
# ============================================================

def orchestrate(user_request):

    print("\n")
    print("=" * 70)
    print("🤖 GAME DEVELOPMENT ORCHESTRATOR")
    print("=" * 70)

    print("\nUSER REQUEST")
    print(user_request)

    # --------------------------------------------------------
    # PLAN
    # --------------------------------------------------------

    print("\n🧠 Asking local Qwen3 8B to create a plan...")

    plan = create_plan(user_request)

    # --------------------------------------------------------
    # HUMAN-IN-THE-LOOP
    # --------------------------------------------------------

    while True:

        print_plan(plan)

        print("\nWhat do you want to do?")

        print("[A] Approve plan and execute")
        print("[R] Regenerate plan")
        print("[C] Cancel")

        choice = input("\n> ").strip().lower()

        # ----------------------------------------------------
        # APPROVE
        # ----------------------------------------------------

        if choice == "a":

            print("\n✅ Plan approved.")
            break

        # ----------------------------------------------------
        # REGENERATE
        # ----------------------------------------------------

        elif choice == "r":

            feedback = input(
                "\nWhat should be changed in the plan?\n> "
            ).strip()

            print(
                "\n🧠 Asking Qwen3 8B to revise the plan..."
            )

            plan = create_plan(
                user_request,
                feedback
            )

        # ----------------------------------------------------
        # CANCEL
        # ----------------------------------------------------

        elif choice == "c":

            print("\n❌ Cancelled.")
            return

        else:

            print(
                "\nPlease enter A, R or C."
            )

    # --------------------------------------------------------
    # START GAME SERVER
    # --------------------------------------------------------

    print("\n🌐 Starting local game server...")

    start_server()

    # --------------------------------------------------------
    # SEND APPROVED PLAN TO CODING AGENT
    # --------------------------------------------------------

    execution_request = f"""
Original user request:

{user_request}

The following implementation plan was created by a
separate planning agent and APPROVED BY THE HUMAN.

APPROVED PLAN:

{json.dumps(plan, indent=2)}

Execute this approved plan.

Important:
- Follow the approved plan as closely as practical.
- You still have authority to make small implementation
  adjustments when required for the game to work.
- Use your existing tools for all file operations.
- Test the game before finishing.
"""

    print("\n🚀 Starting coding agent...\n")

    run_agent(execution_request)


# ============================================================
# MAIN
# ============================================================

if __name__ == "__main__":

    request = input(
        "\nWhat game should I create?\n> "
    ).strip()

    if not request:
        print("No request provided.")
        exit()

    orchestrate(request)