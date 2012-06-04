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
#include "vector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
char* vector_data(const struct vector_t* vect)
{
	return vect->data_;
}
/*------------------------------------*/
unsigned vector_size(const struct vector_t* vect)
{
	return vect->size;
}
/*------------------------------------*/
unsigned vector_maxsize(const struct vector_t* vect)
{
	return vect->max_size;
}
/*----------------------------------------------------------------------------*/
static void vector_realloc(struct vector_t* vect, unsigned sz)
{
	char* data;
	assert(vect != NULL);
	if (sz != vect->max_size) {
		data = (char*)realloc(vect->data_, sz*sizeof(char));
		if (data != NULL) {
			vect->data_ = data;
			vect->max_size = sz;
		}
	}
	assert(((vect->size == 0)&&(vect->max_size == 0))
		   ||(vect->size < vect->max_size));
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void vector_reserve(struct vector_t* vect, unsigned maxsz)
{
	assert(vect != NULL);
	if (maxsz > vect->max_size) {
		vector_realloc(vect, maxsz);
	}
}
/*----------------------------------------------------------------------------*/
void vector_shrink(struct vector_t* vect)
{
	/* 
	 * One addtional space to allow a place for '\0' if required
	 */
	assert(vect != NULL);
	vector_realloc(vect, vect->size+1);
}
/*----------------------------------------------------------------------------*/
void vector_reset(struct vector_t* vect)
{
	assert(vect != NULL);
	vect->size = 0;
	vect->max_size = 0;
	free(vect->data_);
	vect->data_ = NULL;
}
/*----------------------------------------------------------------------------*/
void vector_clear(struct vector_t* vect)
{
	assert(vect != NULL);
	vect->size = 0;
}
/*----------------------------------------------------------------------------*/
int vector_resize(struct vector_t* vect, unsigned newsz)
{
	int ok = 0;
	assert(vect != NULL);
	if ((newsz == 0)||(newsz < vect->max_size)) {
		vect->size = newsz;
	} else {
		vector_realloc(vect, 2*newsz);
		if ((vect->data_ != NULL)&&(newsz < vect->max_size)) {
			vect->size = newsz;
		} else {
			ok = -1;
		}
	}
	assert(((vect->size == 0)&&(vect->max_size == 0))
		   ||(vect->size < vect->max_size));
	return ok;
}
/*----------------------------------------------------------------------------*/
int vector_push(struct vector_t* vect, const void* dat, unsigned datlen)
{
	int ok = 0;
	unsigned maxsz;
	assert(vect != NULL);
	if (datlen > 0) {
		assert(dat != NULL);
		/* 
		 * One addtional space to allow a place for '\0' if required
		 */
		if (vect->size + datlen + 1 > vect->max_size) {
			maxsz = 2 * (vect->size + datlen);
			vector_realloc(vect, maxsz);
		}
		if ((vect->data_ != NULL)&&(vect->size + datlen < vect->max_size)) {
			memcpy(vect->data_ + vect->size, dat, datlen);
			vect->size += datlen;
		} else {
			ok = -1;
		}
	}
	assert(((vect->size == 0)&&(vect->max_size == 0))
		   ||(vect->size < vect->max_size));
	return ok;
}
/*----------------------------------------------------------------------------*/
int vector_push_str(struct vector_t* vect, const char* str)
{
	int ok = vector_push(vect, str, (unsigned)strlen(str));
	vector_reserve(vect, vect->size + 1);
	if (vect->size < vect->max_size) {
		vect->data_[vect->size] = '\0';	/* '\0' is not counted in size */
	}
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int vector_t_chk_members(struct vector_t* p, int code)/*auto*/
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
	struct dummy_vector_t {
		unsigned size;
		char* data_;	/* size max_size */
		unsigned max_size;
	};
	CHK_FIELD__(dummy_vector_t, vector_t, size);
	CHK_FIELD__(dummy_vector_t, vector_t, data_);
	CHK_FIELD__(dummy_vector_t, vector_t, max_size);
	CHK_SIZE__(dummy_vector_t, vector_t);
	return (p!=NULL)&&(code == 310118405);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void vector_t_ctor(struct vector_t* p)/*auto*/
{
	assert(p != NULL);
	assert(vector_t_chk_members(p,310118405));
	p->size = 0;
	p->data_ = NULL;
	p->max_size = 0;
}
/*----------------------------------------------------------------------------*/
void vector_t_dtor(struct vector_t* p)/*auto*/
{
	assert(p != NULL);
	assert(vector_t_chk_members(p,310118405));
	free(p->data_);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void vector_t_asg(struct vector_t* p, const struct vector_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(vector_t_chk_members(p,310118405));
	if (p != o) {
		p->size = o->size;
		free(p->data_);
		if (o->data_ == NULL) {
			p->data_ = NULL;
		} else {
			p->data_ = (char*)malloc(o->max_size*sizeof(char));
			if (p->data_ != NULL) {
				memcpy(p->data_, o->data_, o->size*sizeof(char));
			}
		}
		p->max_size = o->max_size;
	}
}
/*----------------------------------------------------------------------------*/
void vector_t_swap(struct vector_t* p, struct vector_t* o)
{
	unsigned aux;
	char* auxp;
	assert(p != NULL && o != NULL);
	assert(vector_t_chk_members(p,310118405));
	if (p != o) {
		aux = p->size;
		p->size = o->size;
		o->size = aux;
		/*----------------------------*/
		auxp = p->data_;
		p->data_ = o->data_;
		o->data_ = auxp;
		/*----------------------------*/
		aux = p->max_size;
		p->max_size = o->max_size;
		o->max_size = aux;
	}
}
/*----------------------------------------------------------------------------*/
struct vector_t* vector_t_new()/*auto*/
{
	struct vector_t* p = (struct vector_t*)malloc(sizeof(struct vector_t));
	if (p != NULL) {
		vector_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct vector_t* vector_t_clone(const struct vector_t* o)/*auto*/
{
	struct vector_t* p = NULL;
	if (o != NULL) {
		p = (struct vector_t*)malloc(sizeof(struct vector_t));
		if (p != NULL) {
			vector_t_ctor(p);
			vector_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void vector_t_delete(struct vector_t* p)/*auto*/
{
	if (p != NULL) {
		vector_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
