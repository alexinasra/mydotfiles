const char *colorname[] = {

  /* 8 normal colors */
  [0] = "#000101", /* black   */
  [1] = "#00637C", /* red     */
  [2] = "#00718E", /* green   */
  [3] = "#017FA0", /* yellow  */
  [4] = "#01809F", /* blue    */
  [5] = "#018CB0", /* magenta */
  [6] = "#009CC4", /* cyan    */
  [7] = "#77cbe0", /* white   */

  /* 8 bright colors */
  [8]  = "#538e9c",  /* black   */
  [9]  = "#00637C",  /* red     */
  [10] = "#00718E", /* green   */
  [11] = "#017FA0", /* yellow  */
  [12] = "#01809F", /* blue    */
  [13] = "#018CB0", /* magenta */
  [14] = "#009CC4", /* cyan    */
  [15] = "#77cbe0", /* white   */

  /* special colors */
  [256] = "#000101", /* background */
  [257] = "#77cbe0", /* foreground */
  [258] = "#77cbe0",     /* cursor */
};

/* Default colors (colorname index)
 * foreground, background, cursor */
 unsigned int defaultbg = 0;
 unsigned int defaultfg = 257;
 unsigned int defaultcs = 258;
 unsigned int defaultrcs= 258;
