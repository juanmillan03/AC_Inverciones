for L in 10 20 30 40 60; do
    for P1 in $(seq 0.0 0.02 1.0); do
        for P2 in $(seq 0.0 0.02 1.0); do
            echo "Ejecutando con L=$L, P1=$P1, P2=$P2"
            ./main $L $P1 $P2
        done
    done
done