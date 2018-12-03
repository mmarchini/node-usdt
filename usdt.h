#pragma once
#include <napi.h>

extern "C" {
#include <libstapsdt.h>
}

namespace usdt {
class USDTProvider : public Napi::ObjectWrap<USDTProvider> {

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

class USDTProbe : public Napi::ObjectWrap<USDTProbe> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::FunctionReference constructor;
  USDTProbe(const Napi::CallbackInfo& args);
  ~USDTProbe();

 private:
  friend USDTProvider;
  SDTProbe_t *probe_;
  ArgType_t arguments_[MAX_ARGUMENTS];
  size_t argc_;

  Napi::Value Fire(const Napi::CallbackInfo& args);
};
};
