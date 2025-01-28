for L in 40 ; do
    for P1 in 0.1; do
        for P2 in $(seq 0.0 0.1 1.0); do
            echo "Ejecutando con L=$L, P1=$P1, P2=$P2"
            ./main $L $P1 $P2
        done
    done
done