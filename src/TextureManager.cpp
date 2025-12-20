#include "../include/TextureManager.h"

#include <stdexcept>

SDL_Texture* TextureManager::loadTexture(TextureName textureName) {
  const std::string filePath = textureCache[textureName].filePath;
  char* png_path = NULL;
  SDL_asprintf(&png_path, "%s../assets/%s", SDL_GetBasePath(),
               filePath.c_str());

  auto* texture = IMG_LoadTexture(state->renderer, png_path);
  if (!texture) {
    SDL_Log("Couldn't create static texture: %s", SDL_GetError());
    throw std::runtime_error("Failed to load texture");
  }
  SDL_free(png_path);
  return texture;
}

TextureManager::TextureManager(GlobalState* state) : state(state) {
  try {
    // TODO: look into async loading of textures
    for (auto& [name, texture] : textureCache) {
      texture.sdlTexture = loadTexture(name);
    }
  } catch (const std::runtime_error& e) {
    SDL_Log("Error loading textures: %s", e.what());
    // TODO: handle error appropriately
  }
}
TextureManager::~TextureManager() {
  for (auto& [name, texture] : textureCache) {
    if (texture.sdlTexture) {
      SDL_DestroyTexture(texture.sdlTexture);
      texture.sdlTexture = nullptr;
    }
  }
}
SDL_Texture* TextureManager::getTexture(TextureName name) {
  return textureCache[name].sdlTexture;
}