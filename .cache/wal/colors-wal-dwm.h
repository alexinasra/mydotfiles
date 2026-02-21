static const char norm_fg[] = "#77cbe0";
static const char norm_bg[] = "#000101";
static const char norm_border[] = "#538e9c";

static const char sel_fg[] = "#77cbe0";
static const char sel_bg[] = "#00718E";
static const char sel_border[] = "#77cbe0";

static const char urg_fg[] = "#77cbe0";
static const char urg_bg[] = "#00637C";
static const char urg_border[] = "#00637C";

static const char *colors[][3]      = {
    /*               fg           bg         border                         */
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
    [SchemeUrg] =  { urg_fg,      urg_bg,    urg_border },
};
