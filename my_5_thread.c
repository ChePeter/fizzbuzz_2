/*
 ============================================================================
 Name        : my_5_thread.c
 Author      : Peter Che 7210208@gmail.com
 Version     :
 Copyright   : Учитесь
 Description : FizzBuzz in C second round
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define CHUNK_SIZE  238 // 16*10 + 58

pthread_spinlock_t spin_c[3];
pthread_spinlock_t spin_p[3];

struct bufs {
	uint64_t	number;
	size_t		count;
	size_t		len;
	size_t		size;
	char*		start;
	char*		buf;
};
struct bufs buf_g[3];

uint64_t n11[8] = { 0 };
uint64_t test[8] = { 0 };
uint64_t n10[8] = { 0 };
uint64_t n010[8] = { 0 };
uint64_t n1[8] = { 0 };
uint64_t add_3 = 0x0300000000000000;


#define DIGIT_1 do { *(uint64_t*)cur = 0x0a31000000000000; cur -= 2; *((uint64_t*)cur) = t_num; cur -= t_len; } while (0)
#define DIGIT_2 do { *(uint64_t*)cur = 0x0a32000000000000; cur -= 2; *((uint64_t*)cur) = t_num; cur -= t_len; } while (0)
#define DIGIT_3 do { *(uint64_t*)cur = 0x0a33000000000000; cur -= 2; *((uint64_t*)cur) = t_num; cur -= t_len; } while (0)
#define DIGIT_4 do { *(uint64_t*)cur = 0x0a34000000000000; cur -= 2; *((uint64_t*)cur) = t_num; cur -= t_len; } while (0)
#define DIGIT_6 do { *(uint64_t*)cur = 0x0a36000000000000; cur -= 2; *((uint64_t*)cur) = t_num; cur -= t_len; } while (0)
#define DIGIT_7 do { *(uint64_t*)cur = 0x0a37000000000000; cur -= 2; *((uint64_t*)cur) = t_num; cur -= t_len; } while (0)
#define DIGIT_8 do { *(uint64_t*)cur = 0x0a38000000000000; cur -= 2; *((uint64_t*)cur) = t_num; cur -= t_len; } while (0)
#define DIGIT_9 do { *(uint64_t*)cur = 0x0a39000000000000; cur -= 2; *((uint64_t*)cur) = t_num; cur -= t_len; } while (0)

#define FIZZ do { *((uint64_t*) cur) = 0x0a7a7a6946000000; cur -= 5; } while (0)
#define FIZZBUZZ do { *((uint64_t*)cur) = 0x0a0a0a0a0a0a0a0a; cur--; *((uint64_t*)cur) = 0x7a7a75427a7a6946; cur -= 8; } while (0)
#define FIZZ_BUZZ do { *((uint64_t*)cur) = 0x0a7a000000000000;  cur -= 2; *((uint64_t*)cur) = 0x7a75420a7a7a6946; cur -= 8; } while (0)
#define BUZZ_FIZZ do { *((uint64_t*)cur) = 0x0a7a000000000000;  cur -= 2; *((uint64_t*)cur) = 0x7a69460a7a7a7542; cur -= 8; } while (0)


uint64_t num_count = 73*101;
uint64_t num_len = 137*11;
// = 11×73×101×137

uint64_t buf_len;

void* my_sub_0(void* arg) {
	int thread_num;
	int i;
	char* buf;
	char* cur = NULL;
	thread_num = *(int*)arg;
	buf = buf_g[thread_num].buf;

	uint64_t num[3]; // = { 0x3030303030303030, 0x3030303030303030, 0x3030303030303030 };
	int len[3]; 	 // = { 8, 8, 8 };

	uint64_t t_num;
	int t_len;

	int k;
	uint64_t t_number;
	uint64_t number;

	int j;
	int jj = 1; // thread number 0
	while (1) {
		pthread_spin_lock(&spin_c[thread_num]);
		buf_g[thread_num].number = (jj + thread_num) * (buf_g[thread_num].count * 3);
		number = buf_g[thread_num].number;
		if (number > 99999999)
			break;
		jj += 3;
		i = 2;
		while (i >= 0) {
			k = 0;
			t_number = number + i - 2;
			cur = (char*)&num[i] + 8;
			do {
				*--cur = t_number % 10 + '0';
				t_number /= 10;
				k++;
			} while (t_number != 0);
			len[i] = k;
			i--;
		}

		cur = buf + buf_g[thread_num].size - 8;
		for (j = 0; j < buf_g[thread_num].count; j++) {
			t_num = num[2];
			t_len = len[2];
			FIZZ_BUZZ;
			DIGIT_8;
			DIGIT_7;
			BUZZ_FIZZ;
			DIGIT_4;
			FIZZ;
			DIGIT_2;
			DIGIT_1;

			t_num = num[1];
			t_len = len[1];
			FIZZBUZZ;
			DIGIT_9;
			DIGIT_8;
			FIZZ;
			DIGIT_6;
			FIZZ_BUZZ;
			DIGIT_3;
			DIGIT_2;

			t_num = num[0];
			t_len = len[0];
			BUZZ_FIZZ;
			DIGIT_9;
			FIZZ;
			DIGIT_7;
			DIGIT_6;
			FIZZBUZZ;
			DIGIT_4;
			DIGIT_3;
			FIZZ;
			DIGIT_1;

			for (i = 2; i >= 0; i--) {
				k = 0;
				num[i] -= add_3;
				do {
					if ((num[i] & test[k]) >= n010[k])
						break;
					num[i] += n11[k];
					k++;
					if ((k == (len[i] - 1))
						&& ((num[i] & test[k]) == n010[k])) {
						len[i]--;
						break;
					}
				} while (1);
			}
		}
		buf_g[thread_num].len = buf + buf_g[thread_num].size - cur - 8;
		buf_g[thread_num].start = cur + 8;
		pthread_spin_unlock(&spin_p[thread_num]);
		number += 3 * (buf_g[thread_num].count * 3);
	}
	return 0;
}

void* send_out(void* arg) {
	const char *buf_0 = "1\n2\nFizz\n4\nBuzz\nFizz\n7\n8\nFizz\nBuzz\n";
	fwrite(buf_0, 35, 1, stdout);
	do {
		pthread_spin_lock(&spin_p[0]);
		fwrite(buf_g[0].start, buf_g[0].len, 1, stdout);
		pthread_spin_unlock(&spin_c[0]);

		pthread_spin_lock(&spin_p[1]);
		fwrite(buf_g[1].start, buf_g[1].len, 1, stdout);
		pthread_spin_unlock(&spin_c[1]);

		pthread_spin_lock(&spin_p[2]);
		fwrite(buf_g[2].start, buf_g[2].len, 1, stdout);
		pthread_spin_unlock(&spin_c[2]);
		if (buf_g[2].number >= 99999999)
			break;
	} while (1);
	pthread_exit(NULL);
	return 0;
}

int main() {
	pthread_t threads[4];

	buf_g[0].count = num_len;
	buf_g[1].count = num_len;
	buf_g[2].count = num_len;
	buf_g[0].size = CHUNK_SIZE * num_len;
	buf_g[1].size = CHUNK_SIZE * num_len;
	buf_g[2].size = CHUNK_SIZE * num_len;
	buf_g[0].len = 0;
	buf_g[1].len = 0;
	buf_g[2].len = 0;
	buf_g[0].number = 0;
	buf_g[1].number = 0;
	buf_g[2].number = 0;

	buf_g[0].buf = (char*)malloc(buf_g[0].size + 1);
	buf_g[1].buf = (char*)malloc(buf_g[1].size + 1);
	buf_g[2].buf = (char*)malloc(buf_g[2].size + 1);

	n10[0] = 0x0a00000000000000;
	n10[1] = 0x000a000000000000;
	n10[2] = 0x00000a0000000000;
	n10[3] = 0x0000000a00000000;
	n10[4] = 0x000000000a000000;
	n10[5] = 0x00000000000a0000;
	n10[6] = 0x0000000000000a00;
	n10[7] = 0x000000000000000a;

	n010[0] = 0x3000000000000000;
	n010[1] = 0x0030000000000000;
	n010[2] = 0x0000300000000000;
	n010[3] = 0x0000003000000000;
	n010[4] = 0x0000000030000000;
	n010[5] = 0x0000000000300000;
	n010[6] = 0x0000000000003000;
	n010[7] = 0x0000000000000030;

	n1[0] = 0x0001000000000000;
	n1[1] = 0x0000010000000000;
	n1[2] = 0x0000000100000000;
	n1[3] = 0x0000000001000000;
	n1[4] = 0x0000000000010000;
	n1[5] = 0x0000000000000100;
	n1[6] = 0x0000000000000001;
	n1[7] = 0x0000000000000000;

	test[0] = 0xff00000000000000;
	test[1] = 0x00ff000000000000;
	test[2] = 0x0000ff0000000000;
	test[3] = 0x000000ff00000000;
	test[4] = 0x00000000ff000000;
	test[5] = 0x0000000000ff0000;
	test[6] = 0x000000000000ff00;
	test[7] = 0x00000000000000ff;

	int k;
	for (k = 0; k < 8; k++) {
		n11[k] = n10[k] - n1[k];
	}

	pthread_spin_init(&spin_c[0], 0);
	pthread_spin_init(&spin_p[0], 0);
	pthread_spin_init(&spin_c[1], 0);
	pthread_spin_init(&spin_p[1], 0);
	pthread_spin_init(&spin_p[2], 0);
	pthread_spin_init(&spin_c[2], 0);

	pthread_spin_lock(&spin_p[0]);
	pthread_spin_lock(&spin_p[1]);
	pthread_spin_lock(&spin_p[2]);

	int id0 = 0;
	int id1 = 1;
	int id2 = 2;
	pthread_create(&threads[0], NULL, my_sub_0, &id0);
	pthread_create(&threads[1], NULL, my_sub_0, &id1);
	pthread_create(&threads[2], NULL, my_sub_0, &id2);
	pthread_create(&threads[3], NULL, send_out, NULL);

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
	pthread_join(threads[3], NULL);

	pthread_spin_destroy(&spin_c[0]);
	pthread_spin_destroy(&spin_c[1]);
	pthread_spin_destroy(&spin_c[2]);
	pthread_spin_destroy(&spin_p[0]);
	pthread_spin_destroy(&spin_p[1]);
	pthread_spin_destroy(&spin_p[2]);

	return 0;
}
