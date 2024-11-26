all: clean build

build: build_man build_vect build_lib

build_man: without_vectorized.c without_vectorized_func.c
	gcc -O1 -march=native without_vectorized.c without_vectorized_func.c  -o without

build_vect: man_vectorized.c man_vectorized_func.c
	gcc -O1 -march=native -ftree-vectorize -fopt-info-vec-optimized -o man_vectorized man_vectorized.c man_vectorized_func.c -lm

build_lib: lib.c lib_func.c
	gcc -O1 lib.c lib_func.c -o lib -lblas

clean:
	rm -rf without man_vectorized lib