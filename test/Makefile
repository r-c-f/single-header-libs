all: humansize sopt_arg

sopt_arg: sopt_arg.c ../sopt.h
	$(CC) sopt_arg.c -o sopt_arg

humansize: humansize.c ../humansize.h
	$(CC) -lm humansize.c -o humansize

clean:
	rm sopt_arg humansize
