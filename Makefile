all: map1 map2

map1: main1.o kdtree.o nearest.o utils.o 
	gcc -Wall -o map1 main1.o kdtree.o nearest.o utils.o -g -lm

map2: main2.o kdtree.o radius.o utils.o 
	gcc -Wall -o map2 main2.o kdtree.o radius.o utils.o -g -lm

main1.o: main1.c kdtree.h nearest.h utils.h radius.h
	gcc -Wall -c -o main1.o main1.c -lm

main2.o: main1.c kdtree.h nearest.h utils.h radius.h 
	gcc -Wall -c -o main2.o main2.c -lm

kdtree.o: kdtree.c kdtree.h nearest.h
	gcc -Wall -c -o kdtree.o kdtree.c -lm

nearest.o: nearest.c nearest.h 
	gcc -Wall -c -o nearest.o nearest.c -lm

radius.o: radius.c radius.h 
	gcc -Wall -c -o radius.o radius.c -lm

utils.o: utils.c utils.h
	gcc -Wall -c -o utils.o utils.c -lm
