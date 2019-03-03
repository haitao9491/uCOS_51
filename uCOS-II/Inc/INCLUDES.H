/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*
*                                           uCOS_51 for MCS-51
*
* File : INCLUDES.H
* Created by : QQ 591881218
*********************************************************************************************************
*/

// ---------------------------------

#include "..\ucos_51\ucos-ii\mcs-51\os_cpu.h"
#include "..\ucos_51\ucos-ii\inc\os_cfg.h"
#include "..\ucos_51\ucos-ii\src\ucos_ii.h"

// ---------------------------------

//#include <reg51.h>
#include <reg52.h> 
#include <string.h>

// ---------------------------------

#include "..\ucos_51\bsp\serial.h"
#include "..\ucos_51\bsp\system.h"

// ---------------------------------

void InitTimer0(void) REENTRANT;


