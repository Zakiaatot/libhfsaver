package com.libhfsaver;

public class HfsTaskInfo {
    private int taskId;
    private int status; // 注意处理枚举映射到Java
    private int lastError;
    private long lastSaveTime;
    private long savedSize;

    // 无参构造器
    public HfsTaskInfo() {
        // 默认的构造器
    }

    // 带参数的构造器，可选，便于在代码中创建实例
    public HfsTaskInfo(int taskId, int status, int lastError, long lastSaveTime, long savedSize) {
        this.taskId = taskId;
        this.status = status;
        this.lastError = lastError;
        this.lastSaveTime = lastSaveTime;
        this.savedSize = savedSize;
    }

    // 注意：此处的getter和setter方法是公开的，以便JNI使用

    public int getTaskId() {
        return taskId;
    }

    public void setTaskId(int taskId) {
        this.taskId = taskId;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public int getLastError() {
        return lastError;
    }

    public void setLastError(int lastError) {
        this.lastError = lastError;
    }

    public long getLastSaveTime() {
        return lastSaveTime;
    }

    public void setLastSaveTime(long lastSaveTime) {
        this.lastSaveTime = lastSaveTime;
    }

    public long getSavedSize() {
        return savedSize;
    }

    public void setSavedSize(long savedSize) {
        this.savedSize = savedSize;
    }

    // 重写 toString 方法，便于打印和调试
    @Override
    public String toString() {
        return "HfsTaskInfo{" +
                "taskId=" + taskId +
                ", status=" + status +
                ", lastError=" + lastError +
                ", lastSaveTime=" + lastSaveTime +
                ", savedSize=" + savedSize +
                '}';
    }
}
