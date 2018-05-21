#ifndef __SMART_HOME_PRO_H__
#define __SMART_HOME_PRO_H__
#include "zigbee.h"

#define MAX_REGIST_DEVICE 70  // ���֧�ֵ��豸

#define CONTROL_MACHINE_ARRD		0
#define DEVICE_HW_VER						10		//Ӳ���汾
#define DEVICE_SW_VER						10		//�����汾					


enum 
{
	SHP_ST_IDIE,//������
	SHP_ST_WAIT,//�ȴ��ظ�
};

typedef enum
{
	DEVICE_TYPE_ZK = 			0,		// �����豸
	DEVICE_TYPE_DK = 			6,		// �ƿؿ���
	DEVICE_TYPE_TG,				// 7	���⿪��
	DEVICE_TYPE_KT,				// 8	�յ�����
	DEVICE_TYPE_CZ,				// 9	����
	DEVICE_TYPE_CL,				// 10	����
	DEVICE_TYPE_JJ,				// 11	������ť
	DEVICE_TYPE_HW,				// 12	���ⱨ��
	DEVICE_TYPE_YW,				// 13	��������
	DEVICE_TYPE_WS,				// 14	��˹����
	DEVICE_TYPE_MC,				// 15	�Ŵű���
	DEVICE_TYPE_CC,				// 16	���ű���
	DEVICE_TYPE_TY,				// 17	ͨ�ñ���
	DEVICE_TYPE_DS,				// 18	���ӿ�����
	DEVICE_TYPE_WXHW,			// 19	���ߺ���
	DEVICE_TYPE_QJ,				// 20	�龰������
	DEVICE_TYPE_DHX,			// 21	�����ߵƿ�
	DEVICE_TYPE_LED,			// 22	LED������
	DEVICE_TYPE_ZYKT,			// 23	����յ�
	DEVICE_TYPE_BJYY,			// 24	��������
	DEVICE_TYPE_XFXT,			// 25	�·�ϵͳ
	DEVICE_TYPE_FS,		  		// 26	����
	DEVICE_TYPE_SGBJQ,		 	// 27	���ⱨ����
}TC_Device_Type;

typedef enum
{
	NetIn_Report					= 0xf0,			//�����ϱ�
	NetIn_Report_Res				= 0xf1,			//�����ϱ�����
	NetOut_Report					= 0xf2,			//����֪ͨ
	NetOut_Report_Res				= 0xf3,			//����֪ͨ����
	NetIn_Apply						= 0xf4,			//��������
	Report_Status					= 0xf5,			//�ϱ�״̬
	Device_Syn						= 0xfa,			//�豸ͬ��
	Device_Syn_Res					= 0xfb,			//�豸ͬ������
	Demand_Sw_Status				= 0,			//��ѯ����״̬
	Demand_Sw_Status_Res			= 1,			//��ѯ����״̬����
	Demand_Device_Type				= 0x04,			//��ѯ�豸����
	Demand_Device_Type_Res			= 0x05,			//��ѯ�豸���ͷ���
	Demand_Device_Ver				= 0x06,			//��ѯ�豸�̼��汾��
	Demand_Device_Ver_Res			= 0x07,			//��ѯ�豸�̼��汾�ŷ���
	Demand_Time						= 0x08,			//��ѯʱ��
	Demand_Time_Res					= 0x09,			//��ѯʱ�䷵��
	Device_On						= 0x90,			//������Ԫ
	Device_On_Res					= 0x91,			//������Ԫ����
	Device_Off						= 0x92,			//�رյ�Ԫ
	Device_Off_Res					= 0x93,			//�رյ�Ԫ����
	
	Device_Scene		 			= 0xb0,
	Device_Scene_Res 				= 0xb1,
	Cmd_Null						= 0xff,			//������
}TC_CMD;


typedef struct 
{
	uint16_t zAddr;
	uint8_t channel;
	uint8_t id;
}zbDev_TypeDef;	//zigbee�ܱ��豸������Ϣ

typedef struct
{
	uint16_t addr;
	uint8_t  device_type;
	uint8_t  channel_num;	 //ͨ������
	uint8_t  current_channel;//	��ǰͨ��
	uint8_t  cmd;
	uint8_t  param[8];
}SMART_HOME_PRO;	//����Э��

typedef struct 
{
	uint16_t zAddr;		//�ܱ��豸��ַ
	uint8_t channel;	//��ǰͨ��
	uint8_t cmd;		//����ָ��
	uint16_t param;		//����
}MULTI_CTRL_TypeDef;	//Ⱥ������Э�鵥���豸�ṹ
#define MULTI_PACKET_DEV_MAX	((ZIGBEE_UART_MAX-8)/(sizeof(MULTI_CTRL_TypeDef)))	//Ⱥ��ÿ�������豸����

typedef struct 
{
	uint8_t type[3];		//Э������
	MULTI_CTRL_TypeDef dev[MULTI_PACKET_DEV_MAX];
}multiPacket_TypeDef;	//Ⱥ������Э��������ݽṹ



#define SEND_QUEUE_NUM	8

void SHP_Timer(void);

// void SHP_CtrlDevice(uint32_t id,uint32_t param,uint32_t TAG,ipAddr_TypeDef *dAddr,ipAddr_TypeDef *tAddr);

// void SHP_CtrlMulti(multi_log_TypeDef *multi, uint8_t devNum);
// void SHP_Parser(uint8_t *buf, uint8_t len);

// void SHP_ZbSending(void);
// void SHP_SendCmdFromQueue(void);

// uint32_t SHP_DelDev(uint32_t devUnit);

#endif