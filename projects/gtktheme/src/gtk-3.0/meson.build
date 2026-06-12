gtk3_scss = files('gtk.scss')
gtk3_dependencies = files([
  '../_color_theme.scss',
  '../_theme.scss',
  'sass/_common.scss',
  'sass/_globals.scss',
  'sass/_button.scss',
  'sass/_toggle-button.scss',
  'sass/_switch-button.scss',
  'sass/_checkbox.scss',
  'sass/_radio.scss',
  'sass/_scrollbar.scss',
])

gtk3_css = custom_target(
  'gtk3-css',
  input: gtk3_scss,
  output: 'gtk.css',
  command: [sass, sass_opts, '@INPUT@', '@OUTPUT@'],
  depend_files: gtk3_dependencies,
  build_by_default: true,
  install: true,
  install_dir: join_paths(theme_dir, 'gtk-3.0'),
)
