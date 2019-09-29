//
// Created by linus.yang on 2019-09-23.
//
#include <jni.h>
#ifndef FFMPEGSAMPLE_DATACLIENT_H
#define FFMPEGSAMPLE_DATACLIENT_H
#ifdef __cplusplus
extern  "C" {
#endif

JNIEXPORT void JNICALL Java_com_caihui_ffmpegsample_DataClient_startSocketConnection(JNIEnv * , jobject, jstring, jint , jstring, jstring);

JNIEXPORT void JNICALL Java_com_caihui_ffmpegsample_DataClient_stopSocketConnectin(JNIEnv *, jobject);


#ifdef __cplusplus
}
#endif



#endif //FFMPEGSAMPLE_DATACLIENT_H
