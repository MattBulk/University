#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include "AssetManager.h"
#include "Gui.h"
#include "Constants.h"
#include "GameLoop.h"

using namespace std;

/**
* la struttura contiene la sprite alienPath
* il vettore per contenere il percorso di dijkstra
* killed se alieno morto
* v_counter ha il counter della lunghezza del vettore
*/
struct alien {
  sf::Sprite theAlien;
  std::vector<sf::Vector2f> alienPath;
  bool killed;
  uint v_counter;
};

/**
* bomb ha la sprite e la boolean per tracciare esplosione della bomba
*/

struct bomb {
  sf::Sprite theBomb;
  bool on;
};

/** matrice statica che contiere la mappa delle texture delle tile*/
sf::Sprite gameTable[GConst::H_DIM][GConst::W_DIM];
/** matrice usata per contenere il tipo di tile usata per generare la matrice di adiacenza nel modulo PathFinder */
int levelMatrix[GConst::H_DIM][GConst::W_DIM];
/** array Dinamico per contenere un numero x di alieni*/
alien * alienArray;
/** array dimanico che tiene le posizioni iniziali degli alieni */
sf::Vector2f * alienPosition;
/** dimesione dell'array dinamico */
int alienDim;
/** array statico per contenere le bombe */
bomb bombArr[GConst::B_DIM];
/** variabile globale mantiene lo stato corrente del gioco */
std::string GAME_STATE;
/** variabile globale per accesso ad una nuova partita */
bool NEW_GAME;
/** variabile globale mantiene il livello di gioco */
int CURRENT_LEVEL;
/** la sprite contenitore eroe */
sf::Sprite theHero;

/** carico tutte le texture nella fase di init */

void loadGameAssets() {

  assetMan.loadTexture("tile0", "assets/Tile0.png");
  assetMan.loadTexture("tile1", "assets/Tile1.png");
  assetMan.loadTexture("tile2", "assets/Tile2.png");
  assetMan.loadTexture("hero", "assets/Hero4Anim.png");
  assetMan.loadTexture("alien1", "assets/AlienType1.png");
  assetMan.loadTexture("bomb", "assets/NewBomb.png");
  assetMan.loadTexture("poof", "assets/Poof.png");

}

/** creo il personaggio del gioco */

void createHero() {

  theHero.setTexture(assetMan.getRef("hero"));
  theHero.setScale(.5, .5);
  theHero.setTextureRect(sf::IntRect(0, 0, 64, 64));

  float posX = GConst::TILE_SIZE * (GConst::W_DIM/2 );
  float posY = GConst::H_OFFSET + GConst::TILE_SIZE * int(GConst::H_DIM/2);
  theHero.setPosition(sf::Vector2f(posX, posY));

}

/** creo gli alieni: setto texture scaling e la posizione sulla gameTable */

void createAliens() {

  alienArray = new alien[alienDim];

  for (int i = 0; i < alienDim; i++) {

    alienArray[i].theAlien.setTexture(assetMan.getRef("alien1"));
    alienArray[i].theAlien.setScale(.5, .5);
    alienArray[i].theAlien.setTextureRect(sf::IntRect(0, 0, 64, 64));
    alienArray[i].theAlien.setPosition(alienPosition[i].x, alienPosition[i].y + GConst::H_OFFSET);
    alienArray[i].killed = false;
    alienArray[i].v_counter = 0;
  }

  updateAliens(alienDim);

}

/** creo bombe: setto texture scaling e boolean on a false */

void createBombs() {

  for (int i = 0; i < GConst::B_DIM; i++) {

    bombArr[i].theBomb.setTexture(assetMan.getRef("bomb"));
    bombArr[i].theBomb.setScale(.5, .5);
    bombArr[i].on = false;
  }

  updateBombs(GConst::B_DIM);
}

/**
* carico una matrice, rappresenta i nodi del grafo, da un file di testo
* \param l: prende il livello di gioco
*/

void loadLevel(int l) {

  CURRENT_LEVEL = l;

  std::string level_string = "assets/Level" + std::to_string(l) + ".txt";

	ifstream fo(level_string);

  if(!fo) cerr<<"errore";

  for (int i = 0; i < GConst::H_DIM; ++i) {

    for (int j = 0; j < GConst::W_DIM; ++j) fo>>levelMatrix[i][j];

	}

  fo>>alienDim;

  alienPosition = new sf::Vector2f[alienDim];

  for (int p = 0; p < alienDim; p++) {
    fo>>alienPosition[p].x;
    alienPosition[p].x *= GConst::TILE_SIZE;
    fo>>alienPosition[p].y;
    alienPosition[p].y *= GConst::TILE_SIZE;
  }

	fo.close();

  setGameLoopVars();

}

/**
* carico le texture delle tile grazie alla matrice dei nodi e creo le Sprites
*
*/

void createGameTable() {

  int height, width;
  std::string tileName;

  for (int i = 0; i < GConst::H_DIM; ++i) {

    height = i * GConst::TILE_SIZE;

    for (int j = 0; j < GConst::W_DIM; ++j) {

      width = j * GConst::TILE_SIZE;

      tileName = "tile" + std::to_string(levelMatrix[i][j]);

      gameTable[i][j].setTexture(assetMan.getRef(tileName));
      gameTable[i][j].setScale(.5, .5);
      gameTable[i][j].setPosition(sf::Vector2f(width, height + GConst::H_OFFSET));
		}
	}
}

/**
* devo disegnare tutte le istanze a runtime
* \param app: prendo come rifermento il RenderWindow della Main Class
*/

void drawGameSiblings(sf::RenderWindow & app) {

  for (int i = 0; i < GConst::H_DIM; ++i) {

    for (int j = 0; j < GConst::W_DIM; ++j) app.draw(gameTable[i][j]);

	}

  for (int a = 0; a < alienDim; a++) {

    app.draw(alienArray[a].theAlien);
  }

  for (int b = 0; b < GConst::B_DIM; b++) {

    if(bombArr[b].on == true) app.draw(bombArr[b].theBomb);
  }

  app.draw(theHero);

}

/* resetto il livello per giocarlo di nuovo */

void resetLevel() {

  delete[] alienPosition;
  delete[] alienArray;
  loadLevel(CURRENT_LEVEL);

  createBombs();
  createHero();
  createAliens();
}
