#ifndef HELPER_HPP
#define HELPER_HPP

#include "raylib.h"
#include <random>
#include <filesystem>
#include <fstream>

std::mt19937 rng((unsigned)time(NULL));

namespace HamsterCrab {
    class Typewriter {
    public:
        std::string fullText;
        std::string displayText;
        float timer = 0.0f;
        int index = 0;
        float speed = 0.006f;
        int x = 0, y = 0;
        int fontSize = 20;
        bool drawtf = false;
        bool background = false;
        Color color = BLACK;

        Typewriter(std::string text, int px, int py, float spd = 0.05f, int size = 20, bool dtf = true, bool bgtf = true, Color col = BLACK)
        : fullText(text), x(px), y(py), speed(spd), fontSize(size), drawtf(dtf), background(bgtf), color(col) {}

        void Setup(Font font) {
            f = font;
        }

        void Update(bool& move_meter_draw, Vector2& move_vel) {
            timer += GetFrameTime();
            if (index < (int)fullText.size() && timer >= speed) {
                displayText += fullText[index];
                index++;
                timer = 0.0f;
            }

            if (IsKeyPressed(KEY_Z)) {
                if (displayText == fullText) {
                    drawtf = false;
                }
            }

            if (drawtf) {
                move_meter_draw = false;
                move_vel = {0.0f, 0.0f};
                return;
            }
        }

        void Draw() const {
            if (drawtf) {
                if (background) {
                    DrawRectangle(0, 430, 800, 230, {30, 30, 30, 255});
                }

                DrawTextEx(f, displayText.c_str(), {(float)x, (float)y}, fontSize, 3.0f, color);
            }
        }

        void Reset() {
            displayText.clear();
            timer = 0.0f;
            index = 0;
        }

    private:
        Font f;
    };

    void function_BounceCollision(Vector2& player, Vector2 player_vel, Rectangle wall) {
        Rectangle playerRect = {player.x, player.y, 60, 120};

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
                if (dx > 0) player.x += overlapX;
                else player.x -= overlapX;
                player_vel.x *= -0.9f;
            } else {
                if (dy > 0) player.y += overlapY;
                else player.y -= overlapY;
                player_vel.y *= -0.9f;
            }
        }
    }

    void function_loadGame(Vector2& xy) {
        std::filesystem::path path_save = "../data/save.hc";
        std::ifstream file(path_save);
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key;
            if (std::getline(iss, key, '=')) {
                std::string valueStr;
                if (std::getline(iss, valueStr)) {
                    int value = std::stoi(valueStr);
                    if (key == "x") xy.x = value;
                    else if (key == "y") xy.y = value;
                }
            }
        }
    }

    void function_saveGame(Vector2& xy) {
        std::filesystem::path path_save = "../data/save.hc";
        std::ofstream file(path_save);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing: " + path_save.string());
        }

        file << "x=" << xy.x << '\n';
        file << "y=" << xy.y << '\n';
    }

    float function_random_float(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }

    int function_random_int(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    void function_set_music(Music music, const char* path, float volume, float pitch, float pan) {
        music = LoadMusicStream(path);
        SetMusicVolume(music, volume);
        SetMusicPitch(music, pitch);
        SetMusicPan(music, pan);
    }

    void function_set_sound(Sound sound, const char* path, float volume, float pitch, float pan) {
        sound = LoadSound(path);
        SetSoundVolume(sound, volume);
        SetSoundPitch(sound, pitch);
        SetSoundPan(sound, pan);
    }

    bool CheckCollisionOBB(Rectangle rec1, Vector2 origin1, float rot1, Rectangle rec2, Vector2 origin2, float rot2) {
        auto getVertices = [](Rectangle rec, Vector2 origin, float rotation, Vector2 out[4]) {
            Vector2 center = {rec.x + origin.x, rec.y + origin.y};
            Vector2 local[4] = {
                {rec.x, rec.y}, {rec.x + rec.width, rec.y},
                {rec.x + rec.width, rec.y + rec.height}, {rec.x, rec.y + rec.height}
            };
            float s = sinf(rotation * DEG2RAD);
            float c = cosf(rotation * DEG2RAD);
            for (int i = 0; i < 4; i++) {
                float dx = local[i].x - center.x;
                float dy = local[i].y - center.y;
                out[i].x = dx * c - dy * s + center.x;
                out[i].y = dx * s + dy * c + center.y;
            }
        };

        auto project = [](Vector2* verts, int count, Vector2 axis, float& min, float& max) {
            min = max = verts[0].x * axis.x + verts[0].y * axis.y;
            for (int i = 1; i < count; i++) {
                float p = verts[i].x * axis.x + verts[i].y * axis.y;
                if (p < min) min = p;
                if (p > max) max = p;
            }
        };

        auto overlap = [&](Vector2* verts1, Vector2* verts2) {
            for (int i = 0; i < 4; i++) {
                Vector2 p1 = verts1[i];
                Vector2 p2 = verts1[(i+1)%4];
                Vector2 axis = {p2.y - p1.y, -(p2.x - p1.x)}; // 法線ベクトル
                float len = sqrtf(axis.x * axis.x + axis.y * axis.y);
                axis.x /= len; axis.y /= len;

                float min1, max1, min2, max2;
                project(verts1, 4, axis, min1, max1);
                project(verts2, 4, axis, min2, max2);

                if (max1 < min2 || max2 < min1) return false; // 分離している
            }
            return true;
        };

        Vector2 verts1[4], verts2[4];
        getVertices(rec1, origin1, rot1, verts1);
        getVertices(rec2, origin2, rot2, verts2);

        return overlap(verts1, verts2) && overlap(verts2, verts1);
    }
}

#endif
