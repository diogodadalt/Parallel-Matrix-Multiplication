main: 
	gcc -o main matrix_multi_unix_process.c matrix.c read_matrix.c write_matrix.c shared_matrix.c metrics.c
	
clean:
	rm main

post-clean:
	clear	
	
run: clean main post-clean
	./main $(n)
	
debug: clean
	gcc -g matrix_multi_unix_process.c matrix.c read_matrix.c write_matrix.c shared_matrix.c metrics.c -o main
