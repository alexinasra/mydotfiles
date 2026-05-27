-----------------
---- WINDOWS ----
-----------------

-- See https://wiki.hypr.land/Configuring/Basics/Window-Rules/

-- Example window rules that are useful

local suppressMaximizeRule = hl.window_rule({
    -- Ignore maximize requests from all apps. You'll probably like this.
    name  = "suppress-maximize-events",
    match = { class = ".*" },

    suppress_event = "maximize",
})
-- suppressMaximizeRule:set_enabled(false)

hl.window_rule({
    -- Fix some dragging issues with XWayland
    name  = "fix-xwayland-drags",
    match = {
        class      = "^$",
        title      = "^$",
        xwayland   = true,
        float      = true,
        fullscreen = false,
        pin        = false,
    },

    no_focus = true,
})

-- Layer rules also return a handle.
-- local overlayLayerRule = hl.layer_rule({
--     name  = "no-anim-overlay",
--     match = { namespace = "^my-overlay$" },
--     no_anim = true,
-- })
-- overlayLayerRule:set_enabled(false)

-- Hyprland-run windowrule
hl.window_rule({
    name  = "move-hyprland-run",
    match = { class = "hyprland-run" },

    move  = "20 monitor_h-120",
    float = true,
})

hl.window_rule({
  match = {
    class = "appscratcheditor"
  },
  float = true,
  size = { 600,450 },
  move = {"monitor_w - 605",  "monitor_h/2-225"}
})

hl.window_rule({
  match = {
    class = "blender",
    title = "Save.*|Open.*|Blender File View|Preferences"
  },
  float = true,
  size = {"(monitor_w*0.7)", "(monitor_h*0.7)"},
  min_size = {"(monitor_w*0.5)", "(monitor_h*0.5)"},
  persistent_size = true,
  stay_focused = true 
})

hl.window_rule({
  match = { title = "Qalculate!" },
  float = true,
})
