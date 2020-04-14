/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"



static uchar    s_type   =  '-';
static void    *s_head   = NULL;
static void    *s_root   = NULL;
static void    *s_tail   = NULL;
static int      s_count  =    0;

static char     s_levels =    0;
static char     s_result =    0;
static char     s_path   [LEN_DESC]  = "";
static char     s_depth  =    0;
static llong    s_last   =   -1;



/*====================------------------------------------====================*/
/*===----                        little helpers                        ----===*/
/*====================------------------------------------====================*/
static void  o___HELPERS_________o () { return; }

int 
BTREE__depth            (int a_size)
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
BTREE__span             (int a_levels)
{
   if (a_levels <= 0)  return 0;
   return pow (2, a_levels) - 1;
}



/*====================------------------------------------====================*/
/*===----                        build the btree                       ----===*/
/*====================------------------------------------====================*/
static void  o___BUILD___________o () { return; }

void*
BTREE__nextlevel   (uchar a_type, void *a_head, void *a_tail, int a_count, int a_lvl, int a_pos, int a_dist, char a_dir, void *a_node)
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
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   DEBUG_SORT   yLOG_complex ("args"      , "lvl %2d, pos %3d, dist %3d, dir %c, %p", a_lvl, a_pos, a_dist, a_dir, a_node);
   /*---(depth)--------------------------*/
   if (a_lvl + 1 > s_levels)  s_levels = a_lvl + 1;
   /*---(check new position)-------------*/
   x_curr = a_node;
   switch (a_dir) {
   case 'L' :
      x_pos = a_pos - a_dist;
      if (x_pos <= 0) {
         DEBUG_SORT   yLOG_note    ("too far left, skip level");
         x_curr = BTREE__nextlevel (a_type, a_head, a_tail, a_count, a_lvl, a_pos, a_dist / 2, 'L', x_curr);
         DEBUG_SORT   yLOG_exit    (__FUNCTION__);
         return x_curr;
      }
      break;
   case 'R' :
      x_pos = a_pos + a_dist;
      if (x_pos > a_count) {
         DEBUG_SORT   yLOG_note    ("too far right, skip level");
         x_curr = BTREE__nextlevel (a_type, a_head, a_tail, a_count, a_lvl, a_pos, a_dist / 2, 'R', x_curr);
         DEBUG_SORT   yLOG_exit    (__FUNCTION__);
         return x_curr;
      }
      break;
   default  :
      DEBUG_SORT   yLOG_note    ("a_dir not LR, skipping");
      DEBUG_SORT   yLOG_exit    (__FUNCTION__);
      return NULL;
      break;
   }
   DEBUG_SORT   yLOG_value   ("x_pos"     , x_pos);
   /*---(get to new node)----------------*/
   while (c < a_dist) {
      if (a_dir == 'L')   rc = g_cursor ('-', a_head, a_tail, x_curr, &x_curr, '<');
      if (a_dir == 'R')   rc = g_cursor ('-', a_head, a_tail, x_curr, &x_curr, '>');
      ++c;
   }
   /*---(recurse)------------------------*/
   DEBUG_SORT   yLOG_complex ("a_btree"   , "%2d %c %4d %4d", a_lvl, a_dir, a_dist, x_pos);
   x_left  = BTREE__nextlevel (a_type, a_head, a_tail, a_count, a_lvl + 1, x_pos, a_dist / 2, 'L', x_curr);
   x_right = BTREE__nextlevel (a_type, a_head, a_tail, a_count, a_lvl + 1, x_pos, a_dist / 2, 'R', x_curr);
   DEBUG_SORT   yLOG_complex ("found"     , "left %p, right %p", x_left, x_right);
   rc = g_forker (a_type, x_curr, &x_left, &x_right);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return x_curr;
}

char
ySORT_btree             (uchar a_type, void *a_head, void *a_tail, int a_count, void **a_root)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_lvl       =    0;
   int         x_span      =    0;
   int         x_ctr       =    0;
   int         c           =    0;
   void       *x_curr      = NULL;
   void       *x_left      = NULL;
   void       *x_right     = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ySORT_defense  (YSORT_SEARCH, YSORT_NONE, a_head, a_tail);
   DEBUG_SORT   yLOG_value   ("defense"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   s_type  = a_type;
   s_head  = a_head;
   s_tail  = a_tail;
   s_count = a_count;
   /*---(statistics)---------------------*/
   x_lvl   = BTREE__depth (a_count);
   DEBUG_SORT   yLOG_value   ("est depth" , x_lvl);
   x_span  = BTREE__span (x_lvl);
   DEBUG_SORT   yLOG_value   ("max span"  , x_span);
   DEBUG_SORT   yLOG_double  ("est usage" , a_count / (float) x_span);
   x_ctr   = (a_count / 2) + 1;
   DEBUG_SORT   yLOG_value   ("center"    , x_ctr);
   /*---(find root)----------------------*/
   rc = g_cursor (a_type, a_head, a_tail, NULL   , &x_curr, '[');
   c = 1;
   while (c < x_ctr) {
      rc = g_cursor (a_type, a_head, a_tail, x_curr, &x_curr, '>');
      ++c;
   }
   DEBUG_SORT   yLOG_point   ("ROOT"      , x_curr);
   /*---(launch tree making)-------------*/
   x_left  = BTREE__nextlevel (a_type, a_head, a_tail, a_count, 1, x_ctr, x_span / 4 + 1, 'L', x_curr);
   x_right = BTREE__nextlevel (a_type, a_head, a_tail, a_count, 1, x_ctr, x_span / 4 + 1, 'R', x_curr);
   DEBUG_SORT   yLOG_complex ("found"     , "left %p, right %p", x_left, x_right);
   rc = g_forker (a_type, x_curr, &x_left, &x_right);
   DEBUG_SORT   yLOG_value   ("real depth", s_levels);
   /*---(save back)----------------------*/
   if (a_root != NULL)  *a_root = x_curr;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching the btree                   ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char
BTREE__searchdown       (uchar a_type, void *a_node, char *a_dir, void *a_key, void **a_found)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   void       *x_left      = NULL;
   void       *x_right     = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   DEBUG_SORT   yLOG_complex ("args"      , "%c, %p, dir %s", a_type, a_node, a_dir);
   DEBUG_SORT   yLOG_info    ("a_dir"     , a_dir);
   /*---(defense)------------------------*/
   if (a_node == NULL)  {
      DEBUG_SORT   yLOG_exit    (__FUNCTION__);
      return -1;
   }
   /*---(prepare)------------------------*/
   ++s_depth;
   strlcat (s_path, a_dir, LEN_DESC);
   DEBUG_SORT   yLOG_info    ("s_path"    , s_path);
   /*---(compare)------------------------*/
   rc = g_checker (a_type, 0, a_node, a_key, YSORT_SEARCH); 
   DEBUG_SORT   yLOG_value   ("checker"   , rc);
   /*---(no match)-----------------------*/
   g_forker (a_type, a_node, &x_left, &x_right);
   DEBUG_SORT   yLOG_complex ("retrieved" , "left %p, right %p", x_left, x_right);
   if (rc >  0) {
      rc = BTREE__searchdown (a_type, x_left , "L", a_key, a_found);
      DEBUG_SORT   yLOG_value   ("left rc"   , rc);
      DEBUG_SORT   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   if (rc <  0) {
      rc = BTREE__searchdown (a_type, x_right, "R", a_key, a_found);
      DEBUG_SORT   yLOG_value   ("right rc"  , rc);
      DEBUG_SORT   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(match)--------------------------*/
   *a_found = a_node;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySORT_search            (uchar a_type, void *a_root, void *a_key, void **a_found)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_node      = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   s_depth = 0;
   strlcpy (s_path, "", LEN_DESC);
   if (a_found != NULL)  *a_found = NULL;
   /*---(defense)------------------------*/
   --rce;  if (a_root == NULL)  {
      s_last   = NULL;
      s_result = -1;
      strlcpy (s_path, "-", LEN_DESC);
      DEBUG_SORT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (a_key  == NULL)  {
      s_last   = NULL;
      s_result = -1;
      strlcpy (s_path, "-", LEN_DESC);
      DEBUG_SORT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(search)-------------------------*/
   DEBUG_SORT   yLOG_note    ("dive into btree");
   rc = BTREE__searchdown (toupper (a_type), a_root, "@", a_key, &x_node);
   DEBUG_SORT   yLOG_value   ("max depth" , s_levels);
   DEBUG_SORT   yLOG_value   ("s_depth"   , s_depth);
   DEBUG_SORT   yLOG_info    ("s_path"    , s_path);
   /*---(check)--------------------------*/
   --rce;  if (x_node == NULL) {
      DEBUG_SORT   yLOG_note    ("not found");
      s_last   = a_key;
      s_result = -1;
      strlcpy (s_path, "-", LEN_DESC);
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(return)-------------------------*/
   DEBUG_SORT   yLOG_note    ("found");
   if (a_found != NULL)  *a_found = x_node;
   s_last   = a_key;
   s_result = 0;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   data structure reporting                   ----===*/
/*====================------------------------------------====================*/
static void  o___REPORTS_________o () { return; }

/*> char                                                                                                             <* 
 *> BTREE__display        (uchar a_type, int a_lvl, void *a_node)                                                    <* 
 *> {                                                                                                                <* 
 *>    char        x_pre       [LEN_RECD] = "";                                                                      <* 
 *>    int         i           =    0;                                                                               <* 
 *>    if (a_node == NULL)  return 0;                                                                                <* 
 *>    if (a_lvl > 20)      return 0;                                                                                <* 
 *>    BTREE__display      (a_lvl + 1, a_node->b_left, a_file);                                                      <* 
 *>    strlcpy (x_pre, "", LEN_RECD);                                                                                <* 
 *>    for (i =  0; i < a_lvl; ++i)  strcat (x_pre, "··+ ");                                                         <* 
 *>    fprintf (a_file, "%s %-10.10s", x_pre, a_node->label);                                                        <* 
 *>    DEBUG_SORT   yLOG_senter  (__FUNCTION__);                                                                     <* 
 *>    DEBUG_SORT   yLOG_snote   (__FUNCTION__);                                                                     <* 
 *>    fprintf (a_file, "%s %2dt, %3dc, %4dr, %20lld", x_pre, a_node->tab, a_node->col, a_node->row, a_node->key);   <* 
 *>    if      (a_node->key < 0)         fprintf (a_file, "   buffer/register");                                     <* 
 *>    else if (s_prev == a_node->key)   fprintf (a_file, "   DUP");                                                 <* 
 *>    else                              fprintf (a_file, "   -");                                                   <* 
 *>    DEBUG_SORT   yLOG_sexit   (__FUNCTION__);                                                                     <* 
 *>    s_prev = a_node->key;                                                                                         <* 
 *>    fprintf (a_file, "\n");                                                                                       <* 
 *>    BTREE__display      (a_lvl + 1, a_node->b_right, a_file);                                                     <* 
 *>    return 0;                                                                                                     <* 
 *> }                                                                                                                <*/

/*> char                                                                              <* 
 *> BTREE_list              (uchar a_type, void *a_root)                              <* 
 *> {                                                                                 <* 
 *>    BTREE__display (a_type, 0, a_root);                                            <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
BTREE__unit        (char *a_question, int n)
{
   /*---(locals)-------------------------*/
   int         c           =    0;
   char        t           [LEN_LABEL] = "";
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "BTREE            : label could not be parsed");
   /*---(overall)------------------------*/
   /*> if      (strcmp (a_question, "sorts"         ) == 0) {                                                                                  <* 
    *>    snprintf (unit_answer, LEN_FULL, "BTREE sorts      : %4d#, %4dl, %4dc, %4ds, %4dt", s_sorts, s_levels, s_comps, s_swaps, s_teles);   <* 
    *> }                                                                                                                                       <*/
   if      (strcmp (a_question, "result"        ) == 0) {
      snprintf (unit_answer, LEN_FULL, "BTREE result     : %2d %-20.20s  %2d %s", s_result, s_last, s_depth, s_path);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



