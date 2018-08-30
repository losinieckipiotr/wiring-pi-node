#include <node.h>
#include <v8.h>

#include "Buzzer.h"

namespace pionieer600 {
namespace binding {
namespace buzzer {

static Buzzer buzzer;

void IsOn(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(v8::Boolean::New(isolate, buzzer.IsOn()));
}

void On(const v8::FunctionCallbackInfo<v8::Value>& args) {
  buzzer.On();
}

void Off(const v8::FunctionCallbackInfo<v8::Value>& args) {
  buzzer.Off();
}

void BuzzerInstance(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Object> buzzerObj = v8::Object::New(isolate); // create buzzer obj

  NODE_SET_METHOD(buzzerObj, "isOn", IsOn);
  NODE_SET_METHOD(buzzerObj, "on", On);
  NODE_SET_METHOD(buzzerObj, "off", Off);

  args.GetReturnValue().Set(buzzerObj);
}

static void BuzzerClean(void *arg) {
  buzzer.Off();
}

static void BuzzerInit(v8::Local<v8::Object> exports) {
  buzzer.Setup(); // setup buzzer obj

  NODE_SET_METHOD(exports, "buzzer", BuzzerInstance);

  node::AtExit(BuzzerClean);
}

} // namepsace buzzer
} // namepsace binding
} // namespace pionieer600
