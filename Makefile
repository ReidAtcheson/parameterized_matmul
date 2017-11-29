CPP := mpiicpc
CFLAGS := -O3 --std=c++11
TUNEPARAMS := -DTILE0=32 -DTILE1=128 -DTILE2=4


matmul : matmul.cpp
	$(CPP) $(CFLAGS) $(TUNEPARAMS) matmul.cpp -o matmul




.PHONY : clean




clean :
	rm -rf ./matmul
	rm -rf ./*.optrpt
