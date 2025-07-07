#include <raylib.h>
#include <fonts/nomalJPFonts.h>
#include <func.h>
#include <string>

int width;
int height;

std::string windowType = "title";

void DrawTitle(Font nomalJPFonts) {
    Vector2 mousePos = GetMousePosition();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (mousePos.x >= width/2-60 && mousePos.x <= width/2-60+90 && mousePos.y >= height/1.5 && mousePos.y <= height/1.5+30) {
        DrawRectangleLines(width/2-60, height/1.5, 90, 30, RED);
    }
    if (mousePos.x >= width/2-50 && mousePos.x <= width/2-50+60 && mousePos.y >= height/1.2 && mousePos.y <= height/1.2+30) {
        DrawRectangleLines(width/2-50, height/1.2, 60, 30, RED);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CloseWindow();
    }

    DrawTextEx(nomalJPFonts, "はじめる", (Vector2){width/2-60, height/1.5}, 30, 2, BLACK);
    DrawTextEx(nomalJPFonts, "とじる", (Vector2){width/2-50, height/1.2}, 30, 2, BLACK);

    EndDrawing();
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    // 初期化
    // ウィンドウの幅と高さを定義
    const int screenWidth = 800;
    const int screenHeight = 500;

    InitWindow(screenWidth, screenHeight, "");

    Font nomalJPFonts = CreateUniversalJPFont(30);

    // FPSを設定 (オプション)
    SetTargetFPS(60);

    // ゲームループ
    while (!WindowShouldClose())
    {
        width = GetScreenWidth();
        height = GetScreenHeight();

        if (windowType == "title") {
            DrawTitle(nomalJPFonts);
        }
    }

    // クリーンアップ
    CloseWindow();

    return 0;
}
