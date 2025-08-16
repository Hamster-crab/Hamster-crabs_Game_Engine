#include "raylib.h"
#include <cmath>
#include <string>
#include <filesystem>
#include <fstream>

int screenWidth;
int screenHeight;
int number_title;

float number_main_player_move_speed;
bool number_main_player_move_meter;
float number_main_player_move_dash;
const float number_main_player_move_dash_MAX = 5000.0f;
int number_main_player_move_animation_number;
float number_main_player_move_animation_timer;

std::string string_main_player_move_direction;

Rectangle rect_title_mouse;

Vector2 Vec2_main_player;
Vector2 number_main_player_move_vel = {0, 0};

RenderTexture2D target;

Camera2D camera_title;
Camera2D camera_main;

Font font_DotGothic;

bool bool_force_exit;

std::filesystem::path path_save;

const Rectangle rect_main_map_obj_1 = {-300, -800, 1280, 505};
const Rectangle rect_main_map_obj_2 = {-300, -300, 5, 1080};
const Rectangle rect_main_map_obj_3 = {-300, 780, 1280, 5};
const Rectangle rect_main_map_obj_4 = {980, -300, 5, 1080};

class Typewriter {
public:
    std::string fullText;
    std::string displayText;
    float timer = 0.0f;
    int index = 0;
    float speed = 0.05f;
    int x = 0, y = 0;
    int fontSize = 20;
    Color color = BLACK;

    Typewriter(std::string text, int px, int py, float spd = 0.05f, int size = 20, Color col = BLACK)
    : fullText(text), x(px), y(py), speed(spd), fontSize(size), color(col) {}

    void Update() {
        timer += GetFrameTime();
        if (index < (int)fullText.size() && timer >= speed) {
            displayText += fullText[index];
            index++;
            timer = 0.0f;
        }
    }

    void Draw() const {
        DrawRectangle(0, 430, 800, 230, {30, 30, 30, 255});
        DrawTextEx(font_DotGothic, displayText.c_str(), {(float)x, (float)y}, fontSize, 3.0f, color);
    }

    void Reset() {
        displayText.clear();
        timer = 0.0f;
        index = 0;
    }
};

Typewriter twText_1("Press the arrow keys.\nEach press moves you one space.\nBTW, you can run by pressing the E key.\nPress ESC key is exit.", 130, 430, 0.01f, 40, {255, 255, 255, 255});

void BounceCollision(Rectangle wall) {
    Rectangle playerRect = {Vec2_main_player.x, Vec2_main_player.y, 60, 120};

    if (CheckCollisionRecs(playerRect, wall)) {
        float playerCenterX = playerRect.x + playerRect.width / 2.0f;
        float playerCenterY = playerRect.y + playerRect.height / 2.0f;
        float wallCenterX = wall.x + wall.width / 2.0f;
        float wallCenterY = wall.y + wall.height / 2.0f;

        float dx = playerCenterX - wallCenterX;
        float dy = playerCenterY - wallCenterY;

        float overlapX = (playerRect.width / 2.0f + wall.width / 2.0f) - fabsf(dx);
        float overlapY = (playerRect.height / 2.0f + wall.height / 2.0f) - fabsf(dy);

        if (overlapX < overlapY) {
            if (dx > 0) Vec2_main_player.x += overlapX;
            else        Vec2_main_player.x -= overlapX;
            number_main_player_move_vel.x *= -0.9f;
        } else {
            if (dy > 0) Vec2_main_player.y += overlapY;
            else        Vec2_main_player.y -= overlapY;
            number_main_player_move_vel.y *= -0.9f;
        }
    }
}

void function_loadGame() {
    std::ifstream file(path_save);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string valueStr;
            if (std::getline(iss, valueStr)) {
                int value = std::stoi(valueStr);
                if (key == "x") Vec2_main_player.x = value;
                else if (key == "y") Vec2_main_player.y = value;
            }
        }
    }
}

void function_saveGame() {
    std::ofstream file(path_save);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + path_save.string());
    }

    file << "x=" << Vec2_main_player.x << '\n';
    file << "y=" << Vec2_main_player.y << '\n';
}

void function_function_title() {
    if (rect_title_mouse.y <= 345) {
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) rect_title_mouse.y = 405;

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_Z)) {
            number_title = 2;
        }
    } else if (rect_title_mouse.y == 405) {
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) rect_title_mouse.y = 465;
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) rect_title_mouse.y = 345;
    } else if (rect_title_mouse.y >= 465) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) rect_title_mouse.y = 405;

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_Z)) bool_force_exit = true;
    }

    BeginTextureMode(target);
    ClearBackground({0, 0, 0, 255});
    BeginMode2D(camera_title);
    DrawTextEx(font_DotGothic, "Game", {320.0f, 350.0f}, 50.0f, 3.0f, {255, 255, 255, 255}); // 345
    DrawTextEx(font_DotGothic, "Config", {320.0f, 410.0f}, 50.0f, 3.0f, {255, 255, 255, 255}); // 405
    DrawTextEx(font_DotGothic, "Exit", {320.0f, 470.0f}, 50.0f, 3.0f, {255, 255, 255, 255}); // 465
    DrawRectangleLinesEx(rect_title_mouse, 5.0f, {255, 255, 255, 255});
    EndMode2D();
    EndTextureMode();
}

void function_main() {
    number_main_player_move_vel = {0.0f, 0.0f};

    if (number_main_player_move_dash > 0.0f) {
        if (IsKeyDown(KEY_E)) {
            number_main_player_move_dash -= 1000.0f*GetFrameTime();
            number_main_player_move_speed = 3000.0f;
        } else {
            number_main_player_move_speed = 1000.0f;
            number_main_player_move_dash += 800.0f*GetFrameTime();
        }
    } else {
        number_main_player_move_speed = 1000.0f;
        number_main_player_move_dash += 800.0f*GetFrameTime();
    }

    if (number_main_player_move_dash < number_main_player_move_dash_MAX) {
        number_main_player_move_meter = true;
    } else {
        number_main_player_move_meter = false;
        number_main_player_move_dash = number_main_player_move_dash_MAX;
    }

    if (IsKeyDown(KEY_UP)) {
        number_main_player_move_vel.y = -number_main_player_move_speed;
        string_main_player_move_direction = "up";
    }

    if (IsKeyDown(KEY_DOWN)) {
        number_main_player_move_vel.y = number_main_player_move_speed;
        string_main_player_move_direction = "down";
    }

    if (IsKeyDown(KEY_LEFT)) {
        number_main_player_move_vel.x = -number_main_player_move_speed;
        string_main_player_move_direction = "left";
    }

    if (IsKeyDown(KEY_RIGHT)) {
        number_main_player_move_vel.x = number_main_player_move_speed;
        string_main_player_move_direction = "right";
    }

    Vec2_main_player.x += number_main_player_move_vel.x * GetFrameTime();
    Vec2_main_player.y += number_main_player_move_vel.y * GetFrameTime();

    BounceCollision(rect_main_map_obj_1);
    BounceCollision(rect_main_map_obj_2);
    BounceCollision(rect_main_map_obj_3);
    BounceCollision(rect_main_map_obj_4);

    camera_main.target = Vec2_main_player;

    twText_1.Update();

    BeginTextureMode(target);
    ClearBackground({30, 52, 93, 255});
    BeginMode2D(camera_main);

    DrawRectangleRec(rect_main_map_obj_1, {255, 0, 0, 255});
    DrawRectangleRec(rect_main_map_obj_2, {255, 0, 0, 255});
    DrawRectangleRec(rect_main_map_obj_3, {255, 0, 0, 255});
    DrawRectangleRec(rect_main_map_obj_4, {255, 0, 0, 255});

    DrawRectangleV(Vec2_main_player, {60, 120}, {255, 0, 0, 255});

    EndMode2D();

    twText_1.Draw();

    EndTextureMode();
}

int main() {
    SetTraceLogLevel(LOG_ALL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    screenWidth = 800;
    screenHeight = 600;
    number_title = 1;
    number_main_player_move_speed = 1000.0f;
    number_main_player_move_dash = number_main_player_move_dash_MAX;
    number_main_player_move_animation_number = 1;
    number_main_player_move_animation_timer = 0.0f;
    string_main_player_move_direction = "down";

    InitWindow(screenWidth, screenHeight, "Hamster-crab's Game engine");
    SetExitKey(KEY_NULL);

    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    camera_title.offset = {0, 0};
    camera_title.target = {0, 0};
    camera_title.rotation = 0.0f;
    camera_title.zoom = 1.0f;

    camera_main.offset = {375, 250};
    camera_main.target = Vec2_main_player;
    camera_main.rotation = 0.0f;
    camera_main.zoom = 0.5f;

    rect_title_mouse = {295, 345, 205, 60};
    Vec2_main_player = {0, 0};

    bool_force_exit = false;

    font_DotGothic = LoadFont("../fonts/DotGothic16/DotGothic16-Regular.ttf");

    path_save = "../data/save.hc";
    function_loadGame();

    SetTargetFPS(120);

    while (!WindowShouldClose())
    {
        if (bool_force_exit) {
            break;
        }

        switch (number_title) {
            case 1:
                function_function_title();
                break;

            case 2:
                if (IsKeyPressed(KEY_ESCAPE)) {
                    bool_force_exit = true;
                }

                function_main();
                break;
        }

        float screenRatio = (float)GetScreenWidth() / GetScreenHeight();
        float targetRatio = (float)screenWidth / screenHeight;

        Rectangle src = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
        Rectangle dest;

        if (screenRatio > targetRatio) {
            float scale = (float)GetScreenHeight() / screenHeight;
            float width = screenWidth * scale;
            float offsetX = (GetScreenWidth() - width) / 2.0f;
            dest = { offsetX, 0, width, (float)GetScreenHeight() };
        } else {
            float scale = (float)GetScreenWidth() / screenWidth;
            float height = screenHeight * scale;
            float offsetY = (GetScreenHeight() - height) / 2.0f;
            dest = { 0, offsetY, (float)GetScreenWidth(), height };
        }

        BeginDrawing();
        ClearBackground({0, 0, 0, 255});
        DrawTexturePro(target.texture, src, dest, { 0, 0 }, 0.0f, WHITE);
        DrawFPS(0, 0);
        DrawText(TextFormat("window_width: %d", GetScreenWidth()), 0, 30, 20, {255, 255, 255, 255});
        DrawText(TextFormat("window_height: %d", GetScreenHeight()), 0, 50, 20, {255, 255, 255, 255});
        DrawText(TextFormat("player_x: %.3f", Vec2_main_player.x), 0, 70, 20, {255, 255, 255, 255});
        DrawText(TextFormat("player_y: %.3f", Vec2_main_player.y), 0, 90, 20, {255, 255, 255, 255});
        DrawText(TextFormat("direction: %s", string_main_player_move_direction.c_str()), 0, 110, 20, {255, 255, 255, 255});

        if (number_main_player_move_meter) {
            DrawRectangle(400, 420, number_main_player_move_dash/15, 2, {255, 255, 255, 255});
            DrawRectangle(400-number_main_player_move_dash/15, 420, number_main_player_move_dash/15, 2, {255, 255, 255, 255});
            DrawCircle(400, 420, 10.0f, {255, 255, 255, 255});
        }
        EndDrawing();
    }

    UnloadRenderTexture(target);

    UnloadFont(font_DotGothic);

    CloseWindow();

    return 0;
}
