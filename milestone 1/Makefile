build:
	g++ -std=c++11 -o tests/testarray tests/testarraytemplate.cpp
	g++ -std=c++11 -o tests/testcolumn tests/testcolumn.cpp
	g++ -std=c++11 -o tests/testparseintodataframe tests/testparseintodataframe.cpp
	g++ -std=c++11 -o data/generate data/generate.cpp


test:
	data/generate
	mv ./datafile.txt ./data/datafile.txt
	tests/testarray
	tests/testcolumn
	tests/testdataframe

valg:
	data/generate
	mv ./datafile.txt ./data/datafile.txt
	valgrind tests/testarray
	valgrind tests/testparseintodataframe