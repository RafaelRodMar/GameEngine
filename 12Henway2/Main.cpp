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
//Music
std::map<std::string, sf::Music*> mMusic;


#include "CSprite.h"
#include "GameEngine.h"

//global variables
GameEngine *pGame;
int inputdelay;
int numlives;
int score;
bool gameover;
CSprite* pChickenSprite;
Rnd rnd;

void MoveChicken(int iXDistance, int iYDistance);

bool GameInitialize()
{
    pGame = new GameEngine("Henway 2",465,400);
    if(pGame == nullptr) return false;

    pGame->SetFrameRate(30);

    return true;
}

void GameStart()
{
    //load things
    pGame->openMusic("music", "assets/mus/music.ogg");
    pGame->loadSoundBuffer("bok", "assets/snd/bok.wav");
    pGame->loadSoundBuffer("carhorn1", "assets/snd/carhorn1.wav");
    pGame->loadSoundBuffer("carhorn2", "assets/snd/carhorn2.wav");
    pGame->loadSoundBuffer("celebrate", "assets/snd/celebrate.wav");
    pGame->loadSoundBuffer("gameover", "assets/snd/gameover.wav");
    pGame->loadSoundBuffer("squish", "assets/snd/squish.wav");
    pGame->loadFont("font", "assets/fnt/sansation.ttf");
    pGame->loadTexture("car1", "assets/img/car1.png");
    pGame->loadTexture("car2", "assets/img/car2.png");
    pGame->loadTexture("car3", "assets/img/car3.png");
    pGame->loadTexture("car4", "assets/img/car4.png");
    pGame->loadTexture("chicken", "assets/img/chicken.png");
    pGame->loadTexture("chickenhead", "assets/img/chickenhead.png");
    pGame->loadTexture("highway", "assets/img/highway.png");

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

    pSprite = new CSprite(mTextures["car1"], rcBounds, BA_WRAP);
    pSprite->SetPosition(70,0);
    pSprite->SetVelocity(0,7);
    pSprite->SetZOrder(2);
    pSprite->SetName("car");
    pGame->AddSprite(pSprite);

    pSprite = new CSprite(mTextures["car2"], rcBounds, BA_WRAP);
    pSprite->SetPosition(160,0);
    pSprite->SetVelocity(0,3);
    pSprite->SetZOrder(2);
    pSprite->SetName("car");
    pGame->AddSprite(pSprite);

    pSprite = new CSprite(mTextures["car3"], rcBounds, BA_WRAP);
    pSprite->SetPosition(239,400);
    pSprite->SetVelocity(0,-5);
    pSprite->SetZOrder(2);
    pSprite->SetName("car");
    pGame->AddSprite(pSprite);

    pSprite = new CSprite(mTextures["car4"], rcBounds, BA_WRAP);
    pSprite->SetPosition(329,400);
    pSprite->SetVelocity(0,-10);
    pSprite->SetZOrder(2);
    pSprite->SetName("car");
    pGame->AddSprite(pSprite);

    //initialize the remaining global variables
    inputdelay = 0;
    numlives = 3;
    score = 0;
    gameover = false;

    pGame->playMusic("music", true);
}

void GameEnd()
{
    pGame->stopMusic("music");

    pGame->CleanupSprites();
    pGame->CleanupTextures();
    pGame->CleanupSounds();
    pGame->CleanupMusic();
    pGame->CleanupFonts();
    pGame->window.close();
    delete pGame;
}

void GameActivate()
{
    pGame->continueMusic("music");
}

void GameDeactivate()
{
    pGame->pauseMusic("music");
}

void GamePaint(sf::RenderWindow &window)
{
    // Draw the background highway
    pGame->showTexture("highway",0,0,window);

    // Draw the sprites
    pGame->DrawSprites(window);

    // Draw the number of remaining chicken lives
    for( int i=0;i<numlives;i++)
        pGame->showTexture("chickenhead", 406 + mTextures["chickenhead"].getSize().x * i, 382, window);

    if(gameover)
        pGame->Text("Game Over! You scored " + std::to_string(score) + " points.", 0,0,sf::Color::White, 20, "font", pGame->window);

    window.display();
}

void GameCycle(sf::Time delta)
{
    if(!gameover)
    {
        //play a random car sound randomly
        if( rnd.getRndInt(0,100) == 0 )
        {
            if( rnd.getRndInt(0,1) == 0 )
            {
                pGame->playSound("carhorn1");
            }
            else
            {
                pGame->playSound("carhorn2");
            }
        }

        //update the sprites
        pGame->UpdateSprites(delta);
    }
}

void HandleKeys()
{
    if(!gameover)
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

    if( pGame->mouseClicked && gameover)
    {
        numlives = 3;
        score = 0;
        gameover = false;
        pGame->continueMusic("music");
    }
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
        score += 150;
    }
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
    //see if the chicken was hit
    if( pSpriteHittee->GetName() == "chicken" )
    {
        //move the chicken back to the start
        pSpriteHittee->SetPosition(4,175);

        //see if the game is over
        if( --numlives > 0 )
        {
            //play a sound for the chicken getting hit
            pGame->playSound("squish");
        }
        else
        {
            //play a sound for the game ending
            pGame->playSound("gameover");
            gameover = true;
            pGame->pauseMusic("music");
        }

        return false;
    }
    else
    {
        if(pSpriteHitter->GetName() == "chicken")
        {
            pSpriteHitter->SetPosition(4,175);

            if( --numlives > 0 )
                pGame->playSound("squish");
            else
            {
                pGame->playSound("gameover");
                gameover = true;
                pGame->pauseMusic("music");
            }

            return false;
        }
    }

    return true;
}
