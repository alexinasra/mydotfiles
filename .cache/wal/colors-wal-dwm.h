static const char norm_fg[] = "#9abdb8";
static const char norm_bg[] = "#101116";
static const char norm_border[] = "#6b8480";

static const char sel_fg[] = "#9abdb8";
static const char sel_bg[] = "#DC392F";
static const char sel_border[] = "#9abdb8";

static const char urg_fg[] = "#9abdb8";
static const char urg_bg[] = "#A03D3A";
static const char urg_border[] = "#A03D3A";

static const char *colors[][3]      = {
    /*               fg           bg         border                         */
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
    [SchemeUrg] =  { urg_fg,      urg_bg,    urg_border },
};
