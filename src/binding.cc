#include <node.h>
#include <v8.h>

#include "wiringPi/WiringPi.h"
#include "GpioLedBinding.hpp"
#include "JoystickBinding.hpp"
#include "BuzzerBinding.hpp"
#include "Bmp180Binding.hpp"

namespace binding {

void Init(v8::Local<v8::Object> exports) {
  wiringPi::WiringPi::Setup(); // setup wiring pi lib

  gpio_led::GpioLedInit(exports);
  joystick::JoysticInit(exports);
  buzzer::BuzzerInit(exports);
  bmp_180::Bmp180Init(exports);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

} // namespace binding
