#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "Various.h"

//Textures
std::map<std::string, sf::Texture> mTextures;

#include "CSprite.h"
#include "GameEngine.h"

//global variables
GameEngine *pGame;
CSprite *spgolfballsprite[3];
CSprite *spforest;
bool dragging;
int  dragball;
Rnd rnd;

bool GameInitialize()
{
    //create the game engine
    pGame = new GameEngine("Fore", 600, 400);
    if(pGame == nullptr) return false;

    //set the frame rate
    pGame->SetFrameRate(60);

    return true;
}

void GameStart()
{
    //load the textures
    pGame->loadTexture("forest","forest.png");
    pGame->loadTexture("golfball","golfball.png");

    //create the golf ball sprites
    spforest = new CSprite(mTextures["forest"]);

    sf::FloatRect rcBounds(0,0,600,400);
    spgolfballsprite[0] = new CSprite(mTextures["golfball"], rcBounds);
    spgolfballsprite[1] = new CSprite(mTextures["golfball"], rcBounds, BA_WRAP);
    spgolfballsprite[2] = new CSprite(mTextures["golfball"], rcBounds, BA_BOUNCE);

    spgolfballsprite[0]->SetVelocity(20,10);
    spgolfballsprite[1]->SetVelocity(30,-20);
    spgolfballsprite[2]->SetVelocity(70,40);

    spgolfballsprite[0]->SetPosition(20,20);
    spgolfballsprite[1]->SetPosition(100,20);
    spgolfballsprite[2]->SetPosition(20,100);

    pGame->AddSprite(spgolfballsprite[0]);
    pGame->AddSprite(spgolfballsprite[1]);
    pGame->AddSprite(spgolfballsprite[2]);

    //set the initial drag info
    dragging = false;
    dragball = -1;
}

void GameEnd()
{
    //cleanup the game engine
    pGame->CleanupSprites();
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
    //draw the background forest
    spforest->Draw(window);

    //draw the golf ball sprites
    pGame->DrawSprites(window);

    window.display();
}

void GameCycle(sf::Time delta)
{
    //update the golf ball sprites
    pGame->UpdateSprites(delta);

    //check if a ball was clicked with the left mouse button
    if( pGame->mouseClicked && pGame->mouseLeftButton && !dragging)
    {
        int x = pGame->mousePos.x;
        int y = pGame->mousePos.y;

        for(int i=0;i<3;i++)
        {
            if(spgolfballsprite[i]->IsPointInside(x,y))
            {
                //set the drag state and the drag ball
                dragging = true;
                dragball = i;
                //don't check for more balls
                break;
            }
        }
    }

    if( !pGame->mouseLeftButton && dragging ) dragging = false;

    if( pGame->mouseLeftButton && dragging )
    {
        //move the sprite to the mouse cursor position
        int offx = spgolfballsprite[dragball]->GetWidth() / 2;
        int offy = spgolfballsprite[dragball]->GetHeight() / 2;
        int moux = sf::Mouse::getPosition(pGame->window).x;
        int mouy = sf::Mouse::getPosition(pGame->window).y;
        spgolfballsprite[dragball]->SetPosition(moux - offx, mouy - offy);
    }
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
  // Swap the sprite velocities so that they appear to bounce
  sf::Vector2f ptSwapVelocity = pSpriteHitter->GetVelocity();
  pSpriteHitter->SetVelocity(pSpriteHittee->GetVelocity());
  pSpriteHittee->SetVelocity(ptSwapVelocity);
  return true;
}
