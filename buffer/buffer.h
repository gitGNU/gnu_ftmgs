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
	 *   - max_size: size of bytes actually allocated in the buffer
	 *   - data_:    data buffer
	 */
	struct buffer_t {
		unsigned size;
		unsigned max_size;
		char* data_;	/* size max_size */
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
	 * Returns the size of bytes actually allocated in the buffer
	 * 
	 * @param  buff  actual data buffer structure
	 * @return the size of bytes actually allocated in the buffer
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	unsigned buffer_maxsize(const struct buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Increases the allocated buffer of data to hold at least @a maxsz bytes
	 * 
	 * @param  buff  actual data buffer structure
	 * @param  maxsz requested buffer size
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	void buffer_reserve(struct buffer_t* buff, unsigned maxsz);
	/*------------------------------------------------------------------------*/
	/**
	 * Shrinkes the size of the allocated buffer to fit the size required to
	 * hold the stored data
	 * 
	 * @param  buff  actual data buffer structure
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	void buffer_shrink(struct buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Resets the buffer structure, removes stored data and deallocates
	 * buffer memory
	 * 
	 * @param  buff  actual data buffer structure
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	void buffer_reset(struct buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Removes the data stored in the buffer.
	 * 
	 * @param  buff  actual data buffer structure
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	void buffer_clear(struct buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Makes the length of the data bytes stored in the buffer to
	 * match @a newsz. If @a newsz is greater than previous size, then
	 * the new bytes are filled with unspecified random values
	 * 
	 * @param  buff  actual data buffer structure
	 * @param  newsz new size of stored data in buffer
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	void buffer_resize(struct buffer_t* buff, unsigned newsz);
	/*------------------------------------------------------------------------*/
	/**
	 * Pushes a new chunk of @datlen bytes into the buffer
	 * 
	 * @param  buff   actual data buffer structure
	 * @param  dat    input sequence of bytes to store in buffer
	 * @param  datlen length of the input sequence of bytes
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	void buffer_push(struct buffer_t* buff, const void* dat, unsigned datlen);
	/*------------------------------------------------------------------------*/
	/**
	 * Pushes the bytes of a nul-terminated string @str into the buffer
	 * 
	 * @param  buff   actual data buffer structure
	 * @param  str    input sequence of chars to store in buffer
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	void buffer_push_str(struct buffer_t* buff, const char* str);
	/*------------------------------------------------------------------------*/
#ifndef buffer_push_buf
	/**
	 * Pushes the bytes of another buffer @b into the buffer @a buffer
	 * 
	 * @param  buffer  actual data buffer structure
	 * @param  b       input buffer to push into @buffer
	 * @return void
	 * @pre    already constructed ADT buffers (@a buff, @a b)
	 */
#define buffer_push_buf(buffer, b)							\
	buffer_push((buffer), buffer_data(b), buffer_size(b))
	/*------------------------------------------------------------------------*/
	/**
	 * Increases the actual length of the data bytes stored in the buffer by
	 * @a sz. The new bytes are filled with unspecified random values
	 * 
	 * @param  buffer  actual data buffer structure
	 * @param  sz      length of added random bytes
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
#define buffer_resize_add(buffer, sz)					\
	buffer_resize((buffer), buffer_size(buffer)+(sz))
	/*------------------------------------------------------------------------*/
	/**
	 * Increases the allocated buffer of data by at least @a sz bytes
	 * 
	 * @param  buffer  actual data buffer structure
	 * @param  sz      length of added space
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
#define buffer_reserve_add(buffer, sz)						\
	buffer_reserve((buffer), buffer_size(buffer)+(sz)+1)
#endif
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup buff_adt
	 * Buffer ADT: Constructor, Destructor, Asignment, New, Clone, Delete, Swap
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
