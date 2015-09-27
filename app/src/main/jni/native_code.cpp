#include <jni.h>

#include "Game.h"

extern "C" {
    JNIEXPORT void JNICALL Java_app_zeptobird_ZeptoBirdLib_init(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_app_zeptobird_ZeptoBirdLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_app_zeptobird_ZeptoBirdLib_touch(JNIEnv * env, jobject obj, jfloat x, jfloat  y);
};

JNIEXPORT void JNICALL Java_app_zeptobird_ZeptoBirdLib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    ZB::Game::instance().setViewport(width, height);
}

JNIEXPORT void JNICALL Java_app_zeptobird_ZeptoBirdLib_step(JNIEnv * env, jobject obj)
{
    ZB::Game::instance().update();
    ZB::Game::instance().render();
}

JNIEXPORT void JNICALL Java_app_zeptobird_ZeptoBirdLib_touch(JNIEnv * env, jobject obj,  jfloat x, jfloat y)
{
    ZB::Game::instance().touch(x, y);
}