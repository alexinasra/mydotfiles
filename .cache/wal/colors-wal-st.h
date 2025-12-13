const char *colorname[] = {

  /* 8 normal colors */
  [0] = "#101116", /* black   */
  [1] = "#A03D3A", /* red     */
  [2] = "#DC392F", /* green   */
  [3] = "#E23C31", /* yellow  */
  [4] = "#E14235", /* blue    */
  [5] = "#3E827C", /* magenta */
  [6] = "#4D877A", /* cyan    */
  [7] = "#9abdb8", /* white   */

  /* 8 bright colors */
  [8]  = "#6b8480",  /* black   */
  [9]  = "#A03D3A",  /* red     */
  [10] = "#DC392F", /* green   */
  [11] = "#E23C31", /* yellow  */
  [12] = "#E14235", /* blue    */
  [13] = "#3E827C", /* magenta */
  [14] = "#4D877A", /* cyan    */
  [15] = "#9abdb8", /* white   */

  /* special colors */
  [256] = "#101116", /* background */
  [257] = "#9abdb8", /* foreground */
  [258] = "#9abdb8",     /* cursor */
};

/* Default colors (colorname index)
 * foreground, background, cursor */
 unsigned int defaultbg = 0;
 unsigned int defaultfg = 257;
 unsigned int defaultcs = 258;
 unsigned int defaultrcs= 258;
