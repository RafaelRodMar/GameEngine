
class Texture
{
public:
    sf::Texture texture;

    //constructors
    Texture();
    Texture(std::string filename);

    //general methods
    void load(std::string filename);
    int GetWidth() { return texture.getSize().x; };
    int GetHeight() { return texture.getSize().y; };
};

//create a texture from a file
Texture::Texture(std::string filename)
{
    texture.loadFromFile(filename);
}

Texture::Texture()
{

}

void Texture::load(std::string filename)
{
    texture.loadFromFile(filename);
}
