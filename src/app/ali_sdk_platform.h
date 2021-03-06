/*
 * =============================================================================
 *
 *       Filename:  ali_paltform.h
 *
 *    Description:  阿里平台通用接口，兼容1.0,2.0版本
 *
 *        Version:  1.0
 *        Created:  2018-06-14 11:57:19 
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
#ifndef _ALI_SDK_PLATFORM_H
#define _ALI_SDK_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#if (defined  V1)
#include "alink_export_gateway.h"
#include "alink_export_subdev.h"
#include "platform.h"	
#define ALI_SDK_PROTO_TYPE_ZIGBEE PROTO_TYPE_ZIGBEE

extern char *calc_subdev_signature(const char *secret,
		const uint8_t rand[SUBDEV_RAND_BYTES],
        char *sign_buff,
	   	uint32_t buff_size);
#elif (defined V2)
#include "linkkit.h"
#include "iot_import.h"
#include "cJSON.h"
#define ALI_SDK_PROTO_TYPE_ZIGBEE 2
#endif
struct _DeviceStr;
struct _GateWayPrivateAttr;
struct _GateWayAttr;
	extern void aliSdkInit(int argc,char *argv[]);
	extern void aliSdkStart(void);
	extern void aliSdkEnd(void);
	extern int aliSdkReset(int is_reboot);
	extern int aliSdkRegisterSubDevice(struct _DeviceStr *dev);
	extern int aliSdkRegistGwService(char *name, void *func);
	extern int aliSdkRegisterAttribute(struct _GateWayPrivateAttr *attrs);
	extern int aliSdkRegisterGw(char *value);
	extern void aliSdkRegistGwAttr(char *proto_name,int proto_type,struct _GateWayAttr *attr);
	extern void aliSdkSubDevReportAttrs(int protocol,char *id,const char *attr_name[],const char *attr_value[]);

	extern void aliSdkresetWifi(void);
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
