/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"



static int     s_max        = 0;
static int     s_buckets    = 0;
static int     s_sorts      = 0;
static int     s_comps      = 0;
static int     s_swaps      = 0;
static int     s_teles      = 0;


char         /*==> divide list into buckets ==============[ leaf   [ ------ ]=*/
TROLL__scatter     (uchar a_type, uchar a_lvl, void **a_head, void **a_tail, int *a_count, tSLOT a_slots [SEVENBIT])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   uchar       i           = 0;             /* generic iterator               */
   uchar       x_off       = 0;             /* offset to slot                 */
   void       *x_curr      = NULL;          /* current pointer                */
   void       *x_next      = NULL;          /* next pointer                   */
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   for (i = 0; i < SEVENBIT; ++i) {
      a_slots [i].head  = NULL;
      a_slots [i].tail  = NULL;
      a_slots [i].count = 0;
   }
   DEBUG_SORT   yLOG_complex ("bef point" , "head %p, tail %p", *a_head, *a_tail);
   /*---(walk through the list)----------*/
   rc = g_cursor (a_type, *a_head, *a_tail, NULL  , &x_curr, '[');
   while (x_curr != NULL) {
      /*---(save next)-------------------*/
      rc    = g_cursor (a_type, *a_head, *a_tail, x_curr, &x_next, '>');
      /*---(remove from source)----------*/
      rc = g_unlinker (a_type, a_head, a_tail, x_curr);
      --(*a_count);
      /*---(add to slot)-----------------*/
      x_off = g_slotter  (a_lvl, x_curr, g_order);
      DEBUG_SORT   yLOG_value   ("x_off"     , x_off);
      rc = g_linker   (a_type, &(a_slots [x_off].head), &(a_slots [x_off].tail), NULL, x_curr);
      ++a_slots [x_off].count;
      /*---(prepare for next)------------*/
      x_curr = x_next;
      /*---(done)------------------------*/
   }
   DEBUG_SORT   yLOG_complex ("aft point" , "head %p, tail %p", *a_head, *a_tail);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*==> bring slots back into a single list ===[ leaf   [ ------ ]=*/
TROLL__gather      (void **a_head, void **a_tail, int *a_count, tSLOT a_slots [SEVENBIT])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   uchar       i           = 0;             /* generic iterator               */
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   DEBUG_SORT   yLOG_complex ("bef point" , "head %p, tail %p", *a_head, *a_tail);
   /*---(consolidate)--------------------*/
   for (i = 0; i < SEVENBIT; ++i) {
      if (a_slots [i].count <= 0)  continue;
      rc = g_joiner (a_head, a_tail, a_count, &(a_slots [i].head), &(a_slots [i].tail), &(a_slots [i].count));
   }
   DEBUG_SORT   yLOG_complex ("aft point" , "head %p, tail %p", *a_head, *a_tail);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}


char         /*==> bucket sort ===========================[ ------ [ ------ ]=*/
TROLL__driver      (uchar a_type, uchar a_lvl, void **a_head, void **a_tail, int *a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i           =    0;
   tSLOT       x_slots     [SEVENBIT];
   int         x_comps, x_swaps, x_teles;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   DEBUG_SORT   yLOG_complex ("args"      , "%2d, %p, %p", a_lvl, *a_head, *a_tail);
   /*---(scatter into slots)-------------*/
   rc = TROLL__scatter     (a_type, a_lvl, a_head, a_tail, a_count, x_slots);
   /*---(review results)-----------------*/
   for (i = 1; i < SEVENBIT; ++i) {
      if (x_slots[i].count <=  1) continue;
      if (x_slots[i].count <= 10) {
         rc = GNOME_driver  ('-', a_lvl, &(x_slots[i].head), &(x_slots[i].tail));
         GNOME_stats (&x_comps, &x_swaps, &x_teles);
         s_comps += x_comps;
         s_swaps += x_swaps;
         s_teles += x_teles;
         ++s_sorts;
      } else {
         rc = TROLL__driver (a_type, a_lvl + 1, &(x_slots[i].head), &(x_slots[i].tail), &(x_slots[i].count));
      }
   }
   /*---(gather back from slots)---------*/
   rc = TROLL__gather      (a_head, a_tail, a_count, x_slots);
   if (a_lvl > s_max)  s_max = a_lvl;
   ++s_buckets;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySORT_troll             (uchar a_type, uchar a_order, void **a_head, void **a_tail)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_count     =    0;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   s_buckets  = 0;
   s_max      = 0;
   s_sorts    = 0;
   s_comps    = 0;
   s_swaps    = 0;
   s_teles    = 0;
   /*---(defense)------------------------*/
   rc = ySORT_defense   (YSORT_TROLL, a_order, *a_head, *a_tail);
   DEBUG_SORT   yLOG_value   ("defense"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(run driver)---------------------*/
   rc = TROLL__driver (a_type, 0, a_head, a_tail, &x_count);
   DEBUG_SORT   yLOG_value   ("driver"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(output)-------------------------*/
   DEBUG_SORT   yLOG_complex ("stats"      , "%5dm, %5db, %5ds, %5dc, %5ds, %5dt", s_max, s_buckets, s_sorts, s_comps, s_swaps, s_teles);
   DEBUG_SORT   MOCK__printer (*a_head);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNIT_TEST_______________o (void) {;}

char          unit_answer [LEN_RECD];

char*            /* [------] unit test accessor ------------------------------*/
SCATTER__unit      (char *a_question, tSLOT a_slots [SEVENBIT], char a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_LABEL];
   char        t           [LEN_RECD ];
   int         i           =    0;
   int         n           =    0;
   int         c           =    0;
   int         x_fore      =    0;
   int         x_back      =    0;
   tSORT_DATA *x_curr      = NULL;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "SCATTER unit     : unknown request", 100);
   if      (strcmp (a_question, "buckets"       ) == 0) {
      strlcpy (t, "", LEN_RECD);
      for (i = 'a'; i <= 'z'; ++i) {
         if (a_slots [i].count == 0) {
            sprintf (s, " -");
         } else {
            sprintf (s, " %1d", a_slots [i].count);
            ++n;
            c += a_slots [i].count;
         }
         strlcat (t, s  , LEN_RECD);
         if (x_curr!= NULL)  x_curr = x_curr->next;
      }
      snprintf (unit_answer, LEN_RECD, "SCATTER buckets  : %2d %2d %s", c, n, t);
   }
   else if (strcmp (a_question, "slot"          ) == 0) {
      strlcpy (t, "", LEN_RECD);
      x_curr = a_slots [a_num].head;
      while (x_curr != NULL) {
         sprintf (s, " [%-8.8s]", x_curr->label);
         strlcat (t, s  , LEN_RECD);
         ++c;
         x_curr = x_curr->next;
      }
      if (strcmp (t, "") == 0)  strlcpy (t, " -", LEN_RECD);
      snprintf (unit_answer, LEN_RECD, "SCATTER slot (%c) : %2d %s", a_num, c, t);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char*            /* [------] unit test accessor ------------------------------*/
TROLL__unit        (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_LABEL];
   char        t           [LEN_RECD ];
   int         i           =    0;
   int         n           =    0;
   int         x_fore      =    0;
   int         x_back      =    0;
   tSORT_DATA *x_curr      = NULL;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "BUCKET unit      : unknown request", 100);
   /*---(basics)-------------------------*/
   /*---(complete)-----------------------*/
   return unit_answer;
}



