
#include <string>
#include "native-lib.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_stringFromJNI(JNIEnv *env, jobject instance) {


    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
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

}

extern "C"
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
}

extern "C"
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
}

extern "C"
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
}

extern "C"
JNIEXPORT jobject JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_getVideoInfo(JNIEnv *env, jobject instance,
                                                            jstring videoPath_) {

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


    //初始化videoInfo
    jclass clazz_video_info = env->FindClass("nova/android/ffmpegdemo/bean/VideoInfo");

    jmethodID mid_video_info = env->GetMethodID(clazz_video_info, "<init>", "(JJ)V");

    //使用构造方法赋值时长和码率
    jobject video_info = env->NewObject(clazz_video_info, mid_video_info, formatContext->duration,
                                        formatContext->bit_rate);

    //初始化iformat
    AVInputFormat *iformat = formatContext->iformat;

    jclass clazz_iformat = env->FindClass("nova/android/ffmpegdemo/bean/AVInputFormat");
    jmethodID mid_iformat = env->GetMethodID(clazz_iformat, "<init>",
                                             "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

    jstring name = env->NewStringUTF(iformat->name);
    jstring long_name = env->NewStringUTF(iformat->long_name);
    jstring extensions = env->NewStringUTF(iformat->extensions);

    //使用构造方法赋值 名称，长名称，扩展
    jobject jiformat = env->NewObject(clazz_iformat, mid_iformat, name, long_name, extensions);

    jmethodID mid_set_iformat = env->GetMethodID(clazz_video_info, "setAvInputFormat",
                                                 "(Lnova/android/ffmpegdemo/bean/AVInputFormat;)V");

    //调用set方法赋值
    env->CallVoidMethod(video_info, mid_set_iformat, jiformat);

    //初始化codec
    const AVCodec *codec = codecContext->codec;

    jstring codecName = env->NewStringUTF(codec->name);
    jstring codecLongName = env->NewStringUTF(codec->long_name);

    jclass clazz_codec = env->FindClass("nova/android/ffmpegdemo/bean/AVCodec");
    jmethodID mid_codec = env->GetMethodID(clazz_codec, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");

    //创建jcodec对象
    jobject jcodec = env->NewObject(clazz_codec, mid_codec, codecName, codecLongName);


    //初始化需要的值
    jint width = codecContext->width;
    jint height = codecContext->height;
    jint sample_rate = codecContext->sample_rate;
    jint channels = codecContext->channels;

    jclass clazz_codec_context = env->FindClass("nova/android/ffmpegdemo/bean/AVCodecContext");
    jmethodID mid_codec_context = env->GetMethodID(clazz_codec_context, "<init>",
                                                   "(Lnova/android/ffmpegdemo/bean/AVCodec;IIII)V");

    //创建jcodeccontext对象
    jobject jcodec_context = env->NewObject(clazz_codec_context, mid_codec_context, jcodec, width, height, sample_rate,
                                            channels);

    jmethodID mid_set_codec_context = env->GetMethodID(clazz_video_info, "setCodecContext",
                                                       "(Lnova/android/ffmpegdemo/bean/AVCodecContext;)V");

    //调用set方法赋值
    env->CallVoidMethod(video_info, mid_set_codec_context, jcodec_context);

    //释放资源
    avformat_free_context(formatContext);

    return video_info;
}

/**
 * 通过默认构造方法获取java对象
 * @param name
 * @return
 */
jobject getJObject(JNIEnv *env, const char *name) {
    jclass clazz_obj = env->FindClass(name);
    jmethodID mid_obj = env->GetMethodID(clazz_obj, "<init>", "()V");
    return env->NewObject(clazz_obj, mid_obj);
}

extern "C"
JNIEXPORT jint JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_getRotation(JNIEnv *env, jobject instance, jstring videoPath_) {

    const char *input = env->GetStringUTFChars(videoPath_, 0);

    if (input == NULL) {
        FFLOGI("字符串转换失败......");
        return -1;
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
        return -1;
    }

    //为分配的AVFormatContext 结构体中填充数据
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        FFLOGI("读取输入的视频流信息失败。");
        return -1;
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
        return -1;
    }


    //通过编解码器的id——codec_id 获取对应（视频）流解码器
//    AVCodecParameters *codecParameters = formatContext->streams[video_stream_index]->codecpar;

    //获取视频元数据
    AVDictionaryEntry *tag = NULL;

    tag = av_dict_get(formatContext->streams[video_stream_index]->metadata, "rotate", tag, 0);
    tag = av_dict_get(formatContext->metadata, "creation_time", tag, 0);

    int angle = -1;

    if (tag != NULL) {
//        angle = atoi(tag->value);
//        FFLOGD("angle: %d", angle)
        FFLOGD("%s", tag->value)
    }

    //释放资源
    avformat_free_context(formatContext);


    return angle;
}

extern "C"
JNIEXPORT jint JNICALL
Java_nova_android_ffmpegdemo_util_FFmpegHelper_renderVideo(JNIEnv *env, jobject instance, jstring videoPath_,
                                                           jobject surface) {
    const char *videoPath = env->GetStringUTFChars(videoPath_, 0);

    // TODO
    if (videoPath == NULL) {
        FFLOGI("字符串转换失败......");
        return -1;
    }

    //注册FFmpeg所有编解码器，以及相关协议。
    av_register_all();

    //分配结构体
    AVFormatContext *formatContext = avformat_alloc_context();

    //打开视频数据源。由于Android 对SDK存储权限的原因，如果没有为当前项目赋予SDK存储权限，打开本地视频文件时会失败
    int open_state = avformat_open_input(&formatContext, videoPath, NULL, NULL);
    if (open_state < 0) {
        char errbuf[128];
        if (av_strerror(open_state, errbuf, sizeof(errbuf)) == 0) {
            FFLOGI("打开视频输入流信息失败，失败原因： %s", errbuf);
        }
        return -1;
    }

    //为分配的AVFormatContext 结构体中填充数据
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        FFLOGI("读取输入的视频流信息失败。");
        return -1;
    }


    int video_stream_index = -1;//记录视频流所在数组下标
    int audio_stream_index = -1;
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
        return -1;
    }

    for (int i = 0; i < formatContext->nb_streams; i++) {

        //如果是数据流的编码格式为AVMEDIA_TYPE_VIDEO——视频流。
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;//记录音频流下标
            break;
        }
    }

    if (audio_stream_index == -1) {
        FFLOGI("没有找到 音频流。");
        return -1;
    }


    //通过编解码器的id——codec_id 获取对应（视频）流解码器
    AVCodecParameters *codecParameters = formatContext->streams[video_stream_index]->codecpar;
    AVCodec *videoDecoder = avcodec_find_decoder(codecParameters->codec_id);

    if (videoDecoder == NULL) {
        FFLOGI("未找到对应的视频流解码器。");
        return -1;
    }

    AVCodecParameters *pCodecCtx = formatContext->streams[audio_stream_index]->codecpar;
    //获取解码器
    AVCodec *pCodex = avcodec_find_decoder(pCodecCtx->codec_id);
    //打开解码器
    if (pCodex == NULL) {
        FFLOGI("未找到对应的音频流解码器。");
        return -1;
    }


    //通过解码器分配(并用  默认值   初始化)一个解码器context
    AVCodecContext *codecContext = avcodec_alloc_context3(videoDecoder);

    if (codecContext == NULL) {
        FFLOGI("分配 解码器上下文失败。");
        return -1;
    }

    //更具指定的编码器值填充编码器上下文
    if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
        FFLOGI("填充编解码器上下文失败。");
        return -1;
    }

    //通过所给的编解码器初始化编解码器上下文
    if (avcodec_open2(codecContext, videoDecoder, NULL) < 0) {
        FFLOGI("初始化 解码器上下文失败。");
        return -1;
    }

    AVPixelFormat dstFormat = AV_PIX_FMT_RGBA;
    //分配存储压缩数据的结构体对象AVPacket
    //如果是视频流，AVPacket会包含一帧的压缩数据。
    //但如果是音频则可能会包含多帧的压缩数据
    AVPacket *packet = av_packet_alloc();
    //分配解码后的每一数据信息的结构体（指针）
    AVFrame *frame = av_frame_alloc();
    //分配最终显示出来的目标帧信息的结构体（指针）
    AVFrame *outFrame = av_frame_alloc();
    uint8_t *out_buffer = (uint8_t *) av_malloc(
            (size_t) av_image_get_buffer_size(dstFormat, codecContext->width, codecContext->height,
                                              1));
    //更具指定的数据初始化/填充缓冲区
    av_image_fill_arrays(outFrame->data, outFrame->linesize, out_buffer, dstFormat,
                         codecContext->width, codecContext->height, 1);
    //初始化SwsContext
    SwsContext *swsContext = sws_getContext(
            codecContext->width   //原图片的宽
            , codecContext->height  //源图高
            , codecContext->pix_fmt //源图片format
            , codecContext->width  //目标图的宽
            , codecContext->height  //目标图的高
            , dstFormat, SWS_BICUBIC, NULL, NULL, NULL
    );
    if (swsContext == NULL) {
        FFLOGI("swsContext==NULL");
        return -1;
    }

    //native绘制
    //窗体
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    //绘制时的缓冲区
    ANativeWindow_Buffer outBuffer;

    //通过设置宽高限制缓冲区中的像素数量，而非屏幕的物流显示尺寸。
    //如果缓冲区与物理屏幕的显示尺寸不相符，则实际显示可能会是拉伸，或者被压缩的图像
    ANativeWindow_setBuffersGeometry(nativeWindow, codecContext->width, codecContext->height,
                                     WINDOW_FORMAT_RGBA_8888);


    //循环读取数据流的下一帧
    while (av_read_frame(formatContext, packet) == 0) {

        if (packet->stream_index == video_stream_index) {
            //讲原始数据发送到解码器
            int sendPacketState = avcodec_send_packet(codecContext, packet);
            if (sendPacketState == 0) {
                int receiveFrameState = avcodec_receive_frame(codecContext, frame);
                if (receiveFrameState == 0) {
                    //锁定窗口绘图界面
                    ANativeWindow_lock(nativeWindow, &outBuffer, NULL);
                    //对输出图像进行色彩，分辨率缩放，滤波处理
                    sws_scale(swsContext, (const uint8_t *const *) frame->data, frame->linesize, 0,
                              frame->height, outFrame->data, outFrame->linesize);
                    uint8_t *dst = (uint8_t *) outBuffer.bits;
                    //解码后的像素数据首地址
                    //这里由于使用的是RGBA格式，所以解码图像数据只保存在data[0]中。但如果是YUV就会有data[0]
                    //data[1],data[2]
                    uint8_t *src = outFrame->data[0];
                    //获取一行字节数
                    int oneLineByte = outBuffer.stride * 4;
                    //复制一行内存的实际数量
                    int srcStride = outFrame->linesize[0];
                    int size = codecContext->height;
                    int halfSize = size / 2;

                    //折行
                    for (int i = 0; i < halfSize; i++) {
                        memcpy(dst + (i + halfSize) * oneLineByte, src + i * srcStride, static_cast<size_t>(srcStride));
                    }

                    for (int i = 0; i < halfSize; i++) {
                        memcpy(dst + i * oneLineByte, src + (i + halfSize) * srcStride, static_cast<size_t>(srcStride));
                    }


                    //解锁
                    ANativeWindow_unlockAndPost(nativeWindow);
                    //进行短暂休眠。如果休眠时间太长会导致播放的每帧画面有延迟感，如果短会有加速播放的感觉。
                    //一般一每秒60帧——16毫秒一帧的时间进行休眠
                    usleep(1000 * 16);//20毫秒

                } else if (receiveFrameState == AVERROR(EAGAIN)) {
                    FFLOGI("从解码器-接收-数据失败：AVERROR(EAGAIN)");
                } else if (receiveFrameState == AVERROR_EOF) {
                    FFLOGI("从解码器-接收-数据失败：AVERROR_EOF");
                } else if (receiveFrameState == AVERROR(EINVAL)) {
                    FFLOGI("从解码器-接收-数据失败：AVERROR(EINVAL)");
                } else {
                    FFLOGI("从解码器-接收-数据失败：未知");
                }
            } else if (sendPacketState == AVERROR(EAGAIN)) {//发送数据被拒绝，必须尝试先读取数据
                FFLOGI("向解码器-发送-数据包失败：AVERROR(EAGAIN)");//解码器已经刷新数据但是没有新的数据包能发送给解码器
            } else if (sendPacketState == AVERROR_EOF) {
                FFLOGI("向解码器-发送-数据失败：AVERROR_EOF");
            } else if (sendPacketState == AVERROR(EINVAL)) {//遍解码器没有打开，或者当前是编码器，也或者需要刷新数据
                FFLOGI("向解码器-发送-数据失败：AVERROR(EINVAL)");
            } else if (sendPacketState == AVERROR(ENOMEM)) {//数据包无法压如解码器队列，也可能是解码器解码错误
                FFLOGI("向解码器-发送-数据失败：AVERROR(ENOMEM)");
            } else {
                FFLOGI("向解码器-发送-数据失败：未知");
            }
        }
        av_packet_unref(packet);
    }
    //内存释放
    ANativeWindow_release(nativeWindow);
    av_frame_free(&outFrame);
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);

    env->ReleaseStringUTFChars(videoPath_, videoPath);

    return 0;
}