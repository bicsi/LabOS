make:
	gcc D5.c -o rec_rem
	gcc F6.c -o dec2bin
	gcc F7.c -o bin2dec
	gcc matrix_dump.c -o mat_dump
	gcc matrix_gen.c -o mat_gen
	gcc sum.c -o mat_sum
	gcc mul.c -o mat_mul
clean:
	find . -type f -executable -exec rm '{}' \;