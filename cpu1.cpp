#include <stdlib.h>
#include <math.h>

#include "enum.h"
#include "mystack_Lib.cpp"


const int Max_mem_size = 50;
const int Max_reg_kol = 4;


char* Read_buff (FILE* f_mem)
{
    FILE* fin = fopen ("output.cpu", "rb");

    fseek (fin, 0, SEEK_END);
    int size_of_file = ftell (fin);
    rewind (fin);

    char* buff = (char*) my_calloc (f_mem, size_of_file, sizeof (char), __LINE__);

    fread (buff, sizeof (char), size_of_file, fin);

    fclose (fin);

    return buff;
}

void skip_probs (char** buff)
{
    while (**buff == ' ' || **buff == '\n' || **buff == '\r')
        *buff += 1;
}

void run_asm_code (FILE* f_mem, char* buff, Stack_t stk, int* mem, int* reg)
{
    char* buff_beg = buff;

    Stack_t stk_call;
    StackConstructor (&stk_call, "stk_call");

    while (*buff != '\0')
    {
        #define DEF_CMD(name, len_of_name, num, code) \
            case CMD_##name:                          \
                buff++;                               \
                code                                  \
                break;

        switch (*buff)
        {
            #include "com.h"

            default: break;
        }
        skip_probs (&buff);
    }
    #undef DEF_CMD

    return;
}


int main ()
{
    FILE* f_mem = fopen ("mem_info.txt", "a");
    fprintf (f_mem, "\tUsing dynamic memory in cpu\n");

    char* buff = Read_buff (f_mem);

    Stack_t stk;
    StackConstructor (&stk, "stk");

    int* mem = (int*) my_calloc (f_mem, Max_mem_size, sizeof (int), __LINE__);
    int* reg = (int*) my_calloc (f_mem, Max_reg_kol, sizeof (int), __LINE__);

    run_asm_code (f_mem, buff, stk, mem, reg);

    my_free (f_mem, buff, __LINE__);
    my_free (f_mem, mem, __LINE__);
    my_free (f_mem, reg, __LINE__);

    return 0;
}
