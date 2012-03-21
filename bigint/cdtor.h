/*----------------------------------------------------------------------------*/
/* Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain           */
/*                                                                            */
/* This file is part of the <FTMGS> Library                                   */
/*                                                                            */
/* This library is free software: you can redistribute it and/or              */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation, either               */
/* version 2.1 of the License, or (at your option) any later                  */
/* version.                                                                   */
/*                                                                            */
/* This library is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               */
/* GNU Lesser General Public License for more details.                        */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General                  */
/* Public License along with this library. If not, see                        */
/* <http://www.gnu.org/licenses/>.                                            */
/*----------------------------------------------------------------------------*/
#ifndef umalccvbg_cdtor_h__
#define umalccvbg_cdtor_h__	1
#include <assert.h>
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
/*----------------------------------------------------------------------------*/
/* 
 * There can not be any jump (return, goto, break, continue) from inside
 * the scope delimited between BEG_VAR_?(); ... END_VAR_?(); to the outside
 * of such scope
 * Also, any variable defined inside such scope is not accesible from
 * outside
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define BEG_VAR_A(t_y_p_e_1__, v_a_r_1__, n_e_l_m_s__)					\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__[n_e_l_m_s__];							\
	{int ctr_i__; for (ctr_i__ = 0; ctr_i__ < (int)(n_e_l_m_s__); ++ctr_i__) { \
			t_y_p_e_1__##_ctor(&(v_a_r_1__[ctr_i__]));					\
		}}																\
		do{																\
		typedef void Void_XTOR_A__##v_a_r_1__##n_e_l_m_s__ ;			\
		EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_V(t_y_p_e_1__, v_a_r_1__, n_e_l_m_s__)					\
	do{{{																\
	struct t_y_p_e_1__* v_a_r_1__ =										\
		(struct t_y_p_e_1__*)malloc((n_e_l_m_s__)*sizeof(struct t_y_p_e_1__)) ; \
	if (v_a_r_1__) { int ctr_i__;										\
		for (ctr_i__ = 0; ctr_i__ < (int)(n_e_l_m_s__); ++ctr_i__) {	\
			t_y_p_e_1__##_ctor(&(v_a_r_1__[ctr_i__]));					\
		}																\
	}																	\
	do{																	\
	typedef void Void_XTOR_V__##v_a_r_1__##n_e_l_m_s__ ;				\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_1(t_y_p_e_1__, v_a_r_1__)		\
	do{{{										\
	struct t_y_p_e_1__ v_a_r_1__;				\
	t_y_p_e_1__##_ctor(&v_a_r_1__);				\
	do{											\
	typedef void Void_XTOR_1__##v_a_r_1__ ;		\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_2(t_y_p_e_1__, v_a_r_1__,				\
				  t_y_p_e_2__, v_a_r_2__)				\
	do{{{												\
	struct t_y_p_e_1__ v_a_r_1__;						\
	struct t_y_p_e_2__ v_a_r_2__;						\
	t_y_p_e_1__##_ctor(&v_a_r_1__);						\
	t_y_p_e_2__##_ctor(&v_a_r_2__);						\
	do{													\
	typedef void Void_XTOR_2__##v_a_r_1__##v_a_r_2__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_3(t_y_p_e_1__, v_a_r_1__,							\
				  t_y_p_e_2__, v_a_r_2__,							\
				  t_y_p_e_3__, v_a_r_3__)							\
	do{{{															\
	struct t_y_p_e_1__ v_a_r_1__;									\
	struct t_y_p_e_2__ v_a_r_2__;									\
	struct t_y_p_e_3__ v_a_r_3__;									\
	t_y_p_e_1__##_ctor(&v_a_r_1__);									\
	t_y_p_e_2__##_ctor(&v_a_r_2__);									\
	t_y_p_e_3__##_ctor(&v_a_r_3__);									\
	do{																\
	typedef void Void_XTOR_3__##v_a_r_1__##v_a_r_2__##v_a_r_3__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_4(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__)								\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	do{																	\
	typedef void Void_XTOR_4__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_5(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__)								\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	do{																	\
	typedef void Void_XTOR_5__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_6(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__)								\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	do{																	\
	typedef void Void_XTOR_6__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_7(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__)								\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	do{																	\
	typedef void Void_XTOR_7__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_8(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__,								\
				  t_y_p_e_8__, v_a_r_8__)								\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	struct t_y_p_e_8__ v_a_r_8__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	t_y_p_e_8__##_ctor(&v_a_r_8__);										\
	do{																	\
	typedef void Void_XTOR_8__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_9(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__,								\
				  t_y_p_e_8__, v_a_r_8__,								\
				  t_y_p_e_9__, v_a_r_9__)								\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	struct t_y_p_e_8__ v_a_r_8__;										\
	struct t_y_p_e_9__ v_a_r_9__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	t_y_p_e_8__##_ctor(&v_a_r_8__);										\
	t_y_p_e_9__##_ctor(&v_a_r_9__);										\
	do{																	\
	typedef void Void_XTOR_9__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_10(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__)							\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	struct t_y_p_e_8__ v_a_r_8__;										\
	struct t_y_p_e_9__ v_a_r_9__;										\
	struct t_y_p_e_10__ v_a_r_10__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	t_y_p_e_8__##_ctor(&v_a_r_8__);										\
	t_y_p_e_9__##_ctor(&v_a_r_9__);										\
	t_y_p_e_10__##_ctor(&v_a_r_10__);									\
	do{																	\
	typedef void Void_XTOR_10__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_11(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__)							\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	struct t_y_p_e_8__ v_a_r_8__;										\
	struct t_y_p_e_9__ v_a_r_9__;										\
	struct t_y_p_e_10__ v_a_r_10__;										\
	struct t_y_p_e_11__ v_a_r_11__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	t_y_p_e_8__##_ctor(&v_a_r_8__);										\
	t_y_p_e_9__##_ctor(&v_a_r_9__);										\
	t_y_p_e_10__##_ctor(&v_a_r_10__);									\
	t_y_p_e_11__##_ctor(&v_a_r_11__);									\
	do{																	\
	typedef void Void_XTOR_11__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_12(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__)							\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	struct t_y_p_e_8__ v_a_r_8__;										\
	struct t_y_p_e_9__ v_a_r_9__;										\
	struct t_y_p_e_10__ v_a_r_10__;										\
	struct t_y_p_e_11__ v_a_r_11__;										\
	struct t_y_p_e_12__ v_a_r_12__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	t_y_p_e_8__##_ctor(&v_a_r_8__);										\
	t_y_p_e_9__##_ctor(&v_a_r_9__);										\
	t_y_p_e_10__##_ctor(&v_a_r_10__);									\
	t_y_p_e_11__##_ctor(&v_a_r_11__);									\
	t_y_p_e_12__##_ctor(&v_a_r_12__);									\
	do{																	\
	typedef void Void_XTOR_12__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_13(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__)							\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	struct t_y_p_e_8__ v_a_r_8__;										\
	struct t_y_p_e_9__ v_a_r_9__;										\
	struct t_y_p_e_10__ v_a_r_10__;										\
	struct t_y_p_e_11__ v_a_r_11__;										\
	struct t_y_p_e_12__ v_a_r_12__;										\
	struct t_y_p_e_13__ v_a_r_13__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	t_y_p_e_8__##_ctor(&v_a_r_8__);										\
	t_y_p_e_9__##_ctor(&v_a_r_9__);										\
	t_y_p_e_10__##_ctor(&v_a_r_10__);									\
	t_y_p_e_11__##_ctor(&v_a_r_11__);									\
	t_y_p_e_12__##_ctor(&v_a_r_12__);									\
	t_y_p_e_13__##_ctor(&v_a_r_13__);									\
	do{																	\
	typedef void Void_XTOR_13__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_14(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__)							\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	struct t_y_p_e_8__ v_a_r_8__;										\
	struct t_y_p_e_9__ v_a_r_9__;										\
	struct t_y_p_e_10__ v_a_r_10__;										\
	struct t_y_p_e_11__ v_a_r_11__;										\
	struct t_y_p_e_12__ v_a_r_12__;										\
	struct t_y_p_e_13__ v_a_r_13__;										\
	struct t_y_p_e_14__ v_a_r_14__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	t_y_p_e_8__##_ctor(&v_a_r_8__);										\
	t_y_p_e_9__##_ctor(&v_a_r_9__);										\
	t_y_p_e_10__##_ctor(&v_a_r_10__);									\
	t_y_p_e_11__##_ctor(&v_a_r_11__);									\
	t_y_p_e_12__##_ctor(&v_a_r_12__);									\
	t_y_p_e_13__##_ctor(&v_a_r_13__);									\
	t_y_p_e_14__##_ctor(&v_a_r_14__);									\
	do{																	\
	typedef void Void_XTOR_14__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_15(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__,							\
				   t_y_p_e_15__, v_a_r_15__)							\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	struct t_y_p_e_8__ v_a_r_8__;										\
	struct t_y_p_e_9__ v_a_r_9__;										\
	struct t_y_p_e_10__ v_a_r_10__;										\
	struct t_y_p_e_11__ v_a_r_11__;										\
	struct t_y_p_e_12__ v_a_r_12__;										\
	struct t_y_p_e_13__ v_a_r_13__;										\
	struct t_y_p_e_14__ v_a_r_14__;										\
	struct t_y_p_e_15__ v_a_r_15__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	t_y_p_e_8__##_ctor(&v_a_r_8__);										\
	t_y_p_e_9__##_ctor(&v_a_r_9__);										\
	t_y_p_e_10__##_ctor(&v_a_r_10__);									\
	t_y_p_e_11__##_ctor(&v_a_r_11__);									\
	t_y_p_e_12__##_ctor(&v_a_r_12__);									\
	t_y_p_e_13__##_ctor(&v_a_r_13__);									\
	t_y_p_e_14__##_ctor(&v_a_r_14__);									\
	t_y_p_e_15__##_ctor(&v_a_r_15__);									\
	do{																	\
	typedef void Void_XTOR_15__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_16(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__,							\
				   t_y_p_e_15__, v_a_r_15__,							\
				   t_y_p_e_16__, v_a_r_16__)							\
	do{{{																\
	struct t_y_p_e_1__ v_a_r_1__;										\
	struct t_y_p_e_2__ v_a_r_2__;										\
	struct t_y_p_e_3__ v_a_r_3__;										\
	struct t_y_p_e_4__ v_a_r_4__;										\
	struct t_y_p_e_5__ v_a_r_5__;										\
	struct t_y_p_e_6__ v_a_r_6__;										\
	struct t_y_p_e_7__ v_a_r_7__;										\
	struct t_y_p_e_8__ v_a_r_8__;										\
	struct t_y_p_e_9__ v_a_r_9__;										\
	struct t_y_p_e_10__ v_a_r_10__;										\
	struct t_y_p_e_11__ v_a_r_11__;										\
	struct t_y_p_e_12__ v_a_r_12__;										\
	struct t_y_p_e_13__ v_a_r_13__;										\
	struct t_y_p_e_14__ v_a_r_14__;										\
	struct t_y_p_e_15__ v_a_r_15__;										\
	struct t_y_p_e_16__ v_a_r_16__;										\
	t_y_p_e_1__##_ctor(&v_a_r_1__);										\
	t_y_p_e_2__##_ctor(&v_a_r_2__);										\
	t_y_p_e_3__##_ctor(&v_a_r_3__);										\
	t_y_p_e_4__##_ctor(&v_a_r_4__);										\
	t_y_p_e_5__##_ctor(&v_a_r_5__);										\
	t_y_p_e_6__##_ctor(&v_a_r_6__);										\
	t_y_p_e_7__##_ctor(&v_a_r_7__);										\
	t_y_p_e_8__##_ctor(&v_a_r_8__);										\
	t_y_p_e_9__##_ctor(&v_a_r_9__);										\
	t_y_p_e_10__##_ctor(&v_a_r_10__);									\
	t_y_p_e_11__##_ctor(&v_a_r_11__);									\
	t_y_p_e_12__##_ctor(&v_a_r_12__);									\
	t_y_p_e_13__##_ctor(&v_a_r_13__);									\
	t_y_p_e_14__##_ctor(&v_a_r_14__);									\
	t_y_p_e_15__##_ctor(&v_a_r_15__);									\
	t_y_p_e_16__##_ctor(&v_a_r_16__);									\
	do{																	\
	typedef void Void_XTOR_16__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__##v_a_r_16__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define END_VAR_A(t_y_p_e_1__, v_a_r_1__, n_e_l_m_s__)					\
	{EXTERN_C Void_XTOR_A__##v_a_r_1__##n_e_l_m_s__ abort(void);}		\
		}while(0);														\
	{int ctr_i__; for (ctr_i__ = (int)(n_e_l_m_s__)-1; ctr_i__ >= 0 ; --ctr_i__) { \
			t_y_p_e_1__##_dtor(&(v_a_r_1__[ctr_i__]));					\
		}}																\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_V(t_y_p_e_1__, v_a_r_1__, n_e_l_m_s__)					\
	{EXTERN_C Void_XTOR_V__##v_a_r_1__##n_e_l_m_s__ abort(void);}		\
		}while(0);														\
	if (v_a_r_1__) { int ctr_i__;										\
		for (ctr_i__ = (int)(n_e_l_m_s__)-1; ctr_i__ >= 0 ; --ctr_i__) {	\
			t_y_p_e_1__##_dtor(&(v_a_r_1__[ctr_i__]));					\
		}																\
	}																	\
	free(v_a_r_1__);													\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_1(t_y_p_e_1__, v_a_r_1__)				\
	{EXTERN_C Void_XTOR_1__##v_a_r_1__ abort(void);}	\
		}while(0);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);						\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_2(t_y_p_e_1__, v_a_r_1__,						\
				  t_y_p_e_2__, v_a_r_2__)						\
	{EXTERN_C Void_XTOR_2__##v_a_r_1__##v_a_r_2__ abort(void);}	\
		}while(0);												\
	t_y_p_e_2__##_dtor(&v_a_r_2__);								\
	t_y_p_e_1__##_dtor(&v_a_r_1__);								\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_3(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__)								\
	{EXTERN_C Void_XTOR_3__##v_a_r_1__##v_a_r_2__##v_a_r_3__ abort(void);} \
		}while(0);														\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_4(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__)								\
	{EXTERN_C Void_XTOR_4__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__ abort(void);} \
		}while(0);														\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_5(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__)								\
	{EXTERN_C Void_XTOR_5__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__ abort(void);} \
		}while(0);														\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_6(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__)								\
	{EXTERN_C Void_XTOR_6__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__ abort(void);} \
		}while(0);														\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_7(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__)								\
	{EXTERN_C Void_XTOR_7__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__ abort(void);} \
		}while(0);														\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_8(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__,								\
				  t_y_p_e_8__, v_a_r_8__)								\
	{EXTERN_C Void_XTOR_8__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__ abort(void);} \
		}while(0);														\
	t_y_p_e_8__##_dtor(&v_a_r_8__);										\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_9(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__,								\
				  t_y_p_e_8__, v_a_r_8__,								\
				  t_y_p_e_9__, v_a_r_9__)								\
	{EXTERN_C Void_XTOR_9__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__ abort(void);} \
		}while(0);														\
	t_y_p_e_9__##_dtor(&v_a_r_9__);										\
	t_y_p_e_8__##_dtor(&v_a_r_8__);										\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_10(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__)							\
	{EXTERN_C Void_XTOR_10__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__ abort(void);} \
		}while(0);														\
	t_y_p_e_10__##_dtor(&v_a_r_10__);									\
	t_y_p_e_9__##_dtor(&v_a_r_9__);										\
	t_y_p_e_8__##_dtor(&v_a_r_8__);										\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_11(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__)							\
	{EXTERN_C Void_XTOR_11__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__ abort(void);} \
		}while(0);														\
	t_y_p_e_11__##_dtor(&v_a_r_11__);									\
	t_y_p_e_10__##_dtor(&v_a_r_10__);									\
	t_y_p_e_9__##_dtor(&v_a_r_9__);										\
	t_y_p_e_8__##_dtor(&v_a_r_8__);										\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_12(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__)							\
	{EXTERN_C Void_XTOR_12__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__ abort(void);} \
		}while(0);														\
	t_y_p_e_12__##_dtor(&v_a_r_12__);									\
	t_y_p_e_11__##_dtor(&v_a_r_11__);									\
	t_y_p_e_10__##_dtor(&v_a_r_10__);									\
	t_y_p_e_9__##_dtor(&v_a_r_9__);										\
	t_y_p_e_8__##_dtor(&v_a_r_8__);										\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_13(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__)							\
	{EXTERN_C Void_XTOR_13__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__ abort(void);} \
		}while(0);														\
	t_y_p_e_13__##_dtor(&v_a_r_13__);									\
	t_y_p_e_12__##_dtor(&v_a_r_12__);									\
	t_y_p_e_11__##_dtor(&v_a_r_11__);									\
	t_y_p_e_10__##_dtor(&v_a_r_10__);									\
	t_y_p_e_9__##_dtor(&v_a_r_9__);										\
	t_y_p_e_8__##_dtor(&v_a_r_8__);										\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_14(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__)							\
	{EXTERN_C Void_XTOR_14__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__ abort(void);} \
		}while(0);														\
	t_y_p_e_14__##_dtor(&v_a_r_14__);									\
	t_y_p_e_13__##_dtor(&v_a_r_13__);									\
	t_y_p_e_12__##_dtor(&v_a_r_12__);									\
	t_y_p_e_11__##_dtor(&v_a_r_11__);									\
	t_y_p_e_10__##_dtor(&v_a_r_10__);									\
	t_y_p_e_9__##_dtor(&v_a_r_9__);										\
	t_y_p_e_8__##_dtor(&v_a_r_8__);										\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_15(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__,							\
				   t_y_p_e_15__, v_a_r_15__)							\
	{EXTERN_C Void_XTOR_15__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__ abort(void);} \
		}while(0);														\
	t_y_p_e_15__##_dtor(&v_a_r_15__);									\
	t_y_p_e_14__##_dtor(&v_a_r_14__);									\
	t_y_p_e_13__##_dtor(&v_a_r_13__);									\
	t_y_p_e_12__##_dtor(&v_a_r_12__);									\
	t_y_p_e_11__##_dtor(&v_a_r_11__);									\
	t_y_p_e_10__##_dtor(&v_a_r_10__);									\
	t_y_p_e_9__##_dtor(&v_a_r_9__);										\
	t_y_p_e_8__##_dtor(&v_a_r_8__);										\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_16(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__,							\
				   t_y_p_e_15__, v_a_r_15__,							\
				   t_y_p_e_16__, v_a_r_16__)							\
	{EXTERN_C Void_XTOR_16__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__##v_a_r_16__ abort(void);} \
		}while(0);														\
	t_y_p_e_16__##_dtor(&v_a_r_16__);									\
	t_y_p_e_15__##_dtor(&v_a_r_15__);									\
	t_y_p_e_14__##_dtor(&v_a_r_14__);									\
	t_y_p_e_13__##_dtor(&v_a_r_13__);									\
	t_y_p_e_12__##_dtor(&v_a_r_12__);									\
	t_y_p_e_11__##_dtor(&v_a_r_11__);									\
	t_y_p_e_10__##_dtor(&v_a_r_10__);									\
	t_y_p_e_9__##_dtor(&v_a_r_9__);										\
	t_y_p_e_8__##_dtor(&v_a_r_8__);										\
	t_y_p_e_7__##_dtor(&v_a_r_7__);										\
	t_y_p_e_6__##_dtor(&v_a_r_6__);										\
	t_y_p_e_5__##_dtor(&v_a_r_5__);										\
	t_y_p_e_4__##_dtor(&v_a_r_4__);										\
	t_y_p_e_3__##_dtor(&v_a_r_3__);										\
	t_y_p_e_2__##_dtor(&v_a_r_2__);										\
	t_y_p_e_1__##_dtor(&v_a_r_1__);										\
	}}}while(0)
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define BEG_PTR_A(t_y_p_e_1__, v_a_r_1__, n_e_l_m_s__)					\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__[n_e_l_m_s__];							\
	{int ctr_i__; for (ctr_i__ = 0; ctr_i__ < (n_e_l_m_s__); ++ctr_i__) { \
			v_a_r_1__[ctr_i__] = t_y_p_e_1__##_new();					\
		}}																\
		do{																\
		typedef void Void_PXTOR_A__##v_a_r_1__##n_e_l_m_s__ ;			\
		EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_1(t_y_p_e_1__, v_a_r_1__)			\
	do{{{{{											\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();	\
	do{												\
	typedef void Void_PXTOR_1__##v_a_r_1__ ;		\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_2(t_y_p_e_1__, v_a_r_1__,				\
				  t_y_p_e_2__, v_a_r_2__)				\
	do{{{{{												\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();	\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();	\
	do{													\
	typedef void Void_PXTOR_2__##v_a_r_1__##v_a_r_2__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_3(t_y_p_e_1__, v_a_r_1__,							\
				  t_y_p_e_2__, v_a_r_2__,							\
				  t_y_p_e_3__, v_a_r_3__)							\
	do{{{{{															\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();			\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();			\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();			\
	do{																\
	typedef void Void_PXTOR_3__##v_a_r_1__##v_a_r_2__##v_a_r_3__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_4(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__)								\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	do{																	\
	typedef void Void_PXTOR_4__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_5(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__)								\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	do{																	\
	typedef void Void_PXTOR_5__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_6(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__)								\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	do{																	\
	typedef void Void_PXTOR_6__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_7(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__)								\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	do{																	\
	typedef void Void_PXTOR_7__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_8(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__,								\
				  t_y_p_e_8__, v_a_r_8__)								\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	struct t_y_p_e_8__* v_a_r_8__ = t_y_p_e_8__##_new();				\
	do{																	\
	typedef void Void_PXTOR_8__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_9(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__,								\
				  t_y_p_e_8__, v_a_r_8__,								\
				  t_y_p_e_9__, v_a_r_9__)								\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	struct t_y_p_e_8__* v_a_r_8__ = t_y_p_e_8__##_new();				\
	struct t_y_p_e_9__* v_a_r_9__ = t_y_p_e_9__##_new();				\
	do{																	\
	typedef void Void_PXTOR_9__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_10(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__)							\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	struct t_y_p_e_8__* v_a_r_8__ = t_y_p_e_8__##_new();				\
	struct t_y_p_e_9__* v_a_r_9__ = t_y_p_e_9__##_new();				\
	struct t_y_p_e_10__* v_a_r_10__ = t_y_p_e_10__##_new();				\
	do{																	\
	typedef void Void_PXTOR_10__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_11(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__)							\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	struct t_y_p_e_8__* v_a_r_8__ = t_y_p_e_8__##_new();				\
	struct t_y_p_e_9__* v_a_r_9__ = t_y_p_e_9__##_new();				\
	struct t_y_p_e_10__* v_a_r_10__ = t_y_p_e_10__##_new();				\
	struct t_y_p_e_11__* v_a_r_11__ = t_y_p_e_11__##_new();				\
	do{																	\
	typedef void Void_PXTOR_11__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_12(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__)							\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	struct t_y_p_e_8__* v_a_r_8__ = t_y_p_e_8__##_new();				\
	struct t_y_p_e_9__* v_a_r_9__ = t_y_p_e_9__##_new();				\
	struct t_y_p_e_10__* v_a_r_10__ = t_y_p_e_10__##_new();				\
	struct t_y_p_e_11__* v_a_r_11__ = t_y_p_e_11__##_new();				\
	struct t_y_p_e_12__* v_a_r_12__ = t_y_p_e_12__##_new();				\
	do{																	\
	typedef void Void_PXTOR_12__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_13(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__)							\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	struct t_y_p_e_8__* v_a_r_8__ = t_y_p_e_8__##_new();				\
	struct t_y_p_e_9__* v_a_r_9__ = t_y_p_e_9__##_new();				\
	struct t_y_p_e_10__* v_a_r_10__ = t_y_p_e_10__##_new();				\
	struct t_y_p_e_11__* v_a_r_11__ = t_y_p_e_11__##_new();				\
	struct t_y_p_e_12__* v_a_r_12__ = t_y_p_e_12__##_new();				\
	struct t_y_p_e_13__* v_a_r_13__ = t_y_p_e_13__##_new();				\
	do{																	\
	typedef void Void_PXTOR_13__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_14(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__)							\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	struct t_y_p_e_8__* v_a_r_8__ = t_y_p_e_8__##_new();				\
	struct t_y_p_e_9__* v_a_r_9__ = t_y_p_e_9__##_new();				\
	struct t_y_p_e_10__* v_a_r_10__ = t_y_p_e_10__##_new();				\
	struct t_y_p_e_11__* v_a_r_11__ = t_y_p_e_11__##_new();				\
	struct t_y_p_e_12__* v_a_r_12__ = t_y_p_e_12__##_new();				\
	struct t_y_p_e_13__* v_a_r_13__ = t_y_p_e_13__##_new();				\
	struct t_y_p_e_14__* v_a_r_14__ = t_y_p_e_14__##_new();				\
	do{																	\
	typedef void Void_PXTOR_14__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_15(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__,							\
				   t_y_p_e_15__, v_a_r_15__)							\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	struct t_y_p_e_8__* v_a_r_8__ = t_y_p_e_8__##_new();				\
	struct t_y_p_e_9__* v_a_r_9__ = t_y_p_e_9__##_new();				\
	struct t_y_p_e_10__* v_a_r_10__ = t_y_p_e_10__##_new();				\
	struct t_y_p_e_11__* v_a_r_11__ = t_y_p_e_11__##_new();				\
	struct t_y_p_e_12__* v_a_r_12__ = t_y_p_e_12__##_new();				\
	struct t_y_p_e_13__* v_a_r_13__ = t_y_p_e_13__##_new();				\
	struct t_y_p_e_14__* v_a_r_14__ = t_y_p_e_14__##_new();				\
	struct t_y_p_e_15__* v_a_r_15__ = t_y_p_e_15__##_new();				\
	do{																	\
	typedef void Void_PXTOR_15__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_PTR_16(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__,							\
				   t_y_p_e_15__, v_a_r_15__,							\
				   t_y_p_e_16__, v_a_r_16__)							\
	do{{{{{																\
	struct t_y_p_e_1__* v_a_r_1__ = t_y_p_e_1__##_new();				\
	struct t_y_p_e_2__* v_a_r_2__ = t_y_p_e_2__##_new();				\
	struct t_y_p_e_3__* v_a_r_3__ = t_y_p_e_3__##_new();				\
	struct t_y_p_e_4__* v_a_r_4__ = t_y_p_e_4__##_new();				\
	struct t_y_p_e_5__* v_a_r_5__ = t_y_p_e_5__##_new();				\
	struct t_y_p_e_6__* v_a_r_6__ = t_y_p_e_6__##_new();				\
	struct t_y_p_e_7__* v_a_r_7__ = t_y_p_e_7__##_new();				\
	struct t_y_p_e_8__* v_a_r_8__ = t_y_p_e_8__##_new();				\
	struct t_y_p_e_9__* v_a_r_9__ = t_y_p_e_9__##_new();				\
	struct t_y_p_e_10__* v_a_r_10__ = t_y_p_e_10__##_new();				\
	struct t_y_p_e_11__* v_a_r_11__ = t_y_p_e_11__##_new();				\
	struct t_y_p_e_12__* v_a_r_12__ = t_y_p_e_12__##_new();				\
	struct t_y_p_e_13__* v_a_r_13__ = t_y_p_e_13__##_new();				\
	struct t_y_p_e_14__* v_a_r_14__ = t_y_p_e_14__##_new();				\
	struct t_y_p_e_15__* v_a_r_15__ = t_y_p_e_15__##_new();				\
	struct t_y_p_e_16__* v_a_r_16__ = t_y_p_e_16__##_new();				\
	do{																	\
	typedef void Void_PXTOR_16__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__##v_a_r_16__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define END_PTR_A(t_y_p_e_1__, v_a_r_1__, n_e_l_m_s__)					\
	{EXTERN_C Void_PXTOR_A__##v_a_r_1__##n_e_l_m_s__ abort(void);}		\
		}while(0);														\
	{int ctr_i__; for (ctr_i__ = (n_e_l_m_s__)-1; ctr_i__ >= 0 ; --ctr_i__) { \
			t_y_p_e_1__##_delete(v_a_r_1__[ctr_i__]);						\
		}}																\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_1(t_y_p_e_1__, v_a_r_1__)				\
	{EXTERN_C Void_PXTOR_1__##v_a_r_1__ abort(void);}	\
		}while(0);										\
	t_y_p_e_1__##_delete(v_a_r_1__);						\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_2(t_y_p_e_1__, v_a_r_1__,							\
				  t_y_p_e_2__, v_a_r_2__)							\
	{EXTERN_C Void_PXTOR_2__##v_a_r_1__##v_a_r_2__ abort(void);}	\
		}while(0);													\
	t_y_p_e_2__##_delete(v_a_r_2__);									\
	t_y_p_e_1__##_delete(v_a_r_1__);									\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_3(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__)								\
	{EXTERN_C Void_PXTOR_3__##v_a_r_1__##v_a_r_2__##v_a_r_3__ abort(void);} \
		}while(0);														\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_4(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__)								\
	{EXTERN_C Void_PXTOR_4__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__ abort(void);} \
		}while(0);														\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_5(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__)								\
	{EXTERN_C Void_PXTOR_5__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__ abort(void);} \
		}while(0);														\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_6(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__)								\
	{EXTERN_C Void_PXTOR_6__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__ abort(void);} \
		}while(0);														\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_7(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__)								\
	{EXTERN_C Void_PXTOR_7__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__ abort(void);} \
		}while(0);														\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_8(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__,								\
				  t_y_p_e_8__, v_a_r_8__)								\
	{EXTERN_C Void_PXTOR_8__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__ abort(void);} \
		}while(0);														\
	t_y_p_e_8__##_delete(v_a_r_8__);										\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_9(t_y_p_e_1__, v_a_r_1__,								\
				  t_y_p_e_2__, v_a_r_2__,								\
				  t_y_p_e_3__, v_a_r_3__,								\
				  t_y_p_e_4__, v_a_r_4__,								\
				  t_y_p_e_5__, v_a_r_5__,								\
				  t_y_p_e_6__, v_a_r_6__,								\
				  t_y_p_e_7__, v_a_r_7__,								\
				  t_y_p_e_8__, v_a_r_8__,								\
				  t_y_p_e_9__, v_a_r_9__)								\
	{EXTERN_C Void_PXTOR_9__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__ abort(void);} \
		}while(0);														\
	t_y_p_e_9__##_delete(v_a_r_9__);										\
	t_y_p_e_8__##_delete(v_a_r_8__);										\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_10(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__)							\
	{EXTERN_C Void_PXTOR_10__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__ abort(void);} \
		}while(0);														\
	t_y_p_e_10__##_delete(v_a_r_10__);										\
	t_y_p_e_9__##_delete(v_a_r_9__);										\
	t_y_p_e_8__##_delete(v_a_r_8__);										\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_11(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__)							\
	{EXTERN_C Void_PXTOR_11__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__ abort(void);} \
		}while(0);														\
	t_y_p_e_11__##_delete(v_a_r_11__);										\
	t_y_p_e_10__##_delete(v_a_r_10__);										\
	t_y_p_e_9__##_delete(v_a_r_9__);										\
	t_y_p_e_8__##_delete(v_a_r_8__);										\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_12(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__)							\
	{EXTERN_C Void_PXTOR_12__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__ abort(void);} \
		}while(0);														\
	t_y_p_e_12__##_delete(v_a_r_12__);										\
	t_y_p_e_11__##_delete(v_a_r_11__);										\
	t_y_p_e_10__##_delete(v_a_r_10__);										\
	t_y_p_e_9__##_delete(v_a_r_9__);										\
	t_y_p_e_8__##_delete(v_a_r_8__);										\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_13(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__)							\
	{EXTERN_C Void_PXTOR_13__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__ abort(void);} \
		}while(0);														\
	t_y_p_e_13__##_delete(v_a_r_13__);										\
	t_y_p_e_12__##_delete(v_a_r_12__);										\
	t_y_p_e_11__##_delete(v_a_r_11__);										\
	t_y_p_e_10__##_delete(v_a_r_10__);										\
	t_y_p_e_9__##_delete(v_a_r_9__);										\
	t_y_p_e_8__##_delete(v_a_r_8__);										\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_14(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__)							\
	{EXTERN_C Void_PXTOR_14__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__ abort(void);} \
		}while(0);														\
	t_y_p_e_14__##_delete(v_a_r_14__);										\
	t_y_p_e_13__##_delete(v_a_r_13__);										\
	t_y_p_e_12__##_delete(v_a_r_12__);										\
	t_y_p_e_11__##_delete(v_a_r_11__);										\
	t_y_p_e_10__##_delete(v_a_r_10__);										\
	t_y_p_e_9__##_delete(v_a_r_9__);										\
	t_y_p_e_8__##_delete(v_a_r_8__);										\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_15(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__,							\
				   t_y_p_e_15__, v_a_r_15__)							\
	{EXTERN_C Void_PXTOR_15__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__ abort(void);} \
		}while(0);														\
	t_y_p_e_15__##_delete(v_a_r_15__);										\
	t_y_p_e_14__##_delete(v_a_r_14__);										\
	t_y_p_e_13__##_delete(v_a_r_13__);										\
	t_y_p_e_12__##_delete(v_a_r_12__);										\
	t_y_p_e_11__##_delete(v_a_r_11__);										\
	t_y_p_e_10__##_delete(v_a_r_10__);										\
	t_y_p_e_9__##_delete(v_a_r_9__);										\
	t_y_p_e_8__##_delete(v_a_r_8__);										\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_PTR_16(t_y_p_e_1__, v_a_r_1__,								\
				   t_y_p_e_2__, v_a_r_2__,								\
				   t_y_p_e_3__, v_a_r_3__,								\
				   t_y_p_e_4__, v_a_r_4__,								\
				   t_y_p_e_5__, v_a_r_5__,								\
				   t_y_p_e_6__, v_a_r_6__,								\
				   t_y_p_e_7__, v_a_r_7__,								\
				   t_y_p_e_8__, v_a_r_8__,								\
				   t_y_p_e_9__, v_a_r_9__,								\
				   t_y_p_e_10__, v_a_r_10__,							\
				   t_y_p_e_11__, v_a_r_11__,							\
				   t_y_p_e_12__, v_a_r_12__,							\
				   t_y_p_e_13__, v_a_r_13__,							\
				   t_y_p_e_14__, v_a_r_14__,							\
				   t_y_p_e_15__, v_a_r_15__,							\
				   t_y_p_e_16__, v_a_r_16__)							\
	{EXTERN_C Void_PXTOR_16__##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__##v_a_r_16__ abort(void);} \
		}while(0);														\
	t_y_p_e_16__##_delete(v_a_r_16__);										\
	t_y_p_e_15__##_delete(v_a_r_15__);										\
	t_y_p_e_14__##_delete(v_a_r_14__);										\
	t_y_p_e_13__##_delete(v_a_r_13__);										\
	t_y_p_e_12__##_delete(v_a_r_12__);										\
	t_y_p_e_11__##_delete(v_a_r_11__);										\
	t_y_p_e_10__##_delete(v_a_r_10__);										\
	t_y_p_e_9__##_delete(v_a_r_9__);										\
	t_y_p_e_8__##_delete(v_a_r_8__);										\
	t_y_p_e_7__##_delete(v_a_r_7__);										\
	t_y_p_e_6__##_delete(v_a_r_6__);										\
	t_y_p_e_5__##_delete(v_a_r_5__);										\
	t_y_p_e_4__##_delete(v_a_r_4__);										\
	t_y_p_e_3__##_delete(v_a_r_3__);										\
	t_y_p_e_2__##_delete(v_a_r_2__);										\
	t_y_p_e_1__##_delete(v_a_r_1__);										\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#endif
