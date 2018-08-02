package nova.android.ffmpegdemo.util;

import nova.android.ffmpegdemo.bean.VideoInfo;

/**
 * nova.android.ffmpegdemo.util.
 *
 * @author Created by WXG on 2018/8/1 001 15:03.
 * @version V1.0
 */
public class FFmpegHelper {

    static {
        System.loadLibrary("native-lib");
//        System.loadLibrary("avcodec-57");
//        System.loadLibrary("avfilter-6");
//        System.loadLibrary("avformat-57");
//        System.loadLibrary("avutil-55");
//        System.loadLibrary("swresample-2");
//        System.loadLibrary("swscale-4");
    }

    private static volatile FFmpegHelper singleton = null;

    private FFmpegHelper() {

    }

    public static FFmpegHelper getInstance() {
        if (singleton == null) {
            synchronized (FFmpegHelper.class) {
                if (singleton == null) {
                    singleton = new FFmpegHelper();
                }
            }
        }
        return singleton;
    }

    public native String stringFromJNI();

    public native String urlprotocolinfo();
    public native String avformatinfo();
    public native String avcodecinfo();
    public native String avfilterinfo();

    public native VideoInfo getVideoInfo(String videoPath);

}
