int Create_file (stack_t* Data);
int Dump (stack_t* Data);
int Canary (stack_t* Data);
int Fill_Poison (stack_el_t* begin, uint_t quantity);
int Decoder_error (stack_t* Data, int error, int line, const char* name_func);
int Verificator (stack_t* Data);
uint_t Hash (const void* ptr, size_t size);

