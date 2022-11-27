#!/bin/bash

rm -f data_implicit.txt
rm -f data_constrained.txt

g++ main.cpp -o simulator

for u in $(seq 0.03 0.03 1.0); do
echo $u >> data_implicit.txt
python task_generator.py --u $u --nsets 5000 --deadline implicit > task_parameters.txt
./simulator < task_parameters.txt >> data_implicit.txt
done
rm task_parameters.txt
python make_graph.py data_implicit.txt


for u in $(seq 0.03 0.03 1.0); do
echo $u >> data_constrained.txt
python task_generator.py --u $u --nsets 5000 --deadline constrained > task_parameters.txt
./simulator < task_parameters.txt >> data_constrained.txt
done
rm task_parameters.txt
python make_graph.py data_constrained.txt

rm -f data_implicit.txt
rm -f data_constrained.txt