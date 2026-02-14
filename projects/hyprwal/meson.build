project(
  'hyprwal',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++23'],
)

dependencies = [
  dependency('hyprland'),
  dependency('hyprtoolkit'),
  dependency('hyprlang'),
  dependency('hyprutils'),
  dependency('hyprcursor'),
  dependency('hyprgraphics'),
  #  dependency('hyprland-protocols'),
  dependency('hyprwayland-scanner'),
  dependency('pixman-1'),
  dependency('libdrm'),
  dependency('jsoncpp'),
  dependency('libpng'),
  dependency('libjpeg'),
  dependency('libjxl'),
  dependency('Magick++'),
  dependency('threads'),
]

exe = executable(
  'hyprwal',
  'src/main.cpp',
  'src/app.cpp',
  'src/fs.cpp',
  install : true,
  dependencies : dependencies,
)

test('basic', exe)
