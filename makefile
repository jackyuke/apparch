CLFLAGS = -g -Wall -lcppunit -ldl -lm
rbtree : main.cpp rbtree.h
	g++  main.cpp $(CLFLAGS) -o rbtree 

clean:
	rm rbtree 
