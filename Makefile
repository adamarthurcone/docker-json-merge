CC=g++ -std=c++11
JSON=-Ivendor/json/include
FLUENTD=-Ivendor/fluentd/include -Lvendor/fluentd/lib -lfluent
MSGPACK=-Ivendor/msgpack/include -Lvendor/msgpack/lib -lmsgpackc
STD_LIBS=-ldl -lpthread

main: main.cpp fluentd-logging.o
	$(CC) -O1 -o main main.cpp fluentd-logging.o -Iinclude $(JSON) $(MSGPACK) $(FLUENTD) $(STD_LIBS) -static

fluentd-logging.o: src/fluentd-logging.cpp include/fluentd-logging.hpp
	$(CC) -c src/fluentd-logging.cpp -Iinclude $(FLUENTD) $(MSGPACK) $(JSON) -static

clean:
	rm -f main *.o *.out *.json