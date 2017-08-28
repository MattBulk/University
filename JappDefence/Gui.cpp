#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "AssetManager.h"
#include "Constants.h"
#include "GameBuild.h"
#include "Gui.h"

sf::Sprite guiVector[GConst::GUI_DIM];
AssetManager assetMan;

/**carico i font, il background e */

void loadDesiredFont() {

  assetMan.loadFont("assets/AirmoleStripe.ttf");
}

/**carico il background */

void loadBackground() {

  assetMan.loadTexture("background", "assets/background.png");

  guiVector[0].setTexture(assetMan.getRef("background"));

}

/**carico gli elementi della gui */

void loadGuiElementes() {

  assetMan.loadTexture("playBtn", "assets/PlayBtn.png");

  //setto le posisioni degli oggetti in maniera dinamica
  guiVector[1].setTexture(assetMan.getRef("playBtn"));
  guiVector[1].setOrigin(sf::Vector2f(64, 64));
  guiVector[1].setScale(.5, .5);
  guiVector[1].setPosition(sf::Vector2f(GConst::STAGE_WIDTH * .92, GConst::STAGE_HEIGHT * .37));

  assetMan.loadTexture("pauseBtn", "assets/PauseBtn.png");

  guiVector[2].setTexture(assetMan.getRef("pauseBtn"));
  guiVector[2].setOrigin(sf::Vector2f(64, 64));
  guiVector[2].setScale(.5, .5);
  guiVector[2].setPosition(sf::Vector2f(GConst::STAGE_WIDTH * .92, GConst::STAGE_HEIGHT * .47));

  assetMan.loadTexture("newBtn", "assets/StartBtn.png");

  guiVector[3].setTexture(assetMan.getRef("newBtn"));
  guiVector[3].setPosition(sf::Vector2f(GConst::STAGE_WIDTH * .05, GConst::STAGE_HEIGHT * .85));

  assetMan.loadTexture("endScreen", "assets/EndScreen.png");

  //setto le posizioni degli oggetti in maniera statica
  guiVector[4].setTexture(assetMan.getRef("endScreen"));
  guiVector[4].setPosition(sf::Vector2f(100, 300));

  assetMan.loadTexture("nextBtn", "assets/NextBtn.png");

  guiVector[5].setTexture(assetMan.getRef("nextBtn"));
  guiVector[5].setPosition(sf::Vector2f(280, 500));

  assetMan.loadTexture("replayBtn", "assets/ReplayBtn.png");

  guiVector[6].setTexture(assetMan.getRef("replayBtn"));
  guiVector[6].setPosition(sf::Vector2f(280, 500));

}

/**
* LoadGameText ritorna una istanza di test passando d
* \param str stringa con il testo
* \param dimensions grandezza del font
* \param pos positione del testo
* \return una istanza di testo
*/

sf::Text loadGameText(const std::string & str, int dimensions, sf::Vector2f pos) {

  sf::Text text;
  text.setFont(assetMan.theFont);
  text.setString(str);
  text.setCharacterSize(dimensions);
  text.setPosition(pos);
  text.setColor(sf::Color::Yellow);
  //aggiungere colore
  return text;

}

sf::Text levelText = loadGameText("Level:", 36, sf::Vector2f(0, 750));

sf::Text aliensText = loadGameText("Aliens:", 36, sf::Vector2f(320, 750));

sf::Text bombsText = loadGameText("Bombs:", 36, sf::Vector2f(490, 750));

sf::Text endScreenText = loadGameText(" ", 65, sf::Vector2f(150, 400));

/**
* devo disegnare tutte le istanze a runtime
* \param app: prendo come rifermento il RenderWindow della Main Class
*/

void drawGuiSiblings(sf::RenderWindow & app) {

  if(NEW_GAME) {
    app.draw(levelText);
    app.draw(aliensText);
    app.draw(bombsText);
    app.draw(guiVector[1]);
    app.draw(guiVector[2]);
  }
  else app.draw(guiVector[3]);

  if(GAME_STATE == "DIE")  {
    app.draw(guiVector[4]);
    app.draw(endScreenText);
    app.draw(guiVector[6]);
  }

  if(GAME_STATE == "WIN" || GAME_STATE == "END")  {
    app.draw(guiVector[4]);
    app.draw(endScreenText);
    app.draw(guiVector[5]);
  }
}

void drawBackGround(sf::RenderWindow & app) {

  app.draw(guiVector[0]);
}

/**
* update dello score
* \param v: valore di intero
*/

void updateLevel(int v) {

  std::string str = "Level: " + std::to_string(v);

  levelText.setString(str);
}

/**
* update dei nemici uccisi
* \param e: decremento di volta in volta
*/

void updateAliens(int e) {

  std::string str = "Aliens: " + std::to_string(e);

  aliensText.setString(str);
}

/** update delle bombe da poter usare */

void updateBombs(int b) {

  std::string str = "Bombs: " + std::to_string(b);

  bombsText.setString(str);
}

/** update del messaggio di fine livello */

void updateEndScreen(std::string str) {

  endScreenText.setString(str);
}
