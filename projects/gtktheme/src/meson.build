index_theme_conf = configuration_data()
index_theme_conf.set('theme_name', theme_name)

index_theme = configure_file(
  input: 'index.theme.in',
  output: 'index.theme',
  configuration: index_theme_conf,
)

install_data(
  index_theme,
  install_dir: theme_dir,
)

subdir('gtk-3.0')
subdir('gtk-4.0')
