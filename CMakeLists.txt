cmake_minimum_required(VERSION 3.10) # CMakeの最小バージョンを指定

project(main LANGUAGES CXX) # プロジェクト名と使用言語を設定

# Raylibを探す
# Raylibがシステムにインストールされている場合、find_packageで見つけられます。
# インストールされていない場合や特定のバージョンを使用したい場合は、
# RaylibのソースをFetchContent_Declareでサブディレクトリに追加する方法もあります。
find_package(raylib CONFIG REQUIRED)

# 実行可能ファイルを作成
add_executable(main src/main.cpp)

# プロジェクトのヘッダーファイルが存在するディレクトリを指定
# これにより、ソースファイルが `#include "my_header.h"` のように記述できるようになります
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Raylibライブラリをリンク
# find_package(raylib CONFIG) が見つけた raylib ターゲットをリンクします。
# これにより、Raylibのインクルードディレクトリも自動的に設定されることがほとんどです。
target_link_libraries(main PRIVATE raylib)

# 使用するC++標準を設定
# C++17 を推奨します。必要に応じて他のバージョン (例: CXX_STANDARD 20) に変更してください。
set_property(TARGET main PROPERTY CXX_STANDARD 17)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET main PROPERTY CXX_EXTENSIONS OFF)
