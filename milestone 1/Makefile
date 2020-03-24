build:
	g++ -std=c++11 -o tests/testarray tests/testarraytemplate.cpp
	g++ -std=c++11 -o tests/testcolumn tests/testcolumn.cpp
	g++ -std=c++11 -o tests/testdataframe tests/testdataframe.cpp
	g++ -std=c++11 -o tests/testparseintodataframe tests/testparseintodataframe.cpp
	g++ -std=c++11 -o data/generate data/generate.cpp


test:
	data/generate data/small_datafile.txt 5
	tests/testarray
	tests/testcolumn
	tests/testdataframe
	tests/testparseintodataframe

valgrind:
	data/generate
	valgrind tests/testarray
	valgrind tests/testdataframe
	valgrind tests/testcolumn
	valgrind tests/testparseintodataframe