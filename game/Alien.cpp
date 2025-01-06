#include "Alien.hpp"
#include <iostream>

// Initialize static member variable
Texture2D Alien::alienImages[3] = {};

// Constructor
Alien::Alien(int type, Vector2 position) {
    this->type = type;
    this->position = position;
}

// Draw method
void Alien::Draw() {
    if (alienImages[type - 1].id != 0) {
        DrawTextureV(alienImages[type - 1], position, WHITE);
    } else {
        std::cerr << "Cannot draw alien: Texture not loaded for type " << type << std::endl;
    }
}

// Get alien type
int Alien::GetType() const {
    return type;
}

// Update method
void Alien::Update(int direction) {
    position.x += direction;
}

// Get collision rectangle
Rectangle Alien::getRect() const {
    return {
        position.x,
        position.y,
        float(alienImages[type - 1].width),
        float(alienImages[type - 1].height)
    };
}

// Get position
Vector2 Alien::GetPosition() const {
    return position;
}

// Set position
void Alien::SetPosition(Vector2 newPosition) {
    position = newPosition;
}

// Load all textures
void Alien::LoadImages() {
    const char* imagePaths[3] = {
        "/home/muhammad-ilyas-khan/Desktop/raylib main/game/images/alien_1.png",
        "/home/muhammad-ilyas-khan/Desktop/raylib main/game/images/alien_2.png",
        "/home/muhammad-ilyas-khan/Desktop/raylib main/game/images/alien_3.png"
    };

    for (int i = 0; i < 3; i++) {
        Image im = LoadImage(imagePaths[i]);
        if (im.data == nullptr) {
            std::cerr << "Failed to load image: " << imagePaths[i] << std::endl;
        } else {
            alienImages[i] = LoadTextureFromImage(im);
            UnloadImage(im); // Unload the image after loading the texture
            std::cout << "Successfully loaded texture: " << imagePaths[i] << std::endl;
        }
    }
}

// Unload all textures
void Alien::UnloadImages() {
    for (int i = 0; i < 3; i++) {
        if (alienImages[i].id != 0) {
            UnloadTexture(alienImages[i]);
            std::cout << "Unloaded texture for alien type " << (i + 1) << std::endl;
        }
    }
}

// Public getter for alienImages
Texture2D* Alien::getAlienImages() {
    return alienImages;
}