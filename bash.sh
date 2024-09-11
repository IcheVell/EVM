#!/bin/bash
make lab

(time ./final) >> text.txt 2>&1
(time ./final_with_o_zero) >> text.txt 2>&1
(time ./final_with_o_one) >> text.txt 2>&1
(time ./final_with_o_two) >> text.txt 2>&1
(time ./final_with_o_three) >> text.txt 2>&1
(time ./final_with_o_s) >> text.txt 2>&1
(time ./final_with_o_fast) >> text.txt 2>&1
(time ./final_with_o_g) >> text.txt 2>&1
