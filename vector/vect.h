/*------------------------------------------------------------------------*/
/* Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain       */
/*                                                                        */
/* This file is part of the <FTMGS> Library	                              */
/*                                                                        */
/* This library is free software: you can redistribute it and/or          */
/* modify it under the terms of the GNU Lesser General Public             */
/* License as published by the Free Software Foundation, either           */
/* version 2.1 of the License, or (at your option) any later              */
/* version.                                                               */
/*                                                                        */
/* This library is distributed in the hope that it will be useful,        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of         */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           */
/* GNU Lesser General Public License for more details.                    */
/*                                                                        */
/* You should have received a copy of the GNU Lesser General              */
/* Public License along with this library. If not, see                    */
/* <http://www.gnu.org/licenses/>.                                        */
/*------------------------------------------------------------------------*/
#ifndef umalccvbg_vect_h__
#define umalccvbg_vect_h__	1
#include <string.h>
#ifndef NDEBUG
#include <assert.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif
	/*--------------------------------------------------------------------*/
	/*--------------------------------------------------------------------*/
	/* to be used within typedef */
#define vect_t(type__)									\
	struct { int nelms; type__* data_; int max_nelms; }
	/*--------------------------------------------------------------------*/
#define vect_data(v__)		((v__)->data_)
	/*--------------------------------------------------------------------*/
#define vect_nelms(v__)		((v__)->nelms)
	/*--------------------------------------------------------------------*/
#define vect_max_nelms(v__)	((v__)->max_nelms)
	/*--------------------------------------------------------------------*/
#define vect_create(v__)												\
	do{ vect_nelms(v__)=0; vect_data(v__)=0; vect_max_nelms(v__)=0; }while(0)
	/*--------------------------------------------------------------------*/
#define vect_is_created(v__)												\
	((vect_nelms(v__)==0)&&(vect_data(v__)==0)&&(vect_max_nelms(v__)==0))
	/*--------------------------------------------------------------------*/
#define vect_destroy(v__)								\
	do{ free(vect_data(v__)); vect_create(v__); }while(0)
	/*--------------------------------------------------------------------*/
#define vect_reserve(v__, n__)										\
	vect_reserve_(v__, n__, sizeof(*vect_data(v__)))
	/*------------------------------*/
	void vect_reserve_(void* v, int sz__, int elmsz);
	/*--------------------------------------------------------------------*/
#define vect_shrink(v__)										\
	vect_shrink_(v__, sizeof(*vect_data(v__)))
	/*------------------------------*/
	void vect_shrink_(void* v, int elmsz);
	/*--------------------------------------------------------------------*/
#define vect_reset(v__)	vect_destroy(v__)
	/*--------------------------------------------------------------------*/
#define vect_clear(v__)	do{ vect_nelms(v__) = 0; }while(0)
	/*--------------------------------------------------------------------*/
/*
 * #define vect_resize(v__, n__)								\
 * 	do{															\
 * 		int nsz__ = (n__);										\
 * 		if ((nsz__) > vect_max_nelms(v__)) {					\
 * 			vect_reserve((v__), 2*(nsz__));						\
 * 		}														\
 * 		if ((nsz__) <= vect_max_nelms(v__)) {					\
 * 			vect_nelms(v__) = (nsz__);							\
 * 		}														\
 * 	}while(0)
 */
	/*--------------------------------------------------------------------*/
#define vect_resize(v__, n__)										\
	vect_resize_(v__, n__, sizeof(*vect_data(v__)))
	/*------------------------------*/
	void vect_resize_(void* v, int sz__, int elmsz);
	/*--------------------------------------------------------------------*/
#define vect_elm(v__, i__)		(vect_data(v__)[(i__)])
	/*--------------------------------------------------------------------*/
#define vect_elms_zero(v__)												\
	(memset(vect_data(v__), 0, vect_nelms(v__)*sizeof(*vect_data(v__))))
	/*--------------------------------------------------------------------*/
#ifndef NDEBUG
#define vect_pop_elm(v__)												\
	((void)assert(vect_nelms(v__)>0), (vect_elm(v__, (--vect_nelms(v__)))))
#else
#define vect_pop_elm(v__)	(vect_elm(v__, (--vect_nelms(v__))))
#endif
	/*--------------------------------------------------------------------*/
#ifndef NDEBUG
#define vect_top_elm(v__)											\
	((void)assert(vect_nelms(v__)>0), (vect_elm(v__, (vect_nelms(v__)-1))))
#else
#define vect_top_elm(v__)	(vect_elm(v__, (vect_nelms(v__)-1)))
#endif
	/*--------------------------------------------------------------------*/
#ifndef NDEBUG
#define vect_pop(v__)												\
	((void)assert(vect_nelms(v__)>0), (--vect_nelms(v__)))
#else
#define vect_pop(v__)		(--vect_nelms(v__))
#endif
	/*--------------------------------------------------------------------*/
#define vect_pop_check(v__)						\
	do{											\
		if (vect_nelms(v__) > 0) {				\
			--vect_nelms(v__);					\
		}										\
	}while(0)
	/*--------------------------------------------------------------------*/
/*
 * #define vect_push(v__, o__)							\
 * 	do{													\
 * 		if (vect_nelms(v__) >= vect_max_nelms(v__)) {	\
 * 			vect_reserve((v__), 1+(2*vect_nelms(v__)));	\
 * 		}												\
 * 		if (vect_nelms(v__) < vect_max_nelms(v__)) {	\
 * 			vect_elm(v__, vect_nelms(v__)++) = (o__);	\
 * 		}												\
 * 	}while(0)
 */
	/*--------------------------------------------------------------------*/
#define vect_push(v__, o__)									\
	do{														\
		void* dt__;											\
		switch (vect_nelms(v__) < vect_max_nelms(v__)) {	\
		case 0:												\
			dt__ = realloc(vect_data(v__),					\
						   (1+(2*vect_nelms(v__)))			\
						   *sizeof(*vect_data(v__)));		\
			if (dt__ == NULL) {								\
				break;										\
			}												\
			vect_data(v__) = dt__;							\
			vect_max_nelms(v__) = (1+(2*vect_nelms(v__)));	\
		default:											\
			vect_elm(v__, vect_nelms(v__)++) = (o__);		\
		}													\
	}while(0)
	/*--------------------------------------------------------------------*/
#define vect_resize_add(v__, n__)					\
	do{												\
		vect_resize(v__, vect_nelms(v__)+(n__)); 	\
	}while(0)
	/*--------------------------------------------------------------------*/
#define vect_reserve_add(v__, n__)					\
	do{												\
		vect_reserve(v__, vect_nelms(v__)+(n__)); 	\
	}while(0)
	/*--------------------------------------------------------------------*/
	/*-- Ctor/Dtor/etc ---------------------------------------------------*/
	/*--------------------------------------------------------------------*/
#define vect_t_ctor(v__)		vect_create(v__)
	/*--------------------------------------------------------------------*/
	void vect_t_dtor(void* v);
	/*--------------------------------------------------------------------*/
#define vect_t_dtor_data(type__, v__)									\
	vect_t_dtor_dt_(v__, sizeof(*vect_data(v__)), (void(*)(void *))type__##_dtor)
	/*------------------------------*/
	void vect_t_dtor_dt_(void* v, int elmsz, void (*dtor)(void*));
	/*--------------------------------------------------------------------*/
#define vect_t_dtor_ptr_data(type__, v__)		\
	vect_t_dtor_ptr_dt_(v__, (void(*)(void *))type__##_dtor)
	/*------------------------------*/
	void vect_t_dtor_ptr_dt_(void* v, void (*dtor)(void*));
	/*--------------------------------------------------------------------*/
#define vect_t_move(d__, o__)					\
	vect_t_mv_(d__, o__)
	/*------------------------------*/
	void vect_t_mv_(void* d, void* o);
	/*--------------------------------------------------------------------*/
#define vect_t_asg(d__, o__)						\
	vect_t_asg_(d__, o__, sizeof(*vect_data(d__)))
	/*------------------------------*/
	void vect_t_asg_(void* d, const void* o, int elmsz);
	/*--------------------------------------------------------------------*/
#define vect_t_asg_data(type__, d__, o__)						\
	vect_t_asg_dt_(d__, o__, sizeof(*vect_data(d__)),			\
				   (void(*)(void *))type__##_dtor,				\
				   (void (*)(void*,const void*))type__##_asg)
	/*------------------------------*/
	void vect_t_asg_dt_(void* d, const void* o, int elmsz,
						void (*dtor)(void*),
						void (*asg)(void*, const void*));
	/*--------------------------------------------------------------------*/
#define vect_t_asg_ptr_data(type__, d__, o__)						\
	vect_t_asg_ptr_dt_(d__, o__, sizeof(**vect_data(d__)),			\
					   (void(*)(void *))type__##_dtor,				\
					   (void (*)(void*,const void*))type__##_asg)
	/*------------------------------*/
	void vect_t_asg_ptr_dt_(void* d, const void* o, int elmsz,
							void (*dtor)(void*),
							void (*asg)(void*, const void*));
	/*--------------------------------------------------------------------*/
	/*-- Usual Vector Definitions ----------------------------------------*/
	/*--------------------------------------------------------------------*/
#ifndef vect_char_t_defined__
#define vect_char_t_defined__
	typedef vect_t(char)	vect_char_t;
#define vect_char_t_ctor(v__)		vect_t_ctor(v__)
#define vect_char_t_dtor			vect_t_dtor
#define vect_char_t_asg(d__, o__)	vect_t_asg(d__, o__)
#define vect_char_t_move(d__, o__)	vect_t_move(d__, o__)
#endif
#ifndef vect_short_t_defined__
#define vect_short_t_defined__
	typedef vect_t(short)	vect_short_t;
#define vect_short_t_ctor(v__)		vect_t_ctor(v__)
#define vect_short_t_dtor			vect_t_dtor
#define vect_short_t_asg(d__, o__)	vect_t_asg(d__, o__)
#define vect_short_t_move(d__, o__)	vect_t_move(d__, o__)
#endif
#ifndef vect_int_t_defined__
#define vect_int_t_defined__
	typedef vect_t(int)	vect_int_t;
#define vect_int_t_ctor(v__)		vect_t_ctor(v__)
#define vect_int_t_dtor				vect_t_dtor
#define vect_int_t_asg(d__, o__)	vect_t_asg(d__, o__)
#define vect_int_t_move(d__, o__)	vect_t_move(d__, o__)
#endif
#ifndef vect_uint_t_defined__
#define vect_uint_t_defined__
	typedef vect_t(unsigned)	vect_uint_t;
#define vect_uint_t_ctor(v__)		vect_t_ctor(v__)
#define vect_uint_t_dtor			vect_t_dtor
#define vect_uint_t_asg(d__, o__)	vect_t_asg(d__, o__)
#define vect_uint_t_move(d__, o__)	vect_t_move(d__, o__)
#endif
#ifndef vect_long_t_defined__
#define vect_long_t_defined__
	typedef vect_t(long)	vect_long_t;
#define vect_long_t_ctor(v__)		vect_t_ctor(v__)
#define vect_long_t_dtor			vect_t_dtor
#define vect_long_t_asg(d__, o__)	vect_t_asg(d__, o__)
#define vect_long_t_move(d__, o__)	vect_t_move(d__, o__)
#endif
#ifndef vect_float_t_defined__
#define vect_float_t_defined__
	typedef vect_t(float)	vect_float_t;
#define vect_float_t_ctor(v__)		vect_t_ctor(v__)
#define vect_float_t_dtor			vect_t_dtor
#define vect_float_t_asg(d__, o__)	vect_t_asg(d__, o__)
#define vect_float_t_move(d__, o__)	vect_t_move(d__, o__)
#endif
#ifndef vect_double_t_defined__
#define vect_double_t_defined__
	typedef vect_t(double)	vect_double_t;
#define vect_double_t_ctor(v__)			vect_t_ctor(v__)
#define vect_double_t_dtor				vect_t_dtor
#define vect_double_t_asg(d__, o__)		vect_t_asg(d__, o__)
#define vect_double_t_move(d__, o__)	vect_t_move(d__, o__)
#endif
#ifndef vect_str_t_defined__
#define vect_str_t_defined__
	typedef vect_t(char*)	vect_str_t;
#define vect_str_t_ctor(v__)		vect_t_ctor(v__)
#define vect_str_t_dtor				vect_t_dtor_str_
#define vect_str_t_asg(d__, o__)	vect_t_asg_str_(d__, o__)
#define vect_str_t_move(d__, o__)	vect_t_move(d__, o__)
#endif
#ifndef vect_ptr_t_defined__
#define vect_ptr_t_defined__
	typedef vect_t(void*)	vect_ptr_t;
#define vect_ptr_t_ctor(v__)		vect_t_ctor(v__)
#define vect_ptr_t_dtor				vect_t_dtor
#define vect_ptr_t_asg(d__, o__)	vect_t_asg(d__, o__)
#define vect_ptr_t_move(d__, o__)	vect_t_move(d__, o__)
#endif
	/*--------------------------------------------------------------------*/
	/*--------------------------------------------------------------------*/
	void vect_t_dtor_str_(vect_str_t* v__);
	/*--------------------------------------------------------------------*/
	void vect_t_asg_str_(vect_str_t* d__, const vect_str_t* o__);
	/*--------------------------------------------------------------------*/
	/*--------------------------------------------------------------------*/
	/*--------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif
