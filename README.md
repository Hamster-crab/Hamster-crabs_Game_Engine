[説明書]
# ここを読んでもわからないものがあるのならば、 "hamstercrab123@gmail.com" まで

# 設定
### まず、CMake, make, gcc(g++)は設定済みであるのが条件
### このリポジトリをcloneする
### 開く
### mkdir build # buildディレクトリの作成
### cd build
### cmake .. # cmakeの初期設定
### make # ビルド
### 簡単だね
### 次からはbuildディレクトリに入ったのならば "make" だけでいいよ

# セーブアンドロード
### function_saveGame(); 関数で "data/save.hc" に保存
### function_loadGame(); 関数で "data/save.hc" から読み取りしてこちら側に変更を取り込む

## function_loadGame(); 関数は270行目で設定済み
## function_saveGame(); 関数は好きなところにいれて

# タイプライター風文字送り
## Typewriter twText_1("Press the arrow keys.\nEach press moves you one space.\nBTW, you can run by pressing the E key.\nPress ESC key is exit.", 130, 430, 0.01f, 40, {255, 255, 255, 255});
## このような感じなものをどこかしらで作り、
## twText_1.Update();
## twText_1.Draw();
## で更新&表示をする
## 表示のところを変えたいのならば、class Typewriterのvoid Draw()を変えて

# 関数
## void BounceCollision(Rectangle wall);
#### この関数は、Rectangleとplayerが重ならないように押し出す関数

## void function_loadGame();
## void function_saveGame();
#### これらは "# セーブアンドロード" をご覧ください

## void function_function_title();
#### タイトル画面の更新&描画をする物

## void function_main();
#### メインゲーム内の更新&描画をするもの
