@echo off
REM
REM $Notice: Xander Studios @ 2024
REM $Author: Amélie Heinrich
REM $Create Time: 2024-10-16 22:19:31
REM

python -m http.server -d run_tree
start http://localhost:8000/Moroi.html
