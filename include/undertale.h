#include "raylib.h"

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

    Bullet()
    : x(0), y(0), dx(0), dy(0), dx_start(0), dy_start(0), rotation(0), dr(0), dr_start(0), origin{0.0f, 0.0f}, width(0), height(0), active(false), timer(0), type(0) {}

    void initialize(float startX, float startY, float speedX, float speedY, float StartTimeX, float StartTimeY, float rota, float speedRota, float StartTimerRota, Vector2 o, int w, int h, int t) {
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
    }

    void reset() {
        x = y = dx = dy = dx_start = dy_start = 0.0f;
        rotation = dr = dr_start = 0.0f;
        origin = {0.0f, 0.0f};
        width = height = 0;
        active = false;
        timer = 0.0f;
    }

    void update() {
        if (!active) return;
        x += dx * GetFrameTime();
        y += dy * GetFrameTime();
    }

    void draw() const {
        if (active) {
            DrawRectangle((int)x, (int)y, width, height, {255, 255, 255, 255});
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
