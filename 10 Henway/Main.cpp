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
Rnd rnd;
CSprite* pChickenSprite;
int InputDelay;
int NumLives;
int Score;
bool GameOver;
sf::Font font;

void MoveChicken(int iXDistance, int iYDistance);

bool GameInitialize()
{
    pGame = new GameEngine("Henway", 465, 400);
    if(pGame == nullptr) return false;

    pGame->SetFrameRate(30);

    return true;
}

void GameStart()
{
    //load the textures
    pGame->loadTexture("highway","assets/img/highway.png");
    pGame->loadTexture("chicken","assets/img/chicken.png");
    pGame->loadTexture("car0","assets/img/car1.png");
    pGame->loadTexture("car1","assets/img/car2.png");
    pGame->loadTexture("car2","assets/img/car3.png");
    pGame->loadTexture("car3","assets/img/car4.png");
    pGame->loadTexture("chickenhead","assets/img/chickenhead.png");

    // create the chicken and car sprites.
    CSprite* pSprite;
    sf::FloatRect rcBounds(0,0,465,400);

    pSprite = new CSprite(mTextures["chicken"], rcBounds, BA_STOP);
    pSprite->SetPosition(4,175);
    pSprite->SetVelocity(0,0);
    pSprite->SetZOrder(1);
    pSprite->SetName("chicken");
    pGame->AddSprite(pSprite);
    pChickenSprite = pSprite;

    pSprite = new CSprite(mTextures["car0"], rcBounds, BA_WRAP);
    pSprite->SetPosition(70,0);
    pSprite->SetVelocity(0,7);
    pSprite->SetZOrder(2);
    pSprite->SetName("car");
    pGame->AddSprite(pSprite);

    pSprite = new CSprite(mTextures["car1"], rcBounds, BA_WRAP);
    pSprite->SetPosition(160,0);
    pSprite->SetVelocity(0,3);
    pSprite->SetZOrder(2);
    pSprite->SetName("car");
    pGame->AddSprite(pSprite);

    pSprite = new CSprite(mTextures["car2"], rcBounds, BA_WRAP);
    pSprite->SetPosition(239,400);
    pSprite->SetVelocity(0,-5);
    pSprite->SetZOrder(2);
    pSprite->SetName("car");
    pGame->AddSprite(pSprite);

    pSprite = new CSprite(mTextures["car3"], rcBounds, BA_WRAP);
    pSprite->SetPosition(329,400);
    pSprite->SetVelocity(0,-10);
    pSprite->SetZOrder(2);
    pSprite->SetName("car");
    pGame->AddSprite(pSprite);

    //initialize the remaining global variables
    InputDelay = 0;
    NumLives = 3;
    Score = 0;
    GameOver = false;

    font.loadFromFile("assets/img/fnt/sansation.ttf");
}

void GameEnd()
{
    pGame->CleanupSprites();
    pGame->CleanupTextures();
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
    // Draw the background highway
    pGame->showTexture("highway",0,0,window);

    // Draw the sprites
    pGame->DrawSprites(window);

    // Draw the number of remaining chicken lives
    for( int i=0;i<NumLives;i++)
        pGame->showTexture("chickenhead", 406 + mTextures["chickenhead"].getSize().x * i, 382, window);

    window.display();
}

void GameCycle(sf::Time delta)
{
    if(!GameOver)
    {
        //update the sprites
        pGame->UpdateSprites(delta);
    }
}

void HandleKeys()
{
    if(!GameOver)
    {
        //move the chicken based upon the key presses
        if( pGame->KeyPressed(sf::Keyboard::Left) )
            MoveChicken(-20,0);
        else if ( pGame->KeyPressed(sf::Keyboard::Right) )
            MoveChicken(20,0);
        else if ( pGame->KeyPressed(sf::Keyboard::Up) )
            MoveChicken(0,-20);
        else if ( pGame->KeyPressed(sf::Keyboard::Down) )
            MoveChicken(0,20);
    }

    if( pGame->mouseClicked && GameOver)
    {
        NumLives = 3;
        Score = 0;
        GameOver = false;
    }
}

bool SpriteCollision(CSprite* pSpriteHitter, CSprite* pSpriteHittee)
{
    //see if the chicken was hit
    if( pSpriteHittee->GetName() == "chicken" )
    {
        //move the chicken back to the start
        pSpriteHittee->SetPosition(4,175);

        //see if the game is over
        if( --NumLives > 0 )
            //display ouch!
            NumLives = NumLives;
        else
        {
            //display game over message
            GameOver = true;
        }

        return false;
    }
    else
    {
        if(pSpriteHitter->GetName() == "chicken")
        {
            pSpriteHitter->SetPosition(4,175);

            if( --NumLives > 0 )
                NumLives = NumLives;
            else
                GameOver = true;

            return false;
        }
    }



    return true;
}

void MoveChicken(int iXDistance, int iYDistance)
{
    // move the chicken to its new position
    pChickenSprite->OffsetPosition(iXDistance, iYDistance);

    // see if the chicken made it across
    if( pChickenSprite->GetPosition().left > 400 )
    {
        //move the chicken back to the start and add to the score
        pChickenSprite->SetPosition(4,175);
        Score += 150;
    }
}
