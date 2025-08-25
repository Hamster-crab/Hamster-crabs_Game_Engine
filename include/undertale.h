#include "raylib.h"

struct Bullet {
    float x, y;
    float dx, dy;
    float rotation;
    float d_rotation;
    int width, height;
    bool active;
    int type;

    BulletPro()
    : x(0), y(0), dx(0), dy(0), width(0), height(0), active(false) {}

    void initialize(float startX, float startY, float speedX, float speedY, int w, int h, int t) {
        x = startX;
        y = startY;
        dx = speedX;
        dy = speedY;
        width = w;
        height = h;
        active = true;
        type = t;
    }

    void reset() {
        x = y = dx = dy = 0.0f;
        width = height = 0;
        active = false;
    }

    void update() {
        if (!active) return;
        x += dx * GetFrameTime();
        y += dy * GetFrameTime();
        if (x < -width || x > 800 || y < -height || y > 600) {
            active = false;
        }
    }

    void draw() const {
        if (active) {
            if (type == 1) {
                DrawRectangle((int)x, (int)y, width, height, {255, 255, 255, 255});
            }

            if (type == 2) {
                DrawRectangle((int)x, (int)y, width, height, {0, 0, 255, 255});
            }

            if (type == 3) {
                DrawRectangle((int)x, (int)y, width, height, {255, 255, 0, 255});
            }

            if (type == 4) {
                DrawTexture(texture_bullet_fire, (int)x, (int)y, {255, 255, 255, 255});
            }

            if (type == 5) {}

            if (type == 6) {
                DrawRectangle((int)x, (int)y, width, height, {255, 255, 255, 255});
            }
        }
    }
};

namespace HamsterCrab::undertale {
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
