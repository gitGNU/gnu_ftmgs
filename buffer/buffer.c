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
#include "buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
char* buffer_data(const struct buffer_t* buff)
{
	return buff->data_;
}
/*------------------------------------*/
unsigned buffer_size(const struct buffer_t* buff)
{
	return buff->size;
}
/*----------------------------------------------------------------------------*/
void buffer_reserve(struct buffer_t* buff, unsigned sz)
{
	assert(buff != NULL);
	free(buff->data_);
	buff->data_ = (char*)malloc(sz*sizeof(char));
	if (buff->data_ != NULL) {
		buff->size = sz;
	} else {
		buff->size = 0;
	}
}
/*----------------------------------------------------------------------------*/
void buffer_reset(struct buffer_t* buff)
{
	assert(buff != NULL);
	buff->size = 0;
	free(buff->data_);
	buff->data_ = NULL;
}
/*----------------------------------------------------------------------------*/
void buffer_copy(struct buffer_t* buff, void* dat, unsigned datlen)
{
	assert(buff != NULL);
	if (datlen > 0) {
		assert(dat != NULL);
		free(buff->data_);
		buff->data_ = (char*)malloc(datlen*sizeof(char));
		if (buff->data_ != NULL) {
			buff->size = datlen;
			memcpy(buff->data_, dat, datlen);
		} else {
			buff->size = 0;
		}
	}
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int buffer_t_chk_members(const struct buffer_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_buffer_t {
		unsigned size;
		char* data_;	/* size zero */
	};
	CHK_FIELD__(dummy_buffer_t, buffer_t, size);
	CHK_FIELD__(dummy_buffer_t, buffer_t, data_);
	CHK_SIZE__(dummy_buffer_t, buffer_t);
	return (code == 333122365); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void buffer_t_ctor(struct buffer_t* p)/*auto*/
{
	assert(p != NULL);
	assert(buffer_t_chk_members(p,333122365));
	p->size = 0;
	p->data_ = NULL;
}
/*----------------------------------------------------------------------------*/
void buffer_t_dtor(struct buffer_t* p)/*auto*/
{
	assert(p != NULL);
	assert(buffer_t_chk_members(p,333122365));
	if (p->data_) {
		memset(p->data_, 0, (size_t)p->size*sizeof(char));
	}
	free(p->data_);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void buffer_t_asg(struct buffer_t* p, const struct buffer_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(buffer_t_chk_members(p,333122365));
	if (p != o) {
		p->size = o->size;
		if (p->data_) {
			memset(p->data_, 0, (size_t)p->size*sizeof(char));
		}
		free(p->data_);
		if (o->data_ == NULL) {
			p->data_ = NULL;
		} else {
			p->data_ = (char*)malloc(o->size*sizeof(char));
			if (p->data_ != NULL) {
				memcpy(p->data_, o->data_, o->size*sizeof(char));
			}
		}
	}
}
/*----------------------------------------------------------------------------*/
void buffer_t_move(struct buffer_t* p, struct buffer_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(buffer_t_chk_members(p,333122365));
	if (p != o) {
		p->size = o->size;
		if (p->data_) {
			memset(p->data_, 0, (size_t)p->size*sizeof(char));
		}
		free(p->data_);
		p->data_ = o->data_;
		o->data_ = NULL;
	}
}
/*----------------------------------------------------------------------------*/
void buffer_t_swap(struct buffer_t* p, struct buffer_t* o)
{
	unsigned aux;
	char* auxp;
	assert(p != NULL && o != NULL);
	assert(buffer_t_chk_members(p,333122365));
	if (p != o) {
		aux = p->size;
		p->size = o->size;
		o->size = aux;
		/*----------------------------*/
		auxp = p->data_;
		p->data_ = o->data_;
		o->data_ = auxp;
	}
}
/*----------------------------------------------------------------------------*/
struct buffer_t* buffer_t_new()/*auto*/
{
	struct buffer_t* p = (struct buffer_t*)malloc(sizeof(struct buffer_t));
	if (p != NULL) {
		buffer_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct buffer_t* buffer_t_clone(const struct buffer_t* o)/*auto*/
{
	struct buffer_t* p = NULL;
	if (o != NULL) {
		p = (struct buffer_t*)malloc(sizeof(struct buffer_t));
		if (p != NULL) {
			buffer_t_ctor(p);
			buffer_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void buffer_t_delete(struct buffer_t* p)/*auto*/
{
	if (p != NULL) {
		buffer_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
