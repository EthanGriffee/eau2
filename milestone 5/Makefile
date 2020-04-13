build:
	g++ -std=c++11 -o tests/testworddemo -pthread tests/testworddemo.cpp

test:
	./tests/testworddemo

build-network:
	g++ -std=c++11 -o tests/testnetworkip -pthread tests/testnetworkip.cpp 
	g++ -std=c++11 -o tests/testlinusip -pthread tests/testlinusip.cpp 

test-linus:
	./tests/testlinusip -node-number 0 -num-nodes 3 -serverip 127.0.0.1 -serverport 8081 -port 8081 &
	./tests/testlinusip -node-number 1 -num-nodes 3 -serverip 127.0.0.1 -serverport 8081 -port 8327  &
	./tests/testlinusip -node-number 2 -num-nodes 3 -serverip 127.0.0.1 -serverport 8081 -port 8023  

test-network:
	./tests/testnetworkip -node-number 0 -num-nodes 3 -serverip 127.0.0.1 -serverport 8081 -port 8081 &
	./tests/testnetworkip -node-number 1 -num-nodes 3 -serverip 127.0.0.1 -serverport 8081 -port 8327  &
	./tests/testnetworkip -node-number 2 -num-nodes 3 -serverip 127.0.0.1 -serverport 8081 -port 8023  

all-build:
	@make build-milestone1
	@make build-milestone2
	@make build-milestone3

all-test:
	@make test-milestone1
	@make test-milestone2
	@make test-milestone3

valgrind:
	valgrind --leak-check=yes tests/testworddemo

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

	@rm -f tests/test_outputs/testarray_output.txt
	@rm -f tests/test_outputs/testcolumn_output.txt
	@rm -f tests/test_outputs/testcolumn_output.txt
	@rm -f tests/test_outputs/testdataframe_output.txt
	
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
	@tests/testtrivial > tests/test_outputs/testtrivial_output.txt
	@tests/testserial > tests/test_outputs/testserial_output.txt
	@cmp --silent tests/test_outputs/testtrivial_output.txt tests/test_outputs/testtrivial_outputexpected.txt && echo 'TRIVIAL TEST SUCCESS' || echo 'TRIVIAL TEST FAILURE'
	@cmp --silent tests/test_outputs/testserial_output.txt tests/test_outputs/testserial_outputexpected.txt && echo 'SERIAL TEST SUCCESS' || echo 'SERIAL TEST FAILURE'

	@rm -f tests/test_outputs/testtrivial_output.txt
	@rm -f tests/test_outputs/testserial_output.txt

valgrind-milestone2:
	valgrind --leak-check=yes tests/testtrivial
	valgrind --leak-check=yes tests/testserial


build-milestone3:
	g++ -std=c++11 -o tests/testdemo -pthread tests/testdemo.cpp
	g++ -std=c++11 -o tests/testpseudonetwork tests/testpseudonetwork.cpp

test-milestone3:
	@tests/testdemo > tests/test_outputs/testdemo_output.txt
	@tests/testpseudonetwork > tests/test_outputs/testpseudonetwork_output.txt
	@cmp --silent tests/test_outputs/testdemo_output.txt tests/test_outputs/testdemo_expectedoutput.txt && echo 'DEMO TEST SUCCESS' || echo 'DEMO TEST FAILURE'
	@cmp --silent tests/test_outputs/testpseudonetwork_output.txt tests/test_outputs/testpseudonetwork_expectedoutput.txt && echo 'PSEUDO NETWORK TEST SUCCESS' || echo 'PSEUDO NETWORK TEST FAILURE'

	@rm -f tests/test_outputs/testdemo_output.txt
	@rm -f tests/test_outputs/testpseudonetwork_output.txt