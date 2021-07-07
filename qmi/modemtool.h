#ifndef __MODEMTOOL_H__
#define __MODEMTOOL_H__

#include "../lib/libiom.h"
int main_t(int argc, const char *argv[]);

extern int qmi_test_imei(int argc, char *argv[]);
extern int qmi_test_lte(int argc, char *argv[]);

#endif /* !__MODEMTOOL_H__ */
