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

#ifndef KEEP_ALIVE_THREAD_H
#define KEEP_ALIVE_THREAD_H

#include <QThread>
#include <QLineEdit>
#include <QTimer>
#include <QString>

#include "Connection.hpp"

/**
 * This thread periodically sends input to the MUD to prevent idling out.
 *
 * @author Edgar Kalkowski <eMail@edgar-kalkowski.de>
 */
class KeepAliveThread : public QThread {

Q_OBJECT

public:

  /**
   * Create a new KeepAliveThread.
   *
   * @param input       The input line that is used to detect whether the user
   *                    is actually active.
   * @param connection  The connection the random phrases shall be sent over.
   */
  KeepAliveThread(QLineEdit* input, Connection* connection);

  virtual ~KeepAliveThread();

  void run();

public slots:

  /**
   * This slot is connected to the internal timer’s timeout signal and sends a
   * random phrase to the server to keep the user online.
   */
  void send();

  /**
   * Reset the internal counter of this thread. This can be used to prevent this
   * thread from sending input when the user is in fact active.
   */
  void reset();

private:
  Connection* mConnection;
  QTimer* mTimer;
  static const int DELAY = 300000;

  /**
   * Create a random phrase that can be sent to the server.
   *
   * @return A random phrase to be sent to the server.
   */
  QString* getPhrase();

  QString* getSmile();
  QString* getCrack();
  QString* getBlink();
  QString* getMutter();
  QString* getScratch();

};

#endif // KEEP_ALIVE_THREAD_H
