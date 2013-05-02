proc: 
	gcc -o main matrix_multi_unix_process.c matrix.c read_matrix.c write_matrix.c shared_matrix.c metrics.c error_handling.c

thread:
	gcc -o main matrix_multi_pthreads.c matrix.c read_matrix.c write_matrix.c shared_matrix.c metrics.c error_handling.c -lpthread
	
clean:
	@rm main || true

post-clean:
	clear	
	
run_proc: clean proc post-clean
	./main $(n)
	
run_thread: clean thread post-clean
	./main $(n)
	
debug: clean
	gcc -g matrix_multi_unix_process.c matrix.c read_matrix.c write_matrix.c shared_matrix.c metrics.c -o main
