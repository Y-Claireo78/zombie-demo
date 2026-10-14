add_library(Projectiles STATIC
    Pea.cpp
)
target_include_directories(Projectiles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})