/*!
	@file
	@brief printer source code
*/

#ifndef PRINTER
#define PRINTER

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>


static size_t printer_buf_mem_sz   = 1024,  //size of buffer
			  printer_ind_top_buf  = 0;	    //index of top buffer not used element

static bool   printer_buf_have_mem = false; //bool, keep information about have printer mem or not
static char * printer_buf_mem      = NULL;	//printer buffer

/*!
	@brief keep mem in buffer. When buffer is overflow - print him to file
	@param output file for print
	@param buf mem to print
	@param how much bytes need to print
*/
void printer (FILE *output, char *buf, size_t len)
{

	assert(output != NULL);

	if (!printer_buf_have_mem)
	{

		printer_buf_mem = malloc(sizeof(char) * printer_buf_mem_sz);
		printer_buf_have_mem = true;
	}

	size_t input_ind_top_buf = 0;

	for (; input_ind_top_buf < len; ++input_ind_top_buf, ++printer_ind_top_buf)
	{

		if (printer_ind_top_buf == printer_buf_mem_sz)
		{

			fwrite(printer_buf_mem, sizeof(char), printer_buf_mem_sz, output);
			printer_ind_top_buf = 0;
		}

		printer_buf_mem[printer_ind_top_buf] = buf[input_ind_top_buf];
	}
}

/*!
	@brief clear mem of printer and print that have
	@param output file to print
*/
void printer_delete (FILE *output)
{

	assert(output != NULL);

	if (printer_ind_top_buf > 0)
	{

		fwrite(printer_buf_mem, sizeof(char), printer_ind_top_buf, output);
		printer_ind_top_buf = 0;
	}

	free(printer_buf_mem);
	printer_buf_mem = NULL;
	printer_buf_have_mem = false;
}

#endif