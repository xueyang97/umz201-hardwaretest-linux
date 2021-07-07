#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <qmi.h>
#include <string.h>

#include "qmi_client.h"
#include "qmi_client_instance_defs.h"
#include "qmi_cci_target_ext.h"
#include "qmi_idl_lib.h"
#include "voice_service_v02.h"
#include "device_management_service_v01.h"

#include "dtiot_hal_qmi.h"

int qmi_test_imei(int argc, char *argv[])
{
	dtiot_hal_4g_init(NULL);
	
	char imei[128];
	memset(imei,0,sizeof(imei));
	dtiot_hal_4g_imei_get(imei);
	
	if (argc == 1) {
		if (strlen(imei) == 0) {
			printfs("IMEI = %s\n","error");
			return 1;
		} else {
			printfs("IMEI = %s\n",imei);
			return 0;
		}
	} else if(argc == 2) {
		printfs("IMEI = %s\n",imei);
		if (strcmp(argv[1], imei) == 0) {
			return 0;
		}
		return 1;
	}

	printfs("parameters error\n");
    return 1;
}

#include "../lib/libsystem.h"
#include "../lib/libstring.h"
static const char *strmode(DTIOT_HAL_DEVICE_MODE_e mode);
static int get_signal_level(SignalInfo signalinfo);
int qmi_test_lte(int argc, char *argv[])
{
	// char buffer[1024];
	// memset(buffer, 0, sizeof(buffer));
	// systemosp("ping 114.114.114.114 -c 4", buffer, sizeof(buffer));
	// printfs("%s\n", buffer);
	if (argc == 2) {
		systemos(argv[1]);
	}

	SignalInfo signalinfo;
	memset(&signalinfo, 0, sizeof(signalinfo));
	dtiot_hal_4g_init(NULL);
	dtiot_hal_4g_signal_get(&signalinfo);
	printfs("LTE signal RSRP = %d\n", signalinfo.rsrp);

	return 0;

}

static const char *normal_name   = "DTIOT_E_4G_NORMAL_MODE";
static const char *airplane_name = "DTIOT_E_4G_AIRPLANE_MODE";
static const char *error_name    = "DTIOT_E_4G_ERROR_MODE";
static const char *unknown_name  = "DTIOT_E_4G_UNKNOWN_MODE";
static const char *strmode(DTIOT_HAL_DEVICE_MODE_e mode)
{
	switch((int)mode) {
	case DTIOT_E_4G_NORMAL_MODE   : return normal_name;
	case DTIOT_E_4G_AIRPLANE_MODE : return airplane_name;
	case DTIOT_E_4G_ERROR_MODE    : return error_name;
	}
	return unknown_name;
}

static int get_signal_level(SignalInfo signalinfo)
{
	int rsrp = signalinfo.rsrp;
	if (rsrp <= -105 && rsrp > -140) {
		return 6;
	} else if (rsrp <= -95 && rsrp > -105) {
		return 5;
	} else if (rsrp <= -85 && rsrp > -95) {
		return 4;
	} else if (rsrp <= -75 && rsrp > -85) {
		return 3;
	} else if (rsrp <= -65 && rsrp > -75) {
		return 2;
	} else if (rsrp <= -44 && rsrp > -65) {
		return 1;
	}
	return -1;
}
