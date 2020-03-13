build:
	g++ -std=c++11 -o tests/testarray tests/testarraytemplate.cpp
	g++ -std=c++11 -o tests/testdataframe tests/testdataframe.cpp
	g++ -std=c++11 -o data/generate data/generate.cpp


test:
	data/generate
	mv ./datafile.txt ./data/datafile.txt
	tests/testarray
	tests/testdataframe

valg:
	data/generate
	mv ./datafile.txt ./data/datafile.txt
	valgrind tests/testarray
	valgrind tests/testdataframe