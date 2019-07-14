class AlienSprite : public CSprite
{
public:
    //constructor/destructor
    AlienSprite(const std::string &texture, sf::FloatRect rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
    virtual ~AlienSprite();

    //general methods
    virtual SPRITEACTION Update(sf::Time delta);
    virtual CSprite*     AddSprite();
};

/////////////////////////////////////////////////////////////

//constructor/ destructor
AlienSprite::AlienSprite(const std::string &texture, sf::FloatRect rcBounds, BOUNDSACTION baBoundsAction) : CSprite(texture, rcBounds, baBoundsAction)
{
}

AlienSprite::~AlienSprite()
{
}

// AlienSprite General methods
SPRITEACTION AlienSprite::Update(sf::Time delta)
{
    //call the base sprite update() method
    SPRITEACTION saSpriteAction;
    saSpriteAction = CSprite::Update(delta);

    //see if the alien should fire a missile
    if( rnd.getRndInt(0,difficulty/2) == 0 )
        saSpriteAction = (SPRITEACTION) (saSpriteAction | SA_ADDSPRITE);

    return saSpriteAction;
}

CSprite* AlienSprite::AddSprite()
{
    //create a new missile sprite
    sf::FloatRect rcBounds(0,0,640,410);
    sf::FloatRect rcPos = GetPosition();
    CSprite* pSprite = nullptr;
    if( GetName() == "blobbo" )
    {
        //blobbo missile
        pSprite = new CSprite("bmissile", rcBounds, BA_DIE);
        pSprite->SetName("bmissile");
        pSprite->SetVelocity(0,49);
    }
    else if( GetName() == "jelly" )
    {
        //jelly missile
        pSprite = new CSprite("jmissile", rcBounds, BA_DIE);
        pSprite->SetName("jmissile");
        pSprite->SetVelocity(0,35);
    }
    else
    {
        //timmy missile
        pSprite = new CSprite("tmissile", rcBounds, BA_DIE);
        pSprite->SetName("tmissile");
        pSprite->SetVelocity(0,21);
    }

    //set the missile sprite's position and return it
    pSprite->SetPosition(rcPos.left + GetWidth()/2, rcPos.top + rcPos.height);
    return pSprite;
}
