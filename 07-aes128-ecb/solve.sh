#!/bin/zsh
cat input.txt | base64 -d | aes128_ecb -d 'YELLOW SUBMARINE'
