#!/bin/bash
clean=$2
direc=$1
for f in "$direc"/*.c
do
	# echo $f
	name=$(basename $f)
	echo $name
	./sclp -d "$f" > "$f".my.s 2> /dev/null
	./sclpravi -d "$f" > "$f".ref.s 2> /dev/null
	spim -file "$f".my.s > "$f".my.out
	spim -file "$f".ref.s > "$f".ref.out
	diff "$f".my.out "$f".ref.out
done
if [ "$clean" == "c" ]
	then
	rm "$direc"/*.error.*
	rm "$direc"/*.ic
	rm "$direc"/*.s
	rm "$direc"/*.out
fi