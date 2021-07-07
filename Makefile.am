AM_CFLAGS = -Wall -g \
            -Wundef \
            -Wstrict-prototypes \
            -Wno-trigraphs \
			-std=gnu99
            $(QMIFRAMEWORK_CFLAGS) \
	        $(QMI_CFLAGS) \
			$(LOCPLA_CFLAGS) \
			$(LOC_CFLAGS) \
			$(FLP_CFLAGS) 

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -D__packed__= \
              -DIMAGE_APPS_PROC \
              -DFEATURE_Q_SINGLE_LINK \
              -DFEATURE_Q_NO_SELF_QPTR \
              -DFEATURE_LINUX \
              -DFEATURE_NATIVELINUX \
              -DFEATURE_DSM_DUP_ITEMS \
              -DFEATURE_LE_DIAG \
			  -I $(WORKSPACE)/qmi/inc/ \
			  -I $(WORKSPACE)/qmi/platform/ \
			  -I $(WORKSPACE)/qmi/src/ \
			  -I $(WORKSPACE)/qmi/core/lib/inc/ \
			  -I $(WORKSPACE)/qmi-framework/inc/ \
			  -I $(WORKSPACE)/qmi/services/ \
			  -Icmd \
			  -Ilib \
			  -Isrc \
			  -Itestcmd \
			  -Iqmi \
			  $(QMIFRAMEWORK_CFLAGS) \
	          $(QMI_CFLAGS) \
	          $(LOCPLA_CFLAGS) \
	          $(LOC_CFLAGS) \
			  $(FLP_CFLAGS) 

init_ddir = ${sysconfdir}/
init_d_SCRIPTS = ./play.wav

bin_PROGRAMS = letter

requiredlibs = $(QMIFRAMEWORK_LIBS) $(QMI_LIBS) $(LOCPLA_LIBS) $(LOC_LIBS) $(FLP_LIBS)

# library_includedir = $(pkgincludedir)

letter_SOURCES = main.c \
	cmd/hello.c \
	lib/libiom.c lib/libstring.c lib/libsystem.c lib/libuart.c \
	src/shell.c src/shell_cmd_list.c src/shell_ext.c src/terminal.c \
	testcmd/play.c testcmd/serial.c testcmd/usbnum.c \
    qmi/modemtool.c qmi/dtiot_hal_qmi.c
	
#CFLAGS  += -DUSE_GLIB @GLIB_CFLAGS@ $(AM_CFLAGS)
#CPPFLAGS  += -DUSE_GLIB @GLIB_CFLAGS@ $(AM_CFLAGS)
#CFLAGS  += $(AM_CFLAGS)
#CPPFLAGS  +=  $(AM_CFLAGS)

letter_LDFLAGS = -lpthread $(requiredlibs) -lqmiservices -lqmi -lqmi_client_qmux -lqmi_cci -ldl
letter_LDADD = $(requiredlibs) -lstdc++ 


