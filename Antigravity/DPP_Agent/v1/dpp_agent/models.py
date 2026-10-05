"""
DPP Agent – Models Module
Provides a unified streaming interface to:
  • Groq cloud API  (preferred when GROQ_API_KEY is set)
  • Ollama local   (qwen3:1.7b — always-available fallback)

Every token is streamed live to the terminal so you can watch the model
think, research, and write answers in real time. Nothing hangs silently.
"""
from __future__ import annotations

import json
import re
import sys
import time
from typing import Iterator

from rich.console import Console
from rich.live import Live
from rich.panel import Panel
from rich.spinner import Spinner
from rich.text import Text
from rich.columns import Columns
from rich import box

from .config import (
    GROQ_API_KEY, GROQ_MODEL, GROQ_MODEL_PRIORITY,
    OLLAMA_BASE_URL, OLLAMA_MODEL,
)

_console = Console()

# ── Phase labels printed before the stream begins ────────────────────────────

def _phase_header(label: str, model: str, backend: str) -> None:
    color = "cyan" if backend == "groq" else "magenta"
    _console.print(
        f"\n[bold {color}]▶ {label}[/bold {color}]  "
        f"[dim]({backend} · {model})[/dim]"
    )
    _console.rule(style=f"dim {color}")


# ── Live streaming renderer ───────────────────────────────────────────────────

class _StreamRenderer:
    """
    Accumulates streamed tokens and reprints the growing text block live.
    Detects <think>…</think> sections and renders them in a distinct dim style
    so you can see the model's reasoning separately from its final output.
    """

    THINK_OPEN  = re.compile(r"<think>",  re.IGNORECASE)
    THINK_CLOSE = re.compile(r"</think>", re.IGNORECASE)

    def __init__(self, title: str = "Generating …") -> None:
        self.title    = title
        self._buf     = ""          # full accumulated text
        self._in_think = False
        self._think_buf = ""
        self._output_buf = ""
        self._chars   = 0
        self._tokens  = 0
        self._t0      = time.perf_counter()

    def feed(self, chunk: str) -> None:
        self._buf += chunk
        self._chars += len(chunk)
        self._tokens += 1  # approximate

        # Split into think / output regions
        remaining = chunk
        while remaining:
            if self._in_think:
                m = self.THINK_CLOSE.search(remaining)
                if m:
                    self._think_buf += remaining[: m.start()]
                    self._in_think = False
                    remaining = remaining[m.end():]
                else:
                    self._think_buf += remaining
                    remaining = ""
            else:
                m = self.THINK_OPEN.search(remaining)
                if m:
                    self._output_buf += remaining[: m.start()]
                    self._in_think = True
                    remaining = remaining[m.end():]
                else:
                    self._output_buf += remaining
                    remaining = ""

    def render(self) -> Panel:
        elapsed = time.perf_counter() - self._t0
        speed   = self._chars / max(elapsed, 0.001)

        parts: list[str] = []

        if self._think_buf.strip():
            parts.append(
                f"[dim italic]💭 Thinking…\n{self._think_buf.strip()}[/dim italic]"
            )

        if self._output_buf.strip():
            if parts:
                parts.append("[dim]─────────────────[/dim]")
            parts.append(self._output_buf)

        if not parts:
            parts.append("[dim]…[/dim]")

        footer = (
            f"[dim]{self._chars:,} chars · "
            f"{elapsed:.1f}s · "
            f"{speed:.0f} ch/s[/dim]"
        )
        parts.append("\n" + footer)

        return Panel(
            "\n".join(parts),
            title=f"[bold cyan]{self.title}[/bold cyan]",
            border_style="cyan",
            box=box.ROUNDED,
            padding=(0, 1),
        )

    @property
    def full_text(self) -> str:
        return self._buf


# ── Groq streaming ────────────────────────────────────────────────────────────

def _stream_groq(
    prompt: str,
    system: str,
    temperature: float,
    model: str,
) -> str:
    from groq import Groq

    client = Groq(api_key=GROQ_API_KEY)
    messages = []
    if system:
        messages.append({"role": "system", "content": system})
    messages.append({"role": "user", "content": prompt})

    _phase_header("Groq Cloud — streaming", model, "groq")

    renderer = _StreamRenderer(title=f"Groq · {model}")
    with Live(renderer.render(), console=_console, refresh_per_second=8) as live:
        stream = client.chat.completions.create(
            model=model,
            messages=messages,
            temperature=temperature,
            max_tokens=4096,
            stream=True,
        )
        for chunk in stream:
            delta = chunk.choices[0].delta.content or ""
            if delta:
                renderer.feed(delta)
                live.update(renderer.render())

    _console.print(f"[dim green]✔ Groq done · {len(renderer.full_text):,} chars[/dim green]\n")
    return renderer.full_text.strip()


# ── Ollama streaming ──────────────────────────────────────────────────────────

def _stream_ollama(
    prompt: str,
    system: str,
    temperature: float,
) -> str:
    import ollama

    messages = []
    if system:
        messages.append({"role": "system", "content": system})
    messages.append({"role": "user", "content": prompt})

    _phase_header("Ollama Local — streaming", OLLAMA_MODEL, "ollama")

    renderer = _StreamRenderer(title=f"Ollama · {OLLAMA_MODEL}")
    with Live(renderer.render(), console=_console, refresh_per_second=8) as live:
        stream = ollama.chat(
            model=OLLAMA_MODEL,
            messages=messages,
            options={"temperature": temperature},
            stream=True,
        )
        for chunk in stream:
            delta = chunk.get("message", {}).get("content", "")
            if delta:
                renderer.feed(delta)
                live.update(renderer.render())

    _console.print(f"[dim magenta]✔ Ollama done · {len(renderer.full_text):,} chars[/dim magenta]\n")
    return renderer.full_text.strip()


# ── Auto model resolver for Groq ─────────────────────────────────────────────

_resolved_groq_model: str | None = None   # cached after first successful call


def _resolve_groq_model() -> str:
    """
    Try models in GROQ_MODEL_PRIORITY order, pick the first one that exists.
    Uses the GROQ_MODEL env override if set explicitly by the user.
    """
    global _resolved_groq_model
    if _resolved_groq_model:
        return _resolved_groq_model

    # If the user explicitly set GROQ_MODEL and it differs from default, trust it
    import os
    env_model = os.getenv("GROQ_MODEL", "")
    candidate_list = [env_model] + GROQ_MODEL_PRIORITY if env_model else GROQ_MODEL_PRIORITY

    from groq import Groq
    client = Groq(api_key=GROQ_API_KEY)
    try:
        available = {m.id for m in client.models.list().data}
    except Exception:
        available = set()

    for model in candidate_list:
        if model in available:
            _resolved_groq_model = model
            _console.print(f"[dim green]✔ Groq model resolved → [bold]{model}[/bold][/dim green]")
            return model

    # Last resort: return first priority even if not confirmed
    _resolved_groq_model = GROQ_MODEL_PRIORITY[0]
    return _resolved_groq_model


# ── Unified generate ──────────────────────────────────────────────────────────

def generate(
    prompt: str,
    system: str = "",
    temperature: float = 0.7,
    prefer_cloud: bool = True,
) -> str:
    """
    Stream-generate text using the best available model.
    Every token is printed live — nothing hangs silently.

    Priority:
      1. Groq cloud  (if GROQ_API_KEY is valid and prefer_cloud=True)
      2. Ollama local  (always-available fallback)
    """
    if prefer_cloud and GROQ_API_KEY and GROQ_API_KEY not in ("", "your_groq_api_key_here"):
        model = _resolve_groq_model()
        try:
            return _stream_groq(prompt, system=system, temperature=temperature, model=model)
        except Exception as exc:
            _console.print(f"[bold red]✘ Groq failed:[/bold red] {exc}")
            _console.print("[yellow]⟳ Falling back to Ollama …[/yellow]\n")

    return _stream_ollama(prompt, system=system, temperature=temperature)


# ── JSON extraction helper ────────────────────────────────────────────────────

def extract_json(text: str) -> list | dict | None:
    """
    Extract a JSON object / array from raw LLM output.
    Strips <think>…</think> blocks and markdown fences first.
    """
    # Remove think blocks entirely
    cleaned = re.sub(r"<think>.*?</think>", "", text, flags=re.DOTALL | re.IGNORECASE)
    # Strip markdown fences
    cleaned = re.sub(r"```(?:json)?\s*", "", cleaned)
    cleaned = re.sub(r"```", "", cleaned)

    # Find first [...] or {...}
    for pattern in (r"\[.*\]", r"\{.*\}"):
        m = re.search(pattern, cleaned, re.DOTALL)
        if m:
            try:
                return json.loads(m.group())
            except json.JSONDecodeError:
                pass
    return None
