package com.libhfsaver;

import com.libhfsaver.JNI.JNIException;

public class Main {
    public static void main(String[] args) {
        int taskId = 0;
        try {
            taskId = JNI.startTask(
                    "https://hw-adaptive.pull.yximgs.com/gifshow/kwai_actL_ksle_20240318110103_iJP_strL_hd2000.flv?hwTime=661124d6&hwSecret=86ca6ecd1ead5d98ab459a2582726634&tsc=origin&highTraffic=2&oidc=alihb&srcStrm=31lbAKZnuWY&fd=1&ss=s23&kabr_spts=-5000",
                    "out.flv");
        } catch (JNIException e) {
            e.printStackTrace();
        }
        int counter = 1;
        for (;;) {
            counter++;
            try {
                Thread.sleep(1000);
                System.out.println(JNI.queryTask(taskId).toString());
                if (counter == 10) {
                    JNI.endTask(taskId);
                    break;
                }
            } catch (InterruptedException | JNIException e) {
                e.printStackTrace();
            }
        }
        try {
            JNI.splitMp3FromFlv("out.flv", "1.mp3");
        } catch (JNIException e) {
            e.printStackTrace();
        }
        try {
            JNI.getKeyFrame("out.flv", "frames", (float) 0.2);
        } catch (JNIException e) {
            e.printStackTrace();
        }
    }
}