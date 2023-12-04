#include <widgets/naivis.h>

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Naivis w;
  w.show();
  return a.exec();
}
