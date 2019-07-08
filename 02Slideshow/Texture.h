
class Texture
{
public:
    sf::Texture texture;

    //constructors
    Texture(std::string filename);

    //general methods
    int GetWidth() { return texture.getSize().x; };
    int GetHeight() { return texture.getSize().y; };
};

//create a texture from a file
Texture::Texture(std::string filename)
{
    texture.loadFromFile(filename);
}
