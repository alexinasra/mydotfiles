gtk2_dir = join_paths(theme_dir, 'gtk-2.0')

install_subdir(
  'assets',
  install_dir: join_paths(gtk2_dir, 'assets'),
  strip_directory: true,
)

install_data(
  'gtkrc',
  install_dir: gtk2_dir,
)

install_data(
  [
    'apps.rc',
    'hacks.rc',
    'main.rc',
  ],
  install_dir: gtk2_dir,
)
