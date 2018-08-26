#include "../wiringPi/WiringPi.h"

#include "Joystick.h"

constexpr auto PIN = 28;

using namespace std;
using namespace wiringPi;
using namespace pionieer600;

Joystick::Joystick()
{

}

Joystick::~Joystick()
{

}

void Joystick::Setup()
{
  WiringPi::SetPinMode(PIN, 0);//input mode
  WiringPi::pullUpDn(PIN, 2);//pull up
}

void Joystick::Watch(HandlerT clickHandler, HandlerT holdHandler) {
  clickHandler_ = clickHandler;
  holdHandler_ = holdHandler;

  StartWorker();
}

void Joystick::StopWatch() {
  ShutdownWorker();

  uv_close(reinterpret_cast<uv_handle_t*>(&asyncClick_), nullptr);
  uv_close(reinterpret_cast<uv_handle_t*>(&asyncHold_), nullptr);
}

void Joystick::StartWorker()
{
  loop = uv_default_loop();
  uv_async_init(loop, &asyncClick_, [](uv_async_t *handle)
  {
    auto self = static_cast<Joystick*>(handle->data);
    self->clickHandler_();
  });
  uv_async_init(loop, &asyncHold_, [](uv_async_t *handle)
  {
    auto self = static_cast<Joystick*>(handle->data);
    self->holdHandler_();
  });

  ShutdownWorker();
  work_ = true;
  uv_thread_create(&worker_, [](void *arg) {
    auto self = static_cast<Joystick*>(arg);
    self->DoPooling_();
  }, this);
}

void Joystick::DoPooling_()
{
  while (work_)
  {
    holdCtr_ = 0;
    if (WiringPi::DigitalRead(PIN) == 0)
    {
      while (WiringPi::DigitalRead(PIN) == 0 && work_)
      {
        if (holdCtr_ == HOLD_TICKS) {
          asyncHold_.data = static_cast<void*>(this); // can be done only once?
          uv_async_send(&asyncHold_);
        }

        ++holdCtr_;
        WiringPi::Delay(INTERVAL);
      }
      if(holdCtr_ < HOLD_TICKS) {
        asyncClick_.data = static_cast<void*>(this);
        uv_async_send(&asyncClick_);
      }

    }
    WiringPi::Delay(INTERVAL);
  }
}

void Joystick::ShutdownWorker()
{
  work_ = false;
  // TOREMOVE
  // if (worker_.joinable())
  //   worker_.join();
  uv_thread_join(&worker_);
}
