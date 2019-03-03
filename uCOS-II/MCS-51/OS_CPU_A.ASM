;/*
;*********************************************************************************************************
;*                                                uC/OS-II
;*                                          The Real-Time Kernel
;*
;*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
;*                                           All Rights Reserved
;*
;*
;*                                           uCOS_51 for MCS-51
;*
;* File : OS_CPU_A.ASM
;* From : chen shi zhi
;* Last modified by : QQ 591881218
;*********************************************************************************************************
;*/

$NOMOD51
EA		BIT		0A8H.7
SP		DATA	081H
B		DATA	0F0H
ACC		DATA	0E0H
DPH		DATA	083H
DPL		DATA	082H
PSW		DATA	0D0H
TR0		BIT		088H.4
EX0		BIT		0A8H.0
EX1		BIT		0A8H.2
ES		BIT		0A8H.4
TH0		DATA	08CH
TL0		DATA	08AH

TIMER_20MS_TH0 	EQU	 070H	;CPU=22.1184MHz, OS_TICKS_PER_SEC=50, TH0=0x70
TIMER_20MS_TL0	EQU	 000H	;CPU=22.1184MHz, OS_TICKS_PER_SEC=50, TL0=0x00

        NAME OS_CPU_A	;ģ����
        
;�����ض�λ��
;1���޲κ��� ?PR?������?�ļ���
;2���вκ��� ?PR?_������?�ļ���
;3�����뺯�� ?PR?_?������?�ļ���
?PR?OSStartHighRdy?OS_CPU_A    SEGMENT CODE
?PR?OSCtxSw?OS_CPU_A           SEGMENT CODE
?PR?OSIntCtxSw?OS_CPU_A        SEGMENT CODE
?PR?OSTickISR?OS_CPU_A         SEGMENT CODE

?PR?_?SerialISR?OS_CPU_A	   SEGMENT CODE
        
;��������ȫ�ֱ������ⲿ�ӳ���
        EXTRN DATA  (?C_XBP)	;�����ջָ�����ڿ�����ֲ���������

        EXTRN IDATA (OSTCBCur)
        EXTRN IDATA (OSTCBHighRdy)
        EXTRN IDATA (OSRunning)
        EXTRN IDATA (OSPrioCur)
        EXTRN IDATA (OSPrioHighRdy)
    
        EXTRN CODE  (_?OSTaskSwHook)
        EXTRN CODE  (_?OSIntEnter)
        EXTRN CODE  (_?OSIntExit)
        EXTRN CODE  (_?OSTimeTick)

		EXTRN CODE  (_?Serial)        
            
;��������4���������뺯��
        PUBLIC OSStartHighRdy
        PUBLIC OSCtxSw
        PUBLIC OSIntCtxSw
        PUBLIC OSTickISR
        
        ;PUBLIC SerialISR        
    
;�����ջ�ռ䡣ֻ���Ĵ�С����ջ�����keil������ͨ����ſ��Ի��keil�����SP���
?STACK SEGMENT IDATA
        RSEG ?STACK

OSStack:
		;DS n������n���洢��Ԫ
        DS 40H		;����Ӳ����ջ�Ĵ�С

OSStkStart IDATA OSStack-1

;����ѹջ��ջ��
PUSHALL    MACRO
        PUSH PSW
        PUSH ACC
        PUSH B
        PUSH DPL
        PUSH DPH
        MOV  A,R0	;R0~R7��ջ
        PUSH ACC
        MOV  A,R1
        PUSH ACC
        MOV  A,R2
        PUSH ACC
        MOV  A,R3
        PUSH ACC
        MOV  A,R4
        PUSH ACC
        MOV  A,R5
        PUSH ACC
        MOV  A,R6
        PUSH ACC
        MOV  A,R7
        PUSH ACC
        ;PUSH SP	;���ر���SP�������л�ʱ����Ӧ�������
        ENDM
    
POPALL    MACRO
        ;POP  ACC	;���ر���SP�������л�ʱ����Ӧ�������
        POP  ACC	;R0~R7��ջ
        MOV  R7,A
        POP  ACC
        MOV  R6,A
        POP  ACC
        MOV  R5,A
        POP  ACC
        MOV  R4,A
        POP  ACC
        MOV  R3,A
        POP  ACC
        MOV  R2,A
        POP  ACC
        MOV  R1,A
        POP  ACC
        MOV  R0,A
        POP  DPH
        POP  DPL
        POP  B
        POP  ACC
        POP  PSW
        ENDM
    
;--------------------
;�ӳ���
;--------------------

;			--------------------------------------------------
;			---------------- OSStartHighRdy() ----------------
;			--------------------------------------------------

        RSEG ?PR?OSStartHighRdy?OS_CPU_A

;�����û���չ����OSTaskSwHook()
;OSRunning=TRUE
;ȡ�ý�Ҫ�ָ�������Ķ�ջָ�룺Stack pointer=OSTCBHighRdy->OSTCBStkPtr
;�����мĴ������ݴ�����ջ�е�����
;ִ���жϷ���ָ��

OSStartHighRdy:

        USING 0		;�ϵ��51�Զ����жϣ��˴�������CLR EAָ���Ϊ���˴���δ���жϣ��������˳��󣬿��ж�
        LCALL _?OSTaskSwHook
        
        ;OSRunning=TRUE
        MOV  R0,#LOW (OSRunning)
        MOV  @R0,#01

;--------------------
;OSCtxSw_in
;--------------------

OSCtxSw_in:

        ;OSTCBHighRdy ==> DPTR ��õ�ǰ������������ȼ������TCBָ��
        MOV  R0,#LOW (OSTCBHighRdy)	;���OSTCBHighRdyָ��͵�ַ��ָ��ռ3B��+0���� +1��8λ���� +2��8λ����
        INC  R0
        MOV  DPH,@R0    ;ȫ�ֱ���OSTCBHighRdy��IDATA��
        INC  R0
        MOV  DPL,@R0
    
        ;OSTCBHighRdy->OSTCBStkPtr ==> DPTR ����û���ջָ��
        INC  DPTR		;ָ��ռ3B��+0���� +1��8λ���� +2��8λ����
        MOVX A,@DPTR	;OSTCBStkPtr��voidָ��
        MOV  R0,A
        INC  DPTR
        MOVX A,@DPTR
        MOV  R1,A
        MOV  DPH,R0
        MOV  DPL,R1
    
        ;*UserStkPtr ==> R5 �û���ջ��ʼ��ַ����(���û���ջ���ȷ��ڴ˴�)   
        MOVX A,@DPTR	;�û���ջ����unsigned char��������
        MOV  R5,A		;R5=�û���ջ����
    
        ;�ָ��ֳ���ջ����
        MOV  R0,#OSStkStart

;--------------------
;restore_stack
;--------------------
        
restore_stack:
    
        INC  DPTR
        INC  R0
        MOVX A,@DPTR
        MOV  @R0,A
        DJNZ R5,restore_stack
    
        ;�ָ���ջָ��SP
        MOV  SP,R0
    
        ;�ָ������ջָ��?C_XBP        
        INC  DPTR
        MOVX A,@DPTR
        MOV  ?C_XBP,A	;?C_XBP �����ջָ���8λ
        INC  DPTR
        MOVX A,@DPTR
        MOV  ?C_XBP+1,A	;?C_XBP �����ջָ���8λ
  
        POPALL
        SETB EA		;���ж�
        RETI

;			--------------------------------------------------
;			------------------- OSCtxSw() --------------------
;			--------------------------------------------------

        RSEG ?PR?OSCtxSw?OS_CPU_A

;���洦�����Ĵ���
;����ǰ����Ķ�ջָ�뱣�浽��ǰ�����OS_TCB�У�OSTCBCur->OSTCBStkPtr=Stack pointer
;�����û���չ����OSTaskSwHook()
;OSTCBCur=OSTCBHighRdy
;OSPrioCur=OSPrioHighRdy
;�õ���Ҫ�ָ�������Ķ�ջָ�룺Stack pointer=OSTCBHighRdy->OSTCBStkPtr
;�����д������Ĵ�����������Ķ�ջ�лָ�����
;ִ���жϷ���ָ��

OSCtxSw:  

        USING 0  
        PUSHALL

;--------------------
;OSIntCtxSw_in
;--------------------
        
OSIntCtxSw_in:

        ;��ö�ջ���Ⱥ���ʼ��ַ������ջ�ṹʾ��ͼ(OS_CPU_C.C)
        MOV  A,SP
        CLR  C	;��λ��־λCy����
        SUBB A,#OSStkStart	;A <- A-OSStkStart-Cy
        MOV  R5,A     ;��ö�ջ����        
    
        ;OSTCBCur ==> DPTR  ��õ�ǰTCBָ��
        MOV  R0,#LOW (OSTCBCur) ;���OSTCBCurָ��͵�ַ��ָ��ռ3B��+0���� +1��8λ���� +2��8λ����
        INC  R0
        MOV  DPH,@R0	;ȫ�ֱ���OSTCBCur��IDATA��
        INC  R0
        MOV  DPL,@R0
    
        ;OSTCBCur->OSTCBStkPtr ==> DPTR  ����û���ջָ��
        INC  DPTR        ;ָ��ռ3B��+0���� +1��8λ���� +2��8λ����
        MOVX A,@DPTR     ;OSTCBStkPtr��voidָ��
        MOV  R0,A
        INC  DPTR
        MOVX A,@DPTR
        MOV  R1,A
        MOV  DPH,R0
        MOV  DPL,R1
        
        ;�����ջ����
        MOV  A,R5
        MOVX @DPTR,A

        MOV  R0,#OSStkStart	;��ö�ջ��ʼ��ַ

;--------------------
;save_stack
;--------------------

save_stack:
   
        INC  DPTR
        INC  R0
        MOV  A,@R0
        MOVX @DPTR,A
        DJNZ R5,save_stack
        
        ;��������ջָ��?C_XBP
        INC  DPTR
        MOV  A,?C_XBP	;?C_XBP �����ջָ���8λ
        MOVX @DPTR,A
        INC  DPTR
        MOV  A,?C_XBP+1	;?C_XBP �����ջָ���8λ
        MOVX @DPTR,A        
    
        ;�����û�����
        LCALL _?OSTaskSwHook

;--------------------
;OSIntCtxSw_in
;--------------------
        
;OSIntCtxSw_in:

        ;OSTCBCur = OSTCBHighRdy
        MOV  R0,#OSTCBCur
		MOV  R1,#OSTCBHighRdy
		MOV  A,@R1
        MOV  @R0,A
        INC  R0
		INC  R1
		MOV  A,@R1
        MOV  @R0,A
        INC  R0
		INC  R1
		MOV  A,@R1
        MOV  @R0,A
                
        ;OSPrioCur = OSPrioHighRdy ʹ��������������ҪĿ����Ϊ��ʹָ��Ƚϱ�Ϊ�ֽڱȽϣ��Ա��ʡʱ��
        MOV  R0,#OSPrioCur
		MOV  R1,#OSPrioHighRdy
		MOV  A,@R1
        MOV  @R0,A
        
        LJMP OSCtxSw_in

;			-------------------------------------------
;			--------------- OSIntCtxSw() --------------
;			-------------------------------------------

        RSEG ?PR?OSIntCtxSw?OS_CPU_A

;������ջָ����ȥ���ڵ��ù�����ѹ���ջ�Ķ�������
;����ǰ�����ջָ�뱣�浽��ǰ�����OS_TCB�У�OSTCBCur->OSTCBStkPtr=Stack pointer
;�����û���չ����OSTaskSwHook()
;OSTCBCur=OSTCBHighRdy
;OSPrioCur=OSPrioHighRdy
;�õ���Ҫ�ָ�������Ķ�ջָ�룺Stack pointer=OSTCBHighRdy->OSTCBStkPtr
;�����д������Ĵ�����������Ķ�ջ�лָ�����
;ִ���жϷ���ָ��
        
OSIntCtxSw:

        USING 0

		;����SPָ��ȥ���ڵ���OSIntExit()��OSIntCtxSw()������ѹ���ջ�Ķ�������
		;SP=SP-4
		MOV  A,SP
		CLR  C
		SUBB A,#4
		MOV  SP,A

		;LCALL _?OSTaskSwHook
        LJMP OSIntCtxSw_in

;			-------------------------------------------
;			--------------- OSTickISR() ---------------
;			-------------------------------------------

        CSEG AT 000BH	;��ʱ��T0�ж�
;CSEG [AT ���Ե�ַ����ʽ] //���Դ����
;DSEG [AT ���Ե�ַ����ʽ] //�ڲ��������ݶ�
;XSEG [AT ���Ե�ַ����ʽ] //�ⲿ�������ݶ�
;ISEG [AT ���Ե�ַ����ʽ] //�ڲ����Ѱַ�������ݶ�
;BSEG [AT ���Ե�ַ����ʽ] //����λѰַ��
        LJMP OSTickISR
        RSEG ?PR?OSTickISR?OS_CPU_A

;���ж�
;���洦�����Ĵ�����ֵ
;����OSIntEnter()���ǽ�OSIntNesting��1
;��ʱ���ж�
;����OSTimeTick()
;��ʱ���ж�
;����OSIntExit()
;�ָ��������Ĵ�����ֵ
;���������ж�
;ִ���жϷ���ָ��

OSTickISR:        
        
        USING 0		;�����Ĵ���0
        CLR  EA		;���жϣ���ֹ�ж�Ƕ��
        PUSHALL		;�ֳ�����
        LCALL _?OSIntEnter	;֪ͨ�ں˽����ж�
              
		CLR  TR0
        MOV  TH0,#TIMER_20MS_TH0	;����Tick=50��/�룬��0.02��/��
        MOV  TL0,#TIMER_20MS_TL0	;OS_CPU_C.C �� OS_TICKS_PER_SEC
       	LCALL _?OSTimeTick	;�����жϷ����ӳ���
		SETB TR0 

        LCALL _?OSIntExit	;֪ͨ�ں��˳��ж�
        POPALL		;�ָ��ֳ�    
		SETB EA 	;���ж�    
        RETI

;--------------------
;SerialISR
;--------------------

        CSEG AT 0023H	;�����ж�
        LJMP SerialISR
        RSEG ?PR?_?SerialISR?OS_CPU_A
    
SerialISR:
        
        USING 0
        CLR  EA		;���жϣ���ֹ�ж�Ƕ��
        PUSHALL
        LCALL _?OSIntEnter	;֪ͨ�ں˽����ж�
      	
		CLR ES
        LCALL _?Serial	;�����жϷ����ӳ���	 
		SETB ES
		       
        LCALL _?OSIntExit	;֪ͨ�ں��˳��ж�
        POPALL
		SETB EA        
        RETI

;--------------------
END
;--------------------