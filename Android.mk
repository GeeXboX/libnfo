LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libnfo
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := \
	src/nfo.c \
	src/nfo_priv.c \
	src/nfo_osdep.c \
	src/nfo_xml_parser_expat.c \

LOCAL_CFLAGS += -Wall
LOCAL_CFLAGS += -W
LOCAL_CFLAGS += -D_LARGEFILE_SOURCE
LOCAL_CFLAGS += -D_FILE_OFFSET_BITS=64
LOCAL_CFLAGS += -DXML_PARSER_EXPAT

LOCAL_C_INCLUDES += $(LOCAL_PATH)/src
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_STATIC_LIBRARIES += expat

include $(BUILD_SHARED_LIBRARY)
$(call import-module,expat)
