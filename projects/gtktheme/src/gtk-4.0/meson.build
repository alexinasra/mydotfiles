gtk4_scss = files('gtk.scss')
gtk4_dependencies = files([
  '../_color-palette.scss',
  '../_color_theme.scss',
  '../_theme.scss',
  'sass/_common.scss',
])

gtk4_css = custom_target(
  'gtk4-css',
  input: gtk4_scss,
  output: 'gtk.css',
  command: [sass, sass_opts, '@INPUT@', '@OUTPUT@'],
  depend_files: gtk4_dependencies,
  build_by_default: true,
  install: true,
  install_dir: join_paths(theme_dir, 'gtk-4.0'),
)
