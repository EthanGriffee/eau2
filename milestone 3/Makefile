build:
	g++ -std=c++11 -o tests/testdemo -pthread tests/testdemo.cpp
	g++ -std=c++11 -o tests/testpseudonetwork tests/testpseudonetwork.cpp
	g++ -std=c++11 -o tests/testmap tests/testmap.cpp

test:
	@tests/testdemo > tests/test_outputs/testdemo_output.txt
	@tests/testpseudonetwork > tests/test_outputs/testpseudonetwork_output.txt
	@tests/testmap > tests/test_outputs/testmap_output.txt
	@cmp --silent tests/test_outputs/testdemo_output.txt tests/test_outputs/testdemo_expectedoutput.txt && echo 'DEMO TEST SUCCESS' || echo 'DEMO TEST FAILURE'
	@cmp --silent tests/test_outputs/testpseudonetwork_output.txt tests/test_outputs/testpseudonetwork_expectedoutput.txt && echo 'PSEUDO NETWORK TEST SUCCESS' || echo 'PSEUDO NETWORK TEST FAILURE'
	@cmp --silent tests/test_outputs/testmap_output.txt tests/test_outputs/testmap_expectedoutput.txt && echo 'MAP TEST SUCCESS' || echo 'MAP TEST FAILURE'

	@rm -f testdemo_output.txt
	@rm -f testpseudonetwork_output.txt
	@rm -f testmap_output.txt

all-build:
	@make build-milestone1
	@make build-milestone2
	@make build

all-tests:
	@make test-milestone1
	@make test-milestone2
	@make test

valgrind:
	valgrind --leak-check=yes tests/testdemo

build-milestone1:
	g++ -std=c++11 -o tests/testarray tests/testarraytemplate.cpp
	g++ -std=c++11 -o tests/testcolumn tests/testcolumn.cpp
	g++ -std=c++11 -o tests/testdataframe tests/testdataframe.cpp
	g++ -std=c++11 -o tests/testparseintodataframe tests/testparseintodataframe.cpp
	g++ -std=c++11 -o data/generate data/generate.cpp

test-milestone1:
	@data/generate data/small_datafile.txt 5
	@tests/testarray > tests/test_outputs/testarray_output.txt
	@tests/testcolumn > tests/test_outputs/testcolumn_output.txt
	@tests/testdataframe > tests/test_outputs/testdataframe_output.txt
	@tests/testparseintodataframe > tests/test_outputs/testparseintodataframe_output.txt
	@cmp --silent tests/test_outputs/testarray_output.txt tests/test_outputs/testarray_outputexpected.txt && echo 'ARRAY TEST SUCCESS' || echo 'ARRAY TEST FAILURE'
	@cmp --silent tests/test_outputs/testcolumn_output.txt tests/test_outputs/testcolumn_outputexpected.txt && echo 'COLUMN TEST SUCCESS' || echo 'COLUMN TEST FAILURE'
	@cmp --silent tests/test_outputs/testdataframe_output.txt tests/test_outputs/testdataframe_outputexpected.txt && echo 'DATAFRAME TEST SUCCESS' || echo 'DATAFRAME TEST FAILURE'
	@cmp --silent tests/test_outputs/testparseintodataframe_output.txt tests/test_outputs/testparseintodataframe_outputexpected.txt && echo 'PARSE DATAFRAME TEST SUCCESS' || echo 'PARSE DATAFRAME TEST FAILURE'

valgrind-milestone1:
	data/generate
	valgrind --leak-check=yes  tests/testarray
	valgrind --leak-check=yes  tests/testdataframe
	valgrind --leak-check=yes  tests/testcolumn
	valgrind --leak-check=yes  tests/testparseintodataframe

build-milestone2:
	g++ -std=c++11 -o tests/testserial tests/testserial.cpp
	g++ -std=c++11 -o tests/testtrivial -pthread tests/testtrivial.cpp


test-milestone2:
	@tests/testtrivial > testtrivial_outputexpected.txt
	@tests/testserial > testserial_outputexpected.txt

	@tests/testtrivial > tests/test_outputs/testtrivial_output.txt
	@tests/testserial > tests/test_outputs/testserial_output.txt
	@cmp --silent tests/test_outputs/testtrivial_output.txt tests/test_outputs/testtrivial_outputexpected.txt && echo 'TRIVIAL TEST SUCCESS' || echo 'TRIVIAL TEST FAILURE'
	@cmp --silent tests/test_outputs/testserial_output.txt tests/test_outputs/testserial_outputexpected.txt && echo 'SERIAL TEST SUCCESS' || echo 'SERIAL TEST FAILURE'

valgrind-milestone2:
	valgrind --leak-check=yes tests/testtrivial
	valgrind --leak-check=yes tests/testserial