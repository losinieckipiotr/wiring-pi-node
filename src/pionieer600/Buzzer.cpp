#include "../wiringPi/WiringPi.h"

#include "Buzzer.h"

using namespace wiringPi;
using namespace pionieer600;

constexpr auto BEEP = 64 + 7;

Buzzer::Buzzer()
{

}

Buzzer::~Buzzer()
{

}

void Buzzer::Setup(int period)
{
  worker_running_ = false;
  period_ = period;
  WiringPi::PCF8574Setup();
}

bool Buzzer::IsOn() const
{
  return worker_running_;
}

void Buzzer::On()
{
  if (worker_running_ == false)
  {
    Off();
    worker_running_ = true; // worker will be started again
    uv_thread_create(&worker_, [](void *arg)
    {
      auto self = static_cast<Buzzer*>(arg);
      self->workerFunc_();
    }, this);
  }
}

void Buzzer::Off()
{
  if (worker_running_ == true)
  {
    worker_running_ = false;
    uv_thread_join(&worker_);
  }
}

void Buzzer::workerFunc_() const
{
  while (worker_running_)
  {
    WiringPi::DigitalWrite(BEEP, 0);
    WiringPi::Delay(period_ / 2);
    WiringPi::DigitalWrite(BEEP, 1);
    WiringPi::Delay(period_ / 2);
  }
  WiringPi::DigitalWrite(BEEP, 1);
}
