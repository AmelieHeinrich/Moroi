@echo off
REM
REM $Notice: Xander Studios @ 2024
REM $Author: Amélie Heinrich
REM $Create Time: 2024-10-16 22:09:03
REM

if not exist run_tree (
    mkdir run_tree
)

set rootDir=%cd%
set source=%rootDir%/src/main.cpp
set flags=-sUSE_GLFW=3 -sUSE_WEBGPU=1 -sASYNCIFY
set output=Moroi.html

pushd run_tree
call em++ %source% %flags% -o %output%
popd
