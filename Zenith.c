#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#define MaxNumbers 16
time_t start, end;
bool GameOver = false;
int numbers[MaxNumbers] = { 1,3,2,4,5,6,7,8,3,5,4,7,6,8,1,2 };
int revealed[MaxNumbers] = { 0 };
int selected[2] = { -1,-1 };
int matchedPairs = 0;
int borderThickness = 5;
int i, j;
float cornerRadius = 0.9;
float cornerRadius_1 = 0.3;
bool EndScreen = false;

//Fisher Yates Algorithm 
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize(int arr[], int n)
{
    srand(time(NULL));

    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

//StartScreen function
void StartScreen() {
    InitWindow(1190, 670, "Game Start Screen");
    SetTargetFPS(60);
    Image myImg = LoadImage("landscape.png");   //Include your own file path
    Texture2D mytext = LoadTextureFromImage(myImg);
    UnloadImage(myImg);
    Rectangle startButton = { 1190 / 2 - 100, 670 / 2 - 25, 200, 50 };
    Color buttonColor = BLACK;
    bool buttonPressed = false;
    while (!WindowShouldClose()) {
        if (CheckCollisionPointRec(GetMousePosition(), startButton)) {
            buttonColor = YELLOW;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                buttonPressed = true;
            }
        }
        else {
            buttonColor = BLACK;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(mytext, 0, 0, WHITE);
        DrawText("ZENITH", 400, 100, 100, YELLOW);
        DrawRectangleRec(startButton, buttonColor);
        DrawText("Start Game", startButton.x + 40, startButton.y + 10, 20, YELLOW);
        if (buttonPressed) {
            break;
        }
        EndDrawing();
    }
    UnloadTexture(mytext);
    CloseWindow();
}

//Main game function
void Game()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("ZENITH"), 400, 0, 100, WHITE);
    for (int i = 0; i < MaxNumbers; i++)
    {
        if (revealed[i] || matchedPairs == MaxNumbers / 2)
        {
            DrawText(TextFormat("%d", numbers[i]), 140 + 100 * (i % 4), 180 + 100 * (i / 4), 50, RED);
        }
        else
        {
            DrawRectangleRounded((Rectangle) { 100 + 100 * (i % 4), 150 + 100 * (i / 4), 100, 100 }, cornerRadius, 2, BLACK);
            DrawRectangleRounded((Rectangle) { 100 + 100 * (i % 4) + borderThickness, 150 + 100 * (i / 4) + borderThickness, 100 - 2 * borderThickness, 100 - 2 * borderThickness }, cornerRadius_1, 2, WHITE);
        }
        if (GameOver == false)
        {
            start = clock();
            DrawText(TextFormat("%ds", start / 1000), 700, 250, 50, WHITE);
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !revealed[i] && matchedPairs < MaxNumbers / 2)
        {
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, (Rectangle) { 100 + 100 * (i % 4), 150 + 100 * (i / 4), 100, 100 }))
            {
                if (selected[0] == -1)
                {
                    selected[0] = i;
                    revealed[i] = 1;
                }
                else if (selected[1] == -1 && i != selected[0])
                {
                    selected[1] = i;
                    revealed[i] = 1;
                }
                if (selected[0] != -1 && selected[1] != -1)
                {
                    if (numbers[selected[0]] == numbers[selected[1]])
                    {
                        matchedPairs++;
                    }
                    else
                    {
                        revealed[selected[0]] = 0;
                        revealed[selected[1]] = 0;
                    }
                    selected[0] = -1;
                    selected[1] = -1;
                }
            }
        }
    }
    EndDrawing();
}

//Timer & End Screen function
void Timer() {
    InitWindow(1000, 563, "END SCREEN");
    Image leo = LoadImage("Endscreen.png");    //Include your own file path
    Texture2D myleo = LoadTextureFromImage(leo);
    UnloadImage(leo);
    GameOver = true;
    end = clock();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(myleo, 0, 0, WHITE);
        DrawText("CONGRATULATIONS", 250, 50, 50, BLACK);
        DrawText("YOUR TIME WAS", 290, 150, 50, BLACK);
        DrawText(TextFormat("%ds", end / 1000), 475, 250, 50, WHITE);
        EndDrawing();
    }
    UnloadTexture(myleo);
    CloseWindow();
}

int main() {
    StartScreen();
    InitAudioDevice();
    Music music = LoadMusicStream("GameAudio.mp3");     //Include your own file path
    PlayMusicStream(music);
    InitWindow(1190, 670, "Zenith");
    Image myImage = LoadImage("bg.png");    //Include your own file path
    Texture2D myTexture = LoadTextureFromImage(myImage);
    UnloadImage(myImage);
    SetTargetFPS(60);
    randomize(numbers, MaxNumbers);
    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);
        DrawTexture(myTexture, 0, 0, WHITE);
        Game();

        if (matchedPairs == 8)
        {
            EndScreen = true;
            Timer();
        }
    }
    UnloadTexture(myTexture);
    EndDrawing();
    StopMusicStream(music);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}