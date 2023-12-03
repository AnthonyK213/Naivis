include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_SOURCES
  main.cpp
  naivis/naivis.cpp
  naivis/naivis.h
  naivis/naivis.ui
  occtViewer/occtViewer.cpp
  occtViewer/occtViewer.h
  mesh/HalfEdgeMesh_DataSource.cxx
  mesh/HalfEdgeMesh_DataSource.hxx
  mesh/XSDRAWSTLVRML_DataSource.cxx
  mesh/XSDRAWSTLVRML_DataSource.hxx
  io/documentManager.cpp
  io/documentManager.h
  io/logStream.h
  )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(Naivis
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    )
  # Define target properties for Android with Qt 6 as:
  #    set_property(TARGET Naivis APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
  # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(Naivis SHARED
      ${PROJECT_SOURCES}
      )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(Naivis
      ${PROJECT_SOURCES}
      )
  endif()
endif()

target_link_libraries(
  Naivis
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::OpenGLWidgets
  Qt${QT_VERSION_MAJOR}::OpenGL
  ${OpenCASCADE_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Naivis)
endif()
set_target_properties(Naivis PROPERTIES
  ${BUNDLE_ID_OPTION}
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
  )

include(GNUInstallDirs)
install(TARGETS Naivis
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(Naivis)
endif()
