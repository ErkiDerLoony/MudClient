#ifndef KEEP_ALIVE_THREAD_H
#define KEEP_ALIVE_THREAD_H

/**
 * This thread periodically sends input to the MUD to prevent idling out.
 *
 * @author Edgar Kalkowski <eMail@edgar-kalkowski.de>
 */
class KeepAliveThread : public QThread {

public:
  KeepAliveThread();
  virtual ~KeepAliveThread() {}
  void run();

  /**
   * Reset the internal counter of this thread. This can be used to prevent this
   * thread from sending input when the user is in fact active.
   */
  void reset();

};

#endif // KEEP_ALIVE_THREAD_H
