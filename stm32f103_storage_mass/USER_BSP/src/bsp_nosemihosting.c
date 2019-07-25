#include "stdio.h"
#include "rt_sys.h"
#include "rt_heap.h"
#include "rt_fp.h"
#include "rt_locale.h"
#include "rt_misc.h"
#include "rt_memory.s"

#include "ctype.h"
#include "errno.h"
#include "fenv.h"
#include "alloca.h"
#include "assert.h"
#include "float.h"
#include "inttypes.h"
#include "limits.h"
#include "math.h"
//int $super$$fputc(int c, FILE *fp);
//int $sub$$fputc(int c, FILE *fp)
//{
//	if(fp == (FILE*) MAGIC_NUM)
//	{
//		write_to_uart(c);
//	}
//	else
//	{
//		return $super$$fputc(c, fp);
//	}	
//}

#if 1
__asm(".global __use_no_semihosting\n\t");     
#else
#param import(__use_no_semihosting_)
#endif

//Direct semihosting C library function dependencies
//int __user_inital_stackheap(){}
//void _sys_exit(){}
//void _ttywrch(){}
//void _sys_command_string(){}
//void _sys_close(){}
//void _sys_iserror(){}
//void _sys_istty(){}
//void _sys_flen(){}
//void _sys_open(){}
//void _sys_read(){}
//void _sys_seek(){}
//void _sys_write(){}
//void _sys_tmpnam(){}
//void clock(){}
//void _clock_init(){}
//void remove(){}
//void rename(){}
//void system(){}
//void time(){}

//Indirect semihosting C library function dependencies
//int __user_setup_stackheap(){}
//__raise()
//__default_signal_handler()
//__Heap_Initialize()
//ferror(), fputc(), __stdout
//__backspace(), fgetc(), __stdin
//fwrite(), fputs(), puts(),
//fread(), fgets(), gets(), ferror()

