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

if [ -d "$HOME/.bashrc.d" ] ; then
    for file in $HOME/.bashrc.d/*.sh ; do
        source "$file"
    done
fi
# End ~/.bashrc
