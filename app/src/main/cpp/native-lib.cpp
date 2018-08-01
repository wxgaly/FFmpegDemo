#include <jni.h>
#include <string>
#include <android/log.h>


extern "C" {
//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//过滤器
#include "libavfilter/avfilter.h"

#define FFLOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"ffmpeg",FORMAT,##__VA_ARGS__);
#define FFLOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",FORMAT,##__VA_ARGS__);


}

extern "C"
JNIEXPORT jstring JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_stringFromJNI(JNIEnv *env, jobject instance) {


    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}extern "C"
JNIEXPORT jstring JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_urlprotocolinfo(JNIEnv *env, jobject instance) {

    char info[40000] = {0};
    av_register_all();

    struct URLProtocol *pup = NULL;

    struct URLProtocol **p_temp = &pup;
    avio_enum_protocols((void **) p_temp, 0);

    while ((*p_temp) != NULL) {
        sprintf(info, "%sInput: %s\n", info, avio_enum_protocols((void **) p_temp, 0));
    }
    pup = NULL;
    avio_enum_protocols((void **) p_temp, 1);
    while ((*p_temp) != NULL) {
        sprintf(info, "%sInput: %s\n", info, avio_enum_protocols((void **) p_temp, 1));
    }
    return env->NewStringUTF(info);

}extern "C"
JNIEXPORT jstring JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_avformatinfo(JNIEnv *env, jobject instance) {

    char info[40000] = {0};

    av_register_all();

    AVInputFormat *if_temp = av_iformat_next(NULL);
    AVOutputFormat *of_temp = av_oformat_next(NULL);
    while (if_temp != NULL) {
        sprintf(info, "%sInput: %s\n", info, if_temp->name);
        if_temp = if_temp->next;
    }
    while (of_temp != NULL) {
        sprintf(info, "%sOutput: %s\n", info, of_temp->name);
        of_temp = of_temp->next;
    }
    return env->NewStringUTF(info);
}extern "C"
JNIEXPORT jstring JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_avcodecinfo(JNIEnv *env, jobject instance) {

    char info[40000] = {0};

    av_register_all();

    AVCodec *c_temp = av_codec_next(NULL);

    while (c_temp != NULL) {
        if (c_temp->decode != NULL) {
            sprintf(info, "%sdecode:", info);
        } else {
            sprintf(info, "%sencode:", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s(video):", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s(audio):", info);
                break;
            default:
                sprintf(info, "%s(other):", info);
                break;
        }
        sprintf(info, "%s[%10s]\n", info, c_temp->name);
        c_temp = c_temp->next;
    }

    return env->NewStringUTF(info);
}extern "C"
JNIEXPORT jstring JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_avfilterinfo(JNIEnv *env, jobject instance) {

    char info[40000] = {0};
    avfilter_register_all();

    AVFilter *f_temp = (AVFilter *) avfilter_next(NULL);
    while (f_temp != NULL) {
        sprintf(info, "%s%s\n", info, f_temp->name);
        f_temp = f_temp->next;
    }
    return env->NewStringUTF(info);
}extern "C"
JNIEXPORT jstring JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_getVideoInfo(JNIEnv *env, jobject instance, jstring videoPath_) {
    const char *input = env->GetStringUTFChars(videoPath_, 0);

    if (input == NULL) {
        FFLOGI("字符串转换失败......");
        return env->NewStringUTF("字符串转换失败......");
    }

    //注册FFmpeg所有编解码器，以及相关协议。
    av_register_all();

    //分配结构体
    AVFormatContext *formatContext = avformat_alloc_context();

    //打开视频数据源。由于Android 对SDK存储权限的原因，如果没有为当前项目赋予SDK存储权限，打开本地视频文件时会失败
    int open_state = avformat_open_input(&formatContext, input, NULL, NULL);
    if (open_state < 0) {
        char errbuf[128];
        if (av_strerror(open_state, errbuf, sizeof(errbuf)) == 0) {
            FFLOGI("打开视频输入流信息失败，失败原因： %s", errbuf);
        }
        return env->NewStringUTF("打开视频输入流信息失败");
    }

    //为分配的AVFormatContext 结构体中填充数据
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        FFLOGI("读取输入的视频流信息失败。");
        return env->NewStringUTF("读取输入的视频流信息失败");
    }

    int video_stream_index = -1;//记录视频流所在数组下标
    FFLOGI("当前视频数据，包含的数据流数量：%d", formatContext->nb_streams);
    //找到"视频流".AVFormatContext 结构体中的nb_streams字段存储的就是当前视频文件中所包含的总数据流数量——
    //视频流，音频流，字幕流
    for (int i = 0; i < formatContext->nb_streams; i++) {

        //如果是数据流的编码格式为AVMEDIA_TYPE_VIDEO——视频流。
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;//记录视频流下标
            break;
        }
    }

    if (video_stream_index == -1) {
        FFLOGI("没有找到 视频流。");
        return env->NewStringUTF("没有找到 视频流。");
    }

    //通过编解码器的id——codec_id 获取对应（视频）流解码器
    AVCodecParameters *codecParameters = formatContext->streams[video_stream_index]->codecpar;
    AVCodec *videoDecoder = avcodec_find_decoder(codecParameters->codec_id);

    if (videoDecoder == NULL) {
        FFLOGI("未找到对应的流解码器。");
        return env->NewStringUTF("未找到对应的流解码器");
    }

    //通过解码器分配(并用  默认值   初始化)一个解码器context
    AVCodecContext *codecContext = avcodec_alloc_context3(videoDecoder);

    if (codecContext == NULL) {
        FFLOGI("分配 解码器上下文失败。");
        return env->NewStringUTF("分配 解码器上下文失败");
    }

    //更具指定的编码器值填充编码器上下文
    if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
        FFLOGI("填充编解码器上下文失败。");
        return env->NewStringUTF("填充编解码器上下文失败");
    }
    //通过所给的编解码器初始化编解码器上下文
    if (avcodec_open2(codecContext, videoDecoder, NULL) < 0) {
        FFLOGI("初始化 解码器上下文失败。");
        return env->NewStringUTF("初始化 解码器上下文失败");
    }



    return env->NewStringUTF(returnValue);
}