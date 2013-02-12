# This file is used with the GYP meta build system.
{
  'variables': { 'target_arch%': 'ia32' },

  'target_defaults': {
    'default_configuration': 'Debug',
    'configurations': {
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 1, # static debug
          },
        },
      },
      'Release': {
        'defines': [ 'NDEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 0, # static release
          },
        },
      }
    },
    'msvs_settings': {
      'VCLinkerTool': {
        'GenerateDebugInformation': 'true',
      },
    },
    'include_dirs': [
      # platform and arch-specific headers
      'config/<(OS)/<(target_arch)'
    ]
  },

  'targets': [
    # libmodbus
    {
      'target_name': 'modbus',
      'product_prefix': 'lib',
      'type': 'static_library',
      'sources': [
        'src/modbus.c',
        'src/modbus-data.c',
        'src/modbus.rtu',
        'src/modbus.tcp'     
      ],
      'defines': [
        'PIC',
        'HAVE_CONFIG_H'
      ],
      'dependencies': [
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'src',
          # platform and arch-specific headers
          'config/<(OS)/<(target_arch)'
        ],
      },
    },
  ]
}
