/*
 * © Copyright 2010–2012 by Edgar Kalkowski <eMail@edgar-kalkowski.de>
 * 
 * This file is part of Erki’s MudClient.
 * 
 * Erki’s MudClient is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
