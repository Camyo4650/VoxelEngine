#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include "GLHelper.h"
#include <SDL/SDL_image.h>

Engine::GFX::Texture::Texture(const char* path)
{
    glCall(glGenTextures(1, &id));
    glCall(glBindTexture(GL_TEXTURE_2D, id));
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    float maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    glCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 64));
    /*
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        glCall(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    */
    SDL_Surface* img = IMG_Load(path);
    SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(NULL, img);
    int* width = NULL;
    int* height = NULL;
    SDL_QueryTexture(sdlTexture, NULL, NULL, width, height);

    // Get pixel data from SDL texture
    SDL_Surface* surface = SDL_CreateRGBSurface(0, *width, *height, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    if (surface == nullptr) {
        std::cerr << "SDL_CreateRGBSurface failed: " << SDL_GetError() << std::endl;
        
    }
    else {

        SDL_RenderReadPixels(NULL, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    }
}

void Engine::GFX::Texture::use(GLenum texture)
{
    glActiveTexture(texture);
    glBindTexture(GL_TEXTURE_2D, this->id);
}

Engine::GFX::Texture::~Texture()
{
}
