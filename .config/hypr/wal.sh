#!/bin/bash
hyprctl hyprpaper preload "$(cat ~/.cache/wal/wal)"
hyprctl hyprpaper wallpaper ", $(cat ~/.cache/wal/wal)"
