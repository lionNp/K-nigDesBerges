#!/bin/bash

gcc "src/test/weights_training.c" -o "build/learn" -Isrc/header -lm -lpthread -fgnu89-inline
"./build/learn"
exit 0

