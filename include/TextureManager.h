#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "GlobalState.h"

struct Texture {
  SDL_Texture* sdlTexture = nullptr;
  std::string filePath = "";
};

class TextureManager {
 public:
  enum TextureName {
    MENU_BACKGROUND,
    MENU_BUTTON_BACKGROUND,
    GAME_BACKGROUND,
    SPACE_BAR_ICON,
    CARD_PLUS_1,
    CARD_PLUS_2,
    CARD_PLUS_3,
    CARD_TIMES_2,
    CARD_TIMES_3,
    CARD_ECHO,
    CARD_WORM,
  };

 private:
  GlobalState* state = nullptr;
  std::unordered_map<TextureName, Texture> textureCache = {
      {MENU_BACKGROUND, Texture{nullptr, "menu-background.png"}},
      {MENU_BUTTON_BACKGROUND, Texture{nullptr, "menu-button-texture.png"}},
      {GAME_BACKGROUND, Texture{nullptr, "game-background.png"}},
      {SPACE_BAR_ICON, Texture{nullptr, "space-bar.png"}},
      {CARD_PLUS_1, Texture{nullptr, "card+1.png"}},
      {CARD_PLUS_2, Texture{nullptr, "card+2.png"}},
      {CARD_PLUS_3, Texture{nullptr, "card+3.png"}},
      {CARD_TIMES_2, Texture{nullptr, "cardx2.png"}},
      {CARD_TIMES_3, Texture{nullptr, "cardx3.png"}},
      {CARD_ECHO, Texture{nullptr, "card-echo.png"}},
      {CARD_WORM, Texture{nullptr, "card-worm.png"}},
  };
  SDL_Texture* loadTexture(TextureName textureName);

 public:
  TextureManager(GlobalState* state);
  ~TextureManager();
  SDL_Texture* getTexture(TextureName name);
};