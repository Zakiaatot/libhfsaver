#ifndef _JNI_LIBHFSAVER_H_
#define _JNI_LIBHFSAVER_H_
#ifdef __cplusplus
extern "C" {
#endif //!__cplusplus

#include <jni.h>

    // 标记: JNI版本的 http-flv 保存任务接口
    JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskBegin(JNIEnv* env, jclass cls, jstring url, jstring savePath, jintArray taskId);

    JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskEnd(JNIEnv* env, jclass cls, jint taskId);

    JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskQuery(JNIEnv* env, jclass cls, jint taskId, jobject taskInfo);

    JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskQueryAllSize(JNIEnv* env, jclass cls, jintArray taskNum);

    JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskQueryAll(JNIEnv* env, jclass cls, jobjectArray taskInfoArray);

    // 标记: JNI版本的错误信息查询接口
    JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_errorMessageQuery(JNIEnv* env, jclass cls, jint errorCode, jbyteArray errBuf);

    // 标记: JNI版本的工具接口
    JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_utilsSplitMp3FromFlv(JNIEnv* env, jclass cls, jstring flvSavePath, jstring mp3SavePath);

    JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_utilsGetKeyFrame(JNIEnv* env, jclass cls, jstring flvSavePath, jstring keyFrameSavePath, jfloat fps);


#ifdef __cplusplus
}
#endif //!__cplusplus
#endif //!_JNI_LIBHFSAVER_H_