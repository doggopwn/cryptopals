#!/bin/zsh
byline 'echo $line | detect_aes128_ecb -s && echo "$lineidx: $line"' input.txt
