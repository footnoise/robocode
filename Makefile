prog: src/robocode.o
	g++ -o robocode src/robocode.o
clean:
	rm -f robocode
	rm -f src/*.o
	rm -f *~