#include "Spaceship.hpp"

// Constructor
Spaceship::Spaceship() {
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

    position.x = (GetScreenWidth() - image.width)/2; // Initial X position
    position.y = GetScreenHeight()-image.height; // Initial Y position
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
}