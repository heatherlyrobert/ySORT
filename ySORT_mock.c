/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"



/*====================------------------------------------====================*/
/*===----                   program level functions                    ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
ysort_mock_init         (void)
{
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(count)--------------------------*/
   DEBUG_YSORT   yLOG_snote   ("ground/reset globals");
   g_head  = NULL;
   g_tail  = NULL;
   g_count = 0;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysort_mock_wrap         (void)
{
   /*---(locals)-----------+-----------+-*/
   tMOCK *x_curr      = NULL;
   tMOCK *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   DEBUG_YSORT   yLOG_sint    (g_count);
   DEBUG_YSORT   yLOG_spoint  (g_head);
   DEBUG_YSORT   yLOG_spoint  (g_tail);
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
   DEBUG_YSORT   yLOG_sint    (g_count);
   DEBUG_YSORT   yLOG_spoint  (g_head);
   DEBUG_YSORT   yLOG_spoint  (g_tail);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      shared functions                        ----===*/
/*====================------------------------------------====================*/
static void      o___SHARED__________________o (void) {;}

char
ysort_mock__common      (char a_type, void *a_one  , void *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(check pointers)-----------------*/
   DEBUG_YSORT   yLOG_spoint  (a_one);
   --rce;  if (a_one == NULL && a_type != 'i') {
      return rce;
   }
   DEBUG_YSORT   yLOG_spoint  (a_two);
   --rce;  if (a_two == NULL) {
      return rce;
   }
   --rce;  if (a_one == a_two) {
      DEBUG_YSORT   yLOG_snote   ("same, no action");
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    pixie call-backs                          ----===*/
/*====================------------------------------------====================*/
static void      o___PIXIE___________________o (void) {;}

char         /*-> standard cursor function -----------------------------------*/
ysort_mock__move        (tMOCK **a_cur, char a_move)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         c           =    0;
   tMOCK *x_cur       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_spoint  (a_cur);
   --rce;  if (a_cur == NULL) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_cur = *a_cur;
   DEBUG_YSORT   yLOG_spoint  (x_cur);
   --rce;  if (x_cur == NULL) {
      /*---(non-bounce)------------------*/
      if (strchr (YDLST_DREL, a_move) != NULL) {
         DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*---(bounce types)----------------*/
      x_cur = g_head;
      DEBUG_YSORT   yLOG_spoint  (x_cur);
      if (x_cur == NULL) {
         DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(move)---------------------------*/
   DEBUG_YSORT   yLOG_schar   (a_move);
   --rce;  switch (a_move) {
   case YDLST_HEAD : case YDLST_DHEAD :   /* head         */
      x_cur = g_head;
      break;
   case YDLST_PREV : case YDLST_DPREV :   /* previous     */
      x_cur = x_cur->prev;
      break;
   case YDLST_CURR : case YDLST_DCURR :   /* current      */
      x_cur = x_cur;
      break;
   case YDLST_NEXT : case YDLST_DNEXT :   /* next         */
      x_cur = x_cur->next;
      break;
   case YDLST_TAIL : case YDLST_DTAIL :   /* tail         */
      x_cur = g_tail;
      break;
   default  :
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(check end)----------------------*/
   DEBUG_YSORT   yLOG_spoint  (x_cur);
   --rce;  if (x_cur == NULL) {
      /*---(non-bounce)------------------*/
      if (strchr (YDLST_DANGERS, a_move) != NULL) {
         *a_cur = x_cur;
      }
      /*---(all types)-------------------*/
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   *a_cur = x_cur;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char*
ysort_mock__key         (tMOCK *a_cur)
{
   if (a_cur != NULL)  return a_cur->label;
   return "";
}

char
ysort_mock__unhook      (tMOCK *a_cur)
{
   /*---(out of linked list)-------------*/
   if (a_cur->next != NULL)  a_cur->next->prev = a_cur->prev;
   else                      g_tail            = a_cur->prev;
   if (a_cur->prev != NULL)  a_cur->prev->next = a_cur->next;
   else                      g_head            = a_cur->next;
   /*---(ground)-------------------------*/
   a_cur->next = a_cur->prev = NULL;
   /*---(complete)-----------------------*/
   return 0;
}

char
ysort_mock__hook        (tMOCK *a_ref, tMOCK *a_cur)
{
   /*---(append normally)---------------*/
   if (a_ref == NULL) {
      if (g_head  == NULL) {
         g_head  = g_tail  = a_cur;
      } else {
         a_cur->prev   = g_tail;
         g_tail->next  = a_cur;
         g_tail        = a_cur;
      }
   }
   /*---(insert before ref)--------------*/
   else {
      if (a_ref->prev != NULL)   a_ref->prev->next = a_cur;
      else                       g_head            = a_cur;
      a_cur->prev   = a_ref->prev;
      a_cur->next   = a_ref;
      a_ref->prev   = a_cur;
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    gnome call-backs                          ----===*/
/*====================------------------------------------====================*/
static void      o___GNOME___________________o (void) {;}

char
ysort_mock__cursor      (uchar a_type, void *a_head, void *a_tail, void *a_beg, void **a_new, char a_action)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tMOCK *x_beg       = NULL;
   tMOCK *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(check pointers)-----------------*/
   DEBUG_YSORT   yLOG_spoint  (a_beg);
   DEBUG_YSORT   yLOG_spoint  (a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   DEBUG_YSORT   yLOG_snote   ("cast");
   x_beg    = (tMOCK *) a_beg;
   if (x_beg != NULL)  DEBUG_YSORT   yLOG_sint    (x_beg->seq);
   if (x_beg != NULL)  DEBUG_YSORT   yLOG_snote   (x_beg->label);
   /*---(update)-------------------------*/
   DEBUG_YSORT   yLOG_schar   (a_action);
   switch (a_action) {
   case '>' : if (x_beg != NULL) { x_new = x_beg->next;   break; }
   case ']' : x_new = (tMOCK *) a_tail;              break;
   case '<' : if (x_beg != NULL) { x_new = x_beg->prev;   break; }
   case '[' : x_new = (tMOCK *) a_head;              break;
   }
   /*---(save back)----------------------*/
   if (x_new != NULL)  DEBUG_YSORT   yLOG_sint    (x_new->seq);
   if (x_new != NULL)  DEBUG_YSORT   yLOG_snote   (x_new->label);
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysort_mock__checker     (uchar a_type, uchar a_lvl, void *a_one, void *a_two, uchar a_order)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK *x_one       = NULL;
   tMOCK *x_two       = NULL;
   int         x_len1, x_len2;
   char        s           [LEN_LABEL] = "";
   char        t           [LEN_LABEL] = "";
   char       *x_label     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ysort_mock__common ('c', a_one, a_two);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   DEBUG_YSORT   yLOG_snote   ("cast");
   x_one    = (tMOCK *) a_one;
   x_two    = (tMOCK *) a_two;
   DEBUG_YSORT   yLOG_char    ("a_order"   , a_order);
   /*---(handle normal)------------------*/
   if (a_order == YSORT_ASCEND || a_order == YSORT_DESCEND) {
      /*---(prepare)------------------------*/
      x_len1 = strlen (x_one->label);
      x_len2 = strlen (x_two->label);
      DEBUG_YSORT   yLOG_value   ("a_lvl"     , a_lvl);
      if (x_len1 <= a_lvl && x_len2 <= a_lvl) {
         DEBUG_YSORT   yLOG_note    ("both are too short");
         rc = 0;
      }
      else if (x_len1 <= a_lvl) {
         DEBUG_YSORT   yLOG_note    ("first is too short");
         rc = 1;
      }
      else if (x_len2 <= a_lvl) {
         DEBUG_YSORT   yLOG_note    ("second is too short");
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
   /*---(handle search key)--------------*/
   else {
      x_label  = (char *) a_two;
      rc = strcmp (x_one->label, x_label);
   }
   /*---(check reversal)-----------------*/
   DEBUG_YSORT   yLOG_value   ("rc"        , rc);
   if (a_order == YSORT_DESCEND || a_order == YSORT_REVERSE) {
      rc *= -1;
      DEBUG_YSORT   yLOG_value   ("updated"   , rc);
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysort_mock__unlinker    (uchar a_type, void **a_head, void **a_tail, void *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK *x_head      = NULL;
   tMOCK *x_tail      = NULL;
   tMOCK *x_two       = NULL;
   char        h           [LEN_LABEL] = "NULL";
   char        t           [LEN_LABEL] = "NULL";
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ysort_mock__common ('i', NULL, a_two);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   DEBUG_YSORT   yLOG_complex ("pointers"  , "head %p, tail %p", a_head, a_tail);
   x_head   = * ((tMOCK **) a_head);
   x_tail   = * ((tMOCK **) a_tail);
   DEBUG_YSORT   yLOG_complex ("pointers"  , "head %p, tail %p", x_head, x_tail);
   x_two    = (tMOCK *) a_two;
   if (x_head != NULL)  ystrlcpy (h, x_head->label, LEN_LABEL);
   if (x_tail != NULL)  ystrlcpy (t, x_tail->label, LEN_LABEL);
   DEBUG_YSORT   yLOG_complex ("labels"    , "%-10.10s, %-10.10s, %-10.10s", h, t, x_two->label);
   /*---(unlink current from list)-------*/
   DEBUG_YSORT   yLOG_note    ("unlink");
   if (x_two->next != NULL)   x_two->next->prev = x_two->prev;
   else                       x_tail            = x_two->prev;
   if (x_two->prev != NULL)   x_two->prev->next = x_two->next;
   else                       x_head            = x_two->next;
   /*---(ground pointers)----------------*/
   DEBUG_YSORT   yLOG_note    ("ground pointers");
   x_two->next = NULL;
   x_two->prev = NULL;
   /*---(save back)----------------------*/
   DEBUG_YSORT   yLOG_complex ("pointers"  , "head %p, tail %p", x_head, x_tail);
   DEBUG_YSORT   yLOG_note    ("save back");
   *a_head = x_head;
   *a_tail = x_tail;
   DEBUG_YSORT   ysort_mock_printer (x_head);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysort_mock__linker      (uchar a_type, void **a_head, void **a_tail, void *a_one, void *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK *x_head      = NULL;
   tMOCK *x_tail      = NULL;
   tMOCK *x_one       = NULL;
   tMOCK *x_two       = NULL;
   char        h           [LEN_LABEL] = "NULL";
   char        t           [LEN_LABEL] = "NULL";
   char        o           [LEN_LABEL] = "NULL";
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ysort_mock__common ('i', a_one, a_two);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   x_head   = * ((tMOCK **) a_head);
   x_tail   = * ((tMOCK **) a_tail);
   x_one    = (tMOCK *) a_one;
   x_two    = (tMOCK *) a_two;
   if (x_head != NULL)  ystrlcpy (h, x_head->label, LEN_LABEL);
   if (x_tail != NULL)  ystrlcpy (t, x_tail->label, LEN_LABEL);
   if (x_one  != NULL)  ystrlcpy (o, x_one->label , LEN_LABEL);
   DEBUG_YSORT   yLOG_complex ("labels"    , "%-10.10s, %-10.10s, %-10.10s, %-10.10s", h, t, o, x_two->label);
   /*---(insert back in)-----------------*/
   if (a_one == NULL) {
      if (x_head == NULL) {
         DEBUG_YSORT   yLOG_note    ("add first");
         x_head       = x_two;
         x_two->prev  = NULL;
      } else {
         DEBUG_YSORT   yLOG_note    ("append to tail");
         x_tail->next = x_two;
         x_two->prev  = x_tail;
      }
      x_tail       = x_two;
      x_two->next  = NULL;
   } else {
      DEBUG_YSORT   yLOG_note    ("insert before");
      if (x_one->prev != NULL)   x_one->prev->next = x_two;
      else                       x_head            = x_two;
      x_two->prev   = x_one->prev;
      x_two->next   = x_one;
      x_one->prev   = x_two;
   }
   /*---(save back)----------------------*/
   DEBUG_YSORT   yLOG_note    ("save back");
   *a_head = x_head;
   *a_tail = x_tail;
   DEBUG_YSORT   ysort_mock_printer (x_head);
   /*---(output)-------------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    troll call-backs                          ----===*/
/*====================------------------------------------====================*/
static void      o___TROLL___________________o (void) {;}

char
ysort_mock__slotter     (uchar a_lvl, void *a_two, uchar a_order)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tMOCK *x_two       = NULL;
   int         x_len       =    0;
   char        t           [LEN_LABEL] = "";
   uchar       x_slot      =    0;
   /*---(begin)--------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_sint    (a_lvl);
   --rce;  if (a_lvl < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_spoint  (a_two);
   --rce;  if (a_two == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(cast)---------------------------*/
   x_two = (tMOCK *) a_two;
   /*---(normal)-------------------------*/
   if (a_order == YSORT_ASCEND || a_order == YSORT_DESCEND) {
      DEBUG_YSORT   yLOG_snote   (x_two->label);
      x_len = strlen (x_two->label);
      DEBUG_YSORT   yLOG_sint    (x_len);
      if (a_lvl < x_len)    x_slot = x_two->label [a_lvl];
      else                  x_slot = 0;
   }
   /*---(original)-----------------------*/
   else if (a_order == YSORT_ORIGINAL || a_order == YSORT_REVERSE) {
      if (a_lvl >= 5)  return 0;
      sprintf (t, "%05d", x_two->seq);
      DEBUG_YSORT   yLOG_snote   (t);
      x_len = strlen (t);
      DEBUG_YSORT   yLOG_sint    (x_len);
      if (a_lvl < x_len)    x_slot = t [a_lvl];
   }
   /*---(check reversal)-----------------*/
   DEBUG_YSORT   yLOG_sint    (x_slot);
   if (a_order == YSORT_DESCEND || a_order == YSORT_REVERSE) {
      x_slot = SEVENBIT - x_slot - 1;
      DEBUG_YSORT   yLOG_sint    (x_slot);
   }
   /*---(slot)---------------------------*/
   DEBUG_YSORT   yLOG_schar   (ychrvisible (x_slot));
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return x_slot;
}

char
ysort_mock__joiner      (void **a_bighead, void **a_bigtail, int *a_bigcount, void **a_subhead, void **a_subtail, int *a_subcount)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tMOCK *x_one       = NULL;
   tMOCK *x_two       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_point   ("a_subhead" , *a_subhead);
   --rce;  if (a_subhead == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(initial load)-------------------*/
   DEBUG_YSORT   yLOG_point   ("a_bighead" , *a_bighead);
   if (*a_bighead == NULL) {
      DEBUG_YSORT   yLOG_note    ("initial load");
      *a_bighead   = *a_subhead;
   }
   /*---(appending load)-----------------*/
   else {
      DEBUG_YSORT   yLOG_note    ("appending load");
      x_one = (tMOCK *) *a_bigtail;
      x_two = (tMOCK *) *a_subhead;
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
   DEBUG_YSORT   ysort_mock_printer (*a_bighead);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       searching support                      ----===*/
/*====================------------------------------------====================*/
static void      o___BTREE___________________o (void) {;}

char
ysort_mock__forker      (uchar a_type, void *a_node, void **a_left, void **a_right)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK *x_curr      = NULL;
   tMOCK *x_left      = NULL;
   tMOCK *x_right     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   DEBUG_YSORT   yLOG_complex ("args"      , "%c, %p, %p, %p", a_type, a_node, a_left, a_right);
   /*---(prepare)------------------------*/
   x_curr   = (tMOCK *) a_node;
   DEBUG_YSORT   yLOG_point   ("x_curr"    , x_curr);
   DEBUG_YSORT   yLOG_info    ("label"     , x_curr->label);
   /*---(set is lower)-------------------*/
   if (a_type == tolower (a_type)) {
      DEBUG_YSORT   yLOG_note    ("saving run");
      x_left  = * ((tMOCK **) a_left);
      x_right = * ((tMOCK **) a_right);
      if (a_left  != NULL)  x_curr->left  = x_left;
      if (a_right != NULL)  x_curr->right = x_right;
   }
   /*---(get is upper)-------------------*/
   else {
      DEBUG_YSORT   yLOG_note    ("retrieving run");
      if (a_left  != NULL)  *a_left  = x_left  = x_curr->left;
      if (a_right != NULL)  *a_right = x_right = x_curr->right;
   }
   /*---(output)-------------------------*/
   if (x_left  != NULL) { DEBUG_YSORT   yLOG_complex ("x_left"    , "%p, %s", x_left , x_left->label);  }
   else                 { DEBUG_YSORT   yLOG_complex ("x_left"    , "%p, %s", NULL   , "-"          );  }
   if (x_right != NULL) { DEBUG_YSORT   yLOG_complex ("x_right"   , "%p, %s", x_right, x_right->label); }
   else                 { DEBUG_YSORT   yLOG_complex ("x_right"   , "%p, %s", NULL   , "-"          );  }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                      testing use                             ----===*/
/*====================------------------------------------====================*/
static void      o___TESTING_________________o (void) {;}

char
ysort_mock_creator      (char *a_label)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_tries     =    0;
   tMOCK *x_new       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_info    ("a_label"   , a_label);
   /*---(create cell)--------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tMOCK *) malloc (sizeof (tMOCK));
      if (x_tries > 10)   break;
   }
   DEBUG_YSORT   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_YSORT   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   strlcpy (x_new->label, a_label, LEN_LABEL);
   x_new->seq   = g_count;
   x_new->left  = NULL;
   x_new->right = NULL;
   ++g_count;
   DEBUG_YSORT   yLOG_value   ("g_count"   , g_count);
   /*---(link it in)---------------------*/
   rc = ysort_mock__linker   ('-', &g_head, &g_tail, NULL, x_new);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysort_mock_printer      (tMOCK *x_head)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   tMOCK *x_curr      = NULL;
   x_curr = x_head;
   while (x_curr != NULL) {
      DEBUG_YSORT   yLOG_complex ("row"       , "%2d#, %2ds, %-20.20s, %p, %p, %p", i, x_curr->seq, x_curr->label, x_curr, x_curr->prev, x_curr->next);
      ++i;
      x_curr = x_curr->next;
   }
   return 0;
}


