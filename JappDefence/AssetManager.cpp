#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "AssetManager.h"

/**
* carico le texture per le Sprite tramite map
* \param name : nome associato alla texture per caricarla
* \param filename : percorso del file immagine all'interno del progetto
*/

void AssetManager::loadTexture(const std::string & name, const std::string & filename) {

    //carico la texture
    sf::Texture tex;
    tex.loadFromFile(filename);
    tex.setSmooth(true);

    this -> textures[name] = tex;
}

/**
* richiamo una texture tramite nome
* \param texture : stringa nome del file da mettere
* \return una texture da inserire nella sprite
*/

sf::Texture & AssetManager::getRef(const std::string & texture) {

    return this->textures.at(texture);
}

/**carica un font da una string */

void AssetManager::loadFont(const std::string & name) {

    theFont.loadFromFile(name);
}
