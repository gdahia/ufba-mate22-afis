#!/bin/bash

for (( i = 1; i <= 50; i++ )); do
	for (( j = 1; j <= 5; j++ )); do
		./extract_minutiae.out /home/gdahia/Documents/digital/DIGITAL/$i\_$j.jpg result/$i\_$j.txt
	done
done
