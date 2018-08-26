#include <node.h>
#include <v8.h>

#include "GpioLed.h"

namespace pionieer600 {
namespace binding {

static GpioLed gpioLed;

void IsOn(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(v8::Boolean::New(isolate, gpioLed.IsOn()));
}

void On(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gpioLed.On();
}

void Off(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gpioLed.Off();
}

void GpioLedInstance(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Object> gpioLedObj = v8::Object::New(isolate);

  NODE_SET_METHOD(gpioLedObj, "isOn", IsOn);
  NODE_SET_METHOD(gpioLedObj, "on", On);
  NODE_SET_METHOD(gpioLedObj, "off", Off);

  args.GetReturnValue().Set(gpioLedObj);
}

void GpioLedInit(v8::Local<v8::Object> exports) {
  gpioLed.Setup();

  NODE_SET_METHOD(exports, "gpioLed", GpioLedInstance);
}

} // namepsace binding
} // namespace pionieer600