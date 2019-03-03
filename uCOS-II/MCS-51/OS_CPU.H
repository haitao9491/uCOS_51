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
* File : OS_CPU.H
* Created by : QQ 591881218
*********************************************************************************************************
*/

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*************************************** ��������йص��������� ***************************************/

typedef unsigned char 	BOOLEAN;	/* ����ʹ��bit���壬��Ϊ�ڽṹ�����޷�ʹ�� 	*/
typedef unsigned char 	INT8U;		/* �޷���8λ�� 								*/
typedef signed char 	INT8S;		/* �з���8λ�� 								*/
typedef unsigned int 	INT16U;		/* �޷���16λ�� 							*/
typedef signed int 		INT16S;		/* �з���16λ��								*/
typedef unsigned long 	INT32U;		/* �޷���32λ�� 							*/		
typedef signed long 	INT32S;		/* �з���32λ�� 							*/
typedef float 			FP32;		/* �����ȸ����� 							*/
typedef double 			FP64;		/* ˫���ȸ����� 							*/
typedef unsigned char 	OS_STK;		/* �����ջ��ڿ���Ϊ8λ 					*/
typedef unsigned char 	OS_CPU_SR;	/* ����CPU״̬�ֵĿ���Ϊ8λ 				*/

// ---------------------------------

#define REENTRANT		reentrant 	/* ʹKeil���ɿ�������� 					*/

/* �洢���͵ĺ� 																*/

#define DF_DATA			data
#define DF_BDATA		bdata
#define DF_IDATA		idata
#define DF_XDATA		xdata
#define DF_PDATA		pdata
#define DF_CODE			code

// ---------------------------------

/* �����Ǽ���uC/OS v1.XX���������ͣ���uC/OS-II��û��ʹ��  						*/

#define BYTE 			INT8S
#define UBYTE 			INT8U
#define WORD 			INT16S
#define UWORD 			INT16U
#define LONG 			INT32S
#define ULONG 			INT32U

/***************************************** �봦�����йصĴ��� *****************************************/

#define OS_CRITICAL_METHOD	1 /* ������ʹ�õ�һ�ַ�ʽ�����ж� 			*/

#if OS_CRITICAL_METHOD==1
#define OS_ENTER_CRITICAL()	EA=0 /* ֱ�ӽ�ֹ�ж� 						*/
#define OS_EXIT_CRITICAL()	EA=1 /* ֱ�������ж� 						*/
#endif

#if OS_CRITICAL_METHOD==2 
#define OS_ENTER_CRITICAL() /* ���ö�ջ�����ж�״̬���ٹ��ж� 			*/
#define OS_EXIT_CRITICAL() /* ������״̬�ִӶ�ջ�������ٿ��ж� 			*/
#endif

/* ���ź��������ַ�ʽ�����Բ�ˬ�� */
#if OS_CRITICAL_METHOD==3
#define OS_ENTER_CRITICAL()	cpu_sr=IE & 0x80;IE &= 0x7F /* ��ֹ�ж� 	*/
#define OS_EXIT_CRITICAL()	IE |= cpu_sr /* �����ж� 					*/
#endif 

#define OS_STK_GROWTH	0 /* MCS-51��ջ��������������1=���£�0=���� 	*/

#define OS_TASK_SW()	OSCtxSw() /* ���񼶵������л��� 				*/
