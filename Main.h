int Ctor (stack_t* Data, ssize_t capacity ONDEBUG(,const char* name, const char* file, int line));
int Push (stack_t* Data, stack_el_t elem);
int Pop (stack_t* Data);
int Dtor (stack_t* Data);
int Stack_Realloc_Up (stack_t* Data);
int Stack_Realloc_Down (stack_t* Data); //
