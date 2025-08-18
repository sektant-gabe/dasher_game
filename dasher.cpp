#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2   pos;
    int       frame;
    float     updateTime;
    float     runningTime;
};

int main()
{
    // --- Window / Game setup ---
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;
    const Color background_color{ BLACK };
    const int   target_fps{ 60 };
    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher Game");
    SetTargetFPS(target_fps);

    // --- Physics ---
    const int gravity{ 1'000 };
    const int player_jump_velocity{ -600 };
    bool      isInAir{};
    int       velocity{};

    // --- Player ---
    Texture2D player = LoadTexture("textures/scarfy.png");
    AnimData  playerData;
    playerData.rec.width   = player.width / 6;
    playerData.rec.height  = player.height;
    playerData.rec.x       = 0;
    playerData.rec.y       = 0;
    playerData.pos.x       = windowDimensions[0] / 2 - playerData.rec.width / 2;
    playerData.pos.y       = windowDimensions[1] - playerData.rec.height;
    playerData.frame       = 0;
    playerData.updateTime  = 1.0 / 12.0;
    playerData.runningTime = 0;

    // --- Nebula ---
    int       nebulaVelocity{ -400 };
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    AnimData nebulaData{
        { 0.0, 0.0, nebula.width / 8.0f, nebula.height / 8.0f }, // Rectangle rec
        { windowDimensions[0] * 1.0f,
         windowDimensions[1] - nebula.height / 8.0f }, // Vector2 pos
        0, // int  frame
        1.0 / 12.0, // float updateTime
        0.0  // float runningTime
    };

    AnimData nebula2Data{
        { 0.0, 0.0, nebula.width / 8.0f, nebula.height / 8.0f }, // Rectangle rec
        { windowDimensions[0] + 300.0f,
         windowDimensions[1] - nebula.height / 8.0f }, // Vector2 pos
        0, // int  frame
        1.0 / 16.0, // float updateTime
        0.0  // float runningTime
    };

    AnimData nebulae[2]{ nebulaData, nebula2Data };

    while (!WindowShouldClose())
    {
        const float dt{ GetFrameTime() };

        BeginDrawing();

        ClearBackground(background_color);

        if (playerData.pos.y >= windowDimensions[1] - playerData.rec.height)
        {
            velocity = 0;
            isInAir  = false;
        }
        else
        {
            velocity += gravity * dt;
            isInAir   = true;
        }

        if (IsKeyDown(KEY_SPACE) && !isInAir)
        {
            velocity += player_jump_velocity;
        }

        nebulae[0].pos.x += nebulaVelocity * dt;
        nebulae[1].pos.x += nebulaVelocity * dt;
        playerData.pos.y += velocity * dt;

        nebulae[0].runningTime += dt;
        nebulae[1].runningTime += dt;
        playerData.runningTime += dt;

        if (nebulae[0].runningTime >= nebulae[0].updateTime)
        {
            nebulae[0].runningTime = 0.0;
            nebulae[0].rec.x       = nebulae[0].frame * nebulae[0].rec.width;
            nebulae[0].frame++;
            if (nebulae[0].frame > 7)
            {
                nebulae[0].frame = 0;
            }
        }
        if (nebulae[1].runningTime >= nebulae[1].updateTime)
        {
            nebulae[1].runningTime = 0.0;
            nebulae[1].rec.x       = nebulae[1].frame * nebulae[1].rec.width;
            nebulae[1].frame++;
            if (nebulae[1].frame > 7)
            {
                nebulae[1].frame = 0;
            }
        }

        if (nebulae[0].pos.x < 0 - nebulae[0].rec.width)
        {
            nebulae[0].pos.x = windowDimensions[0] + nebulae[0].rec.width;
        }

        if (nebulae[1].pos.x < 0 - nebulae[1].rec.width)
        {
            nebulae[1].pos.x = windowDimensions[0] + nebulae[1].rec.width;
        }

        if (playerData.runningTime >= playerData.updateTime && !isInAir)
        {
            playerData.runningTime = 0.0;
            playerData.rec.x       = playerData.frame * playerData.rec.width;
            playerData.frame++;
            if (playerData.frame > 5)
            {
                playerData.frame = 0;
            }
        }
        DrawTextureRec(nebula, nebulae[0].rec, nebulae[0].pos, WHITE);
        DrawTextureRec(nebula, nebulae[1].rec, nebulae[1].pos, RED);

        DrawTextureRec(player, playerData.rec, playerData.pos, WHITE);

        EndDrawing();
    }
    UnloadTexture(nebula);
    UnloadTexture(player);
    CloseWindow();
}
