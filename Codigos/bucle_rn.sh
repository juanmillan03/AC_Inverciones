

#!/bin/bash

L=40

# Generar combinaciones con parallel
parallel -j 2 ./main_random $L {1} {2} ::: $(seq 0.0 0.02 1.0) ::: $(seq 0.0 0.02 1.0)
