#include <string>
#include <iostream>
#include "AssetManager.h"
#include "GameBuild.h"
#include "GameLoop.h"
#include "Gui.h"
#include "Constants.h"
#include <stdlib.h>
#include <time.h>

using namespace std;
/** costante per il calcolo dei nodi della matrice di adiacenza */
const int WH_DIM = GConst::W_DIM * GConst::H_DIM;
/** array delle distanze di dijkstra */
int dist[WH_DIM];
/** contiene il cammino breve */
int parentPath[WH_DIM];
/** array dei nodi visitati */
bool done[WH_DIM];
/** costante infinity settata a valore alto */
const int MAX = 999;

/** array degli span delle righe */
int delta_rows[4] = {-1, 1, 0, 0};
/** array degli span delle colonne */
int delta_cols[4] = {0, 0, -1, 1};

/** array della matrice di adiacenza */
int theGraph[WH_DIM][WH_DIM];

void printGrafo();
void dijkstra(int s);
void translateToRowsAndColums(int v, int a);
void printPath(int p[], int j, int a);
void printAliens(int w);

/**semplice inizialiazzazione a zero della matrice di agiacenza */

void initG() {

  for (int i = 0; i < WH_DIM; ++i) {

    for (int j = 0; j < WH_DIM; ++j) theGraph[i][j] = -1;

	}
}

/*
* questo metodo controlla se mi sono spostato su una tile valida
* \param row: righe
* \param col: colonna
* \return true se posizione valida
*/

bool is_valid(int row, int col) {

  if(row < 0 || row >= GConst::H_DIM) return false;
  if(col < 0 || col >= GConst::W_DIM) return false;
  return true;
}

/*
* riempio la matrice di adiacenza, per il peso dei rami lo ottengo random per movimentare
il movimento degli alieni durante il game play
*
*/

void graphBuilder() {

  initG();

  srand(time(NULL));

  int vertex = 0;
  int edge = 0;
  int randomEdge = 0;

  for (int i = 0; i < GConst::H_DIM; ++i) {

    for (int j = 0; j < GConst::W_DIM; ++j) {
      if(levelMatrix[i][j] != 0) {
        for (size_t m = 0; m < 4; m++) {
          int new_row = i + delta_rows[m];
          int new_col = j + delta_cols[m];
          if(is_valid(new_row, new_col) && levelMatrix[new_row][new_col] != 0) {
            //calcola edge
            if(delta_rows[m] == -1 && delta_cols[m] == 0) edge = vertex - GConst::W_DIM;
            else if (delta_rows[m] == 1 && delta_cols[m] == 0) edge = vertex + GConst::W_DIM;
            else if (delta_rows[m] == 0 && delta_cols[m] == -1) edge = vertex - 1;
            else if (delta_rows[m] == 0 && delta_cols[m] == 1) edge = vertex + 1;
            //posso creare valori random fra 0 e 10 tanto per movimentare
            randomEdge = rand() % 100 + 1;

            theGraph[vertex][edge] = randomEdge;
          }
        }
        vertex++;
      }
      else vertex++;
    }
	}

}

/*
* creo ogni path per ogni alien
*
*/

void createPaths() {

  for (int a = 0; a < alienDim; a++) {

    if(alienArray[a].killed == false) {

      alienArray[a].alienPath.clear();
      alienArray[a].v_counter = 0;

      dijkstra(objectPositionToVertex(alienArray[a].theAlien.getPosition()));
      printPath(parentPath, objectPositionToVertex(theHero.getPosition()), a);

    }
  }
}

/**
* implemento algoritmo visto a lezione ma nella versione matrice di adiacenza
*
*/

void dijkstra(int s) {

  for (int i = 0; i < WH_DIM; i++) {

    dist[i] = MAX;
    parentPath[s] = 0;
    done[i] = false;

  }

  parentPath[s] = -1;
  dist[s] = 0;

  while (true) {
    int u = 1;
    int bestDist = MAX;
    for(int i = 0; i < WH_DIM; i++) if(!done[i] && dist[i] < bestDist) {
      u = i;
      bestDist = dist[i];
    }
    if(bestDist == MAX) break;
    // relax degli edge vicini
    for(int v = 0; v < WH_DIM; v++) {

      if(!done[v] && theGraph[u][v] != -1 && dist[v] > dist[u] + theGraph[u][v]) {

          dist[v] = dist[u] + theGraph[u][v];
          parentPath[v] = u;
      }
    }
    done[u] = true;
  }
}

/** stampa ogni nodo del percorso */

void printPath(int p[], int j, int a) {

  if (p[j] == -1) return;

  printPath(p, p[j], a);
  translateToRowsAndColums(j, a);

}

/**
* trasformo il vertice nelle sue coordinate x ed y rispetto alla matrice
*
*/

void translateToRowsAndColums(int v, int a) {

  int vertex, row, col;

  vertex = v;

  row = vertex / GConst::W_DIM;
  col = vertex - GConst::W_DIM * row;

  sf::Vector2f posAlien;

  posAlien.x = col * GConst::TILE_SIZE;
  posAlien.y = row * GConst::TILE_SIZE;

  alienArray[a].alienPath.push_back(posAlien);

}

//
//
//
//FUNZIONI DEBUG

void printAliens(int w) {

  cout<<endl;

  for (uint i = 0; i < alienArray[w].alienPath.size(); i++) {

    cout<<alienArray[w].alienPath[i].x<<" "<<alienArray[w].alienPath[i].y<<endl;

  }
}

void printGrafo() {

  for (int i = 0; i < WH_DIM; ++i) {
    cout <<"VERTICE: "<<i<<" ";

    for (int j = 0; j < WH_DIM; ++j) cout<<theGraph[i][j];

    cout<<endl;

	}
}
