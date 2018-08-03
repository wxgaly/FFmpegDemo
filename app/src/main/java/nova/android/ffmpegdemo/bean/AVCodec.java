package nova.android.ffmpegdemo.bean;

/**
 * nova.android.ffmpegdemo.bean.
 *
 * @author Created by WXG on 2018/8/3 003 9:04.
 * @version V1.0
 */
public class AVCodec {

    /**
     * 编码器名称
     */
    private String name;

    /**
     * 编码器的长名称
     */
    private String longName;

    public AVCodec() {
    }

    public AVCodec(String name, String longName) {
        this.name = name;
        this.longName = longName;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getLongName() {
        return longName;
    }

    public void setLongName(String longName) {
        this.longName = longName;
    }

    @Override
    public String toString() {
        return "AVCodec{" +
                "name='" + name + '\'' +
                ", longName='" + longName + '\'' +
                '}';
    }
}
