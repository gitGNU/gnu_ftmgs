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
#ifndef umalccvbg_vector_h__
#define umalccvbg_vector_h__	1
#ifdef __cplusplus
extern "C" {
#endif
	/*------------------------------------------------------------------------*/
	/**
	 * Vector data type
	 *   - size:     number of bytes actually stored in the vector
	 *   - data_:    data vector
	 *   - max_size: size of bytes actually allocated in the vector
	 */
	struct vector_t {
		unsigned size;
		char* data_;	/* size max_size */
		unsigned max_size;
	};
	/*------------------------------------------------------------------------*/
	/**
	 * Returns the pointer to the allocated vector that holds the data bytes
	 * 
	 * @param  vect  actual data vector structure
	 * @return the pointer to the allocated vector that holds the data bytes
	 * @pre    already constructed ADT vector (@a vect)
	 */
	char* vector_data(const struct vector_t* vect);
	/*------------------------------------------------------------------------*/
	/**
	 * Returns the number of bytes actually stored in the vector
	 * 
	 * @param  vect  actual data vector structure
	 * @return the number of bytes actually stored in the vector
	 * @pre    already constructed ADT vector (@a vect)
	 */
	unsigned vector_size(const struct vector_t* vect);
	/*------------------------------------------------------------------------*/
	/**
	 * Returns the size of bytes actually allocated in the vector
	 * 
	 * @param  vect  actual data vector structure
	 * @return the size of bytes actually allocated in the vector
	 * @pre    already constructed ADT vector (@a vect)
	 */
	unsigned vector_maxsize(const struct vector_t* vect);
	/*------------------------------------------------------------------------*/
	/**
	 * Increases the allocated vector of data to hold at least @a maxsz bytes
	 * 
	 * @param  vect  actual data vector structure
	 * @param  maxsz requested vector size
	 * @return void
	 * @pre    already constructed ADT vector (@a vect)
	 */
	void vector_reserve(struct vector_t* vect, unsigned maxsz);
	/*------------------------------------------------------------------------*/
	/**
	 * Shrinkes the size of the allocated vector to fit the size required to
	 * hold the stored data
	 * 
	 * @param  vect  actual data vector structure
	 * @return void
	 * @pre    already constructed ADT vector (@a vect)
	 */
	void vector_shrink(struct vector_t* vect);
	/*------------------------------------------------------------------------*/
	/**
	 * Resets the vector structure, removes stored data and deallocates
	 * vector memory
	 * 
	 * @param  vect  actual data vector structure
	 * @return void
	 * @pre    already constructed ADT vector (@a vect)
	 */
	void vector_reset(struct vector_t* vect);
	/*------------------------------------------------------------------------*/
	/**
	 * Removes the data stored in the vector.
	 * 
	 * @param  vect  actual data vector structure
	 * @return void
	 * @pre    already constructed ADT vector (@a vect)
	 */
	void vector_clear(struct vector_t* vect);
	/*------------------------------------------------------------------------*/
	/**
	 * Makes the length of the data bytes stored in the vector to
	 * match @a newsz. If @a newsz is greater than previous size, then
	 * the new bytes are filled with unspecified random values
	 * 
	 * @param  vect  actual data vector structure
	 * @param  newsz new size of stored data in vector
	 * @return Zero on success, -1 on failure
	 * @pre    already constructed ADT vector (@a vect)
	 */
	int vector_resize(struct vector_t* vect, unsigned newsz);
	/*------------------------------------------------------------------------*/
	/**
	 * Pushes a new chunk of @datlen bytes into the vector
	 * 
	 * @param  vect   actual data vector structure
	 * @param  dat    input sequence of bytes to store in vector
	 * @param  datlen length of the input sequence of bytes
	 * @return Zero on success, -1 on failure
	 * @pre    already constructed ADT vector (@a vect)
	 */
	int vector_push(struct vector_t* vect, const void* dat, unsigned datlen);
	/*------------------------------------------------------------------------*/
	/**
	 * Pushes the bytes of a nul-terminated string @str into the vector
	 * 
	 * @param  vect   actual data vector structure
	 * @param  str    input sequence of chars to store in vector
	 * @return Zero on success, -1 on failure
	 * @pre    already constructed ADT vector (@a vect)
	 */
	int vector_push_str(struct vector_t* vect, const char* str);
	/*------------------------------------------------------------------------*/
#ifndef vector_push_buf
	/**
	 * Pushes the bytes of another vector @b into the vector @a vector
	 * 
	 * @param  vector  actual data vector structure
	 * @param  b       input vector to push into @vector
	 * @return Zero on success, -1 on failure
	 * @pre    already constructed ADT vectors (@a vect, @a b)
	 */
#define vector_push_buf(vector, b)							\
	vector_push((vector), vector_data(b), vector_size(b))
	/*------------------------------------------------------------------------*/
	/**
	 * Increases the actual length of the data bytes stored in the vector by
	 * @a sz. The new bytes are filled with unspecified random values
	 * 
	 * @param  vector  actual data vector structure
	 * @param  sz      length of added random bytes
	 * @return Zero on success, -1 on failure
	 * @pre    already constructed ADT vector (@a vect)
	 */
#define vector_resize_add(vector, sz)					\
	vector_resize((vector), vector_size(vector)+(sz))
	/*------------------------------------------------------------------------*/
	/**
	 * Increases the allocated vector of data by at least @a sz bytes
	 * 
	 * @param  vector  actual data vector structure
	 * @param  sz      length of added space
	 * @return void
	 * @pre    already constructed ADT vector (@a vect)
	 */
#define vector_reserve_add(vector, sz)						\
	vector_reserve((vector), vector_size(vector)+(sz)+1)
#endif
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup vector_adt
	 * vector ADT: Constructor, Destructor, Asignment, New, Clone, Delete, Swap
	 * @{
	 */
	void vector_t_swap(struct vector_t* p, struct vector_t* o);
	void vector_t_ctor(struct vector_t* p);
	void vector_t_dtor(struct vector_t* p);
	void vector_t_asg(struct vector_t* p, const struct vector_t* o);
	struct vector_t* vector_t_new();
	struct vector_t* vector_t_clone(const struct vector_t* o);
	void vector_t_delete(struct vector_t* p);
	/** @} */
	/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif
