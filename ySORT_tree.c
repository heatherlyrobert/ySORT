/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"



static uchar    s_type   =  '-';
static int      s_span   =    0;
static char     s_levels =    0;
static int      s_count  =    0;
static float    s_filled =  0.0;

static char     s_result =    0;
static char     s_path   [LEN_DESC]  = "";
static char     s_depth  =    0;
static void    *s_last   = NULL;



static char    (*s_callback)   (uchar a_type, int a_lvl, void *a_node, char *a_path, char *a_pre);



/*====================------------------------------------====================*/
/*===----                        little helpers                        ----===*/
/*====================------------------------------------====================*/
static void  o___HELPERS_________o () { return; }

int 
ysort_tree__depth            (int a_size)
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
ysort_tree__span             (int a_levels)
{
   if (a_levels <= 0)  return 0;
   return pow (2, a_levels) - 1;
}



/*====================------------------------------------====================*/
/*===----                        build the btree                       ----===*/
/*====================------------------------------------====================*/
static void  o___BUILD___________o () { return; }

void*
ysort_tree__nextlevel   (uchar a_type, void *a_head, void *a_tail, int a_count, int a_lvl, int a_pos, int a_dist, char a_dir, void *a_node)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         x_pos       =    0;
   int         c           =    0;
   void       *x_curr      = NULL;
   void       *x_left      = NULL;
   void       *x_right     = NULL;
   /*---(pre-defense)--------------------*/
   if (a_dist == 0)   return NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   DEBUG_YSORT   yLOG_complex ("args"      , "lvl %2d, pos %3d, dist %3d, dir %c, %p", a_lvl, a_pos, a_dist, a_dir, a_node);
   /*---(depth)--------------------------*/
   if (a_lvl + 1 > s_levels)  s_levels = a_lvl + 1;
   /*---(check new position)-------------*/
   x_curr = a_node;
   switch (a_dir) {
   case 'L' :
      x_pos = a_pos - a_dist;
      if (x_pos <= 0) {
         DEBUG_YSORT   yLOG_note    ("too far left, skip level");
         x_curr = ysort_tree__nextlevel (a_type, a_head, a_tail, a_count, a_lvl, a_pos, a_dist / 2, 'L', x_curr);
         DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
         return x_curr;
      }
      break;
   case 'R' :
      x_pos = a_pos + a_dist;
      if (x_pos > a_count) {
         DEBUG_YSORT   yLOG_note    ("too far right, skip level");
         x_curr = ysort_tree__nextlevel (a_type, a_head, a_tail, a_count, a_lvl, a_pos, a_dist / 2, 'R', x_curr);
         DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
         return x_curr;
      }
      break;
   default  :
      DEBUG_YSORT   yLOG_note    ("a_dir not LR, skipping");
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return NULL;
      break;
   }
   DEBUG_YSORT   yLOG_value   ("x_pos"     , x_pos);
   /*---(get to new node)----------------*/
   while (c < a_dist) {
      if (a_dir == 'L')   rc = g_cursor ('-', a_head, a_tail, x_curr, &x_curr, '<');
      if (a_dir == 'R')   rc = g_cursor ('-', a_head, a_tail, x_curr, &x_curr, '>');
      ++c;
   }
   /*---(recurse)------------------------*/
   DEBUG_YSORT   yLOG_complex ("a_btree"   , "%2d %c %4d %4d", a_lvl, a_dir, a_dist, x_pos);
   x_left  = ysort_tree__nextlevel (a_type, a_head, a_tail, a_count, a_lvl + 1, x_pos, a_dist / 2, 'L', x_curr);
   x_right = ysort_tree__nextlevel (a_type, a_head, a_tail, a_count, a_lvl + 1, x_pos, a_dist / 2, 'R', x_curr);
   DEBUG_YSORT   yLOG_complex ("found"     , "left %p, right %p", x_left, x_right);
   rc = g_forker (a_type, x_curr, &x_left, &x_right);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return x_curr;
}

char
ySORT_treeify           (uchar a_type, void *a_head, void *a_tail, int a_count, void **a_root)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_ctr       =    0;
   int         c           =    0;
   int         x_lvl       =    0;
   void       *x_curr      = NULL;
   void       *x_left      = NULL;
   void       *x_right     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ysort_defense  (YSORT_SEARCH, YSORT_NONE, a_head, a_tail);
   DEBUG_YSORT   yLOG_value   ("defense"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   s_type   = a_type;
   s_count  = a_count;
   s_levels = 0;
   /*---(statistics)---------------------*/
   x_lvl = ysort_tree__depth (a_count);
   DEBUG_YSORT   yLOG_value   ("est depth" , x_lvl);
   s_span  = ysort_tree__span (x_lvl);
   DEBUG_YSORT   yLOG_value   ("max span"  , s_span);
   s_filled = a_count / (float) s_span;
   DEBUG_YSORT   yLOG_double  ("est usage" , s_filled);
   x_ctr   = (a_count / 2) + 1;
   DEBUG_YSORT   yLOG_value   ("center"    , x_ctr);
   /*---(find root)----------------------*/
   rc = g_cursor (a_type, a_head, a_tail, NULL   , &x_curr, '[');
   c = 1;
   while (c < x_ctr) {
      rc = g_cursor (a_type, a_head, a_tail, x_curr, &x_curr, '>');
      ++c;
   }
   DEBUG_YSORT   yLOG_point   ("ROOT"      , x_curr);
   /*---(launch tree making)-------------*/
   x_left  = ysort_tree__nextlevel (a_type, a_head, a_tail, a_count, 1, x_ctr, s_span / 4 + 1, 'L', x_curr);
   x_right = ysort_tree__nextlevel (a_type, a_head, a_tail, a_count, 1, x_ctr, s_span / 4 + 1, 'R', x_curr);
   DEBUG_YSORT   yLOG_complex ("found"     , "left %p, right %p", x_left, x_right);
   rc = g_forker (a_type, x_curr, &x_left, &x_right);
   DEBUG_YSORT   yLOG_value   ("real depth", s_levels);
   /*---(save back)----------------------*/
   if (a_root != NULL)  *a_root = x_curr;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching the btree                   ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char
ysort_tree__searchdown       (uchar a_type, void *a_node, char *a_dir, void *a_key, void **a_found)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   void       *x_left      = NULL;
   void       *x_right     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   DEBUG_YSORT   yLOG_complex ("args"      , "%c, %p, dir %s", a_type, a_node, a_dir);
   DEBUG_YSORT   yLOG_info    ("a_dir"     , a_dir);
   /*---(defense)------------------------*/
   if (a_node == NULL)  {
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return -1;
   }
   /*---(prepare)------------------------*/
   ++s_depth;
   strlcat (s_path, a_dir, LEN_DESC);
   DEBUG_YSORT   yLOG_info    ("s_path"    , s_path);
   /*---(compare)------------------------*/
   rc = g_checker (a_type, 0, a_node, a_key, YSORT_SEARCH); 
   DEBUG_YSORT   yLOG_value   ("checker"   , rc);
   /*---(no match)-----------------------*/
   g_forker (a_type, a_node, &x_left, &x_right);
   DEBUG_YSORT   yLOG_complex ("retrieved" , "left %p, right %p", x_left, x_right);
   if (rc >  0) {
      rc = ysort_tree__searchdown (a_type, x_left , "L", a_key, a_found);
      DEBUG_YSORT   yLOG_value   ("left rc"   , rc);
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   if (rc <  0) {
      rc = ysort_tree__searchdown (a_type, x_right, "R", a_key, a_found);
      DEBUG_YSORT   yLOG_value   ("right rc"  , rc);
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(match)--------------------------*/
   *a_found = a_node;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysort_tree__notfound    (void)
{
   s_last   = NULL;
   s_result = -1;
   s_depth  = 0;
   strlcpy (s_path, "-", LEN_DESC);
}

char
ySORT_search            (uchar a_type, void *a_root, void *a_key, void **a_found)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_node      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   s_depth = 0;
   strlcpy (s_path, "", LEN_DESC);
   if (a_found != NULL)  *a_found = NULL;
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_point   ("a_root"    , a_root);
   --rce;  if (a_root == NULL)  {
      ysort_tree__notfound ();
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("a_key"     , a_key);
   --rce;  if (a_key  == NULL)  {
      ysort_tree__notfound ();
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(search)-------------------------*/
   DEBUG_YSORT   yLOG_note    ("dive into btree");
   rc = ysort_tree__searchdown (toupper (a_type), a_root, "@", a_key, &x_node);
   DEBUG_YSORT   yLOG_value   ("max depth" , s_levels);
   DEBUG_YSORT   yLOG_value   ("s_depth"   , s_depth);
   DEBUG_YSORT   yLOG_info    ("s_path"    , s_path);
   /*---(check)--------------------------*/
   --rce;  if (x_node == NULL) {
      DEBUG_YSORT   yLOG_note    ("not found");
      ysort_tree__notfound ();
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(return)-------------------------*/
   DEBUG_YSORT   yLOG_note    ("found");
   if (a_found != NULL)  *a_found = x_node;
   s_last   = x_node;
   s_result = 0;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         dumper                               ----===*/
/*====================------------------------------------====================*/
static void  o___DUMP____________o () { return; }

char
ysort_tree__walk      (uchar a_type, int a_lvl, void *a_node, char *a_path)
{
   /*---(locals)-----------+-----+-----+-*/
   void       *x_left      = NULL;
   void       *x_right     = NULL;
   char        x_path      [LEN_DESC]  = "";
   char        x_pre       [LEN_RECD]  = "";
   int         i           =    0;
   int         x_len       =    0;
   /*---(early defense)------------------*/
   if (a_node == NULL)  return 0;
   if (a_lvl > 20)      return 0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   DEBUG_YSORT   yLOG_info    ("a_path"    , a_path);
   /*---(get paths)----------------------*/
   g_forker (toupper (a_type), a_node, &x_left, &x_right);
   DEBUG_YSORT   yLOG_complex ("retrieved" , "left %p, right %p", x_left, x_right);
   /*---(go left)------------------------*/
   sprintf (x_path, "%sL", a_path, LEN_DESC);
   ysort_tree__walk  (a_type, a_lvl + 1, x_left, x_path);
   /*---(self)---------------------------*/
   strlcpy (x_pre, "", LEN_RECD);
   for (i =  0; i < a_lvl - 1; ++i)  strlcat (x_pre, "   ", LEN_RECD);
   strlcat (x_pre, "+--", LEN_RECD);
   for (i = 1; i < 20; ++i) {
      if      (strncmp (a_path + i, "LR", 2) == 0)  x_pre [3 * i] = '|';
      else if (strncmp (a_path + i, "RL", 2) == 0)  x_pre [3 * i] = '|';
   }
   s_callback        (a_type, a_lvl, a_node, a_path, x_pre);
   /*---(go right)-----------------------*/
   sprintf (x_path, "%sR", a_path, LEN_DESC);
   ysort_tree__walk  (a_type, a_lvl + 1, x_right, x_path);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySORT_walk            (uchar a_type, void *a_root, void *p_callback)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_point   ("a_root"    , a_root);
   --rce;  if (a_root   == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("p_callback", p_callback);
   --rce;  if (p_callback == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_callback = p_callback;
   /*---(start)--------------------------*/
   rc = ysort_tree__walk (a_type, 1, a_root, "@");
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySORT_treeify_stats     (int *a_count, int *a_span, int *a_levels, float *a_filled)
{
   if (a_count   != NULL)  *a_count   = s_count;
   if (a_span    != NULL)  *a_span    = s_span;
   if (a_levels  != NULL)  *a_levels  = s_levels;
   if (a_filled  != NULL)  *a_filled  = s_filled;
   return 0;
}

char
ySORT_search_stats      (int *a_result, void **a_last, int *a_depth, char *a_path)
{
   if (a_result  != NULL)  *a_result  = s_result;
   if (a_last    != NULL)  *a_last    = s_last;
   if (a_depth   != NULL)  *a_depth   = s_depth;
   if (a_path    != NULL)  strlcpy (a_path, s_path, LEN_DESC);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ysort_tree__unit        (char *a_question, int n)
{
   /*---(locals)-------------------------*/
   int         c           =    0;
   char        t           [LEN_LABEL] = "";
   tMOCK      *x_last      = NULL;
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "TREE             : label could not be parsed");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "result"        ) == 0) {
      if (s_last == NULL)   strcpy  (t, "(null)");
      else                  strlcpy (t, ((tMOCK *) s_last)->label, LEN_LABEL);
      snprintf (unit_answer, LEN_FULL, "TREE result      : %2d %-20.20s  %2d %s", s_result, t, s_depth, s_path);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



