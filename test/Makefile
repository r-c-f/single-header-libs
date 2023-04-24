all: humansize sopt_arg ssb ssb_bench

ssb: ssb.c ../humansize.h ../ssb.h
	$(CC) $(CFLAGS) ssb.c -o ssb

ssb_bench: ssb_bench.c ../humansize.h ../ssb.h
	$(CC) $(CFLAGS) ssb_bench.c -o ssb_bench


sopt_arg: sopt_arg.c ../sopt.h
	$(CC) $(CFLAGS) sopt_arg.c -o sopt_arg

humansize: humansize.c ../humansize.h
	$(CC) $(CFLAGS) -lm humansize.c -o humansize

clean:
	rm -f sopt_arg humansize ssb ssb_bench
