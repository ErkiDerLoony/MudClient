set(SRC
  MudClient.cpp
  UserInputParser.cpp
  ServerInputParser.cpp
  InputLine.cpp
  MudInputParser.cpp
  KeepAliveThread.cpp
  TimeThread.cpp
  Connection.cpp
)

set(QT_USE_QTNETWORK true)

include(${QT_USE_FILE})
qt4_automoc(${SRC})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(mudclient ${SRC} ${MOC_SRC})
target_link_libraries(mudclient ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARY})

install(TARGETS mudclient DESTINATION ${CMAKE_BINARY_DIR})
