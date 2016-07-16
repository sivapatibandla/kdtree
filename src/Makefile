CC = clang++ -std=c++11
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
LIBS = -L/opt/local/lib -lboost_serialization-mt
INCLUDES = -I/opt/local/include
BUILD_OBJS = build_kdtree_main.o build_kdtree.o util.o
QUERY_OBJS = query_kdtree_main.o query_kdtree.o util.o

#TEST_LFLAGS = -lpthread -lgtest
#TARGET_OBJS = query_kdtree.o query_kdtree_main.o build_kdtree.o build_kdtree_main.o
#TEST_OBJS = test_query_kdtree.o learn_gtest.o

all: query_kdtree build_kdtree

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

clean:
	\rm *.o build query