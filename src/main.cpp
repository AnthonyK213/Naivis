#include <Widget/naivis.h>

#include <QApplication>

int main(int argc, char *argv[]) {
#ifdef _WIN32
  // never use ANGLE on Windows, since OCCT 3D Viewer does not expect this
  QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#endif

  QApplication a(argc, argv);
  Naivis w;
  w.show();
  return a.exec();
}
