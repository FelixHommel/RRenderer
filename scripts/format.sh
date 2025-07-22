#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

find $SCRIPT_DIR/../src -regex '.*\.\(cpp\|hpp\)' -exec clang-format-19 -i {} +
