main:
  gcc -o main matrix_multi_unix_process.c matrix.c read_matrix.c write_matrix.c
	
clean:
  rm main

post-clean:
	clear	
	
run: clean main post-clean
  ./main $(n)
	
