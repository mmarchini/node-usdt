#include <algorithm>

#include "usdt.h"

namespace usdt {

using Napi::CallbackInfo;
using Napi::Function;
using Napi::FunctionReference;
using Napi::HandleScope;
using Napi::Object;
using Napi::ObjectReference;
using Napi::Persistent;
using Napi::String;
using Napi::TypeError;
using Napi::Value;

FunctionReference USDTProvider::constructor;

USDTProvider::USDTProvider(const CallbackInfo& args)
    : ObjectWrap<USDTProvider>(args),
      provider_(nullptr) {
  std::string provider_name = args[0].As<String>();
  if ((provider_ = providerInit(provider_name.c_str())) == nullptr) {
    // TODO(mmarchini) Throw error
    // ThrowError("providerInit failed");
    return;
  }
}

USDTProvider::~USDTProvider() {
  providerDestroy(provider_);
}

Object USDTProvider::Init(Napi::Env env, Object exports) {
  HandleScope scope(env);

  Function func = DefineClass(
      env, "USDTProvider",
      {
          InstanceMethod("addProbe", &USDTProvider::AddProbe),
          InstanceMethod("enable", &USDTProvider::Enable),
          InstanceMethod("disable", &USDTProvider::Disable),
      });

  constructor = Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("USDTProvider", func);
  return exports;
}

Value USDTProvider::AddProbe(const CallbackInfo& args) {
  std::string probe_name(args[0].As<String>());
  Object probe_obj = USDTProbe::constructor.New({});
  USDTProbe* probe = ObjectWrap<USDTProbe>::Unwrap(probe_obj);

  // TODO(mmarchini) validate args len <= MAX_ARGUMENTS
  probe->argc = std::min(MAX_ARGUMENTS, args.Length());
  for (unsigned int i = 0; i < probe->argc; i++) {
    std::string type = args[i + 1].As<String>();

    if (type == "char *") {
      probe->arguments[i] = uint64;
    } else if (type == "int") {
      probe->arguments[i] = int32;
    } else {
      probe->arguments[i] = uint64;
    }
  }

  switch (probe->argc) {
    case 6:
      probe->probe = providerAddProbe(provider_, probe_name.c_str(), probe->argc,
        probe->arguments[0],
        probe->arguments[1],
        probe->arguments[2],
        probe->arguments[3],
        probe->arguments[4],
        probe->arguments[5]
      );
      break;
    case 5:
      probe->probe = providerAddProbe(provider_, probe_name.c_str(), probe->argc,
        probe->arguments[0],
        probe->arguments[1],
        probe->arguments[2],
        probe->arguments[3],
        probe->arguments[4]
      );
      break;
    case 4:
      probe->probe = providerAddProbe(provider_, probe_name.c_str(), probe->argc,
        probe->arguments[0],
        probe->arguments[1],
        probe->arguments[2],
        probe->arguments[3]
      );
      break;
    case 3:
      probe->probe = providerAddProbe(provider_, probe_name.c_str(), probe->argc,
        probe->arguments[0],
        probe->arguments[1],
        probe->arguments[2]
      );
      break;
    case 2:
      probe->probe = providerAddProbe(provider_, probe_name.c_str(), probe->argc,
        probe->arguments[0],
        probe->arguments[1]
      );
      break;
    case 1:
      probe->probe = providerAddProbe(provider_, probe_name.c_str(), probe->argc,
        probe->arguments[0]
      );
      break;
    case 0:
    default:
      probe->probe = providerAddProbe(provider_, probe_name.c_str(), probe->argc);
      break;
  }

  return probe_obj;
}

Value USDTProvider::Enable(const CallbackInfo& args) {
  Napi::Env env = args.Env();
  if (providerLoad(provider_) != 0) {
    // TODO (mmarchini) get error string from libstapsdt
    // TODO (mmarchini) throw error
    // ThrowError("Unable to load provider");
    return env.Null();
  }

  return env.Null();
}

Value USDTProvider::Disable(const CallbackInfo& args) {
  Napi::Env env = args.Env();
  if (providerUnload(provider_) != 0) {
    // TODO (mmarchini) throw error
    // ThrowError("Unable to unload provider");
    return env.Null();
  }

  return env.Null();
}
};
