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
}

#endif
