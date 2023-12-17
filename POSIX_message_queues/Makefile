all:	recv sender

sender:	sender.cpp
	g++ sender.cpp -o sender -lrt

recv:	recv.cpp
	g++ recv.cpp -o recv -lrt

clean:
	rm -rf sender recv
