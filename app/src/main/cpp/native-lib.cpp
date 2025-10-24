#include <jni.h>
#include "Renderer.h"
#include "Blackjack.h"
#include "ButtonManager.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

static Renderer* renderer = nullptr;
static Blackjack* blackjack = nullptr;
static ButtonManager* buttonManager = nullptr;



extern "C" JNIEXPORT void JNICALL
Java_com_example_game_NativeRenderer_init(JNIEnv* env,
    jclass,
    jobject assetManager,
    jint screenWidth,
    jint screenHeight
    )
{
    auto aam = AAssetManager_fromJava(env, assetManager);

    renderer = new Renderer(aam, screenWidth, screenHeight);
    buttonManager = new ButtonManager();
    blackjack = new Blackjack(aam, renderer, buttonManager);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_game_NativeRenderer_resize(JNIEnv*, jclass, jint width, jint height)
{
    if (renderer)
        renderer->Resize(static_cast<float>(width), static_cast<float>(height));
}


extern "C" JNIEXPORT void JNICALL
Java_com_example_game_NativeRenderer_render(JNIEnv*, jclass)
{
    if (buttonManager && blackjack && renderer)
    {
        buttonManager->UpdateButtons();
        blackjack->Update();
        renderer->Render();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_NativeRenderer_onTouchMove(JNIEnv *env, jclass clazz, jfloat x, jfloat y, int pointerId)
{
    if (buttonManager)
        buttonManager->OnTouchMove({x,y}, pointerId);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_NativeRenderer_onTouchDown(JNIEnv *env, jclass clazz, jfloat x, jfloat y, int pointerId)
{
    if (buttonManager)
    buttonManager->OnTouchDown({x,y}, pointerId);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_NativeRenderer_onTouchUp(JNIEnv *env, jclass clazz, jfloat x, jfloat y, int pointerId)
{
    if (buttonManager)
        buttonManager->OnTouchUp({x,y}, pointerId);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_NativeRenderer_updateOrientation(JNIEnv *env, jclass clazz, jfloat v,
                                                       jfloat v1, jfloat v2)
{
    if (blackjack)
    {
        blackjack->UpdatePhoneOrientation({v,v1,v2});
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_NativeRenderer_AddAcceleration(JNIEnv *env, jclass clazz, jfloat v1,
                                                     jfloat v2, jfloat v3) {
    if (blackjack)
    {
        blackjack->UpdateAcceleration({v1,v2,v3});
    }
}