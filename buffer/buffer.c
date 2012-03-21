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
/*------------------------------------*/
unsigned buffer_maxsize(const struct buffer_t* buff)
{
	return buff->max_size;
}
/*----------------------------------------------------------------------------*/
static void buffer_realloc(struct buffer_t* buff, unsigned sz)
{
	assert(buff != NULL);
	if (sz != buff->max_size) {
		buff->data_ = (char*)realloc(buff->data_, sz*sizeof(char));
		if (buff->data_ != NULL) {
			buff->max_size = sz;
		} else {
			buff->size = 0;
			buff->max_size = 0;
		}
	}
	assert(((buff->size == 0)&&(buff->max_size == 0))
		   ||(buff->size < buff->max_size));
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void buffer_reserve(struct buffer_t* buff, unsigned maxsz)
{
	assert(buff != NULL);
	if (maxsz > buff->max_size) {
		buffer_realloc(buff, maxsz);
	}
}
/*----------------------------------------------------------------------------*/
void buffer_shrink(struct buffer_t* buff)
{
	/* 
	 * One addtional space to allow a place for '\0' if required
	 */
	assert(buff != NULL);
	buffer_realloc(buff, buff->size+1);
}
/*----------------------------------------------------------------------------*/
void buffer_reset(struct buffer_t* buff)
{
	assert(buff != NULL);
	buff->size = 0;
	buff->max_size = 0;
	free(buff->data_);
	buff->data_ = NULL;
}
/*----------------------------------------------------------------------------*/
void buffer_clear(struct buffer_t* buff)
{
	assert(buff != NULL);
	buff->size = 0;
}
/*----------------------------------------------------------------------------*/
void buffer_resize(struct buffer_t* buff, unsigned newsz)
{
	assert(buff != NULL);
	if ((newsz == 0)||(newsz < buff->max_size)) {
		buff->size = newsz;
	} else {
		buffer_realloc(buff, 2*newsz);
		if (buff->data_ != NULL) {
			buff->size = newsz;
		}
	}
	assert(((buff->size == 0)&&(buff->max_size == 0))
		   ||(buff->size < buff->max_size));
}
/*----------------------------------------------------------------------------*/
void buffer_push(struct buffer_t* buff, const void* dat, unsigned datlen)
{
	unsigned maxsz;
	assert(buff != NULL);
	if (datlen > 0) {
		assert(dat != NULL);
		/* 
		 * One addtional space to allow a place for '\0' if required
		 */
		if (buff->size + datlen + 1 > buff->max_size) {
			maxsz = 2 * (buff->size + datlen);
			buffer_realloc(buff, maxsz);
		}
		if (buff->data_ != NULL) {
			memcpy(buff->data_ + buff->size, dat, datlen);
			buff->size += datlen;
		}
	}
	assert(((buff->size == 0)&&(buff->max_size == 0))
		   ||(buff->size < buff->max_size));
}
/*----------------------------------------------------------------------------*/
void buffer_push_str(struct buffer_t* buff, const char* str)
{
	buffer_push(buff, str, (unsigned)strlen(str));
	buffer_reserve(buff, buff->size + 1);
	buff->data_[buff->size] = '\0';	/* '\0' is not counted in size */
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int buffer_t_chk_members(struct buffer_t* p, int code)/*auto*/
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
		unsigned max_size;
		char* data_;	/* size max_size */
	};
	CHK_FIELD__(dummy_buffer_t, buffer_t, size);
	CHK_FIELD__(dummy_buffer_t, buffer_t, max_size);
	CHK_FIELD__(dummy_buffer_t, buffer_t, data_);
	CHK_SIZE__(dummy_buffer_t, buffer_t);
	return (p!=NULL)&&(code == 255796141);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void buffer_t_ctor(struct buffer_t* p)/*auto*/
{
	assert(p != NULL);
	assert(buffer_t_chk_members(p,255796141));
	p->size = 0;
	p->max_size = 0;
	p->data_ = NULL;
}
/*----------------------------------------------------------------------------*/
void buffer_t_dtor(struct buffer_t* p)/*auto*/
{
	assert(p != NULL);
	assert(buffer_t_chk_members(p,255796141));
	free(p->data_);
}
/*----------------------------------------------------------------------------*/
void buffer_t_asg(struct buffer_t* p, const struct buffer_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(buffer_t_chk_members(p,255796141));
	if (p != o) {
		p->size = o->size;
		p->max_size = o->max_size;
		free(p->data_);
		if (o->data_ == NULL) {
			p->data_ = NULL;
		} else {
			p->data_ = (char*)malloc(o->max_size*sizeof(char));
			if (p->data_ != NULL) {
				memcpy(p->data_, o->data_, o->size*sizeof(char));
			}
		}
	}
}
/*----------------------------------------------------------------------------*/
void buffer_t_swap(struct buffer_t* p, struct buffer_t* o)
{
	unsigned aux;
	char* auxp;
	assert(p != NULL && o != NULL);
	assert(buffer_t_chk_members(p,255796141));
	if (p != o) {
		aux = p->size;
		p->size = o->size;
		o->size = aux;
		/*----------------------------*/
		aux = p->max_size;
		p->max_size = o->max_size;
		o->max_size = aux;
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
