
lib_LTLIBRARIES = libvcu_apm.la

AM_DEFAULT_SOURCE_EXT = .c

vcu_apm_lib_INC_DIR = ./include
vcu_apm_lib_SRC_DIR = ./src

nodist_libvcu_apm_la_SOURCES = \
		$(vcu_apm_lib_INC_DIR)/perfapm.h \
		$(vcu_apm_lib_INC_DIR)/uio_common.h \
		$(vcu_apm_lib_INC_DIR)/uio_perfmon.h \
		$(vcu_apm_lib_INC_DIR)/uio_perfmon_hw.h

libvcu_apm_la_includedir = /usr/include
libvcu_apm_la_include_HEADERS = \
		$(vcu_apm_lib_INC_DIR)/perfapm.h

libvcu_apm_la_SOURCES = \
		$(vcu_apm_lib_SRC_DIR)/uio_common.c \
		$(vcu_apm_lib_SRC_DIR)/uio_perfmon.c \
		$(vcu_apm_lib_SRC_DIR)/perfmon.c

libvcu_apm_la_CFLAGS = -I$(vcu_apm_lib_INC_DIR)
