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
    const int gravity{ 1'300 };
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
    int       nebulaVelocity{ -230 };
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{ 3 };

    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x       = 0.0;
        nebulae[i].rec.y       = 0.0;
        nebulae[i].rec.y       = 0.0;
        nebulae[i].rec.width   = nebula.width / 8;
        nebulae[i].rec.height  = nebula.height / 8;
        nebulae[i].pos.x       = windowDimensions[0] + 900 * i;
        nebulae[i].pos.y       = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame       = 0;
        nebulae[i].updateTime  = 1.0 / 16.0;
        nebulae[i].runningTime = 0.0;
    }

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

        playerData.pos.y       += velocity * dt;
        playerData.runningTime += dt;

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            if (nebulae[i].runningTime >= nebulae[i].updateTime)
            {
                nebulae[i].runningTime = 0.0;
                nebulae[i].rec.x       = nebulae[i].frame * nebulae[i].rec.width;
                nebulae[i].frame++;
                if (nebulae[i].frame > 7)
                {
                    nebulae[i].frame = 0;
                }
            }
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x       += nebulaVelocity * dt;
            nebulae[i].runningTime += dt;
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }

        DrawTextureRec(player, playerData.rec, playerData.pos, WHITE);

        EndDrawing();
    }

    UnloadTexture(nebula);
    UnloadTexture(player);
    CloseWindow();
}
