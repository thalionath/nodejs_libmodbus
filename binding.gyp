{
  'targets': [
    {
      'target_name': 'modbus_binding',
      'include_dirs': [
        './libmodbus/src',
      ],
      'link_settings': {
        'ldflags': ['-L./libmodbus/src', '-lmodbus'],
      },
      'sources': [
        'src/main.cpp',
      ],
      'cflags': [
        '-Wall',
        '-I./libmodbus/src',
      ],
      'cflags_cc': [
        '-Wall',
        '-I./libmodbus/src',
      ],
    }
  ]
}
