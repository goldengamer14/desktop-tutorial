"""
DPP Agent – Rich Terminal UI (display.py)
Renders problem sets in the terminal using the Rich library.
"""
from __future__ import annotations

from rich.console import Console
from rich.panel import Panel
from rich.table import Table
from rich.text import Text
from rich import box

from .generator import ProblemSet, Problem

console = Console()

DIFF_COLOR = {"easy": "green", "medium": "yellow", "hard": "red"}
TYPE_LABEL = {
    "mcq": "MCQ",
    "short_answer": "Short Answer",
    "long_answer": "Long Answer",
    "fill_blank": "Fill in Blank",
    "true_false": "True / False",
}


def print_header(ps: ProblemSet) -> None:
    color = DIFF_COLOR.get(ps.difficulty, "white")
    table = Table(box=box.SIMPLE, show_header=False)
    table.add_column(style="bold cyan", no_wrap=True)
    table.add_column()
    table.add_row("Topic",      ps.topic)
    table.add_row("Difficulty", f"[{color}]{ps.difficulty.capitalize()}[/{color}]")
    table.add_row("Problems",   str(len(ps.problems)))
    table.add_row("Marks",      str(ps.total_marks))

    console.print(Panel(table, title="📚 [bold]DPP Problem Set[/bold]", border_style="bright_blue"))


def print_problem(p: Problem, show_answer: bool = False) -> None:
    color = DIFF_COLOR.get(p.difficulty, "white")
    label = TYPE_LABEL.get(p.type, p.type)

    header = Text()
    header.append(f"Q{p.id}. ", style="bold white")
    header.append(f"[{label}]", style="dim")
    header.append(f"  {p.difficulty.capitalize()}", style=f"bold {color}")
    header.append(f"  • {p.marks} mark{'s' if p.marks > 1 else ''}", style="cyan")

    body_lines: list[str] = [p.question, ""]

    if p.options:
        for idx, opt in enumerate(p.options):
            body_lines.append(f"  [bold]{chr(65+idx)})[/bold] {opt}")
        body_lines.append("")

    if p.hint:
        body_lines.append(f"[dim]💡 Hint: {p.hint}[/dim]")
        body_lines.append("")

    body = "\n".join(body_lines)

    console.print(Panel(
        f"[bold]{p.question}[/bold]\n" +
        ("\n".join(f"  [bold]{chr(65+i)})[/bold] {o}" for i, o in enumerate(p.options)) + "\n" if p.options else "") +
        (f"\n[dim]💡 {p.hint}[/dim]" if p.hint else ""),
        title=header,
        border_style=color,
        padding=(0, 1),
    ))

    if show_answer:
        ans_text = f"✅  [bold green]{p.answer}[/bold green]"
        if p.explanation:
            ans_text += f"\n[dim]{p.explanation}[/dim]"
        console.print(Panel(ans_text, title="Answer", border_style="green", padding=(0, 2)))


def print_problem_set(ps: ProblemSet, show_answers: bool = False) -> None:
    print_header(ps)
    for p in ps.problems:
        print_problem(p, show_answer=show_answers)
    console.rule("[bold cyan]End of Problem Set[/bold cyan]")


def print_success(message: str) -> None:
    console.print(f"[bold green]✔[/bold green]  {message}")


def print_error(message: str) -> None:
    console.print(f"[bold red]✘[/bold red]  {message}")


def print_info(message: str) -> None:
    console.print(f"[bold cyan]ℹ[/bold cyan]  {message}")
