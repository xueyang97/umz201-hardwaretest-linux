/*
 * @Author: chuen.hce
 * @Date: 2020-07-23 14:40:22
 * @LastEditTime: 2020-07-28 22:04:30
 * @LastEditors: Please set LastEditors
 * @Description: bt hal interface
 * @FilePath: dtiot_hal_bt.h
 */ 


#ifndef __DT_HAL_FOURTH_GENERATION__
#define __DT_HAL_FOURTH_GENERATION__


#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include <stdint.h>
#define PBM_NUMBER_MAX_LENGTH_V01 40
#define PBM_NAME_MAX_LENGTH_V01 241
#define PBM_MAX_NUM_BASIC_RECORD_INSTANCE_V01 10

typedef enum  {
	DTIOT_E_4G_NORMAL_MODE                    = 0x00,
	DTIOT_E_4G_AIRPLANE_MODE,                 
	DTIOT_E_4G_ERROR_MODE                  
}DTIOT_HAL_DEVICE_MODE_e;
typedef enum {
	RADIO_IF_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
    RADIO_IF_NO_SVC_V01 = 0x00, /**<  None (no service) \n  */
    RADIO_IF_CDMA_1X_V01 = 0x01, /**<  cdma2000\textsuperscript{\textregistered} 1X \n  */
    RADIO_IF_CDMA_1XEVDO_V01 = 0x02, /**<  cdma2000\textsuperscript{\textregistered} HRPD (1xEV-DO) \n  */
    RADIO_IF_AMPS_V01 = 0x03, /**<  AMPS \n  */
    RADIO_IF_GSM_V01 = 0x04, /**<  GSM \n  */
    RADIO_IF_UMTS_V01 = 0x05, /**<  UMTS \n  */
    RADIO_IF_WLAN_V01 = 0x06, /**<  WLAN \n  */
    RADIO_IF_GPS_V01 = 0x07, /**<  GPS \n  */
    RADIO_IF_LTE_V01 = 0x08, /**<  LTE \n  */
    RADIO_IF_TDSCDMA_V01 = 0x09, /**<  TD-SCDMA \n  */
    RADIO_IF_NR5G_V01 = 0x0C, /**<  NR5G \n  */
    RADIO_IF_NO_CHANGE_V01 = -1, /**<  No change  */
    RADIO_IF_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}RADIO_TYPE_e;
typedef enum {
	PB_TYPE_ADN_V01,
	PB_TYPE_FDN_V01,
	PB_TYPE_MSISDN_V01,
	PB_TYPE_MBDN_V01,
	PB_TYPE_SDN_V01,
	PB_TYPE_BDN_V01,
	PB_TYPE_LND_V01,
	PB_TYPE_MBN_V01,
	PB_TYPE_GAS_V01,
	PB_TYPE_AAS_V01
}PB_TYPE_e;
typedef enum {
	NOT_REGISTERED,
	REGISTERED,
	NOT_REGISTERED_SEARCHING,
	REGISTRATION_DENIED,
	REGISTRATION_UNKNOWN,
}REGISTRATION_STATE_e;

typedef enum {
	UNKNOWN,
	ATTACHED,
	DETACHED,
}ATTACH_STATE_e;

typedef struct CellInfo_t{
	int lac;
	int cid;
	int rsrp;
	int earfcn;
	char plmn[5];
}CellInfo;

typedef struct ServiceInfo_t{
	REGISTRATION_STATE_e registration_state;
	RADIO_TYPE_e radio_type;
	int lac;
	int cellid;
	int mcc;
	int mnc;
	ATTACH_STATE_e cs_state;
	ATTACH_STATE_e ps_state;
}ServiceInfo;

typedef struct SignalInfo_t{
	int rsrp;
}SignalInfo;

typedef struct PbmRecord_t{
	int record_id;
	char number[PBM_NUMBER_MAX_LENGTH_V01];
	char name[PBM_NAME_MAX_LENGTH_V01];
}PbmRecord;
typedef struct PbmRecordInstances_t{
	int record_num;
	PbmRecord record[PBM_MAX_NUM_BASIC_RECORD_INSTANCE_V01];
}PbmRecordInstances;

typedef struct CallBack_t
{
	void (*read_callback)(PbmRecordInstances instances);
	void (*write_callback)(int status,int record);
}PBCallBack;
int dtiot_hal_4g_init(PBCallBack *callBack);
int dtiot_hal_4g_deinit();
int dtiot_hal_4g_status_get(DTIOT_HAL_DEVICE_MODE_e *mode);
int dtiot_hal_4g_status_switch(DTIOT_HAL_DEVICE_MODE_e mode);
int dtiot_hal_4g_apn_get(char* apn);
int dtiot_hal_4g_apn_set(char* apn);
int dtiot_hal_4g_cell_get(CellInfo *cellinfo);
int dtiot_hal_4g_service_get(ServiceInfo *serviceinfo);
int dtiot_hal_4g_iccid_get(char* iccid);
int dtiot_hal_4g_signal_get(SignalInfo *signalinfo);
int dtiot_hal_4g_cpb_get(PB_TYPE_e pb_type);
int dtiot_hal_4g_cpb_set(PB_TYPE_e pb_type,PbmRecord record);
int dtiot_hal_4g_cpb_del(PB_TYPE_e pb_type,int record_id);
int dtiot_hal_4g_imsi_get(char* imsi);
int dtiot_hal_4g_imei_get(char* imei);

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
}
#endif
#endif //__DT_HAL_FOURTH_GENERATION__

