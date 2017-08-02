LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := zxjson_static

LOCAL_MODULE_FILENAME := libzxjson

LOCAL_SRC_FILES := \
CCJSON.cpp \
StringToNumber.cpp \
ToUnicode.cpp \
ToUTF8.cpp \


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \

LOCAL_C_INCLUDES := $(LOCAL_PATH) \


LOCAL_LDLIBS := -lGLESv2 \
                -llog \
                -lz \
                -landroid

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog \
                       -lz \
                       -landroid

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

# define the macro to compile through support/zip_support/ioapi.c
LOCAL_CFLAGS   := -Wno-psabi  -DUSE_FILE32API
LOCAL_CPPFLAGS := -Wno-literal-suffix -Wno-deprecated-declarations
LOCAL_EXPORT_CFLAGS   := -Wno-psabi -DUSE_FILE32API
LOCAL_EXPORT_CPPFLAGS := -Wno-literal-suffix -Wno-deprecated-declarations

include $(BUILD_STATIC_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../cocos2d)

$(call import-module, cocos)
