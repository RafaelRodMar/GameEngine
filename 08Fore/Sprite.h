
#pragma once

//actions in case of sprite reaching a bound.
enum BOUNDSACTION {BA_STOP, BA_WRAP, BA_BOUNCE, BA_DIE};

class CSprite
{
protected:
  // Member Variables
  sf::Texture   ptexture;
  sf::Sprite    psprite;
  sf::IntRect   position;
  sf::Vector2i  velocity;
  int           ZOrder;
  sf::IntRect      rcBounds;
  BOUNDSACTION  BoundsAction;
  bool          Hidden;

public:
  // Constructor(s)/Destructor
  CSprite(sf::Texture pTexture);
  CSprite(sf::Texture pTexture, sf::IntRect &Bounds,
    BOUNDSACTION baBoundsAction = BA_STOP);
  CSprite(sf::Texture pTexture, sf::Vector2i pPosition, sf::Vector2i pVelocity, int iZOrder,
    sf::IntRect &Bounds, BOUNDSACTION baBoundsAction = BA_STOP);
  virtual ~CSprite();

  // General Methods
  virtual void  Update();
  void          Draw(sf::RenderWindow &window);
  bool          IsPointInside(int x, int y);

  // Accessor Methods
  sf::IntRect   GetPosition() { return position; };
  void    SetPosition(int x, int y);
  void    SetPosition(sf::Vector2i vPosition);
  void    SetPosition(sf::IntRect pPosition)
    { position = pPosition; };
  void    OffsetPosition(int x, int y);
  sf::Vector2i   GetVelocity() { return velocity; };
  void    SetVelocity(int x, int y);
  void    SetVelocity(sf::Vector2i pvelocity);
  bool    GetZOrder()               { return ZOrder; };
  void    SetZOrder(int iZOrder)    { ZOrder = iZOrder; };
  void    SetBounds(sf::IntRect prcBounds) { rcBounds = prcBounds; };
  void    SetBoundsAction(BOUNDSACTION ba) { BoundsAction = ba; };
  bool    IsHidden()                { return Hidden; };
  void    SetHidden(bool bHidden)   { Hidden = bHidden; };
  int     GetWidth()                { return ptexture.getSize().x; };
  int     GetHeight()               { return ptexture.getSize().y; };
};

//-----------------------------------------------------------------
// Sprite Inline General Methods
//-----------------------------------------------------------------
inline bool CSprite::IsPointInside(int x, int y)
{
  return position.contains(x,y);
}

//-----------------------------------------------------------------
// Sprite Inline Accessor Methods
//-----------------------------------------------------------------
inline void CSprite::SetPosition(int x, int y)
{
    position.left = x;
    position.top = y;
    psprite.setPosition(position.left, position.top);
}

inline void CSprite::SetPosition(sf::Vector2i vPosition)
{
    position.left = vPosition.x;
    position.top = vPosition.y;
    psprite.setPosition(position.left, position.top);
}

inline void CSprite::OffsetPosition(int x, int y)
{
    position.left = x;
    position.top = y;
    psprite.setPosition(position.left, position.top);
}

inline void CSprite::SetVelocity(int x, int y)
{
  velocity.x = x;
  velocity.y = y;
}

inline void CSprite::SetVelocity(sf::Vector2i pvelocity)
{
  velocity.x = pvelocity.x;
  velocity.y = pvelocity.y;
}

CSprite::CSprite(sf::Texture pTexture)
{
  // Initialize the member variables
  ptexture = pTexture;

  position.top = 0;
  position.left = 0;
  position.width = ptexture.getSize().x;
  position.height = ptexture.getSize().y;

  velocity.x = velocity.y = 0;
  ZOrder = 0;
  rcBounds.top = 0;
  rcBounds.left = 0;
  rcBounds.width = 640; //default values
  rcBounds.height = 480;

  BoundsAction = BA_STOP;
  Hidden = false;

  psprite.setTexture(ptexture);
  psprite.setPosition(position.top,position.left);
}

CSprite::CSprite(sf::Texture pTexture, sf::IntRect &prcBounds, BOUNDSACTION baBoundsAction)
{
  // Calculate a random position
  int iXPos = rand() % (prcBounds.width);
  int iYPos = rand() % (prcBounds.height);

  // Initialize the member variables
  ptexture = pTexture;

  position.top = iXPos;
  position.left = iYPos;
  position.width = ptexture.getSize().x;
  position.height = ptexture.getSize().y;

  velocity.x = velocity.y = 0;
  ZOrder = 0;
  rcBounds = prcBounds;
  BoundsAction = baBoundsAction;
  Hidden = false;

  psprite.setTexture(ptexture);
  psprite.setPosition(position.top, position.left);
}

CSprite::CSprite(sf::Texture pTexture, sf::Vector2i ptPosition, sf::Vector2i ptVelocity, int iZOrder,
    sf::IntRect &prcBounds, BOUNDSACTION baBoundsAction)
{
  // Initialize the member variables
  ptexture = pTexture;

  position.top = ptPosition.x;
  position.left = ptPosition.y;
  position.width = ptexture.getSize().x;
  position.height = ptexture.getSize().y;

  velocity = ptVelocity;
  ZOrder = iZOrder;
  rcBounds = prcBounds;
  BoundsAction = baBoundsAction;
  Hidden = false;

  psprite.setTexture(ptexture);
  psprite.setPosition(position.top, position.left);
}

CSprite::~CSprite()
{
}

//-----------------------------------------------------------------
// Sprite General Methods
//-----------------------------------------------------------------
void CSprite::Update()
{
  // Update the position
  sf::Vector2i ptNewPosition, ptSpriteSize, ptBoundsSize;
  ptNewPosition.x = position.left + velocity.x;
  ptNewPosition.y = position.top + velocity.y;
  ptSpriteSize.x = position.width;
  ptSpriteSize.y = position.height;
  ptBoundsSize.x = rcBounds.width;
  ptBoundsSize.y = rcBounds.height;

  // Check the bounds
  // Wrap?
  if (BoundsAction == BA_WRAP)
  {
    if ((ptNewPosition.x + ptSpriteSize.x) < rcBounds.left)
      ptNewPosition.x = rcBounds.left + rcBounds.width;
    else if (ptNewPosition.x > rcBounds.left + rcBounds.width)
      ptNewPosition.x = rcBounds.left - ptSpriteSize.x;
    if ((ptNewPosition.y + ptSpriteSize.y) < rcBounds.top)
      ptNewPosition.y = rcBounds.top + rcBounds.height;
    else if (ptNewPosition.y > rcBounds.top + rcBounds.height)
      ptNewPosition.y = rcBounds.top - ptSpriteSize.y;
  }
  // Bounce?
  else if (BoundsAction == BA_BOUNCE)
  {
    bool bBounce = false;
    sf::Vector2i ptNewVelocity = velocity;
    if (ptNewPosition.x < rcBounds.left)
    {
      bBounce = true;
      ptNewPosition.x = rcBounds.left;
      ptNewVelocity.x = -ptNewVelocity.x;
    }
    else if ((ptNewPosition.x + ptSpriteSize.x) > rcBounds.left + rcBounds.width)
    {
      bBounce = true;
      ptNewPosition.x = rcBounds.left + rcBounds.width - ptSpriteSize.x;
      ptNewVelocity.x = -ptNewVelocity.x;
    }
    if (ptNewPosition.y < rcBounds.top)
    {
      bBounce = true;
      ptNewPosition.y = rcBounds.top;
      ptNewVelocity.y = -ptNewVelocity.y;
    }
    else if ((ptNewPosition.y + ptSpriteSize.y) > rcBounds.top + rcBounds.height)
    {
      bBounce = true;
      ptNewPosition.y = rcBounds.top + rcBounds.height - ptSpriteSize.y;
      ptNewVelocity.y = -ptNewVelocity.y;
    }
    if (bBounce)
      SetVelocity(ptNewVelocity);
  }
  // Stop (default)
  else
  {
    if (ptNewPosition.x  < rcBounds.left ||
      ptNewPosition.x > (rcBounds.left + rcBounds.width - ptSpriteSize.x))
    {
      ptNewPosition.x = std::max(rcBounds.left, std::min(ptNewPosition.x,
        rcBounds.left + rcBounds.width - ptSpriteSize.x));
      SetVelocity(0, 0);
    }
    if (ptNewPosition.y  < rcBounds.top ||
      ptNewPosition.y > (rcBounds.top + rcBounds.height - ptSpriteSize.y))
    {
      ptNewPosition.y = std::max(rcBounds.top, std::min(ptNewPosition.y,
        rcBounds.top + rcBounds.height - ptSpriteSize.y));
      SetVelocity(0, 0);
    }
  }
  SetPosition(ptNewPosition);
}

void CSprite::Draw(sf::RenderWindow &window)
{
  // Draw the sprite if it isn't hidden
  if (!Hidden)
    window.draw(psprite);
}

