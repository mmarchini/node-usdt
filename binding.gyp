{
  "targets": [
    {
      "target_name": "usdt",
      "sources": [
        "usdt-provider.cc",
        "usdt-probe.cc",
      ],
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "libraries": [
        "-lstapsdt"
      ]
    }
  ]
}
