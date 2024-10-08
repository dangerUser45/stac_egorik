
#include "Common.h"
#include "Main.h"
#include "Debug.h"


int main ()
{
    stack_t Data = {};
    ONDEBUG (Create_file (&Data);)

    Ctor (&Data, 4);

    Pop (&Data) OR DIE;
    ONDEBUG (Dump (&Data);)

    Dtor (&Data) OR DIE;
    ONDEBUG (Dump (&Data);)

    return NO_ERROR_;
}

int Ctor (stack_t* Data, ssize_t capacity ONDEBUG(, const char* name, const char* file, int line))
{

    Data -> capacity = capacity;
    Data -> size = 0;
    stack_el_t* buffer = (stack_el_t*) calloc (capacity ONDEBUG(+ 2), sizeof (stack_el_t));

    if (buffer == NULL) return BUFFER_NULL;
    Data -> buffer = buffer;

    ONDEBUG (Data -> canary1_struct = CANARY_S;)
    ONDEBUG (Data -> name = name;)
    ONDEBUG (Data -> file = file;)
    ONDEBUG (Data -> line = line;)
    ONDEBUG (Data -> canary2_struct = CANARY_S;)
    ONDEBUG (Canary (Data);)

    ONDEBUG (Fill_Poison (Data -> buffer + Data -> size + 1, Data -> capacity);)

    CHECK ( , "Ctor")

    return NO_ERROR_;
}

//==================================================================================================
int Push (stack_t* Data, stack_el_t elem)
{
    CHECK (return GENERAL_ERROR;, "Push")

    if (Data -> size >= Data -> capacity - 1)
    {
        fprintf (Data -> fp, "its realloc up\n");
        Stack_Realloc_Up (Data);
        ONDEBUG (Fill_Poison (Data -> buffer + Data -> size + 1, Data -> capacity);)
        ONDEBUG (Canary (Data);)
    }
    ONDEBUG (fprintf (Data -> fp, "\t\t\tsize = %zd\n", Data -> size);)
    ssize_t size = Data->size;
    Data -> buffer [size  ONDEBUG( + 1)] = elem;
    Data -> size += 1;

    CHECK (return GENERAL_ERROR;, "Push")

    return NO_ERROR_;
}
//==================================================================================================
int Pop (stack_t* Data)
{

    //CHECK ( , "Pop")
    ONDEBUG (fprintf (Data -> fp, "HERE size = %zd\n", Data -> size);)

    if (Data -> size <= 0) return BAD_SIZE;
    ONDEBUG (fprintf (Data -> fp, "RTRTRTR\n");) //!!!
    if (Data -> size < Data -> capacity / (MAGIC_NUM * MAGIC_NUM) )
    {
        ONDEBUG (fprintf (Data -> fp, "Its realloc_down\n");)
        Stack_Realloc_Down (Data);
        ONDEBUG (Fill_Poison (Data -> buffer + Data -> size + 1, Data -> capacity);)
        ONDEBUG (Canary (Data);)
    }

    ssize_t size = Data -> size;
    ONDEBUG (fprintf (Data -> fp, "size = %zd\n", size);)
    ONDEBUG (fprintf(Data -> fp, "addr_popa = %p\n",  Data -> buffer +size );)

    Data -> buffer [size] = POISON;
    Data -> size -= 1;
    CHECK ( , "Pop")

    return NO_ERROR_;
}
//==================================================================================================
int Dtor (stack_t* Data)
{

    CHECK (return GENERAL_ERROR;, "Dtor")

    free (Data->buffer);
    ONDEBUG (fprintf(Data ->fp, "зафричилось\n");)

    CHECK ( , "Dtor") // noreturn because use after free ()

    return NO_ERROR_;
}
//==================================================================================================

#define PLEASE_NOTE_THAT_DNLX_DID_THAT(...) __VA_ARGS__

int Stack_Realloc_Up (stack_t* Data)
{
    CHECK ( , "Stack_Realloc_Up") //
    Data -> buffer = (stack_el_t*) realloc (Data -> buffer,
                                            PLEASE_NOTE_THAT_DNLX_DID_THAT ((Data -> capacity * MAGIC_NUM + 2) * sizeof (Data->capacity))); //capacity is
    Data -> capacity = Data ->capacity * MAGIC_NUM;

    CHECK ( , "Stack_Realloc_Up")
    return NO_ERROR_;
}
#undef PLEASE_NOTE_THAT_DNLX_DID_THAT

//==================================================================================================
int Stack_Realloc_Down (stack_t* Data)
{
    CHECK ( , "Stack_Realloc_Down")
    Data -> buffer = (stack_el_t*) realloc (Data -> buffer, (Data -> capacity * MAGIC_NUM + 2) * sizeof (Data->capacity));
    Data -> capacity = Data -> capacity / (MAGIC_NUM * MAGIC_NUM);
    CHECK ( , "Stack_Realloc_Down")

    return NO_ERROR_;
}
//==================================================================================================
