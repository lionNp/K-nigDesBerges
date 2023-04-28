#!/bin/bash

files=()

if [ $# == 0 ] || [ "$1" == "main" ]; then

  for file in ./src/*.c; do
    if [ -f "$file" ]; then
      files+=("$file")
    fi
  done

  gcc "${files[@]}" -o build/main -Isrc/header -lm -lpthread -fgnu89-inline
  ./build/main
  exit 0
fi

if ! [ -f "./src/test/$1.c" ]; then
  echo "Error: src/test/ does not contain a file named: '$1'.c"
  exit 1
fi

for file in ./src/*.c; do
  if [ -f "$file" ] && [ "$file" != "./src/main.c" ]; then
    files+=("$file")
  fi
done
files+=("./src/test/$1.c")

output="build/$1"

gcc "${files[@]}" -o "$output" -Isrc/header -lm -lpthread -fgnu89-inline
"./build/$1"
exit 0