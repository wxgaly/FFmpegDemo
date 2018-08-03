package nova.android.ffmpegdemo.bean;

/**
 * nova.android.ffmpegdemo.bean.
 *
 * @author Created by WXG on 2018/8/3 003 9:03.
 * @version V1.0
 */
public class AVCodecContext {

    /**
     * 编解码器
     */
    private AVCodec avCodec;

    /**
     * 图像的宽（只针对视频）
     */
    private int width;

    /**
     * 图像的高（只针对视频）
     */
    private int height;

    /**
     * 采样率（只针对音频）
     */
    private int sampleRate;

    /**
     * 声道数（只针对音频）
     */
    private int channels;

    public AVCodecContext() {
    }

    public AVCodecContext(AVCodec avCodec, int width, int height, int sampleRate, int channels) {
        this.avCodec = avCodec;
        this.width = width;
        this.height = height;
        this.sampleRate = sampleRate;
        this.channels = channels;
    }

    public AVCodec getAvCodec() {
        return avCodec;
    }

    public void setAvCodec(AVCodec avCodec) {
        this.avCodec = avCodec;
    }

    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getSampleRate() {
        return sampleRate;
    }

    public void setSampleRate(int sampleRate) {
        this.sampleRate = sampleRate;
    }

    public int getChannels() {
        return channels;
    }

    public void setChannels(int channels) {
        this.channels = channels;
    }

    @Override
    public String toString() {
        return "AVCodecContext{" +
                "avCodec=" + avCodec +
                ", width=" + width +
                ", height=" + height +
                ", sampleRate=" + sampleRate +
                ", channels=" + channels +
                '}';
    }
}
