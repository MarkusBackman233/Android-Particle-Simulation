#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES3/gl3.h>

struct Texture
{
    GLuint TextureHandle;
    int Width;
    int Height;
};

class ResourceManager
{
public:
    ResourceManager(AAssetManager* assetManager);

    Texture GetTexture(const std::string& name);

private:
    std::unordered_map<std::string, Texture> m_textures;
    AAssetManager* m_assetManager;
};
