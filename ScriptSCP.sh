#!/bin/bash

for P in `seq 1 5`;
do
    echo "Executando teste${P}"
    ./SetCoveringProblem casos/teste${P}.txt 1000 0.05 2.0 sup > logs/teste${P}.log1 &
    ./SetCoveringProblem casos/teste${P}.txt 100 0.5 10.0 sup > logs/teste${P}.log2 &
    ./SetCoveringProblem casos/teste${P}.txt 1000 0.5 10.0 sup > logs/teste${P}.log3 &
    ./SetCoveringProblem casos/teste${P}.txt 500 0.25 10.0 sup > logs/teste${P}.log4 &
    wait
done

echo "Execucao finalizada"

