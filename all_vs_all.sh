#!/bin/bash

for (( i = 1; i <= 50; i++ )); do
	for (( j = 1; j <= 5; j++ )); do
		for (( k = i; k <= 50; k++ )); do
		    for (( l = 1; l <= 5; l++ )); do
                if [[ i -ne k ]] || [[ j -ne l ]]; then
		              echo $i $j $k $l
                      ./recognize.out $i\_$j $k\_$l
                fi
		    done
		done
	done
done
