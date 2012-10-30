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
#ifndef umalccvbg_bigint_cdtor_h__
#define umalccvbg_bigint_cdtor_h__	1
/*------------------------------------------------------------------------*/
#include "bigint.h"
/*------------------------------------------------------------------------*/
#ifndef EXTERN_C
#ifdef __cplusplus
#define BEGIN_EXTERN_C	extern "C" {
#define END_EXTERN_C	}
#define EXTERN_C		extern "C"
#else
#define BEGIN_EXTERN_C
#define END_EXTERN_C
#define EXTERN_C		extern
#endif
#endif
/*------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define BEG_VAR_A_bigint_t(v_a_r_1__, n_e_l_m_s__)							\
	do{{{{{																\
	bigint_t v_a_r_1__[n_e_l_m_s__];									\
	{int ctr_i__; for (ctr_i__ = 0; ctr_i__ < (n_e_l_m_s__); ++ctr_i__) { \
		bi_ctor(v_a_r_1__[ctr_i__]);									\
	}}																	\
	do{																	\
	typedef void Void_XTOR_A_bigint_t_##v_a_r_1__##n_e_l_m_s__ ;		\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_1_bigint_t(v_a_r_1__)					\
	do{{{{{											\
	bigint_t v_a_r_1__;								\
	bi_ctor(v_a_r_1__);								\
	do{												\
	typedef void Void_XTOR_1_bigint_t_##v_a_r_1__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_2_bigint_t(v_a_r_1__,								\
						v_a_r_2__)								\
	do{{{{{														\
	bigint_t v_a_r_1__;											\
	bigint_t v_a_r_2__;											\
	bi_ctor(v_a_r_1__);											\
	bi_ctor(v_a_r_2__);											\
	do{															\
	typedef void Void_XTOR_2_bigint_t_##v_a_r_1__##v_a_r_2__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_3_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	do{																	\
	typedef void Void_XTOR_3_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_4_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	do{																	\
	typedef void Void_XTOR_4_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_5_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	do{																	\
	typedef void Void_XTOR_5_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_6_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	do{																	\
	typedef void Void_XTOR_6_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_7_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	do{																	\
	typedef void Void_XTOR_7_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_8_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__,										\
						v_a_r_8__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	do{																	\
	typedef void Void_XTOR_8_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_9_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__,										\
						v_a_r_8__,										\
						v_a_r_9__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	do{																	\
	typedef void Void_XTOR_9_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_10_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	do{																	\
	typedef void Void_XTOR_10_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_11_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	do{																	\
	typedef void Void_XTOR_11_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_12_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	do{																	\
	typedef void Void_XTOR_12_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_13_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bigint_t v_a_r_13__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	bi_ctor(v_a_r_13__);												\
	do{																	\
	typedef void Void_XTOR_13_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_14_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bigint_t v_a_r_13__;												\
	bigint_t v_a_r_14__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	bi_ctor(v_a_r_13__);												\
	bi_ctor(v_a_r_14__);												\
	do{																	\
	typedef void Void_XTOR_14_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_15_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__,									\
						 v_a_r_15__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bigint_t v_a_r_13__;												\
	bigint_t v_a_r_14__;												\
	bigint_t v_a_r_15__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	bi_ctor(v_a_r_13__);												\
	bi_ctor(v_a_r_14__);												\
	bi_ctor(v_a_r_15__);												\
	do{																	\
	typedef void Void_XTOR_15_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_16_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__,									\
						 v_a_r_15__,									\
						 v_a_r_16__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bigint_t v_a_r_13__;												\
	bigint_t v_a_r_14__;												\
	bigint_t v_a_r_15__;												\
	bigint_t v_a_r_16__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	bi_ctor(v_a_r_13__);												\
	bi_ctor(v_a_r_14__);												\
	bi_ctor(v_a_r_15__);												\
	bi_ctor(v_a_r_16__);												\
	do{																	\
	typedef void Void_XTOR_16_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__##v_a_r_16__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define END_VAR_A_bigint_t(v_a_r_1__, n_e_l_m_s__)							\
	{EXTERN_C Void_XTOR_A_bigint_t_##v_a_r_1__##n_e_l_m_s__ abort(void);} \
	}while(0);															\
	{int ctr_i__; for (ctr_i__ = (n_e_l_m_s__)-1; ctr_i__ >= 0 ; --ctr_i__) { \
			bi_dtor(v_a_r_1__[ctr_i__]);								\
	}}																	\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_1_bigint_t(v_a_r_1__)										\
	{EXTERN_C Void_XTOR_1_bigint_t_##v_a_r_1__ abort(void);}			\
	}while(0);															\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_2_bigint_t(v_a_r_1__,										\
						v_a_r_2__)										\
	{EXTERN_C Void_XTOR_2_bigint_t_##v_a_r_1__##v_a_r_2__ abort(void);}	\
		}while(0);														\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_3_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__)										\
	{EXTERN_C Void_XTOR_3_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_4_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__)										\
	{EXTERN_C Void_XTOR_4_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_5_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__)										\
	{EXTERN_C Void_XTOR_5_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_6_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__)										\
	{EXTERN_C Void_XTOR_6_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_7_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__)										\
	{EXTERN_C Void_XTOR_7_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_8_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__,										\
						v_a_r_8__)										\
	{EXTERN_C Void_XTOR_8_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_9_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__,										\
						v_a_r_8__,										\
						v_a_r_9__)										\
	{EXTERN_C Void_XTOR_9_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_10_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__)									\
	{EXTERN_C Void_XTOR_10_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_11_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__)									\
	{EXTERN_C Void_XTOR_11_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_12_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__)									\
	{EXTERN_C Void_XTOR_12_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_13_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__)									\
	{EXTERN_C Void_XTOR_13_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_13__);												\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_14_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__)									\
	{EXTERN_C Void_XTOR_14_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_14__);												\
	bi_dtor(v_a_r_13__);												\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_15_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__,									\
						 v_a_r_15__)									\
	{EXTERN_C Void_XTOR_15_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_15__);												\
	bi_dtor(v_a_r_14__);												\
	bi_dtor(v_a_r_13__);												\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_16_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__,									\
						 v_a_r_15__,									\
						 v_a_r_16__)									\
	{EXTERN_C Void_XTOR_16_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__##v_a_r_16__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_16__);												\
	bi_dtor(v_a_r_15__);												\
	bi_dtor(v_a_r_14__);												\
	bi_dtor(v_a_r_13__);												\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/

	/*-------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
#endif
/*------------------------------------------------------------------*/
