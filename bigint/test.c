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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bigint.h"
#include "iexport.h"
#include "birnd.h"
#include "cdtor.h"

void check_compilation()
{
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	long long_o1 = 5;
	unsigned long ulong_o1 = 7;
	long long_o2 = 5;
	unsigned long ulong_o2 = 7;
	unsigned long ulong_n = 7;
	const char* str_o1 = "1234";
	const char* str_o2;
	int base = 0;
	char buff[256];
	unsigned len = 256;
	int int_ntests = 5;
	int x;
	size_t sz;
	unsigned long ul;
	unsigned long ulong_i = 7;
	unsigned long ulong_k = 7;
	unsigned long ulong_ref_rem = 7;
	/*--------------------------------*/
	BEG_VAR_8_bigint_t(bigint_var,
					bigint_var2,
					bigint_o1,
					bigint_o2,
					bigint_o3,
					bigint_o4,
					bigint_n,
					bigint_rem);
	/*------------------------------*/
	bi_asg_si(bigint_var2,  long_o1);
	bi_asg_ui(bigint_o1,  ulong_o1);
	bi_asg_str(bigint_o2, str_o1);
	bi_asg_str(bigint_o3, str_o1);
	bi_asg_str(bigint_o4, str_o1);
	bi_asg(bigint_n,  bigint_o1);
	/*------------------------------*/
	bi_asg_si(bigint_var,  long_o1);
	/*------------------------------*/
	long_o1 = bi2long(bigint_o1);
	ulong_o1 = bi2ulong(bigint_o1);
	str_o2 = bi2str(bigint_o1, base);
	bi2strx(bigint_o1, base, buff, len);
	/*------------------------------*/
	bi_swap(bigint_var, bigint_var2);
	/*------------------------------*/
	x = bi_sign(bigint_o1);
	sz = bi_nbits(bigint_o1);
	sz = bi_nbytes(bigint_o1);
	sz = bi_nbytes_excl_sgn(bigint_o1);
	x = bi_bit(bigint_o1, ulong_i);
	x = bi_compare(bigint_o1, bigint_o2);
	x = bi_compare_si(bigint_o1, long_o2);
	x = bi_compare_ui(bigint_o1, ulong_o2);
	x = bi_equals(bigint_o1, bigint_o2);
	x = bi_equals_si(bigint_o1, long_o2);
	x = bi_equals_ui(bigint_o1, ulong_o2);
	x = bi_iszero(bigint_o1);
	x = bi_isodd(bigint_o1);
	x = bi_iseven(bigint_o1);
	x = bi_isdiv(bigint_o1, bigint_o2);
	x = bi_isdiv_ui(bigint_o1, ulong_o1);
	x = bi_iscongruent(bigint_o1, bigint_o2, bigint_n);
	x = bi_iscongruent_ui(bigint_o1, ulong_o2, ulong_n);
	x = bi_isprime(bigint_o1, int_ntests);
	/*-------------------------------------------------------------*/
	bi_scanf(stdin, bigint_var, base);
	bi_printf(stdout, bigint_var, base);
	/*------------------------------*/
	bi_nextprime(bigint_var, bigint_o1);
	/*------------------------------*/
	bi_setbit(bigint_var, ulong_i);
	bi_clrbit(bigint_var, ulong_i);
	bi_combit(bigint_var, ulong_i);
	/*------------------------------*/
	/* var = o1 AND o2	*/
	bi_and(bigint_var, bigint_o1, bigint_o2);
	/* var = o1 OR o2	*/
	bi_ior(bigint_var, bigint_o1, bigint_o2);
	/* var = o1 XOR o2	*/
	bi_xor(bigint_var, bigint_o1, bigint_o2);
	/* var = complement_one(o1)	*/
	bi_ones_compl(bigint_var, bigint_o1);
	/*----------------------------	*/
	/* var = o1 * (2 ^ k)	*/
	bi_lshift(bigint_var, bigint_o1, ulong_k);
	/* var = o1 / (2 ^ k)	*/
	bi_rshift(bigint_var, bigint_o1, ulong_k);
	/*----------------------------	*/
	bi_mkodd(bigint_var);
	/*----------------------------	*/
	/* var = abs(o1)	*/
	bi_abs(bigint_var, bigint_o1);
	/* var = -o1	*/
	bi_negate(bigint_var, bigint_o1);
	/* var = o1 % o2	*/
	bi_mod(bigint_var, bigint_o1, bigint_o2);
	/* var = o1 % o2	*/
	bi_mod_ui(bigint_var, bigint_o1, ulong_o2);
	/* return (o1 % 2)	*/
	ul = bi_mod2(bigint_o1);
	/* var = o1 % (2 ^ k)	*/
	bi_mod2k(bigint_var, bigint_o1, ulong_k);
	/* var = o1 + o2	*/
	bi_add(bigint_var, bigint_o1, bigint_o2);
	/* var = o1 + o2	*/
	bi_add_ui(bigint_var, bigint_o1, ulong_o2);
	/* var = o1 + o2 (mod n)	*/
	bi_addmod(bigint_var, bigint_o1, bigint_o2, bigint_n);
	/* var = o1 - o2	*/
	bi_sub(bigint_var, bigint_o1, bigint_o2);
	/* var = o1 - o2	*/
	bi_sub_ui(bigint_var, bigint_o1, ulong_o2);
	/* var = o1 - o2	*/
	bi_sub_uibi(bigint_var, ulong_o1, bigint_o2);
	/* var = o1 - o2	*/
	bi_subpos(bigint_var, bigint_o1, bigint_o2);
	/* var = o1 - o2 (mod n)	*/
	bi_submod(bigint_var, bigint_o1, bigint_o2, bigint_n);
	/* var += o1 * o2	*/
	bi_addmul(bigint_var, bigint_o1, bigint_o2);
	/* var += o1 * o2	*/
	bi_addmul_ui(bigint_var, bigint_o1, ulong_o2);
	/* var -= o1 * o2	*/
	bi_submul(bigint_var, bigint_o1, bigint_o2);
	/* var -= o1 * o2	*/
	bi_submul_ui(bigint_var, bigint_o1, ulong_o2);
	/* var = o1 * o2	*/
	bi_mul(bigint_var, bigint_o1, bigint_o2);
	/* var = o1 * o2	*/
	bi_mul_si(bigint_var, bigint_o1, long_o2);
	/* var = o1 * o2	*/
	bi_mul_ui(bigint_var, bigint_o1, ulong_o2);
	/* var = o1 * 2	*/
	bi_mul2(bigint_var, bigint_o1);
	/* var = o1 * (2 ^ k)	*/
	bi_mul2k(bigint_var, bigint_o1, ulong_k);
	/* var = o1 * o2 (mod n)	*/
	bi_mulmod(bigint_var, bigint_o1, bigint_o2, bigint_n);
	/* var = o1 * o1 ; // var = o1 ^ 2	*/
	bi_sq(bigint_var, bigint_o1);
	/* var = o1 * o1 (mod n) ; // var = o1 ^ 2 (mod n)	*/
	bi_sqmod(bigint_var, bigint_o1, bigint_n);
	/* var = o1 / o2 ; rem = o1 % o2	*/
	bi_div(bigint_var, bigint_o1, bigint_o2, bigint_rem);
	/* var = o1 / o2 	*/
	bi_divq(bigint_var, bigint_o1, bigint_o2);
	/* var = o1 / o2 ; rem = o1 % o2	*/
	bi_div_ui(bigint_var, bigint_o1, ulong_o2, ulong_ref_rem);
	/* var = o1 / 2	*/
	bi_div2(bigint_var, bigint_o1);
	/* var = o1 / (2 ^ k)	*/
	bi_div2k(bigint_var, bigint_o1, ulong_k);
	/* var = o1 / o2 (mod n)	*/
	bi_divmod(bigint_var, bigint_o1, bigint_o2, bigint_n);
	/* var = inv(o1) (mod n)	*/
	bi_invmod(bigint_var, bigint_o1, bigint_n);
	/* var = o1 ^ o2	*/
	bi_pow(bigint_var, bigint_o1, ulong_o2);
	/* var = 2 ^ k	*/
	bi_pow2k(bigint_var, ulong_k);
	/* var = o1 ^ o2 (mod n)	*/
	bi_powmod_sec(bigint_var, bigint_o1, bigint_o2, bigint_n);
	/* var = o1 ^ o2 (mod n)	*/
	bi_powmod(bigint_var, bigint_o1, bigint_o2, bigint_n);
	/* var = o1 ^ o2 (mod n)	*/
	bi_powmod_ui(bigint_var, bigint_o1, ulong_o2, bigint_n);
	/* var = 2 ^ o1 (mod n)	*/
	bi_powmod2k(bigint_var, bigint_o1, bigint_n);
	/* var = (o1^o2) * (o3^o4) (mod n)	*/
	bi_mul_powmod(bigint_var, bigint_o1, bigint_o2,
				 bigint_o3, bigint_o4, bigint_n);
	/*----------------------------	*/
#ifndef NDEBUG
	/* var = o1 ^ o2 (mod n)	*/
	bi_powmod_dbg(bigint_var, bigint_o1, bigint_o2, bigint_n);
#endif
	/*----------------------------	*/
	/* var = gcd(o1, o2) // greatest common divisor	*/
	bi_gcd(bigint_var, bigint_o1, bigint_o2);
	/* var = gcd(o1, o2) // greatest common divisor	*/
	bi_gcd_ui(bigint_o1, ulong_o2);
	/*------------------------------*/
	END_VAR_8_bigint_t(bigint_var,
					bigint_var2,
					bigint_o1,
					bigint_o2,
					bigint_o3,
					bigint_o4,
					bigint_n,
					bigint_rem);
	/*------------------------------*/
	END_BIGINT_CHK();
}

void check_export_1()
{
	BEG_BIGINT_CHK();
	char buff[1024];
	unsigned len = 1024;
	BEG_VAR_2_bigint_t(x, y);
	/*------------------------------*/
	printf("Export/Import\n");
	bi_asg_si(x, 0x123456);
	bi_mul2k(x, x, 19);
	printf("X: "); bi_printf(stdout, x, 16); printf("\n");
	bi_export(x, buff, &len);
	printf("B: "); bi_print_bytes(stdout, buff, len); printf("\n");
	bi_import(y, buff, len);
	printf("Y: "); bi_printf(stdout, y, 16); printf("\n");
	if (! bi_equals(x, y)) {
		printf("\tError: Export/Import\n");
	} else {
		printf("\tOK\n");
	}
	/*------------------------------*/
	END_VAR_2_bigint_t(x, y);
	END_BIGINT_CHK();
}

void check_export_2()
{
	BEG_BIGINT_CHK();
	char buff[1024];
	unsigned len = 1024;
	BEG_VAR_2_bigint_t(x, y);
	/*------------------------------*/
	printf("Export/Import\n");
	bi_asg_si(x, -0x123456);
	bi_mul2k(x, x, 19);
	printf("X: "); bi_printf(stdout, x, 16); printf("\n");
	bi_export(x, buff, &len);
	printf("B: "); bi_print_bytes(stdout, buff, len); printf("\n");
	bi_import(y, buff, len);
	printf("Y: "); bi_printf(stdout, y, 16); printf("\n");
	if (! bi_equals(x, y)) {
		printf("\tError: Export/Import\n");
	} else {
		printf("\tOK\n");
	}
	/*------------------------------*/
	END_VAR_2_bigint_t(x, y);
	END_BIGINT_CHK();
}

void check_export()
{
	check_export_1();
	check_export_2();
}

void check_arit()
{
	BEG_BIGINT_CHK();
	BEG_VAR_4_bigint_t(x, y, z, w);
	/*------------------------------*/
	printf("Addition/Subtraction\n");
	/*------------------------------*/
	bi_asg_ui(x, 10000);
	bi_nextprime(x, x);
	bi_asg_ui(y, 100000);
	bi_nextprime(y, y);
	/*------------------------------*/
	bi_add(z, x, y);
	bi_sub(w, z, y);
	if (! bi_equals(x, w)) {
		printf("\tError: Addition/Subtraction\n");
	} else {
		printf("\tOK\n");
	}
	/*------------------------------*/
	printf("Multiplication/Division\n");
	/*------------------------------*/
	bi_mul(z, x, y);
	bi_divq(w, z, y);
	if (! bi_equals(x, w)) {
		printf("\tError: Multiplication/Division\n");
	} else {
		printf("\tOK\n");
	}
	/*------------------------------*/
	END_VAR_4_bigint_t(x, y, z, w);
	END_BIGINT_CHK();
}

void check_rsa()
{
	BEG_BIGINT_CHK();
	BEG_VAR_1(rndctx_t, rnd_ctx);
	BEG_VAR_11_bigint_t(n,
					 p,
					 q,
					 p1,
					 q1,
					 phi,
					 e,
					 d,
					 aux1,
					 aux2,
					 aux3);
	/*------------------------------*/
	printf("RSA Encrypt/Decrypt\n");
	/*------------------------------*/
	random_seed(&rnd_ctx, PSEUDO_ENTROPY);
	/*random_seed(&rnd_ctx, NO_ENTROPY);*/
	/*------------------------------*/
	{
		INIT_TIMER(RSA_Modulus);
		do {
			bi_random_sgprime_nb(p, p1, 1024/2, DEF_NTESTS, &rnd_ctx);
			bi_random_sgprime_nb(q, q1, 1024/2, DEF_NTESTS, &rnd_ctx);
			bi_mul(n, p, q);
		} while (bi_nbits(n) != 1024);
		PRINT_TIMER(RSA_Modulus);
	}
	bi_sub_ui(p, p, 1);
	bi_sub_ui(q, q, 1);
	bi_mul(phi, p, q); /*bi_lcm(phi, p, q);*/
	/*------------------------------*/
	do {
		bi_random_coprime(e, phi, &rnd_ctx);
	} while (!(bi_compare_ui(e, 1)>0
			   && bi_compare(e, phi)<0
			   && bi_iscoprime(e, phi) ));
	bi_invmod(d, e, phi);
	if (!(bi_compare_ui(d, 1)>0
			 && bi_compare(d, phi)<0
			 && bi_iscoprime(d, phi) )) {
		printf("\tError: d private exponent\n");
	}
	/*------------------------------*/
	do {
		bi_random_coprime(aux1, n, &rnd_ctx);
	} while (!(bi_compare_ui(aux1, 1)>0
			   && bi_compare(aux1, n)<0 ));
	/*------------------------------*/
	bi_powmod(aux2, aux1, e, n);	/* encrypt */
	bi_powmod(aux3, aux2, d, n);	/* decrypt */
	if (! bi_equals(aux1, aux3)) {
		printf("\tError: RSA\n");
	} else {
		printf("\tOK\n");
	}
	/*------------------------------*/
	END_VAR_11_bigint_t(n,
					 p,
					 q,
					 p1,
					 q1,
					 phi,
					 e,
					 d,
					 aux1,
					 aux2,
					 aux3);
	END_VAR_1(rndctx_t, rnd_ctx);
	END_BIGINT_CHK();
}

int main()
{
	BEG_BIGINT_CHK();
	/*check_compilation();*/
	check_export();
	check_arit();
	check_rsa();
	END_BIGINT_CHK();
	return 0;
}
