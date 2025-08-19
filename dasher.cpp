#include "raylib.h"

enum Axis
{
    X,
    Y
};

struct AnimData
{
    Rectangle rec;
    Vector2   pos;
    int       frame;
    float     updateTime;
    float     runningTime;
};

bool isOnGround(AnimData& data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

void animateSprite(AnimData& data, int framesAmount)
{
    if (data.runningTime >= data.updateTime && isOnGround(data, 380))
    {
        data.runningTime = 0.0;
        data.rec.x       = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > framesAmount - 1)
        {
            data.frame = 0;
        }
    }
}

void updatePosition(AnimData& data, int velocity, float dt, Axis axis)
{
    if (axis == X)
    {
        data.pos.x += velocity * dt;
    }
    if (axis == Y)
    {
        data.pos.y += velocity * dt;
    }
    data.runningTime += dt;
}

void respawnNebula(AnimData& nebula, int windowWidth, int spacing, int index)
{
    if (nebula.pos.x <= -nebula.rec.width)
    {
        nebula.pos.x = windowWidth + spacing * index;
    }
}

void drawLayer(float x, Texture2D texture, float scale = 2.0f)
{
    Vector2 pos1{ x, 0.0f };
    DrawTextureEx(texture, pos1, 0.0f, scale, WHITE);

    Vector2 pos2{ x + texture.width * scale, 0.0f };
    DrawTextureEx(texture, pos2, 0.0f, scale, WHITE);
}

void updateLayer(float& x, Texture2D texture, float dt, float speed, float scale = 2.0f)
{
    x += -speed * dt;
    if (x <= -texture.width * scale)
    {
        x = 0.0f;
    }
}

int main()
{
    // --- Window / Game setup ---
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;
    const Color background_color{ BLACK };
    const int   target_fps{ 60 };
    bool        colision{};

    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher Game");
    SetTargetFPS(target_fps);

    // --- Physics ---
    const int gravity{ 1'300 };
    const int player_jump_velocity{ -600 };
    int       velocity{};

    // --- Background ---
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float     backgroundX{ 0 };
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float     foregroundX{ 0 };
    Texture2D middleground = LoadTexture("textures/back-buildings.png");
    float     middlegroundX{ 0 };

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
    const int sizeOfNebulae{ 5 };
    AnimData  nebulae[sizeOfNebulae];

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x       = 0.0;
        nebulae[i].rec.y       = 0.0;
        nebulae[i].rec.width   = nebula.width / 8;
        nebulae[i].rec.height  = nebula.height / 8;
        nebulae[i].pos.x       = 512 + 900 * i;
        nebulae[i].pos.y       = 380 - nebula.height / 8;
        nebulae[i].frame       = 0;
        nebulae[i].updateTime  = 1.0 / 16.0;
        nebulae[i].runningTime = 0.0;
    }
    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };

    while (!WindowShouldClose())
    {
        // Delta time
        const float dt{ GetFrameTime() };

        BeginDrawing();

        ClearBackground(background_color);

        finishLine += nebulaVelocity * dt;

        updateLayer(backgroundX, background, dt, 20.0f);
        drawLayer(backgroundX, background);

        updateLayer(middlegroundX, middleground, dt, 40.0f);
        drawLayer(middlegroundX, middleground);

        updateLayer(foregroundX, foreground, dt, 60.0f);
        drawLayer(foregroundX, foreground);

        // Game loop

        // Jump
        if (IsKeyDown(KEY_SPACE) && isOnGround(playerData, windowDimensions[1]))
        {
            velocity += player_jump_velocity;
        }

        animateSprite(playerData, 6);
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            animateSprite(nebulae[i], 8);
        }

        // Player
        updatePosition(playerData, velocity, dt, Y);
        if (isOnGround(playerData, windowDimensions[1]))
        {
            velocity = 0;
        }
        else
        {
            velocity += gravity * dt;
        }

        // Nebulas
        for (int i = 0; i < sizeOfNebulae; i++)

        {
            updatePosition(nebulae[i], nebulaVelocity, dt, X);
            // respawnNebula(nebulae[i], windowDimensions[0], 900, i);
        }
        for (AnimData nebula : nebulae)
        {
            float     padding{ 50 };
            Rectangle nebRec{
                nebula.pos.x + padding,
                nebula.pos.y + padding,
                nebula.rec.width - 2 * padding,
                nebula.rec.height - 2 * padding,
            };
            Rectangle playerRec{ playerData.pos.x,
                                 playerData.pos.y,
                                 playerData.rec.width,
                                 playerData.rec.height };

            if (CheckCollisionRecs(nebRec, playerRec))
            {
                colision = true;
            }
        }
        if (colision)
        {
            DrawText("Game Over!", windowDimensions[0] / 4, windowDimensions[1] / 2, 44, WHITE);
        }
        else if (playerData.pos.x >= finishLine + nebula.width)
        {
            DrawText("YOU WON!", windowDimensions[0] / 4, windowDimensions[1] / 2, 66, GREEN);
        }
        else
        {
            // Draw actors
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            DrawTextureRec(player, playerData.rec, playerData.pos, WHITE);
        }
        // Game loop end
        EndDrawing();
    }
    UnloadTexture(player);
    UnloadTexture(nebula);
    UnloadTexture(foreground);
    UnloadTexture(middleground);
    UnloadTexture(background);
    CloseWindow();
}
