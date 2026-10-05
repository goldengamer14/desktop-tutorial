"""
DPP Agent – Research Module
Fetches topic context from Wikipedia, Open-Library, and optionally
a free REST quiz/trivia API, then consolidates a rich context string
for the LLM to use when generating problems.

Every fetch step prints live status so users can see exactly what is
being retrieved (no silent hanging).
"""
from __future__ import annotations

import textwrap
from typing import Optional
import requests
import wikipediaapi
from rich.console import Console

from .config import WIKI_LANG

_console = Console()


# ── Wikipedia ─────────────────────────────────────────────────────────────────

def _fetch_wikipedia(topic: str, sentences: int = 15) -> str:
    """Return the first `sentences`-worth of text for the Wikipedia article."""
    _console.print(f"  [cyan]📖 Wikipedia[/cyan]  → searching [bold]{topic}[/bold] …")
    wiki = wikipediaapi.Wikipedia(
        language=WIKI_LANG,
        user_agent="DPP-ProblemSetAgent/1.0 (educational-tool)",
    )
    page = wiki.page(topic)
    if not page.exists():
        _console.print(f"  [yellow]  ↳ exact page not found, trying fuzzy search …[/yellow]")
        search_url = (
            "https://en.wikipedia.org/w/api.php"
            f"?action=opensearch&search={topic}&limit=1&format=json"
        )
        try:
            resp = requests.get(search_url, timeout=10)
            results = resp.json()
            if results and len(results) > 1 and results[1]:
                matched = results[1][0]
                _console.print(f"  [yellow]  ↳ fuzzy match: [bold]{matched}[/bold][/yellow]")
                page = wiki.page(matched)
        except Exception:
            pass
    if not page.exists():
        _console.print(f"  [red]  ↳ Wikipedia: no page found[/red]")
        return ""
    # Truncate to roughly `sentences` sentences
    text = page.text
    parts = text.split(". ")
    truncated = ". ".join(parts[:sentences]) + ("." if len(parts) >= sentences else "")
    _console.print(f"  [green]  ↳ Wikipedia: fetched {len(text):,} chars ({len(parts)} sentences)[/green]")
    return textwrap.fill(truncated, width=120)


# ── Open-Library (book/subject info) ─────────────────────────────────────────

def _fetch_openlibrary(topic: str) -> str:
    """Return a brief paragraph from Open Library's subject search."""
    _console.print(f"  [cyan]📚 Open Library[/cyan] → searching [bold]{topic}[/bold] …")
    url = f"https://openlibrary.org/subjects/{topic.lower().replace(' ', '_')}.json?limit=3"
    try:
        resp = requests.get(url, timeout=10)
        data = resp.json()
        works = data.get("works", [])
        if not works:
            _console.print("  [yellow]  ↳ Open Library: no results[/yellow]")
            return ""
        titles = [w.get("title", "") for w in works]
        _console.print(f"  [green]  ↳ Open Library: found {len(titles)} book(s)[/green]")
        return f"Related books on Open Library: {', '.join(titles)}."
    except Exception as e:
        _console.print(f"  [red]  ↳ Open Library error: {e}[/red]")
        return ""


# ── Free trivia / quiz API (Open Trivia DB) ───────────────────────────────────

CATEGORY_MAP: dict[str, int] = {
    "general": 9,
    "science": 17,
    "computer science": 18,
    "mathematics": 19,
    "history": 23,
    "geography": 22,
    "politics": 24,
    "art": 25,
    "celebrities": 26,
    "animals": 27,
    "vehicles": 28,
}


def _fetch_trivia(topic: str, amount: int = 5) -> str:
    """Pull sample MCQs from Open Trivia DB for inspiration."""
    cat_id = None
    for key, val in CATEGORY_MAP.items():
        if key in topic.lower():
            cat_id = val
            break
    cat_label = f"category={cat_id}" if cat_id else "general"
    _console.print(f"  [cyan]🎯 Open Trivia DB[/cyan] → {amount} questions ({cat_label}) …")
    url = "https://opentdb.com/api.php?type=multiple&encode=url3986"
    url += f"&amount={amount}"
    if cat_id:
        url += f"&category={cat_id}"
    try:
        resp = requests.get(url, timeout=10)
        data = resp.json()
        if data.get("response_code") != 0:
            _console.print("  [yellow]  ↳ Trivia DB: no results (try a broader topic)[/yellow]")
            return ""
        lines = []
        for q in data.get("results", []):
            import urllib.parse
            question = urllib.parse.unquote(q.get("question", ""))
            answer   = urllib.parse.unquote(q.get("correct_answer", ""))
            lines.append(f"- Q: {question}  →  A: {answer}")
        _console.print(f"  [green]  ↳ Trivia DB: fetched {len(lines)} sample questions[/green]")
        return "Sample trivia questions for reference:\n" + "\n".join(lines)
    except Exception as e:
        _console.print(f"  [red]  ↳ Trivia DB error: {e}[/red]")
        return ""


# ── Aggregator ────────────────────────────────────────────────────────────────

def research_topic(topic: str, depth: str = "medium") -> str:
    """
    Aggregate research context for a given topic.

    Args:
        topic: The subject to research (e.g. "Chemical Bonding", "UPSC Polity").
        depth: 'light' | 'medium' | 'deep' – controls how many sources are hit.

    Returns:
        A consolidated multi-paragraph string with topic context.
    """
    _console.rule(f"[bold cyan]🔍 Researching: {topic}[/bold cyan]")
    sections: list[str] = []

    # Always fetch Wikipedia
    wiki_text = _fetch_wikipedia(topic)
    if wiki_text:
        sections.append(f"[Wikipedia – {topic}]\n{wiki_text}")

    if depth in ("medium", "deep"):
        # Open Library
        ol_text = _fetch_openlibrary(topic)
        if ol_text:
            sections.append(f"[Open Library]\n{ol_text}")

        # Trivia DB
        trivia_text = _fetch_trivia(topic)
        if trivia_text:
            sections.append(f"[Sample Questions]\n{trivia_text}")

    if not sections:
        _console.print(f"[yellow]⚠ No external data found for '{topic}'. Model will rely on training knowledge.[/yellow]")
        return f"No external data found for topic: '{topic}'. Generating from model knowledge only."

    total_chars = sum(len(s) for s in sections)
    _console.print(
        f"\n[bold green]✔ Research complete[/bold green] — "
        f"{len(sections)} source(s) · {total_chars:,} chars of context\n"
    )
    return "\n\n".join(sections)
