#include <node.h>
#include <v8.h>

#include "wiringPi/WiringPi.h"
#include "pionieer600/GpioLedBinding.hpp"
#include "pionieer600/JoystickBinding.hpp"

namespace binding {

void Init(v8::Local<v8::Object> exports) {
  wiringPi::WiringPi::Setup(); // setup wiring pi lib
  pionieer600::binding::GpioLedInit(exports);
  pionieer600::binding::JoysticInit(exports);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

} // namespace binding
