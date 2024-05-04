run_test:
	gcc -o test test_company.c company.c -lcunit && ./test && rm test

run_cli_mem:
	gcc -o main main.c company.c && ./main && rm main

run_cli_file:
	gcc -o main main.c company.c company_file.c && ./main ./test && rm main

build_cli:
	gcc -o main main.c company.c company_file.c