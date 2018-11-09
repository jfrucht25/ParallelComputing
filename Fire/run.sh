#!/bin/bash
rm data.txt
for i in {2..35..2};
do
	echo $i
	/usr/bin/time -f "%U" --append --output="data.txt" mpirun -np $i -machinefile hosts.txt mpifire
done
