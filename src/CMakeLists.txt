set(SRC
  MudClient.cpp
)

include(${QT_USE_FILE})
qt4_automoc(${SRC})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(mudclient ${SRC} ${MOC_SRC})
target_link_libraries(mudclient ${QT_LIBRARIES})
