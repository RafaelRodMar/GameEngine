#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <SFML/Graphics.hpp>
#include "Texture.h"
#include "GameEngine.h"

//global variables
GameEngine *pGame;
const int numslides = 6;
Texture *pTextures[numslides];
int currSlide;
sf::Sprite sprite;


bool GameInitialize()
{
    //Create the game engine
    pGame = new GameEngine("SlideShow",640,480);

    if(pGame==nullptr) return false;

    //set the frame rate
    pGame->SetFrameRate(1);

    return true;
}

void GameStart()
{
    //create and load the slide bitmaps
    pTextures[0] = new Texture("image1.png");
    pTextures[1] = new Texture("image2.png");
    pTextures[2] = new Texture("image3.png");
    pTextures[3] = new Texture("image4.png");
    pTextures[4] = new Texture("image5.png");
    pTextures[5] = new Texture("image6.png");

    //set the position of the sprite
    sprite.setPosition(0,0);

    //set the first slide
    currSlide = 0;
}

void GameEnd()
{
    //cleanup the slide bitmaps
    for(int i = 0;i < numslides; i++) delete pTextures[i];

    //cleanup the game engine
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
    sprite.setTexture(pTextures[currSlide]->texture);
    window.draw(sprite);
    window.display();
}

void GameCycle(sf::Time delta)
{
    currSlide++;
    if( currSlide > 5 ) currSlide = 0;
}
