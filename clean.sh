#!/bin/bash


for FILE in $(find ./ -type f -name "*.c" -or \
	-name "*.cpp" -or -name "*.h" -or \( -name "semestralwork" -prune \))	
do
	uncrustify -c uncrustify.cfg  --no-backup ${FILE};
done



