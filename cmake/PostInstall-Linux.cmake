execute_process(
  COMMAND $ENV{LINUXDEPLOYQT}
    ${CMAKE_INSTALL_PREFIX}/bin/Naivis
    -unsupported-allow-new-glibc
    -qmake="$ENV{QT_DIR}/bin/qmake"
  )
