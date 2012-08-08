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

#ifndef TIME_THREAD_HPP
#define TIME_THREAD_HPP

#include <QThread>
#include <QTextEdit>
#include <QTimer>

/**
 * This thread appends the current time to the program’s output every minute
 * unless the most recently added line was already a timeline (prevent spamming
 * the output with timelines).
 */
class TimeThread : public QThread {

Q_OBJECT

public:
  TimeThread(QTextEdit* output);
  virtual ~TimeThread();
  void run();

public slots:

  /**
   * This slot is connected to the textChanged signal of the QTextEdit that is
   * is the output. It is thus detected if some text is added to the output and
   * accordingly another timeline will be printed.
   */
  void reset();

  /**
   * This slot is connected to the internal timer and called periodically to
   * check if the current time shall be printed to the output.
   */
  void time();

private:
  QTextEdit* mOutput;
  int mMinute;
  bool mLastLineIsTime;
  QTimer* mTimer;

};

#endif // TIME_THREAD_HPP
