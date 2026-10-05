"""
DPP Agent – Config Module
Loads all settings from .env and exposes typed constants.
"""
from __future__ import annotations

import os
from pathlib import Path
from dotenv import load_dotenv

# ── Locate repo root & load .env ─────────────────────────────────────────────
ROOT = Path(__file__).resolve().parent.parent
load_dotenv(ROOT / ".env", override=False)

# ── API / model settings ─────────────────────────────────────────────────────
GROQ_API_KEY:    str = os.getenv("GROQ_API_KEY", "")
GROQ_MODEL:      str = os.getenv("GROQ_MODEL", "qwen/qwen3.8-27b")
OLLAMA_BASE_URL: str = os.getenv("OLLAMA_BASE_URL", "http://localhost:11434")
OLLAMA_MODEL:    str = os.getenv("OLLAMA_MODEL", "qwen3:1.7b")

# Ordered list of Groq models to try (best → most available)
GROQ_MODEL_PRIORITY: list[str] = [
    "qwen/qwen3.8-27b",
    "openai/gpt-oss-120b",
    "openai/gpt-oss-20b",
    "allam-2-7b",
]

# ── Research settings ─────────────────────────────────────────────────────────
WIKI_LANG: str = os.getenv("WIKI_LANG", "en")

# ── Output settings ───────────────────────────────────────────────────────────
OUTPUT_DIR: Path = Path(os.getenv("OUTPUT_DIR", str(ROOT / "output")))
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

# ── Difficulty levels ─────────────────────────────────────────────────────────
DIFFICULTIES = ["easy", "medium", "hard"]

# ── Problem types ─────────────────────────────────────────────────────────────
PROBLEM_TYPES = [
    "mcq",           # Multiple Choice Question
    "short_answer",  # Short Answer
    "long_answer",   # Long / Essay Answer
    "fill_blank",    # Fill in the Blank
    "true_false",    # True / False
]
