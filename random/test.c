/*----------------------------------------------------------------------------*/
/* Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain           */
/*                                                                            */
/* This program is free software: you can redistribute it and/or              */
/* modify it under the terms of the GNU General Public License as             */
/* published by the Free Software Foundation, either version 2 of             */
/* the License, or (at your option) any later version.                        */
/*                                                                            */
/* This program is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               */
/* GNU General Public License for more details.                               */
/*                                                                            */
/* You should have received a copy of the GNU General Public                  */
/* License along with this program. If not, see                               */
/* <http://www.gnu.org/licenses/>.                                            */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "random.h"

/*----------------------------------------------------------------------------*/
int frecuency_test(const void* data, unsigned datalen)
{
	int ok;
	unsigned i, j;
	unsigned char d;
	unsigned n[1<<1] = { 0 };
	const unsigned char* p = (const unsigned char*) data;
	assert(datalen == 2500);	/* 2500 == 20000/8; Menezes HAC pg 183 */
	for (i = 0; i < datalen; ++i) {
		d = p[i];
		for (j = 0; j < CHAR_BIT; ++j) {
			++n[(d & 0x01)];
			d = (unsigned char)((d & 0x0FF) >> 1U);
		}
	}
	ok = (9654 < n[1]) && (n[1] < 10346);
	printf("Frecuency test: %s [%u] (%u..%u)\n", (ok?"Success":"Fail"), n[1],
		   9654, 10346);
	return ok;
}
/*----------------------------------------------------------------------------*/
int poker_test(const void* data, unsigned datalen)
{
	int ok;
	unsigned i, j, sum;
	unsigned char d;
	unsigned n[1<<4] = { 0 };
	const unsigned char* p = (const unsigned char*) data;
	unsigned k;
	double x;
	assert(datalen == 2500);	/* 2500 == 20000/8; Menezes HAC pg 183 */
	for (i = 0; i < datalen; ++i) {
		d = p[i];
		for (j = 0; j < CHAR_BIT/4; ++j) {
			++n[(d & 0x0F)];
			d = (unsigned char)((d & 0x0FF) >> 4U);
		}
	}
	sum = 0;
	for (i = 0; i < 1<<4; ++i) {
		sum += n[i]*n[i];
	}
	k = datalen*CHAR_BIT/4;
	x = (double)(1<<4)/k * sum - k;
	ok = (1.03 < x) && (x < 57.4);
	printf("Poker test: %s [%.2f] (%.2f..%.2f)\n", (ok?"Success":"Fail"), x,
		   1.03, 57.4);
	return ok;
}
/*----------------------------------------------------------------------------*/
int runs_test(const void* data, unsigned datalen)
{
	int ok = 1;
	unsigned i, j;
	unsigned char d;
	unsigned b[6] = { 0 };
	unsigned g[6] = { 0 };
	const unsigned char* p = (const unsigned char*) data;
	double x, e, aux;
	unsigned ant, act, cnt;
	assert(datalen == 2500);	/* 2500 == 20000/8; Menezes HAC pg 183 */
	cnt = 0;
	ant = 9;
	for (i = 0; ok && (i < datalen); ++i) {
		d = p[i];
		for (j = 0; j < CHAR_BIT; ++j) {
			act = (d & 0x01);
			if (act == ant) {
				++cnt;
			} else {
				if (cnt >= 33) {
					ok = 0;
				} else if (ant == 0) {
					++g[cnt < 6 ? cnt : 5];
				} else if (ant == 1) {
					++b[cnt < 6 ? cnt : 5];
				}
				cnt = 0;
			}
			ant = act;
			d = (unsigned char)((d & 0x0FF) >> 1U);
		}
	}
	if (cnt >= 33) {
		ok = 0;
	} else if (ant == 0) {
		++g[cnt < 6 ? cnt : 5];
	} else if (ant == 1) {
		++b[cnt < 6 ? cnt : 5];
	}
	ok = ok
		&&((2267 < b[0]) && (b[0] < 2733))
		&&((1079 < b[1]) && (b[1] < 1421))
		&&((502 < b[2]) && (b[2] < 748))
		&&((223 < b[3]) && (b[3] < 402))
		&&((90 < b[4]) && (b[4] < 223))
		&&((90 < b[5]) && (b[5] < 223));
	ok = ok
		&&((2267 < g[0]) && (g[0] < 2733))
		&&((1079 < g[1]) && (g[1] < 1421))
		&&((502 < g[2]) && (g[2] < 748))
		&&((223 < g[3]) && (g[3] < 402))
		&&((90 < g[4]) && (g[4] < 223))
		&&((90 < g[5]) && (g[5] < 223));
	x = 0;
	for (i = 0; i < 6; ++i) {
		e = (double)(datalen*CHAR_BIT - (i+1) + 3) / (1 <<(i+1+2)); 
		aux = b[i] - e;
		x += (aux * aux) / e;
		aux = g[i] - e;
		x += (aux * aux) / e;
	}
	printf("Runs test: %s [%.2f]\n", (ok?"Success":"Fail"), x);
	return ok;
}
/*----------------------------------------------------------------------------*/
/* Menezes HAC pg 183 */
int test(struct rndctx_t* rnd_ctx)
{
	int ok = 1;
	char data[2500];	/* 2500 == 20000/8; Menezes HAC pg 183 */
	random_bytes(data, sizeof(data), rnd_ctx);
	/*--------------------------------*/
	ok = frecuency_test(data, sizeof(data)) && ok;
	ok = poker_test(data, sizeof(data)) && ok;
	ok = runs_test(data, sizeof(data)) && ok;
	/*--------------------------------*/
	return ok;
}
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
	struct rndctx_t rnd_ctx;
	unsigned aux, i, j;
	unsigned entropy_src = PSEUDO_ENTROPY;
	int ok;
	if ((argc == 2)&&(strcmp(argv[1],"-t")==0)) {
		entropy_src = TRUE_ENTROPY;
	}
	rndctx_t_ctor(&rnd_ctx);
	printf("Entropy Source: %s\n",
		   entropy_src == TRUE_ENTROPY ? "TRUE_ENTROPY" : "PSEUDO_ENTROPY");
	random_seed(&rnd_ctx, entropy_src);
	printf("\n");
	for (j = 0; j < 2; ++j) {
		for (i = 0; i < 5; ++i) {
			random_bytes(&aux, sizeof(aux), &rnd_ctx);
			printf("0x%08x ", aux);
		}
		printf("\n");
	}
	printf("\n");
	ok = test(&rnd_ctx);
	printf("Test: %s\n", (ok?"Success":"Fail"));
	rndctx_t_dtor(&rnd_ctx);
	return 0;
}
