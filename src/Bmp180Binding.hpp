#include <node.h>
#include <v8.h>

#include "pionieer600/BMP180.h"

namespace binding {
namespace bmp_180 {

static pionieer600::Bmp180 bmp180;
static v8::Persistent<v8::Function> onMeasure;

void Measure(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(args[0]);
  onMeasure.Reset(isolate, cb);

  const auto handler = [](float temp, float press) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate); // has to be done to create js value
    const unsigned argc = 2;
    v8::Local<v8::Value> argv[argc] = {
      v8::Number::New(isolate, temp),
      v8::Number::New(isolate, press),
    };
    v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, onMeasure);
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  };

  bmp180.Measure(handler);
}

void Bmp180Instance(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Object> bmp180Obj = v8::Object::New(isolate); // create bmp180 obj

  NODE_SET_METHOD(bmp180Obj, "measure", Measure);

  args.GetReturnValue().Set(bmp180Obj);
}

static void Bmp180Init(v8::Local<v8::Object> exports) {
  bmp180.SetupI2C();
  bmp180.Calibrate();

  NODE_SET_METHOD(exports, "bmp180", Bmp180Instance);
}

} // namesapce BMP180
} // namepsace binding