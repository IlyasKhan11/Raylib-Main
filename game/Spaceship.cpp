#include "Spaceship.hpp"

// Constructor
Spaceship::Spaceship() : lastFiretime(0.0) {
    // Use the correct path to the image file
    const char* imagePath = "/home/muhammad-ilyas-khan/Desktop/raylib main/game/assets/spaceship.png";
    Image im = LoadImage(imagePath); // Load the image
    if (im.data == nullptr) {
        TraceLog(LOG_ERROR, "Failed to load image: %s", imagePath);
        return; // Exit the constructor if the image fails to load
    } else {
        TraceLog(LOG_INFO, "Image loaded successfully!");
    }

    image = LoadTextureFromImage(im); // Convert the image to a texture
    if (image.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load texture from image!");
    } else {
        TraceLog(LOG_INFO, "Texture loaded successfully!");
    }

    UnloadImage(im); // Unload the image (the texture is already loaded)

    position.x = (GetScreenWidth() - image.width) / 2; // Initial X position
    position.y = GetScreenHeight() - image.height; // Initial Y position
}

// Destructor
Spaceship::~Spaceship() {
    UnloadTexture(image); // Unload the texture
}

// Draw the spaceship
void Spaceship::Draw() {
    // Debug: Draw text to verify the Draw method is being called
    DrawText("Spaceship", position.x, position.y - 20, 20, WHITE);

    // Debug: Draw a rectangle to verify drawing functions
    // Draw the spaceship texture (only if the texture is valid)
    if (image.id != 0) {
        DrawTexture(image, position.x, position.y, WHITE);
    } else {
        TraceLog(LOG_WARNING, "Cannot draw texture: Texture is not loaded!");
    }

    // Draw all lasers
    for (auto& laser : lasers) {
        laser.Draw();
    }
}

void Spaceship::MoveLeft() {
    position.x -= 5; // Move left by 5 units
    if (position.x < 0) {
        position.x = 0; // Prevent the spaceship from moving off the screen
    }
}

// Move the spaceship right
void Spaceship::MoveRight() {
    position.x += 5; // Move right by 5 units
    if (position.x > GetScreenWidth() - image.width) {
        position.x = GetScreenWidth() - image.width; // Prevent the spaceship from moving off the screen
    }
}

// Fire a laser
void Spaceship::FireLaser() {
    if (GetTime() - lastFiretime >= 0.35) {
        lasers.push_back(Laser({position.x + image.width / 2 - 2, position.y}, -6));
        lastFiretime = GetTime(); // Update the last fire time
    }
}

// Public method to access lasers
std::vector<Laser>& Spaceship::GetLasers() {
    return lasers;
}


Rectangle Spaceship::getRect() {
    return {
        position.x,               // X position of the top-left corner
        position.y,               // Y position of the top-left corner
        static_cast<float>(image.width),  // Width of the spaceship
        static_cast<float>(image.height)  // Height of the spaceship
    };
}