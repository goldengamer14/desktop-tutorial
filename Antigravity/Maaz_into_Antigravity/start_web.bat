@echo off
title Qwen Desktop Controller (Web Dashboard)
chcp 65001 >nul
cd /d "%~dp0"
echo =======================================================
echo Starting Qwen Desktop Controller Web Server...
echo Model: qwen3:1.7b (via Ollama)
echo Dashboard: http://127.0.0.1:8088
echo =======================================================
python web_server.py
pause
