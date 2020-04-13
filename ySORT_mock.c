/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"



/*====================------------------------------------====================*/
/*===----                   program level functions                    ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
MOCK__init              (void)
{
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(count)--------------------------*/
   DEBUG_SORT   yLOG_snote   ("ground/reset globals");
   g_head  = NULL;
   g_tail  = NULL;
   g_count = 0;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
MOCK__wrap              (void)
{
   /*---(locals)-----------+-----------+-*/
   tSORT_DATA *x_curr      = NULL;
   tSORT_DATA *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   DEBUG_SORT   yLOG_sint    (g_count);
   DEBUG_SORT   yLOG_spoint  (g_head);
   DEBUG_SORT   yLOG_spoint  (g_tail);
   /*---(purge all)----------------------*/
   x_curr = g_head;
   while (x_curr != NULL) {
      x_next = x_curr->next;
      if (g_tail == x_curr)  g_tail = NULL;
      g_head = x_next;
      x_curr->next = NULL;
      x_curr->prev = NULL;
      free (x_curr);
      --g_count;
      x_curr = x_next;
   }
   DEBUG_SORT   yLOG_sint    (g_count);
   DEBUG_SORT   yLOG_spoint  (g_head);
   DEBUG_SORT   yLOG_spoint  (g_tail);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      shared functions                        ----===*/
/*====================------------------------------------====================*/
static void      o___SHARED__________________o (void) {;}

char
MOCK__common            (char a_type, void *a_one  , void *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(check pointers)-----------------*/
   DEBUG_SORT   yLOG_spoint  (a_one);
   --rce;  if (a_one == NULL && a_type != 'i') {
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (a_two);
   --rce;  if (a_two == NULL) {
      return rce;
   }
   --rce;  if (a_one == a_two) {
      DEBUG_SORT   yLOG_snote   ("same, no action");
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    gnome call-backs                          ----===*/
/*====================------------------------------------====================*/
static void      o___GNOME___________________o (void) {;}

char
MOCK__cursor            (uchar a_type, void *a_head, void *a_tail, void *a_beg, void **a_new, char a_action)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tSORT_DATA *x_beg       = NULL;
   tSORT_DATA *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(check pointers)-----------------*/
   DEBUG_SORT   yLOG_spoint  (a_beg);
   DEBUG_SORT   yLOG_spoint  (a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   DEBUG_SORT   yLOG_snote   ("cast");
   x_beg    = (tSORT_DATA *) a_beg;
   if (x_beg != NULL)  DEBUG_SORT   yLOG_sint    (x_beg->seq);
   if (x_beg != NULL)  DEBUG_SORT   yLOG_snote   (x_beg->label);
   /*---(update)-------------------------*/
   DEBUG_SORT   yLOG_schar   (a_action);
   switch (a_action) {
   case '>' : if (x_beg != NULL) { x_new = x_beg->next;   break; }
   case ']' : x_new = (tSORT_DATA *) a_tail;              break;
   case '<' : if (x_beg != NULL) { x_new = x_beg->prev;   break; }
   case '[' : x_new = (tSORT_DATA *) a_head;              break;
   }
   /*---(save back)----------------------*/
   if (x_new != NULL)  DEBUG_SORT   yLOG_sint    (x_new->seq);
   if (x_new != NULL)  DEBUG_SORT   yLOG_snote   (x_new->label);
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
MOCK__checker           (uchar a_type, uchar a_lvl, void *a_one, void *a_two, uchar a_order)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT_DATA *x_one       = NULL;
   tSORT_DATA *x_two       = NULL;
   int         x_len1, x_len2;
   char        s           [LEN_LABEL] = "";
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = MOCK__common ('c', a_one, a_two);
   --rce;  if (rc < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   DEBUG_SORT   yLOG_snote   ("cast");
   x_one    = (tSORT_DATA *) a_one;
   x_two    = (tSORT_DATA *) a_two;
   DEBUG_SORT   yLOG_char    ("a_order"   , a_order);
   /*---(handle normal)------------------*/
   if (a_order == YSORT_ASCEND || a_order == YSORT_DESCEND) {
      /*---(prepare)------------------------*/
      x_len1 = strlen (x_one->label);
      x_len2 = strlen (x_two->label);
      DEBUG_SORT   yLOG_value   ("a_lvl"     , a_lvl);
      if (x_len1 <= a_lvl && x_len2 <= a_lvl) {
         DEBUG_SORT   yLOG_note    ("both are too short");
         rc = 0;
      }
      else if (x_len1 <= a_lvl) {
         DEBUG_SORT   yLOG_note    ("first is too short");
         rc = 1;
      }
      else if (x_len2 <= a_lvl) {
         DEBUG_SORT   yLOG_note    ("second is too short");
         rc = -1;
      }
      else {
         rc = strcmp (x_one->label + a_lvl, x_two->label + a_lvl);
      }
   }
   /*---(handle original)----------------*/
   else if (a_order == YSORT_ORIGINAL || a_order == YSORT_REVERSE) {
      if (a_lvl >= 5)  return 0;
      sprintf (s, "%05d", x_one->seq);
      sprintf (t, "%05d", x_two->seq);
      rc = strcmp (s + a_lvl, t + a_lvl);
   }
   /*---(check reversal)-----------------*/
   DEBUG_SORT   yLOG_value   ("rc"        , rc);
   if (a_order == YSORT_DESCEND || a_order == YSORT_REVERSE) {
      rc *= -1;
      DEBUG_SORT   yLOG_value   ("updated"   , rc);
   }
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
MOCK__unlinker          (uchar a_type, void **a_head, void **a_tail, void *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT_DATA *x_head      = NULL;
   tSORT_DATA *x_tail      = NULL;
   tSORT_DATA *x_two       = NULL;
   char        h           [LEN_LABEL] = "NULL";
   char        t           [LEN_LABEL] = "NULL";
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = MOCK__common ('i', NULL, a_two);
   --rce;  if (rc < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   DEBUG_SORT   yLOG_complex ("pointers"  , "head %p, tail %p", a_head, a_tail);
   x_head   = * ((tSORT_DATA **) a_head);
   x_tail   = * ((tSORT_DATA **) a_tail);
   DEBUG_SORT   yLOG_complex ("pointers"  , "head %p, tail %p", x_head, x_tail);
   x_two    = (tSORT_DATA *) a_two;
   if (x_head != NULL)  strlcpy (h, x_head->label, LEN_LABEL);
   if (x_tail != NULL)  strlcpy (t, x_tail->label, LEN_LABEL);
   DEBUG_SORT   yLOG_complex ("labels"    , "%-10.10s, %-10.10s, %-10.10s", h, t, x_two->label);
   /*---(unlink current from list)-------*/
   DEBUG_SORT   yLOG_note    ("unlink");
   if (x_two->next != NULL)   x_two->next->prev = x_two->prev;
   else                       x_tail            = x_two->prev;
   if (x_two->prev != NULL)   x_two->prev->next = x_two->next;
   else                       x_head            = x_two->next;
   /*---(ground pointers)----------------*/
   DEBUG_SORT   yLOG_note    ("ground pointers");
   x_two->next = NULL;
   x_two->prev = NULL;
   /*---(save back)----------------------*/
   DEBUG_SORT   yLOG_complex ("pointers"  , "head %p, tail %p", x_head, x_tail);
   DEBUG_SORT   yLOG_note    ("save back");
   *a_head = x_head;
   *a_tail = x_tail;
   DEBUG_SORT   MOCK__printer (x_head);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MOCK__linker            (uchar a_type, void **a_head, void **a_tail, void *a_one, void *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT_DATA *x_head      = NULL;
   tSORT_DATA *x_tail      = NULL;
   tSORT_DATA *x_one       = NULL;
   tSORT_DATA *x_two       = NULL;
   char        h           [LEN_LABEL] = "NULL";
   char        t           [LEN_LABEL] = "NULL";
   char        o           [LEN_LABEL] = "NULL";
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = MOCK__common ('i', a_one, a_two);
   --rce;  if (rc < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   x_head   = * ((tSORT_DATA **) a_head);
   x_tail   = * ((tSORT_DATA **) a_tail);
   x_one    = (tSORT_DATA *) a_one;
   x_two    = (tSORT_DATA *) a_two;
   if (x_head != NULL)  strlcpy (h, x_head->label, LEN_LABEL);
   if (x_tail != NULL)  strlcpy (t, x_tail->label, LEN_LABEL);
   if (x_one  != NULL)  strlcpy (o, x_one->label , LEN_LABEL);
   DEBUG_SORT   yLOG_complex ("labels"    , "%-10.10s, %-10.10s, %-10.10s, %-10.10s", h, t, o, x_two->label);
   /*---(insert back in)-----------------*/
   if (a_one == NULL) {
      if (x_head == NULL) {
         DEBUG_SORT   yLOG_note    ("add first");
         x_head       = x_two;
         x_two->prev  = NULL;
      } else {
         DEBUG_SORT   yLOG_note    ("append to tail");
         x_tail->next = x_two;
         x_two->prev  = x_tail;
      }
      x_tail       = x_two;
      x_two->next  = NULL;
   } else {
      DEBUG_SORT   yLOG_note    ("insert before");
      if (x_one->prev != NULL)   x_one->prev->next = x_two;
      else                       x_head            = x_two;
      x_two->prev   = x_one->prev;
      x_two->next   = x_one;
      x_one->prev   = x_two;
   }
   /*---(save back)----------------------*/
   DEBUG_SORT   yLOG_note    ("save back");
   *a_head = x_head;
   *a_tail = x_tail;
   DEBUG_SORT   MOCK__printer (x_head);
   /*---(output)-------------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   /*---(complete)-----------------------*/
   return 0;
}

/*> char                                                                                            <* 
 *> MOCK__mover             (char a_type, void **a_head, void **a_tail, void *a_one, void *a_two)   <* 
 *> {                                                                                               <* 
 *>    /+---(locals)-----------+-----------+-+/                                                     <* 
 *>    char        rce         =  -10;                                                              <* 
 *>    char        rc          =    0;                                                              <* 
 *>    /+---(header)-------------------------+/                                                     <* 
 *>    DEBUG_SORT   yLOG_enter   (__FUNCTION__);                                                    <* 
 *>    /+---(defense)------------------------+/                                                     <* 
 *>    rc = MOCK__common ('i', a_one, a_two);                                                       <* 
 *>    --rce;  if (rc < 0) {                                                                        <* 
 *>       DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);                                            <* 
 *>       return 0;                                                                                 <* 
 *>    }                                                                                            <* 
 *>    /+---(process)------------------------+/                                                     <* 
 *>    rc = MOCK__unlinker       (a_type, a_head, a_tail, a_two);                                   <* 
 *>    rc = MOCK__linker         (a_type, a_head, a_tail, a_one, a_two);                            <* 
 *>    /+---(complete)-----------------------+/                                                     <* 
 *>    DEBUG_SORT   yLOG_exit    (__FUNCTION__);                                                    <* 
 *>    /+---(output)-------------------------+/                                                     <* 
 *>    MOCK__printer ();                                                                            <* 
 *>    return 0;                                                                                    <* 
 *> }                                                                                               <*/



/*====================------------------------------------====================*/
/*===----                    troll call-backs                          ----===*/
/*====================------------------------------------====================*/
static void      o___TROLL___________________o (void) {;}

char
MOCK__slotter           (uchar a_lvl, void *a_two, uchar a_order)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tSORT_DATA *x_two       = NULL;
   int         x_len       =    0;
   char        t           [LEN_LABEL] = "";
   uchar       x_slot      =    0;
   /*---(begin)--------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_sint    (a_lvl);
   --rce;  if (a_lvl < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (a_two);
   --rce;  if (a_two == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(cast)---------------------------*/
   x_two = (tSORT_DATA *) a_two;
   /*---(normal)-------------------------*/
   if (a_order == YSORT_ASCEND || a_order == YSORT_DESCEND) {
      DEBUG_SORT   yLOG_snote   (x_two->label);
      x_len = strlen (x_two->label);
      DEBUG_SORT   yLOG_sint    (x_len);
      if (a_lvl < x_len)    x_slot = x_two->label [a_lvl];
      else                  x_slot = 0;
   }
   /*---(original)-----------------------*/
   else if (a_order == YSORT_ORIGINAL || a_order == YSORT_REVERSE) {
      if (a_lvl >= 5)  return 0;
      sprintf (t, "%05d", x_two->seq);
      DEBUG_SORT   yLOG_snote   (t);
      x_len = strlen (t);
      DEBUG_SORT   yLOG_sint    (x_len);
      if (a_lvl < x_len)    x_slot = t [a_lvl];
   }
   /*---(check reversal)-----------------*/
   DEBUG_SORT   yLOG_sint    (x_slot);
   if (a_order == YSORT_DESCEND || a_order == YSORT_REVERSE) {
      x_slot = SEVENBIT - x_slot - 1;
      DEBUG_SORT   yLOG_sint    (x_slot);
   }
   /*---(slot)---------------------------*/
   DEBUG_SORT   yLOG_schar   (chrvisible (x_slot));
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return x_slot;
}

char
MOCK__joiner            (void **a_bighead, void **a_bigtail, int *a_bigcount, void **a_subhead, void **a_subtail, int *a_subcount)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tSORT_DATA *x_one       = NULL;
   tSORT_DATA *x_two       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_point   ("a_subhead" , *a_subhead);
   --rce;  if (a_subhead == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(initial load)-------------------*/
   DEBUG_SORT   yLOG_point   ("a_bighead" , *a_bighead);
   if (*a_bighead == NULL) {
      DEBUG_SORT   yLOG_note    ("initial load");
      *a_bighead   = *a_subhead;
   }
   /*---(appending load)-----------------*/
   else {
      DEBUG_SORT   yLOG_note    ("appending load");
      x_one = (tSORT_DATA *) *a_bigtail;
      x_two = (tSORT_DATA *) *a_subhead;
      x_one->next = x_two;
      x_two->prev = x_one;
   }
   /*---(common actions)-----------------*/
   *a_bigtail   = *a_subtail;
   *a_bigcount += *a_subcount;
   /*---(wipe sublist)-------------------*/
   *a_subhead   = NULL;
   *a_subtail   = NULL;
   *a_subcount  = 0;
   DEBUG_SORT   MOCK__printer (*a_bighead);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      testing use                             ----===*/
/*====================------------------------------------====================*/
static void      o___TESTING_________________o (void) {;}

char
MOCK__creator           (char *a_label)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_tries     =    0;
   tSORT_DATA *x_new       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_info    ("a_label"   , a_label);
   /*---(create cell)--------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tSORT_DATA *) malloc (sizeof (tSORT_DATA));
      if (x_tries > 10)   break;
   }
   DEBUG_SORT   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_SORT   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   strlcpy (x_new->label, a_label, LEN_LABEL);
   x_new->seq = g_count;
   ++g_count;
   DEBUG_SORT   yLOG_value   ("g_count"   , g_count);
   /*---(link it in)---------------------*/
   rc = MOCK__linker         ('-', &g_head, &g_tail, NULL, x_new);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
MOCK__printer           (tSORT_DATA *x_head)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   tSORT_DATA *x_curr      = NULL;
   x_curr = x_head;
   while (x_curr != NULL) {
      DEBUG_SORT   yLOG_complex ("row"       , "%2d#, %2ds, %-20.20s, %p, %p, %p", i, x_curr->seq, x_curr->label, x_curr, x_curr->prev, x_curr->next);
      ++i;
      x_curr = x_curr->next;
   }
   return 0;
}


