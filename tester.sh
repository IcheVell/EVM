#!/bin/bash

echo "Without vectorization" 

for i in {1..15}; do
    ./without random <<EOF
580
10
EOF
done

echo "Manual vectorization" 

for i in {1..15}; do
    ./man_vectorized random <<EOF
580
10
EOF
done

echo "Blas" 

for i in {1..15}; do
    ./lib random <<EOF
580
10
EOF
done

