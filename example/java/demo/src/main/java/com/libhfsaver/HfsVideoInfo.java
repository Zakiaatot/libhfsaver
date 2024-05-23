package com.libhfsaver;

public class HfsVideoInfo {
    private long size;
    private long duration;
    private String rawInfo;

    // 无参构造器
    public HfsVideoInfo() {
        // 默认的构造器
    }

    // 带参数的构造器，可选，便于在代码中创建实例
    public HfsVideoInfo(long size, long duration, String rawInfo) {
        this.size = size;
        this.duration = duration;
        this.rawInfo = rawInfo;
    }

    // 注意：此处的getter 和setter方法是公开的，以便 JNI 使用

    public long getSize() {
        return size;
    }

    public void setSize(long size) {
        this.size = size;
    }

    public long getDuration() {
        return duration;
    }

    public void setDuration(long duration) {
        this.duration = duration;
    }

    public String getRawInfo() {
        return rawInfo;
    }

    public void setRawInfo(String rawInfo) {
        this.rawInfo = rawInfo;
    }

    // 重写 toString 方法，便于打印和调试
    @Override
    public String toString() {
        return "HfsVideoInfo{" +
                "size=" + size +
                ", duration=" + duration +
                ", rawInfo='" + rawInfo + '\'' +
                '}';
    }
}