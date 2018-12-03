#include "usdt.h"

namespace usdt {

using Napi::CallbackInfo;
using Napi::Function;
using Napi::FunctionReference;
using Napi::HandleScope;
using Napi::Number;
using Napi::Object;
using Napi::Persistent;
using Napi::String;
using Napi::TypeError;
using Napi::Boolean;
using Napi::Value;

FunctionReference USDTProbe::constructor;


USDTProbe::USDTProbe(const CallbackInfo& args)
    : ObjectWrap<USDTProbe>(args),
      probe_(nullptr),
      argc_(0) {};

USDTProbe::~USDTProbe() {
  probe_ = nullptr;
}

Object USDTProbe::Init(Napi::Env env, Object exports) {
  HandleScope scope(env);

  Function func = DefineClass(
      env, "USDTProbe",
      {
          InstanceMethod("fire", &USDTProbe::Fire),
      });

  constructor = Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("USDTProbe", func);
  return exports;
}

Value USDTProbe::Fire(const CallbackInfo& args) {
  Napi::Env env = args.Env();
  if (probeIsEnabled(probe_) == 0) {
    return Boolean::New(env, false);
  }

  if (args.Length() != argc_) {
    // TODO(mmarchini) throw TypeError
    return Boolean::New(env, false);
  }

  void *argv[MAX_ARGUMENTS];

  // convert each argument value
  for (size_t i = 0; i < argc_; i++) {
    if (args[i].IsString()) {
      std::string s = args[i].As<String>();
      argv[i] = static_cast<void *>(strdup(s.c_str()));
    } else if (args[i].IsNumber()) {
      argv[i] = reinterpret_cast<void *>(args[i].As<Number>().Uint32Value());
    } else {
      // TODO(mmarchini) Throw TypeError
      return Boolean::New(env, false);
    }
  }

  switch(argc_) {
    case 6:
      probeFire(probe_, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
      break;
    case 5:
      probeFire(probe_, argv[0], argv[1], argv[2], argv[3], argv[4]);
      break;
    case 4:
      probeFire(probe_, argv[0], argv[1], argv[2], argv[3]);
      break;
    case 3:
      probeFire(probe_, argv[0], argv[1], argv[2]);
      break;
    case 2:
      probeFire(probe_, argv[0], argv[1]);
      break;
    case 1:
      probeFire(probe_, argv[0]);
      break;
    case 0:
    default:
      probeFire(probe_);
      break;
  }

  for (size_t i = 0; i < argc_; i++) {
    if (args[i].IsString()) {
      free(argv[i]);
    }
  }

  return Boolean::New(env, true);
}
} // namespace usdt
