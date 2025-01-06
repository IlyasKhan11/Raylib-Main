#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include <raylib.h>
#include <vector> // For storing lasers
#include "Laser.hpp" // Include the full definition of the Laser class

class Spaceship {
public:
    Spaceship();  // Constructor
    ~Spaceship(); // Destructor
    void Draw();  // Draw method
    void MoveLeft();  // Move left method
    void MoveRight(); // Move right method
    void FireLaser(); // Fire laser method
    Rectangle getRect();
    std::vector<Laser>& GetLasers(); // Public method to access lasers

private:
    Texture2D image; // Texture for the spaceship
    Vector2 position; // Position of the spaceship
    double lastFiretime; // Time when the last laser was fired
    std::vector<Laser> lasers; // List of active lasers
};

#endif // SPACESHIP_HPP