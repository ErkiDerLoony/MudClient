#include <QApplication>

#include "MudClient.h"
#include "MudClient.moc"

MudClient::MudClient() {

}

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  MudClient mud;
  mud.show();
  return app.exec();
}
