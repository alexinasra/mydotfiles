# Begin ~/.bashrc
# Written for Beyond Linux From Scratch
# by James Robertson <jameswrobertson@earthlink.net>

# Personal aliases and functions.

# Personal environment variables and startup programs should go in
# ~/.bash_profile.  System wide environment variables and startup
# programs are in /etc/profile.  System wide aliases and functions are
# in /etc/bashrc.

if [ -f "/etc/bashrc" ] ; then
  source /etc/bashrc
fi

# Set up user specific i18n variables
#export LANG=en_IL.UTF-8

# Import colorscheme from 'wal' asynchronously
# &   # Run the process in the background.
# ( ) # Hide shell job control messages.
# Not supported in the "fish" shell.
(cat ~/.cache/wal/sequences &)

# Alternative (blocks terminal for 0-3ms)
#cat ~/.cache/wal/sequences

# To add support for TTYs this line can be optionally added.
source ~/.cache/wal/colors-tty.sh

wal-tile() {
    wal -c
    wal -n -i "$@" && wait 1
    hyprctl hyprpaper preload "$(< "$HOME/.cache/wal/wal")"
    hyprctl hyprpaper wallpaper ,"$(< "$HOME/.cache/wal/wal")"
    pkill hyprlauncher
    killall -SIGUSR2 waybar
    pywalfox update
    pushd $HOME/projects/walcustom
        cp -v $HOME/.cache/wal/colors.scss src/_colors.scss
        rm build -rf
        meson setup -Ddatadir=".themes" --prefix=$HOME build
        ninja -C build install
    popd
}

# End ~/.bashrc
