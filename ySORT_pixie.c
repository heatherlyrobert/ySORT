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



/*> void   *g_head       = NULL;                                                      <*/
/*> void   *g_tail       = NULL;                                                      <*/
/*> int     g_count      = 0;                                                         <*/


char    (*g_walk)      (void **a_cur, uchar a_mov);
char*   (*g_key)       (void *a_cur);
char    (*g_unhook)    (void *a_cur);
char    (*g_hook)      (void *a_ref, void *a_cur);





static void   *s_root       = NULL;
static int     s_pass       = 0;
static int     s_comps      = 0;
static int     s_swaps      = 0;
static int     s_teles      = 0;



/*====================------------------------------------====================*/
/*===----                       basic gnome sort                       ----===*/
/*====================------------------------------------====================*/
static void      o___BASIC___________________o (void) {;}

char         /*-> check sort arguments ---------------[ leaf   [gn.530.341.50]*/ /*-[02.0000.000.!]-*/ /*-[--.---.---.--]-*/
ysort_pixie__prepare    (void **a_comp, void **a_curr, void **a_next, void **a_tele)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(get first)----------------------*/
   rc = g_walk (a_comp, YDLST_DHEAD);
   DEBUG_YSORT   yLOG_complex ("a_comp"    , "%4d, %p", rc, *a_comp);
   --rce;  if (rc < 0 || *a_comp == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *a_curr = *a_comp;
   rc = g_walk (a_curr, YDLST_DNEXT);
   DEBUG_YSORT   yLOG_complex ("a_curr"    , "%4d, %p", rc, *a_curr);
   --rce;  if (rc < 0 || *a_curr == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *a_next = *a_curr;
   DEBUG_YSORT   yLOG_complex ("a_next"    , "%4d, %p", rc, *a_next);
   *a_tele = *a_curr;
   rc = g_walk (a_tele, YDLST_DNEXT);
   DEBUG_YSORT   yLOG_complex ("a_tele"    , "%4d, %p", rc, *a_tele);
   /*---(prepare)------------------------*/
   s_pass  = s_swaps = s_comps = s_teles = 0;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysort_pixie__swap_tele  (char *a_swap, void **a_comp, void **a_next, void **a_curr, void **a_tele)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(check forced swap)--------------*/
   if (*a_swap == 'y') {
      DEBUG_YSORT   yLOG_note    ("SWAP then teleport");
      ++s_swaps;
      g_unhook    (*a_curr);
      g_hook      (*a_next, *a_curr);
      *a_swap = '-';
   } else {
      DEBUG_YSORT   yLOG_note    ("teleport only");
   }
   /*---(teleport)-----------------------*/
   *a_curr = *a_tele;
   if (*a_curr != NULL) {
      *a_comp = *a_tele = *a_curr;
      rc = g_walk (a_comp, YDLST_DPREV);
      rc = g_walk (a_tele, YDLST_DNEXT);
   }
   ++s_teles;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysort_pixie__back_up    (char *a_swap, void **a_comp, void **a_next)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(step back one)------------------*/
   *a_swap = 'y';
   *a_next = *a_comp;
   DEBUG_YSORT   yLOG_point   ("*a_next"   , *a_next);
   *a_comp = *a_next;
   rc = g_walk (a_comp, YDLST_DPREV);
   DEBUG_YSORT   yLOG_point   ("*a_comp"   , *a_comp);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysort_pixie              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_curr      = NULL;          /* current position               */
   void       *x_comp      = NULL;          /* comparison item                */
   void       *x_next      = NULL;          /* item after comparison          */
   void       *x_tele      = NULL;          /* teleport point                 */
   char        x_swap      =  '-';
   uchar      *s           = NULL;
   uchar      *t           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = ysort_pixie__prepare (&x_comp, &x_curr, &x_next, &x_tele);
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
         rc = ysort_pixie__swap_tele  (&x_swap, &x_comp, &x_next, &x_curr, &x_tele);
      } else {
         ++s_comps;
         s = g_key (x_comp);
         t = g_key (x_curr);
         rc = strcmp (s, t);
         if (rc <= 0) {
            rc = ysort_pixie__swap_tele  (&x_swap, &x_comp, &x_next, &x_curr, &x_tele);
         } else {
            rc = ysort_pixie__back_up    (&x_swap, &x_comp, &x_next);
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
ySORT_pixie              (void *p_walk, void *p_key, void *p_unhook, void *p_hook)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   ysort__reinit ();
   /*---(walk/cursor)--------------------*/
   DEBUG_YSORT   yLOG_point   ("p_walk"    , p_walk);
   --rce;  if (p_walk == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_walk   = p_walk;
   /*---(key provider)-------------------*/
   DEBUG_YSORT   yLOG_point   ("p_key"     , p_key);
   --rce;  if (p_key == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_key    = p_key;
   /*---(list unhook)--------------------*/
   DEBUG_YSORT   yLOG_point   ("p_unhook"  , p_unhook);
   --rce;  if (p_unhook  == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_unhook    = p_unhook;
   /*---(list hook)----------------------*/
   DEBUG_YSORT   yLOG_point   ("p_hook"    , p_hook);
   --rce;  if (p_hook  == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_hook      = p_hook;
   /*---(run)----------------------------*/
   rc = ysort_pixie  ();
   DEBUG_YSORT   yLOG_value   ("driver"    , rc);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNIT_TEST_______________o (void) {;}

/*> char          unit_answer [LEN_RECD];                                             <*/

/*> char*            /+ [------] unit test accessor ------------------------------+/                                                 <* 
 *> ysort_gnome__unit       (char *a_question, int a_num)                                                                            <* 
 *> {                                                                                                                                <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                                      <* 
 *>    char        s           [LEN_LABEL];                                                                                          <* 
 *>    char        t           [LEN_RECD ];                                                                                          <* 
 *>    int         i           =    0;                                                                                               <* 
 *>    int         n           =    0;                                                                                               <* 
 *>    int         x_fore      =    0;                                                                                               <* 
 *>    int         x_back      =    0;                                                                                               <* 
 *>    tMOCK *x_curr      = NULL;                                                                                               <* 
 *>    /+---(initialize)---------------------+/                                                                                      <* 
 *>    strlcpy (unit_answer, "GNOME unit       : unknown request", 100);                                                             <* 
 *>    /+---(basics)-------------------------+/                                                                                      <* 
 *>    if      (strcmp (a_question, "count"         ) == 0) {                                                                        <* 
 *>       x_curr = (tMOCK *) g_head; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->next; }                                <* 
 *>       x_curr = (tMOCK *) g_tail; while (x_curr != NULL) { ++x_back; x_curr = x_curr->prev; }                                <* 
 *>       snprintf (unit_answer, LEN_FULL, "GNOME count      : %4dn, %4df, %4db", g_count, x_fore, x_back);                          <* 
 *>    }                                                                                                                             <* 
 *>    else if (strcmp (a_question, "seq"           ) == 0) {                                                                        <* 
 *>       x_curr = (tMOCK *) g_head;                                                                                            <* 
 *>       strlcpy (t, "", LEN_RECD);                                                                                                 <* 
 *>       for (i = 0; i < 20; ++i) {                                                                                                 <* 
 *>          if (x_curr == NULL)  sprintf (s, "--");                                                                                 <* 
 *>          else                 sprintf (s, "%02d", x_curr->seq);                                                                  <* 
 *>          if (i > 0)  strlcat (t, " ", LEN_RECD);                                                                                 <* 
 *>          strlcat (t, s  , LEN_RECD);                                                                                             <* 
 *>          if (x_curr!= NULL)  x_curr = x_curr->next;                                                                              <* 
 *>       }                                                                                                                          <* 
 *>       snprintf (unit_answer, LEN_RECD, "GNOME seq        : %s", t);                                                              <* 
 *>    }                                                                                                                             <* 
 *>    else if (strcmp (a_question, "sort"          ) == 0) {                                                                        <* 
 *>       x_curr  = (tMOCK *) g_head;                                                                                           <* 
 *>       while (x_curr != NULL) {                                                                                                   <* 
 *>          if (n == a_num)  break;                                                                                                 <* 
 *>          ++n;                                                                                                                    <* 
 *>          x_curr = x_curr->next;                                                                                                  <* 
 *>       }                                                                                                                          <* 
 *>       if (x_curr == NULL)  n = -1;                                                                                               <* 
 *>       if (x_curr == NULL)  snprintf (unit_answer, LEN_RECD, "GNOME sort  (%2d) : --, ---", a_num);                               <* 
 *>       else                 snprintf (unit_answer, LEN_RECD, "GNOME sort  (%2d) : %2d, %s", a_num, x_curr->seq, x_curr->label);   <* 
 *>    }                                                                                                                             <* 
 *>    /+---(complete)-----------------------+/                                                                                      <* 
 *>    return unit_answer;                                                                                                           <* 
 *> }                                                                                                                                <*/


