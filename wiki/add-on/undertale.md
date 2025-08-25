弾幕の使い方

std::vector<HamsterCrab::undertale::Bullet> bullets(100000);

void bullet_spawn(float startX, float startY, float speedX, float speedY, float StartTimeX, float StartTimeY, float rota, float speedRota, float StartTimerRota, Vector2 o, int w, int h, int t, int d, float inv_MAX) {
    for (auto& b : bullets) {
        if (!b.active) {
            b.initialize(float startX, float startY, float speedX, float speedY, float StartTimeX, float StartTimeY, float rota, float speedRota, float StartTimerRota, Vector2 o, int w, int h, int t, int d, float inv_MAX);
            return;
        }
    }
    if ((int)bullets.size() < 100000) {
        HamsterCrab::undertale::Bullet newBullet;
        newBullet.initialize(float startX, float startY, float speedX, float speedY, float StartTimeX, float StartTimeY, float rota, float speedRota, float StartTimerRota, Vector2 o, int w, int h, int t, int d, float inv_MAX);
        bullets.push_back(newBullet);
    }
}

void updateBullets() {
    for (auto& b : bullets) {
        b.update();
    }
}

void drawBullets() {
    for (const auto& b : bullets) {
        b.draw();
    }
}

これをmain.cppに定義する。
あとは関数通りに
