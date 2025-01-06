#include "Game.hpp"
#include <iostream>

// Constructor
Game::Game() {
    obstacles = CreateObstacles(); // Initialize obstacles
}

// Destructor
Game::~Game() {
    // Clean up resources (if needed)
}

// Draw method
void Game::Draw() {
    spaceship.Draw(); // Draw the spaceship

    for (auto& obstacle : obstacles) {
        obstacle.Draw(); // Draw each obstacle
    }
}

// Handle input method
void Game::HandleInput() {
    if (IsKeyDown(KEY_LEFT)) {
        spaceship.MoveLeft(); // Move the spaceship left
    } else if (IsKeyDown(KEY_RIGHT)) {
        spaceship.MoveRight(); // Move the spaceship right
    } else if (IsKeyDown(KEY_SPACE)) {
        spaceship.FireLaser(); // Fire a laser
    }
}

// Update method
void Game::Update() {
    // Update lasers
    for (auto& laser : spaceship.GetLasers()) {
        laser.Update();
    }
    DeleteInactiveLasers(); // Delete inactive lasers
    std::cout << "Vector Size:------------> " << spaceship.GetLasers().size() << std::endl;
}

// Delete inactive lasers
void Game::DeleteInactiveLasers() {
    auto& lasers = spaceship.GetLasers(); // Access the lasers vector
    for (auto it = lasers.begin(); it != lasers.end(); ) {
        if (!it->active) {
            it = lasers.erase(it); // Erase the inactive laser and update the iterator
        } else {
            ++it; // Move to the next laser
        }
    }
}

// Create obstacles
std::vector<Obstacle> Game::CreateObstacles() {
    std::vector<Obstacle> obstacles;
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;

    for (int i = 0; i < 4; i++) {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 200)}));
    }
    return obstacles;
}