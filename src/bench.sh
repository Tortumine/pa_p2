#!/bin/bash
for ((n=0;n<$2;n++))
do
    ./test -s $1 -f ../bench_res/bench$1.txt
done
