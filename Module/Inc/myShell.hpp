#ifndef __myshell_H
#define __myshell_H

#include "communicate.hpp"
#include "ntshell.h"
#include "ntlibc.h"
#include "ntopt.h"

extern "C"
{
	void Myshell_Initialize(void);
	void Myshell_Execute(void);
}

#endif