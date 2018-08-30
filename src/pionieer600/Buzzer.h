#ifndef PIONIEER_600_BUZZER_H
#define PIONIEER_600_BUZZER_H

#include <uv.h>

namespace pionieer600 {

class Buzzer
{
public:
  Buzzer();
  ~Buzzer();

  void Setup(int period = DEFAULT_PERIOD);

  bool IsOn() const;

  void On();
  void Off();

private:
  void workerFunc_() const;

  bool worker_running_;
  int period_;

  uv_thread_t worker_;
public:
  enum
  {
    DEFAULT_PERIOD = 500
  };
};

} // namespace pionieer600

#endif // !PIONIEER_600_BUZZER_H
