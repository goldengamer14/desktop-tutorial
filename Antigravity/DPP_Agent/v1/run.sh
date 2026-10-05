#!/usr/bin/env bash
# ─── DPP Agent launcher ───────────────────────────────────────────────────────
# Activates the .venv and runs main.py, passing all arguments through.
# Usage:  ./run.sh [--topic "..." --difficulty ... ...]
# ─────────────────────────────────────────────────────────────────────────────

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VENV="$SCRIPT_DIR/.venv"

if [[ ! -d "$VENV" ]]; then
    echo "❌  .venv not found at $VENV"
    echo "    Run: python3 -m venv .venv && source .venv/bin/activate && pip install -r requirements.txt"
    exit 1
fi

# Activate
source "$VENV/bin/activate"

# Verify we're inside the venv
PYTHON="$VENV/bin/python"

echo "🐍  Using: $($PYTHON --version)"
echo "📦  Venv : $VENV"
echo ""

exec "$PYTHON" "$SCRIPT_DIR/main.py" "$@"
