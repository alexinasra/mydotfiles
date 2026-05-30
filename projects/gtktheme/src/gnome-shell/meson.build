gnome_shell = find_program('gnome-shell', required: false)

if get_option('gnome_shell_version') != ''
  gnome_shell_full_version = get_option('gnome_shell_version')
elif gnome_shell.found()
  gnome_shell_full_version = run_command(gnome_shell, '--version', check: false).stdout().split()[-1]
else
  message('gnome-shell not found, using styles for last GNOME Shell version available')
  gnome_shell_full_version = '3.38'
endif

gnome_shell_version_array = gnome_shell_full_version.split('.')
if gnome_shell_version_array[0].version_compare('>= 40')
  gnome_shell_version = gnome_shell_version_array[0]
elif gnome_shell_version_array[1].to_int().is_even()
  gnome_shell_version = gnome_shell_version_array[0] + '.' + gnome_shell_version_array[1]
else
  gnome_shell_version = gnome_shell_version_array[0] + '.' + (gnome_shell_version_array[1].to_int() + 1).to_string()
endif
message('Set gnome-shell version to ' + gnome_shell_version)

gnome_shell_dir = join_paths(theme_dir, 'gnome-shell')

install_subdir(
  'assets',
  install_dir: join_paths(gnome_shell_dir, 'assets'),
  strip_directory: true,
)

install_subdir(
  'extensions',
  install_dir: gnome_shell_dir,
)

install_subdir(
  'icons',
  install_dir: gnome_shell_dir,
)

install_data(
  [
    'gnome-shell-start.svg',
    'gnome-shell-theme.gresource.xml',
    'noise-texture.png',
    'pad-osd.css',
    'process-working.svg',
  ],
  install_dir: gnome_shell_dir,
)

gnome_shell_scss_conf = configuration_data()
gnome_shell_scss_conf.set('dark_theme', 'false')
gnome_shell_scss_conf.set('light_topbar', 'false')
gnome_shell_scss_conf.set('compact', 'false')
gnome_shell_scss_conf.set('version', gnome_shell_version)
gnome_shell_scss_conf.set('current_source_dir', meson.current_source_dir())

gnome_shell_scss = configure_file(
  input: 'gnome-shell.scss.in',
  output: 'gnome-shell.scss',
  configuration: gnome_shell_scss_conf,
)

gnome_shell_css = custom_target(
  'gnome-shell-css',
  input: gnome_shell_scss,
  output: 'gnome-shell.css',
  command: [sass, sass_opts, '@INPUT@', '@OUTPUT@'],
  depend_files: files([
    '../_color-palette.scss',
    '../_color_theme.scss',
    '../_shadow.scss',
    '../_theme.scss',
    '../_theme-color.scss',
    'sass/components/base/_base.scss',
    'sass/components/base/_button.scss',
    'sass/components/base/_check-box.scss',
    'sass/components/base/_entry.scss',
    'sass/components/base/_scrollbar.scss',
    'sass/components/base/_slider.scss',
    'sass/components/base/_switch.scss',
    'sass/components/dialog/_dialog.scss',
    'sass/components/dialog/_network-dialog.scss',
    'sass/components/gdm/_login-dialog.scss',
    'sass/components/gdm/_screen-shield.scss',
    'sass/components/menu/_calendar.scss',
    'sass/components/menu/_candidate-popup.scss',
    'sass/components/menu/_menu.scss',
    'sass/components/menu/_message-list.scss',
    'sass/components/misc/_a11y.scss',
    'sass/components/misc/_keyboard.scss',
    'sass/components/misc/_looking-glass.scss',
    'sass/components/misc/_misc.scss',
    'sass/components/misc/_tile-preview.scss',
    'sass/components/notification/_hotplug.scss',
    'sass/components/notification/_notification.scss',
    'sass/components/osd/_osd.scss',
    'sass/components/osd/_switcher.scss',
    'sass/components/osd/_workspace-switcher.scss',
    'sass/components/overview/_app-grid.scss',
    'sass/components/overview/_dash.scss',
    'sass/components/overview/_overview.scss',
    'sass/components/overview/_search-entry.scss',
    'sass/components/overview/_search-results.scss',
    'sass/components/overview/_window-picker.scss',
    'sass/components/overview/_workspace-thumbnails.scss',
    'sass/components/panel/_panel.scss',
    'sass/components/panel/_ripple-box.scss',
    'sass/components/_index.scss',
    'sass/_drawing.scss',
    'sass/_extensions.scss',
    'sass/_gnome-shell.scss',
    'sass/_typography.scss',
  ]),
  build_by_default: true,
  install: true,
  install_dir: gnome_shell_dir,
)
