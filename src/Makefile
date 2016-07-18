CC = clang++ -std=c++11
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
LIBS = -L/opt/local/lib -lboost_serialization-mt -lgtest -lpthread
INCLUDES = -I/opt/local/include
BUILD_OBJS = build_kdtree_main.o build_kdtree.o util.o
QUERY_OBJS = query_kdtree_main.o query_kdtree.o util.o
TEST_OBJS = test_kdtree.o

all: query build test

#kdtree.o: kdtree.h
#	$(CC) $(CFLAGS) $(INCLUDES) kdtree.h

build_kdtree.o: build_kdtree.cpp build_kdtree.h kdtree.h
	$(CC) $(CFLAGS) $(INCLUDES) build_kdtree.cpp

build_kdtree_main.o: build_kdtree_main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) build_kdtree_main.cpp

util.o: util.cpp
	$(CC) $(CFLAGS) $(INCLUDES) util.cpp
	
build: $(BUILD_OBJS)
	$(CC) $(LFLAGS) $(LIBS) $(BUILD_OBJS) -o build

query_kdtree.o: query_kdtree.cpp query_kdtree.h kdtree.h
	$(CC) $(CFLAGS) $(INCLUDES) query_kdtree.cpp

query_kdtree_main.o: query_kdtree_main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) query_kdtree_main.cpp

query: $(QUERY_OBJS)
	$(CC) $(LFLAGS) $(LIBS) $(QUERY_OBJS) -o query
	
test_kdtree.o: test_kdtree.cpp kdtree.h
	$(CC) $(CFLAGS) $(INCLUDES) test_kdtree.cpp kdtree.h

test: $(TEST_OBJS)
	$(CC) $(LFLAGS) $(LIBS) $(TEST_OBJS) -o test

clean:
	\rm *.o build query test