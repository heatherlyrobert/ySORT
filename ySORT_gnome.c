/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"

/*
 * "simple algoritms are best when n is small and n is almost always small"
 *
 * like its name-sake sort...
 *    -- its stupid simple and easy to fix
 *    -- its reliable and quick to debug
 *    -- its stable (in the sorting order sense)
 *    -- on small data sets (maybe less than 1,000) its plenty quick (see below)
 *
 * the basic gnome sort features a couple of modifications...
 *    -- it teleports back after a swap to avoid wasted movement
 *    -- since its a linked-list, it delays the actual swap until forced
 *
 * this is my signature sort because its...
 *    -- fast enough (hell, its fast enough for gyges, my spreadsheet)
 *    -- easy to memorize (even for a moron like me)
 *    -- and quick to type-in (if forced)
 *    -- most common scenario is single add/update/delete to pre-sorted data
 *    -- it is very fast on nearly sorted data (complex sorts are predictable)
 *
 */



tMOCK   *g_head       = NULL;
tMOCK   *g_tail       = NULL;
int           g_count      = 0;
uchar         g_order      = '-';








static void   *s_root       = NULL;
static int     s_pass       = 0;
static int     s_sorts      = 0;
static int     s_comps      = 0;
static int     s_swaps      = 0;
static int     s_teles      = 0;



/*====================------------------------------------====================*/
/*===----                       basic gnome sort                       ----===*/
/*====================------------------------------------====================*/
static void      o___BASIC___________________o (void) {;}

char         /*-> check sort arguments ---------------[ leaf   [gn.530.341.50]*/ /*-[02.0000.000.!]-*/ /*-[--.---.---.--]-*/
ysort_gnome__prepare    (char a_type, void *a_head, void *a_tail, void **a_comp, void **a_curr, void **a_next, void **a_tele)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(check endpoints)----------------*/
   DEBUG_YSORT   yLOG_point   ("a_head"    , a_head);
   --rce;  if (a_head == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("a_tail"    , a_tail);
   --rce;  if (a_tail == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get first)----------------------*/
   DEBUG_YSORT   yLOG_complex ("ends"      , "head %p, tail %p", a_head, a_tail);
   DEBUG_YSORT   yLOG_note    ("entering g_cursor");
   rc = g_cursor (a_type, a_head, a_tail, NULL   , a_comp, '[');
   DEBUG_YSORT   yLOG_note    ("exiting g_cursor");
   DEBUG_YSORT   yLOG_complex ("a_comp"    , "%4d, %p", rc, *a_comp);
   s_root  = *a_comp;
   DEBUG_YSORT   yLOG_complex ("s_root"    , "%4d, %p", rc,  s_root);
   rc = g_cursor (a_type, a_head, a_tail, *a_comp, a_curr, '>');
   DEBUG_YSORT   yLOG_complex ("a_curr"    , "%4d, %p", rc, *a_curr);
   *a_next = *a_curr;
   DEBUG_YSORT   yLOG_complex ("a_next"    , "%4d, %p", rc, *a_next);
   rc = g_cursor (a_type, a_head, a_tail, *a_curr, a_tele, '>');
   DEBUG_YSORT   yLOG_complex ("a_tele"    , "%4d, %p", rc, *a_tele);
   /*---(prepare)------------------------*/
   s_pass  = 0;
   s_swaps = 0;
   s_comps = 0;
   s_teles = 0;
   ++s_sorts;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysort_gnome__swap_tele  (char a_type, void **a_head, void **a_tail, char *a_swap, void **a_comp, void **a_next, void **a_curr, void **a_tele)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(check forced swap)--------------*/
   if (*a_swap == 'y') {
      DEBUG_YSORT   yLOG_note    ("SWAP then teleport");
      ++s_swaps;
      g_unlinker (a_type, a_head, a_tail, *a_curr);
      g_linker   (a_type, a_head, a_tail, *a_next, *a_curr);
      *a_swap = '-';
   } else {
      DEBUG_YSORT   yLOG_note    ("teleport only");
   }
   /*---(teleport)-----------------------*/
   *a_curr = *a_tele;
   if (*a_curr != NULL) {
      rc = g_cursor (a_type, *a_head, *a_tail, *a_curr, a_comp, '<');
      rc = g_cursor (a_type, *a_head, *a_tail, *a_curr, a_tele, '>');
   }
   ++s_teles;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysort_gnome__back_up    (char a_type, void **a_head, void **a_tail, char *a_swap, void **a_comp, void **a_next)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(step back one)------------------*/
   *a_swap = 'y';
   *a_next = *a_comp;
   DEBUG_YSORT   yLOG_point   ("*a_next"   , *a_next);
   rc = g_cursor (a_type, *a_head, *a_tail, *a_next, a_comp, '<');
   DEBUG_YSORT   yLOG_point   ("*a_comp"   , *a_comp);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysort_gnome_driver       (uchar a_type, uchar a_lvl, void **a_head, void **a_tail)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_curr      = NULL;          /* current position               */
   void       *x_comp      = NULL;          /* comparison item                */
   void       *x_next      = NULL;          /* item after comparison          */
   void       *x_tele      = NULL;          /* teleport point                 */
   char        x_swap      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = ysort_gnome__prepare (a_type, *a_head, *a_tail, &x_comp, &x_next, &x_curr, &x_tele);
   DEBUG_YSORT   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(do the gnome walk)--------------*/
   while (x_curr != NULL) {
      DEBUG_YSORT   yLOG_complex ("PASS"      , "#%-4d, comp %p, next %p, curr %p, tele %p", s_pass, x_comp, x_next, x_curr, x_tele);
      ++s_pass;
      /*---(hit beginning)---------------*/
      if (x_comp == NULL) {
         rc = ysort_gnome__swap_tele  (a_type, a_head, a_tail, &x_swap, &x_comp, &x_next, &x_curr, &x_tele);
      } else {
         ++s_comps;
         rc = g_checker (a_type, a_lvl, x_comp, x_curr, g_order);
         if (rc <= 0) {
            rc = ysort_gnome__swap_tele  (a_type, a_head, a_tail, &x_swap, &x_comp, &x_next, &x_curr, &x_tele);
         } else {
            rc = ysort_gnome__back_up    (a_type, a_head, a_tail, &x_swap, &x_comp, &x_next);
         }
      }
      /*---(next)------------------------*/
   }
   DEBUG_YSORT   yLOG_complex ("stats"      , "%5dp, %5dc, %5ds, %5dt", s_pass, s_comps, s_swaps, s_teles);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySORT_gnome              (uchar a_type, uchar a_order, void **a_head, void **a_tail)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ysort_defense  (YSORT_GNOME, a_order, *a_head, *a_tail);
   DEBUG_YSORT   yLOG_value   ("defense"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(run)----------------------------*/
   rc = ysort_gnome_driver (a_type, 0, a_head, a_tail);
   DEBUG_YSORT   yLOG_value   ("driver"    , rc);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ySORT_gnome_stats       (int *a_sorts, int *a_comps, int *a_swaps, int *a_teles)
{
   if (a_sorts != NULL)  *a_sorts = s_sorts;
   if (a_comps != NULL)  *a_comps = s_comps;
   if (a_swaps != NULL)  *a_swaps = s_swaps;
   if (a_teles != NULL)  *a_teles = s_teles;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNIT_TEST_______________o (void) {;}

char          unit_answer [LEN_RECD];

char*            /* [------] unit test accessor ------------------------------*/
ysort_gnome__unit       (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_LABEL];
   char        t           [LEN_RECD ];
   int         i           =    0;
   int         n           =    0;
   int         x_fore      =    0;
   int         x_back      =    0;
   tMOCK *x_curr      = NULL;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "GNOME unit       : unknown request", 100);
   /*---(basics)-------------------------*/
   if      (strcmp (a_question, "count"         ) == 0) {
      x_curr = (tMOCK *) g_head; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->next; }
      x_curr = (tMOCK *) g_tail; while (x_curr != NULL) { ++x_back; x_curr = x_curr->prev; }
      snprintf (unit_answer, LEN_FULL, "GNOME count      : %4dn, %4df, %4db", g_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "seq"           ) == 0) {
      x_curr = (tMOCK *) g_head; 
      strlcpy (t, "", LEN_RECD);
      for (i = 0; i < 20; ++i) {
         if (x_curr == NULL)  sprintf (s, "--");
         else                 sprintf (s, "%02d", x_curr->seq);
         if (i > 0)  strlcat (t, " ", LEN_RECD);
         strlcat (t, s  , LEN_RECD);
         if (x_curr!= NULL)  x_curr = x_curr->next;
      }
      snprintf (unit_answer, LEN_RECD, "GNOME seq        : %s", t);
   }
   else if (strcmp (a_question, "sort"          ) == 0) {
      x_curr  = (tMOCK *) g_head;
      while (x_curr != NULL) {
         if (n == a_num)  break;
         ++n;
         x_curr = x_curr->next;
      }
      if (x_curr == NULL)  n = -1;
      if (x_curr == NULL)  snprintf (unit_answer, LEN_RECD, "GNOME sort  (%2d) : --, ---", a_num);
      else                 snprintf (unit_answer, LEN_RECD, "GNOME sort  (%2d) : %2d, %s", a_num, x_curr->seq, x_curr->label);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


