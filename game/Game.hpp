#ifndef GAME_HPP
#define GAME_HPP

#include "Spaceship.hpp"
#include "Alien.hpp"
#include "Obstacle.hpp"
#include "Laser.hpp"
#include <vector>
#include <string>

class Game {
public:
    Game(); // Constructor
    ~Game(); // Destructor

    void Draw(); // Draw everything
    void HandleInput(); // Handle user input
    void Update(); // Update game state
    bool run;
    int GetScore();
    void ResetGame();
    void InitGame();
    bool isOver;
private:
    Spaceship spaceship; // Player spaceship
    std::vector<Obstacle> obstacles; // List of obstacles
    std::vector<Alien> aliens; // List of aliens
    std::vector<Laser> alienLasers; // List of alien lasers
    int alienDirection; // Direction of alien movement (1 = right, -1 = left)
    double timeLastAlienFired; // Last time an alien fired a laser
    double alienLaserShootInterval; // Interval between alien laser shots
    int lives;
    int highscore;

    void DeleteInactiveLasers(); // Delete inactive lasers
    std::vector<Obstacle> CreateObstacles(); // Create obstacles
    std::vector<Alien> CreateAliens(); // Create aliens
    void CheckForCollisions();
    void GameOver();

    void MoveAliens(); // Move aliens
    void MoveDownAliens(int distance); // Move aliens down
    void AlienShootLaser(); // Alien shoots laser
};

#endif // GAME_HPP