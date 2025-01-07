#include "Game.hpp"
#include <iostream>
#include "raylib.h"
#include <string>

// Constructor
Game::Game() {
    Alien::LoadImages(); // Load alien textures once
    obstacles = CreateObstacles(); // Initialize obstacles
    aliens = CreateAliens(); // Initialize aliens
    alienDirection = 1; // Start moving aliens to the right
    timeLastAlienFired = GetTime(); // Initialize last fire time
    alienLaserShootInterval = 0.5; // Set interval between alien laser shots
    lives = 3;
    run = true;
    highscore = 0;
    isOver=false;
}

// Destructor
Game::~Game() {
    Alien::UnloadImages(); // Unload alien textures
}

// Draw method
void Game::Draw() {
    spaceship.Draw(); // Draw the spaceship

    // Draw lives
    DrawText("Lives: ", 10, 10, 20, WHITE);
    if (lives == 3) {
        DrawText("O O O", 100, 10, 20, WHITE);
    } else if (lives == 2) {
        DrawText("O O", 100, 10, 20, WHITE);
    } else if (lives == 1) {
        DrawText("O", 100, 10, 20, WHITE);
    }else if(lives==0){
        DrawText("GAME OVER",GetScreenWidth()-100,20,50,RED);
    }

    // Draw score
    std::string scoreText = "Score: " + std::to_string(highscore); // Convert score to string
    DrawText(scoreText.c_str(), GetScreenWidth() - 150, 10, 20, WHITE); // Display score in top-right corner

    // Draw obstacles
    for (auto& obstacle : obstacles) {
        obstacle.Draw();
    }

    // Draw aliens
    for (auto& alien : aliens) {
        alien.Draw();
    }

    // Draw alien lasers
    for (auto& laser : alienLasers) {
        laser.Draw();
    }
}

// Handle input method
void Game::HandleInput() {
    if (run) {
        if (IsKeyDown(KEY_LEFT)) {
            spaceship.MoveLeft(); // Move the spaceship left
        } else if (IsKeyDown(KEY_RIGHT)) {
            spaceship.MoveRight(); // Move the spaceship right
        } else if (IsKeyDown(KEY_SPACE)) {
            spaceship.FireLaser(); // Fire a laser
        }
    }
}

// Update method
void Game::Update() {
    if (run) {
        // Update spaceship lasers
        for (auto& laser : spaceship.GetLasers()) {
            laser.Update();
        }

        // Update alien lasers
        for (auto& laser : alienLasers) {
            laser.Update();
        }

        DeleteInactiveLasers(); // Delete inactive lasers
        MoveAliens(); // Move aliens
        AlienShootLaser(); // Alien shoots laser
        CheckForCollisions();
    }else{
        isOver=true;
    }
}

// Delete inactive lasers
void Game::DeleteInactiveLasers() {
    auto& lasers = spaceship.GetLasers(); // Access the spaceship lasers vector
    for (auto it = lasers.begin(); it != lasers.end(); ) {
        if (!it->active) {
            it = lasers.erase(it); // Erase the inactive laser and update the iterator
        } else {
            ++it; // Move to the next laser
        }
    }

    // Delete inactive alien lasers
    for (auto it = alienLasers.begin(); it != alienLasers.end(); ) {
        if (!it->active) {
            it = alienLasers.erase(it); // Erase the inactive laser and update the iterator
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

// Create aliens
std::vector<Alien> Game::CreateAliens() {
    std::vector<Alien> aliens;
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 11; column++) { // Reduced columns for better spacing
            int alienType;
            if (row == 0) {
                alienType = 3;
            } else if (row == 1 || row == 2) {
                alienType = 2;
            } else {
                alienType = 1;
            }

            float x = 75 + column * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}

// Move aliens
void Game::MoveAliens() {
    for (auto& alien : aliens) {
        if (alien.getRect().x + alien.getRect().width > GetScreenWidth() - 25) {
            alienDirection = -1; // Move left
            MoveDownAliens(4); // Move aliens down
        }
        if (alien.getRect().x < 25) {
            alienDirection = 1; // Move right
            MoveDownAliens(4); // Move aliens down
        }

        alien.Update(alienDirection); // Update alien position
    }
}

// Move aliens down
void Game::MoveDownAliens(int distance) {
    for (auto& alien : aliens) {
        Vector2 newPosition = alien.GetPosition();
        newPosition.y += distance;
        alien.SetPosition(newPosition); // Update alien position
    }
}

// Alien shoots laser
void Game::AlienShootLaser() {
    double currentTime = GetTime();

    if (currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()) {
        int numLasersToFire = GetRandomValue(1, 3); // Fire 1 to 3 lasers

        for (int i = 0; i < numLasersToFire; i++) {
            int randomIndex = GetRandomValue(0, aliens.size() - 1);
            Alien& alien = aliens[randomIndex];

            Texture2D* images = Alien::getAlienImages();
            Vector2 alienPosition = alien.GetPosition();
            float laserX = alienPosition.x + images[alien.GetType() - 1].width / 2;
            float laserY = alienPosition.y + images[alien.GetType() - 1].height;

            alienLasers.push_back(Laser({laserX, laserY}, 6));
        }

        timeLastAlienFired = currentTime;
    }
}

// Check for collisions
void Game::CheckForCollisions() {
    // Spaceship lasers
    for (auto& laser : spaceship.GetLasers()) {
        auto it = aliens.begin();
        while (it != aliens.end()) {
            if (CheckCollisionRecs(it->getRect(), laser.getRect())) {
                highscore += 10;
                it = aliens.erase(it);
                laser.active = false;
            } else {
                ++it;
            }
        }

        for (auto& obstacle : obstacles) {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end()) {
                if (CheckCollisionRecs(it->getRect(), laser.getRect())) {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else {
                    ++it;
                }
            }
        }
    }

    // Alien lasers
    for (auto& laser : alienLasers) {
        if (CheckCollisionRecs(laser.getRect(), spaceship.getRect())) {
            laser.active = false;
            lives--;
            if (lives == 0) {
                GameOver(); // Call GameOver when lives reach 0
            }
        }

        for (auto& obstacle : obstacles) {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end()) {
                if (CheckCollisionRecs(it->getRect(), laser.getRect())) {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else {
                    ++it;
                }
            }
        }
    }

    // Alien collision with obstacles
    for (auto& alien : aliens) {
        for (auto& obstacle : obstacles) {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end()) {
                if (CheckCollisionRecs(it->getRect(), alien.getRect())) {
                    it = obstacle.blocks.erase(it);
                } else {
                    ++it;
                }
            }
        }

        if (CheckCollisionRecs(alien.getRect(), spaceship.getRect())) {
            GameOver(); // Call GameOver if alien collides with spaceship
        }
    }
}

// Game over method
void Game::GameOver() {
    run = false; // Stop the game
    isOver=true;

    // Reset the game state if needed
    ResetGame();
}

// Get score method
int Game::GetScore() {
    return highscore;
}

// Reset game method
void Game::ResetGame() {
    lives = 3;
    highscore = 0;
    aliens = CreateAliens(); // Reinitialize aliens
    obstacles = CreateObstacles(); // Reinitialize obstacles
    // spaceship.Reset(); // Reset the spaceship position
    alienLasers.clear(); // Clear alien lasers
    spaceship.GetLasers().clear(); // Clear player lasers
}