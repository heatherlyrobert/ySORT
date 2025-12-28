/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"

/*
 *  internally used sorts for btree to test theories...
 *
 *  1) standard any, immediate, head gnome ······· ysort_intern_gnome
 *       for when it must be dead-simple
 *
 *  2) teleporting, any, delayed, head gnome ····· ysort_intern_dgnome
 *       for when you wait to then end to sort/btree
 *
 *  3) teleporting, single, delayed, tail gnome ·· ysort_intern_sgnome
 *       for when the sort/btree are used before adding items (dup checking)
 *
 */



static  char     s_abbr     = '-';
static  tSORT  **s_head     = NULL;
static  tSORT  **s_tail     = NULL;
static  int      s_count    = 0;

static  int      s_loops    = 0;
static  int      s_comps    = 0;
static  int      s_swaps    = 0;
static  int      s_teles    = 0;

static  int      s_SORTS    = 0;
static  int      s_LOOPS    = 0;
static  int      s_COMPS    = 0;
static  int      s_SWAPS    = 0;
static  int      s_TELES    = 0;


char*
ysort_intern_show       (void)
{
   char        x_head      [LEN_LABEL] = "";
   char        x_tail      [LEN_LABEL] = "";
   /*> if      (s_head  == NULL)  strcpy  (x_head, "(null)");                         <* 
    *> else if (*s_head == NULL)  strcpy  (x_head, "(unset)");                        <* 
    *> else                       sprintf (x_head, "%p", *s_head);                    <* 
    *> if      (s_tail  == NULL)  strcpy  (x_tail, "(null)");                         <* 
    *> else if (*s_tail == NULL)  strcpy  (x_tail, "(unset)");                        <* 
    *> else                       sprintf (x_tail, "%p", *s_tail);                    <*/
   sprintf (unit_answer, "%c  %-12.12s  %-12.12s  %3d    %3d  %3d  %3d  %3d    %3d  %3d  %3d  %3d  %3d  Ï",
         s_abbr, x_head, x_tail, s_count,
         s_loops, s_comps, s_swaps, s_teles,
         s_SORTS, s_LOOPS, s_COMPS, s_SWAPS, s_TELES);
   return unit_answer;
}

char
ysort_intern_init       (void)
{
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(initialize)---------------------*/
   s_abbr  = '-';
   s_head  = s_tail  = NULL;
   s_count = 0;
   s_loops = s_comps = s_swaps = s_teles = 0;
   s_LOOPS = s_COMPS = s_SWAPS = s_TELES = 0;
   s_SORTS = 0;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 1;
}

char
ysort_intern_prep       (tSORT *a_abbr, int *r_loops, int *r_comps, int *r_swaps, int *r_teles)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_name      [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_loops != NULL)  *r_loops = 0;
   if (r_comps != NULL)  *r_comps = 0;
   if (r_swaps != NULL)  *r_swaps = 0;
   if (r_teles != NULL)  *r_teles = 0;
   /*---(initialize)---------------------*/
   s_abbr  = a_abbr;
   s_loops = s_comps = s_swaps = s_teles = 0;
   /*---(get basics)---------------------*/
   DEBUG_YSORT   yLOG_char    ("a_abbr"    , a_abbr);
   rc = ysort_btree_data   (a_abbr, x_name, NULL, &s_head, &s_tail, &s_count);
   DEBUG_YSORT   yLOG_value   ("data"      , rc);
   --rce;  if (rc <= 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_YSORT   yLOG_info    ("x_name"    , x_name);
   DEBUG_YSORT   yLOG_point   ("s_head"    , s_head);
   DEBUG_YSORT   yLOG_point   ("*s_head"   , *s_head);
   DEBUG_YSORT   yLOG_point   ("s_tail"    , s_tail);
   DEBUG_YSORT   yLOG_point   ("*s_tail"   , *s_tail);
   DEBUG_YSORT   yLOG_value   ("s_count"   , s_count);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort_intern_done       (int *r_loops, int *r_comps, int *r_swaps, int *r_teles)
{
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(display)------------------------*/
   DEBUG_YSORT   yLOG_value   ("size"       , s_count);
   DEBUG_YSORT   yLOG_value   ("loops"      , s_loops);
   DEBUG_YSORT   yLOG_value   ("compares"   , s_comps);
   DEBUG_YSORT   yLOG_value   ("swaps"      , s_swaps);
   DEBUG_YSORT   yLOG_value   ("teleports"  , s_teles);
   /*---(accumulate)---------------------*/
   ++s_SORTS;
   s_LOOPS += s_loops;
   s_COMPS += s_comps;
   s_SWAPS += s_swaps;
   s_TELES += s_teles;
   /*---(save-back)----------------------*/
   if (r_loops != NULL)  *r_loops = s_loops;
   if (r_comps != NULL)  *r_comps = s_comps;
   if (r_swaps != NULL)  *r_swaps = s_swaps;
   if (r_teles != NULL)  *r_teles = s_teles;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort_intern_swap       (tSORT *a_one, tSORT *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(check preparation)--------------*/
   DEBUG_YSORT   yLOG_point   ("s_head"    , s_head);
   DEBUG_YSORT   yLOG_point   ("s_tail"    , s_tail);
   DEBUG_YSORT   yLOG_point   ("s_count"   , s_count);
   --rce;  if (s_head == NULL || s_tail == NULL) {
      DEBUG_YSORT   yLOG_note    ("must call ysort_intern_prep first");
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("*s_head"   , *s_head);
   DEBUG_YSORT   yLOG_point   ("*s_tail"   , *s_tail);
   /*---(check on one/two)---------------*/
   DEBUG_YSORT   yLOG_point   ("a_one"     , a_one);
   --rce;  if (a_one == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("a_two"     , a_two);
   --rce;  if (a_two == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check on self-reference)--------*/
   --rce;  if (a_one == a_two) {
      DEBUG_YSORT   yLOG_note    ("same, no action");
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(pull two out of list)-----------*/
   DEBUG_YSORT   yLOG_note    ("unlink");
   if (a_two->next != NULL)   a_two->next->prev = a_two->prev;
   else                       *s_tail           = a_two->prev;
   if (a_two->prev != NULL)   a_two->prev->next = a_two->next;
   else                       *s_head           = a_two->next;
   a_two->prev = NULL;
   a_two->next = NULL;
   /*---(insert before one)--------------*/
   DEBUG_YSORT   yLOG_note    ("insert");
   if (a_one->prev != NULL)   a_one->prev->next = a_two;
   else                       *s_head           = a_two;
   a_two->prev = a_one->prev;
   a_two->next = a_one;
   a_one->prev = a_two;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

/*
 *  any, delayed, head gnome
 *  -- any     means any number of additions could be present
 *  -- delayed means swaps are only performed after right position found
 *  -- head    means no assumpions on additions, so it starts at the head
 *
 */

char
ysort_intern_dgnome     (char a_abbr, int *r_loops, int *r_comps, int *r_swaps, int *r_teles)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT      *p           = NULL;          /* main index                     */
   tSORT      *t           = NULL;          /* teleport point                 */
   tSORT      *o           = NULL;          /* origin point                   */
   int         x_match     =    0;
   char        x_flag      =  '-';
   char        x_swap      =  '-';
   /*> /+---(header)-------------------------+/                                                                                                                    <* 
    *> DEBUG_YSORT   yLOG_enter   (__FUNCTION__);                                                                                                                  <* 
    *> /+---(defense)------------------------+/                                                                                                                    <* 
    *> rc = ysort_intern_prep (a_abbr, r_loops, r_comps, r_swaps, r_teles);                                                                                        <* 
    *> DEBUG_YSORT   yLOG_value   ("data"      , rc);                                                                                                              <* 
    *> --rce;  if (rc <= 0) {                                                                                                                                      <* 
    *>    DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rc);                                                                                                           <* 
    *>    return rc;                                                                                                                                               <* 
    *> }                                                                                                                                                           <* 
    *> /+---(prepare)------------------------+/                                                                                                                    <* 
    *> o   = (*s_head)->next;                                                                                                                                      <* 
    *> if (o != NULL) {                                                                                                                                            <* 
    *>    p = o->prev;                                                                                                                                             <* 
    *>    t = o->next;                                                                                                                                             <* 
    *> }                                                                                                                                                           <* 
    *> /+---(do the gnome walk)--------------+/                                                                                                                    <* 
    *> while (o != NULL) {                                                                                                                                         <* 
    *>    /+---(beginning)-------------------+/                                                                                                                    <* 
    *>    if (p == NULL) {                                                                                                                                         <* 
    *>       DEBUG_YSORT   yLOG_note    ("bounce off beginning");                                                                                                  <* 
    *>       if (x_swap == 'y') {                                                                                                                                  <* 
    *>          ++s_swaps;                                                                                                                                         <* 
    *>          ysort_intern_swap (*s_head, o);                                                                                                                    <* 
    *>          x_swap = '-';                                                                                                                                      <* 
    *>       }                                                                                                                                                     <* 
    *>       ++s_teles;                                                                                                                                            <* 
    *>       o = t;                                                                                                                                                <* 
    *>       if (o != NULL) {                                                                                                                                      <* 
    *>          p = o->prev;                                                                                                                                       <* 
    *>          t = o->next;                                                                                                                                       <* 
    *>       }                                                                                                                                                     <* 
    *>       continue;                                                                                                                                             <* 
    *>    }                                                                                                                                                        <* 
    *>    /+---(compare)---------------------+/                                                                                                                    <* 
    *>    ++s_comps;                                                                                                                                               <* 
    *>    x_match = strcmp (p->sort, o->sort);                                                                                                                     <* 
    *>    x_flag  = (x_match <= 0) ? 'y' : '#';                                                                                                                    <* 
    *>    DEBUG_YSORT   yLOG_complex ("compare"   , "%-20.20s v %-20.20s   %c %4d   %4d %4d %4d", p->sort, o->sort, x_flag, x_match, s_comps, s_teles, s_swaps);   <* 
    *>    if (x_match <= 0) {                                                                                                                                      <* 
    *>       if (x_swap == 'y') {                                                                                                                                  <* 
    *>          ++s_swaps;                                                                                                                                         <* 
    *>          ysort_intern_swap (p->next, o);                                                                                                                    <* 
    *>          x_swap = '-';                                                                                                                                      <* 
    *>       }                                                                                                                                                     <* 
    *>       ++s_teles;                                                                                                                                            <* 
    *>       o = t;                                                                                                                                                <* 
    *>       if (o != NULL) {                                                                                                                                      <* 
    *>          p = o->prev;                                                                                                                                       <* 
    *>          t = o->next;                                                                                                                                       <* 
    *>       }                                                                                                                                                     <* 
    *>       continue;                                                                                                                                             <* 
    *>    }                                                                                                                                                        <* 
    *>    /+---(swap)------------------------+/                                                                                                                    <* 
    *>    x_swap = 'y';                                                                                                                                            <* 
    *>    p = p->prev;                                                                                                                                             <* 
    *>    /+---(next)------------------------+/                                                                                                                    <* 
    *> }                                                                                                                                                           <* 
    *> /+---(statistics)---------------------+/                                                                                                                    <* 
    *> ysort_intern_done (r_loops, r_comps, r_swaps, r_teles);                                                                                                     <* 
    *> /+---(complete)-----------------------+/                                                                                                                    <*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*
 *  single, delayed, tail gnome
 *  -- single  means sorts after every single addition (no group additions)
 *  -- delayed means swaps are only performed after right position found
 *  -- tail    means additions are always added to the tail
 *
 */

char
ysort_intern_sgnome     (char a_abbr, int *r_loops, int *r_comps, int *r_swaps, int *r_teles)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT      *x_curr      = NULL;          /* current                        */
   tSORT      *x_prev      = NULL;          /* previous                       */
   int         x_match     =    0;
   char        x_flag      =  '-';
   char        x_swap      =  '-';
   /*> /+---(header)-------------------------+/                                                                                                                              <* 
    *> DEBUG_YSORT   yLOG_enter   (__FUNCTION__);                                                                                                                            <* 
    *> /+---(defense)------------------------+/                                                                                                                              <* 
    *> rc = ysort_intern_prep (a_abbr, r_loops, r_comps, r_swaps, r_teles);                                                                                                  <* 
    *> DEBUG_YSORT   yLOG_value   ("data"      , rc);                                                                                                                        <* 
    *> --rce;  if (rc <= 0) {                                                                                                                                                <* 
    *>    DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rc);                                                                                                                     <* 
    *>    return rc;                                                                                                                                                         <* 
    *> }                                                                                                                                                                     <* 
    *> /+---(prepare)------------------------+/                                                                                                                              <* 
    *> x_curr  = s_tail;                                                                                                                                                     <* 
    *> x_prev  = x_curr->prev;                                                                                                                                               <* 
    *> /+---(do the gnome walk)--------------+/                                                                                                                              <* 
    *> while (x_curr != NULL) {                                                                                                                                              <* 
    *>    /+---(loop)------------------------+/                                                                                                                              <* 
    *>    DEBUG_YSORT   yLOG_complex ("position"  , "%4d) p=%p, c=%p, t=%p", ++s_loops, x_prev, x_curr, s_tail);                                                             <* 
    *>    if (x_prev == NULL && x_curr != s_tail) {                                                                                                                          <* 
    *>       DEBUG_YSORT   yLOG_note    ("hit head, no more comparisons");                                                                                                   <* 
    *>       break;                                                                                                                                                          <* 
    *>    }                                                                                                                                                                  <* 
    *>    /+---(compare)---------------------+/                                                                                                                              <* 
    *>    ++s_comps;                                                                                                                                                         <* 
    *>    if (x_prev == NULL)  x_match = -1;                                                                                                                                 <* 
    *>    else                 x_match = strcmp (x_prev->sort, x_curr->sort);                                                                                                <* 
    *>    x_flag  = (x_match <= 0) ? 'y' : '#';                                                                                                                              <* 
    *>    /+---(swap)------------------------+/                                                                                                                              <* 
    *>    DEBUG_YSORT   yLOG_complex ("compare"   , "%-20.20s v %-20.20s   %c %4d   %4d %4d %4d", x_prev->sort, x_curr->sort, x_flag, x_match, s_comps, s_teles, s_swaps);   <* 
    *>    if (x_match <= 0) {                                                                                                                                                <* 
    *>       DEBUG_YSORT   yLOG_note    ("found position, no more comparisons");                                                                                             <* 
    *>       break;                                                                                                                                                          <* 
    *>    }                                                                                                                                                                  <* 
    *>    /+---(swap)------------------------+/                                                                                                                              <* 
    *>    x_swap = 'y';                                                                                                                                                      <* 
    *>    x_curr = x_prev;                                                                                                                                                   <* 
    *>    x_prev = x_curr->prev;                                                                                                                                             <* 
    *>    /+---(next)------------------------+/                                                                                                                              <* 
    *> }                                                                                                                                                                     <* 
    *> /+---(swap if required)---------------+/                                                                                                                              <* 
    *> if (x_swap == 'y') {                                                                                                                                                  <* 
    *>    DEBUG_YSORT   yLOG_note    ("timp to swap");                                                                                                                       <* 
    *>    ++s_swaps;                                                                                                                                                         <* 
    *>    ysort_intern_swap (x_curr, s_tail);                                                                                                                                <* 
    *>    rc = 1;                                                                                                                                                            <* 
    *> } else {                                                                                                                                                              <* 
    *>    DEBUG_YSORT   yLOG_note    ("no swapping required, stop");                                                                                                         <* 
    *>    rc = 0;                                                                                                                                                            <* 
    *> }                                                                                                                                                                     <* 
    *> /+---(statistics)---------------------+/                                                                                                                              <* 
    *> ysort_intern_done (r_loops, r_comps, r_swaps, r_teles);                                                                                                               <* 
    *> /+---(complete)-----------------------+/                                                                                                                              <*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char*
ysort_entry             (int a_index, tSORT *a_entry)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   int         c           =    0;
   tSORT      *o           = NULL;
   char        x_num       [LEN_SHORT] = "";
   char        x_prev      [LEN_DESC]  = "";
   char        x_curr      [LEN_DESC]  = "";
   char        x_next      [LEN_DESC]  = "";
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_sint    (a_index);
   DEBUG_YSORT   yLOG_spoint  (a_entry);
   if (a_entry == NULL && a_index < 0)     return "(n/a)";
   /*---(walk)---------------------------*/
   if (a_entry == NULL) {
      DEBUG_YSORT   yLOG_snote   ("walking");
      DEBUG_YSORT   yLOG_spoint  (*s_head);
      o = *s_head;
      DEBUG_YSORT   yLOG_spoint  (o);
      while (c < a_index) {
         DEBUG_YSORT_M yLOG_sint    (c);
         if (o == NULL)  break;
         o = o->next;
         DEBUG_YSORT   yLOG_spoint  (o);
         ++c;
      }
      sprintf (x_num, "%02d", c);
   } else {
      o = a_entry;
      strcpy  (x_num, "··");
   }
   /*---(trouble)------------------------*/
   --rce;  if (o == NULL) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return "(n/a)";
   }
   /*---(write line)---------------------*/
   if (o->prev == NULL)  strcpy  (x_prev, "å-----head-----æ ··········");
   else                  sprintf (x_prev, "å%-12.12pæ %-10.10s", o->prev, o->prev->sort);
   DEBUG_YSORT   yLOG_snote   (x_prev);
   sprintf (x_curr, "å%-12.12pæ %-10.10s", o, o->sort);
   DEBUG_YSORT   yLOG_snote   (x_curr);
   if (o->next == NULL)  strcpy  (x_next, "å-----tail-----æ ··········");
   else                  sprintf (x_next, "å%-12.12pæ %-10.10s", o->next, o->next->sort);
   DEBUG_YSORT   yLOG_snote   (x_next);
   sprintf (unit_answer, "%-2.2s  %-27.27s  %-27.27s  %-27.27s  Ï", x_num, x_prev, x_curr, x_next);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return unit_answer;
}


