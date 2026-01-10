/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"



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


/*
 * ySORT_hook ()
 *    -- r_link can be null and work awesome
 *    -- a_sort must be in the a_data structure so it endures (we don't copy)
 *
 *
 *
 *
 *
 */

/*----------+-----------+-----------+-----------+-----------+-----------+-----*/
#define     MAX_BTREE   20
typedef     struct      cROOTS      tROOTS;
struct      cROOTS {
   /*---(ref)---------------*/
   char        abbr;
   char        name        [LEN_LABEL];
   char        ready;
   /*---(tree)--------------*/
   tSORT      *head;
   tSORT      *tail;
   tSORT      *root;
   /*---(searches)----------*/
   char        search      [LEN_TITLE];
   /*---(sequential)--------*/
   tSORT      *saved;
   tSORT      *push;
   /*---(stats)-------------*/
   int         count;
   int         depth;
   /*---(stats)-------------*/
   tSORT      *last;
   /*---(done)--------------*/
} static s_trees [MAX_BTREE];
char        s_ntree    = 0;

#define     B_ABBR      s_trees [n].abbr
#define     B_NAME      s_trees [n].name
#define     B_READY     s_trees [n].ready
#define     B_HEAD      s_trees [n].head
#define     B_TAIL      s_trees [n].tail
#define     B_ROOT      s_trees [n].root
#define     B_SEARCH    s_trees [n].search
#define     B_SAVED     s_trees [n].saved
#define     B_PUSH      s_trees [n].push
#define     B_COUNT     s_trees [n].count
#define     B_DEPTH     s_trees [n].depth
#define     B_LAST      s_trees [n].last


int   g_depth   = 0;
char  g_path    [LEN_HUND] = "";



/*====================------------------------------------====================*/
/*===----                     creation/destruction                     ----===*/
/*====================------------------------------------====================*/
static void  o___TREES___________o () { return; }

char
ysort_btree_by_abbr     (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(defense)------------------------*/
   for (i = 0; i < s_ntree; ++i) {
      if (s_trees [i].abbr == a_abbr)   return i;
   }
   /*---(complete)-----------------------*/
   return rce;
}

char
ysort_btree_wipe        (char n)
{
   B_ABBR  = '-';
   ystrlcpy (B_NAME, "", LEN_LABEL);
   B_READY = '-';
   B_HEAD  = NULL;
   B_TAIL  = NULL;
   B_ROOT  = NULL;
   B_COUNT = 0;
   B_DEPTH = 0;
   B_SAVED = NULL;
   B_PUSH  = NULL;
   ystrlcpy (B_SEARCH, "", LEN_TITLE);
   B_LAST  = NULL;
   return 0;
}

char
ySORT_btree             (uchar a_abbr, cchar *a_sort)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        n           =    0;
   /*---(begin)--------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_sint    (s_ntree);
   --rce;  if (s_ntree >= MAX_BTREE) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(check abbr)---------------------*/
   DEBUG_YSORT   yLOG_schar   (a_abbr);
   --rce;  if (a_abbr <= 32 || a_abbr == 127) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(check duplicate)----------------*/
   n = ysort_btree_by_abbr (a_abbr);
   DEBUG_YSORT   yLOG_sint    (n);
   if (n >= 0)   ySORT_purge (a_abbr);
   else          n = s_ntree++;
   /*---(wipe)---------------------------*/
   DEBUG_YSORT   yLOG_snote   ("clear");
   ysort_btree_wipe (n);
   /*---(save)---------------------------*/
   s_trees [n].abbr = a_abbr;
   if (a_sort != NULL)  ystrlcpy (s_trees [n].name, a_sort, LEN_LABEL);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysort_btree_data        (char a_abbr, char *r_id, char r_name [LEN_LABEL], char *r_ready, tSORT ***r_head, tSORT ***r_tail, int *r_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_name  != NULL)  strcpy (r_name, "");
   if (r_id    != NULL)  *r_id    =   0;
   if (r_ready != NULL)  *r_ready = '-';
   if (r_head  != NULL)  *r_head  = NULL;
   if (r_tail  != NULL)  *r_tail  = NULL;
   if (r_count != NULL)  *r_count = -1;
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_YSORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_char    ("B_READY"   , B_READY);
   DEBUG_YSORT   yLOG_point   ("B_HEAD"    , B_HEAD);
   DEBUG_YSORT   yLOG_point   ("&(B_HEAD)" , &(B_HEAD));
   DEBUG_YSORT   yLOG_point   ("B_TAIL"    , B_TAIL);
   DEBUG_YSORT   yLOG_point   ("&(B_TAIL)" , &(B_TAIL));
   DEBUG_YSORT   yLOG_value   ("B_COUNT"   , B_COUNT);
   /*---(save-back)----------------------*/
   if (r_name  != NULL)  ystrlcpy (r_name, B_NAME, LEN_LABEL);
   if (r_id    != NULL)  *r_id    =   n;
   if (r_ready != NULL)  *r_ready = B_READY;
   if (r_head  != NULL)  *r_head  = &(B_HEAD);
   if (r_tail  != NULL)  *r_tail  = &(B_TAIL);
   if (r_count != NULL)  *r_count = B_COUNT;
   /*---(trouble)------------------------*/
   --rce;  if (B_TAIL == NULL) {
      DEBUG_YSORT   yLOG_note    ("zero items, no sorting required");
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (B_COUNT <= 1) {
      DEBUG_YSORT   yLOG_note    ("one item, no sorting required");
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}



/*====================------------------------------------====================*/
/*===----                       hooking/unhooking                      ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char         /*-> add item to tail -------------------------------------------*/
ySORT_hook              (uchar a_abbr, void *a_data, char a_sort [LEN_TITLE], tSORT **r_link)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        n           =   -1;
   int         l           =    0;
   tSORT      *x_new       = NULL;
   int         x_tries     =    0;
   /*---(begin)--------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_schar   (a_abbr);
   n = ysort_btree_by_abbr  (a_abbr);
   DEBUG_YSORT   yLOG_sint    (n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_snote   (B_NAME);
   DEBUG_YSORT   yLOG_spoint  (a_data);
   --rce;  if (a_data == NULL) {
      DEBUG_YSORT   yLOG_snote   ("a_data null");
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_spoint  (a_sort);
   --rce;  if (a_sort == NULL || a_sort [0] == '\0') {
      DEBUG_YSORT   yLOG_snote   ("a_sort null/empty");
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_snote   (a_sort);
   l = strlen (a_sort);
   DEBUG_YSORT   yLOG_sint    (l);
   --rce;  if (l >= LEN_TITLE) {
      DEBUG_YSORT   yLOG_snote   ("a_sort too long");
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(create cell)--------------------*/
   DEBUG_YSORT   yLOG_snote   ("alloc");
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tSORT *) malloc (sizeof (tSORT));
      if (x_tries > 10)   break;
   }
   DEBUG_YSORT   yLOG_sint    (x_tries);
   DEBUG_YSORT   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear)--------------------------*/
   DEBUG_YSORT   yLOG_snote   ("data");
   x_new->n       = n;
   x_new->sort    = a_sort;
   x_new->data    = a_data;
   /*---(into linked list)---------------*/
   DEBUG_YSORT   yLOG_snote   ("link");
   x_new->next    = NULL;
   x_new->prev    = NULL;
   x_new->left    = NULL;
   x_new->right   = NULL;
   if (B_HEAD == NULL) {
      B_HEAD = B_TAIL = x_new;
   } else {
      x_new->prev  = B_TAIL;
      B_TAIL->next = x_new;
      B_TAIL       = x_new;
   }
   /*---(update count)-------------------*/
   ++B_COUNT;
   DEBUG_YSORT   yLOG_sint    (B_COUNT);
   DEBUG_YSORT   yLOG_snote   ("successful");
   /*---(update last)--------------------*/
   B_LAST = x_new;
   /*---(reverse link)-------------------*/
   if (r_link != NULL)  *r_link = x_new;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ySORT_bhook             (uchar a_abbr, void *a_data, char a_sort [LEN_TITLE], tSORT **r_link, int *r_dur)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT      *x_new       = NULL;
   char        x_offset    =  '-';
   tSORT      *x_closest   = NULL;
   tSORT      *n           = NULL;
   long        x_dur       =   0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(start clock)--------------------*/
   ysort__intern_nbeg ();
   /*---(find closest)-------------------*/
   rc = ysort_by_tree (a_abbr, a_sort, NULL, NULL, NULL, NULL, NULL, &x_offset, &x_closest);
   DEBUG_YSORT   yLOG_value   ("closest"    , rc);
   /*---(hook at tail)-------------------*/
   rc = ySORT_hook (a_abbr, a_data, a_sort, &x_new);
   DEBUG_YSORT   yLOG_value   ("hook"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set insert point)---------------*/
   DEBUG_YSORT   yLOG_char    ("x_offset"   , x_offset);
   --rce;  switch (x_offset) {
   case  '>' : n = x_closest->next; break;
   case  '=' : n = x_closest->next; break;
   case  '<' : n = x_closest;       break;
   default   : return rce;          break;
   }
   DEBUG_YSORT   yLOG_point   ("n"          , n);
   if (n == NULL)  {
      DEBUG_YSORT   yLOG_note    ("already at tail");
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(swap)---------------------------*/
   rc = ysort_intern_swap (n, x_new);
   DEBUG_YSORT   yLOG_value   ("swap"       , rc);
   /*---(start clock)--------------------*/
   x_dur = ysort__intern_nend ();
   if (r_dur != NULL)  *r_dur = x_dur;
   /*---(update btree)-------------------*/
   rc = ysort_btree_build (a_abbr);
   DEBUG_YSORT   yLOG_value   ("build"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(start clock)--------------------*/
   /*> x_dur = ysort__intern_nend ();                                                 <*/
   /*> if (r_dur != NULL)  *r_dur = x_dur;                                            <*/
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ySORT_unhook            (tSORT **r_link)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        n           =   -1;
   tSORT      *x_old       = NULL;
   /*---(beginning)----------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   DEBUG_YSORT   yLOG_spoint  (r_link);
   --rce;  if (r_link == NULL) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_old = *r_link;
   DEBUG_YSORT   yLOG_spoint  (x_old);
   --rce;  if (x_old == NULL) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(set index for macros)-----------*/
   n = x_old->n;
   DEBUG_YSORT   yLOG_schar   (B_ABBR);
   DEBUG_YSORT   yLOG_snote   (B_NAME);
   /*---(out of linked list)-------------*/
   DEBUG_YSORT   yLOG_snote   ("unlink");
   if (x_old->next != NULL)   x_old->next->prev = x_old->prev;
   else                       B_TAIL            = x_old->prev;
   if (x_old->prev != NULL)   x_old->prev->next = x_old->next;
   else                       B_HEAD            = x_old->next;
   /*---(update count)-------------------*/
   --B_COUNT;
   DEBUG_YSORT   yLOG_sint    (B_COUNT);
   /*---(free main)----------------------*/
   DEBUG_YSORT   yLOG_snote   ("free and null");
   x_old->data    = NULL;
   x_old->sort    = NULL;
   x_old->next    = NULL;
   x_old->prev    = NULL;
   x_old->left    = NULL;
   x_old->right   = NULL;
   free (x_old);
   *r_link = NULL;
   /*---(reset ready)--------------------*/
   DEBUG_YSORT   yLOG_snote   ("reset");
   B_READY = '-';
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ySORT_purge             (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        n           =   -1;
   char        rc          =    0;
   tSORT      *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_schar   (a_abbr);
   n = ysort_btree_by_abbr  (a_abbr);
   DEBUG_YSORT   yLOG_sint    (n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk through list)--------------*/
   x_curr = B_HEAD;
   DEBUG_YSORT   yLOG_spoint  (B_HEAD);
   while (x_curr != NULL) {
      rc = ySORT_unhook (&x_curr);
      x_curr = B_HEAD;
   }
   DEBUG_YSORT   yLOG_sint    (B_COUNT);
   --rce;  if (B_COUNT > 0) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(clean ends)---------------------*/
   B_HEAD  = NULL;
   B_TAIL  = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ySORT_purge_all         (void)
{
   int         i           =    0;
   for (i = 0; i < s_ntree; ++i) {
      ySORT_purge (s_trees [i].abbr);
      ysort_btree_wipe (i);
   }
   s_ntree = 0;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          sorting                             ----===*/
/*====================------------------------------------====================*/
static void  o___SORT____________o () { return; }

char
ySORT_list              (uchar a_abbr)
{
   char        rce         =  -10;
   int         n           =   -1;
   int         c           =    0;
   tSORT      *o           = NULL;          /* origin point                   */
   n = ysort_btree_by_abbr   (a_abbr);
   --rce;  if (n < 0)  return rce;
   o   = B_HEAD;
   while (o != NULL) {
      DEBUG_YSORT  yLOG_complex ("entry"     , "%2d %2d %s", c, o->n, o->sort);
      o   = o->next;
      ++c;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      sequential access                       ----===*/
/*====================------------------------------------====================*/
static void  o___SEQUENCE________o () { return; }

char
ySORT_push              (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_schar   (a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_YSORT   yLOG_sint    (n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-off)-----------------------*/
   DEBUG_YSORT   yLOG_spoint  (B_PUSH);
   DEBUG_YSORT   yLOG_spoint  (B_SAVED);
   B_PUSH  = B_SAVED;
   DEBUG_YSORT   yLOG_spoint  (B_PUSH);
   DEBUG_YSORT   yLOG_spoint  (B_SAVED);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ySORT_pop               (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_schar   (a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_YSORT   yLOG_sint    (n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-off)-----------------------*/
   DEBUG_YSORT   yLOG_spoint  (B_PUSH);
   DEBUG_YSORT   yLOG_spoint  (B_SAVED);
   B_SAVED = B_PUSH;
   DEBUG_YSORT   yLOG_spoint  (B_PUSH);
   DEBUG_YSORT   yLOG_spoint  (B_SAVED);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

int
ySORT_count             (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_YSORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(count)--------------------------*/
   DEBUG_YSORT   yLOG_value   ("count"     , B_COUNT);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return B_COUNT;
}

char
ysort_by_name           (uchar a_abbr, char a_name [LEN_TITLE], tSORT **r_entry, void **r_data, int *r_tries)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   tSORT      *o           = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_entry != NULL)  *r_entry = NULL;
   if (r_data  != NULL)  *r_data  = NULL;
   if (r_tries != NULL)  *r_tries = 0;
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_YSORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL || a_name [0] == '\0') {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_info    ("a_name"    , a_name);
   /*---(prepare)------------------------*/
   DEBUG_YSORT   yLOG_point   ("BHEAD"     , B_HEAD);
   --rce;  if (B_HEAD == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk)---------------------------*/
   DEBUG_YSORT   yLOG_note    ("walking");
   o = B_HEAD;
   while (o != NULL) {
      DEBUG_YSORT_M yLOG_complex ("check"     , "%d %p", c, o);
      if (strcmp (o->sort, a_name) == 0)  break;
      o = o->next;
      ++c;
   }
   /*---(save tries)---------------------*/
   DEBUG_YSORT   yLOG_value   ("c"         , c);
   if (r_tries != NULL)   *r_tries = c;
   /*---(trouble)------------------------*/
   DEBUG_YSORT   yLOG_point   ("o"         , o);
   --rce;  if (o == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save results)-------------------*/
   B_SAVED   = o;
   DEBUG_YSORT   yLOG_point   ("B_SAVED"   , B_SAVED->data);
   if (r_entry != NULL)   *r_entry = o;
   if (r_data  != NULL)   *r_data  = o->data;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char ySORT_by_name    (uchar a_abbr, char a_name [LEN_TITLE], void **r_data, int *r_tries) { return ysort_by_name   (a_abbr, a_name, NULL, r_data, r_tries); }

char
ysort_by_cursor         (uchar a_abbr, char a_dir, tSORT** r_entry, void **r_data, int *r_tries)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   tSORT      *o           = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_entry != NULL)  *r_entry = NULL;
   if (r_data  != NULL)  *r_data  = NULL;
   if (r_tries != NULL)  *r_tries = 0;
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_YSORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("B_HEAD"    , B_HEAD);
   --rce;  if (B_HEAD == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   if (B_SAVED == NULL)  B_SAVED = B_HEAD;
   /*---(navigate)-----------------------*/
   DEBUG_YSORT   yLOG_char    ("a_dir"     , a_dir);
   --rce;  if (strchr ("[<.>]+", a_dir) == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  switch (a_dir) {
   case YDLST_HEAD : o = B_HEAD;         c = 1;  break;
   case YDLST_PREV : o = B_SAVED->prev;  c = 1;  break;
   case YDLST_CURR : o = B_SAVED;        c = 0;  break;
   case YDLST_NEXT : o = B_SAVED->next;  c = 1;  break;
   case YDLST_TAIL : o = B_TAIL;         c = 1;  break;
   case YDLST_LADD : o = B_LAST;         c = 1;  break;
   }
   /*---(save tries)---------------------*/
   DEBUG_YSORT   yLOG_value   ("c"         , c);
   if (r_tries != NULL)   *r_tries = c;
   /*---(bounds)-------------------------*/
   DEBUG_YSORT   yLOG_point   ("o"         , o);
   --rce;  if (o == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save results)-------------------*/
   B_SAVED   = o;
   DEBUG_YSORT   yLOG_point   ("B_SAVED"   , B_SAVED->data);
   if (r_entry != NULL)   *r_entry = o;
   if (r_data  != NULL)   *r_data  = o->data;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char ySORT_by_cursor  (uchar a_abbr, char a_dir, void **r_data, int *r_tries) { return ysort_by_cursor (a_abbr, a_dir, NULL, r_data, r_tries); }

char*
ySORT_validate          (uchar a_abbr, char a_dir)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tSORT      *x_entry     = NULL;
   rc = ysort_by_cursor (a_abbr, a_dir, &x_entry, NULL, NULL);
   if (rc < 0)           return "(n/a)";
   if (x_entry == NULL)  return "(null)";
   return x_entry->sort;
}

char
ysort_by_index          (uchar a_abbr, int a_index, tSORT **r_entry, void **r_data, int *r_tries)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   int         c           =    0;
   tSORT      *o           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_entry != NULL)  *r_entry = NULL;
   if (r_data  != NULL)  *r_data  = NULL;
   if (r_tries != NULL)  *r_tries = 0;
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_YSORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_value   ("a_index"   , a_index);
   --rce;  if (a_index < 0) {
      DEBUG_YSORT   yLOG_note    ("requested negative position");
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_value   ("count"     , B_COUNT);
   --rce;  if (a_index >= B_COUNT) {
      DEBUG_YSORT   yLOG_note    ("requested position above current count");
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   DEBUG_YSORT   yLOG_point   ("BHEAD"     , B_HEAD);
   --rce;  if (B_HEAD == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk)---------------------------*/
   DEBUG_YSORT   yLOG_note    ("walking");
   o = B_HEAD;
   while (c < a_index) {
      DEBUG_YSORT_M yLOG_complex ("check"     , "%d %p", c, o);
      if (o == NULL)  break;
      o = o->next;
      ++c;
   }
   /*---(save tries)---------------------*/
   DEBUG_YSORT   yLOG_value   ("c"         , c);
   if (r_tries != NULL)   *r_tries = c;
   /*---(trouble)------------------------*/
   DEBUG_YSORT   yLOG_point   ("o"         , o);
   --rce;  if (o == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save results)-------------------*/
   B_SAVED   = o;
   DEBUG_YSORT   yLOG_point   ("B_SAVED"   , B_SAVED->data);
   if (r_entry != NULL)   *r_entry = o;
   if (r_data  != NULL)   *r_data  = o->data;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char ySORT_by_index   (uchar a_abbr, int a_index, void **r_data, int *r_tries) { return ysort_by_index (a_abbr, a_index, NULL, r_data, r_tries); }



/*====================------------------------------------====================*/
/*===----                         building                             ----===*/
/*====================------------------------------------====================*/
static void  o___BUILD___________o () { return; }

int 
ySORT_btree_depth       (int a_size)
{
   int         c           =    0;
   if (a_size <= 0)  return 0;
   while (a_size > 1) {
      a_size /= 2;
      ++c;
   }
   ++c;
   return c;
}

int 
ySORT_btree_span        (int a_levels)
{
   if (a_levels <= 0)  return 0;
   return pow (2, a_levels) - 1;
}

tSORT*
ysort_btree_nextlevel   (int n, int a_lvl, int a_pos, int a_dist, char a_dir, tSORT *a_node)
{
   /*---(locals)-----------+-----+-----+-*/
   tSORT      *x_node      = NULL;
   int         x_pos       =    0;
   int         c           =    0;
   /*---(pre-defense)--------------------*/
   if (a_dist == 0)   return NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(depth)--------------------------*/
   if (a_lvl + 1 > B_DEPTH)  B_DEPTH = a_lvl + 1;
   /*---(check new position)-------------*/
   x_node = a_node;
   switch (a_dir) {
   case 'L' :
      x_pos = a_pos - a_dist;
      if (x_pos <= 0) {
         DEBUG_YSORT   yLOG_note    ("too far left, skip level");
         x_node = ysort_btree_nextlevel (n, a_lvl, a_pos, a_dist / 2, 'L', x_node);
         DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
         return x_node;
      }
      break;
   case 'R' :
      x_pos = a_pos + a_dist;
      if (x_pos > B_COUNT) {
         DEBUG_YSORT   yLOG_note    ("too far right, skip level");
         x_node = ysort_btree_nextlevel (n, a_lvl, a_pos, a_dist / 2, 'R', x_node);
         DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
         return x_node;
      }
      break;
   default  :
      DEBUG_YSORT   yLOG_note    ("a_dir not LR, skipping");
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return NULL;
      break;
   }
   /*---(get to new node)----------------*/
   while (c < a_dist) {
      if (a_dir == 'L')   x_node = x_node->prev;
      if (a_dir == 'R')   x_node = x_node->next;
      ++c;
   }
   /*---(recurse)------------------------*/
   DEBUG_YSORT   yLOG_complex ("a_abbr"    , "%2d %c %4d %4d %s", a_lvl, a_dir, a_dist, x_pos, x_node->sort);
   x_node->left  = ysort_btree_nextlevel (n, a_lvl + 1, x_pos, a_dist / 2, 'L', x_node);
   x_node->right = ysort_btree_nextlevel (n, a_lvl + 1, x_pos, a_dist / 2, 'R', x_node);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return x_node;
}

char
ysort_btree_build       (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tSORT      *x_node      = NULL;
   int         x_lvl       =    0;
   int         x_span      =    0;
   int         x_ctr       =    0;
   int         c           =    0;
   int         n           =  -1;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_YSORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (B_COUNT == 0) {
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   x_lvl  = ySORT_btree_depth (B_COUNT);
   DEBUG_YSORT   yLOG_value   ("est depth" , x_lvl);
   x_span = ySORT_btree_span (x_lvl);
   DEBUG_YSORT   yLOG_value   ("max span"  , x_span);
   DEBUG_YSORT   yLOG_double  ("est usage" , B_COUNT / 2.0 + 1);
   x_ctr  = (B_COUNT / 2) + 1;
   DEBUG_YSORT   yLOG_value   ("center"    , x_ctr);
   B_ROOT = B_HEAD;
   c = 1;
   while (c < x_ctr) {
      B_ROOT = B_ROOT->next;
      ++c;
   }
   DEBUG_YSORT   yLOG_info    ("ROOT"      , B_ROOT->sort);
   B_ROOT->left  = ysort_btree_nextlevel (n, 1, x_ctr, x_span / 4 + 1, 'L', B_ROOT);
   B_ROOT->right = ysort_btree_nextlevel (n, 1, x_ctr, x_span / 4 + 1, 'R', B_ROOT);
   DEBUG_YSORT   yLOG_value   ("real depth", B_DEPTH);
   /*---(mark as ready)------------------*/
   B_READY = 'y';
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char
ysort_btree_display     (int a_lvl, tSORT *a_node)
{
   char        x_pre       [LEN_RECD] = "";
   int         i           =    0;
   if (a_node == NULL)  return 0;
   if (a_lvl > 20)      return 0;
   for (i = 0; i < a_lvl; ++i)  strcat (x_pre, "  ");
   ysort_btree_display (a_lvl + 1, a_node->left);
   DEBUG_YSORT  yLOG_complex ("entry"     , "%s%s", x_pre, a_node->sort);
   ysort_btree_display (a_lvl + 1, a_node->right);
   return 0;
}

char
ySORT_treeform          (uchar a_abbr)
{
   char        rce         =  -10;
   int         n           =   -1;
   n = ysort_btree_by_abbr   (a_abbr);
   --rce;  if (n < 0)  return rce;
   ysort_btree_display (0, B_ROOT);
   return 0;
}

tSORT*
ysort_btree_searchdown  (tSORT *a_node, char *a_dir, char *a_key, char *b_depth, char b_path [LEN_TITLE], char *r_offset, tSORT **r_closest)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   /*---(defense)------------------------*/
   if (a_node == NULL)  return NULL;
   /*---(defailt)------------------------*/
   if (r_closest != NULL)  *r_closest = a_node;
   /*---(prepare)------------------------*/
   if (b_depth   != NULL)  *b_depth  += 1;
   if (b_path    != NULL)  ystrlcat (b_path, a_dir, LEN_TITLE);
   /*---(compare)------------------------*/
   rc = strcmp  (a_node->sort, a_key);
   DEBUG_YSORT_M yLOG_complex ("compare"   , "%s  %-20.20s  %4d", a_dir, a_node->sort, rc);
   /*---(handle)-------------------------*/
   if (rc >  0) {
      if (r_offset != NULL)  *r_offset = '<';
      return ysort_btree_searchdown (a_node->left , "L", a_key, b_depth, b_path, r_offset, r_closest);
   }
   if (rc <  0) {
      if (r_offset != NULL)  *r_offset = '>';
      return ysort_btree_searchdown (a_node->right, "R", a_key, b_depth, b_path, r_offset, r_closest);
   }
   /*---(perfecto)-----------------------*/
   if (r_offset != NULL)  *r_offset = '=';
   return a_node;
}

char
ysort_by_tree           (uchar a_abbr, char a_key [LEN_TITLE], tSORT **r_entry, void **r_data, int *r_tries, char *r_depth, char r_path [LEN_TITLE], char *r_offset, tSORT **r_closest)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   tSORT      *o           = NULL;
   char        x_depth     =    0;
   char        x_path      [LEN_TITLE] = "";
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_entry   != NULL)  *r_entry   = NULL;
   if (r_data    != NULL)  *r_data    = NULL;
   if (r_tries   != NULL)  *r_tries   = 0;
   if (r_depth   != NULL)  *r_depth   = 0;
   if (r_path    != NULL)  strcpy (r_path, "");
   if (r_offset  != NULL)  *r_offset  = '-';
   if (r_closest != NULL)  *r_closest = NULL;
   /*---(find tree)----------------------*/
   DEBUG_YSORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_YSORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_point   ("a_key"     , a_key);
   --rce;  if (a_key == NULL || a_key [0] == '\0') {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_info    ("a_key"     , a_key);
   /*---(check ready)--------------------*/
   DEBUG_YSORT   yLOG_char    ("B_READY"   , B_READY);
   --rce;  if (B_READY != 'y') {
      DEBUG_YSORT   yLOG_note    ("do not run ySORT_prepare yet");
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(search)-------------------------*/
   DEBUG_YSORT   yLOG_note    ("dive into btree");
   o = ysort_btree_searchdown (B_ROOT, "@", a_key, &x_depth, x_path, r_offset, r_closest);
   DEBUG_YSORT   yLOG_value   ("max depth" , B_DEPTH);
   DEBUG_YSORT   yLOG_value   ("x_depth"   , x_depth);
   DEBUG_YSORT   yLOG_info    ("x_path"    , x_path);
   /*---(save tries)---------------------*/
   DEBUG_YSORT   yLOG_value   ("c"         , x_depth);
   if (r_tries   != NULL)  *r_tries   = x_depth;
   if (r_depth   != NULL)  *r_depth   = x_depth;
   if (r_path    != NULL)  strlcpy (r_path, x_path, LEN_TITLE);
   g_depth = x_depth;
   strlcpy (g_path, x_path, LEN_TITLE);
   /*---(check)--------------------------*/
   --rce;  if (o == NULL) {
      DEBUG_YSORT   yLOG_note    ("not found");
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save results)-------------------*/
   DEBUG_YSORT   yLOG_note    ("found");
   ystrlcpy (B_SEARCH, a_key, LEN_TITLE);
   B_SAVED   = o;
   DEBUG_YSORT   yLOG_point   ("B_SAVED"   , B_SAVED->data);
   /*---(save-back)----------------------*/
   if (r_entry   != NULL)  *r_entry   = o;
   if (r_data    != NULL)  *r_data    = o->data;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySORT_by_tree           (uchar a_abbr, char a_key [LEN_TITLE], void **r_data, int *r_tries)
{
   return ysort_by_tree (a_abbr, a_key, NULL, r_data, r_tries, NULL, NULL, NULL, NULL);
}



/*====================------------------------------------====================*/
/*===----                      full treatment                          ----===*/
/*====================------------------------------------====================*/
static void  o___FULL____________o () { return; }

char
ySORT_prepare           (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare proj)-------------------*/
   DEBUG_YSORT   yLOG_char    ("a_abbr"    , a_abbr);
   rc = ysort_intern (YSORT_DGNOME, a_abbr, NULL, NULL, NULL, NULL, NULL, NULL);
   DEBUG_YSORT   yLOG_value   ("dgnome"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ysort_btree_build (a_abbr);
   DEBUG_YSORT   yLOG_value   ("build"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*
ysort_btree_list        (char n, tSORT *a_beg, tSORT *a_end, int a_slots)
{
   int         i           =    0;
   int         c           =    0;
   tSORT      *o           = NULL;          /* origin point                   */
   char        x_num       [LEN_SHORT] = "";
   static char x_print     [LEN_RECD]  = "";
   if (n < 0)  return "(n/a)";
   if (a_slots <= 0)  return "(empty)";
   if (a_slots > 50)  return "(overrun)";
   o   = a_beg;
   sprintf (x_num, "%02d", a_slots);
   strlcpy (x_print, x_num, LEN_RECD);
   for (i = 0; i < a_slots; ++i) {
      if (o == NULL )  break;
      strlcat (x_print, "  "   , LEN_RECD);
      strlcat (x_print, o->sort, LEN_RECD);
      o   = o->next;
      ++c;
      if (o == a_end->next)  break;
   }
   if (c > 0) {
      strlcat (x_print, "  "   , LEN_RECD);
      sprintf (x_num, "[%02d]", c);
      strlcat (x_print, x_num, LEN_RECD);
      strlcat (x_print, "  Ï"  , LEN_RECD);
   }
   return x_print;
}

char*
ySORT_btree_list        (uchar a_abbr)
{
   int         n           =   -1;
   n = ysort_btree_by_abbr   (a_abbr);
   return ysort_btree_list (n, B_HEAD, B_TAIL, B_COUNT);
}

char*        /*-> tbd --------------------------------[ light  [us.JC0.271.X1]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ysort_btree__unit       (uchar a_btree, char *a_question, int i)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   tSORT      *u           = NULL;
   int         c           =    0;
   char        s           [LEN_LABEL] = "";
   char        t           [LEN_RECD]  = "åæ";
   /*---(early)--------------------------*/
   if        (strcmp (a_question, "trees"     )     == 0) {
      strcpy  (t, "å");
      for (i = 0; i < s_ntree; ++i) {
         sprintf (s, "%c", s_trees [i].abbr);
         ystrlcat (t, s, LEN_HUND);
      }
      ystrlcat (t, "æ", LEN_HUND);
      snprintf (unit_answer, LEN_RECD, "BTREE trees      : %2d  %s", s_ntree, t);
      return unit_answer;
   }
   /*---(defense)------------------------*/
   n = ysort_btree_by_abbr   (a_btree);
   --rce;  if (n < 0) {
      snprintf (unit_answer, LEN_RECD, "BTREE unit       : tree type (%c) unknown", a_btree);
      return unit_answer;
   }
   if (i <  0) {
      snprintf (unit_answer, LEN_RECD, "BTREE unit       : index (%d) is negative", i);
      return unit_answer;
   }
   u = B_HEAD;
   while (c < i) {
      if (u == NULL) break;
      u = u->next;
      ++c;
   }
   /*---(simple)-------------------------*/
   if (u != NULL) sprintf  (t, "å%.20sæ", u->sort);
   if        (strcmp (a_question, "summary"   )     == 0) {
      snprintf (unit_answer, LEN_RECD, "BTREE summary    : %c %-20.20s %3d", s_trees [n].abbr, s_trees [n].name, B_COUNT);
   }
   else if   (strcmp (a_question, "entry"     )     == 0) {
      snprintf (unit_answer, LEN_RECD, "BTREE entry (%2d) : %c %s", i, s_trees [n].abbr, t);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


