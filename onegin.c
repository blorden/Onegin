/* cp1251 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <locale.h>


#include "msort.c"
#include "printer.c"

//this defines need to make eazy work with files
#define ONEGIN_INPUT  "onegin.txt"
#define ONEGIN_OUTPUT "onegin_output.txt"

//defines. clear_rubbish - if true, delete rubbish when call print_onegin
//separator - if true, separate onegin-output when call print_onegin
#define clear_rubbish true
#define separator     true

//macros, make script smaller
#define cmp_macro                                                       \
                                                                        \
                                                                        \
    int a_lyt = (unsigned char)str_a->str[a_ind],                       \
        b_lyt = (unsigned char)str_b->str[b_ind];                       \
                                                                        \
    if (a_lyt >= (unsigned char)'À' && a_lyt <= (unsigned char)'ß')     \
        a_lyt = a_lyt - 'À' + 'à';                                      \
    if (b_lyt >= (unsigned char)'À' && b_lyt <= (unsigned char)'ß')     \
        b_lyt = b_lyt - 'À' + 'à';                                      \
                                                                        \
    if (a_lyt == '¨')                                                   \
        a_lyt = '¸';                                                    \
    if (b_lyt == '¨')                                                   \
        b_lyt = '¸';                                                    \
                                                                        \
    a_lyt--;                                                            \
    b_lyt--;                                                            \
                                                                        \
    if (a_lyt == (unsigned char)'¸' - 1u)                               \
        a_lyt = (unsigned char)'å' + 1u;                                \
    else if (a_lyt > (unsigned char)'å' - 1u)                           \
        a_lyt++;                                                        \
                                                                        \
    if (b_lyt == (unsigned char)'¸' - 1u)                               \
        b_lyt = (unsigned char)'å' + 1u;                                \
    else if (b_lyt > (unsigned char)'å' - 1u)                           \
        b_lyt++; 
//-----------------------------------------------------------------------

/*!
    @brief keep information about onegin-str
    str  - pointer to begin of string in buffer
    size - length of string 
*/
struct _onegin_str
{
    char * str;
    size_t size;
};

typedef struct 
       _onegin_str onegin_str; 


/*!
    @brief get size of file
    @param file_name name of file, his size we need
    @return return size of file
*/
size_t get_file_size (const char *file_name);  


/*!
    @brief read onegin and write info in buffer, create mem for buffer
    @param file_name name file with onegin text
    @param buf buffer, where we want write info
    @param file_size size of file with onegin text
*/
void   read_onegin   (const char *file_name, char **buf, const size_t file_size);


/*!
    @brief print onegin from onegin_str_arr
    @param onegin_str_arr array with onegin_str
    @param onegin_arr_size size of onegin_str_arr
    @param onegin_text_size size of buffer
    @param file_name name file, where we want print
*/
void   print_onegin  (const onegin_str *onegin_str_arr, 
                       const size_t onegin_arr_size, const size_t onegin_text_size,
                       const char *file_name);


/*!
    @brief create onegin_str_arr
    @param onegin_str_arr arr, we create him
    @param buf_onegin_text buffer with onegin text
    @param onegin_text_size size of buffer
    @return size of onegin_str_arr
*/
size_t mk_oneg_arr   (onegin_str **onegin_str_arr, 
                       const char *buf_onegin_text, const size_t onegin_text_size);


/*!
    @brief cmp-func for sort, sort by left end
    @param a pointer to first string
    @param b pointer to second string
    @return cmp-int
*/
int cmp_l (const void *a, const void *b);


/*!
    @brief cmp-func for sort, sort by rigth end
    @param a pointer to first string
    @param b pointer to second string
    @return cmp-int
*/
int cmp_r (const void *a, const void *b);


/*!
    @brief check, this lyt rus or not
    @param a lyt
    @return false or true
*/
bool is_rus_lyt (unsigned char a);


int main ()
{

    char *buf = NULL;

    char *file_name  = ONEGIN_INPUT;
    size_t file_size = get_file_size(file_name);

    read_onegin(file_name, &buf, file_size);

    onegin_str *onegin_str_arr = NULL;
    size_t onegin_str_arr_size = 0;

    onegin_str_arr_size = mk_oneg_arr(&onegin_str_arr, buf, file_size);


    char *output = ONEGIN_OUTPUT;


    qsort(onegin_str_arr, onegin_str_arr_size, sizeof(onegin_str), cmp_l);
    print_onegin(onegin_str_arr, onegin_str_arr_size, file_size, output);

    msort(onegin_str_arr, onegin_str_arr_size, sizeof(onegin_str), cmp_r);
    print_onegin(onegin_str_arr, onegin_str_arr_size, file_size, output);

    FILE * last_output = fopen(output, "aw");
    printer(last_output, buf, file_size);

    free(buf);

    printer_delete(last_output);
    fclose(last_output);

    return 0;
}

//==============================================================================

size_t get_file_size (const char *file_name)
{
    
    FILE *input_file = fopen(file_name, "rb");
    assert(input_file != NULL);

    fseek(input_file, 0, SEEK_END);

    size_t file_size = ftello(input_file);
    fclose(input_file);

    return file_size;
}

//==============================================================================

void read_onegin (const char *file_name, char** buf, const size_t file_size)
{

    FILE *file = fopen(file_name, "rb");

    (*buf) = malloc(file_size);

    fread((void*)(*buf), 1, file_size, file);

    fclose(file);
}

//==============================================================================

void print_onegin (const onegin_str *onegin_str_arr, 
                    const size_t onegin_arr_size, const size_t onegin_text_size,
                    const char *file_name)
{

    for (int i = 0; i < onegin_arr_size; ++i)
    {

#if clear_rubbish

        size_t count_of_rubbish = 0;

        for (int j = 0; j < onegin_str_arr[i].size; ++j)
            count_of_rubbish += is_rus_lyt(onegin_str_arr[i].str[j]);

        if (count_of_rubbish == 0)
            continue;

#endif

        static size_t count_of_print = 0;

        FILE * output = NULL;

        if (count_of_print == 0)
            output = fopen(file_name, "wb");
        else
            output = fopen(file_name, "awb");

        printer(output, onegin_str_arr[i].str, onegin_str_arr[i].size);

        count_of_print++;
        fclose(output);
    }       

#if separator

    FILE *output = fopen(file_name, "aw");
    assert(output != NULL);


    printer(output, "\n\n\n\n\n\n ONEGIN_BY_ARTEM \n\n\n\n\n\n"
                    "\n\n\n\n\n\n   BEST_ONEGIN   \n\n\n\n\n\n"
                    "\n\n\n\n\n\n     (MAY BE)    \n\n\n\n\n\n", 87);

    fclose(output);

#endif  
}

//==============================================================================

size_t mk_oneg_arr (onegin_str **onegin_str_arr, 
                     const char *buf_onegin_text, const size_t onegin_text_size)

{

    size_t size_of_onegin_str = 0;

    int last_sl_n = -1;
    for (int i = 0; i < onegin_text_size; ++i)
        if (buf_onegin_text[i] == '\n')
        {

            size_of_onegin_str++;
            last_sl_n = i;  
        }


    if (onegin_text_size > 0 && (onegin_text_size - last_sl_n > 1 || last_sl_n == -1))
        size_of_onegin_str++;

    (*onegin_str_arr) = malloc(sizeof(onegin_str) * size_of_onegin_str);

    
    size_t cur_str_sz     = 0,
           onegin_str_num = 0;

    const char *cur_str_ptr = NULL;


    for (int i = 0; i < onegin_text_size; ++i)
    {

        if (cur_str_ptr == NULL)
        {

            cur_str_ptr = buf_onegin_text;
            cur_str_sz  = 1;
        }

        if (*buf_onegin_text == '\n')
        {

            (*onegin_str_arr)[onegin_str_num].str  = (char*) cur_str_ptr;
            (*onegin_str_arr)[onegin_str_num].size = cur_str_sz;

            cur_str_ptr = NULL;
            onegin_str_num++;

            cur_str_sz  = 0;
            buf_onegin_text++;
            continue;
        }

        cur_str_sz++;
        buf_onegin_text++;
    }     

    if (cur_str_sz > 0)
    {

        (*onegin_str_arr)[onegin_str_num].str  = (char*) cur_str_ptr;
        (*onegin_str_arr)[onegin_str_num].size = cur_str_sz - 1;

        onegin_str_num++;
    }

    return onegin_str_num;
}

//==============================================================================

bool is_rus_lyt (unsigned char a)
{
    if ((unsigned char)a >= (unsigned char)'à' 
            && (unsigned char)a <= (unsigned char)'ÿ')
        return true;

    if ((unsigned char)a >= (unsigned char)'À' 
            && (unsigned char)a <= (unsigned char)'ß')
        return true;

    if ((unsigned char)a == (unsigned char)'¸' 
            || (unsigned char)a == (unsigned char)'¨')
        return true;

    return false;
}

//==============================================================================


int cmp_l (const void *a, const void *b)
{
    onegin_str *str_a = (onegin_str*) a,
               *str_b = (onegin_str*) b;

    size_t a_sz = str_a->size,
           b_sz = str_b->size;

    int a_ind = 0,
        b_ind = 0;


    while (a_ind < a_sz && b_ind < b_sz)
    {

        if (!is_rus_lyt(str_a->str[a_ind]))
        {
    
            a_ind++;
            continue;
        }

        if (!is_rus_lyt(str_b->str[b_ind]))
        {
    
            b_ind++;
            continue;
        }

        cmp_macro

        if (a_lyt == b_lyt)
        {

            a_ind++;
            b_ind++;
            continue;
        }   

        return a_lyt - b_lyt;
    }       

    return a_sz - b_sz;        
}

//==============================================================================

int cmp_r (const void *a, const void *b)
{
    onegin_str *str_a = (onegin_str*) a,
               *str_b = (onegin_str*) b;

    size_t a_sz = str_a->size,
           b_sz = str_b->size;

    int a_ind = a_sz - 1,
        b_ind = b_sz - 1;    


    while (a_ind > 0 && b_ind > 0)
    {
        if (!is_rus_lyt(str_a->str[a_ind]))
        {

            a_ind--;
            continue;
        }

        if (!is_rus_lyt(str_b->str[b_ind]))
        {

            b_ind--;
            continue;
        }       

        cmp_macro

        if (a_lyt == b_lyt)
        {

            a_ind--;
            b_ind--;
            continue;
        }   

        return a_lyt - b_lyt;
    }

    return a_sz - b_sz;
}

//==============================================================================
