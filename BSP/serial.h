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
* File : serial.h
* From : yang yi
* Last modified by : QQ 591881218
*********************************************************************************************************
*/

/* �շ����������� */
#define LenTxBuf        2000    // Don't too small, because it will cause display abnormal.
#define LenRxBuf        50

#define MaxLenStr       100     // buf[MaxLenStr+1] for '\0'

#define	TABNum          4       // TAB���ƶ�����


void Serial(void) REENTRANT; // �����жϷ����ӳ���

void InitSerial(void) REENTRANT; // ���ڳ�ʼ��
void InitSerialBuffer(void) REENTRANT; // ���ڻ�������ʼ��
bit Getch(unsigned char *ch) REENTRANT;	// �Ӵ��ڻ�������1�ֽ�����
void PrintChar(unsigned char ch) REENTRANT;	// ��ʾ�ַ�
void PrintStr(unsigned char *str) REENTRANT; // ��ʾ�ַ���
void ClrScr(void) REENTRANT; // ����

