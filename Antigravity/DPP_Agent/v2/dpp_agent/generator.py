"""
DPP Agent – Problem Set Generator Core
Builds structured problem sets from topic + difficulty + type parameters.
"""
from __future__ import annotations

import json
import textwrap
from dataclasses import dataclass, field
from typing import Optional

from .config import DIFFICULTIES, PROBLEM_TYPES
from .models import generate, extract_json
from .research import research_topic


# ── Data models ───────────────────────────────────────────────────────────────

@dataclass
class Problem:
    id: int
    type: str           # mcq | short_answer | long_answer | fill_blank | true_false
    difficulty: str     # easy | medium | hard
    question: str
    options: list[str] = field(default_factory=list)   # MCQ options
    answer: str = ""
    explanation: str = ""
    hint: str = ""
    marks: int = 1


@dataclass
class ProblemSet:
    topic: str
    difficulty: str
    problems: list[Problem] = field(default_factory=list)
    total_marks: int = 0
    context_used: str = ""

    def __post_init__(self):
        self.total_marks = sum(p.marks for p in self.problems)


# ── System prompt ─────────────────────────────────────────────────────────────

_SYSTEM = """You are an expert educational content creator and examiner.
Your task is to create high-quality, accurate, and well-structured practice problems.
Always return VALID JSON matching the schema provided. No extra prose outside the JSON block."""


# ── Prompt builder ────────────────────────────────────────────────────────────

def _build_prompt(
    topic: str,
    difficulty: str,
    problem_type: str,
    count: int,
    context: str,
    include_hints: bool,
    include_explanations: bool,
) -> str:
    marks_map = {"easy": 1, "medium": 2, "hard": 3}
    marks = marks_map.get(difficulty, 2)

    hint_field = '"hint": "<optional hint string>",' if include_hints else ""
    exp_field  = '"explanation": "<detailed explanation>",' if include_explanations else ""

    type_instructions = {
        "mcq":
            'options: exactly 4 strings (A/B/C/D), answer: the correct option letter (A, B, C, or D)',
        "short_answer":
            'options: [], answer: concise 1-2 sentence answer',
        "long_answer":
            'options: [], answer: detailed paragraph answer (6-10 sentences)',
        "fill_blank":
            'question must contain "___" placeholder; options: [], answer: the missing word/phrase',
        "true_false":
            'options: ["True", "False"], answer: "True" or "False"',
    }

    schema_note = type_instructions.get(problem_type, "options: [], answer: string")

    prompt = textwrap.dedent(f"""
        Topic: {topic}
        Difficulty: {difficulty}
        Problem type: {problem_type}
        Number of problems: {count}
        Marks per problem: {marks}

        Research context (use this to ensure accuracy):
        ---
        {context[:3000]}
        ---

        Generate exactly {count} {difficulty}-level {problem_type} problems on "{topic}".
        Schema for each problem:
          {{
            "id": <integer starting at 1>,
            "type": "{problem_type}",
            "difficulty": "{difficulty}",
            "question": "<clear, unambiguous question>",
            "options": [...],   // {schema_note}
            "answer": "<answer>",
            {exp_field}
            {hint_field}
            "marks": {marks}
          }}

        Return ONLY a JSON array of {count} problem objects. No extra text.
    """).strip()
    return prompt


# ── Core generator ────────────────────────────────────────────────────────────

def generate_problem_set(
    topic: str,
    difficulty: str = "medium",
    problem_type: str = "mcq",
    count: int = 10,
    research_depth: str = "medium",
    include_hints: bool = True,
    include_explanations: bool = True,
    prefer_cloud: bool = True,
) -> ProblemSet:
    """
    Generate a structured problem set for the given topic.

    Args:
        topic:              Subject (e.g., "Chemical Bonding", "UPSC Polity").
        difficulty:         "easy" | "medium" | "hard".
        problem_type:       One of PROBLEM_TYPES.
        count:              Number of problems to generate (1–30).
        research_depth:     "light" | "medium" | "deep".
        include_hints:      Whether to ask for per-problem hints.
        include_explanations: Whether to ask for per-problem explanations.
        prefer_cloud:       Use Groq if API key is available, else Ollama.

    Returns:
        A ProblemSet dataclass instance.
    """
    # Validate inputs
    difficulty   = difficulty.lower()   if difficulty   in DIFFICULTIES    else "medium"
    problem_type = problem_type.lower() if problem_type in PROBLEM_TYPES   else "mcq"
    count        = max(1, min(count, 30))

    # Research
    context = research_topic(topic, depth=research_depth)

    # Build prompt & call LLM
    prompt = _build_prompt(
        topic=topic,
        difficulty=difficulty,
        problem_type=problem_type,
        count=count,
        context=context,
        include_hints=include_hints,
        include_explanations=include_explanations,
    )
    raw = generate(prompt, system=_SYSTEM, temperature=0.7, prefer_cloud=prefer_cloud)

    # Parse JSON
    problems_data = extract_json(raw)
    if not isinstance(problems_data, list):
        # Fallback: wrap if single object returned
        if isinstance(problems_data, dict):
            problems_data = [problems_data]
        else:
            problems_data = []

    problems: list[Problem] = []
    for i, item in enumerate(problems_data, start=1):
        if not isinstance(item, dict):
            continue
        problems.append(Problem(
            id=item.get("id", i),
            type=item.get("type", problem_type),
            difficulty=item.get("difficulty", difficulty),
            question=item.get("question", ""),
            options=item.get("options", []),
            answer=item.get("answer", ""),
            explanation=item.get("explanation", ""),
            hint=item.get("hint", ""),
            marks=item.get("marks", 1),
        ))

    return ProblemSet(
        topic=topic,
        difficulty=difficulty,
        problems=problems,
        context_used=context,
    )
