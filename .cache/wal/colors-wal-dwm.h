static const char norm_fg[] = "#dec89d";
static const char norm_bg[] = "#0d1924";
static const char norm_border[] = "#9b8c6d";

static const char sel_fg[] = "#dec89d";
static const char sel_bg[] = "#6D726D";
static const char sel_border[] = "#dec89d";

static const char urg_fg[] = "#dec89d";
static const char urg_bg[] = "#53696B";
static const char urg_border[] = "#53696B";

static const char *colors[][3]      = {
    /*               fg           bg         border                         */
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
    [SchemeUrg] =  { urg_fg,      urg_bg,    urg_border },
};
