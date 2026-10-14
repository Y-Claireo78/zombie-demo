add_library(UI STATIC
    UI.cpp
    DeploymentLane.cpp
    Sun.cpp 
)
target_include_directories(UI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})