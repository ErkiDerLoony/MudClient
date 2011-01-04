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
