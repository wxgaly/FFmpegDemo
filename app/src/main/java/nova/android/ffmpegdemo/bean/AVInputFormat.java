package nova.android.ffmpegdemo.bean;

/**
 * nova.android.ffmpegdemo.bean.
 *
 * @author Created by WXG on 2018/8/2 002 14:13.
 * @version V1.0
 */
public class AVInputFormat {

    /**
     * 封装格式名称
     */
    private String name;

    /**
     * 封装格式的长名称
     */
    private String longName;

    /**
     * 封装格式的扩展名
     */
    private String extensions;

    public AVInputFormat() {
    }

    public AVInputFormat(String name, String longName, String extensions) {
        this.name = name;
        this.longName = longName;
        this.extensions = extensions;
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

    public String getExtensions() {
        return extensions;
    }

    public void setExtensions(String extensions) {
        this.extensions = extensions;
    }

}
