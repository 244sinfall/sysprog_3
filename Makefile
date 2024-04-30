run_test:
	gcc -o test test_company.c company.c -lcunit && ./test && rm test

run_cli_mem:
	gcc -o main main.c company.c && ./main && rm main