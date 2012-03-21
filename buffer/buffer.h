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
#ifndef umalccvbg_buffer_h__
#define umalccvbg_buffer_h__	1
#ifdef __cplusplus
extern "C" {
#endif
	/*------------------------------------------------------------------------*/
	struct buffer_t {
		unsigned size;
		unsigned max_size;
		char* data_;	/* size max_size */
	};
	/*------------------------------------------------------------------------*/
	char* buffer_data(const struct buffer_t* buff);
	unsigned buffer_size(const struct buffer_t* buff);
	unsigned buffer_maxsize(const struct buffer_t* buff);
	/*------------------------------------------------------------------------*/
	void buffer_reserve(struct buffer_t* buff, unsigned maxsz);
	void buffer_shrink(struct buffer_t* buff);
	void buffer_reset(struct buffer_t* buff);
	/*--------------------------------*/
	void buffer_clear(struct buffer_t* buff);
	void buffer_resize(struct buffer_t* buff, unsigned newsz);
	void buffer_push(struct buffer_t* buff, const void* dat, unsigned datlen);
	void buffer_push_str(struct buffer_t* buff, const char* str);
	/*------------------------------------------------------------------------*/
#ifndef buffer_push_buf
#define buffer_push_buf(buffer, b)							\
	buffer_push((buffer), buffer_data(b), buffer_size(b))
#define buffer_resize_add(buffer, sz)					\
	buffer_resize((buffer), buffer_size(buffer)+(sz))
#define buffer_reserve_add(buffer, sz)						\
	buffer_reserve((buffer), buffer_size(buffer)+(sz)+1)
#endif
	/*------------------------------------------------------------------------*/
	void buffer_t_swap(struct buffer_t* p, struct buffer_t* o);
	void buffer_t_ctor(struct buffer_t* p);
	void buffer_t_dtor(struct buffer_t* p);
	void buffer_t_asg(struct buffer_t* p, const struct buffer_t* o);
	struct buffer_t* buffer_t_new();
	struct buffer_t* buffer_t_clone(const struct buffer_t* o);
	void buffer_t_delete(struct buffer_t* p);
	/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

