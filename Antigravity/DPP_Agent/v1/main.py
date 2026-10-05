#!/usr/bin/env python3
"""
DPP Agent – Main CLI Entry Point
─────────────────────────────────
Usage examples:
  python main.py
  python main.py --topic "Chemical Bonding" --difficulty hard --type mcq --count 10
  python main.py --topic "UPSC Polity" --difficulty medium --type short_answer --count 5 --no-cloud
  python main.py --topic "Data Structures" --difficulty easy --type true_false --count 8 --export all
"""
from __future__ import annotations

import argparse
import sys
from pathlib import Path

from rich.prompt import Prompt, Confirm, IntPrompt
from rich.console import Console
from rich.panel import Panel

# Ensure we can import the dpp_agent package
sys.path.insert(0, str(Path(__file__).parent))

from dpp_agent.config import DIFFICULTIES, PROBLEM_TYPES, OUTPUT_DIR, GROQ_API_KEY
from dpp_agent.generator import generate_problem_set
from dpp_agent.exporter import export_json, export_markdown, export_text
from dpp_agent.display import (
    print_problem_set, print_success, print_error, print_info, console
)

BANNER = """
[bold cyan]
╔══════════════════════════════════════════════╗
║          🎯  DPP Problem Set Agent           ║
║   Powered by Groq Cloud + Ollama qwen3:1.7b  ║
╚══════════════════════════════════════════════╝
[/bold cyan]
"""


# ── Interactive mode ──────────────────────────────────────────────────────────

def interactive_mode() -> None:
    console.print(BANNER)

    # Model backend indicator
    if GROQ_API_KEY and GROQ_API_KEY != "your_groq_api_key_here":
        print_info("Groq API key detected — cloud model will be preferred.")
    else:
        print_info("No Groq API key — using Ollama (qwen3:1.7b) locally.")

    topic = Prompt.ask(
        "\n[bold yellow]Enter topic[/bold yellow]",
        default="Chemical Bonding",
    )

    difficulty = Prompt.ask(
        "[bold yellow]Difficulty[/bold yellow]",
        choices=DIFFICULTIES,
        default="medium",
    )

    ptype = Prompt.ask(
        "[bold yellow]Problem type[/bold yellow]",
        choices=PROBLEM_TYPES,
        default="mcq",
    )

    count = IntPrompt.ask(
        "[bold yellow]Number of problems[/bold yellow]",
        default=5,
    )

    depth = Prompt.ask(
        "[bold yellow]Research depth[/bold yellow]",
        choices=["light", "medium", "deep"],
        default="medium",
    )

    include_hints = Confirm.ask("[bold yellow]Include hints?[/bold yellow]", default=True)
    include_explanations = Confirm.ask("[bold yellow]Include explanations?[/bold yellow]", default=True)
    show_answers = Confirm.ask("[bold yellow]Show answers in terminal?[/bold yellow]", default=True)

    prefer_cloud = GROQ_API_KEY not in ("", "your_groq_api_key_here")

    console.rule("[bold cyan]Generating …[/bold cyan]")

    problem_set = generate_problem_set(
        topic=topic,
        difficulty=difficulty,
        problem_type=ptype,
        count=count,
        research_depth=depth,
        include_hints=include_hints,
        include_explanations=include_explanations,
        prefer_cloud=prefer_cloud,
    )

    if not problem_set.problems:
        print_error("No problems were generated. Check model connectivity and try again.")
        return

    print_problem_set(problem_set, show_answers=show_answers)

    # Export
    console.print("\n[bold]Export options:[/bold]")
    fmt = Prompt.ask(
        "Export format",
        choices=["json", "markdown", "text", "all", "none"],
        default="markdown",
    )
    _do_export(problem_set, fmt)


# ── CLI mode ──────────────────────────────────────────────────────────────────

def cli_mode(args: argparse.Namespace) -> None:
    console.print(BANNER)

    if args.groq_key:
        import os
        os.environ["GROQ_API_KEY"] = args.groq_key

    prefer_cloud = not args.no_cloud and bool(
        GROQ_API_KEY and GROQ_API_KEY != "your_groq_api_key_here"
    )

    console.rule("[bold cyan]Generating …[/bold cyan]")
    problem_set = generate_problem_set(
        topic=args.topic,
        difficulty=args.difficulty,
        problem_type=args.type,
        count=args.count,
        research_depth=args.depth,
        include_hints=not args.no_hints,
        include_explanations=not args.no_explanations,
        prefer_cloud=prefer_cloud,
    )

    if not problem_set.problems:
        print_error("No problems were generated.")
        sys.exit(1)

    print_problem_set(problem_set, show_answers=args.show_answers)
    _do_export(problem_set, args.export)


# ── Export helper ─────────────────────────────────────────────────────────────

def _do_export(problem_set, fmt: str) -> None:
    if fmt == "none":
        return

    paths: list[Path] = []
    if fmt in ("json", "all"):
        p = export_json(problem_set)
        paths.append(p)
    if fmt in ("markdown", "all"):
        p = export_markdown(problem_set)
        paths.append(p)
    if fmt in ("text", "all"):
        p = export_text(problem_set)
        paths.append(p)

    for path in paths:
        print_success(f"Saved → [bold]{path}[/bold]")


# ── Argument parser ───────────────────────────────────────────────────────────

def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        description="DPP Problem Set Generator Agent",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    p.add_argument("--topic",       type=str,   default=None,       help="Topic to generate problems on")
    p.add_argument("--difficulty",  type=str,   default="medium",   choices=DIFFICULTIES)
    p.add_argument("--type",        type=str,   default="mcq",      choices=PROBLEM_TYPES, dest="type")
    p.add_argument("--count",       type=int,   default=5,          help="Number of problems (1-30)")
    p.add_argument("--depth",       type=str,   default="medium",   choices=["light","medium","deep"])
    p.add_argument("--export",      type=str,   default="markdown", choices=["json","markdown","text","all","none"])
    p.add_argument("--show-answers",action="store_true", default=False)
    p.add_argument("--no-hints",    action="store_true", default=False)
    p.add_argument("--no-explanations", action="store_true", default=False)
    p.add_argument("--no-cloud",    action="store_true", default=False, help="Force local Ollama even if Groq key is set")
    p.add_argument("--groq-key",    type=str,   default=None,       help="Override GROQ_API_KEY at runtime")
    return p


# ── Entry point ───────────────────────────────────────────────────────────────

if __name__ == "__main__":
    parser = build_parser()
    args = parser.parse_args()

    if args.topic:
        cli_mode(args)
    else:
        interactive_mode()
