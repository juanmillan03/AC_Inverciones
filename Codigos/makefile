compile:
	g++ -O3 main.cpp -o main
compile_rand:
	g++ -O3 main_random.cpp -o main_random
compile_rand2:
	g++ -O3 main_random2.cpp -o main_random
compile_sw:
	g++ -O3 main_Sw.cpp -o main_sw


run: main
	./main $(arg) $(arg1) $(arg2) > datos.txt
clean:
	rm datos_t_inter.txt
for: compile
	chmod +x bucle.sh
	bash bucle.sh
for_rand: compile_rand
	chmod +x bucle_rn.sh
	bash bucle_rn.sh
for_rand2: compile_rand2
	chmod +x bucle_rn.sh
	bash bucle_rn.sh
for_sw: compile_sw
	chmod +x bucle_sw.sh
	bash bucle_sw.sh
