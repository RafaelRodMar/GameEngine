#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Various.h"

//Textures
std::map<std::string, sf::Texture> mTextures;
//fonts
std::map<std::string, sf::Font> mFonts;
//Sound Buffers
std::map<std::string, sf::SoundBuffer> mSoundBuffers;
//Sounds
std::map<std::string, sf::Sound> mSounds;


#include "CSprite.h"
#include "GameEngine.h"

//global variables
GameEngine *pGame;
CSprite* image[9];
bool TileState[4][4];
int Tile[4][4];
sf::Vector2i Tile1, Tile2;
int matches, tries;
bool gameover = false;
Rnd rnd;

bool GameInitialize()
{
    pGame = new GameEngine("Brainiac 2",528,508);
    if(pGame == nullptr) return false;

    pGame->SetFrameRate(15);

    return true;
}

void GameStart()
{
    //load textures
    int c = 0;
    pGame->loadTexture(std::to_string(c),"assets/img/tileblank.png");
    c++;
    for(int i=1;i<9;i++)
    {
        pGame->loadTexture(std::to_string(c), "assets/img/tile" + std::to_string(c) + ".png");
        c++;
    }

    //assign textures to sprites
    for(int i=0;i<9;i++)
    {
        CSprite *sp = new CSprite(pGame->getTexture(std::to_string(i)));
        image[i] = sp;
    }

    //clear the tile states and images
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            TileState[i][j] = false;
            Tile[i][j] = 0;
        }
    }

    //initialize the tile images randomly
    for(int i=0;i<2;i++)
        for(int j=0;j<9;j++)
        {
            int x = rnd.getRndInt(0,3);
            int y = rnd.getRndInt(0,3);
            while(Tile[x][y] != 0)
            {
                x = rnd.getRndInt(0,3);
                y = rnd.getRndInt(0,3);
            }
            Tile[x][y] = j;
        }


    //load the sounds
    pGame->loadSoundBuffer("Select", "assets/sounds/select.wav");
    pGame->loadSoundBuffer("Match", "assets/sounds/match.wav");
    pGame->loadSoundBuffer("Mismatch", "assets/sounds/mismatch.wav");
    pGame->loadSoundBuffer("Win", "assets/sounds/win.wav");

    //load the font
    pGame->loadFont("font", "assets/fonts/sansation.ttf");

    //initialize the tile selections and match/try count
    Tile1.x = Tile1.y = -1;
    Tile2.x = Tile2.y = -1;
    matches = tries = 0;
}

void GameEnd()
{
    pGame->CleanupSprites();
    pGame->CleanupTextures();
    pGame->CleanupSounds();
    pGame->CleanupFonts();
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
    int TileWidth = mTextures["0"].getSize().x;
    int TileHeight = mTextures["0"].getSize().y;

    for(int i = 0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(TileState[i][j] || ((i == Tile1.x) && (j == Tile1.y)) ||
               ((i == Tile2.x) && (j== Tile2.y)))
            {
                image[Tile[i][j]]->SetPosition(i * TileWidth, j * TileHeight);
                image[Tile[i][j]]->Draw(window);
            }
            else
            {
                image[0]->SetPosition(i * TileWidth, j * TileHeight);
                image[0]->Draw(window);
            }
        }
    }

    if( gameover == true )
        pGame->Text("You Won in " + std::to_string(tries) + " tries.", 200,150,sf::Color::White,20,"font",window);

    window.display();
}

void GameCycle(sf::Time delta)
{
        if( pGame->mouseClicked && pGame->mouseLeftButton )
    {
        int x = pGame->mousePos.x;
        int y = pGame->mousePos.y;

        //determine which tile was clicked
        int Tilex = x / mTextures["0"].getSize().x;
        int Tiley = y / mTextures["0"].getSize().y;

        //make sure the tile hasn't already been matched
        if( !TileState[Tilex][Tiley] )
        {
            //see if this is the first tile selected
            if( Tile1.x == -1 )
            {
                //play sound for tile selection
                pGame->playSound("Select");

                //set the first tile selection
                Tile1.x = Tilex;
                Tile1.y = Tiley;
            }
            else if((Tilex != Tile1.x) || (Tiley != Tile1.y))
            {
                if( Tile2.x == -1)
                {
                    //play a sound for the tile selection
                    pGame->playSound("Select");

                    //increase the number of tiles
                    tries++;

                    //set the second tile selection
                    Tile2.x = Tilex;
                    Tile2.y = Tiley;

                    //see if it's a match
                    if( Tile[Tile1.x][Tile1.y] == Tile[Tile2.x][Tile2.y])
                    {
                        //play a sound for the tile match
                        pGame->playSound("Match");

                        //set the tile state to indicate the match
                        TileState[Tile1.x][Tile1.y] = true;
                        TileState[Tile2.x][Tile2.y] = true;

                        //clear the tile selections
                        Tile1.x = Tile1.y = Tile2.x = Tile2.y = -1;

                        //update the match count and check for winner
                        if(++matches == 8)
                        {
                            //play a victory sound
                            pGame->playSound("Win");
                            gameover = true;
                        }
                    }
                    else
                    {
                        //play a sound for the tile mismatch
                        pGame->playSound("Mismatch");
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

bool SpriteCollision(CSprite* pSpriteHitter, CSprite* pSpriteHittee)
{
    return true;
}
