#!/bin/zsh
echo -n "${(pl:16::\0:)}$(cat input.txt | base64 -d)" | aes128_cbc -d 'YELLOW SUBMARINE'
