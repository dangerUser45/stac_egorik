#include "Common.h"
#include "Debug.h"

int Create_file (stack_t* Data)
{
    FILE* fp = 0;
    if ((fp = fopen("StackLog.txt", "w+")) == NULL)
    {
        fprintf (stdout, "Не удается открыть файл\n") ;
        return 0;
    }
    setvbuf (fp, 0, 0, _IONBF);
    Data -> fp = fp;

    return 0;/* code_error */
}
//==================================================================================================
int Dump (stack_t* Data)
{
    FILE* fp = Data -> fp;
    assert (fp);
    fprintf (fp, "//================================================================================================\n");

    fprintf (fp, "\tSTRUCT:\n");
    fprintf (fp, "  canary1_struct = %d\n", Data -> canary1_struct);
    fprintf (fp, "  name of struct = %s\n", Data -> name);
    fprintf (fp, "  file = %s\n", Data -> file);
    fprintf (fp, "  buffer = %p\n", Data -> buffer);
    fprintf (fp, "  size = %zd\n", Data -> size);
    fprintf (fp, "  capacity = %zd\n", Data -> capacity);
    fprintf (fp, "  canary2_struct = %d\n", Data -> canary2_struct);

    fprintf (fp, "\t\t\t\tSTACK:\n");
    fprintf (fp, "\tcanary1_buf) <%016d> --- address: %p\n", Data -> buffer[0], Data -> buffer);
    for (ssize_t i = 1; i < Data -> capacity + 1; ++i)
        fprintf (fp, "\t\t  %zd) <%016d> --- address: %p\n", i, Data -> buffer[i], Data -> buffer + i);
    fprintf (fp, "\tcanary2_buf) <%016d> --- address: %p\n", Data -> buffer[Data -> capacity + 1], Data -> buffer + Data -> capacity + 1);
    fprintf (fp, "\n");
    fprintf (fp,"//================================================================================================\n");
    return 0;
}
//==================================================================================================
int Canary (stack_t* Data)
{
    Data -> buffer[0] = CANARY_B;
    Data -> buffer[Data -> capacity + 1] = CANARY_B;

    return 0;
}
//==================================================================================================
int Fill_Poison (stack_el_t* begin, uint_t quantity)
{
    for (uint_t i = 0; i < quantity; ++i)
        begin[i] = POISON;

    return 0;
}
//==================================================================================================
int Verificator (stack_t* Data)
{
    int error = 0;
    if (Data == NULL)
    {
        error = error | BAD_POINTER;
        return error;
    }

    if (Data->buffer == NULL)               // 1111'1111
        error = error | BUFFER_NULL;     // 1 csb
    else
    {
        if (Data -> buffer[0] != CANARY_B)
            error = error | BAD_CANARY1_B;

        if (Data -> buffer[Data -> capacity + 1] != CANARY_B)
            error = error | BAD_CANARY2_B;
        if (Data->size < 0)
            error = error | BAD_SIZE;     // 2

        /*uint_t hash_struct = Hash (Data, sizeof (stack_t));
        uint_t hash_buffer = Hash (Data -> buffer, Data -> (capacity + 2) * sizeof (stack_el_t));

        if (Data -> hash_struct != ???)//!!!
            error = error | BAD_HASH_STRUCT;

        if (Data -> hash_bufer != hash_buffer)//!!!
            error = error | BAD_HASH_BUF;*/      //!!! finish it

    }

    if (Data ->capacity < 0)
        error = error | BAD_CAPACITY;     // 3

    if (Data -> fp == NULL)
        error = error | FILE_NULL;     // 4

    if (Data -> canary1_struct != CANARY_S)
        error = error | BAD_CANARY1_S;     // 5

    if (Data -> canary2_struct != CANARY_S)
        error = error | BAD_CANARY2_S;     // 6

    return error;
}
//==================================================================================================
int Decoder_error (stack_t* Data, int error, int line, const char* name_func)
{

    fprintf (Data -> fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf (Data -> fp, "\tERRORS  in  line: %d\n\tFunction: %s\n", line, name_func);

    if (error & BUFFER_NULL)
        fprintf (Data -> fp, "Buffer is BAD: buffer = %p\n", Data -> buffer);

    if (error & BAD_SIZE)
        fprintf (Data -> fp, "Size is BAD: size = %zd\n", Data -> size);

    if (error & BAD_CAPACITY)
        fprintf (Data -> fp, "Capacity is BAD: capacity = %zd\n",Data -> capacity);

    if (error & FILE_NULL)
        fprintf (Data -> fp, "File pointer = NULL: file pointer = %p\n", Data -> fp);

    if (error & BAD_CANARY1_S)
        fprintf (Data -> fp, "Canary1_struct is BAD: canary1_struct = %d\n", Data -> canary1_struct);

    if (error & BAD_CANARY2_S)
        fprintf (Data -> fp, "Canary2_struct is BAD: canary2_struct = %d\n", Data -> canary2_struct);

    if (error & BAD_CANARY1_B)
        fprintf (Data -> fp, "Canary1_buf is BAD: canary1_buf = %d\n", Data -> buffer[0]);

    if (error & BAD_CANARY2_B)
        fprintf (Data -> fp, "Canary2_buf is BAD: canary2_buf = %d\n", Data -> buffer[Data -> capacity + 1]);

    if (error & BAD_HASH_STRUCT)
         fprintf (Data -> fp, "Hash_struct is BAD: hash_struct = %lld\n", Data -> hash_struct);

    if (error & BAD_HASH_BUF)
         fprintf (Data -> fp, "Hash_buf is BAD: hash_buf = %lld\n", Data -> hash_buffer);

    if (error == 0)
        fprintf (Data -> fp, "All it is OK\n");

    fprintf (Data -> fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    return error;
}
//==================================================================================================
uint_t Hash (const void* ptr, size_t size)
{
    uint_t hash = 5381;
    const char* data = (const char*) ptr;
    for (uint_t i = 0; i < ; ++i)
        hash = hash * 33 ^ data[i];
    return hash;
}
//==================================================================================================


