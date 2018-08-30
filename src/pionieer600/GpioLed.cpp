#include "../wiringPi/WiringPi.h"

#include "GpioLed.h"

constexpr auto LED_PIN = 25;
constexpr auto OUTPUT = 1;

using namespace wiringPi;
using namespace pionieer600;

GpioLed::GpioLed()
{

}

GpioLed::~GpioLed()
{

}

void GpioLed::Setup()
{
  WiringPi::SetPinMode(LED_PIN, OUTPUT);
  WiringPi::DigitalWrite(LED_PIN, 0);
  isOn_ = false;
}

bool GpioLed::IsOn() const
{
  return isOn_;
}

void GpioLed::On()
{
  if (!isOn_) {
    WiringPi::DigitalWrite(LED_PIN, 1);
  }

  isOn_ = true;
}

void GpioLed::Off()
{
  if (isOn_) {
    WiringPi::DigitalWrite(LED_PIN, 0);
  }

  isOn_ = false;
}
