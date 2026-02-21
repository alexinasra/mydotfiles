# Begin ~/.profile
# Personal environment variables and startup programs.

if [ -d "$HOME/.bin" ] ; then
  pathprepend $HOME/.bin
fi
if [ -d "$HOME/.cargo/bin" ] ; then
    pathprepend $HOME/.cargo/bin
fi
# Set up user specific i18n variables
#export LANG=<ll>_<CC>.<charmap><@modifiers>
export MAKEFLAGS="-j$(nproc)"
# End ~/.profile
