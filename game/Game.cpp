#include "Game.hpp"
#include <iostream>

// Constructor
Game::Game() {
    Alien::LoadImages(); // Load alien textures once
    obstacles = CreateObstacles(); // Initialize obstacles
    aliens = CreateAliens(); // Initialize aliens
    alienDirection = 1; // Start moving aliens to the right
    timeLastAlienFired = GetTime(); // Initialize last fire time
    alienLaserShootInterval = 0.5; // Set interval between alien laser shots
}

// Destructor
Game::~Game() {
    Alien::UnloadImages(); // Unload alien textures
}

// Draw method
void Game::Draw() {
    spaceship.Draw(); // Draw the spaceship

    for (auto& obstacle : obstacles) {
        obstacle.Draw(); // Draw each obstacle
    }

    for (auto& alien : aliens) {
        alien.Draw(); // Draw each alien
    }

    for (auto& laser : alienLasers) {
        laser.Draw(); // Draw each alien laser
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
    std::cout << "Vector Size:------------> " << spaceship.GetLasers().size() << std::endl;
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
        for (int column = 0; column < 18; column++) { // Reduced columns for better spacing
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