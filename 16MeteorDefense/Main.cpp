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
#include "Background.h"
#include "GameEngine.h"

//global variables
GameEngine *pGame;
Rnd rnd;
StarryBackground* pBackground;
CSprite* pTargetSprite;
int numcities = 0, score = 0, difficulty = 0;
bool canfire = true;
bool gameover = false;

void NewGame();
void AddMeteor();

bool GameInitialize()
{
    pGame = new GameEngine("Meteor Defense",600,450);
    if(pGame == nullptr) return false;

    pGame->SetFrameRate(30);

    return true;
}

void GameStart()
{
    pGame->loadAssets(); //assets loading
    pBackground = new StarryBackground(600,450); //create background
    pGame->playMusic("music",true); //play music
    NewGame(); //start the game.
}

void GameEnd()
{
    delete pBackground;
    pGame->CleanupAll();
    pGame->window.close();
    delete pGame;
}

void GameActivate()
{
    pGame->pauseMusic("music");
}

void GameDeactivate()
{
    pGame->continueMusic("music");
}

void GamePaint(sf::RenderWindow &window)
{
    pBackground->Draw(window);
    pGame->showTexture("ground",0,398,window);
    pGame->DrawSprites(window);

    //draw the score
    pGame->Text(std::to_string(score),275,0,sf::Color::White, 20, "font", window);

    //draw the game over message if necessary
    if(gameover)
        pGame->showTexture("gameover", 170, 150, window);

    window.display();
}

void GameCycle(sf::Time delta)
{
    if(!gameover)
    {
        //randomly add meteors
        if( rnd.getRndInt(0,difficulty) == 0 )
            AddMeteor();

        //update the background
        pBackground->Update();

        //update the sprites
        pGame->UpdateSprites(delta);
    }

  //check for mouse activity
  if(pGame->mouseClicked && canfire)
  {
    MouseButtonDown(pGame->mousePos.x, pGame->mousePos.y, pGame->mouseLeftButton);
  }

  if(pGame->mouseClicked == false)
  {
      MouseButtonUp(pGame->mousePos.x, pGame->mousePos.y, pGame->mouseLeftButton);
  }

    //update the target position with the mouse position
    int mx = sf::Mouse::getPosition(pGame->window).x;
    int my = sf::Mouse::getPosition(pGame->window).y;
    MouseMove(mx,my);
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, bool bLeft)
{
    if(!gameover && bLeft)
    {
        //create a missile sprite and set its position
        sf::FloatRect rcBounds(0,0,600,450);
        int iXPos = (x < 300) ? 144 : 449;
        CSprite* pSprite = new CSprite(pGame->getTexture("missile"), rcBounds, BA_DIE);
        pSprite->SetPosition(iXPos, 365);

        //calculate the velocity so that it is aimed at the target
        int iXVel, iYVel = -48;
        y = std::min(y,300);
        iXVel = (iYVel * ((iXPos + 8) - x)) / (365 - y);
        pSprite->SetVelocity(iXVel, iYVel);

        //add the sprite missile
        pSprite->SetName("missile");
        pGame->AddSprite(pSprite);

        //play the fire sound
        pGame->playSound("fire");

        //update the score
        score = std::max(--score, 0);

        canfire = false; //avoid multiple
    }
    else if(gameover && !bLeft)
    {
        //start a new game
        NewGame();
    }
}

void MouseButtonUp(int x, int y, bool bLeft)
{
    canfire = true;
}

void MouseMove(int x, int y)
{
    //track the mouse with the target sprite
    int tw = pGame->getTexture("target").getSize().x;
    int th = pGame->getTexture("target").getSize().y;
    pTargetSprite->SetPosition( x - tw / 2, y - th / 2);
}

bool SpriteCollision(CSprite* pSpriteHitter, CSprite* pSpriteHittee)
{
    //see if a missile and a meteor have collided
    if((pSpriteHitter->GetName() == "missile" &&
        pSpriteHittee->GetName() == "meteor") ||
       (pSpriteHitter->GetName() == "meteor" &&
        pSpriteHittee->GetName() == "missile"))
    {
        //kill both sprites
        pSpriteHittee->Kill();
        pSpriteHitter->Kill();

        //update the score
        score += 6;
        difficulty = std::max(50 - (score/10), 5);
    }

    //see if a meteor has collided with a city
    if(pSpriteHitter->GetName() == "meteor" &&
       pSpriteHittee->GetName() == "city")
    {
        //play the big explosion sound
        pGame->playSound("bigexpl");

        //kill both sprites
        pSpriteHitter->Kill();
        pSpriteHittee->Kill();

        //see if the game is over
        if(--numcities == 0)
        {
            gameover = true;
            pGame->stopMusic("music");
        }
    }

    return false;
}

void SpriteDying(CSprite* pSprite)
{
    //see if a meteor sprite is dying
    if(pSprite->GetName() == "meteor")
    { std::cout << "explosion" << std::endl;
        //play the explosion sound
        pGame->playSound("expl");

        //create a explosion sprite at the meteor's position
        sf::FloatRect rcBounds(0,0,600,450);
        sf::FloatRect rcPos = pSprite->GetPosition();
        pSprite = new CSprite(pGame->getTexture("expl"), rcBounds);
        pSprite->setNumFrames(12,true);
        pSprite->SetPosition(rcPos.left, rcPos.top);
        pGame->AddSprite(pSprite);
    }
}

//functions //////////////////////////////////////////
void NewGame()
{
    //clear the sprites
    pGame->CleanupSprites();

    //create the target sprite
    sf::FloatRect rcBounds(0,0,600,450);
    pTargetSprite = new CSprite(pGame->getTexture("target"), rcBounds, BA_STOP);
    pTargetSprite->SetZOrder(10);
    pGame->AddSprite(pTargetSprite);

    //create the city sprites
    CSprite* pSprite = new CSprite(pGame->getTexture("city"), rcBounds);
    pSprite->SetPosition(2,370);
    pSprite->SetName("city");
    pGame->AddSprite(pSprite);
    pSprite = new CSprite(pGame->getTexture("city"), rcBounds);
    pSprite->SetPosition(186,370);
    pSprite->SetName("city");
    pGame->AddSprite(pSprite);
    pSprite = new CSprite(pGame->getTexture("city"), rcBounds);
    pSprite->SetPosition(302,370);
    pSprite->SetName("city");
    pGame->AddSprite(pSprite);
    pSprite = new CSprite(pGame->getTexture("city"), rcBounds);
    pSprite->SetPosition(490,370);
    pSprite->SetName("city");
    pGame->AddSprite(pSprite);

    //initialize the game variables
    score = 0;
    numcities = 4;
    difficulty = 50;
    gameover = false;

    pGame->continueMusic("music");
}

void AddMeteor()
{
  // Create a new meteor sprite and set its position
  sf::FloatRect rcBounds(0, 0, 600, 390);
  int iXPos = rnd.getRndInt(0,600);
  CSprite* pSprite = new CSprite(pGame->getTexture("meteor"), rcBounds, BA_DIE);
  pSprite->setNumFrames(14, false);
  pSprite->SetPosition(iXPos, 0);
  pSprite->SetName("meteor");

  // Calculate the velocity so that it is aimed at one of the cities
  int iXVel, iYVel = rnd.getRndInt(0,3) + 3;
  switch(rnd.getRndInt(0,3))
  {
  case 0:
    iXVel = (iYVel * (56 - (iXPos + 50))) / 400;
    break;
  case 1:
    iXVel = (iYVel * (240 - (iXPos + 50))) / 400;
    break;
  case 2:
    iXVel = (iYVel * (360 - (iXPos + 50))) / 400;
    break;
  case 3:
    iXVel = (iYVel * (546 - (iXPos + 50))) / 400;
    break;
  }
  pSprite->SetVelocity(iXVel, iYVel);

  // Add the meteor sprite
  pGame->AddSprite(pSprite);
}
