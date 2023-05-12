#ifndef __myshell_H
#define __myshell_H

#include "ntshell.h"
#include "ntlibc.h"
#include "ntopt.h"
#include "communicate.hpp"
#include "imu.hpp"
#include "encoder.hpp"

extern "C"
{
	void Myshell_Initialize(void);
	void Myshell_Execute(void);
}

#endif