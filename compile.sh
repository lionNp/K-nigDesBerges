#!/bin/bash

files=()

for file in ./src/*.c; do
  if [ -f "$file" ]; then
    files+=("$file")
  fi
done

gcc "${files[@]}" -o build/run -Isrc/header -lm -lpthread