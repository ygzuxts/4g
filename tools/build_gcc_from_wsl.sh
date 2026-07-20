#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TMP="${GCC_BUILD_TMP:-/mnt/c/Users/2y0z0x4/AppData/Local/Temp/4g-module-gcc-build}"
JOBS="${JOBS:-$(nproc)}"

rm -rf "$TMP"
mkdir -p "$TMP"

rsync -a \
  --exclude='.git' \
  --exclude='build' \
  "$ROOT"/ "$TMP"/

(
  cd "$TMP"
  make clean
  make -j"$JOBS"
)

rm -rf "$ROOT/build/gcc"
mkdir -p "$ROOT/build" "$ROOT/output"
rsync -a "$TMP/build/gcc/" "$ROOT/build/gcc/"

cp "$TMP/build/gcc/4GCatModule.elf" "$ROOT/output/4GCatModule.elf"
cp "$TMP/build/gcc/4GCatModule.hex" "$ROOT/output/4GCatModule.hex"
cp "$TMP/build/gcc/4GCatModule.bin" "$ROOT/output/4GCatModule.bin"
cp "$TMP/build/gcc/4GCatModule.map" "$ROOT/output/4GCatModule.map"

echo "Built:"
echo "  $ROOT/output/4GCatModule.hex"
echo "  $ROOT/output/4GCatModule.bin"
