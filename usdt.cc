#include "usdt.h"
#include "napi.h"

namespace usdt {
Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  Napi::Object new_exports = USDTProbe::Init(env, exports);
  return USDTProvider::Init(env, new_exports);
}

NODE_API_MODULE(usdt, InitAll)
}  // namespace usdt
