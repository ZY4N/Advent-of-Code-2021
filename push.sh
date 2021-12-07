#!/bin/bash
curl https://adventofcode.com/2021/day/$1 --output raw.html --cookie session=$AOC2021_SESSION
./common/testExtractor raw.html day$1/README.MD "article class=\"day-desc\"" "**</p>"
rm raw.html
git add -A
git commit -m "added day$1"
git push
