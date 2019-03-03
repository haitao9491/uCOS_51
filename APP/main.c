/**
 * 	uCOS_51 for MCS-51
 * 	��ȡ������ϵͳʱ��
 *     OSTimeGet(); ��ȡϵͳʱ��
 *     OSTimeSet( ); ����ϵͳʱ��
 *    2012-5-31     lzy
 */

#include "..\ucos_51\ucos-ii\inc\includes.h"

// �������ȼ����� 
#define APPSTARTPRIO 	TASK_START_PRIO
#define POWERLEDPRIO 	OS_LOWEST_PRIO - 2		// �����Ϊ������ȼ��� ����������

sbit WorkLedCtrl=P3^4;	// �����Ŷ���


// ջ����
OS_STK AppStartStk[TASK_STK_SIZE];
OS_STK PowerLedStk[TASK_STK_SIZE];

// ��������
void AppStart(void *ppdata) REENTRANT;
void PowerLed(void *ppdata) REENTRANT;		// ����ָʾ������

int  main()
{
	InitSystem();	  // ϵͳ��ʼ��

	OSTaskCreate(AppStart, (void *)0, AppStartStk, APPSTARTPRIO);

	
	OSStart(); // �������������

	return 0;
}

  
/*
********************************************************************************************************* 
*                                             ϵͳ��ʼ����
*
* ����       : ����ʱ��T0�жϣ�������Ϣ�����Լ���������������Ȼ������ɾ����
*
* ����       : ppdata
*
* ע��       : ϵͳ�������һʱ�俪��ʱ���жϣ�����ϵͳ���������޷��õ�ִ�С�		                        	
*********************************************************************************************************
*/
void AppStart(void *ppdata) REENTRANT
{
	ppdata=ppdata;

	ET0=1; /* ���ǣ�ϵͳ�������һʱ�俪��ʱ���ж� */
	
	OSTaskCreate(PowerLed, (void *)0, PowerLedStk, POWERLEDPRIO);

	OSTaskDel(OS_PRIO_SELF);	// ����ɾ��	
}


// ����˸����
void PowerLed(void *ppdata) REENTRANT
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif

	ppdata=ppdata;

	for(;;)
	{
		if(OSTimeGet() == 10)  // ���ϵͳʱ��
		{
			OS_ENTER_CRITICAL();
			WorkLedCtrl=!WorkLedCtrl; /* �����ٽ���Դ */
			OS_EXIT_CRITICAL();

			OSTimeSet(0);		   // ����ϵͳʱ��
		}
	//	OSTimeDlyHMSM(0,0,1,0);
	}
}

