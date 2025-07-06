#include <raylib.h>
#include <fonts/nomalJPFonts.h>

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    // 初期化
    // ウィンドウの幅と高さを定義
    const int screenWidth = 800;
    const int screenHeight = 500;

    // ウィンドウを作成し、サイズ変更可能に設定
    InitWindow(screenWidth, screenHeight, "");

    int fontCodepoints[2027048];
    int charsCount = 30;

    Font nomalJPFonts = LoadFontFromMemory(".ttf",
                                           DotGothic16_Regular_ttf,
                                           DotGothic16_Regular_ttf_len,
                                           300,
                                           fontCodepoints,
                                           charsCount);

    // FPSを設定 (オプション)
    SetTargetFPS(60);

    // ゲームループ
    while (!WindowShouldClose())    // ESCキーが押されるか、クローズボタンがクリックされるまで
    {
        // 描画開始
        BeginDrawing();

        ClearBackground(RAYWHITE); // 背景を白でクリア
        DrawTextEx(nomalJPFonts, "こんにちは、世界！", (Vector2){ 50, 50 }, 30, 2, BLACK);

        // 描画終了
        EndDrawing();
    }

    // クリーンアップ
    CloseWindow();

    return 0;
}
