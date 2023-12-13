#include "texture.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
    string path = "images/";
    path += fileName;

    textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName)
{
    // if the texture DOESN'T exist...
    if (textures.find(textureName) == textures.end()) {
        LoadTexture(textureName);
    }

    return textures[textureName];

}

void TextureManager::Clear()
{
    textures.clear(); // get rid of all stored objects
}