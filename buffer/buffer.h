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
/** @file */
#ifndef umalccvbg_buffer_h__
#define umalccvbg_buffer_h__	1
#ifdef __cplusplus
extern "C" {
#endif
	/*------------------------------------------------------------------------*/
	/**
	 * Buffer data type
	 *   - size:     number of bytes actually stored in the buffer
	 *   - data_:    data buffer
	 */
	struct buffer_t {
		unsigned size;
		char* data_;	/* size */
	};
	/*------------------------------------------------------------------------*/
	/**
	 * Returns the pointer to the allocated buffer that holds the data bytes
	 * 
	 * @param  buff  actual data buffer structure
	 * @return the pointer to the allocated buffer that holds the data bytes
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	char* buffer_data(const struct buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Returns the number of bytes actually stored in the buffer
	 * 
	 * @param  buff  actual data buffer structure
	 * @return the number of bytes actually stored in the buffer
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	unsigned buffer_size(const struct buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Allocates space for @a sz bytes in the data buffer
	 * 
	 * @param  buff  actual data structure
	 * @param  sz    size of allocated space
	 * @return void
	 * @pre    already constructed ADT data (@a buff)
	 */
	void buffer_reserve(struct buffer_t* buff, unsigned sz);
	/*------------------------------------------------------------------------*/
	/**
	 * Resets the data structure, removes stored data and deallocates
	 * data memory
	 * 
	 * @param  buff  actual data structure
	 * @return void
	 * @pre    already constructed ADT data (@a buff)
	 */
	void buffer_reset(struct buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Allocates space and copy @a datlen bytes from @a dat into data buffer
	 * 
	 * @param  buff   actual data structure
	 * @param  dat    input data to be copied
	 * @param  datlen length of input data
	 * @return void
	 * @pre    already constructed ADT data (@a buff)
	 */
	void buffer_copy(struct buffer_t* buff, void* dat, unsigned datlen);
	/*------------------------------------------------------------------------*/
	/**
	 * Allocates space and copy bytes from a data buffer
	 * 
	 * @param  data   actual data structure
	 * @param  b      input buffer to be copied
	 * @return void
	 * @pre    already constructed ADT data (@a data)
	 */
#ifndef buffer_copy_buf
#define buffer_copy_buf(data, b)	data_copy(data, (b)->data_, (b)->size)
#endif
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup buffer_adt
	 * Data ADT: Constructor, Destructor, Asignment, New, Clone, Delete, Swap
	 * @{
	 */
	void buffer_t_swap(struct buffer_t* p, struct buffer_t* o);
	void buffer_t_ctor(struct buffer_t* p);
	void buffer_t_dtor(struct buffer_t* p);
	void buffer_t_asg(struct buffer_t* p, const struct buffer_t* o);
	struct buffer_t* buffer_t_new();
	struct buffer_t* buffer_t_clone(const struct buffer_t* o);
	void buffer_t_delete(struct buffer_t* p);
	/** @} */
	/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

