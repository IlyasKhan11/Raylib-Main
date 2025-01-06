#ifndef ALIEN_HPP
#define ALIEN_HPP

#include <raylib.h>

class Alien {
public:
    Alien(int type, Vector2 position); // Constructor
    void Draw(); // Draw method
    int GetType() const; // Get alien type
    void Update(int direction); // Update method
    Rectangle getRect() const; // Get collision rectangle
    Vector2 GetPosition() const; // Get position
    void SetPosition(Vector2 newPosition); // Set position
    static void LoadImages(); // Load all textures
    static void UnloadImages(); // Unload all textures
    static Texture2D* getAlienImages(); // Public getter for alienImages

private:
    int type; // Alien type (1, 2, or 3)
    Vector2 position; // Alien position
    static Texture2D alienImages[3]; // Static array of alien textures
};

#endif // ALIEN_HPP