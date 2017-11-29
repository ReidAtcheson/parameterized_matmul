

for t0 in 2 4 8 16 32 64
do
  for t1 in 2 4 8 16 32 64
  do
    for t2 in 2 4 8 16 32 64
    do
      mpiicpc -std=c++11 -O3 -DTILE0=$t0 -DTILE1=$t1 -DTILE2=$t2 matmul.cpp -o matmul;
      c=`mpirun -np 24 ./matmul 512`
      printf "%d,%d,%d,%f\n" $t0 $t1 $t2 $c
    done
  done
done
