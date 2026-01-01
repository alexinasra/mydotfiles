const char *colorname[] = {

  /* 8 normal colors */
  [0] = "#0d1924", /* black   */
  [1] = "#53696B", /* red     */
  [2] = "#6D726D", /* green   */
  [3] = "#9B773D", /* yellow  */
  [4] = "#90765F", /* blue    */
  [5] = "#758770", /* magenta */
  [6] = "#9A8D63", /* cyan    */
  [7] = "#dec89d", /* white   */

  /* 8 bright colors */
  [8]  = "#9b8c6d",  /* black   */
  [9]  = "#53696B",  /* red     */
  [10] = "#6D726D", /* green   */
  [11] = "#9B773D", /* yellow  */
  [12] = "#90765F", /* blue    */
  [13] = "#758770", /* magenta */
  [14] = "#9A8D63", /* cyan    */
  [15] = "#dec89d", /* white   */

  /* special colors */
  [256] = "#0d1924", /* background */
  [257] = "#dec89d", /* foreground */
  [258] = "#dec89d",     /* cursor */
};

/* Default colors (colorname index)
 * foreground, background, cursor */
 unsigned int defaultbg = 0;
 unsigned int defaultfg = 257;
 unsigned int defaultcs = 258;
 unsigned int defaultrcs= 258;
