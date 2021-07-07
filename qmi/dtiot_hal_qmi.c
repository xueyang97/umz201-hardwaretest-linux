#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <qmi.h>
#include <string.h>
#include <cutils/properties.h>
#include "common_log.h"

#include "qmi_client.h"
#include "qmi_client_instance_defs.h"
#include "qmi_cci_target_ext.h"
#include "qmi_idl_lib.h"
#include "access_terminal_service_v01.h"
#include "device_management_service_v01.h"
#include "network_access_service_v01.h"
#include "data_system_determination_v01.h"
#include "phonebook_manager_service_v01.h"
#include "wireless_data_service_v01.h"
#include "user_identity_module_v01.h"

#include "dtiot_hal_qmi.h"

static qmi_idl_service_object_type nas_obj;
static qmi_client_type nas_type;
static qmi_client_os_params nas_par;

static qmi_idl_service_object_type dms_obj;
static qmi_client_type dms_type;
static qmi_client_os_params dms_par;

static qmi_idl_service_object_type dsd_obj;
static qmi_client_type dsd_type;
static qmi_client_os_params dsd_par;

static qmi_idl_service_object_type pbm_obj;
static qmi_client_type pbm_type;
static qmi_client_os_params pbm_par;

static qmi_idl_service_object_type wds_obj;
static qmi_client_type wds_type;
static qmi_client_os_params wds_par;

static qmi_idl_service_object_type uim_obj;
static qmi_client_type uim_type;
static qmi_client_os_params uim_par;


#define TIMEOUT 4000
#define TRUE 1
#define FALSE 0
#define ONLINE_MODE 0x00
#define LPM_MODE 0x01
#define PBM_MSG_DEFAULT_TIMEOUT 5000

static PBCallBack *pbcallBack;

//#define LOGI(...) fprintf(stderr, "I:" __VA_ARGS__)


static void qmi_util_unsolicited_nas_ind_handler ( qmi_client_type     user_handle,
                                     unsigned long      msg_id,
                                     unsigned char      *ind_buf,
                                     int                ind_buf_len,
                                     void               *ind_cb_data
								  )
{
//TODO:
}
static void qmi_util_unsolicited_dms_ind_handler ( qmi_client_type     user_handle,
                                     unsigned long      msg_id,
                                     unsigned char      *ind_buf,
                                     int                ind_buf_len,
                                     void               *ind_cb_data
								  )
{
//TODO:
}

static void qmi_util_unsolicited_dsd_ind_handler ( qmi_client_type     user_handle,
                                     unsigned long      msg_id,
                                     unsigned char      *ind_buf,
                                     int                ind_buf_len,
                                     void               *ind_cb_data
								  )
{
//TODO:
}

static void qmi_util_unsolicited_wds_ind_handler ( qmi_client_type     user_handle,
                                     unsigned long      msg_id,
                                     unsigned char      *ind_buf,
                                     int                ind_buf_len,
                                     void               *ind_cb_data
								  )
{
//TODO:
}

static void qmi_util_unsolicited_uim_ind_handler ( qmi_client_type     user_handle,
                                     unsigned long      msg_id,
                                     unsigned char      *ind_buf,
                                     int                ind_buf_len,
                                     void               *ind_cb_data
								  )
{
//TODO:
}


static int getWideStringLength(char* wide_string)
{
    int i = 0;
    char first, second;

    if (wide_string == NULL)
    {
        return 0;
    }

    for (i = 0; ((wide_string[i] != '\0') || (wide_string[i+1] != '\0')); i+=2)
    {}

    return i/2;
}

static void convertToNarrowString(char* wide_string, char* narrow_string)
{
    int i, len;

    if (wide_string == NULL)
    {
        narrow_string[0] = '\0';
        return;
    }

    len = getWideStringLength(wide_string);

    for (i = 0; i < len; i++)
    {
         // Copy over the first (MSB) octet of the code point
        narrow_string[i] = wide_string[2*i];
    }
    narrow_string[len] = '\0';
}
static void convertToWideString(char* narrow_string, char* wide_string)
{
    int i, len;

    if (narrow_string == NULL)
    {
        wide_string[0] = '\0';
        wide_string[1] = '\0';
        return;
    }

    len = strlen(narrow_string);
    for (i = 0; i < len; i++)
    {
        wide_string[2*i] = narrow_string[i];
        wide_string[2*i+1] = '\0';
    }
    wide_string[2*len] = '\0';
    wide_string[2*len + 1] = '\0';
}
static void stripLineFeed(char* str)
{
    int i;

    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
        }
    }
}

static void handle_read_indication(void* payload, int payload_len)
{

    pbm_record_read_ind_msg_v01 pbm_read_ind;
    uint i = 0;
	PbmRecordInstances instances;
	instances.record_num = 0;
    memset(&pbm_read_ind, 0, sizeof(pbm_read_ind));
    {
        memcpy(&pbm_read_ind, payload, payload_len);
        printf("Number of record instances in the read indication: %d\n", pbm_read_ind.basic_record_data.record_instances_len);
		instances.record_num = pbm_read_ind.basic_record_data.record_instances_len;
        for (i = 0; i < pbm_read_ind.basic_record_data.record_instances_len && i< PBM_MAX_NUM_BASIC_RECORD_INSTANCE_V01; i++)
        {
            printf("\n\nIndex of record in: %d\n", pbm_read_ind.basic_record_data.record_instances[i].record_id);
            printf("Name in the record: %s\n", pbm_read_ind.basic_record_data.record_instances[i].name);
            printf("Number in the record: %s\n", pbm_read_ind.basic_record_data.record_instances[i].number);
            printf("Number type in the record: %d\n", (int)pbm_read_ind.basic_record_data.record_instances[i].num_type);
            printf("Number plan in the record: %d\n", (int)pbm_read_ind.basic_record_data.record_instances[i].num_plan);
			strcpy(instances.record[i].name,pbm_read_ind.basic_record_data.record_instances[i].name);
			strcpy(instances.record[i].number,pbm_read_ind.basic_record_data.record_instances[i].number);
			instances.record[i].record_id = pbm_read_ind.basic_record_data.record_instances[i].record_id;
        }
    }
	if(pbcallBack && pbcallBack->read_callback)
		pbcallBack->read_callback(instances);
}

static void qmi_util_unsolicited_pbm_ind_handler ( qmi_client_type     user_handle,
                                     unsigned long      msg_id,
                                     unsigned char      *ind_buf,
                                     int                ind_buf_len,
                                     void               *ind_cb_data
								  )
{
	uint decoded_payload_len = 0;
    qmi_client_error_type rc;
    void* decoded_payload;
	qmi_idl_get_message_c_struct_len(pbm_obj,
                                QMI_IDL_INDICATION,
                                msg_id,
                                &decoded_payload_len);

    decoded_payload = malloc(decoded_payload_len);
    if (decoded_payload == NULL)
    {
        printf("Error: Unable to allocate memory\n");
    }
    else
    {
        rc = qmi_client_message_decode(pbm_type,
                                  QMI_IDL_INDICATION,
                                  msg_id,
                                  ind_buf,
                                  ind_buf_len,
                                  decoded_payload,
                                  decoded_payload_len);

        if (rc != QMI_NO_ERR)
        {
            printf("Error: Decoding unsolicited indication with id = %ld, returned in error = %d\n", msg_id, (int)rc);
        }
        else
        {
            if (msg_id == QMI_PBM_RECORD_READ_IND_V01)
            {
                handle_read_indication(decoded_payload, decoded_payload_len);
            }
            else
            {
                /* ignore all other indications */
            }
        }
        free(decoded_payload);
    }
}


static int qmi_ril_set_modem_mode(int mode)
{
	dms_set_operating_mode_req_msg_v01 request_data;
	dms_set_operating_mode_resp_msg_v01 rsp_data;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	request_data.operating_mode = mode;
	qmi_client_error_type ret;
	ret = qmi_client_send_msg_sync(dms_type,
                                     QMI_DMS_SET_OPERATING_MODE_REQ_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		printf("\n qmi_ril_set_modem_mode success\n");
		return 0;
	}else
	{
		printf("\n qmi_ril_set_modem_mode error\n");
		return -1;
	}
}

static int qmi_ril_get_modem_mode(int *mode)
{
	dms_get_operating_mode_resp_msg_v01 rsp_data;
	memset(&rsp_data,0,sizeof(rsp_data));
	qmi_client_error_type ret;
	ret = qmi_client_send_msg_sync(dms_type,
                                     QMI_DMS_GET_OPERATING_MODE_REQ_V01,
                                     NULL,
                                     0,
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		printf("qmi_ril_get_modem_mode success\n");
		*mode = rsp_data.operating_mode;
		return 0;
	}else
	{
		printf("qmi_ril_get_modem_mode error\n");
		return -1;
	}
}

static int qmi_ril_get_cell_location_info(CellInfo *cellinfo)
{
	nas_get_cell_location_info_req_msg_v01 request_data;
	nas_get_cell_location_info_resp_msg_v01 rsp_data;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	qmi_client_error_type ret;
	ret = qmi_client_send_msg_sync(nas_type,
                                     QMI_NAS_GET_CELL_LOCATION_INFO_REQ_MSG_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		printf("qmi_ril_get_cell_location_info success\n");
		if(rsp_data.geran_info_valid)
		{
			cellinfo->lac = rsp_data.geran_info.lac;
			cellinfo->cid = rsp_data.geran_info.cell_id;
			cellinfo->earfcn = rsp_data.geran_info.arfcn;
			cellinfo->rsrp = rsp_data.geran_info.rx_lev;
			cellinfo->plmn[0] = rsp_data.geran_info.plmn[0]&0x0f;
			cellinfo->plmn[1] = (rsp_data.geran_info.plmn[0]>>4)&0x0f;
			cellinfo->plmn[2] = rsp_data.geran_info.plmn[1]&0x0f;
			cellinfo->plmn[3] = rsp_data.geran_info.plmn[2]&0x0f;
			cellinfo->plmn[4] = (rsp_data.geran_info.plmn[2]>>4)&0x0f;
			return 0;
		}else if(rsp_data.umts_info_valid)
		{
			cellinfo->lac = rsp_data.umts_info.lac;
			cellinfo->cid = rsp_data.umts_info.rscp;
			cellinfo->earfcn = rsp_data.umts_info.uarfcn;
			cellinfo->rsrp = rsp_data.umts_info.rscp;
			cellinfo->plmn[0] = rsp_data.umts_info.plmn[0]&0x0f;
			cellinfo->plmn[1] = (rsp_data.umts_info.plmn[0]>>4)&0x0f;
			cellinfo->plmn[2] = rsp_data.umts_info.plmn[1]&0x0f;
			cellinfo->plmn[3] = rsp_data.umts_info.plmn[2]&0x0f;
			cellinfo->plmn[4] = (rsp_data.umts_info.plmn[2]>>4)&0x0f;
			return 0;
		}
		else if(rsp_data.lte_intra_valid)
		{
			cellinfo->lac = rsp_data.lte_intra.tac;
			cellinfo->cid = rsp_data.lte_intra.serving_cell_id;
			cellinfo->earfcn = rsp_data.lte_intra.earfcn;
			cellinfo->plmn[0] = rsp_data.lte_intra.plmn[0]&0x0f;
			cellinfo->plmn[1] = (rsp_data.lte_intra.plmn[0]>>4)&0x0f;
			cellinfo->plmn[2] = rsp_data.lte_intra.plmn[1]&0x0f;
			cellinfo->plmn[3] = rsp_data.lte_intra.plmn[2]&0x0f;
			cellinfo->plmn[4] = (rsp_data.lte_intra.plmn[2]>>4)&0x0f;
			int i =0;
			for(i=0;i<rsp_data.lte_intra.cells_len;i++)
			 {
			 	if(rsp_data.lte_intra.cells[i].pci==rsp_data.lte_intra.serving_cell_id)
			 	{
					cellinfo->rsrp = rsp_data.lte_intra.cells[i].rsrp;
					return 0;	
			 	}
			}
		}
		return 0;
	}else
	{
		printf("qmi_ril_get_cell_location_info error\n");
		return -1;
	}
}


static int qmi_ril_get_service_info(ServiceInfo *serviceinfo)
{
	nas_get_serving_system_req_msg_v01 request_data;
	nas_get_serving_system_resp_msg_v01 rsp_data;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	qmi_client_error_type ret;
	ret = qmi_client_send_msg_sync(nas_type,
                                     QMI_NAS_GET_SERVING_SYSTEM_REQ_MSG_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		printf("\nqmi_ril_get_service_info success\n");
		serviceinfo->registration_state = rsp_data.serving_system.registration_state;
		serviceinfo->cs_state = rsp_data.serving_system.cs_attach_state;
		serviceinfo->ps_state = rsp_data.serving_system.ps_attach_state;
		if(rsp_data.cell_id_valid)
			serviceinfo->cellid = rsp_data.cell_id;
		if(rsp_data.lac_valid)
			serviceinfo->lac = rsp_data.lac;
		if(rsp_data.tac_valid)
			serviceinfo->lac = rsp_data.tac;
		if(rsp_data.current_plmn_valid)
		{
			serviceinfo->mcc = rsp_data.current_plmn.mobile_country_code;
			serviceinfo->mnc = rsp_data.current_plmn.mobile_network_code;
		}
		if(rsp_data.serving_system.radio_if_len > 0)
			serviceinfo->radio_type = rsp_data.serving_system.radio_if[0];
		return 0;
	}else
	{
		printf("\nqmi_ril_get_service_info error\n");
		return -1;
	}
}

static int qmi_ril_get_signal_info(SignalInfo *signalinfo)
{
	nas_get_signal_strength_req_msg_v01 request_data;
	nas_get_signal_strength_resp_msg_v01 rsp_data;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	qmi_client_error_type ret;
	request_data.request_mask_valid = true;
	request_data.request_mask = QMI_NAS_REQUEST_SIG_INFO_LTE_RSRP_MASK_V01;
	ret = qmi_client_send_msg_sync(nas_type,
                                     QMI_NAS_GET_SIGNAL_STRENGTH_REQ_MSG_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		printf("qmi_ril_get_signal_info success\n");
		if(rsp_data.lte_rsrp_valid)
		{
			signalinfo->rsrp = rsp_data.lte_rsrp;
			return 0;
		}
	}else
	{
		printf("qmi_ril_get_signal_info error\n");
		return -1;
	}
}

static int qmi_ril_set_apn(char* apn)
{
	wds_modify_profile_settings_req_msg_v01 request_data;
	wds_modify_profile_settings_resp_msg_v01 rsp_data;
	qmi_client_error_type ret;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	request_data.apn_name_valid = 1;
	memcpy(request_data.apn_name,apn,strlen(apn));
	request_data.profile.profile_index = 1;
	request_data.profile.profile_type = 0;
	//request_data.auth_password_valid = 1;
	//request_data.username_valid = 1;
	//request_data.user_id_valid = 1;
	printf("apn:%s\n",apn);
	
	ret = qmi_client_send_msg_sync(wds_type,
                                     QMI_WDS_MODIFY_PROFILE_SETTINGS_REQ_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		printf("\n qmi_ril_set_apn success\n");
		return 0;
	}else
	{
		printf("\n qmi_ril_set_apn error error code:%d\n",rsp_data.resp.error);
		return -1;
	}
}

static int qmi_ril_get_apn(char* apn)
{
	wds_get_profile_settings_req_msg_v01 request_data;
	wds_get_profile_settings_resp_msg_v01 rsp_data;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	request_data.profile.profile_index = 1;
	request_data.profile.profile_type = 0;
	qmi_client_error_type ret;
	ret = qmi_client_send_msg_sync(wds_type,
                                     QMI_WDS_GET_PROFILE_SETTINGS_REQ_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		printf("qmi_ril_get_apn success %s\n",rsp_data.apn_name);
		if(rsp_data.apn_name_valid)
		{
			memcpy(apn,rsp_data.apn_name,strlen(rsp_data.apn_name));
		}
		return 0;
	}else
	{
		printf("qmi_ril_get_apn error ret:%d, error code:%d\n",ret,rsp_data.resp.error);
		return -1;
	}
}
typedef struct{
	char imsi[32];
	char iccid[32];
}ubiot_ex_data;

static int qmi_ril_get_iccid(char* iccid)
{
	ubiot_ex_data ubiot_ex;
	dms_read_user_data_req_msg_v01 request_data;
	dms_read_user_data_resp_msg_v01 rsp_data;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	qmi_client_error_type ret;
	ret = qmi_client_send_msg_sync(dms_type,
                                     QMI_DMS_READ_USER_DATA_REQ_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		memcpy(&ubiot_ex,&rsp_data.user_data.data,sizeof(ubiot_ex));
		memcpy(iccid,ubiot_ex.iccid,strlen(ubiot_ex.iccid));
		printf("\n qmi_ril_get_iccid success %s\n",ubiot_ex.iccid);
		return 0;
	}else
	{
		printf("\n qmi_ril_get_iccid error ret:%d,error:%d\n",ret,rsp_data.resp.error);
		return -1;
	}
}

static int qmi_ril_get_imsi(char* imsi)
{
	ubiot_ex_data ubiot_ex;
	dms_read_user_data_req_msg_v01 request_data;
	dms_read_user_data_resp_msg_v01 rsp_data;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	qmi_client_error_type ret;
	int i = 0;
	ret = qmi_client_send_msg_sync(dms_type,
                                     QMI_DMS_READ_USER_DATA_REQ_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		memcpy(&ubiot_ex,&rsp_data.user_data.data,sizeof(ubiot_ex));
		memcpy(imsi,ubiot_ex.imsi,strlen(ubiot_ex.imsi));
		printf("\n qmi_ril_get_imsi success %s\n",ubiot_ex.imsi);
		return 0;
	}else
	{
		printf("\n qmi_ril_get_imsi error ret:%d,error:%d\n",ret,rsp_data.resp.error);
		return -1;
	}
}

static int qmi_ril_get_pb_info(pbm_get_pb_capabilities_req_msg_v01 *pbm_info_req,pbm_get_pb_capabilities_resp_msg_v01 *pbm_info_resp)
{
	qmi_client_error_type rc;
	rc = qmi_client_send_msg_sync(pbm_type,
                                  QMI_PBM_GET_PB_CAPABILITIES_REQ_V01,
                                  pbm_info_req,
                                  sizeof(*pbm_info_req),
                                  pbm_info_resp,
                                  sizeof(*pbm_info_resp),
                                  PBM_MSG_DEFAULT_TIMEOUT);

    if (rc != QMI_NO_ERR)
    {
        printf("Getting PB info for PB type %d returned error %d\n", (int)(pbm_info_req->phonebook_info.pb_type), (int)rc);
    }
    return rc;
}

static int qmi_ril_read_pb_info(PB_TYPE_e pb_type)
{
	pbm_read_records_req_msg_v01 pbm_read_req;
    pbm_read_records_resp_msg_v01 pbm_read_resp;
    pbm_get_pb_capabilities_req_msg_v01 pbm_info_req;
    pbm_get_pb_capabilities_resp_msg_v01 pbm_info_resp;

    qmi_client_error_type rc;
    /* memset the request response structure to zero */
    memset(&pbm_info_req,0,sizeof(pbm_info_req));
    memset(&pbm_info_resp,0,sizeof(pbm_info_resp));

    pbm_info_req.phonebook_info.session_type = PBM_SESSION_TYPE_GLOBAL_PB_SLOT1_V01;
	switch(pb_type)
	{
		case PB_TYPE_ADN_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_ADN_V01;
			break;
		case PB_TYPE_FDN_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_FDN_V01;
			break;
		case PB_TYPE_MSISDN_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_MSISDN_V01;
			break;
		case PB_TYPE_MBDN_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_MBDN_V01;
			break;
		case PB_TYPE_SDN_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_SDN_V01;
			break;
		case PB_TYPE_BDN_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_BDN_V01;
			break;
		case PB_TYPE_LND_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_LND_V01;
			break;
		case PB_TYPE_MBN_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_MBN_V01;
			break;
		case PB_TYPE_GAS_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_GAS_V01;
			break;
		case PB_TYPE_AAS_V01:
			pbm_info_req.phonebook_info.pb_type = PBM_PB_TYPE_AAS_V01;
			break;
	}
	if (qmi_ril_get_pb_info(&pbm_info_req, &pbm_info_resp) == QMI_NO_ERR)
	{
		if (pbm_info_resp.capability_basic_info_valid)
		{
			memset(&pbm_read_req,0,sizeof(pbm_read_req));
            memset(&pbm_read_resp,0,sizeof(pbm_read_resp));
			
            pbm_read_req.record_info.pb_type = pbm_info_req.phonebook_info.pb_type;
            pbm_read_req.record_info.session_type = PBM_SESSION_TYPE_GLOBAL_PB_SLOT1_V01;
            pbm_read_req.record_info.record_start_id = 1;
            pbm_read_req.record_info.record_end_id = pbm_info_resp.capability_basic_info.max_records;

            rc = qmi_client_send_msg_sync(pbm_type,
                                          QMI_PBM_READ_RECORDS_REQ_V01,
                                          &pbm_read_req,
                                          sizeof(pbm_read_req),
                                          &pbm_read_resp,
                                          sizeof(pbm_read_resp),
                                          PBM_MSG_DEFAULT_TIMEOUT);
            if (rc != QMI_NO_ERR)
            {
                printf("Read request for ADN failed with error code = %d\n", (int)rc);
                return -1;
            }
            else
            {
                if ((pbm_read_resp.resp.result == QMI_RESULT_SUCCESS_V01) && (pbm_read_resp.num_of_recs_valid))
                {
                    if (pbm_read_resp.num_of_recs == 0)
                    {
                        printf("There are no entries filled in ADN, nothing to read\n");
                    }
                    else
                    {
                        printf("There are %d filled entries in ADN\n", pbm_read_resp.num_of_recs);
                    }
                }
            }
		}else
		{
			printf(" Error: Information received for ADN did not return total number of records\n");
            return -1;
		}
	}
	return 0;
}
static void pbm_write_cb ( qmi_client_type              user_handle,
                           unsigned long                msg_id,
                           void                         *resp_c_struct,
                           int                          resp_c_struct_len,
                           void                         *resp_cb_data,
                           qmi_client_error_type        transp_err
                          )
{
    pbm_write_record_resp_msg_v01* p_pbm_write_resp;

    if (transp_err !=  QMI_NO_ERR)
    {
        printf("Error: QMI PBM request for writing failed with error code %d\n", (int) transp_err);
		if(pbcallBack && pbcallBack->write_callback)
			pbcallBack->write_callback(transp_err,-1);
		return;
    }
    else
    {
        p_pbm_write_resp = (pbm_write_record_resp_msg_v01*) resp_c_struct;
        if (p_pbm_write_resp->resp.result == QMI_RESULT_SUCCESS_V01)
        {
            printf("Writing a record using PBM was successful\n");
            if (p_pbm_write_resp->record_id_valid)
            {
                printf("New record was added at index %d\n", p_pbm_write_resp->record_id);
				if(pbcallBack && pbcallBack->write_callback)
					pbcallBack->write_callback(QMI_NO_ERR,p_pbm_write_resp->record_id);
				return;
            }
			if(pbcallBack && pbcallBack->write_callback)
				pbcallBack->write_callback(QMI_NO_ERR,-1);
        }
        else
        {
            printf("Writing a record using PBM failed with error code %d\n", p_pbm_write_resp->resp.error);
        }
		if(pbcallBack && pbcallBack->write_callback)
			pbcallBack->write_callback(p_pbm_write_resp->resp.error,-1);
    }
}

static int qmi_ril_write_pb_info(PB_TYPE_e pb_type,PbmRecord record)
{
    qmi_client_error_type rc;
    pbm_write_record_req_msg_v01 pbm_write_req;
	pbm_write_record_resp_msg_v01 pbm_write_resp;
	qmi_txn_handle txn_handle;

    /* memset the request response structure to zero */
    memset(&pbm_write_req,0,sizeof(pbm_write_req));
    memset(&pbm_write_resp,0,sizeof(pbm_write_resp));
    stripLineFeed(record.name);
	stripLineFeed(record.number);
	strcpy(pbm_write_req.record_information.name,record.name);
	strcpy(pbm_write_req.record_information.number,record.number);
    pbm_write_req.record_information.number_len = strlen(pbm_write_req.record_information.number);
	pbm_write_req.record_information.name_len = strlen(pbm_write_req.record_information.name);
	printf("qmi_ril_write_pb_info name_len:%d,number_len:%d\n",pbm_write_req.record_information.name_len,pbm_write_req.record_information.number_len);
	
	switch(pb_type)
	{
		case PB_TYPE_ADN_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_ADN_V01;
			break;
		case PB_TYPE_FDN_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_FDN_V01;
			break;
		case PB_TYPE_MSISDN_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_MSISDN_V01;
			break;
		case PB_TYPE_MBDN_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_MBDN_V01;
			break;
		case PB_TYPE_SDN_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_SDN_V01;
			break;
		case PB_TYPE_BDN_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_BDN_V01;
			break;
		case PB_TYPE_LND_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_LND_V01;
			break;
		case PB_TYPE_MBN_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_MBN_V01;
			break;
		case PB_TYPE_GAS_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_GAS_V01;
			break;
		case PB_TYPE_AAS_V01:
			pbm_write_req.record_information.phonebook_type = PBM_PB_TYPE_AAS_V01;
			break;
	}
    pbm_write_req.record_information.session_type = PBM_SESSION_TYPE_GLOBAL_PB_SLOT1_V01;
	pbm_write_req.record_information.record_id = record.record_id;
    rc =  qmi_client_send_msg_async(pbm_type,
                                     QMI_PBM_WRITE_RECORD_REQ_V01,
                                     &pbm_write_req,
                                     sizeof(pbm_write_req),
                                     &pbm_write_resp,
                                     sizeof(pbm_write_resp),
                                     &pbm_write_cb,
                                     NULL,
                                     &txn_handle
                                     );
    if (rc != QMI_NO_ERR)
    {
        printf("Error: Sending request for writing record failed with error code %d\n", (int)rc);
        return -1;
    }
	return 0;
}

static int qmi_ril_delete_pb_info(PB_TYPE_e pb_type,int record_id)
{
	pbm_delete_record_req_msg_v01 request_data;
	pbm_delete_record_resp_msg_v01 rsp_data;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	qmi_client_error_type ret;
	switch(pb_type)
	{
		case PB_TYPE_ADN_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_ADN_V01;
			break;
		case PB_TYPE_FDN_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_FDN_V01;
			break;
		case PB_TYPE_MSISDN_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_MSISDN_V01;
			break;
		case PB_TYPE_MBDN_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_MBDN_V01;
			break;
		case PB_TYPE_SDN_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_SDN_V01;
			break;
		case PB_TYPE_BDN_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_BDN_V01;
			break;
		case PB_TYPE_LND_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_LND_V01;
			break;
		case PB_TYPE_MBN_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_MBN_V01;
			break;
		case PB_TYPE_GAS_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_GAS_V01;
			break;
		case PB_TYPE_AAS_V01:
			request_data.record_info.pb_type = PBM_PB_TYPE_AAS_V01;
			break;
	}
	request_data.record_info.session_type = PBM_SESSION_TYPE_GLOBAL_PB_SLOT1_V01;
	request_data.record_info.record_id = record_id;
	ret = qmi_client_send_msg_sync(pbm_type,
                                     QMI_PBM_DELETE_RECORD_REQ_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		printf("\n qmi_ril_delete_pb_info success\n");
		return 0;
	}else
	{
		printf("\n qmi_ril_delete_pb_info error, ret:%d, error:%d\n",ret,rsp_data.resp.error);
		return -1;
	}
}

static int qmi_ril_get_imei(char* imei)
{
	dms_get_device_serial_numbers_req_msg_v01 request_data;
	dms_get_device_serial_numbers_resp_msg_v01 rsp_data;
	memset(&request_data,0,sizeof(request_data));
	memset(&rsp_data,0,sizeof(rsp_data));
	qmi_client_error_type ret;
	ret = qmi_client_send_msg_sync(dms_type,
                                     QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_REQ_V01,
                                     &request_data,
                                     sizeof(request_data),
                                     &rsp_data,
                                     sizeof(rsp_data),
                                     500
                                     );
	if(ret == QMI_NO_ERR && rsp_data.resp.result == QMI_RESULT_SUCCESS_V01)
	{
		printf("qmi_ril_get_imei success\n");
		if(rsp_data.imei_valid)
			memcpy(imei,rsp_data.imei,strlen(rsp_data.imei));
		return 0;
	}else
	{
		printf("qmi_ril_get_imei error\n");
		return -1;
	}
}



int dtiot_hal_4g_init(PBCallBack *callBack)
{
	pbcallBack = callBack;
	qmi_client_error_type ret;
	nas_obj = nas_get_service_object_v01();
	ret = qmi_client_init_instance( nas_obj,
                      				 0xffff,
                                     qmi_util_unsolicited_nas_ind_handler,
                                     NULL,
                                     &nas_par,
                                     TIMEOUT,
                                     &nas_type);
	if(ret != QMI_NO_ERR)
	{
		printf("qmi_client_init_instance nas_obj fail ret:%d",ret);
		return -1;
	}
	dms_obj = dms_get_service_object_v01();
	ret = qmi_client_init_instance( dms_obj,
                      				 0xffff,
                                     qmi_util_unsolicited_dms_ind_handler,
                                     NULL,
                                     &dms_par,
                                     TIMEOUT,
                                     &dms_type);
	if(ret != QMI_NO_ERR)
	{
		printf("qmi_client_init_instance dms_obj fail ret:%d",ret);
		return -1;
	}

	dsd_obj = dsd_get_service_object_v01();
	ret = qmi_client_init_instance( dsd_obj,
                      				 0xffff,
                                     qmi_util_unsolicited_dsd_ind_handler,
                                     NULL,
                                     &dsd_par,
                                     TIMEOUT,
                                     &dsd_type);
	if(ret != QMI_NO_ERR)
	{
		printf("qmi_client_init_instance data_obj fail ret:%d",ret);
		return -1;
	}

	pbm_obj = pbm_get_service_object_v01();
	ret = qmi_client_init_instance( pbm_obj,
                      				 0xffff,
                                     qmi_util_unsolicited_pbm_ind_handler,
                                     NULL,
                                     &pbm_par,
                                     TIMEOUT,
                                     &pbm_type);
	if(ret != QMI_NO_ERR)
	{
		printf("qmi_client_init_instance data_obj fail ret:%d",ret);
		return -1;
	}

	wds_obj = wds_get_service_object_v01();
	ret = qmi_client_init_instance( wds_obj,
                      				 0xffff,
                                     qmi_util_unsolicited_wds_ind_handler,
                                     NULL,
                                     &wds_par,
                                     TIMEOUT,
                                     &wds_type);
	if(ret != QMI_NO_ERR)
	{
		printf("qmi_client_init_instance data_obj fail ret:%d",ret);
		return -1;
	}

	uim_obj = uim_get_service_object_v01();
	ret = qmi_client_init_instance( uim_obj,
                      				 0xffff,
                                     qmi_util_unsolicited_uim_ind_handler,
                                     NULL,
                                     &uim_par,
                                     TIMEOUT,
                                     &uim_type);
	if(ret != QMI_NO_ERR)
	{
		printf("qmi_client_init_instance data_obj fail ret:%d",ret);
		return -1;
	}
	return 0;
}

int dtiot_hal_4g_deinit()
{
	qmi_client_release(nas_type);
	qmi_client_release(dms_type);
	qmi_client_release(dsd_type);
	qmi_client_release(pbm_type);
	qmi_client_release(wds_type);
	qmi_client_release(uim_type);
	return 0;
}

int dtiot_hal_4g_status_get(DTIOT_HAL_DEVICE_MODE_e *mode)
{
	int modem_mode = -1;
	qmi_ril_get_modem_mode(&modem_mode);
	if(modem_mode == ONLINE_MODE)
	{
		*mode = DTIOT_E_4G_NORMAL_MODE;
	}else if(modem_mode == LPM_MODE)
	{
		*mode = DTIOT_E_4G_AIRPLANE_MODE;
	}else
	{
		*mode = DTIOT_E_4G_ERROR_MODE;
	}
	return 0;
}


int dtiot_hal_4g_status_switch(DTIOT_HAL_DEVICE_MODE_e mode)
{
	int modem_mode = -1;
	if(mode == DTIOT_E_4G_NORMAL_MODE)
	{
		modem_mode = ONLINE_MODE;
	}else
	{
		modem_mode = LPM_MODE;
	}
	return qmi_ril_set_modem_mode(modem_mode);
}

int dtiot_hal_4g_apn_get(char* apn)
{
	return qmi_ril_get_apn(apn);
}

int dtiot_hal_4g_apn_set(char* apn)
{
	return qmi_ril_set_apn(apn);
}

int dtiot_hal_4g_cell_get(CellInfo *cellinfo)
{
	return qmi_ril_get_cell_location_info(cellinfo);
}

int dtiot_hal_4g_service_get(ServiceInfo *serviceinfo)
{
	return qmi_ril_get_service_info(serviceinfo);
}

int dtiot_hal_4g_iccid_get(char* iccid)
{
	return qmi_ril_get_iccid(iccid);
}

int dtiot_hal_4g_signal_get(SignalInfo *signalinfo)
{
	return qmi_ril_get_signal_info(signalinfo);
}

int dtiot_hal_4g_cpb_get(PB_TYPE_e pb_type)
{
	return qmi_ril_read_pb_info(pb_type);
}
int dtiot_hal_4g_cpb_set(PB_TYPE_e pb_type,PbmRecord record)
{
	return qmi_ril_write_pb_info(pb_type,record);
}

int dtiot_hal_4g_cpb_del(PB_TYPE_e pb_type,int record_id)
{
	return qmi_ril_delete_pb_info(pb_type,record_id);
}

int dtiot_hal_4g_imsi_get(char* imsi)
{
	return qmi_ril_get_imsi(imsi);
}
int dtiot_hal_4g_imei_get(char* imei)
{
	return qmi_ril_get_imei(imei);
}

