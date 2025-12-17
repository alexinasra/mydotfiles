static const char norm_fg[] = "#cabdc3";
static const char norm_bg[] = "#0b0604";
static const char norm_border[] = "#8d8488";

static const char sel_fg[] = "#cabdc3";
static const char sel_bg[] = "#8B4E35";
static const char sel_border[] = "#cabdc3";

static const char urg_fg[] = "#cabdc3";
static const char urg_bg[] = "#6B5561";
static const char urg_border[] = "#6B5561";

static const char *colors[][3]      = {
    /*               fg           bg         border                         */
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
    [SchemeUrg] =  { urg_fg,      urg_bg,    urg_border },
};
