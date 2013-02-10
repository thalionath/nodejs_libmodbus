{
  'targets': [
    {
      'target_name': 'modbus_binding',
      'include_dirs': [
        './libmodbus/src',
      ],
      'direct_dependent_settings': {
        'linkflags': [
          './libmodbus/src/.libs/modbus.o',
          './libmodbus/src/.libs/modbus-data.o',
          './libmodbus/src/.libs/modbus-rtu.o',
          './libmodbus/src/.libs/modbus-tcp.o',
          './libmodbus/src/.libs/modbus-tcp.o'
        ],
      },
      'sources': [
        'src/main.cpp',
      ],
      'conditions': [
        ['OS=="linux"', {
          'cflags!': [
            '-Wall',
            '-I./libmodbus/src',
          ],
        }],
      ],
    }
  ]
}
