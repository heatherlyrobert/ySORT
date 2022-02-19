/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"


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
   /*---(stats)-------------*/
   int         count;
   int         depth;
   /*---(done)--------------*/
} static s_trees [MAX_BTREE];
char        s_ntree    = 0;

#define     B_READY     s_trees [n].ready
#define     B_HEAD      s_trees [n].head
#define     B_TAIL      s_trees [n].tail
#define     B_ROOT      s_trees [n].root
#define     B_COUNT     s_trees [n].count
#define     B_DEPTH     s_trees [n].depth
#define     B_SEARCH    s_trees [n].search
#define     B_SAVED     s_trees [n].saved


int   g_depth   = 0;
char  g_path    [LEN_HUND] = "";



/*====================------------------------------------====================*/
/*===----                     creation/destruction                     ----===*/
/*====================------------------------------------====================*/
static void  o___TREES___________o () { return; }

char
ySORT_btree             (char a_abbr, char *a_sort)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        n           =    0;
   /*---(begin)--------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_sint    (s_ntree);
   --rce;  if (s_ntree >= MAX_BTREE) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(check abbr)---------------------*/
   DEBUG_SORT   yLOG_schar   (a_abbr);
   --rce;  if (strchr (YSTR_CHARS, a_abbr) == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   DEBUG_SORT   yLOG_snote   ("clear");
   n = s_ntree;
   s_trees [n].abbr = '-';
   strlcpy (s_trees [n].name, "", LEN_LABEL);
   B_READY = '-';
   B_HEAD  = NULL;
   B_TAIL  = NULL;
   B_ROOT  = NULL;
   B_COUNT = 0;
   B_DEPTH = 0;
   B_SAVED = NULL;
   strlcpy (B_SEARCH, "", LEN_TITLE);
   /*---(save)---------------------------*/
   s_trees [n].abbr = a_abbr;
   if (a_sort != NULL)  strlcpy (s_trees [n].name, a_sort, LEN_LABEL);
   /*---(increment)----------------------*/
   ++s_ntree;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysort_btree_by_abbr     (char a_abbr)
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



/*====================------------------------------------====================*/
/*===----                       hooking/unhooking                      ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
ySORT_hook              (char a_abbr, void *a_data, char *a_sort, tSORT **r_link)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        n           =   -1;
   tSORT      *x_new       = NULL;
   int         x_tries     =    0;
   /*---(begin)--------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_schar   (a_abbr);
   n = ysort_btree_by_abbr  (a_abbr);
   DEBUG_SORT   yLOG_sint    (n);
   --rce;  if (n < 0) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (a_data);
   --rce;  if (a_data == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (a_sort);
   --rce;  if (a_sort == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(create cell)--------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tSORT *) malloc (sizeof (tSORT));
      if (x_tries > 10)   break;
   }
   DEBUG_SORT   yLOG_sint    (x_tries);
   DEBUG_SORT   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear)--------------------------*/
   DEBUG_SORT   yLOG_snote   ("data");
   x_new->n       = n;
   x_new->sort    = a_sort;
   x_new->data    = a_data;
   /*---(into linked list)---------------*/
   DEBUG_SORT   yLOG_snote   ("link");
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
   DEBUG_SORT   yLOG_sint    (B_COUNT);
   DEBUG_SORT   yLOG_snote   ("successful");
   /*---(reverse link)-------------------*/
   if (r_link != NULL)  *r_link = x_new;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
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
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   DEBUG_SORT   yLOG_spoint  (r_link);
   --rce;  if (r_link == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_old = *r_link;
   DEBUG_SORT   yLOG_spoint  (x_old);
   --rce;  if (x_old == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(set index for macros)-----------*/
   n = x_old->n;
   /*---(out of linked list)-------------*/
   DEBUG_SORT   yLOG_snote   ("unlink");
   if (x_old->next != NULL)   x_old->next->prev = x_old->prev;
   else                       B_TAIL            = x_old->prev;
   if (x_old->prev != NULL)   x_old->prev->next = x_old->next;
   else                       B_HEAD            = x_old->next;
   /*---(update count)-------------------*/
   --B_COUNT;
   DEBUG_SORT   yLOG_sint    (B_COUNT);
   /*---(free main)----------------------*/
   DEBUG_SORT   yLOG_snote   ("free and null");
   x_old->data    = NULL;
   x_old->sort    = NULL;
   x_old->next    = NULL;
   x_old->prev    = NULL;
   x_old->left    = NULL;
   x_old->right   = NULL;
   free (x_old);
   *r_link = NULL;
   /*---(reset ready)--------------------*/
   DEBUG_SORT   yLOG_snote   ("reset");
   B_READY = '-';
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ySORT_purge             (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        n           =   -1;
   char        rc          =    0;
   tSORT      *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_schar   (a_abbr);
   n = ysort_btree_by_abbr  (a_abbr);
   DEBUG_SORT   yLOG_sint    (n);
   --rce;  if (n < 0) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk through list)--------------*/
   x_curr = B_HEAD;
   DEBUG_SORT   yLOG_spoint  (B_HEAD);
   while (x_curr != NULL) {
      rc = ySORT_unhook (&x_curr);
      x_curr = B_HEAD;
   }
   DEBUG_SORT   yLOG_sint    (B_COUNT);
   --rce;  if (B_COUNT > 0) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(clean ends)---------------------*/
   B_HEAD  = NULL;
   B_TAIL  = NULL;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ySORT_purge_all         (void)
{
   int         i           =    0;
   for (i = 0; i < s_ntree; ++i) {
      ySORT_purge (s_trees [i].abbr);
   }
   s_ntree = 0;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          sorting                             ----===*/
/*====================------------------------------------====================*/
static void  o___SORT____________o () { return; }

static  s_swaps      = 0;
static  s_comps      = 0;
static  s_teles      = 0;

char
ysort_btree_swap        (char n, tSORT *a_one, tSORT *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   /*---(beginning)----------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   DEBUG_SORT   yLOG_spoint  (a_one);
   --rce;  if (a_one == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (a_two);
   --rce;  if (a_two == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_one == a_two) {
      DEBUG_SORT   yLOG_snote   ("same, no action");
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(cut two from list)--------------*/
   DEBUG_SORT   yLOG_snote   ("unlink");
   if (a_two->next != NULL)   a_two->next->prev = a_two->prev;
   else                       B_TAIL            = a_two->prev;
   if (a_two->prev != NULL)   a_two->prev->next = a_two->next;
   else                       B_HEAD            = a_two->next;
   /*---(insert before one)--------------*/
   DEBUG_SORT   yLOG_snote   ("insert");
   if (a_one->prev != NULL)   a_one->prev->next = a_two;
   else                       B_HEAD            = a_two;
   a_two->prev = a_one->prev;
   a_two->next = a_one;
   a_one->prev = a_two;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysort_btree_dgnome      (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   int         x_match     =    0;
   char        x_flag      =  '-';
   tSORT      *p           = NULL;          /* main index                     */
   tSORT      *t           = NULL;          /* teleport point                 */
   tSORT      *o           = NULL;          /* origin point                   */
   char        x_swap      =  '-';
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_SORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check list)---------------------*/
   DEBUG_SORT   yLOG_point   ("head"      , B_HEAD);
   --rce;  if (B_HEAD == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      /*> return rce;                                                                 <*/
      return 0;
   }
   DEBUG_SORT   yLOG_point   ("->next"    , B_HEAD->next);
   --rce;  if (B_HEAD->next == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   s_swaps = 0;
   s_comps = 0;
   s_teles = 0;
   o   = B_HEAD->next;
   if (o != NULL) {
      p = o->prev;
      t = o->next;
   }
   /*---(do the gnome walk)--------------*/
   while (o != NULL) {
      /*---(beginning)-------------------*/
      if (p == NULL) {
         DEBUG_SORT   yLOG_note    ("bounce off beginning");
         if (x_swap == 'y') {
            ++s_swaps;
            ysort_btree_swap (n, B_HEAD, o);
            x_swap = '-';
         }
         ++s_teles;
         o = t;
         if (o != NULL) {
            p = o->prev;
            t = o->next;
         }
         continue;
      }
      /*---(compare)---------------------*/
      ++s_comps;
      x_match = strcmp (p->sort, o->sort);
      x_flag  = (x_match <= 0) ? 'y' : '#';
      DEBUG_SORT   yLOG_complex ("compare"   , "%-20.20s v %-20.20s   %c %4d   %4d %4d %4d", p->sort, o->sort, x_flag, x_match, s_comps, s_teles, s_swaps);
      if (x_match <= 0) {
         if (x_swap == 'y') {
            ++s_swaps;
            ysort_btree_swap (n, p->next, o);
            x_swap = '-';
         }
         ++s_teles;
         o = t;
         if (o != NULL) {
            p = o->prev;
            t = o->next;
         }
         continue;
      }
      /*---(swap)------------------------*/
      x_swap = 'y';
      p = p->prev;
      /*---(next)------------------------*/
   }
   DEBUG_SORT   yLOG_value   ("size"       , B_COUNT);
   DEBUG_SORT   yLOG_value   ("compares"   , s_comps);
   DEBUG_SORT   yLOG_value   ("teleports"  , s_teles);
   DEBUG_SORT   yLOG_value   ("swaps"      , s_swaps);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      sequential access                       ----===*/
/*====================------------------------------------====================*/
static void  o___SEQUENCE________o () { return; }

char
ySORT_by_cursor         (char a_abbr, char a_dir, void **r_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   tSORT      *o           = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_data != NULL)  *r_data = NULL;
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_schar   (a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_SORT   yLOG_sint    (n);
   --rce;  if (n < 0) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (r_data);
   --rce;  if (r_data == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (B_HEAD);
   --rce;  if (B_HEAD == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   if (B_SAVED == NULL)  B_SAVED = B_HEAD;
   /*---(navigate)-----------------------*/
   DEBUG_SORT   yLOG_schar   (a_dir);
   --rce;  if (strchr ("[<.>]", a_dir) == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  switch (a_dir) {
   case YDLST_HEAD : o = B_HEAD;         break;
   case YDLST_PREV : o = B_SAVED->prev;  break;
   case YDLST_CURR : o = B_SAVED;        break;
   case YDLST_NEXT : o = B_SAVED->next;  break;
   case YDLST_TAIL : o = B_TAIL;         break;
   }
   /*---(bounds)-------------------------*/
   DEBUG_SORT   yLOG_spoint  (o);
   --rce;  if (o == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save position)------------------*/
   B_SAVED   = o;
   /*---(save results)-------------------*/
   DEBUG_SORT   yLOG_spoint  (B_SAVED->data);
   *r_data = o->data;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ySORT_by_index          (char a_abbr, int i, void **r_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   int         c           =    0;
   tSORT      *o           = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_data != NULL)  *r_data = NULL;
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_SORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_point   ("r_data"    , r_data);
   --rce;  if (r_data == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_value   ("i"         , i);
   --rce;  if (i < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_point   ("BHEAD"     , B_HEAD);
   --rce;  if (B_HEAD == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_note    ("walking");
   o = B_HEAD;
   while (c < i) {
      if (o == NULL)  break;
      o = o->next;
      ++c;
   }
   DEBUG_SORT   yLOG_point   ("o"         , o);
   --rce;  if (o == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save position)------------------*/
   B_SAVED   = o;
   /*---(save results)-------------------*/
   DEBUG_SORT   yLOG_point   ("data"      , B_SAVED->data);
   *r_data = o->data;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

int
ySORT_count             (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_SORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(count)--------------------------*/
   DEBUG_SORT   yLOG_value   ("count"     , B_COUNT);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return B_COUNT;
}



/*====================------------------------------------====================*/
/*===----                         building                             ----===*/
/*====================------------------------------------====================*/
static void  o___BUILD___________o () { return; }

int 
ysort_btree_depth       (int a_size)
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
ysort_btree_span        (int a_levels)
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
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(depth)--------------------------*/
   if (a_lvl + 1 > B_DEPTH)  B_DEPTH = a_lvl + 1;
   /*---(check new position)-------------*/
   x_node = a_node;
   switch (a_dir) {
   case 'L' :
      x_pos = a_pos - a_dist;
      if (x_pos <= 0) {
         DEBUG_SORT   yLOG_note    ("too far left, skip level");
         x_node = ysort_btree_nextlevel (n, a_lvl, a_pos, a_dist / 2, 'L', x_node);
         DEBUG_SORT   yLOG_exit    (__FUNCTION__);
         return x_node;
      }
      break;
   case 'R' :
      x_pos = a_pos + a_dist;
      if (x_pos > B_COUNT) {
         DEBUG_SORT   yLOG_note    ("too far right, skip level");
         x_node = ysort_btree_nextlevel (n, a_lvl, a_pos, a_dist / 2, 'R', x_node);
         DEBUG_SORT   yLOG_exit    (__FUNCTION__);
         return x_node;
      }
      break;
   default  :
      DEBUG_SORT   yLOG_note    ("a_dir not LR, skipping");
      DEBUG_SORT   yLOG_exit    (__FUNCTION__);
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
   DEBUG_SORT   yLOG_complex ("a_abbr"    , "%2d %c %4d %4d %s", a_lvl, a_dir, a_dist, x_pos, x_node->sort);
   x_node->left  = ysort_btree_nextlevel (n, a_lvl + 1, x_pos, a_dist / 2, 'L', x_node);
   x_node->right = ysort_btree_nextlevel (n, a_lvl + 1, x_pos, a_dist / 2, 'R', x_node);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return x_node;
}

char
ysort_btree_build       (char a_abbr)
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
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_SORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (B_COUNT == 0) {
      DEBUG_SORT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   x_lvl  = ysort_btree_depth (B_COUNT);
   DEBUG_SORT   yLOG_value   ("est depth" , x_lvl);
   x_span = ysort_btree_span (x_lvl);
   DEBUG_SORT   yLOG_value   ("max span"  , x_span);
   DEBUG_SORT   yLOG_double  ("est usage" , B_COUNT / 2.0 + 1);
   x_ctr  = (B_COUNT / 2) + 1;
   DEBUG_SORT   yLOG_value   ("center"    , x_ctr);
   B_ROOT = B_HEAD;
   c = 1;
   while (c < x_ctr) {
      B_ROOT = B_ROOT->next;
      ++c;
   }
   DEBUG_SORT   yLOG_info    ("ROOT"      , B_ROOT->sort);
   B_ROOT->left  = ysort_btree_nextlevel (n, 1, x_ctr, x_span / 4 + 1, 'L', B_ROOT);
   B_ROOT->right = ysort_btree_nextlevel (n, 1, x_ctr, x_span / 4 + 1, 'R', B_ROOT);
   DEBUG_SORT   yLOG_value   ("real depth", B_DEPTH);
   /*---(mark as ready)------------------*/
   B_READY = 'y';
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
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
   printf ("%s%s\n", x_pre, a_node->sort);
   ysort_btree_display (a_lvl + 1, a_node->right);
   return 0;
}

char
ySORT_list              (char a_abbr)
{
   char        rce         =  -10;
   int         n           =   -1;
   n = ysort_btree_by_abbr   (a_abbr);
   --rce;  if (n < 0)  return rce;
   ysort_btree_display (0, B_ROOT);
   return 0;
}

tSORT*
ysort_btree_searchdown  (tSORT *a_node, char *a_dir, char *a_key)
{
   int         rc          =    0;
   if (a_node == NULL)  return NULL;
   ++g_depth;
   strlcat (g_path, a_dir, LEN_HUND);
   rc = strcmp  (a_node->sort, a_key);
   DEBUG_SORT   yLOG_complex ("compare"   , "%s  %-20.20s  %4d", a_dir, a_node->sort, rc);
   if (rc >  0)  return ysort_btree_searchdown (a_node->left , "L", a_key);
   if (rc <  0)  return ysort_btree_searchdown (a_node->right, "R", a_key);
   return a_node;
}

char
ySORT_by_name           (char a_abbr, char *a_key, void **r_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   tSORT      *o           = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   DEBUG_SORT   yLOG_char    ("a_abbr"    , a_abbr);
   /*---(default)------------------------*/
   if (r_data != NULL)  *r_data = NULL;
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_point   ("a_key"     , a_key);
   --rce;  if (a_key == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_info    ("a_key"     , a_key);
   DEBUG_SORT   yLOG_point   ("r_data"    , r_data);
   --rce;  if (r_data == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find tree)----------------------*/
   n = ysort_btree_by_abbr   (a_abbr);
   DEBUG_SORT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check ready)--------------------*/
   --rce;  if (B_READY != 'y') {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   g_depth = 0;
   strlcpy (g_path, "", LEN_HUND);
   /*---(search)-------------------------*/
   DEBUG_SORT   yLOG_note    ("dive into btree");
   o = ysort_btree_searchdown (B_ROOT, "@", a_key);
   DEBUG_SORT   yLOG_value   ("max depth" , B_DEPTH);
   DEBUG_SORT   yLOG_value   ("g_depth"   , g_depth);
   DEBUG_SORT   yLOG_info    ("g_path"    , g_path);
   /*---(check)--------------------------*/
   --rce;  if (o == NULL) {
      DEBUG_SORT   yLOG_note    ("not found");
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   DEBUG_SORT   yLOG_note    ("found");
   strlcpy (B_SEARCH, a_key, LEN_TITLE);
   B_SAVED   = o;
   /*---(save results)-------------------*/
   DEBUG_SORT   yLOG_spoint  (B_SAVED->data);
   *r_data = o->data;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      full treatment                          ----===*/
/*====================------------------------------------====================*/
static void  o___FULL____________o () { return; }

char
ySORT_prepare           (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare proj)-------------------*/
   DEBUG_PROG   yLOG_char    ("a_abbr"    , a_abbr);
   rc = ysort_btree_dgnome (a_abbr);
   DEBUG_PROG   yLOG_value   ("dgnome"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ysort_btree_build (a_abbr);
   DEBUG_PROG   yLOG_value   ("build"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> tbd --------------------------------[ light  [us.JC0.271.X1]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ysort_btree__unit       (char a_btree, char *a_question, int i)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   tSORT      *u           = NULL;
   int         c           =    0;
   char        s           [LEN_LABEL] = "";
   char        t           [LEN_RECD]  = "еж";
   /*---(early)--------------------------*/
   if        (strcmp (a_question, "trees"     )     == 0) {
      strcpy  (t, "е");
      for (i = 0; i < s_ntree; ++i) {
         sprintf (s, "%c", s_trees [i].abbr);
         strlcat (t, s, LEN_HUND);
      }
      strlcat (t, "ж", LEN_HUND);
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
   if (u != NULL) sprintf  (t, "е%.20sж", u->sort);
   if        (strcmp (a_question, "summary"   )     == 0) {
      snprintf (unit_answer, LEN_RECD, "BTREE summary    : %c %-20.20s %3d", s_trees [n].abbr, s_trees [n].name, B_COUNT);
   }
   else if   (strcmp (a_question, "entry"     )     == 0) {
      snprintf (unit_answer, LEN_RECD, "BTREE entry (%2d) : %c %s", i, s_trees [n].abbr, t);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


