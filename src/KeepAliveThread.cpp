#include <cstdlib>

#include "KeepAliveThread.hpp"
#include "KeepAliveThread.moc"

KeepAliveThread::KeepAliveThread(QLineEdit* input, Connection* connection)
  : mConnection(connection) {
  mTimer = new QTimer();
  QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(send()));
  QObject::connect(input, SIGNAL(keyPressed()), this, SLOT(reset()));
}

KeepAliveThread::~KeepAliveThread() {
  delete mTimer;
}

void KeepAliveThread::send() {

  if (mConnection->isConnected()) {
    QString* phrase = getPhrase();

    if (phrase != NULL) {
      mConnection->send(*phrase);
      delete phrase;
    } else
      qFatal("NullPointerException!");
  }
}

void KeepAliveThread::reset() {
  mTimer->setInterval(DELAY);
}

void KeepAliveThread::run() {
  mTimer->start(DELAY);
  exec();
}

QString* KeepAliveThread::getPhrase() {
  int rnd = (double(rand())/RAND_MAX)*5;

  switch (rnd) {
  case 0: return getSmile();
  case 1: return getCrack();
  case 2: return getBlink();
  case 3: return getMutter();
  case 4: return getScratch();
  default: return NULL;
  }
}

QString* KeepAliveThread::getSmile() {
  int rnd = (double(rand())/RAND_MAX)*7;

  switch (rnd) {
  case 0: return new QString("smile cutely");
  case 1: return new QString("smile manically");
  case 2: return new QString("smile patiently");
  case 3: return new QString("smile innocently");
  case 4: return new QString("smile kindly");
  case 5: return new QString("smile politely");
  case 6: return new QString("smile brightly");
  default: return NULL;
  }
}

QString* KeepAliveThread::getCrack() {
  int rnd = (double(rand())/RAND_MAX)*7;

  switch (rnd) {
  case 0: return new QString("crack nose");
  case 1: return new QString("crack neck");
  case 2: return new QString("crack finger");
  case 3: return new QString("crack cheek");
  case 4: return new QString("crack ears");
  default: return NULL;
  }
}

QString* KeepAliveThread::getBlink() {
  int rnd = (double(rand())/RAND_MAX)*2;

  switch (rnd) {
  case 0: return new QString("blink full of boredom");
  case 1: return new QString("blink amazed of the silence");
  default: return NULL;
  }
}

QString* KeepAliveThread::getMutter() {
  int rnd = (double(rand())/RAND_MAX)*7;

  switch (rnd) {
  case 0: return new QString("mutter youth");
  case 1: return new QString("mutter dark robes");
  case 2: return new QString("mutter little green elves");
  case 3: return new QString("mutter dark spydry thygys");
  case 4: return new QString("mutter a stolen crucifix");
  case 5: return new QString("mutter HIM");
  case 6: return new QString("mutter control and world domination");
  default: return NULL;
  }
}

QString* KeepAliveThread::getScratch() {
  int rnd = (double(rand())/RAND_MAX)*5;

  switch (rnd) {
  case 0: return new QString("scratch nose");
  case 1: return new QString("scratch face");
  case 2: return new QString("scratch finger");
  case 3: return new QString("scratch cheek");
  case 4: return new QString("scratch ears");
  default: return NULL;
  }
}
