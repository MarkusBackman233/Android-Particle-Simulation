#include "ResourceManager.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ResourceManager::ResourceManager(AAssetManager* assetManager)
    : m_assetManager(assetManager)
{

}
Texture ResourceManager::GetTexture(const std::string& name)
{
    auto it = m_textures.find(name);
    if (it != m_textures.end())
    {
        return it->second;
    }


    AAsset* texAsset = AAssetManager_open(m_assetManager, std::string("textures/").append(name).c_str(), AASSET_MODE_BUFFER);
    size_t size = AAsset_getLength(texAsset);
    std::vector<unsigned char> buffer(size);
    AAsset_read(texAsset, buffer.data(), size);
    AAsset_close(texAsset);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load_from_memory(buffer.data(), buffer.size(), &width, &height, &channels, STBI_rgb_alpha);
    Texture texture;
    texture.Width = width;
    texture.Height = height;
    glGenTextures(1, &texture.TextureHandle);
    glBindTexture(GL_TEXTURE_2D, texture.TextureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    m_textures.emplace(name, texture);
    return texture;
}