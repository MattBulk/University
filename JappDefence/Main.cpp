#include <SFML/Graphics.hpp>
#include <iostream>
#include "AssetManager.h"
#include "Gui.h"
#include "GameBuild.h"
#include "GameLoop.h"
#include "PathFinder.h"
#include "Constants.h"

using namespace std;

/** instanza del framework usato per giocare */
sf::RenderWindow App;

void init();

/**
* nel main il loop while cattura tutti gli eventi del mouse e della tastiera e gli smista ai vari moduli
* possiamo pensarlo come il cervello del programma, funge da modulo eventi 
*/

int main()
{
    App.create(sf::VideoMode(GConst::STAGE_WIDTH, GConst::STAGE_HEIGHT), "JAPP DEFENCE", sf::Style::Close | sf::Style::Titlebar); //no resize

    App.setFramerateLimit(60);

    init();

    GAME_STATE = "IDLE";

    std::string keyType;

    while (App.isOpen())
    {
        sf::Event event;

        while (App.pollEvent(event))
        {
            switch (event.type) {

              case sf::Event::Closed:

              App.close();
              break;

              case sf::Event::MouseButtonPressed:

              if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

                sf::Vector2i position = sf::Mouse::getPosition(App);

                if(guiVector[1].getGlobalBounds().contains(position.x, position.y)) {
                  GAME_STATE = "PLAY";
                  guiVector[1].setScale(.4, .4);
                  guiVector[2].setScale(.5, .5);
                }
                if(guiVector[2].getGlobalBounds().contains(position.x, position.y)) {
                  GAME_STATE = "PAUSE";
                  guiVector[2].setScale(.4, .4);
                  guiVector[1].setScale(.5, .5);
                }

                if(guiVector[3].getGlobalBounds().contains(position.x, position.y)) NEW_GAME = true;

                if(guiVector[5].getGlobalBounds().contains(position.x, position.y)) {

                  if(GAME_STATE == "DIE") {
                    resetLevel();
                    createPaths();
                    GAME_STATE = "IDLE";
                    guiVector[1].setScale(.5, .5);
                  }
                }

                if(guiVector[6].getGlobalBounds().contains(position.x, position.y)) {

                  if(GAME_STATE == "WIN") CURRENT_LEVEL++;

                  if(GAME_STATE == "END") {
                    NEW_GAME = false;
                    GAME_STATE == "IDLE";
                    CURRENT_LEVEL = 1;
                  }

                  updateLevel(CURRENT_LEVEL);
                  resetLevel();
                  createGameTable();
                  graphBuilder();
                  createPaths();
                  GAME_STATE = "IDLE";
                  guiVector[1].setScale(.5, .5);

                }
              }
              break;

              case sf::Event::KeyPressed:

              if(GAME_STATE != "PLAY") break;
              if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) keyType = "left";
              else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) keyType = "right";
              else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) keyType = "up";
              else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) keyType = "down";
              else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) keyType = "setBomb";
              else if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)) keyType = "getBomb";
              break;

              case sf::Event::KeyReleased:
              getInput(keyType);
              keyType = "NULL";
              break;
            }
        }

        App.clear();

        drawBackGround(App);

        if(NEW_GAME) {

          drawGameSiblings(App);

          if(GAME_STATE != "DIE" && GAME_STATE != "WIN") updateGame();
        }

        drawGuiSiblings(App);

        App.display();
    }
    return 0;
}

/**
* carico la gui e costruisco gli oggetti del gioco
* fanno parte del moduli Gui, GameBuild, PathFinder
*/

void init() {

  CURRENT_LEVEL = 1;
  loadBackground();
  loadGuiElementes();
  loadDesiredFont();
  loadGameAssets();

  loadLevel(CURRENT_LEVEL);

  createGameTable();
  createBombs();
  createHero();
  createAliens();

  updateLevel(CURRENT_LEVEL);

  graphBuilder();
  createPaths();

}
