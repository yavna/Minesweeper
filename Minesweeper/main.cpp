#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "board.h"
#include "tile.h"
#include "texture.h"
using namespace std;

int main()
{
    // Reading config file
    ifstream configFile("boards/config.cfg");
    int cols = 0;
    int rows = 0;
    int numMines = 0;

    if (configFile.is_open()) {
        configFile >> cols >> rows >> numMines;
        configFile.close();
    }
    else {
        cout << "Failed to open config file!" << endl;
    }

    int width = cols * 32;
    int height = (rows * 32) + 100;

    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);

    // Initializing board
    Board board(rows, cols, numMines);

    while (window.isOpen()) // DO EVERYTHING IN THIS WHILE LOOP
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();  // Keep this
            }

            window.clear(sf::Color::White); // Drawings below here

            board.HandleLeftClick(window);
            board.HandleRightClick(window);

            window.display(); // Drawings above here
        }
    }
    return 0;
}