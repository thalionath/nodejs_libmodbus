{
  'targets': [
    {
      'target_name': 'modbus_binding',
      'sources': [
        'src/main.cpp',
      ],
      'cflags': [
        '-Wall'
      ],
      'cflags_cc': [
        '-Wall'
      ],
      'dependencies': [
        'deps/libmodbus/libmodbus.gyp:modbus'
      ]
    }
  ]
}
