#!/usr/bin/env bash
set -euo pipefail

root=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
output=${1:-"$root/dark-messiah-community-loader.exe"}
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

llvm-dlltool -m i386 -k -d "$root/kernel32.def" -l "$work/kernel32.lib"
llvm-dlltool -m i386 -k -d "$root/user32.def" -l "$work/user32.lib"
clang --target=i686-w64-windows-gnu -Os -ffreestanding -fno-stack-protector \
    -fno-asynchronous-unwind-tables -c "$root/community-loader.c" -o "$work/community-loader.obj"
lld-link /machine:x86 /subsystem:windows /entry:mainCRTStartup /nodefaultlib \
    "/out:$output" "$work/community-loader.obj" "$work/kernel32.lib" "$work/user32.lib"
sha256sum "$output"
