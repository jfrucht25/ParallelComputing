mpicc -o parametersearch parametersearch.c -lm
mpirun -np 20 -machinefile hosts.txt parametersearch
