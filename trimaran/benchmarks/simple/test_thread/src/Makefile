
test_thread: main.c
	gcc -o test_thread main.c \
		-lpthread \
		-ldvliw_send_recv \
		-L${TRIMARAN_ROOT}/benchmarks/dvliw_pthreads/lib 

clean:
	rm -f test_thread
