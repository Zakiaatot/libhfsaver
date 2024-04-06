#include <stdlib.h>
#include "jni_libhfsaver.h"
#include "libhfsaver.h"
#include "error_map.hpp"

JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskBegin
(
    JNIEnv* env, jclass cls, jstring url, jstring savePath, jintArray taskId
)
{

    const char* nativeUrl = env->GetStringUTFChars(url, 0);
    const char* nativeSavePath = env->GetStringUTFChars(savePath, 0);
    int nativeTaskId;

    HfsRet result = hfs_task_begin(nativeUrl, nativeSavePath, &nativeTaskId);

    env->ReleaseStringUTFChars(url, nativeUrl);
    env->ReleaseStringUTFChars(savePath, nativeSavePath);

    if (result == OK)
    {
        env->SetIntArrayRegion(taskId, 0, 1, &nativeTaskId);
    }

    return result;
}

JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskEnd
(
    JNIEnv* env, jclass cls, jint taskId
)
{

    return hfs_task_end(taskId);
}

JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskQuery
(
    JNIEnv* env, jclass cls, jint taskId, jobject taskInfo
)
{
    // 获取Java类中的字段ID
    jclass taskInfoClass = env->GetObjectClass(taskInfo);

    // 获取Java类字段ID
    jfieldID fidTaskId = env->GetFieldID(taskInfoClass, "taskId", "I");
    jfieldID fidStatus = env->GetFieldID(taskInfoClass, "status", "I");
    jfieldID fidLastError = env->GetFieldID(taskInfoClass, "lastError", "I");
    jfieldID fidLastSaveTime = env->GetFieldID(taskInfoClass, "lastSaveTime", "J");
    jfieldID fidSavedSize = env->GetFieldID(taskInfoClass, "savedSize", "J");

    if (fidTaskId == NULL || fidStatus == NULL || fidLastError == NULL ||
        fidLastSaveTime == NULL || fidSavedSize == NULL) {
        return ERROR_JNI_OBJECT_DEFINE; // 适当的错误处理
    }

    HfsTaskInfo nativeTaskInfo;
    HfsRet result = hfs_task_query(taskId, &nativeTaskInfo);

    if (result != OK) {
        return result; // 直接返回错误结果
    }

    // 更新 Java 对象的字段
    env->SetIntField(taskInfo, fidTaskId, (jint)nativeTaskInfo.task_id);
    env->SetIntField(taskInfo, fidStatus, (jint)nativeTaskInfo.status);
    env->SetIntField(taskInfo, fidLastError, (jint)nativeTaskInfo.last_error);
    env->SetLongField(taskInfo, fidLastSaveTime, (jlong)nativeTaskInfo.last_save_time);
    env->SetLongField(taskInfo, fidSavedSize, (jlong)nativeTaskInfo.saved_size);

    return result;
}

JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskQueryAllSize
(
    JNIEnv* env, jclass cls, jintArray taskNum
)
{

    int nativeTaskNum;
    HfsRet result = hfs_task_query_all_size(&nativeTaskNum);

    if (result == OK)
    {
        env->SetIntArrayRegion(taskNum, 0, 1, &nativeTaskNum);
    }

    return result;
}

JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_taskQueryAll
(
    JNIEnv* env, jclass cls, jobjectArray taskInfoArray
)
{
    // 首先获取任务数量
    int task_num = 0;
    HfsRet retNum = hfs_task_query_all_size(&task_num);

    if (retNum != OK || task_num == 0)
    {
        // 错误处理或者没有任务
        return retNum;
    }

    HfsTaskInfo* taskInfoList = (HfsTaskInfo*)malloc(task_num * sizeof(HfsTaskInfo));
    if (taskInfoList == NULL)
    {
        // 内存分配失败
        return ERROR_JNI_MEMORY_ALLOC;
    }

    // 获取所有任务信息
    HfsRet result = hfs_task_query_all(taskInfoList);
    if (result != OK)
    {
        // 错误处理
        free(taskInfoList);
        return result;
    }

    // 获取 Java 类和字段 ID
    jclass taskInfoClass = env->FindClass("com/libhfsaver/HfsTaskInfo");
    if (taskInfoClass == NULL)
    {
        // 未找到类
        free(taskInfoList);
        return ERROR_JNI_OBJECT_DEFINE;
    }

    // 假设已经有一个以这个类为模板的构造函数
    jmethodID constructor = env->GetMethodID(taskInfoClass, "<init>", "()V");

    // 假设字段签名相同
    jfieldID fidTaskId = env->GetFieldID(taskInfoClass, "taskId", "I");
    jfieldID fidStatus = env->GetFieldID(taskInfoClass, "status", "I");
    jfieldID fidLastError = env->GetFieldID(taskInfoClass, "lastError", "I");
    jfieldID fidLastSaveTime = env->GetFieldID(taskInfoClass, "lastSaveTime", "J");
    jfieldID fidSavedSize = env->GetFieldID(taskInfoClass, "savedSize", "J");

    // 检查所有字段 ID 是否已获取
    if (constructor == NULL || fidTaskId == NULL || fidStatus == NULL || fidLastError == NULL ||
        fidLastSaveTime == NULL || fidSavedSize == NULL) {
        // 错误处理
        free(taskInfoList);
        return ERROR_JNI_OBJECT_DEFINE;
    }

    // 遍历所有任务信息，构造 Java 对象并填充数据
    for (int i = 0; i < task_num; i++) {
        jobject taskInfoObj = env->NewObject(taskInfoClass, constructor);

        // 设置字段
        env->SetIntField(taskInfoObj, fidTaskId, (jint)taskInfoList[i].task_id);
        env->SetIntField(taskInfoObj, fidStatus, (jint)taskInfoList[i].status);
        env->SetIntField(taskInfoObj, fidLastError, (jint)taskInfoList[i].last_error);
        env->SetLongField(taskInfoObj, fidLastSaveTime, (jlong)taskInfoList[i].last_save_time);
        env->SetLongField(taskInfoObj, fidSavedSize, (jlong)taskInfoList[i].saved_size);

        // 将 Java 对象设置到数组元素中
        env->SetObjectArrayElement(taskInfoArray, i, taskInfoObj);
    }

    // 释放 C 端数组资源
    free(taskInfoList);

    return OK;
}

JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_errorMessageQuery
(
    JNIEnv* env, jclass cls, jint errorCode, jbyteArray errBuf
)
{

    char nativeErrBuf[HFS_ERROR_BUF_SIZE];
    HfsRet result = hfs_error_message_query(errorCode, nativeErrBuf);

    if (result == OK)
    {
        env->SetByteArrayRegion(errBuf, 0, HFS_ERROR_BUF_SIZE, (jbyte*)nativeErrBuf);
    }

    return result;
}

JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_utilsSplitMp3FromFlv
(
    JNIEnv* env, jclass cls, jstring flvSavePath, jstring mp3SavePath
)
{

    const char* nativeFlvSavePath = env->GetStringUTFChars(flvSavePath, 0);
    const char* nativeMp3SavePath = env->GetStringUTFChars(mp3SavePath, 0);

    HfsRet result = hfs_utils_split_mp3_from_flv(nativeFlvSavePath, nativeMp3SavePath);

    env->ReleaseStringUTFChars(flvSavePath, nativeFlvSavePath);
    env->ReleaseStringUTFChars(mp3SavePath, nativeMp3SavePath);

    return result;
}

JNIEXPORT jint JNICALL Java_com_libhfsaver_JNI_utilsGetKeyFrame
(
    JNIEnv* env, jclass cls, jstring flvSavePath, jstring keyFrameSavePath, jfloat fps
)
{

    const char* nativeFlvSavePath = env->GetStringUTFChars(flvSavePath, 0);
    const char* nativeKeyFrameSavePath = env->GetStringUTFChars(keyFrameSavePath, 0);

    HfsRet result = hfs_utils_get_key_frame(nativeFlvSavePath, nativeKeyFrameSavePath, fps);

    env->ReleaseStringUTFChars(flvSavePath, nativeFlvSavePath);
    env->ReleaseStringUTFChars(keyFrameSavePath, nativeKeyFrameSavePath);

    return result;
}