/*
 * =============================================================================
 *
 *       Filename:  device_fresh_air.c
 *
 *    Description:  新风设备 
 *
 *        Version:  1.0
 *        Created:  2018-05-09 08:46:55
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
/* ---------------------------------------------------------------------------*
 *                      include head files
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "device_fresh_air.h"
#include "sql_handle.h"

/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#define MAX_VALUE_LENG 32
enum {
	ATTR_ERROR,
	ATTR_SWICH,
	ATTR_SPEED,
};

/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static int getAttrCb(DeviceStr *dev, const char *attr_set[])
{
    printf("get attr, devid:%s, attribute name:\n", dev->id);
    unsigned int i = 0;
    while (attr_set[i++]) {
        printf("attr_%d: %s\n", i - 1, attr_set[i - 1]);
    }
	for (i=0; dev->type_para->attr[i].name != NULL; i++) {
		if (strcmp(attr_set[0],dev->type_para->attr[i].name) == 0) {
			const char *attr_name[2] = {NULL};
			const char *attr_value[2] = {NULL};
			attr_name[0] = dev->type_para->attr[i].name;
			attr_value[0] = dev->value[i];
			// printf("[%s]--->%s\n", attr_name[0],attr_value[0]);
			aliSdkSubDevReportAttrs(dev->type_para->proto_type,
					dev->id, attr_name,attr_value);
		}
	}


    return 0;
}


static int setAttrCb(DeviceStr *dev, const char *attr_name, const char *attr_value)
{
    unsigned int i = 0;
	for (i=0; dev->type_para->attr[i].name != NULL; i++) {
		if (strcmp(attr_name,dev->type_para->attr[i].name) == 0) {
			sprintf(dev->value[i],"%s",attr_value);
			printf("[%s,%s]%s:%s\n",__FUNCTION__,__FILE__,attr_name,attr_value);
			if (dev->type_para->attr[i].attrcb)
				dev->type_para->attr[i].attrcb(dev,dev->value[i]);
			break;
		}
	}

    return 0;
}

static int execCmdCb(DeviceStr *dev, const char *cmd_name, const char *cmd_args)
{
    printf("exec cmd, devid:%s, cmd_name:%s, cmd_args:%s\n",
           dev->id, cmd_name, cmd_args);
    return 0;
}

static int removeDeviceCb(DeviceStr **device)
{
	DeviceStr *dev = *device;
    printf("remove device, devid:%s\n",dev->id);
	int i;
	for (i=0; dev->type_para->attr[i].name != NULL; i++) {
		if (dev->value[i]) {
			free(dev->value[i]);
		}
		dev->value[i] = NULL;
	}
	sqlDeleteDevice(dev->id);
	free(dev);
	*device = NULL;
    return 0;
}

static void cmdSwich(DeviceStr *dev,char *value)
{
	int value_int = atoi(value);
	sprintf(dev->value[ATTR_SWICH],"%s",value);
	printf("[%s]value:%s,int:%d,buf:%s,speed:%s\n",
			__FUNCTION__,
			value,
			value_int,
			dev->value[ATTR_SWICH],
			dev->value[ATTR_SPEED] );
	if (value_int) {
		uint8_t speed = atoi(dev->value[ATTR_SPEED]);
		if (speed)// app调节范围为2-4,实际新风调节范围为1-3,所以要-1
			speed -= 1;
		printf("%s:%d\n", __FUNCTION__,speed);
		smarthomeFreshAirCmdCtrOpen(dev,speed);
	} else
		smarthomeFreshAirCmdCtrClose(dev);
}

static void cmdWindSpeed(DeviceStr *dev,char *value)
{
	int value_int = atoi(value);
	sprintf(dev->value[ATTR_SPEED],"%s",value);
	if (value_int) {
		uint8_t speed = atoi(dev->value[ATTR_SPEED]);
		if (speed)// app调节范围为2-4,实际新风调节范围为1-3,所以要-1
			speed -=1;
		smarthomeFreshAirCmdCtrOpen(dev,speed);
	}
}

static void cmdGetSwichStatus(DeviceStr *dev)
{
	// smarthomeAllDeviceCmdGetSwichStatus(dev,1);
}

static void reportPowerOnCb(DeviceStr *dev,char *param)
{
	// 固定为开
	sprintf(dev->value[ATTR_SWICH],"1");
	// app调节范围为2-4,实际新风调节范围为1-3,所以要+1
	sprintf(dev->value[ATTR_SPEED],"%d",param[0] + 1); 
	const char *attr_name[] = {
		dev->type_para->attr[ATTR_SWICH].name,
		dev->type_para->attr[ATTR_SPEED].name,
		NULL};
	const char *attr_value[] = {
		dev->value[ATTR_SWICH],
		dev->value[ATTR_SPEED],
		NULL};
	aliSdkSubDevReportAttrs(dev->type_para->proto_type,
			dev->id, attr_name,attr_value);
}

static void reportPowerOffCb(DeviceStr *dev)
{
	sprintf(dev->value[ATTR_SWICH],"0");
	const char *attr_name[] = {
		dev->type_para->attr[ATTR_SWICH].name,
		NULL};
	const char *attr_value[] = {
		dev->value[ATTR_SWICH],
		NULL};
	aliSdkSubDevReportAttrs(dev->type_para->proto_type,
			dev->id, attr_name,attr_value);
}

static DeviceTypePara fresh_air = {
	.name = "fresh_air",
	.short_model = 0x002824cd,
	.secret = "BCCcnkxFXVdi65csHXxJMfiSIcyjSQZCQHoIXdN7",
	.proto_type = ALI_SDK_PROTO_TYPE_ZIGBEE,
	.device_type = DEVICE_TYPE_XFXT,
	.attr = {
		{"ErrorCode",NULL},
		{"Switch",cmdSwich},
		{"WindSpeed",cmdWindSpeed},
		{"CurrentTemperature",NULL},
		{"CurrentHumidity",NULL},
		{"TVOC",NULL},
		{"PM25",NULL},
		{NULL,NULL},
	},
	.getAttr = getAttrCb,
	.setAttr = setAttrCb,
	.execCmd = execCmdCb,
	.remove = removeDeviceCb,
	.getSwichStatus = cmdGetSwichStatus,
	.reportPowerOn = reportPowerOnCb,
	.reportPowerOff = reportPowerOffCb,
};


DeviceStr * registDeviceFreshAir(char *id,uint16_t addr,uint16_t channel)
{
	int i;
	DeviceStr *This = (DeviceStr *)calloc(1,sizeof(DeviceStr));
	strcpy(This->id,id);
	memset(This->value,0,sizeof(This->value));
	This->type_para = &fresh_air;
	This->addr = addr;
	This->channel = channel;
	printf("[%s]addr:%x,channel:%d\n",__FUNCTION__,This->addr,This->channel );
	// 初始化属性
	for (i=0; This->type_para->attr[i].name != NULL; i++) {
		This->value[i] = (char *)calloc(1,MAX_VALUE_LENG);
		sprintf(This->value[i],"%s","0");
	}	

	return This;
}
