/*!
	@file
	@brief msort source code
*/

#ifndef MSORT
#define MSORT

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

///pointer to temp-array of memory for msort
static void *temp = NULL;

///bool variable, keep information about mem of temp-array (have, have not)
static bool temp_have_mem = false;

///number of elements of first rec-call msort
static size_t main_number = 0;


/*!
	@brief msort function
	@param first poiner to first element of array
	@param number number elents in array
	@param size size of one element in bytes
	@param cmp function-comparator
*/
void msort (void *first, size_t number, size_t size, 
			 int (*cmp) (const void *, const void *))
{
	assert(first != NULL);

	if (!temp_have_mem)
	{

		temp = malloc(number * size);
		temp_have_mem = true;
		main_number = number;
	}

	if (number == 1)
		return;

	size_t half_number = number / 2,
		   mod2_number = number % 2;

	msort(first, half_number, size, cmp);
	msort(first + half_number * size, half_number + mod2_number, size, cmp);

	void *l_half_ptr = first,
		 *r_half_ptr = first + half_number * size;

	void *e_l_half_ptr = r_half_ptr,
		 *e_r_half_ptr = r_half_ptr + (half_number + mod2_number) * size;

    void *temp_ptr = temp;

    while (l_half_ptr != e_l_half_ptr || r_half_ptr != e_r_half_ptr)
    {
    	if (l_half_ptr == e_l_half_ptr)
    	{

    		memcpy(temp_ptr, r_half_ptr, size);
    		r_half_ptr += size, temp_ptr += size;
    		continue;
    	}

    	if (r_half_ptr == e_r_half_ptr)
    	{

    		memcpy(temp_ptr, l_half_ptr, size);
    		l_half_ptr += size, temp_ptr += size;
    		continue;
    	}

    	if (cmp(l_half_ptr, r_half_ptr) <= 0)
    	{

    		memcpy(temp_ptr, l_half_ptr, size);
    		l_half_ptr += size, temp_ptr += size;
    	}
    	else
    	{

    		memcpy(temp_ptr, r_half_ptr, size);
    		r_half_ptr += size, temp_ptr += size;
    	}
    }

    void *temp_first = first, 
    	 *e_temp_first = first + number * size;

    temp_ptr = temp;	 

    while (temp_first != e_temp_first)
    {

    	memcpy(temp_first, temp_ptr, size);
    	temp_first += size, temp_ptr += size;
    }

    if (main_number == number)
    {

    	free(temp);
    	temp_have_mem = false;
    }
}

#endif