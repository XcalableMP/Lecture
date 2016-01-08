#!/bin/bash
#SBATCH -p d024h
#SBATCH -t 0:01:00
#SBATCH -n 2
#SBATCH -J test_openmpi
#SBATCH -o stdout.%J
#SBATCH -e stderr.%J
module load gnu/openmpi165
mpirun ./a.out
