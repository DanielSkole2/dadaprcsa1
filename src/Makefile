CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=gnu99 -g 
LDFLAGS=-lm
PROGRAMS=random_ids id_query_naive coord_query_naive id_query_indexed id_query_binsort coord_query_kdtree 
TESTS=Test_files/coord_query_naive_test Test_files/coord_query_kdtree_test Test_files/id_query_binsort_test Test_files/id_query_indexed_test Test_files/id_query_naive_test

.PHONY: all test clean ../src.zip

all: $(PROGRAMS)

random_ids: random_ids.o record.o
	gcc -o $@ $^ $(LDFLAGS)

id_query_%: id_query_%.o record.o id_query.o 
	gcc -o $@ $^ $(LDFLAGS)

coord_query_%: coord_query_%.o record.o coord_query.o
	gcc -o $@ $^ $(LDFLAGS)

id_query.o: id_query.c
	$(CC) -c $< $(CFLAGS)

coord_query.o: coord_query.c
	$(CC) -c $< $(CFLAGS)

record.o: record.c
	$(CC) -c $< $(CFLAGS)

sort.o: sort.c
	$(CC) -c $< $(CFLAGS)

coord_query_%_nomains.o: coord_query_%.c
	$(CC) -c $< $(CFLAGS) -DCOORD_QUERY_NO_MAIN -o $@

id_query_%_nomains.o: id_query_%.c
	$(CC) -c $< $(CFLAGS) -DID_QUERY_NO_MAIN -o $@


Test_files/coord_query_%_test: Test_files/coord_query_%_test.o record.o coord_query.o coord_query_%_nomains.o
	$(CC) -o $@ $^ $(LDFLAGS)

Test_files/id_query_%_test: Test_files/id_query_%_test.o record.o id_query.o id_query_%_nomains.o
	$(CC) -o $@ $^ $(LDFLAGS)

test: $(TESTS)
	@set e; for test in $(TESTS); do echo ./$$test; ./$$test; done

clean:
	rm -rf core *.o $(PROGRAMS)
	rm -rf core Test_files/*.o $(TESTS)

planet-latest-geonames.tsv:
	wget https://github.com/OSMNames/OSMNames/releases/download/v2.0.4/planet-latest_geonames.tsv.gz
	gunzip planet-latest_geonames.tsv.gz

../src.zip:
	make clean
	cd .. && zip src.zip -r src

.SECONDARY:
