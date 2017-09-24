CLFLAGS = -g -Wall -lcppunit -ldl -lm
rbtree : main.cpp rbtree.hxx
	g++  main.cpp $(CLFLAGS) -o rbtree 

clean:
	rm rbtree 
