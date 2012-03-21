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
#ifndef umalccvbg_data_h__
#define umalccvbg_data_h__	1
#ifdef __cplusplus
extern "C" {
#endif
	/*------------------------------------------------------------------------*/
	typedef struct data_t {
		unsigned size;
		char* data_;	/* size */
	} data_t;
	/*------------------------------------------------------------------------*/
	void data_reserve(data_t* buff, unsigned sz);
	void data_reset(data_t* buff);
	void data_copy(data_t* buff, void* dat, unsigned datlen);
	/*------------------------------------------------------------------------*/
#define data_copy_buf(data, b)	data_copy(data, (b)->data_, (b)->size)
	/*------------------------------------------------------------------------*/
	void data_t_swap(struct data_t* p, struct data_t* o);
	void data_t_ctor(struct data_t* p);
	void data_t_dtor(struct data_t* p);
	void data_t_asg(struct data_t* p, const struct data_t* o);
	struct data_t* data_t_new();
	struct data_t* data_t_clone(const struct data_t* o);
	void data_t_delete(struct data_t* p);
	/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

