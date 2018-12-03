#include <napi.h>

extern "C" {
#include <libstapsdt.h>
}

namespace usdt {
class USDTProbe : public Napi::ObjectWrap<USDTProbe> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::FunctionReference constructor;
  USDTProbe(const Napi::CallbackInfo& args);
  ~USDTProbe();

 private:
  SDTProbe_t *probe;
  ArgType_t arguments[MAX_ARGUMENTS];
  size_t argc;

  Napi::Value Fire(const Napi::CallbackInfo& args);

  // v8::Local<Value> _fire(Nan::NAN_METHOD_ARGS_TYPE, size_t);
};

class USDTProvider : public Nan::ObjectWrap<USDTProvider> {

 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::FunctionReference constructor;
  USDTProvider(const Napi::CallbackInfo& args);
  ~USDTProvider();

 private:
  SDTProvider_t *provider_;

  Napi::Value AddProbe(const Napi::CallbackInfo& args);
  Napi::Value Enable(const Napi::CallbackInfo& args);
  Napi::Value Disable(const Napi::CallbackInfo& args);
};
};
