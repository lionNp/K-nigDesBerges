#!/bin/bash

gcc "src/test/eval_weights_training.c" -o "build/learn" -Isrc/header -lm -lpthread -fgnu89-inline
"./build/learn"
exit 0
