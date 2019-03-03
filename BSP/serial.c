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
* File : serial.c
* From : yang yi
* Last modified by : QQ 591881218
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "..\ucos_51\ucos-ii\inc\includes.h"
#endif

unsigned char TxBuf[LenTxBuf],RxBuf[LenRxBuf];
unsigned char *inTxBuf,*outTxBuf,*inRxBuf,*outRxBuf;
bit TIflag=1;

void InitSerial(void) REENTRANT // ���ڳ�ʼ��
{
	TMOD=TMOD&0x0F;
	TMOD=TMOD|0x20; // ģʽ2(8λ�Զ���װ)������TR1����
	TH1=0xFD;TL1=0xFD, // CPU=22.1184MHz��������=19200�����=0% 
	SCON=0x50;PCON=0x00; // ģʽ2��SM2=0��SMOD=0
	TR1=1;
}

void InitSerialBuffer(void) REENTRANT // ���ڻ�������ʼ��
{
	inTxBuf=TxBuf;outTxBuf=TxBuf;
	inRxBuf=RxBuf;outRxBuf=RxBuf;
	ES=1;
}

void Serial(void) REENTRANT	// �����жϷ����ӳ����ж��ڻ����ʵ�֣�ȥ��interrupt 4 
{  
	unsigned char *t;
	
	/* ѭ�������� */
	if(TI){
		TI=0;
		if(inTxBuf==outTxBuf) {TIflag=1;return;} // TxBuf Empty
		SBUF=*outTxBuf;outTxBuf++;
		if(outTxBuf==TxBuf+LenTxBuf) outTxBuf=TxBuf;	
	}
	if(RI){
		RI=0;
		t=inRxBuf;t++;
		if(t==RxBuf+LenRxBuf) t=RxBuf;
		if(t==outRxBuf) return; // RxBuf Full
		*inRxBuf=SBUF;
		inRxBuf=t;
	}
}

bit Getch(unsigned char *ch) REENTRANT // �Ӵ��ڻ�������1�ֽ�����
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
	
	OS_ENTER_CRITICAL();/* �����ٽ��� */      
	if(inRxBuf==outRxBuf) {OS_EXIT_CRITICAL();/* �˳��ٽ��� */return 0;} // RxBuf Empty
	*ch=*outRxBuf;outRxBuf++;
	if(outRxBuf==RxBuf+LenRxBuf) outRxBuf=RxBuf;  
	OS_EXIT_CRITICAL();/* �˳��ٽ��� */     
	return 1;
}

void PrintChar(unsigned char ch) REENTRANT // ��ʾ�ַ�
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif

	unsigned char *t;

	OS_ENTER_CRITICAL();/* �����ٽ��� */

/*
#pragma ASM
    PUSH IE; // �����ж�״̬
    CLR EA; // ���ж�
#pragma ENDASM
*/

	t=inTxBuf;t++;
	if(t==TxBuf+LenTxBuf) t=TxBuf;
	if(t==outTxBuf){
		OS_EXIT_CRITICAL();/* �˳��ٽ��� */

		/*
		#pragma ASM
		    POP IE;	// �ָ�֮ǰ�ж�״̬
		#pragma ENDASM
		*/

		return;
	} // TxBuf Full
	*inTxBuf=ch;
	inTxBuf=t;

	OS_EXIT_CRITICAL();/* �˳��ٽ��� */

/*
#pragma ASM
    POP IE; // �ָ�֮ǰ�ж�״̬
#pragma ENDASM
*/

	if(TIflag){	
		OS_ENTER_CRITICAL();/* �����ٽ��� */	
		TIflag=0;
		OS_EXIT_CRITICAL();/* �˳��ٽ��� */ 
		TI=1; // �������ڷ����ж�	
	}
}

void PrintStr(unsigned char *str) REENTRANT // ��ʾ�ַ���
{ 
	int i;
	unsigned char j;
	unsigned char ch;
		
/*
#pragma ASM
    PUSH IE;
    CLR EA;
#pragma ENDASM
*/
    /* ����������Ҫ�ٽ籣�� */
	for(i=0;i<MaxLenStr;i++){
		ch=*(str+i);
		if(ch=='\0') break;
		else if(ch=='\n'){PrintChar(10);PrintChar(13);} 
		else if(ch=='\t'){
			for(j=0;j<TABNum;j++)
				PrintChar(' ');
		}
		else PrintChar(ch);
	}

/*
#pragma ASM
    POP IE;
#pragma ENDASM
*/
}

void ClrScr(void) REENTRANT // ����
{        
	PrintStr("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); // 25���س���������
}
