#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "AssetManager.h"
#include "GameBuild.h"
#include "PathFinder.h"
#include "Gui.h"
#include "Constants.h"

using namespace std;

/** timer che fa avanzare il gioco */
sf::Clock theClock;
/** costante per settare ad ogni quanto il timer deve essere risettato */
const float GATE = .5;
/** vettore per conservare attuale posizione eroe durante il gameplay */
sf::Vector2f theHeroPos;
/** variabili di contro animazione , alieni uccisi e bombe usate*/
int span, aliensKilled, currentBomb;

//creo i prototipi delle funzioni
void setBomb();
void getBomb();
void moveHero(std::string direction);
void checkTile(std::string direction);

/** setto le variabili del modulo GameLoop, devo farlo ad ogni partita */
void setGameLoopVars() {

  currentBomb = GConst::B_DIM - 1;
  span = 0;
  aliensKilled = 0;
}

/** questa funzione gestisce il valore di tastiera e lo smista
* \param input: valore della stringa
*/
void getInput(std::string input) {

  if(input == "setBomb") {
    if(currentBomb >= 0) setBomb();
  }
  else if(input == "getBomb") {
    if(currentBomb < GConst::B_DIM) getBomb();
  }
  else moveHero(input);
}

/** setto la bomba */
void setBomb() {

  theHeroPos = theHero.getPosition();
  bombArr[currentBomb].theBomb.setPosition(theHeroPos);
  bombArr[currentBomb].on = true;
  currentBomb--;
  updateBombs(1 + currentBomb);

}

/** prendo la bomba */
void getBomb() {

  for (int i = 0; i < GConst::B_DIM; i++) {

    if(theHero.getGlobalBounds().intersects(bombArr[i].theBomb.getGlobalBounds()) && bombArr[i].on) {

      bombArr[i].on = false;
      currentBomb++;
      updateBombs(1 + currentBomb);
    }
  }
}

/**
* muovo eroe nelle quattro direzioni controllando di non superare uscire dallo stage
* la chiamata a createPaths non fa altro che l'update del cammino di dijkstra
* \param direction: valore delle quattro direzioni
*/
void moveHero(std::string direction) {

  theHeroPos = theHero.getPosition();

  if(direction == "left") {

    if(theHeroPos.x > 0) {
      theHero.move(-GConst::TILE_SIZE, 0);
      theHero.setTextureRect(sf::IntRect(192, 0, 64, 64));
    }
  }
  else if(direction == "right") {

    if(theHeroPos.x < GConst::TILE_SIZE * (GConst::W_DIM - 1)) {
      theHero.move(GConst::TILE_SIZE, 0);
      theHero.setTextureRect(sf::IntRect(128, 0, 64, 64));
    }
  }
  else if(direction == "up") {

    if(theHeroPos.y > GConst::H_OFFSET) {
      theHero.move(0, -GConst::TILE_SIZE);
      theHero.setTextureRect(sf::IntRect(64, 0, 64, 64));
    }
  }
  else if(direction == "down") {

    if(theHeroPos.y < GConst::H_OFFSET + GConst::TILE_SIZE * (GConst::H_DIM-1)) {
      theHero.move(0, GConst::TILE_SIZE);
      theHero.setTextureRect(sf::IntRect(0, 0, 64, 64));
    }
  }
  checkTile(direction);

  createPaths();
}

/** controllo se eroe puo' andare in una determinata tile*/
void checkTile(std::string direction) {

  for (int i = 0; i < GConst::H_DIM; ++i) {

    for (int j = 0; j < GConst::W_DIM; ++j) {
      //add border ! same way !
      if(theHero.getGlobalBounds().intersects(gameTable[i][j].getGlobalBounds()) && levelMatrix[i][j] == 0) {

        if(direction == "left") theHero.move(GConst::TILE_SIZE, 0);

        else if(direction == "right") theHero.move(-GConst::TILE_SIZE, 0);

        else if(direction == "up") theHero.move(0, GConst::TILE_SIZE);

        else if(direction == "down") theHero.move(0, -GConst::TILE_SIZE);

        break;
      }
		}
	}
}

/** animazione di ogni singola sprite del videoGame */
void animateAliens(int span) {

  for (int a = 0; a < alienDim; a++) {

    if(alienArray[a].killed == false) {

      alienArray[a].theAlien.setTextureRect(sf::IntRect(span, 0, 64, 64));
    }

  }
}

/**
* muovo gli alieni e controllo se gli alieni uccidono eroe.
*/

void moveAliens() {

  for (int a = 0; a < alienDim; a++) {

    if(alienArray[a].killed == false) {

      if(alienArray[a].v_counter < alienArray[a].alienPath.size()) {

        alienArray[a].theAlien.setPosition(alienArray[a].alienPath[alienArray[a].v_counter].x, alienArray[a].alienPath[alienArray[a].v_counter].y + GConst::H_OFFSET);
        alienArray[a].v_counter++;
      }

      if(alienArray[a].theAlien.getGlobalBounds().intersects(theHero.getGlobalBounds())) {

        theHero.setTexture(assetMan.getRef("poof"));
        theHero.setTextureRect(sf::IntRect(0, 0, 64, 64));
        GAME_STATE = "DIE";
      }

    }

  }
}

/**
* controllo se un singolo alieno colpisce una bomba attiva
* se si setto alieno come ucciso e cambio texture per indicare il punto di morte
* se la var win arriva a 0 significa che ho vinto
*/

void checkBombs() {

  for (int a = 0; a < alienDim; a++) {

    if(alienArray[a].killed == false) {

      for (int i = 0; i < GConst::B_DIM; i++) {

        if(alienArray[a].theAlien.getGlobalBounds().intersects(bombArr[i].theBomb.getGlobalBounds()) && bombArr[i].on) {

          alienArray[a].killed = true;
          alienArray[a].theAlien.setTexture(assetMan.getRef("poof"));
          alienArray[a].theAlien.setTextureRect(sf::IntRect(0, 0, 64, 64));
          bombArr[i].on = false;
          aliensKilled++;

          int win = alienDim - aliensKilled;
          updateAliens(win);
          if(win == 0) GAME_STATE = "WIN";
        }
      }
    }
  }
}

/**
* questa funzione tramita una posizione x ed y un un vertice del grafo
* \param objPos vettore con due float
* \return vertex un int vertice della matrice di adiacenza
*/

int objectPositionToVertex(sf::Vector2f objPos) {

  objPos.x = objPos.x / GConst::TILE_SIZE;
  objPos.y = (objPos.y - GConst::H_OFFSET) / GConst::TILE_SIZE;

  int vertex = objPos.y * GConst::W_DIM + objPos.x;

  return vertex;
}

/**
* la funzione cuore, il loop in cui ad un preciso tick vengono eseguite le funzioni del modulo GameLoop, Gui
*/

void updateGame() {

  if(GAME_STATE == "PLAY") {

    if(theClock.getElapsedTime().asSeconds() > GATE) {

      moveAliens();

      checkBombs();

      if(span == 320) span = 0;
      else {
        animateAliens(span);
        span += 64;
      }

      theClock.restart();
    }
  }

  if(GAME_STATE == "DIE") {

    updateEndScreen("YOU DIED !!");

  }

  if(GAME_STATE == "WIN") {

    if(CURRENT_LEVEL == 3) {
      updateEndScreen("END GAME !!!");
      GAME_STATE = "END";
    }
    else updateEndScreen("YOU WIN !!!");

  }

}
