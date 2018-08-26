#include <node.h>
#include <v8.h>

#include "Joystick.h"

namespace pionieer600 {
namespace binding {

static Joystick joystick;
static v8::Persistent<v8::Function> onClick;
static v8::Persistent<v8::Function> onHold;

void Watch(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  if (args.Length() > 0 && args[0]->IsFunction()) {
    v8::Local<v8::Function> clickCb = v8::Local<v8::Function>::Cast(args[0]);
    onClick.Reset(isolate, clickCb);
  }

  if (args.Length() > 1 && args[1]->IsFunction()) {
    v8::Local<v8::Function> holdCb = v8::Local<v8::Function>::Cast(args[1]);
    onHold.Reset(isolate, holdCb);
  }

  auto clickHandler = []() {
    if (onClick.IsEmpty()) {
      return;
    }

    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate); // has to be done to create js value
    v8::Local<v8::Function> clickCb = v8::Local<v8::Function>::New(isolate, onClick);
    clickCb->Call(isolate->GetCurrentContext()->Global(), 0, nullptr);
  };

  auto holdHandler = []() {
    if (onHold.IsEmpty()) {
      return;
    }

    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate); // has to be done to create js value
    v8::Local<v8::Function> holdCb = v8::Local<v8::Function>::New(isolate, onHold);
    holdCb->Call(isolate->GetCurrentContext()->Global(), 0, nullptr);
  };

  joystick.Watch(clickHandler, holdHandler);
}

void StopWatch(const v8::FunctionCallbackInfo<v8::Value>& args) {
  joystick.StopWatch();
}

void JoystickInstance(const v8::FunctionCallbackInfo<v8::Value>& args) {
  joystick.Setup(); // setup joystick

  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Object> joystickObj = v8::Object::New(isolate); // create joystick obj

  NODE_SET_METHOD(joystickObj, "watch", Watch);
    NODE_SET_METHOD(joystickObj, "stopWatch", StopWatch);

  args.GetReturnValue().Set(joystickObj);
}

void JoysticInit(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "joystick", JoystickInstance);
}

} // namepsace binding
} // namespace pionieer600