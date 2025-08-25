#ifndef UNDERTALE_HPP
#define UNDERTALE_HPP

#include "raylib.h"
#include "Hamster-crab/helper.hpp"

namespace HamsterCrab::undertale {
    namespace bullet {
        struct Bullet {
            float x, y;
            float dx, dy;
            float dx_start, dy_start;
            float rotation;
            float dr;
            float dr_start;
            Vector2 origin;
            int width, height;
            bool active;
            float timer;
            int type;
            int damage;
            float invincible_MAX;

            Bullet()
            : x(0), y(0), dx(0), dy(0), dx_start(0), dy_start(0), rotation(0), dr(0), dr_start(0), origin{0.0f, 0.0f}, width(0), height(0), active(false), timer(0), type(0), damage(0), invincible_MAX(0) {}

            void initialize(float startX, float startY, float speedX, float speedY, float StartTimeX, float StartTimeY, float rota, float speedRota, float StartTimerRota, Vector2 o, int w, int h, int t, int d, float inv_MAX) {
                x = startX;
                y = startY;
                dx = speedX;
                dy = speedY;
                dx_start = StartTimeX;
                dy_start = StartTimeY;
                rotation = rota;
                dr = speedRota;
                dr_start = StartTimerRota;
                origin = o;
                width = w;
                height = h;
                active = true;
                timer = 0.0f;
                type = t;
                damage = d;
                invincible_MAX = inv_MAX;
            }

            void reset() {
                x = y = dx = dy = dx_start = dy_start = 0.0f;
                rotation = dr = dr_start = 0.0f;
                origin = {0.0f, 0.0f};
                width = height = 0;
                active = false;
                timer = 0.0f;
                damage = 0;
                invincible_MAX = 0.0f;
            }

            void update() {
                timer += GetFrameTime();

                if (!active) return;

                if (timer > dx_start) {x += dx * GetFrameTime();}

                if (timer > dy_start) {y += dy * GetFrameTime();}

                if (timer > dr_start) {rotation += dr * GetFrameTime();}
            }

            void hit(Rectangle player, int hp) {
                static float invincible;
                if (active) {
                    if (CheckCollisionOBB(player, {0.0f, 0.0f}, 0.0f, {x, y, (float)width, (float)height}, origin, rotation)) {
                        invincible += GetFrameTime();
                        if (invincible > invincible_MAX) {
                            hp -= damage;
                            invincible = 0.0f;
                        }
                    }
                }
            }

            void draw() const {
                if (active) {
                    DrawRectanglePro({x, y, (float)width, (float)height}, origin, rotation, {255, 255, 255, 255});
                }
            }
        };
    }

    Rectangle rect_frame = {300, 270, 200, 200};

    void update_frame_change(Rectangle target, int frame, int durationFrames) {
        if (frame >= durationFrames) rect_frame = target;

        float t = (float)frame / (float)durationFrames;

        Rectangle result;
        result.x = rect_frame.x + (target.x - rect_frame.x) * t;
        result.y = rect_frame.y + (target.y - rect_frame.y) * t;
        result.width  = rect_frame.width  + (target.width  - rect_frame.width)  * t;
        result.height = rect_frame.height + (target.height - rect_frame.height) * t;
        rect_frame = result;
    }

    void draw_frame() {
        DrawRectangleLinesEx(rect_frame, 3.0f, {255, 255, 255, 255});
    }
}

#endif
