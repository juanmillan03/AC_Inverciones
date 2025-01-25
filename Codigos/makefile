compile:
	g++ -O3 main.cpp -o main

run: main
	./main $(arg) $(arg1) $(arg2) > datos.txt
clean:
	rm datos_t_inter.txt
for: compile
	chmod +x bucle.sh
	bash bucle.sh
