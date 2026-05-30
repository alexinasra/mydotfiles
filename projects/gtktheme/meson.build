project(
  'gtktheme',
  version: '0.1.0',
  license: 'GPL-3.0-or-later',
  meson_version: '>= 0.56.0',
  default_options: ['prefix=/usr'],
)

theme_name = get_option('theme_name')
theme_dir = join_paths(get_option('datadir'), theme_name)

sass = find_program('sass', required: false)
sass_opts = ['--no-source-map', '--style=expanded']

if not sass.found()
  message('sass not found in PATH, trying local npm dependency')
  npm = find_program('npm', required: false)
  if npm.found()
    run_command(npm, 'install', check: false)
    sass = find_program('./node_modules/.bin/sass', required: false)
  endif
endif

if not sass.found()
  error('Dart Sass is required. Install sass or run npm install.')
endif

subdir('src')
