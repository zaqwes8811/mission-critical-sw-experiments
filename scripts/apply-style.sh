#!/bin/sh
set -e

DIRS=("tests/" "apps/")

for DIR in "${DIRS[@]}"
do
    find $DIR -iname "*.hpp" -o -iname "*.cpp" -o -iname "*.cc" -o -iname "*.c" -o -iname "*.h"  | xargs clang-format -i
done
