#include "raylib.h"
#include <iostream>
#include <string>

int player1Score = 0;
int player2Score = 0;

enum class GameMode {
    LOADING,
    MENU,
    SINGLE_PLAYER,
    TWO_PLAYER
};

class Ball {
    public:
        float x, y;
        int speedX, speedY;
        int radius = 20;
        const int StartSpeed = 7;

        void draw() {
            DrawRectangle(x, y, radius, radius, WHITE);
        }

        void update() {
            x += speedX;
            y += speedY;

            if (y < 0 || y + radius > GetScreenHeight()) {
                speedY *= -1;
            }

            if (x < 0) {
                player2Score++;
                resetBall();
            }

            if (x > GetScreenWidth() - radius) {
                player1Score++;
                resetBall();
            }
        }

        void resetBall() {
            x = GetScreenWidth() / 2 - radius / 2;
            y = GetScreenHeight() / 2 - radius / 2;
            speedY = (rand() % 2 == 0) ? -StartSpeed : StartSpeed;
            speedX = (rand() % 2 == 0) ? -StartSpeed : StartSpeed;
        }
};

class Paddle {
    protected:
        void handleCollision() {
            if (y <= 0) {
                y = 0;
            }
            if (y + height >= GetScreenHeight()) {
                y = GetScreenHeight() - height;
            }
        }
    
    public:
        float x, y;
        int width = 20;
        int height = 100;
        int speed = 10;

        void draw() {
            DrawRectangle(x, y, width, height, WHITE);
        }

        void update() {
            // Player input
            if (IsKeyDown(KEY_UP)) {
                y = y - speed;
            }
            if (IsKeyDown(KEY_DOWN)) {
                y = y + speed;
            }

            handleCollision();
        }
};

class Paddle2Player : public Paddle {
    public:
        void update() {
            // Player input
            if (IsKeyDown(KEY_W)) {
                y = y - speed;
            }
            if (IsKeyDown(KEY_S)) {
                y = y + speed;
            }

            handleCollision();
        }
};

class CpuPaddle : public Paddle {
    public:
        void update(int ball_y){
            if (y + height / 2 > ball_y) {
                y = y - speed;
            }
            if (y + height / 2 <= ball_y) {
                y = y + speed;
            }
            handleCollision();
        }
};

GameMode gameMode = GameMode::MENU;

Ball ball;
Paddle player1;
Paddle2Player player2;
CpuPaddle cpuPlayer;

int cursorPosition = 0;

void menuUpdate() {
    DrawText("Pong", 20, 100, 50, WHITE);

    std::string singlePlayerText = (cursorPosition == 0 ? "> " : "  ") + std::string("Single Player");
    DrawText(singlePlayerText.c_str(), 20, 200, 30, WHITE);

    std::string twoPlayerText = (cursorPosition == 1 ? "> " : "  ") + std::string("Two Player");
    DrawText(twoPlayerText.c_str(), 20, 250, 30, WHITE);

    if (IsKeyDown(KEY_DOWN) && cursorPosition < 1) {
        cursorPosition++;
    }
    if (IsKeyDown(KEY_UP) && cursorPosition > 0) {
        cursorPosition--;
    }

    if (IsKeyDown(KEY_ENTER)) {
        if (cursorPosition == 0) {
            // Single Player
            player1.x = player1.width;
            player1.y = GetScreenHeight() / 2 - player1.height / 2;

            cpuPlayer.x = GetScreenWidth() - cpuPlayer.width * 2;
            cpuPlayer.y = GetScreenHeight() / 2 - cpuPlayer.height / 2;

            ball.resetBall();
            player1Score = 0;
            player2Score = 0;

            cursorPosition = 0;
            // Switch to single player mode
            gameMode = GameMode::SINGLE_PLAYER;
        } else if (cursorPosition == 1) {
            // Two Player
            player1.x = player1.width;
            player1.y = GetScreenHeight() / 2 - player1.height / 2;

            player2.x = GetScreenWidth() - player2.width * 2;
            player2.y = GetScreenHeight() / 2 - player2.height / 2;

            ball.resetBall();
            player1Score = 0;
            player2Score = 0;

            cursorPosition = 0;
            // Switch to two player mode
            gameMode = GameMode::TWO_PLAYER;
        }
    }
}

void singlePlayerUpdate() {
    // Update
    ball.update();
    player1.update();
    cpuPlayer.update(ball.y);

    BoundingBox playerBox1 = { {player1.x, player1.y, 0}, {player1.x + player1.width, player1.y + player1.height, 0} };
    BoundingBox playerBox2 = { {cpuPlayer.x, player2.y, 0}, {cpuPlayer.x + cpuPlayer.width, cpuPlayer.y + cpuPlayer.height, 0} };
    BoundingBox ballBox = { {ball.x, ball.y, 0}, {ball.x + ball.radius, ball.y + ball.radius, 0} };
    if (CheckCollisionBoxes(
        playerBox1, ballBox
    )) {
        ball.speedX *= -1;
    }

    if (CheckCollisionBoxes(
        playerBox2,
        ballBox
    )) {
        ball.speedX *= -1;
    }

    if (IsKeyDown(KEY_ESCAPE)) {
        gameMode = GameMode::MENU;
    }

    // Draw
    ball.draw();
    player1.draw();
    cpuPlayer.draw();

    DrawText(std::to_string(player1Score).c_str(), GetScreenWidth() / 4, 20, 40, WHITE);
    DrawText(std::to_string(player2Score).c_str(), GetScreenWidth() * 3 / 4, 20, 40, WHITE);
}

void twoPlayerUpdate() {
    // Update
    ball.update();
    player1.update();
    player2.update();

    BoundingBox playerBox1 = { {player1.x, player1.y, 0}, {player1.x + player1.width, player1.y + player1.height, 0} };
    BoundingBox playerBox2 = { {player2.x, player2.y, 0}, {player2.x + player2.width, player2.y + player2.height, 0} };
    BoundingBox ballBox = { {ball.x, ball.y, 0}, {ball.x + ball.radius, ball.y + ball.radius, 0} };
    if (CheckCollisionBoxes(
        playerBox1, ballBox
    )) {
        ball.speedX *= -1;
    }

    if (CheckCollisionBoxes(
        playerBox2,
        ballBox
    )) {
        ball.speedX *= -1;
    }

    if (IsKeyDown(KEY_ESCAPE)) {
        gameMode = GameMode::MENU;
    }

    // Draw
    ball.draw();
    player1.draw();
    player2.draw();

    DrawText(std::to_string(player1Score).c_str(), GetScreenWidth() / 4, 20, 40, WHITE);
    DrawText(std::to_string(player2Score).c_str(), GetScreenWidth() * 3 / 4, 20, 40, WHITE);
}

int main() {
    InitWindow(800, 600, "Pong");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        switch(gameMode){
            case GameMode::MENU:
                menuUpdate();
                break;
            case GameMode::SINGLE_PLAYER:
                singlePlayerUpdate();
                break;
            case GameMode::TWO_PLAYER:
                twoPlayerUpdate();
                break;
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
