cinnamon_dir = join_paths(theme_dir, 'cinnamon')

install_subdir(
  'assets',
  install_dir: cinnamon_dir,
)

install_data(
  'thumbnail.png',
  install_dir: cinnamon_dir,
)

cinnamon_scss_conf = configuration_data()
cinnamon_scss_conf.set('dark_theme', 'false')
cinnamon_scss_conf.set('light_topbar', 'false')
cinnamon_scss_conf.set('compact', 'false')
cinnamon_scss_conf.set('current_source_dir', meson.current_source_dir())

cinnamon_scss = configure_file(
  input: 'cinnamon.scss.in',
  output: 'cinnamon.scss',
  configuration: cinnamon_scss_conf,
)

cinnamon_css = custom_target(
  'cinnamon-css',
  input: cinnamon_scss,
  output: 'cinnamon.css',
  command: [sass, sass_opts, '@INPUT@', '@OUTPUT@'],
  depend_files: files([
    '../_color-palette.scss',
    '../_color_theme.scss',
    '../_shadow.scss',
    '../_theme.scss',
    '../_theme-color.scss',
    '../gnome-shell/sass/_drawing.scss',
    '../gnome-shell/sass/_typography.scss',
    'sass/_common.scss',
    'sass/_extends.scss',
  ]),
  build_by_default: true,
  install: true,
  install_dir: cinnamon_dir,
)
