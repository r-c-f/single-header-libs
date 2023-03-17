all: humansize sopt_arg

sopt_arg: sopt_arg.c ../sopt.h
	$(CC) $(CFLAGS) sopt_arg.c -o sopt_arg

humansize: humansize.c ../humansize.h
	$(CC) $(CFLAGS) -lm humansize.c -o humansize

clean:
	rm sopt_arg humansize
