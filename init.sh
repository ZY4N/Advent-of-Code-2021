#!/bin/bash
mkdir day$1
cp -R ./common/template/* ./day$1
cp -R ./common/template/.vscode ./day$1
curl https://adventofcode.com/2021/day/$1 --output raw.html
./common/testExtractor raw.html day$1/testInput.txt
rm raw.html
curl https://adventofcode.com/2021/day/$1/input -o ./day$1/input.txt --cookie session=$AOC2021_SESSION
cd ./day$1
code -n .
