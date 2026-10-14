add_library(Zombies STATIC
    Zombie.cpp
    RegularZombie.cpp
    PoleVaultingZombie.cpp 
)
target_include_directories(Zombies PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})