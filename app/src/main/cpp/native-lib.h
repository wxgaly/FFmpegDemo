//
// Created by Administrator on 2018/8/2 002.
//

#ifndef FFMPEGDEMO_NATIVE_LIB_H
#define FFMPEGDEMO_NATIVE_LIB_H

#endif //FFMPEGDEMO_NATIVE_LIB_H

#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <unistd.h>


extern "C" {
//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//过滤器
#include "libavfilter/avfilter.h"

#include "libswscale/swscale.h"

#include <libavutil/imgutils.h>

#define FFLOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"ffmpeg",FORMAT,##__VA_ARGS__);
#define FFLOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",FORMAT,##__VA_ARGS__);
#define FFLOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG,"ffmpeg",FORMAT,##__VA_ARGS__);

}

jobject getJObject(JNIEnv *env, const char *name);
