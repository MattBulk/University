#include "Constants.h"

struct alien {
  sf::Sprite theAlien;
  std::vector<sf::Vector2f> alienPath;
  bool killed;
  uint v_counter;
};

struct bomb {
  sf::Sprite theBomb;
  bool on;
};

extern std::string GAME_STATE;
extern bool NEW_GAME;
extern int CURRENT_LEVEL;

extern int levelMatrix[][GConst::W_DIM];
extern sf::Sprite gameTable[GConst::H_DIM][GConst::W_DIM];
extern sf::Sprite theHero;

extern bomb bombArr[GConst::B_DIM];

extern alien * alienArray;
extern int alienDim;

void createGameTable();
void drawGameSiblings(sf::RenderWindow &App);
void loadGameAssets();
void loadLevel(int l);
void createHero();
void createAliens();
void createBombs();
void resetLevel();
