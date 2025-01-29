for L in 40  ; do
    for P1 in 0.2; do
        for P2 in 0.8; do
            echo "Ejecutando con L=$L, P1=$P1, P2=$P2"
            ./main $L $P1 $P2
        done
    done
done