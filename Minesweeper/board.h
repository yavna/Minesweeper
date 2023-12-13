#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include "tile.h"
#include <map>

using namespace std;

class Board {
private:
    bool isGameOver;
    bool isGameLost;
    int rows;
    int cols;
    int numMines;
    int numTestMines;
    int numCounter;
    int tilesRevealed;
    int testOneMines;
    int testTwoMines;
    int testThreeMines;

    map<std::string, sf::Texture> textures;
    std::vector<std::vector<Tile>> board;

    vector<bool> mines;
    vector<int> testOne;
    vector<int> testTwo;
    vector<int> testThree;

    sf::Sprite debug;
    sf::Sprite test_1;
    sf::Sprite test_2;
    sf::Sprite test_3;
    sf::Sprite face;
    sf::Sprite tileHidden;
    sf::Sprite tileRevealed;
    sf::Sprite mine;
    sf::Sprite number_1;
    sf::Sprite number_2;
    sf::Sprite number_3;
    sf::Sprite number_4;
    sf::Sprite number_5;
    sf::Sprite number_6;
    sf::Sprite number_7;
    sf::Sprite number_8;
    sf::Sprite flag;
    sf::Sprite digits;
    sf::Sprite negative;
    sf::Sprite digit1;
    sf::Sprite digit2;
    sf::Sprite digit3;


public:
    vector<Tile> tiles;
    Board(int rows, int cols, int numMines);
    void LoadTextures();
    void ReadTestFile();
    void PlaceTiles();
    void SetMines();
    void PlaceMines();
    void ShowNumAdjacent();
    void UpdateCounter();
    void ResetCounterWin();
    void Debug();
    void UpdateFace();
    void ShowTestMines(vector<int>& mines, int testboard);

    void HandleLeftClick(sf::RenderWindow& window);
    void HandleRightClick(sf::RenderWindow& window);
    void DrawBoard(sf::RenderWindow& window);
};