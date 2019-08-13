cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIRS} src)

add_executable(Asteroids src/main.cpp src/AsteroidGame.cpp src/CTexture.cpp src/CVector.cpp src/GameObject.cpp src/GameObjectAsteroid.cpp src/GameObjectExplosion.cpp src/GameObjectLaser.cpp src/GameObjectShip.cpp src/GameObjectStatic.cpp src/Menu.cpp src/MenuMain.cpp src/MenuPause.cpp src/MenuNext.cpp src/MenuGameOver.cpp)
target_link_libraries(Asteroids ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${SDL2_MIXER_LIBRARIES})
