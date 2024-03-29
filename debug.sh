#!/bin/bash

function compile_exec {
    gcc "${files[@]}" -o "$output" -Isrc/header -lm -lpthread -fgnu89-inline -g
    gdb "./$output"
    exit 0
}

files=()

if [ $# == 0 ] || [ "$1" == "main" ]; then

  output="build/main_debug"

  for file in ./src/*.c; do
    if [ -f "$file" ]; then
      files+=("$file")
    fi
  done

  compile_exec
fi

if ! [ -f "./src/unit_tests/$1.c" ]; then
  echo "Error: src/test/ does not contain a file named: '$1'.c"
  exit 1
fi

for file in ./src/*.c; do
  if [ -f "$file" ] && [ "$file" != "./src/main.c" ]; then
    files+=("$file")
  fi
done
files+=("./src/unit_tests/$1.c")

output="build/$1_debug"

compile_exec