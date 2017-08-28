#include "Constants.h"

extern sf::Sprite guiVector[GConst::GUI_DIM];
extern AssetManager assetMan;
//extern sf::Font theFont; LA RENDO PRIVATA

//queste sono le funzioni avrei potuto usare una classe ma ho optato per questa versione per rimanere fedele al corso

void loadBackground();

void loadGuiElementes();

void loadDesiredFont();

void drawGuiSiblings(sf::RenderWindow &App);
void drawBackGround(sf::RenderWindow &App);

void updateLevel(int v);

void updateAliens(int a);

void updateBombs(int b);

void updateEndScreen(std::string str);

sf::Text loadGameText(const std::string & str, int dimensions,  sf::Vector2f pos);
