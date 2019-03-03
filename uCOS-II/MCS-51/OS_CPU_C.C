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
* File : OS_CPU_C.C
* Created by : QQ 591881218
*********************************************************************************************************
*/

#define  OS_CPU_GLOBALS

#include "..\ucos_51\ucos-ii\inc\includes.h"

/*
*********************************************************************************************************
*                                             ��ʼ������ջ
*
* ����       : ������ΪOSTaskCreate()��OSTaskCreateExt()���������ã���������ջ�ĳ�ʼ������ʼ���������ջ
*			   ��������ոշ�����һ���жϲ������еļĴ�����������˶�ջ������һ����
*
* ����       : task		��������ָ�롣
*
*			   ppdata	������ʼִ��ʱ���ݸ�����Ĳ�����ָ�롣
*
*			   ptos		����������ջ��ջ��ָ�롣
*
*			   opt		�����趨OSTaskCreateExt()��ѡ�ָ���Ƿ�������ջ���飬�Ƿ񽫶�ջ���㣬�Ƿ����
*						��������ȡ�����OSTaskCreate()��������ʱ������Ϊ0��
*
* ����ֵ	 : ����ջ��ָ��
* 
* ע��       : ����ջ�ṹʾ��ͼ(����)
*
*                                    ---------- -
*                 �û�ջ��ߵ�ַ---->|        | |
*                                    ---------- |
*                                    |   ...  | �����ջ�ռ�
*----------                          ---------- | ÿ����һ��
*|OSTCBCur|               ?C_XBP---->|        | | KEIL�Զ�����
*----------                          ---------- -
*    |                               |���м��|
*    |     -----------------------   ----------                           ----------
*    \---->|OSTCBCur->OSTCBStkPtr|   |?C_XBP��|                    SP---->|        |
*          -----------------------   ----------                           ----------
*                     |              |?C_XBP��|                           |        |
*                     |              ---------- -                         ----------
*                     |              |        | |                         |   .    |
*                     |              ---------- |                         |   .    |
*                     |              |        | |                         |   .    |
*                     |              ---------- |                         ----------
*                     |              |   .    |����                       |        | +1
*                     |              |   .    | |                         ----------
*                     |              |   .    | |             OSStack---->|        | 0
*                     |              ---------- |                         ----------
*                     |              |        | |          OSStkStart---->| ������ | -1  �͵�ַ
*                     |              ---------- -                         ----------
*                     \------------->|  ����  | �͵�ַ                   ϵͳӲ����ջ
*                                    ----------
*                                     �û���ջ                        ����=SP-OSStkStart
*********************************************************************************************************
*/
void *OSTaskStkInit(void(*task)(void *pd), void *ppdata, void *ptos, INT16U opt) REENTRANT
{
	OS_STK *stk;
	ppdata=ppdata;
	opt	  =opt;						/* optû���õ�������������ֹ������� 			*/
	stk	  =(OS_STK *)ptos;			/* �����ջ�����Ч��ַ 						*/
	*stk++=15;						/* �����ջ���� 								*/
	*stk++=(INT16U)task & 0xFF;		/* ��������ַ��8λ 							*/
	*stk++=(INT16U)task >> 8;		/* ��������ַ��8λ 							*/
	/* �������ǰ��ض���˳�򽫼Ĵ��������ջ�ģ������û��ڽ��Ĵ��������ջ��ʱ��ҲҪ������һ˳�� */
	*stk++=0x00;					/* PSW 											*/
	*stk++=0x0A;					/* ACC 											*/
	*stk++=0x0B;					/* B 											*/
	*stk++=0x00;					/* DPL 											*/
	*stk++=0x00;					/* DPH 											*/
	*stk++=0x00;					/* R0 											*/
	*stk++=0x01;					/* R1 											*/
	*stk++=0x02;					/* R2 											*/
	*stk++=0x03;					/* R3 											*/	
	*stk++=0x04;					/* R4 											*/
	*stk++=0x05;					/* R5 											*/
	*stk++=0x06;					/* R6 											*/
	*stk++=0x07;					/* R7 											*/
	/* ���ñ���SP�������л�ʱ�����û���ջ���ȼ���ó� 								*/
	*stk++=(INT16U)(ptos+MAX_STK_SIZE) >> 8;	/* ?C_XBP �����ջָ���8λ 		*/
	*stk++=(INT16U)(ptos+MAX_STK_SIZE) & 0xFF;	/* ?C_XBP �����ջ��8λ 			*/
	return ((void *)ptos);	/* ������͵�ַ�����ﲻ�õ���ջ��ָ����Ϊ����߼���Ч�� */
}

/*********************************************** ���Ӻ��� ***********************************************/

#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookEnd (void)
{
}
#endif

/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook (OS_TCB *ptcb) REENTRANT
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskDelHook (OS_TCB *ptcb) REENTRANT
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void) REENTRANT
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskStatHook (void) REENTRANT
{
}

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OSTCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSTCBInitHook (OS_TCB *ptcb) REENTRANT
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif

/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

void OSTimeTickHook (void) REENTRANT
{
}

/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION >= 251
void OSTaskIdleHook (void) REENTRANT
{
}
#endif

#endif

/********************************************************************************************************/

/* ��ʼ����ʱ��0�����ڲ���ʱ�ӽ��� */
void InitTimer0(void) REENTRANT
{
    TMOD=TMOD&0xF0;
    TMOD=TMOD|0x01;	/* ģʽ1(16λ��ʱ��)������TR0���� */
	TH0=TIMER_20MS_TH0;
	TL0=TIMER_20MS_TL0;
	EA=0; /* EA��ET0��51�ϵ�ȱʡֵΪ0��EA����OSStartHighRdy()�д� */
	ET0=0; /* ������OSStart()ǰ������ʱ���жϵ�Ҫ��ϵͳ�������һʱ�俪��ʱ��T0�ж� */	
	TR0=1;   
}
