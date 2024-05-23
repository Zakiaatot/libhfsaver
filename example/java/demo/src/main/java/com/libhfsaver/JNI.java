package com.libhfsaver;

public class JNI {
    static {
        try {
            // 根据操作系统类型构建库文件路径
            String osName = System.getProperty("os.name").toLowerCase();
            String libPath = System.getProperty("user.dir");
            if (osName.contains("win")) {
                libPath += "\\lib\\";
                libPath += "hfsaver_shared.dll";
            } else if (osName.contains("nux") || osName.contains("nix")) {
                libPath += "/lib/";
                libPath += "libhfsaver_shared.so";
            } else {
                throw new UnsupportedOperationException("不支持的操作系统: " + osName);
            }
            System.out.println(libPath);
            System.load(libPath);
        } catch (UnsatisfiedLinkError e) {
            System.err.println("无法加载库文件: " + e.getMessage());
            e.printStackTrace();
        }
    }

    // native 方法声明
    public static native int taskBegin(String url, String savePath, int[] taskId);

    public static native int taskEnd(int taskId);

    public static native int taskQuery(int taskId, HfsTaskInfo taskInfo);

    public static native int taskQueryAllSize(int[] taskNum);

    public static native int taskQueryAll(HfsTaskInfo[] taskInfoArray);

    public static native int errorMessageQuery(int errorCode, byte[] errBuf);

    public static native int utilsSplitMp3FromFlv(String flvSavePath, String mp3SavePath);

    public static native int utilsGetKeyFrame(String flvSavePath, String keyFrameSavePath, float fps);

    public static native int utilsFlvToMp4(String flvSavePath, String mp4SavePath);

    public static native int utilsCutVideo(String inPath, String outPath, long start_sec, long end_sec);

    public static native int utilsGetVideoInfo(String inPath, HfsVideoInfo videoInfo);

    public static int startTask(String url, String savePath) throws JNIException {
        int[] taskId = new int[1];
        int resultCode = taskBegin(url, savePath, taskId);

        if (resultCode != 0) {
            throw new JNIException("Failed to start task", resultCode);
        }

        return taskId[0];
    }

    public static void endTask(int taskId) throws JNIException {
        int resultCode = taskEnd(taskId);

        if (resultCode != 0) {
            throw new JNIException("Failed to end task", resultCode);
        }
    }

    public static HfsTaskInfo queryTask(int taskId) throws JNIException {
        HfsTaskInfo taskInfo = new HfsTaskInfo();
        int resultCode = taskQuery(taskId, taskInfo);

        if (resultCode != 0) {
            throw new JNIException("Failed to query task", resultCode);
        }

        return taskInfo;
    }

    public static int queryAllTasksSize() throws JNIException {
        int[] taskNum = new int[1];
        int resultCode = taskQueryAllSize(taskNum);

        if (resultCode != 0) {
            throw new JNIException("Failed to query all tasks size", resultCode);
        }

        return taskNum[0];
    }

    public static HfsTaskInfo[] queryAllTasks() throws JNIException {
        int taskCount = queryAllTasksSize();
        HfsTaskInfo[] taskInfoArray = new HfsTaskInfo[taskCount];

        for (int i = 0; i < taskCount; i++) {
            taskInfoArray[i] = new HfsTaskInfo();
        }

        int resultCode = taskQueryAll(taskInfoArray);

        if (resultCode != 0) {
            throw new JNIException("Failed to query all tasks", resultCode);
        }

        return taskInfoArray;
    }

    public static String errorMessageQuery(int errorCode) throws JNIException {
        byte[] errBuf = new byte[256]; // Suppose that error message won't exceed 256 bytes.
        int resultCode = errorMessageQuery(errorCode, errBuf);

        if (resultCode != 0) {
            throw new JNIException("Failed to query error message", resultCode);
        }

        return new String(errBuf).trim();
    }

    public static void splitMp3FromFlv(String flvSavePath, String mp3SavePath) throws JNIException {
        int resultCode = utilsSplitMp3FromFlv(flvSavePath, mp3SavePath);

        if (resultCode != 0) {
            throw new JNIException("Failed to split MP3 from FLV", resultCode);
        }
    }

    public static void getKeyFrame(String flvSavePath, String keyFrameSavePath, float fps) throws JNIException {
        int resultCode = utilsGetKeyFrame(flvSavePath, keyFrameSavePath, fps);

        if (resultCode != 0) {
            throw new JNIException("Failed to get key frame", resultCode);
        }
    }

    public static void flvToMp4(String flvSavePath, String mp4SavePath) throws JNIException {
        int resultCode = utilsFlvToMp4(flvSavePath, mp4SavePath);

        if (resultCode != 0) {
            throw new JNIException("Failed to convert FLV to MP4", resultCode);
        }
    }

    // 自定义异常类
    public static class JNIException extends Exception {
        private final int errorCode;

        public JNIException(String message, int errorCode) {
            super(message);
            this.errorCode = errorCode;
        }

        public int getErrorCode() {
            return errorCode;
        }
    }
}
