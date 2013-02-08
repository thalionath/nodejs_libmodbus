{
  'targets': [
    {
      'target_name': 'libmodbus',
      'type': '<(library)',
      'dependencies': [
      ],
      'defines': [
      ],
      'include_dirs': [
        'libmodbus/src',
      ],
      'direct_dependent_settings': {
        'defines': [
        ],
        'linkflags': [
          './libmodbus/src/.libs/modbus.o',
          './libmodbus/src/.libs/modbus-data.o',
          './libmodbus/src/.libs/modbus-rtu.o',
          './libmodbus/src/.libs/modbus-tcp.o',
          './libmodbus/src/.libs/modbus-tcp.o'
        ],
      },
      'export_dependent_settings': [
      ],
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
