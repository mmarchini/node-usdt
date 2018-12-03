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
  providerDestroy(provider);
}

Object USDTProvider::Init(Napi::Env env, Object exports) {
  HandleScope scope(env);

  Function func = DefineClass(
      env, "USDTProvider",
      {
          InstanceMethod("addProbe", &USDTProvider::addProbe),
          InstanceMethod("enable", &USDTProvider::enable),
          InstanceMethod("disable", &USDTProvider::disable),
      });

  constructor = Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("USDTProvider", func);
  return exports;
}

Value USDTProvider::AddProbe(const CallbackInfo& args) {
  std::string probe_name(args[0].As<String>());
  USDTProbe probe = USDTProbe::constructor.New({});

  this->Set(probe_name, probe);

  // TODO(mmarchini) validate args len <= MAX_ARGUMENTS
  probe->argc = std::min(MAX_ARGUMENTS, args.Length());
  for (int i = 0; i < probe->argc; i++) {
    std::string type = args[i + 1].As<String>();

    switch {
      case "char *":
        probe->arguments[i] = uint64;
        break;
      case "int":
        probe->arguments[i] = int32;
        break;
      default:
        probe->arguments[i] = uint64;
        break;
    }
  }

  switch (probe->argc) {
    case 6:
      probe->probe = providerAddProbe(provider_, probe_name, probe->argc,
        probe->arguments[0],
        probe->arguments[1],
        probe->arguments[2],
        probe->arguments[3],
        probe->arguments[4],
        probe->arguments[5]
      );
      break;
    case 5:
      probe->probe = providerAddProbe(provider_, probe_name, probe->argc,
        probe->arguments[0],
        probe->arguments[1],
        probe->arguments[2],
        probe->arguments[3],
        probe->arguments[4]
      );
      break;
    case 4:
      probe->probe = providerAddProbe(provider_, probe_name, probe->argc,
        probe->arguments[0],
        probe->arguments[1],
        probe->arguments[2],
        probe->arguments[3]
      );
      break;
    case 3:
      probe->probe = providerAddProbe(provider_, probe_name, probe->argc,
        probe->arguments[0],
        probe->arguments[1],
        probe->arguments[2]
      );
      break;
    case 2:
      probe->probe = providerAddProbe(provider_, probe_name, probe->argc,
        probe->arguments[0],
        probe->arguments[1]
      );
      break;
    case 1:
      probe->probe = providerAddProbe(provider_, probe_name, probe->argc,
        probe->arguments[0]
      );
      break;
    case 0:
    default:
      probe->probe = providerAddProbe(provider->provider, *name, probe->argc);
      break;
  }

  return probe;
}

Value USDTProvider::Enable(const CallbackInfo& args) {
  if (providerLoad(provider_) != 0) {
    // TODO (mmarchini) get error string from libstapsdt
    // TODO (mmarchini) throw error
    // ThrowError("Unable to load provider");
    return;
  }

  return;
}

Value USDTProvider::Enable(const CallbackInfo& args) {
  if (providerUnload(provider_) != 0) {
    // TODO (mmarchini) throw error
    // ThrowError("Unable to unload provider");
    return;
  }

  return;
}
}
