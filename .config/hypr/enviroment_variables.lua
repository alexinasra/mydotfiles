-------------------------------
---- ENVIRONMENT VARIABLES ----
-------------------------------

-- See https://wiki.hypr.land/Configuring/Advanced-and-Cool/Environment-variables/
--- Nvidia Driver ---
hl.env("GBM_BACKEND", "nvidia-drm")
hl.env("__GLX_VENDOR_LIBRARY_NAME", "nvidia")



--- Cursor ---
hl.env("XCURSOR_SIZE", CURSOR_SIZE)
hl.env("XCURSOR_THEME", CURSOR_THEME)
hl.env("HYPRCURSOR_SIZE", CURSOR_SIZE)
hl.env("HYPRCURSOR_THEME", CURSOR_THEME)

--- QT ---
hl.env("QT_AUTO_SCREEN_SCALE_FACTOR", "1")
hl.env("QT_QPA_PLATFORMTHEME", QT_QPA_PLATFORMTHEME)


