{
  'targets': [
    {
      'target_name': 'wiringPiNode',
      'defines': [
        # 'FAKE_WP'  # mock wiringPi
      ],
      'sources': [
        './src/binding.cc',
        './src/wiringPi/WiringPi.cpp',
        './src/pionieer600/GpioLed.cpp',
        './src/pionieer600/Joystick.cpp'
      ],
      'include_dirs': [
        # wiringPi headers
        './include'
      ],
      'libraries': [
        # wiringPi static lib
        '<!(pwd)/lib/libwiringPi.a'
      ],
      'cflags': [
        '-Wall'
      ]
    }
  ]
}
