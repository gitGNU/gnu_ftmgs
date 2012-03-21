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
#include "data.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------------*/
void data_reserve(data_t* buff, unsigned sz)
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
void data_reset(data_t* buff)
{
	assert(buff != NULL);
	buff->size = 0;
	free(buff->data_);
	buff->data_ = NULL;
}
/*----------------------------------------------------------------------------*/
void data_copy(data_t* buff, void* dat, unsigned datlen)
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
#ifndef NDEBUG
static int data_t_chk_members(struct data_t* p, int code)/*auto*/
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
	struct dummy_data_t {
		unsigned size;
		char* data_;	/* size */
	};
	CHK_FIELD__(dummy_data_t, data_t, size);
	CHK_FIELD__(dummy_data_t, data_t, data_);
	CHK_SIZE__(dummy_data_t, data_t);
	return (p!=NULL)&&(code == 264154485);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void data_t_ctor(struct data_t* p)/*auto*/
{
	assert(p != NULL);
	assert(data_t_chk_members(p,264154485));
	p->size = 0;
	p->data_ = NULL;
}
/*----------------------------------------------------------------------------*/
void data_t_dtor(struct data_t* p)/*auto*/
{
	assert(p != NULL);
	assert(data_t_chk_members(p,264154485));
	free(p->data_);
}
/*----------------------------------------------------------------------------*/
void data_t_asg(struct data_t* p, const struct data_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(data_t_chk_members(p,264154485));
	if (p != o) {
		p->size = o->size;
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
void data_t_swap(struct data_t* p, struct data_t* o)
{
	unsigned aux;
	char* auxp;
	assert(p != NULL && o != NULL);
	assert(data_t_chk_members(p,264154485));
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
struct data_t* data_t_new()/*auto*/
{
	struct data_t* p = (struct data_t*)malloc(sizeof(struct data_t));
	if (p != NULL) {
		data_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct data_t* data_t_clone(const struct data_t* o)/*auto*/
{
	struct data_t* p = NULL;
	if (o != NULL) {
		p = (struct data_t*)malloc(sizeof(struct data_t));
		if (p != NULL) {
			data_t_ctor(p);
			data_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void data_t_delete(struct data_t* p)/*auto*/
{
	if (p != NULL) {
		data_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
