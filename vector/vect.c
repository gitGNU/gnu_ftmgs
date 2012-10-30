/*------------------------------------------------------------------------*/
/* Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain       */
/*                                                                        */
/* This file is part of the <FTMGS> Library                               */
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
#include "vect.h"
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
/*------------------------------------------------------------------------*/
/*-- Vect ----------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
void vect_reserve_(void* v, int sz__, int elmsz)
{
	vect_t(char)* v__ = v;
	if (sz__ > vect_max_nelms(v__)) {
		void* dt__ = realloc(vect_data(v__), sz__ * elmsz);
		if (dt__ != NULL) {
			vect_data(v__) = dt__;
			vect_max_nelms(v__) = sz__;
		}
	}
}
/*------------------------------------------------------------------------*/
void vect_shrink_(void* v, int elmsz)
{
	vect_t(char)* v__ = v;
	if (vect_nelms(v__) == 0) {
		vect_reset(v__);
	} else if (vect_max_nelms(v__) > vect_nelms(v__)) {
		void* dt__ = realloc(vect_data(v__), vect_nelms(v__) * elmsz);
		if (dt__ != NULL) {
			vect_data(v__) = dt__;
			vect_max_nelms(v__) = vect_nelms(v__);
		}
	}
}
/*------------------------------------------------------------------------*/
void vect_resize_(void* v, int nsz__, int elmsz)
{
	vect_t(char)* v__ = v;
	void* dt__;
	switch (nsz__ <= vect_max_nelms(v__)) {
	case 0:
		dt__ = realloc(vect_data(v__), 2 * nsz__ * elmsz);
		if (dt__ == NULL) {
			break;
		}
		vect_data(v__) = dt__;
		vect_max_nelms(v__) = 2 * nsz__;
	default:
		vect_nelms(v__) = nsz__;
	}
}
/*------------------------------------------------------------------------*/
/*-- Dtor/Asg ------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
void vect_t_dtor(void* v)
{
	vect_t(char)* v__ = v;
	vect_destroy(v__);
}
/*------------------------------------------------------------------------*/
void vect_t_dtor_dt_(void* v, int elmsz, void (*dtor)(void*))
{
	vect_t(char)* v__ = v;
	int i__;
	for (i__ = 0; i__ < elmsz*vect_nelms(v__); i__ += elmsz) {
		dtor(&vect_elm(v__, i__));
	}
	vect_destroy(v__);
}
/*------------------------------------------------------------------------*/
void vect_t_dtor_ptr_dt_(void* v, void (*dtor)(void*))
{
	vect_t(void*)* v__ = v;
	int i__;
	for (i__ = 0; i__ < vect_nelms(v__); ++i__) {
		dtor(vect_elm(v__, i__));
		free(vect_elm(v__, i__));
	}
	vect_destroy(v__);
}
/*------------------------------------------------------------------------*/
void vect_t_mv_(void* d, void* o)
{
	vect_t(void*)* d__ = d;
	vect_t(void*)* o__ = o;
	if (vect_data(d__) != vect_data(o__)) {
		vect_destroy(d__);
		if (vect_nelms(o__) > 0) {
			vect_data(d__) = vect_data(o__);
			vect_max_nelms(d__) = vect_max_nelms(o__);
			vect_nelms(d__) = vect_nelms(o__);
			vect_create(o__);
		}
	}
}
/*------------------------------------------------------------------------*/
void vect_t_asg_(void* d, const void* o, int elmsz)
{
	vect_t(char)* d__ = d;
	const vect_t(char)* o__ = o;
	if (vect_data(d__) != vect_data(o__)) {
		vect_destroy(d__);
		if (vect_nelms(o__) > 0) {
			void* dt__ = realloc(vect_data(d__), vect_nelms(o__)*elmsz);
			if (dt__ != NULL) {
				vect_data(d__) = dt__;
				vect_max_nelms(d__) = vect_nelms(o__);
				vect_nelms(d__) = vect_nelms(o__);
				memcpy(vect_data(d__), vect_data(o__),
					   vect_nelms(o__)*elmsz);
			}
		}
	}
}
/*------------------------------------------------------------------------*/
void vect_t_asg_dt_(void* d, const void* o, int elmsz,
					void (*dtor)(void*),
					void (*asg)(void*,const void*))
{
	vect_t(char)* d__ = d;
	const vect_t(char)* o__ = o;
	if (vect_data(d__) != vect_data(o__)) {
		vect_t_dtor_dt_(d, elmsz, dtor);
		if (vect_nelms(o__) > 0) {
			void* dt__ = realloc(vect_data(d__), vect_nelms(o__)*elmsz);
			if (dt__ != NULL) {
				int i__;
				vect_data(d__) = dt__;
				vect_max_nelms(d__) = vect_nelms(o__);
				vect_nelms(d__) = vect_nelms(o__);
				for (i__ = 0; i__ < vect_nelms(o__); i__ += elmsz) {
					asg(&vect_elm(d__, i__), &vect_elm(o__, i__));
				}
			}
		}
	}
}
/*------------------------------------------------------------------------*/
void vect_t_asg_ptr_dt_(void* d, const void* o, int elmsz,
						void (*dtor)(void*),
						void (*asg)(void*,const void*))
{
	vect_t(void*)* d__ = d;
	const vect_t(void*)* o__ = o;
	if (vect_data(d__) != vect_data(o__)) {
		vect_t_dtor_ptr_dt_(d, dtor);
		if (vect_nelms(o__) > 0) {
			void* dt__ = realloc(vect_data(d__), vect_nelms(o__)
								 *sizeof(*vect_data(o__)));
			if (dt__ != NULL) {
				int i__;
				vect_data(d__) = dt__;
				vect_max_nelms(d__) = vect_nelms(o__);
				vect_nelms(d__) = vect_nelms(o__);
				for (i__ = 0; i__ < vect_nelms(o__); ++i__) {
					if (vect_elm(o__, i__) == NULL) {
						vect_elm(d__, i__) = NULL;
					} else {
						vect_elm(d__, i__) = malloc(elmsz);
						if (vect_elm(d__, i__) != NULL) {
							asg(vect_elm(d__, i__), vect_elm(o__, i__));
						}
					}
				}
			}
		}
	}
}
/*------------------------------------------------------------------------*/
void vect_t_dtor_str_(vect_str_t* v__)
{
	int i__;
	for (i__ = 0; i__ < vect_nelms(v__); ++i__) {
		free(vect_elm(v__, i__));
	}
	vect_destroy(v__);
}
/*------------------------------------------------------------------------*/
void vect_t_asg_str_(vect_str_t* d__, const vect_str_t* o__)
{
	if (vect_data(d__) != vect_data(o__)) {
		vect_t_dtor_str_(d__);
		if (vect_nelms(o__) > 0) {
			void* dt__ = realloc(vect_data(d__), vect_nelms(o__)
								 *sizeof(*vect_data(o__)));
			if (dt__ != NULL) {
				int i__;
				vect_data(d__) = dt__;
				vect_max_nelms(d__) = vect_nelms(o__);
				vect_nelms(d__) = vect_nelms(o__);
				for (i__ = 0; i__ < vect_nelms(o__); ++i__) {
					if (vect_elm(o__, i__) == NULL) {
						vect_elm(d__, i__) = NULL;
					} else {
						vect_elm(d__, i__)
							= malloc(1+strlen(vect_elm(o__, i__)));
						if (vect_elm(d__, i__) != NULL) {
							strcpy(vect_elm(d__, i__),
								   vect_elm(o__, i__));
						}
					}
				}
			}
		}
	}
}
/*------------------------------------------------------------------------*/

