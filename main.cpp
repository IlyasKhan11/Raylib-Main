#include "raylib.h"
#include "sqlite3.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string>

#include "game/Spaceship.hpp"
#include "game/Game.hpp"
#include "game/Obstacle.hpp"
#include "game/Alien.hpp"

#define MAX_INPUT_LENGTH 50

typedef enum {
    PAGE_SIGNIN,
    PAGE_SIGNUP,
    PAGE_MENU,
    PAGE_CREDITS,
    PAGE_HIGHSCORE,
    PAGE_GAME
} PageType;

typedef struct {
    char text[MAX_INPUT_LENGTH];
    int length;
    Rectangle bounds;
    bool isActive;
} InputField;

typedef struct {
    char username[MAX_INPUT_LENGTH];
    bool isLoggedIn;
} UserState;

sqlite3* db = NULL;
UserState currentUser = {0};
const char* logedInUser="";
int currentScore=0;

void InitInputField(InputField* field, Rectangle bounds) {
    field->text[0] = '\0';
    field->length = 0;
    field->bounds = bounds;
    field->isActive = false;
}

//shahzaib
void InitDatabase() {
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS users ("
        "    username TEXT PRIMARY KEY,"
        "    password TEXT NOT NULL,"
        "    highscore INTEGER DEFAULT 0"
        ");";
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, createTableSQL, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}


void UpdateInputField(InputField* field) {
    if (CheckCollisionPointRec(GetMousePosition(), field->bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        field->isActive = true;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        field->isActive = false;
    }

    if (field->isActive) {
        int key = GetCharPressed();
        while (key > 0) {
            if (field->length < MAX_INPUT_LENGTH - 1) {
                field->text[field->length] = (char)key;
                field->text[field->length + 1] = '\0';
                field->length++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && field->length > 0) {
            field->length--;
            field->text[field->length] = '\0';
        }
    }
}

bool ValidateCredentials(const char* username, const char* password) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT password FROM users WHERE username = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return false;
    
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
    
    bool isValid = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* storedPassword = (const char*)sqlite3_column_text(stmt, 0);
        isValid = (strcmp(password, storedPassword) == 0);
    }
    
    sqlite3_finalize(stmt);
    return isValid;
}

bool InsertNewUser(const char* username, const char* password) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO users (username, password, highscore) VALUES (?, ?, 0)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return false;
    
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_TRANSIENT);
    
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}















#include "raylib.h"

#include "raylib.h"
#include <iostream> // For debugging

#include "raylib.h"

void DrawGame(Texture2D background) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Load texture from file
    Texture2D backgroundTexture = LoadTexture("game/assts/spaceship.png");
    Texture2D bg = LoadTexture("hbg.png");

    // Define the red X button
    const int buttonSize = 30; // Size of the button (square)
    Rectangle buttonBounds = { screenWidth - buttonSize - 10, 10, buttonSize, buttonSize }; // Position at top-right corner
    Color buttonColor = RED; // Color of the button

    Game game;

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw the game
        game.HandleInput();
        game.Update();
        game.Draw();

        // Draw the red X button
        DrawRectangleRec(buttonBounds, buttonColor); // Draw the button
        DrawText("X", screenWidth - buttonSize - 5, 15, 20, WHITE); // Draw the "X" text

        // Draw the current score
        currentScore = game.GetScore(); // Get the current score from the game
        std::string scoreText = "Score: " + std::to_string(currentScore); // Convert score to string
        // DrawText(scoreText.c_str(), screenWidth - 150, 50, 20, WHITE); // Display score below the X button

        // Compare with high score (example: assume high score is stored in a variable)
        static int highScore = 0; // Example high score (you can replace this with actual high score logic)
        if (currentScore > highScore) {
            highScore = currentScore; // Update high score if current score is higher
        }




        // Check if the red X button is clicked
        if (CheckCollisionPointRec(GetMousePosition(), buttonBounds)) {
            // Change button color when hovered
            buttonColor = Color{ 255, 0, 0, 150 }; // Semi-transparent red

            // Check if the button is clicked
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                break; // Exit the game loop
            }
        } else {
            buttonColor = RED; // Reset button color
        }

        EndDrawing();
    }

    // Unload texture and close window
    UnloadTexture(backgroundTexture);
}

void DrawCreditsPage(Texture2D background) {
    

    const char* title = "CREDITS PAGE";
    Texture2D bg=LoadTexture("bg.jpg");
    const char* names[] = {
        "1. Shahzaib (23-cs-141)",
        "2. Ilyas (23-cs-115)",
        "3. Suliha (23-cs-29)",
        "4. Tayyaba (23-cs-97)",
        "5. Bilal (23-cs-119)"
    };
    const int nameCount = sizeof(names) / sizeof(names[0]);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw background
        DrawTexturePro(bg,
            (Rectangle){0, 0, background.width, background.height},
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0}, 0.0f, WHITE);

        // Draw title
        // DrawRectangle(GetScreenWidth()-(GetScreenWidth()-400),20,1000,1000,PURPLE);
        DrawText(title, GetScreenWidth() / 2 - MeasureText(title, 40) / 2, 50, 40, WHITE);

        // Draw names
        for (int i = 0; i < nameCount; i++) {
            DrawText(names[i], GetScreenWidth() / 2 - MeasureText(names[i], 30) / 2, 
                     150 + i * 50, 30, WHITE);
        }

        // Add an option to return to the menu
        const char* backButtonLabel = "Back to Menu";
        Rectangle backButton = {GetScreenWidth() / 2 - 100, GetScreenHeight() - 100, 200, 50};
        Vector2 mousePoint = GetMousePosition();

        Color backButtonColor = CheckCollisionPointRec(mousePoint, backButton) ? DARKBLUE : BLUE;
        DrawRectangleRec(backButton, backButtonColor);
        DrawText(backButtonLabel, backButton.x + 25, backButton.y + 15, 20, WHITE);

        if (CheckCollisionPointRec(mousePoint, backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return; // Exit the credits page and return to the menu
        }

        EndDrawing();
    }
}



#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

// Function to update the high score for the current logged-in user
void UpdateHighScore(sqlite3* db, const char* username, int currentScore) {
    // Step 1: Retrieve the current high score from the database
    char selectQuery[256];
    snprintf(selectQuery, sizeof(selectQuery),
             "SELECT highscore FROM users WHERE username = '%s';", username);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, selectQuery, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    int highScore = 0; // Default high score
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        highScore = sqlite3_column_int(stmt, 0); // Get the high score from the database
    }
    sqlite3_finalize(stmt);

    // Step 2: Compare the current score with the high score
    if (currentScore > highScore) {
        // Step 3: Update the high score in the database
        char updateQuery[256];
        snprintf(updateQuery, sizeof(updateQuery),
                 "UPDATE users SET highscore = %d WHERE username = '%s';", currentScore, username);

        if (sqlite3_exec(db, updateQuery, NULL, NULL, NULL) != SQLITE_OK) {
            printf("Error updating high score: %s\n", sqlite3_errmsg(db));
        } else {
            printf("High score updated for user %s: %d\n", username, currentScore);
        }
    } else {
        printf("Current score (%d) is not higher than the high score (%d). No update needed.\n", currentScore, highScore);
    }
}


#include "raylib.h"

bool DrawGameOverScreen(int score, Texture2D background) {
    const char* gameOverText = "GAME OVER";
    const char* scoreText = TextFormat("Your Score: %d", score);
    const char* backButtonLabel = "Back to Menu";

    // Define the back button
    Rectangle backButton = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 100, 200, 50};
    Vector2 mousePoint = GetMousePosition();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw background
        DrawTexturePro(background,
            (Rectangle){0, 0, background.width, background.height},
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0}, 0.0f, WHITE);

        // Draw "Game Over" text
        DrawText(gameOverText, GetScreenWidth() / 2 - MeasureText(gameOverText, 50) / 2, GetScreenHeight() / 2 - 100, 50, RED);

        // Draw the score
        DrawText(scoreText, GetScreenWidth() / 2 - MeasureText(scoreText, 30) / 2, GetScreenHeight() / 2 - 30, 30, WHITE);

        // Draw the back button
        Color backButtonColor = CheckCollisionPointRec(mousePoint, backButton) ? Color{ 200, 0, 0, 255 } : RED; // Darker red when hovered
        DrawRectangleRec(backButton, backButtonColor);
        DrawText(backButtonLabel, backButton.x + (backButton.width - MeasureText(backButtonLabel, 20)) / 2, backButton.y + 15, 20, WHITE);

        // Check if the back button is clicked
        if (CheckCollisionPointRec(mousePoint, backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            EndDrawing();
            return true; // Signal to return to the menu
        }

        EndDrawing();
    }

    return false; // If the window is closed, return false
}

void DrawHighscorePage(Texture2D background, sqlite3* db) {
    const char* title = "TOP 5 HIGHSCORES";
    const int maxScores = 5;

    char query[256];
    snprintf(query, sizeof(query),
             "SELECT username, highscore FROM users ORDER BY highscore DESC LIMIT %d;", maxScores);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    char players[maxScores][MAX_INPUT_LENGTH];
    int scores[maxScores];
    int count = 0;

    Texture2D bg=LoadTexture("hbg.jpg");

    while (sqlite3_step(stmt) == SQLITE_ROW && count < maxScores) {
        const char* username = (const char*)sqlite3_column_text(stmt, 0);
        int highscore = sqlite3_column_int(stmt, 1);

        snprintf(players[count], MAX_INPUT_LENGTH, "%s", username);
        scores[count] = highscore;
        count++;
    }

    sqlite3_finalize(stmt);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw background
        DrawTexturePro(bg,
            (Rectangle){0, 0, background.width, background.height},
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0}, 0.0f, WHITE);

        // Draw title
        DrawText(title, GetScreenWidth() / 2 - MeasureText(title, 40) / 2, 50, 40, WHITE);

        // Draw highscore list
        for (int i = 0; i < count; i++) {
            char displayText[100];
            snprintf(displayText, sizeof(displayText), "%d. %s - %d", i + 1, players[i], scores[i]);
            DrawText(displayText, GetScreenWidth() / 2 - MeasureText(displayText, 30) / 2, 
                     150 + i * 50, 30, WHITE);
        }

        // Add an option to return to the menu
        const char* backButtonLabel = "Back to Menu";
        Rectangle backButton = {GetScreenWidth() / 2 - 100, GetScreenHeight() - 100, 200, 50};
        Vector2 mousePoint = GetMousePosition();

        Color backButtonColor = CheckCollisionPointRec(mousePoint, backButton) ? DARKBLUE : BLUE;
        DrawRectangleRec(backButton, backButtonColor);
        DrawText(backButtonLabel, backButton.x + 25, backButton.y + 15, 20, WHITE);

        if (CheckCollisionPointRec(mousePoint, backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return; // Exit the highscores page and return to the menu
        }

        EndDrawing();
    }
}

void DrawMenuPage(Texture2D background, bool* shouldClose, PageType* currentPage) {
    Rectangle buttons[] = {
        {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 100, 200, 50}, // Play
        {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 200, 50},       // Highscores
        {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 100, 200, 50}, // Credits
        {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 200, 200, 50}  // Quit
    };

    const char* labels[] = {"Play", "Highscores", "Credits", "Quit"};

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw background
        DrawTexturePro(background,
            (Rectangle){0, 0, background.width, background.height},
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0}, 0.0f, WHITE);

        // Draw title
        DrawText("MAIN MENU", GetScreenWidth() / 2 - 150, 50, 40, WHITE);
        
        DrawText("Current Player: ",10,20,20,WHITE);
        DrawText(logedInUser,200,20,20,WHITE);

        // Draw buttons
        for (int i = 0; i < 4; i++) {
            Color color = CheckCollisionPointRec(mousePoint, buttons[i]) ? DARKBLUE : BLUE;
            DrawRectangleRec(buttons[i], color);
            DrawText(labels[i], buttons[i].x + 75, buttons[i].y + 15, 20, WHITE);

            if (CheckCollisionPointRec(mousePoint, buttons[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if(i==0){
                    DrawGame(background);
                    UpdateHighScore(db,logedInUser,currentScore);
                    
                }
                else if (i == 1) {
                    DrawHighscorePage(background, db); // Show highscore page
                } else if (i == 2) {
                    DrawCreditsPage(background); // Show credits page
                } else if (i == 3) {
                    *shouldClose = true; // Exit the application
                }
                return;
            }
        }

        EndDrawing();
    }
}



int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Login System");
    ToggleFullscreen();
    SetTargetFPS(60);
    
    // Load resources
    Texture2D background = LoadTexture("bg2.jpg");
    Texture2D introImage = LoadTexture("intro.jpg"); // Intro PNG image
    
    int rc = sqlite3_open("users.db", &db);
    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    InitDatabase();
    
    InputField usernameField, passwordField;
    InitInputField(&usernameField, (Rectangle){GetScreenWidth()/2 - 150, GetScreenHeight()/2 - 60, 300, 40});
    InitInputField(&passwordField, (Rectangle){GetScreenWidth()/2 - 150, GetScreenHeight()/2 + 20, 300, 40});
    
    PageType currentPage = PAGE_SIGNIN;
    bool actionSuccess = false;
    bool actionError = false;
    const char* message = NULL;
    bool shouldClose = false;
    
    // Intro screen timer
    float introTime = 3.0f; // 3 seconds
    bool showIntro = true;
    
    while (!WindowShouldClose() && !shouldClose) {

        // DrawGame(background);
        // continue;

        if (showIntro) {
            // Countdown timer for the intro screen
            introTime -= GetFrameTime();
            if (introTime <= 0) {
                showIntro = false; // Transition to main application
            }
            
            BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // Draw the intro image
            DrawTexturePro(introImage,
                (Rectangle){0, 0, introImage.width, introImage.height},
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){0, 0}, 0.0f, WHITE);
            
            EndDrawing();
            continue;
        }
        
        // Handle menu or form page logic
        if (currentPage == PAGE_MENU) {
            DrawMenuPage(background, &shouldClose,&currentPage);
            continue;
        }
        
        if (IsKeyPressed(KEY_TAB)) {
            currentPage = (currentPage == PAGE_SIGNIN) ? PAGE_SIGNUP : PAGE_SIGNIN;
            usernameField.text[0] = '\0';
            usernameField.length = 0;
            passwordField.text[0] = '\0';
            passwordField.length = 0;
            message = NULL;
            actionSuccess = false;
            actionError = false;
        }
        
        UpdateInputField(&usernameField);
        UpdateInputField(&passwordField);
        
        Rectangle actionButton = {GetScreenWidth()/2 - 100, GetScreenHeight()/2 + 100, 200, 40};
        Vector2 mousePoint = GetMousePosition();
        
        if (CheckCollisionPointRec(mousePoint, actionButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (strlen(usernameField.text) > 0 && strlen(passwordField.text) > 0) {
                if (currentPage == PAGE_SIGNUP) {
                    if (InsertNewUser(usernameField.text, passwordField.text)) {
                        actionSuccess = true;
                        message = "Signup successful!";
                        usernameField.text[0] = '\0';
                        usernameField.length = 0;
                        passwordField.text[0] = '\0';
                        passwordField.length = 0;
                    } else {
                        actionError = true;
                        message = "Username already exists!";
                    }
                } else {
                    if (ValidateCredentials(usernameField.text, passwordField.text)) {
                        actionSuccess = true;
                        message = "Login successful!";
                        strncpy(currentUser.username, usernameField.text, MAX_INPUT_LENGTH-1);
                        currentUser.isLoggedIn = true;
                        currentPage = PAGE_MENU;
                        logedInUser=usernameField.text;
                    } else {
                        actionError = true;
                        message = "Invalid username or password!";
                    }
                }
            } else {
                actionError = true;
                message = "Please fill all fields!";
            }
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawTexturePro(background,
            (Rectangle){0, 0, background.width, background.height},
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0}, 0.0f, WHITE);
        
        const char* title = (currentPage == PAGE_SIGNIN) ? "Sign In" : "Sign Up";
        DrawText(title, GetScreenWidth()/2 - MeasureText(title, 40)/2, GetScreenHeight()/2 - 150, 40, WHITE);
        
        DrawRectangleRec(usernameField.bounds, LIGHTGRAY);
        DrawRectangleRec(passwordField.bounds, LIGHTGRAY);
        DrawText("Username:", usernameField.bounds.x - 100, usernameField.bounds.y + 10, 20, WHITE);
        DrawText("Password:", passwordField.bounds.x - 100, passwordField.bounds.y + 10, 20, WHITE);
        
        DrawText(usernameField.text, usernameField.bounds.x + 5, usernameField.bounds.y + 10, 20, DARKGRAY);
        
        char passwordDisplay[MAX_INPUT_LENGTH];
        memset(passwordDisplay, '*', strlen(passwordField.text));
        passwordDisplay[strlen(passwordField.text)] = '\0';
        DrawText(passwordDisplay, passwordField.bounds.x + 5, passwordField.bounds.y + 10, 20, DARKGRAY);
        
        if (usernameField.isActive) DrawRectangleLinesEx(usernameField.bounds, 2, BLUE);
        if (passwordField.isActive) DrawRectangleLinesEx(passwordField.bounds, 2, BLUE);
        
        DrawRectangleRec(actionButton, CheckCollisionPointRec(mousePoint, actionButton) ? DARKBLUE : BLUE);
        DrawText(title, actionButton.x + (actionButton.width - MeasureText(title, 20))/2, 
                actionButton.y + 10, 20, WHITE);
        
        if (message) {
            DrawText(message, GetScreenWidth()/2 - MeasureText(message, 20)/2, 
                    GetScreenHeight()/2 + 150, 20, actionSuccess ? GREEN : RED);
        }
        
        DrawText("Press TAB to switch between Sign In and Sign Up", 
                GetScreenWidth()/2 - MeasureText("Press TAB to switch between Sign In and Sign Up", 20)/2, 
                GetScreenHeight() - 40, 20, WHITE);
        
        EndDrawing();
    }
    
    sqlite3_close(db);
    UnloadTexture(background);
    UnloadTexture(introImage);
    CloseWindow();
    return 0;
}


