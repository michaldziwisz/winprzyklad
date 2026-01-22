#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"

mkdir -p "${BUILD_DIR}"

TMP_DOCKER_CONFIG="$(mktemp -d)"
trap 'rm -rf "${TMP_DOCKER_CONFIG}"' EXIT

DOCKER_CONFIG="${TMP_DOCKER_CONFIG}" docker run --rm \
  -v "${ROOT_DIR}:/src" \
  -w /src \
  ubuntu:24.04 bash -lc '
    set -euo pipefail
    apt-get update
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends mingw-w64 make ca-certificates
    ./scripts/build-win-mingw.sh
  '

echo "OK: ${BUILD_DIR}/DostepneKontrolki.exe"
