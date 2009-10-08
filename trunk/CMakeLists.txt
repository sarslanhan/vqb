find_package(KDE4 REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Qt4 REQUIRED)
pkg_search_module(Soprano REQUIRED soprano)

include_directories(
  ${KDE4_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${SOPRANO_INCLUDE_DIR}
  ${QT_INCLUDE_DIR}
  )

set(vqb_SRCS
  main.cpp
  vqbform.cpp
  querythread.cpp
  constraint.cpp
  combobox.cpp
  sparqlhighlighter.cpp
  subjecttree.cpp
  querynode.cpp
  )

kde4_add_ui_files(vqb_SRCS vqbform.ui)
kde4_add_executable(vqb ${vqb_SRCS})

target_link_libraries(vqb soprano sopranoclient
  ${KDE4_KDEUI_LIBS}
  ${SOPRANO_LIBRARIES}
  ${QT_QTCORE_LIBRARY}
  ${SOPRANO_CLIENT_LIBRARIES}
)

install(TARGETS vqb DESTINATION ${BIN_INSTALL_DIR})
