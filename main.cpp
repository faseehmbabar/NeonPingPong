#include "raylib.h"

// Window and game settings
const int W = 1000, H = 650;
const int PW = 16, PH = 110, R = 10;
const int WIN_SCORE = 5;

// Neon colours
const Color BG = {10, 12, 28, 255};
const Color NEON_BLUE = {0, 190, 255, 255};
const Color NEON_RED = {255, 55, 95, 255};
const Color NEON_YELLOW = {255, 235, 70, 255};
const Color NEON_WHITE = {220, 235, 255, 255};

// Arrays store both players' positions and scores
int px[2] = {45, W - 61};
int py[2] = {270, 270};
int score[2] = {0, 0};

int bx = W / 2, by = H / 2;
int dx = 5, dy = 4;
bool gameOver = false;

void ResetBall()
{
    bx = W / 2;
    by = H / 2;
    dx = -dx;
    dy = GetRandomValue(3, 5);

    if (GetRandomValue(0, 1) == 0)
        dy = -dy;
}

void ResetGame()
{
    score[0] = 0;
    score[1] = 0;
    py[0] = H / 2 - PH / 2;
    py[1] = H / 2 - PH / 2;
    gameOver = false;
    dx = 5;
    ResetBall();
}

void MovePaddles()
{
    if (IsKeyDown(KEY_W)) py[0] -= 7;
    if (IsKeyDown(KEY_S)) py[0] += 7;
    if (IsKeyDown(KEY_UP)) py[1] -= 7;
    if (IsKeyDown(KEY_DOWN)) py[1] += 7;

    for (int i = 0; i < 2; i++)
    {
        if (py[i] < 10) py[i] = 10;
        if (py[i] > H - PH - 10)
            py[i] = H - PH - 10;
    }
}

void MoveBall()
{
    bx += dx;
    by += dy;

    // Bounce off the top and bottom
    if (by - R < 10)
    {
        by = 10 + R;
        dy = -dy;
    }

    if (by + R > H - 10)
    {
        by = H - 10 - R;
        dy = -dy;
    }

    // Left paddle collision
    if (dx < 0 && bx - R <= px[0] + PW &&
        bx + R >= px[0] && by + R >= py[0] &&
        by - R <= py[0] + PH)
    {
        bx = px[0] + PW + R;
        dx = -dx;
    }

    // Right paddle collision
    if (dx > 0 && bx + R >= px[1] &&
        bx - R <= px[1] + PW && by + R >= py[1] &&
        by - R <= py[1] + PH)
    {
        bx = px[1] - R;
        dx = -dx;
    }

    // Award points when the ball leaves the screen
    if (bx + R < 0)
    {
        score[1]++;
        ResetBall();
    }

    if (bx - R > W)
    {
        score[0]++;
        ResetBall();
    }

    if (score[0] >= WIN_SCORE || score[1] >= WIN_SCORE)
        gameOver = true;
}

void DrawGame()
{
    ClearBackground(BG);

    // Neon border and center line
    DrawRectangleLinesEx({8, 8, W - 16, H - 16}, 2, NEON_BLUE);

    for (int y = 20; y < H - 20; y += 35)
        DrawRectangle(W / 2 - 2, y, 4, 18,
                      Fade(NEON_BLUE, 0.7f));

    // Paddle glow
    DrawRectangle(px[0] - 3, py[0] - 3, PW + 6, PH + 6,
                  Fade(NEON_BLUE, 0.18f));
    DrawRectangle(px[1] - 3, py[1] - 3, PW + 6, PH + 6,
                  Fade(NEON_RED, 0.18f));

    // Paddles
    DrawRectangle(px[0], py[0], PW, PH, NEON_BLUE);
    DrawRectangle(px[1], py[1], PW, PH, NEON_RED);

    // Round yellow ball with glow
    DrawCircle(bx, by, R + 5, Fade(NEON_YELLOW, 0.18f));
    DrawCircle(bx, by, R, NEON_YELLOW);

    // Scores and title (Raylib's built-in font)
    DrawText(TextFormat("%d", score[0]), 405, 35, 48, NEON_BLUE);
    DrawText(TextFormat("%d", score[1]), 570, 35, 48, NEON_RED);
    DrawText("NEON PING PONG", 350, 100, 30, NEON_WHITE);

    DrawText("PLAYER 1", 35, H - 65, 20, NEON_BLUE);
    DrawText("W / S", 35, H - 40, 18, NEON_WHITE);
    DrawText("PLAYER 2", 850, H - 65, 20, NEON_RED);
    DrawText("UP / DOWN", 850, H - 40, 18, NEON_WHITE);

    // Game-over screen
    if (gameOver)
    {
        DrawRectangle(180, 205, 640, 240, {20, 24, 48, 245});
        DrawRectangleLinesEx({180, 205, 640, 240}, 2, NEON_BLUE);

        if (score[0] >= WIN_SCORE)
            DrawText("PLAYER 1 WINS!", 355, 245, 38, NEON_BLUE);
        else
            DrawText("PLAYER 2 WINS!", 355, 245, 38, NEON_RED);

        DrawText("GAME OVER", 410, 300, 28, NEON_YELLOW);
        DrawText("PRESS R TO PLAY AGAIN", 330, 355, 24, NEON_WHITE);
    }
}

int main()
{
    InitWindow(W, H, "Neon Ping Pong");
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)GetTime());

    ResetGame();

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            MovePaddles();
            MoveBall();
        }
        else if (IsKeyPressed(KEY_R))
        {
            ResetGame();
        }

        BeginDrawing();
        DrawGame();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}