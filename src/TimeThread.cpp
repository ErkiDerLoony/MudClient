#include <QTime>

#include "TimeThread.hpp"
#include "TimeThread.moc"

TimeThread::TimeThread(QTextEdit* output)
  : mOutput(output), mLastLineIsTime(false) {
  QTime time;
  time.start();
  mMinute = time.minute();

  mTimer = new QTimer();
  QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(time()));

  QObject::connect(output, SIGNAL(textChanged()), this, SLOT(reset()));
}

TimeThread::~TimeThread() {
  delete mTimer;
}

void TimeThread::run() {
  QTime time;
  time.start();
  mTimer->start((61 - time.second()) * 1000);
  exec();
}

void TimeThread::time() {
  QTime time;
  time.start();

  if (!mLastLineIsTime && time.minute() != mMinute) {
    QString text = QString("<font color=grey>");
    text += "--------------------------------------- ";
    text += time.toString("hh:mm");
    text += " ---------------------------------------";
    text += "</font><br>";
    mOutput->textCursor().insertHtml(text);
    mMinute = time.minute();
    mLastLineIsTime = true;
  }

  mTimer->setInterval((61 - time.second()) * 1000);
}

void TimeThread::reset() {
  mLastLineIsTime = false;
}
