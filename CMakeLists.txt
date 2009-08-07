project( vqb )
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} )

set(vqb_SRCS
  main.cpp
  vqbform.cpp
  )

kde4_add_ui_files(vqb_SRCS vqbform.ui)
kde4_add_executable(vqb ${vqb_SRCS})

target_link_libraries(vqb
  ${QT_QTCORE_LIBRARY}
  ${KDE4_KDEUI_LIBS})

install(TARGETS vqb DESTINATION ${BIN_INSTALL_DIR})

