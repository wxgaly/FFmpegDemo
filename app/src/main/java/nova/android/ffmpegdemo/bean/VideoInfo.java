package nova.android.ffmpegdemo.bean;

/**
 * nova.android.ffmpegdemo.bean.
 *
 * @author Created by WXG on 2018/8/2 002 9:26.
 * @version V1.0
 */
public class VideoInfo {

    /**
     * 视频时长
     */
    private long duration;

    /**
     * 视频码率
     */
    private long bitRate;

    /**
     * 封装格式信息
     */
    private AVInputFormat avInputFormat;

    /**
     * 编解码器信息
     */
    private AVCodecContext codecContext;

    public VideoInfo() {
    }

    public VideoInfo(long duration, long bitRate) {
        this.duration = duration;
        this.bitRate = bitRate;
    }

    public VideoInfo(long duration, long bitRate, AVInputFormat avInputFormat) {
        this.duration = duration;
        this.bitRate = bitRate;
        this.avInputFormat = avInputFormat;
    }

    public VideoInfo(long duration, long bitRate, AVInputFormat avInputFormat, AVCodecContext codecContext) {
        this.duration = duration;
        this.bitRate = bitRate;
        this.avInputFormat = avInputFormat;
        this.codecContext = codecContext;
    }

    public long getDuration() {
        return duration;
    }

    public void setDuration(long duration) {
        this.duration = duration;
    }

    public long getBitRate() {
        return bitRate;
    }

    public void setBitRate(long bitRate) {
        this.bitRate = bitRate;
    }

    public AVInputFormat getAvInputFormat() {
        return avInputFormat;
    }

    public void setAvInputFormat(AVInputFormat avInputFormat) {
        this.avInputFormat = avInputFormat;
    }

    public AVCodecContext getCodecContext() {
        return codecContext;
    }

    public void setCodecContext(AVCodecContext codecContext) {
        this.codecContext = codecContext;
    }

    @Override
    public String toString() {
        return "VideoInfo{" +
                "duration=" + duration +
                ", bitRate=" + bitRate +
                ", avInputFormat=" + avInputFormat +
                ", codecContext=" + codecContext +
                '}';
    }
}
