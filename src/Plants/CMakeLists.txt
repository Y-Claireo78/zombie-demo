add_library(Plants STATIC
    Plant.cpp
    Peashooter.cpp
    Sunflower.cpp
    WallNut.cpp
    Repeater.cpp 
)
target_include_directories(Plants PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Plants PUBLIC UI)