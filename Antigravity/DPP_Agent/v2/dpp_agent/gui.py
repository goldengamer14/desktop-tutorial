"""Native desktop interface for the DPP Agent.

The UI deliberately lives on top of the existing generator and exporter.  A
worker thread keeps research and model calls off the Qt event loop, so the
window remains responsive while a set is being created.
"""
from __future__ import annotations

import html
import sys

from PySide6.QtCore import QObject, QThread, Qt, Signal, Slot
from PySide6.QtWidgets import (
    QApplication,
    QCheckBox,
    QComboBox,
    QFormLayout,
    QGroupBox,
    QHBoxLayout,
    QLabel,
    QMainWindow,
    QMessageBox,
    QPushButton,
    QScrollArea,
    QSpinBox,
    QSplitter,
    QTextBrowser,
    QVBoxLayout,
    QWidget,
)

from .config import DIFFICULTIES, OUTPUT_DIR, PROBLEM_TYPES, GROQ_API_KEY
from .exporter import export_json, export_markdown, export_text
from .generator import Problem, ProblemSet, generate_problem_set


TYPE_LABELS = {
    "mcq": "Multiple choice",
    "short_answer": "Short answer",
    "long_answer": "Long answer",
    "fill_blank": "Fill in the blank",
    "true_false": "True / False",
}


def _problem_html(problem: Problem, show_answers: bool) -> str:
    options = "".join(
        f"<li><b>{chr(65 + i)})</b> {html.escape(str(option))}</li>"
        for i, option in enumerate(problem.options)
    )
    answer = ""
    if show_answers:
        answer = (
            '<div class="answer"><b>Answer:</b> '
            f"{html.escape(str(problem.answer))}"
            + (f"<br><span>{html.escape(problem.explanation)}</span>" if problem.explanation else "")
            + "</div>"
        )
    hint = f'<div class="hint">Hint: {html.escape(problem.hint)}</div>' if problem.hint else ""
    return f"""
    <article class="problem">
      <h3>Q{problem.id} <small>{html.escape(TYPE_LABELS.get(problem.type, problem.type))}
      · {html.escape(problem.difficulty.title())} · {problem.marks} mark(s)</small></h3>
      <p>{html.escape(problem.question)}</p>
      {f'<ol type="A">{options}</ol>' if options else ''}
      {hint}{answer}
    </article>
    """


def problem_set_html(problem_set: ProblemSet, show_answers: bool) -> str:
    cards = "".join(_problem_html(p, show_answers) for p in problem_set.problems)
    return f"""
    <html><head><style>
      body {{ font-family: sans-serif; color: #f1f5ff; background: #151a24; line-height: 1.45; }}
      h1 {{ color: #9fc2ff; margin-bottom: 4px; }}
      .meta {{ color: #b0bdd2; margin-bottom: 18px; }}
      .problem {{ border: 1px solid #3a465c; background: #1d2431; border-radius: 10px; padding: 12px 16px; margin: 12px 0; }}
      .problem h3 {{ color: #9fc2ff; margin: 0 0 8px; }}
      small {{ color: #b0bdd2; font-weight: normal; }}
      .hint {{ color: #ffe29a; background: #3b321b; border: 1px solid #735f27; border-radius: 6px; padding: 7px 10px; margin-top: 10px; }}
      .answer {{ color: #a9f0c4; background: #193727; border: 1px solid #2e724b; border-radius: 6px; padding: 8px 10px; margin-top: 10px; }}
      .answer span {{ color: #c9e8d3; }}
    </style></head><body>
      <h1>{html.escape(problem_set.topic)}</h1>
      <div class="meta">{html.escape(problem_set.difficulty.title())} · {len(problem_set.problems)} problems · {problem_set.total_marks} marks</div>
      {cards}
    </body></html>
    """


class GenerationWorker(QObject):
    finished = Signal(object)
    failed = Signal(str)

    def __init__(self, settings: dict):
        super().__init__()
        self.settings = settings

    @Slot()
    def run(self) -> None:
        try:
            result = generate_problem_set(**self.settings)
            self.finished.emit(result)
        except Exception as exc:  # surface model/research errors in the UI
            self.failed.emit(f"{type(exc).__name__}: {exc}")


class DPPWindow(QMainWindow):
    def __init__(self) -> None:
        super().__init__()
        self.problem_set: ProblemSet | None = None
        self.thread: QThread | None = None
        self.worker: GenerationWorker | None = None
        self.setWindowTitle("DPP Agent · Practice Set Generator")
        self.resize(1180, 760)
        self._build_ui()

    def _build_ui(self) -> None:
        root = QWidget()
        outer = QVBoxLayout(root)
        title = QLabel("DPP Agent")
        title.setObjectName("title")
        subtitle = QLabel("Create focused practice sets from any topic")
        subtitle.setObjectName("subtitle")
        outer.addWidget(title)
        outer.addWidget(subtitle)

        splitter = QSplitter(Qt.Horizontal)
        splitter.addWidget(self._build_form())
        splitter.addWidget(self._build_results())
        splitter.setStretchFactor(1, 1)
        outer.addWidget(splitter, 1)
        self.setCentralWidget(root)
        self.statusBar().showMessage("Ready")

    def _build_form(self) -> QWidget:
        panel = QWidget()
        layout = QVBoxLayout(panel)
        box = QGroupBox("Practice set settings")
        form = QFormLayout(box)

        self.topic = QComboBox()
        self.topic.setEditable(True)
        self.topic.addItems(["Chemical Bonding", "Photosynthesis", "Data Structures", "UPSC Polity"])
        self.topic.setCurrentText("Chemical Bonding")
        form.addRow("Topic", self.topic)

        self.difficulty = QComboBox()
        self.difficulty.addItems(DIFFICULTIES)
        self.difficulty.setCurrentText("medium")
        form.addRow("Difficulty", self.difficulty)

        self.problem_type = QComboBox()
        for value in PROBLEM_TYPES:
            self.problem_type.addItem(TYPE_LABELS.get(value, value), value)
        form.addRow("Question type", self.problem_type)

        self.count = QSpinBox()
        self.count.setRange(1, 30)
        self.count.setValue(5)
        form.addRow("Number of questions", self.count)

        self.depth = QComboBox()
        self.depth.addItems(["light", "medium", "deep"])
        self.depth.setCurrentText("medium")
        form.addRow("Research depth", self.depth)
        layout.addWidget(box)

        options = QGroupBox("Content options")
        option_layout = QVBoxLayout(options)
        self.hints = QCheckBox("Include hints")
        self.hints.setChecked(True)
        self.explanations = QCheckBox("Include explanations")
        self.explanations.setChecked(True)
        self.answers = QCheckBox("Show answers in results")
        self.answers.setChecked(True)
        self.cloud = QCheckBox("Prefer Groq cloud model")
        self.cloud.setChecked(bool(GROQ_API_KEY and GROQ_API_KEY != "your_groq_api_key_here"))
        for checkbox in (self.hints, self.explanations, self.answers, self.cloud):
            option_layout.addWidget(checkbox)
        layout.addWidget(options)

        self.generate_button = QPushButton("Generate practice set")
        self.generate_button.setObjectName("primary")
        self.generate_button.clicked.connect(self.generate)
        layout.addWidget(self.generate_button)
        layout.addStretch(1)
        return panel

    def _build_results(self) -> QWidget:
        panel = QWidget()
        layout = QVBoxLayout(panel)
        bar = QHBoxLayout()
        self.result_label = QLabel("Your generated questions will appear here")
        self.result_label.setObjectName("resultLabel")
        bar.addWidget(self.result_label, 1)
        self.answers_button = QPushButton("Hide answers")
        self.answers_button.setEnabled(False)
        self.answers_button.clicked.connect(self.toggle_answers)
        bar.addWidget(self.answers_button)
        layout.addLayout(bar)

        self.results = QTextBrowser()
        self.results.setOpenExternalLinks(False)
        self.results.setHtml("<p style='color:#b0bdd2'>Set your options on the left, then generate a new DPP.</p>")
        layout.addWidget(self.results, 1)

        export_bar = QHBoxLayout()
        self.export_buttons = []
        for label, fmt in (("Export Markdown", "markdown"), ("Export JSON", "json"), ("Export text", "text")):
            button = QPushButton(label)
            button.setEnabled(False)
            button.clicked.connect(lambda checked=False, f=fmt: self.export(f))
            self.export_buttons.append(button)
            export_bar.addWidget(button)
        layout.addLayout(export_bar)
        return panel

    def settings(self) -> dict:
        return {
            "topic": self.topic.currentText().strip(),
            "difficulty": self.difficulty.currentText(),
            "problem_type": self.problem_type.currentData(),
            "count": self.count.value(),
            "research_depth": self.depth.currentText(),
            "include_hints": self.hints.isChecked(),
            "include_explanations": self.explanations.isChecked(),
            "prefer_cloud": self.cloud.isChecked(),
        }

    @Slot()
    def generate(self) -> None:
        settings = self.settings()
        if not settings["topic"]:
            QMessageBox.warning(self, "Topic required", "Enter a topic before generating.")
            return
        self.generate_button.setEnabled(False)
        self.result_label.setText("Generating… research and model calls may take a moment")
        self.results.setHtml("<p style='color:#b0bdd2'>Working in the background…</p>")
        self.statusBar().showMessage("Generating practice set…")
        self.thread = QThread(self)
        self.worker = GenerationWorker(settings)
        self.worker.moveToThread(self.thread)
        self.thread.started.connect(self.worker.run)
        self.worker.finished.connect(self.on_finished)
        self.worker.failed.connect(self.on_failed)
        self.worker.finished.connect(self.thread.quit)
        self.worker.failed.connect(self.thread.quit)
        self.thread.finished.connect(self.thread.deleteLater)
        self.thread.start()

    @Slot(object)
    def on_finished(self, problem_set: ProblemSet) -> None:
        self.problem_set = problem_set
        self.result_label.setText(f"{len(problem_set.problems)} questions · {problem_set.total_marks} marks")
        self.results.setHtml(problem_set_html(problem_set, self.answers.isChecked()))
        self.answers_button.setEnabled(True)
        self.answers_button.setText("Hide answers" if self.answers.isChecked() else "Show answers")
        for button in self.export_buttons:
            button.setEnabled(bool(problem_set.problems))
        self.generate_button.setEnabled(True)
        self.statusBar().showMessage("Practice set ready")

    @Slot(str)
    def on_failed(self, message: str) -> None:
        self.generate_button.setEnabled(True)
        self.statusBar().showMessage("Generation failed")
        QMessageBox.critical(self, "Generation failed", message)

    def toggle_answers(self) -> None:
        if not self.problem_set:
            return
        visible = self.answers_button.text() == "Hide answers"
        self.results.setHtml(problem_set_html(self.problem_set, not visible))
        self.answers_button.setText("Show answers" if visible else "Hide answers")

    def export(self, fmt: str) -> None:
        if not self.problem_set:
            return
        exporters = {"markdown": export_markdown, "json": export_json, "text": export_text}
        path = exporters[fmt](self.problem_set, out_dir=OUTPUT_DIR)
        self.statusBar().showMessage(f"Saved {path}")
        QMessageBox.information(self, "Export complete", f"Saved to:\n{path}")


def run_gui() -> int:
    app = QApplication(sys.argv)
    app.setStyle("Fusion")
    app.setStyleSheet("""
      QWidget { color: #f1f5ff; background: #151a24; font-size: 13px; }
      QMainWindow, QStatusBar { background: #151a24; }
      #title { font-size: 28px; font-weight: 700; color: #9fc2ff; }
      #subtitle, #resultLabel { color: #b0bdd2; }
      QGroupBox { color: #e5ebf7; border: 1px solid #3a465c; border-radius: 8px; font-weight: 600; margin-top: 12px; padding-top: 12px; }
      QComboBox, QSpinBox, QLineEdit { color: #f1f5ff; background: #202938; border: 1px solid #4b5a72; border-radius: 5px; padding: 6px; selection-background-color: #315d9f; }
      QComboBox QAbstractItemView { color: #f1f5ff; background: #202938; selection-background-color: #315d9f; }
      QCheckBox { color: #e1e8f5; spacing: 7px; }
      QCheckBox:disabled, QPushButton:disabled { color: #718096; }
      QPushButton { color: #eaf1ff; background: #293449; border: 1px solid #4b5a72; border-radius: 6px; padding: 8px 12px; }
      QPushButton:hover { background: #34435e; }
      QPushButton#primary { color: #ffffff; background: #3569bd; border: 1px solid #5788d4; border-radius: 6px; font-weight: 600; }
      QPushButton#primary:hover { background: #447bd2; }
      QTextBrowser { color: #f1f5ff; background: #151a24; border: 1px solid #3a465c; border-radius: 8px; padding: 4px; }
      QScrollBar:vertical { background: #1d2431; width: 12px; margin: 0; }
      QScrollBar::handle:vertical { background: #53627a; border-radius: 5px; min-height: 24px; }
      QSplitter::handle { background: #303b50; }
    """)
    window = DPPWindow()
    window.show()
    return app.exec()
