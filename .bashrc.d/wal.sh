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
    wal -n -i "$@"
    cp -v $HOME/{.cache/wal/,.config/hypr/}hyprpaper.conf
    cp -v $HOME/{.cache/wal/,.config/hypr/}hyprtoolkit.conf
    cp -v $HOME/{.cache/wal/,.config/hypr/}colors.lua
    cp -v $HOME/{.cache/wal/,.config/waybar/}colors-waybar.css
    cp -v $HOME/{.cache/wal/,.vim/}colors-wal.vim
    pkill hyprpaper
    hyprpaper & disown 
    pkill hyprlauncher
    killall -SIGUSR2 waybar
    pywalfox update
    pushd $HOME/projects/gtktheme
        npm install
        ./scripts/generate-color-theme.sh
        meson setup -Ddatadir=".themes" --prefix="$HOME" build --wipe
        ninja -C build install
    popd
    WALL=$(< "$HOME/.cache/wal/wal")
 }
