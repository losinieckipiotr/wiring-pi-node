#ifndef PIONIEER_600_GPIO_LED_H
#define PIONIEER_600_GPIO_LED_H

namespace pionieer600 {

class GpioLed
{
public:
  GpioLed();
  ~GpioLed();

  void Setup();
  bool IsOn() const { return isOn_; }
  void On();
  void Off();

private:
  bool isOn_;
};

} // namespace pionieer600

#endif // !PIONIEER_600_GPIO_LED_H
