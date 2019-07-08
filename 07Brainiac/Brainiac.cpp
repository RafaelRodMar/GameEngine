#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <SFML/Graphics.hpp>
#include "Texture.h"
#include "GameEngine.h"

//global variables
GameEngine *pGame;
Texture texts[9];
sf::Sprite image[9];
bool TileState[4][4];
int Tile[4][4];
sf::Vector2i Tile1, Tile2;
int matches, tries;


bool GameInitialize()
{
    pGame = new GameEngine("Brainiac", 528, 508);
    if(pGame == nullptr) return false;

    pGame->SetFrameRate(15);

    return true;
}

void GameStart()
{
    srand(time(NULL));

    //load textures
    texts[0].load("img/tileblank.png");
    texts[1].load("img/tile1.png");
    texts[2].load("img/tile2.png");
    texts[3].load("img/tile3.png");
    texts[4].load("img/tile4.png");
    texts[5].load("img/tile5.png");
    texts[6].load("img/tile6.png");
    texts[7].load("img/tile7.png");
    texts[8].load("img/tile8.png");

    //assign textures to sprites
    for(int i=0;i<9;i++)
        image[i].setTexture(texts[i].texture);

    //clear the tile states and images
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
        {
            TileState[i][j] = false;
            Tile[i][j] = 0;
        }

    //initialize the tile iamges randomly
    for(int i=0;i<2;i++)
        for(int j=0;j<9;j++)
        {
            int x = rand() % 4;
            int y = rand() % 4;
            while(Tile[x][y] != 0)
            {
                x = rand() % 4;
                y = rand() % 4;
            }
            Tile[x][y] = j;
        }

    //initialize the tile selections and match/try count
    Tile1.x = Tile1.y = -1;
    Tile2.x = Tile2.y = -1;
    matches = tries = 0;
}

void GameEnd()
{
    //cleanup
    pGame->window.close();
    delete pGame;
}

void GameActivate()
{

}

void GameDeactivate()
{

}

void GamePaint(sf::RenderWindow &window)
{
    //draw the tiles
    int TileWidth = texts[0].GetWidth();
    int TileHeight = texts[0].GetHeight();
    for(int i = 0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(TileState[i][j] || ((i == Tile1.x) && (j == Tile1.y)) ||
               ((i == Tile2.x) && (j== Tile2.y)))
            {
                image[Tile[i][j]].setPosition(i * TileWidth, j * TileHeight);
                window.draw(image[Tile[i][j]]);
            }
            else
            {
                image[0].setPosition(i * TileWidth, j * TileHeight);
                window.draw(image[0]);
            }
        }
    }
    window.display();
}

void GameCycle(sf::Time delta)
{
    if( pGame->mouseClicked && pGame->mouseLeftButton )
    {
        int x = pGame->mousePos.x;
        int y = pGame->mousePos.y;

        //determine which tile was clicked
        int Tilex = x / texts[0].GetWidth();
        int Tiley = y / texts[0].GetHeight();

        //make sure the tile hasn't already been matched
        if( !TileState[Tilex][Tiley] )
        {
            //see if this is the first tile selected
            if( Tile1.x == -1 )
            {
                //set the first tile selection
                Tile1.x = Tilex;
                Tile1.y = Tiley;
            }
            else if((Tilex != Tile1.x) || (Tiley != Tile1.y))
            {
                if( Tile2.x == -1)
                {
                    //increase the number of tiles
                    tries++;

                    //set the second tile selection
                    Tile2.x = Tilex;
                    Tile2.y = Tiley;

                    //see if it's a match
                    if( Tile[Tile1.x][Tile1.y] == Tile[Tile2.x][Tile2.y])
                    {
                        //set the tile state to indicate the match
                        TileState[Tile1.x][Tile1.y] = true;
                        TileState[Tile2.x][Tile2.y] = true;

                        //clear the tile selections
                        Tile1.x = Tile1.y = Tile2.x = Tile2.y = -1;

                        //update the match count and check for winner
                        if(++matches == 8)
                        {
                            //text
                        }
                    }
                }
                else
                {
                    //clear the tile selections
                    Tile1.x = Tile1.y = Tile2.x = Tile2.y = -1;
                }
            }
        }
    }
    pGame->mouseClicked = false;
    pGame->mouseLeftButton = false;
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, bool bLeft)
{

}

void MouseButtonUp(int x, int y, bool bLeft)
{

}

void MouseMove(int x, int y)
{

}
