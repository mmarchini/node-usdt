#include "usdt.h"

namespace usdt {
FunctionReference USDTProbe::constructor;


USDTProbe::USDTProbe(const CallbackInfo& args)
    : ObjectWrap<USDTProvider>(args),
      argc_(0),
      probe_(nullptr);

USDTProbe::~USDTProbe() {
  probe_ = nullptr;
}

Object USDTProbe::Init(Napi::Env env, Object exports) {
  HandleScope scope(env);

  Function func = DefineClass(
      env, "USDTProbe",
      {
          InstanceMethod("fire", &USDTProbe::fire),
      });

  constructor = Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("USDTProbe", func);
  return exports;
}

Value USDTProvider::Fire(const CallbackInfo& args) {
  Napi::Env env = args.Env();
  if (probeIsEnabled(probe_) == 0) {
    return env.False();
  }

  if (args.Length() != argc) {
    // TODO(mmarchini) throw TypeError
    return env.False();
  }

  void *argv[MAX_ARGUMENTS];

  // convert each argument value
  for (size_t i = 0; i < argc; i++) {
    if (args[i].IsString()) {
      std::string s = args[i].As<String>();
      argv[i] = static_cast<void *>(strdup(s.c_str()));
    } else if (args[i].IsNumber()) {
      argv[i] = reinterpret_cast<void *>(args[i].As<Number>().Uint32Value());
    } else {
      // TODO(mmarchini) Throw TypeError
      return env.False();
    }
  }

  switch(argc) {
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

  for (size_t i = 0; i < argc; i++) {
    if (args[i].IsString()) {
      free(argv[i]);
    }
  }

  return env.True();
}
} // namespace usdt
