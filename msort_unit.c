/*!
    @file
    @brief small unit-tester for msort
*/

#include <stdio.h>
#include <stdbool.h>

#include "msort.c"


/*!
    @brief cmp-func for strings
    @param a pointer to first string
    @param b ponter to second string
    @return cmp-int
*/
int str_cmp (const void *a, const void *b);

/*!
    @brief cmp-func for ints
    @param a pointer to first int
    @param b pointer to second int
    @return cmp-int
*/
int int_cmp (const void *a, const void *b);

int main ()
{

    int ints[][9]    = {
                        {1, 6, 5, 4, 5, 9, 4, 3, 6},  
                        {7, 4, 1, 2, 2, 4, 5, 6, 1},
                        {5, 3, 2, 1, 6, 7, 4, 3, 3}
                       };

    int ints_nd[][9] = {
                        {1, 3, 4, 4, 5, 5, 6, 6, 9},
                        {1, 1, 2, 2, 4, 4, 5, 6, 7},
                        {1, 2, 3, 3, 3, 4, 5, 6, 7}
                       };        

    char *str[][3]   = {
                        {"lala", "koko", "abacaba"}, 
                        {"tata", "eeeee", "usdfsdfsd"},
                        {"trid", "len", "in"}
                       };                 

    char *str_nd[][3]= {
                        {"abacaba", "koko", "lala"},
                        {"eeeee", "tata", "usdfsdfsd"},
                        {"in", "len", "trid"}
                       };                 


    for (int i = 0; i < 3; ++i)
        msort(ints[i], 9, sizeof(int), int_cmp);

    for (int i = 0; i < 3; ++i)
        msort(str[i], 3, sizeof(char*), str_cmp);


    printf("\nINT TEST START\n\n");

    for (int i = 0; i < 3; ++i)
    {

        printf("%d:\t", i + 1);

        bool test_ok = true;
        int j_err    = -1;

        for (int j = 0; j < 9; ++j)
            if (ints[i][j] != ints_nd[i][j])
            {

                j_err = j;
                test_ok = false;
            }

        if (test_ok)
            printf("OK\n");
        else
            printf("error! %d element of ints incorrect\n", j_err);
    }


    printf("\nSTR TEST START\n\n");

    for (int i = 0; i < 3; ++i)
    {

        printf("%d:\t", i + 1);

        bool test_ok = true;
        int j_err    = -1;

        for (int j = 0; j < 3; ++j)
        {

            if (strcmp(str[i][j], str_nd[i][j]) != 0)
            {

                j_err = j;
                test_ok = false;
            }
        }

        if (test_ok)
            printf("OK\n");
        else
            printf("error! %d element of str incorrect\n", j_err);      
    }

    return 0;
}

int str_cmp (const void *a, const void *b)
{

    char *_a = *(char**)a,
         *_b = *(char**)b;

    return strcmp(_a, _b);   
}

int int_cmp (const void *a, const void *b)
{
    
    int _a = *(int*)a,
        _b = *(int*)b;

    return _a - _b; 
}