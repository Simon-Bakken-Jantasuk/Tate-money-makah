#include "raylib.h"
#include "resource_dir.h"	
#include <stdio.h>

int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    float scale = 0.3f;

    SearchAndSetResourceDir("resources");
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tate moneymakah");

    Image matrixImage = LoadImage("background-image.gif");
    Texture2D matrixTexture = LoadTextureFromImage(matrixImage);
    UnloadImage(matrixImage);

    InitAudioDevice();
    Wave brokeWave = LoadWave("broke.wav");
    Sound brokeSound = LoadSoundFromWave(brokeWave); 
    UnloadWave(brokeWave);

    Music backgroundMusic = LoadMusicStream("background-music.mp3");
    PlayMusicStream(backgroundMusic);

    Texture2D andrewTate = LoadTexture("andrew-tate.jpg");
    Texture2D money = LoadTexture("100-dollar-32x32.png");

    Vector2 positionAndrewTate = { SCREEN_WIDTH / 2 - andrewTate.width / 2, SCREEN_HEIGHT / 2 - andrewTate.height / 2 }; 
    Vector2 positionMoney = { 0, 0 };
    bool isVisible = false;
    float displayTimer = 2.0f;
    float runningTimer = 0.0f;
    int moneyCounter = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateMusicStream(backgroundMusic);

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) positionAndrewTate.y -= 5.0f; 
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) positionAndrewTate.x -= 5.0f; 
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) positionAndrewTate.y += 5.0f; 
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) positionAndrewTate.x += 5.0f; 

        if (isVisible) {
            Rectangle rectAndrewTate = { positionAndrewTate.x, positionAndrewTate.y, andrewTate.width * scale, andrewTate.height * scale };
            Rectangle rectMoney = { positionMoney.x, positionMoney.y, money.width, money.height };

            if (CheckCollisionRecs(rectAndrewTate, rectMoney)) {
                moneyCounter++;
                PlaySound(brokeSound);
                isVisible = false;
                runningTimer = 0.0f; 
            }
        }

        if (!isVisible) {
            positionMoney.x = (float)GetRandomValue(0, SCREEN_WIDTH - money.width);
            positionMoney.y = (float)GetRandomValue(0, SCREEN_HEIGHT - money.height);
            isVisible = true;
            runningTimer = 0.0f;
        } else {
            runningTimer += GetFrameTime();
            if (runningTimer >= displayTimer) isVisible = false;
        }
        
        BeginDrawing();
            DrawTexture(matrixTexture, 0, 0, WHITE);
            DrawText(TextFormat("Capital: $%d", moneyCounter), 0, 0, 32, WHITE);
            DrawTextureEx(andrewTate, positionAndrewTate, 0.0f, scale, WHITE); 

            if (isVisible) {
                DrawTexture(money, positionMoney.x, positionMoney.y, WHITE);
            }
        EndDrawing();
    }
    UnloadTexture(matrixTexture);
    UnloadTexture(andrewTate);
    UnloadTexture(money);
    UnloadMusicStream(backgroundMusic);
    UnloadSound(brokeSound);


    CloseAudioDevice();
    CloseWindow();
    return 0;
}