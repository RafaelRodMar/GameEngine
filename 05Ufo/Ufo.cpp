#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <SFML/Graphics.hpp>
#include "Texture.h"
#include "GameEngine.h"

//global variables
GameEngine *pGame;
const int MAXSPEED = 8;
Texture tbackground;
Texture tsaucer;
sf::Sprite background;
sf::Sprite saucer;
int saucerx, saucery;
int speedx, speedy;


bool GameInitialize()
{
    //create the game engine
    pGame = new GameEngine("UFO", 500, 400);
    if( pGame == nullptr ) return false;

    //set the frame rate
    pGame->SetFrameRate(30);

    return true;
}

void GameStart()
{
    //create and load the background and saucer bitmaps
    tbackground.load("background.png");
    background.setTexture(tbackground.texture);
    tsaucer.load("saucer.png");
    saucer.setTexture(tsaucer.texture);

    //set the initial saucer position and speed
    saucerx = 250 - (tsaucer.GetWidth()/2);
    saucery = 250 - (tsaucer.GetHeight()/2);
    speedx = 0;
    speedy = 0;
}

void GameEnd()
{
    // cleanup
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
    //draw the background and saucer bitmaps
    window.draw(background);
    saucer.setPosition( (float) saucerx, (float) saucery );
    window.draw(saucer);
    window.display();
}

void GameCycle(sf::Time delta)
{
    //update the saucer position
    saucerx = std::min( 500 - tsaucer.GetWidth(), std::max(0, saucerx + speedx));
    saucery = std::min(320, std::max(0, saucery + speedy));

    //saucerx *= delta.asSeconds();
    //saucery *= delta.asSeconds();
}

void HandleKeys()
{
    //change the speed of the saucer in response to arrow key presses
    if( pGame->KeyPressed(sf::Keyboard::Left) || pGame->KeyHeld(sf::Keyboard::Left) )
        speedx = std::max( -MAXSPEED, --speedx );
    else if( pGame->KeyPressed(sf::Keyboard::Right) || pGame->KeyHeld(sf::Keyboard::Right) )
        speedx = std::min( MAXSPEED, ++speedx );
    if( pGame->KeyPressed(sf::Keyboard::Up) || pGame->KeyHeld(sf::Keyboard::Up) )
        speedy = std::max( -MAXSPEED, --speedy );
    else if( pGame->KeyPressed(sf::Keyboard::Down) || pGame->KeyHeld(sf::Keyboard::Down) )
        speedy = std::min( MAXSPEED, ++speedy );

    std::cout << saucerx << "," << saucery << std::endl;
}

void MouseButtonDown(int x, int y, bool bLeft)
{
    if( bLeft )
    {
        //set the saucer position to the mouse position
        saucerx = x - (tsaucer.GetWidth() / 2);
        saucery = y - (tsaucer.GetHeight() / 2);
    }
    else
    {
        //stop the saucer
        speedx = speedy = 0;
    }
}

void MouseButtonUp(int x, int y, bool bLeft)
{

}

void MouseMove(int x, int y)
{

}
