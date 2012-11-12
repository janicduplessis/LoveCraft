#!/bin/bash

SAVEIFS=$IFS
IFS=$(echo -en "\n\b")
for f in $(git ls-files -i -X .gitignore)
do
git rm --cached "$f"
done
IFS=$SAVEIFS