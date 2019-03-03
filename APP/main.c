/**
 * 	uCOS_51 for MCS-51
 * 	获取和设置系统时间
 *     OSTimeGet(); 获取系统时间
 *     OSTimeSet( ); 设置系统时间
 *    2012-5-31     lzy
 */

#include "..\ucos_51\ucos-ii\inc\includes.h"

// 任务优先级别定义 
#define APPSTARTPRIO 	TASK_START_PRIO
#define POWERLEDPRIO 	OS_LOWEST_PRIO - 2		// 定义灯为最低优先级别 【空闲任务】

sbit WorkLedCtrl=P3^4;	// 灯引脚定义


// 栈定义
OS_STK AppStartStk[TASK_STK_SIZE];
OS_STK PowerLedStk[TASK_STK_SIZE];

// 任务声明
void AppStart(void *ppdata) REENTRANT;
void PowerLed(void *ppdata) REENTRANT;		// 工作指示灯任务

int  main()
{
	InitSystem();	  // 系统初始化

	OSTaskCreate(AppStart, (void *)0, AppStartStk, APPSTARTPRIO);

	
	OSStart(); // 启动多任务管理

	return 0;
}

  
/*
********************************************************************************************************* 
*                                             系统开始任务
*
* 描述       : 开定时器T0中断，创建消息邮箱以及所有其他的任务，然后自我删除。
*
* 参数       : ppdata
*
* 注意       : 系统启动后第一时间开定时器中断，否则系统其它任务无法得到执行。		                        	
*********************************************************************************************************
*/
void AppStart(void *ppdata) REENTRANT
{
	ppdata=ppdata;

	ET0=1; /* 谨记：系统启动后第一时间开定时器中断 */
	
	OSTaskCreate(PowerLed, (void *)0, PowerLedStk, POWERLEDPRIO);

	OSTaskDel(OS_PRIO_SELF);	// 自我删除	
}


// 灯闪烁任务
void PowerLed(void *ppdata) REENTRANT
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif

	ppdata=ppdata;

	for(;;)
	{
		if(OSTimeGet() == 10)  // 获得系统时间
		{
			OS_ENTER_CRITICAL();
			WorkLedCtrl=!WorkLedCtrl; /* 保护临界资源 */
			OS_EXIT_CRITICAL();

			OSTimeSet(0);		   // 设置系统时间
		}
	//	OSTimeDlyHMSM(0,0,1,0);
	}
}

