#include    "ySORT_solo.h"



/*===[[ GNU GENERAL PUBLIC LICENSE (GPL) ]]===================================*/
/*´´·········1·········2·········3·········4·········5·········6·········7·········8  */

#define  P_COPYRIGHT   \
   "copyright (c) 2020 robert.s.heatherly at balsashrike at gmail dot com"

#define  P_LICENSE     \
   "the only place you could have gotten this code is my github, my website,¦"   \
   "or illegal sharing. given that, you should be aware that this is GPL licensed."

#define  P_COPYLEFT    \
   "the GPL COPYLEFT REQUIREMENT means any modifications or derivative works¦"   \
   "must be released under the same GPL license, i.e, must be free and open."

#define  P_INCLUDE     \
   "the GPL DOCUMENTATION REQUIREMENT means that you must include the original¦" \
   "copyright notice and the full licence text with any resulting anything."

#define  P_AS_IS       \
   "the GPL NO WARRANTY CLAUSE means the software is provided without any¦"      \
   "warranty and the author cannot be held liable for damages."

#define  P_THEFT    \
   "if you knowingly violate the spirit of these ideas, i suspect you might¦"    \
   "find any number of freedom-minded hackers may take it quite personally ;)"

/*´´·········1·········2·········3·········4·········5·········6·········7·········8  */
/*===[[ GNU GENERAL PUBLIC LICENSE (GPL) ]]===================================*/



char
ysort__umods            (char c_phase, char *a_base, short a_wide, short a_focus, short a_tall)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         i           =    0;
   int         j           =    0;
   int         l           =    0;
   char        x_ch        =  '·';
   char       *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_char    ("c_phase"   , c_phase);
   --rce;  if (c_phase == 0 || strchr ("be", c_phase) == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_value   ("a_wide"    , a_wide);
   --rce;  if (a_wide < 5 || a_wide > LEN_HUND) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_value   ("a_focus"   , a_focus);
   --rce;  if (a_focus < 5 || a_focus > a_wide) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_value   ("a_tall"    , a_tall);
   --rce;  if (a_tall < 5 || a_tall > LEN_FULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   --rce;  for (i = 0; i < a_tall; ++i) {
      x_line = a_base + (a_wide * i);
      l = strlen (x_line);
      DEBUG_YSORT   yLOG_complex ("line"      , "%2d, %2då%sæ", i, l, x_line);
      --rce;  if (l >= a_wide) {  /* run-away strings */
         DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      for (j = 0; j < a_focus; ++j) {
         x_ch = x_line [j];
         if (c_phase == 'b' && x_ch == '·')  x_ch = ' ';
         if (c_phase == 'e' && x_ch == ' ')  x_ch = '·';
         x_line [j] = x_ch;
      }
      DEBUG_YSORT   yLOG_complex ("after"     , "%2d, %2då%sæ", i, l, x_line);
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysort_ustrings          (char *a_base, short a_wide, short a_focus, short a_tall)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char       *a           = NULL;          /* comparison entry one           */
   char       *b           = NULL;          /* comparison entry two           */
   char        t           [LEN_RECD]  = "";
   int         x_tele      =   -1;          /* teleport point to speed sort   */
   short       x_comp      =    0;
   short       x_swap      =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = ysort__umods ('b', a_base, a_wide, a_focus, a_tall);
   DEBUG_YSORT   yLOG_char    ("beg"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(sort)---------------------------*/
   x_comp = x_swap = 0;
   i = 1;
   while (i < a_tall) {
      /*---(load vars)-------------------*/
      a  = a_base + (a_wide * (i - 1));
      b  = a_base + (a_wide * i);
      DEBUG_YSORT   yLOG_complex ("current"   , "compare %3d (%3d)  å%sæ vs å%sæ", i, x_tele, a, b);
      /*---(compare)---------------------*/
      ++x_comp;
      if (i == 0 || strcmp (a, b) <= 0) {
         if (x_tele >= 0) {
            i    = x_tele;
            x_tele = -1;
         } else {
            ++i;
         }
         continue;
      }
      /*---(swap)------------------------*/
      ++x_swap;
      strlcpy (t, a, a_wide);
      strlcpy (a, b, a_wide);
      strlcpy (b, t, a_wide);
      a  = a_base + (a_wide * (i - 1));
      b  = a_base + (a_wide * i);
      DEBUG_YSORT   yLOG_complex ("swapped"   , "now     %3d (%3d)  å%sæ vs å%sæ", i, x_tele, a, b);
      /*---(update)----------------------*/
      if (x_tele < 0) x_tele = i;
      if (i > 1) --i;
   }
   DEBUG_YSORT   yLOG_complex ("stats"     , "count = %3d, comps = %5d, swaps = %5d", a_tall, x_comp, x_swap);
   /*---(clean-up)-----------------------*/
   rc = ysort__umods ('e', a_base, a_wide, a_focus, a_tall);
   DEBUG_YSORT   yLOG_char    ("end"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)------------------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



