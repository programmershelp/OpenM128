#include<avr/io.h>
#include<avr/delay.h>
#include<avr/interrupt.h>
#include<stdio.h>
#include"USART.H"
#include"AT24Cxx.h"


void twi_init(void)
{
	TWBR = 0X0F;		//����TWI������  
	TWSR &= 0XFC;		//����TWIԤ��Ƶ Ϊ 1
	
}


/*--------------------------------------------------------------------
�������ƣ�I2C Start
�������ܣ�
ע�����
��ʾ˵����
��    �룺
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_Start()					
{
	I2CStart();						
	I2CWaitAck();
	if( I2CChkAck()!=I2C_START ) 
		return I2C_ERR;
	return I2C_CRR;
}
/*--------------------------------------------------------------------
�������ƣ�I2C ReStart
�������ܣ�
ע�����
��ʾ˵����
��    �룺
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_Restart()					
{
	I2CStart();						
	I2CWaitAck();
	if( I2CChkAck()!=I2C_RESTART ) 
		return I2C_ERR;
	return I2C_CRR;
}
/*--------------------------------------------------------------------
�������ƣ����� 7λ ����д��ַ: XXXX XXX0
�������ܣ�
ע�����
��ʾ˵����
��    �룺
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_SendWrDAdr(uint8_t wrDAdr)	
{
	I2CSendByte(wrDAdr);			//���� ����д��ַ
	I2CWaitAck();
	if( I2CChkAck()!=I2C_MT_SLA_ACK )
		return I2C_ERR;
	return I2C_CRR;
}
/*--------------------------------------------------------------------
�������ƣ����� 10λ ����д��ַ: 1111 0XX0, XXXX XXXX
�������ܣ�
ע�����
��ʾ˵�������� ���� 7λ ����д��ַ: XXXX XXX0
��    �룺
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_SendWrDAdr_(uint16_t wrDAdr)	
{
	if( (wrDAdr&0xF000) == 0xF000 )	//�ж��Ƿ�Ϊ 10λ ������ַ
		if( I2C_SendWrDAdr( (uint8_t)wrDAdr>>8 )==I2C_ERR )	//���� ����λ������д��ַ
			return I2C_ERR;
	if( I2C_SendWrDAdr( (uint8_t)wrDAdr )==I2C_ERR )	//���� ����λ������д��ַ
		return I2C_ERR;
	return I2C_CRR;
}
/*--------------------------------------------------------------------
�������ƣ����� 7λ ��������ַ: XXXX XXX1
�������ܣ�
ע�����
��ʾ˵����
��    �룺
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_SendRdDAdr(uint8_t rdDAdr)	
{
	I2CSendByte(rdDAdr);			//���� ��������ַ
	I2CWaitAck();
	if( I2CChkAck()!=I2C_MR_SLA_ACK )  
		return I2C_ERR;
	return I2C_CRR;	
}
/*--------------------------------------------------------------------
�������ƣ����� 10λ ��������ַ: 1111 0XX0, XXXX XXXX
�������ܣ�
ע�����
��ʾ˵����
��    �룺
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_SendRdDAdr_(uint8_t rdDAdr)	
{
	if( (rdDAdr&0xF000) == 0xF000)	//�ж��Ƿ�Ϊ 10λ ������ַ
		if( I2C_SendWrDAdr( (uint8_t)rdDAdr>>8 )==I2C_ERR )	//���� ����λ����������ַ
			return I2C_ERR;
	if( I2C_SendWrDAdr( (uint8_t)rdDAdr )==I2C_ERR )	//���� ����λ����������ַ
		return I2C_ERR;
	return I2C_CRR;
}
/*--------------------------------------------------------------------
�������ƣ�I2C��������
�������ܣ�
ע�����
��ʾ˵����
��    �룺
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_SendDat(uint8_t configDat)	
{
	I2CSendByte(configDat);			
	I2CWaitAck();
	if( I2CChkAck()!=I2C_MT_DATA_ACK ) 
		return I2C_ERR;
	return I2C_CRR;	
}
/*--------------------------------------------------------------------
�������ƣ�I2C���������Ҳ�����Ӧ��
�������ܣ�
ע�����
��ʾ˵����
��    �룺
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_RcvNAckDat(uint8_t *pRdDat)	
{
	I2CRcvNckByte();
	I2CWaitAck();
	if( I2CChkAck()!=I2C_MR_DATA_NACK )
		return I2C_ERR;
	*pRdDat=TWDR;
	return I2C_CRR;
}
/*--------------------------------------------------------------------
�������ƣ�I2C���������Ҳ���Ӧ��
�������ܣ�
ע�����
��ʾ˵����
��    �룺
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_RcvAckDat(uint8_t *pRdDat)	
{
	I2CRcvAckByte();					
	I2CWaitAck();
	if( I2CChkAck()!=I2C_MR_DATA_ACK )
		return I2C_ERR;
	*pRdDat=TWDR;
	return I2C_CRR;
}
/*--------------------------------------------------------------------
�������ƣ�I2Cд������дһ������
�������ܣ�
ע�����
��ʾ˵����
��    �룺wrDAdr: write device-address д������ַ
		 wordAdr: word address �ֵ�ַ
		 dat: data ����
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_Write(uint16_t wrDAdr,uint8_t wordAdr,uint8_t dat)
{
	if( I2C_Start()==I2C_ERR )
		return I2C_ERR;

	if( I2C_SendWrDAdr_(wrDAdr)==I2C_ERR )
		return I2C_ERR;

	if( I2C_SendDat(wordAdr)==I2C_ERR )
		return I2C_ERR;

	if( I2C_SendDat(dat)==I2C_ERR )
		return I2C_ERR;

	I2C_Stop();

	return I2C_CRR;
}
/*--------------------------------------------------------------------
�������ƣ�I2Cд������дN������
�������ܣ�
ע�����
��ʾ˵����
��    �룺wrDAdr: write device-address д������ַ
		 wordAdr: word address �ֵ�ַ
		 *pWrDat: p->write data д������ָ��
		 num: number д�����ݸ���
��    �أ�
--------------------------------------------------------------------*/
//uint8_t I2C_Write_(uint16 wrDAdr,uint8 wordAdr,
//				  uint8 *pWrDat,uint8 num)
//{
//
//} 
/*--------------------------------------------------------------------
�������ƣ�I2C����������һ������
�������ܣ�
ע�����
��ʾ˵����
��    �룺wrDAdr: write device-address д������ַ
		 wordAdr: word address �ֵ�ַ
		 rdDAdr: read device-address ��������ַ
		 *pRdDat: p->read data ��ȡ����ָ��
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_Read(uint16_t wrDAdr,uint8_t wordAdr,
			  uint8_t rdDAdr,uint8_t *pRdDat)
{
	if( I2C_Start()==I2C_ERR )
		return I2C_ERR;

	if( I2C_SendWrDAdr_(wrDAdr)==I2C_ERR )
		return I2C_ERR;

	if( I2C_SendDat(wordAdr)==I2C_ERR )
		return I2C_ERR;

	if( I2C_Restart()==I2C_ERR )
		return I2C_ERR;

	if( I2C_SendRdDAdr(rdDAdr)==I2C_ERR )
		return I2C_ERR;

	if( I2C_RcvNAckDat(pRdDat)==I2C_ERR )
		return I2C_ERR;

	I2C_Stop();

	return I2C_CRR;
}
/*--------------------------------------------------------------------
�������ƣ�I2C����������N������
�������ܣ�
ע�����
��ʾ˵����
��    �룺wrDAdr: write device-address д������ַ
		 wordAdr: word address �ֵ�ַ
		 rdDAdr: read device-address ��������ַ
		 *pRdDat: p->read data ��ȡ����ָ��
		 num: number ��ȡ���ݸ���
��    �أ�
--------------------------------------------------------------------*/
uint8_t I2C_Read_(uint16_t wrDAdr,uint8_t wordAdr,
			   uint8_t rdDAdr,uint8_t *pRdDat,uint8_t num)
{
 	uint8_t i;
	
	if( I2C_Start()==I2C_ERR )
		return I2C_ERR;

	if( I2C_SendWrDAdr_(wrDAdr)==I2C_ERR )
		return I2C_ERR;

	if( I2C_SendDat(wordAdr)==I2C_ERR )
		return I2C_ERR;

	if( I2C_Restart()==I2C_ERR )
		return I2C_ERR;

	if( I2C_SendRdDAdr(rdDAdr)==I2C_ERR )
		return I2C_ERR;

	for(i=0;i<num-1;i++)
		if( I2C_RcvAckDat(pRdDat+i)==I2C_ERR )
			return I2C_ERR;
	
	if( I2C_RcvNAckDat(pRdDat+i)==I2C_ERR )
			return I2C_ERR;

	I2C_Stop();
	
	return I2C_CRR;
}
