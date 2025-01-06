#ifndef GAME_HPP
#define GAME_HPP

#include "Spaceship.hpp"
#include "Obstacle.hpp" // Include the Obstacle class
#include <vector>

class Game {
public:
    Game();  // Constructor
    ~Game(); // Destructor
    void Draw();  // Draw method
    void Update(); // Update method
    void HandleInput(); // Handle input method

private:
    Spaceship spaceship; // Spaceship object
    std::vector<Obstacle> obstacles; // List of obstacles
    void DeleteInactiveLasers(); // Delete inactive lasers
    std::vector<Obstacle> CreateObstacles(); // Create obstacles
};

#endif // GAME_HPP