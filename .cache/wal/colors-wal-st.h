const char *colorname[] = {

  /* 8 normal colors */
  [0] = "#0b0604", /* black   */
  [1] = "#6B5561", /* red     */
  [2] = "#8B4E35", /* green   */
  [3] = "#8C605C", /* yellow  */
  [4] = "#8C6B72", /* blue    */
  [5] = "#827183", /* magenta */
  [6] = "#927887", /* cyan    */
  [7] = "#cabdc3", /* white   */

  /* 8 bright colors */
  [8]  = "#8d8488",  /* black   */
  [9]  = "#6B5561",  /* red     */
  [10] = "#8B4E35", /* green   */
  [11] = "#8C605C", /* yellow  */
  [12] = "#8C6B72", /* blue    */
  [13] = "#827183", /* magenta */
  [14] = "#927887", /* cyan    */
  [15] = "#cabdc3", /* white   */

  /* special colors */
  [256] = "#0b0604", /* background */
  [257] = "#cabdc3", /* foreground */
  [258] = "#cabdc3",     /* cursor */
};

/* Default colors (colorname index)
 * foreground, background, cursor */
 unsigned int defaultbg = 0;
 unsigned int defaultfg = 257;
 unsigned int defaultcs = 258;
 unsigned int defaultrcs= 258;
