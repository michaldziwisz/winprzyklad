#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"

mkdir -p "${BUILD_DIR}"

x86_64-w64-mingw32-windres "${ROOT_DIR}/resources/app.rc" -O coff -o "${BUILD_DIR}/app_res.o"

x86_64-w64-mingw32-g++ -std=c++17 -O2 -Wall -Wextra -municode -mwindows \
  -DUNICODE -D_UNICODE \
  "${ROOT_DIR}/src/main.cpp" "${BUILD_DIR}/app_res.o" \
  -static-libgcc -static-libstdc++ \
  -lcomctl32 -luxtheme \
  -o "${BUILD_DIR}/DostepneKontrolki.exe"

echo "OK: ${BUILD_DIR}/DostepneKontrolki.exe"

