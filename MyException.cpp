#include "stdafx.h"
#include "MyException.h"
#include <eh.h>



void my_except_trans_func( unsigned int u, EXCEPTION_POINTERS* pExp )
{
    printf( "In trans_func.\n" );
    throw CMyException();
}

void MyExceptInit(void)
{
    _set_se_translator(my_except_trans_func);
}
