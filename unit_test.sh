#!/bin/bash

function compile_exec {
    gcc "${files[@]}" -o "$output" -Isrc/header -lm -lpthread -fgnu89-inline
    "./$output"
    exit 0
}

files=()

if [ $# == 0 ]; then
    echo Error: please specify your target: ./unit_test.sh \<target\>
    exit 1
fi

target_file="$1_unit_test"

if ! [ -f "./src/unit_tests/$target_file.c" ]; then
  echo "Error: no unit test found named: '$target_file'.c"
  exit 1
fi

for file in ./src/*.c; do
  if [ -f "$file" ] && [ "$file" != "./src/main.c" ]; then
    files+=("$file")
  fi
done
files+=("./src/unit_tests/$target_file.c")
files+=("./src/unit_tests/assertions.c")

output="build/$target_file"

compile_exec