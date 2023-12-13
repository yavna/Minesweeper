#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

using namespace std;

class TextureManager
{
    //static == one and only one of these in memory, ever
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string textureName); 

public:
    static sf::Texture& GetTexture(string textureName);
    static void Clear(); // Call this once, at the end of main()
};
