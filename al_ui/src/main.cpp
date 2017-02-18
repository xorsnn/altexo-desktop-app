#include "allogger.hpp"
#include "ui/mainwindow.hpp"

int main(int, char **) {
  MainWindow w;
  w.init();
  w.run();
  return 0;
}
