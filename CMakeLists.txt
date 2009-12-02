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
  vqbmainwindow.cpp
  vqbform.cpp
  vqbglobal.cpp
  vqbschemaselect.cpp
  vqbschemaconstruct.cpp
  vqbinstancesselect.cpp
  vqbinstancesconstruct.cpp
  querythread.cpp
  combobox.cpp
  sparqlhighlighter.cpp
  subjecttree.cpp
  querynode.cpp
  completerlineedit.cpp
  )

kde4_add_ui_files(vqb_SRCS vqbmainwindow.ui vqbinstancesselect.ui vqbinstancesconstruct.ui vqbschemaconstruct.ui)
kde4_add_executable(vqb ${vqb_SRCS})

target_link_libraries(vqb soprano sopranoclient
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${SOPRANO_LIBRARIES}
  ${QT_QTCORE_LIBRARY}
  ${SOPRANO_CLIENT_LIBRARIES}
  ${KDE4_PLASMA_LIBS}
)

install(TARGETS vqb DESTINATION ${BIN_INSTALL_DIR})
