#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>

class AssetManager
{
    private:
    /** creo una mappa che mi permette di associare in questo caso una stringa ad una texture */
    std::map<std::string, sf::Texture> textures;

    public:

    /** aggiungo la texture da un file */
    void loadTexture(const std::string & name, const std::string & filename);

    /** ricevo una texture passando un id stringa */
    sf::Texture & getRef(const std::string & texture);

    /**
    * variabile globale della classe font
    * in futuro potrebbe essere un array per contenere maggiori famiglie di fonts
    */
    sf::Font theFont;

    void loadFont(const std::string & name);

    //non potro' cancellare texture ma essendo un piccolo progetto non dovrebbero esservi problemi se carico qualche MB di textures al max

    //inizializzo il Constructor della classe in altri linguaggi si puo' evitare qua non so
    AssetManager() {}

};

#endif
