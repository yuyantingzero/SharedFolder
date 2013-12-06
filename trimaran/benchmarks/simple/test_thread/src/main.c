#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct _my_func_params {

	float a;
	float b;
};

void my_func(float a, float b);
void* my_func_DVLIW_SPLIT_0(void* void_arg);
void* my_func_DVLIW_SPLIT_1(void* void_arg);
void* my_func_DVLIW_SPLIT_2(void* void_arg);
void* my_func_DVLIW_SPLIT_3(void* void_arg);

int main() {
	pthread_init_send_recv();

	my_func(770.0, 404.0);

	pthread_delete_send_recv();
	return 0;
}

void my_func(float a, float b)
{
	pthread_t t0, t1, t2, t3;
	struct _my_func_params params = { a, b };
		
	pthread_create(&t0, NULL, my_func_DVLIW_SPLIT_0, (void*) &params); 
	pthread_create(&t1, NULL, my_func_DVLIW_SPLIT_1, (void*) &params); 
	pthread_create(&t2, NULL, my_func_DVLIW_SPLIT_2, (void*) &params); 
	pthread_create(&t3, NULL, my_func_DVLIW_SPLIT_3, (void*) &params); 

	pthread_join(t0, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
}


void* my_func_DVLIW_SPLIT_0(void* void_arg)
{
	const struct _my_func_params* params = (struct _my_func_params*) void_arg;
	const int cid = 0;

	// ops for cluster 0
	int x = pthread_recv_i(1, cid);
	int y = pthread_recv_i(2, cid);

	printf("c0: value received from c1: %d\n", x);
	printf("c0: value received from c2: %d\n", y);
}

void* my_func_DVLIW_SPLIT_1(void* void_arg)
{
	const struct _my_func_params* params = (struct _my_func_params*) void_arg;
	const int cid = 1;

	// ops for cluster 1

	pthread_send_i(cid, 0, 312);
}

void* my_func_DVLIW_SPLIT_2(void* void_arg)
{
	const struct _my_func_params* params = (struct _my_func_params*) void_arg;
	const int cid = 2;

	// ops for cluster 2 

	pthread_send_i(cid, 0, 847);
}

void* my_func_DVLIW_SPLIT_3(void* void_arg)
{
	const struct _my_func_params* params = (struct _my_func_params*) void_arg;
	const int cid = 3;

	// ops for cluster 3 

}


