#include <jni.h>
#include <string>

extern "C" {
#include "libavformat/avformat.h"
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_caihui_ffmpegsample_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    av_register_all();
   const char * m = avcodec_configuration();
    return env->NewStringUTF(m);
//    return env->NewStringUTF(hello.c_str());
}
