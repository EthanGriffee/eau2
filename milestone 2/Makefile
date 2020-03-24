build:
	g++ -std=c++11 -o tests/testserial tests/testserial.cpp
	g++ -std=c++11 -o tests/testtrivial tests/testtrivial.cpp


test:
	tests/testtrivial
	tests/testserial

valgrind:
	data/generate
	valgrind --leak-check=yes tests/testtrivial
	valgrind --leak-check=yes tests/testserial