{
  "targets": [
    {
      "target_name": "usdt",
      "sources": [
        "usdt-provider.cc",
        "usdt-probe.cc",
        "usdt.cc",
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "libraries": [
        "-lstapsdt"
      ]
    }
  ]
}
