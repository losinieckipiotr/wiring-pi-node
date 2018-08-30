#ifndef PIONIEER_600_JOYSTICK_H
#define PIONIEER_600_JOYSTICK_H

#include <functional>

#include <uv.h>

namespace pionieer600 {

using HandlerT = std::function<void(void)>;

class Joystick
{
public:
  Joystick();
  ~Joystick();

  void Setup();

  void Watch(HandlerT clickHandler, HandlerT holdHandler);
  void StopWatch();

private:
  void StartWorker();
  void ShutdownWorker();

  void DoPooling_();

  bool work_;
  int holdCtr_;

  HandlerT clickHandler_;
  HandlerT holdHandler_;

  uv_thread_t worker_;
  uv_async_t asyncClick_;
  uv_async_t asyncHold_;
public:
  enum
  {
    INTERVAL = 100,
    HOLD_TIME = 1000,
    HOLD_TICKS = HOLD_TIME / INTERVAL
  };
};

} // namespace pionieer600

#endif // !PIONIEER_600_JOYSTICK_H
