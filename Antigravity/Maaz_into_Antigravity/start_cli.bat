@echo off
title Qwen Desktop Controller (CLI)
chcp 65001 >nul
cd /d "%~dp0"
echo =======================================================
echo Starting Qwen Desktop Controller CLI...
echo Model: qwen3:1.7b (via Ollama)
echo =======================================================
python cli_assistant.py
pause
