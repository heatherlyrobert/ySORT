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
 *  internally used sorts for btree to test theories...
 *
 *
 *  classic gnome > any, always, anywhere gnome
 *  -- mods  : any       any number of changes could be present
 *  -- walk  : plodding  can it skip around or walks every move
 *  -- swap  : always    swaps performed immediately after failed comparision
 *  -- adds  : anywhere  no assumpions on location of additions
 *  -- begin : head      start at the front of list
 *
 *
 *  1) CGNOME) any, plodding, always, anywhere, head gnome
 *       for when it must be dead-simple
 *
 *  2) TGNOME) any, teleporting, always, anywhere, head gnome
 *       ... cuts nearly half the comparisons
 *
 *  3) DGNOME) any, teleporting, delayed, anywhere, head gnome
 *       ... cuts nearly half the comparisons AND swaps
 *
 *  4) SGNOME) single, teleporting, delayed, tail, tail gnome
 *       special case, must search after every add gnome
 *
 */

/*
 *                    è/F                                     è/G                                  è/H                                 è/I                                    è/J                                    è/K                                       ô/b                                         ô/c                                         ô/d                    
 *            moderately out-of-order                tail add to sorted list               fully sorted list                     reverse sorted list                   totally random list                   random with dups                      fifty proper names                           hundred proper names                       three-hundred proper names         
 *            -----------------------------------    -----------------------------------   -----------------------------------   -----------------------------------   -----------------------------------   -----------------------------------   -----------------------------------------   -----------------------------------------   -----------------------------------------
 *            loop comp swap tele call usec  rpct    loop comp swap tele call usec  rpct   loop comp swap tele call usec  rpct   loop comp swap tele call usec  rpct   loop comp swap tele call usec  rpct   loop comp swap tele call usec  rpct    loop  comp  swap  tele  call  usec  rpct    loop  comp  swap  tele  call  usec  rpct    loop  comp  swap  tele  call  usec  rpct
 *
 *    cgnome   123  120   49    ´    ´    7  0.56      57   57   16    ´    ´    3  0.33     25   25    0    ´    ´    4  1.00    675  650  325    ´    ´   21  2.71    389  385  182    ´    ´   13  1.83    369  366  172    ´    ´   12  1.67    1233  1230   592     ´     ´    41  2.60    5367  5364  2634     ´     ´   154  3.46   43463 43458 21582     ´     ´  1236  3.67
 *
 *    tgnome    74  (71)  49   25    ´   12  1.11      41  (41)  16   25    ´    5  1.00     25   25    0   24    ´    5  1.50    350  325  325   25    ´   16  2.00    207  203  182   25    ´   11  1.50    197  194  172   25    ´   11  1.50     641   638   592    49     ´    32  2.00    2733  2730  2634    99     ´   109  2.43   21881 21876 21582   299     ´   943  2.80
 *
 *    dgnome    74   71  (24)  25    ´   11 (´´´´)     41   41   (1)  25    ´    5 (´´´´)    25   25    0   24    ´    4 (´´´´)   350  325   25   25    ´    9 (´´´´)   207  203   20   25    ´    8 (´´´´)   197  194   19   25    ´    8 (´´´´)    641   638    46    49     ´    17 (´´´´)   2733  2730    96    99     ´    46  ´´´´   21881 21876   296   299     ´   338  ´´´´
 *
 *    ugnome     "    "    "    "    ´   12     ´       "    "    "    "    ´    5     ´      "    "    "    "    ´    4     ´      "    "    "    "    ´   10     ´      "    "    "    "    ´    8     ´      "    "    "    "    ´    8     ´       "     "     "     "     ´    17     ´       "     "     "     "     ´    55     ´       "     "     "     "     ´   359     ´
 *
 *    sgnome   ---  ---  ---  ---  ---  ---   ---      17   17    1    ´    ´    6   ---    ---  ---  ---  ---  ---  ---   ---    ---  ---  ---  ---  ---  ---   ---    ---  ---  ---  ---  ---  ---   ---    ---  ---  ---  ---  ---  ---   ---     ---   ---   ---   ---   ---   ---   ---     ---   ---   ---   ---   ---   ---   ---     ---   ---   ---   ---   ---   ---   ---
 *
 *    bubble   676  650   24    ´    ´   21  2.11     676  650   16    ´    ´   13  3.67    676  650    0    ´    ´   13  5.50    676  650  325    ´    ´   23  3.00    676  650  182    ´    ´   22  3.33    676  650  172    ´    ´   20  3.00    2500  2450   592     ´     ´    67  4.33   10000  9900  2634     ´     ´   265  5.98   90000 89700 21582     ´     ´  2417  7.19
 *
 *    select   325  325   24    ´    ´   12  1.11     325  325    1    ´    ´    7  1.67    325  325    0    ´    ´    7  2.50    325  325   25    ´    ´    9  1.00    325  325   20    ´    ´   10  1.33    325  325   19    ´    ´    9  1.67    1225  1225    46     ´     ´    25  1.53    4950  4950    96     ´     ´    74  1.64   44850 44850   296     ´     ´   620  1.84
 *
 *    quick    146  146  141    ´   31   23  2.33     181  181  189    ´   49   21  6.33    325  325  350    ´   51   22 10.00    325  325   25    ´   51   14  1.71    110  110   68    ´   37   12  1.67    114  114   77    ´   37   13  1.83     365   365   147     ´    67    22  1.33     590   590   315     ´   139    35 (0.82)   2690  2690  1575     ´   401   136 (0.40)
 *
 *    troll     85   81   30   24    6   17  1.67      51   49    2   24    6   13  3.67     38   37    0   24    6    7  2.50    193  169   37   24    6   11  1.29    121  116   30   24    6   12  1.67    118  114   29   24    6   10  1.33     302   288    86    46    14    23  1.40     590   590   315     ´   139    35  0.91    3499  3429  1367   298    62   136 (0.40)
 *
 *    OVERHEAD                            2                                      2                                     2                                     2                                     2                                     2                                           2                                           2                                           2
 *
 *    at a high level, swapping is much more complex/dangerous, and generally expensive, than comparison.
 *    this is especially true when actually moving the objects themselves.  when these objects are in
 *    a list, most would say its easy to change pointers ;)  true, but only if using a singly-linked list
 *    rather that doubly-linked, trees, or graphs.  in a linked list, swapping any item with another is
 *    the same amount of work.
 *
 *    in my case, for list safety, i typically use double-linked lists with head/tail pointers.  swapping
 *    is much quicker that arrays, etc, but definately non-trivial.
 *
 *    at the same high level, until you are creating sort keys on-the-fly, the comparisions are trivial.
 *
 *    having said all this, i prefer simple design, and low and predictable swap counts.
 *
 *    secondly, i sort single-addition lists very frequently as i often (not always) search them for 
 *    duplicates before adding another.  since i regularly allow duplicates in other lists, i also
 *    prefer stable sorts to preserve order of secondary data.
 *
 *
 *
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
static  int      s_calls    = 0;
static  llong    s_dur      = 0;

static  int      s_SORTS    = 0;
static  int      s_LOOPS    = 0;
static  int      s_COMPS    = 0;
static  int      s_SWAPS    = 0;
static  int      s_TELES    = 0;
static  int      s_CALLS    = 0;

static  char     s_path      [LEN_TITLE] = "";


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
   s_loops = s_comps = s_swaps = s_teles = s_calls = 0;
   s_LOOPS = s_COMPS = s_SWAPS = s_TELES = s_CALLS = 0;
   s_SORTS = 0;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 1;
}

char
ysort_intern_prep       (tSORT *a_abbr, int *r_loops, int *r_comps, int *r_swaps, int *r_teles, int *r_calls)
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
   if (r_calls != NULL)  *r_calls = 0;
   /*---(initialize)---------------------*/
   s_abbr  = a_abbr;
   s_loops = s_comps = s_swaps = s_teles = s_calls = 0;
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
ysort_intern_done       (int *r_loops, int *r_comps, int *r_swaps, int *r_teles, int *r_calls)
{
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(display)------------------------*/
   DEBUG_YSORT   yLOG_value   ("size"       , s_count);
   DEBUG_YSORT   yLOG_value   ("loops"      , s_loops);
   DEBUG_YSORT   yLOG_value   ("compares"   , s_comps);
   DEBUG_YSORT   yLOG_value   ("swaps"      , s_swaps);
   DEBUG_YSORT   yLOG_value   ("teleports"  , s_teles);
   DEBUG_YSORT   yLOG_value   ("func-calls" , s_calls);
   /*---(accumulate)---------------------*/
   ++s_SORTS;
   s_LOOPS += s_loops;
   s_COMPS += s_comps;
   s_SWAPS += s_swaps;
   s_TELES += s_teles;
   s_CALLS += s_calls;
   /*---(save-back)----------------------*/
   if (r_loops != NULL)  *r_loops = s_loops;
   if (r_comps != NULL)  *r_comps = s_comps;
   if (r_swaps != NULL)  *r_swaps = s_swaps;
   if (r_teles != NULL)  *r_teles = s_teles;
   if (r_calls != NULL)  *r_calls = s_calls;
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
   /*---(save-back)----------------------*/
   ++s_swaps;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort__intern_bubble    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tSORT      *h           = NULL;          /* head of each wave              */
   tSORT      *o           = NULL;          /* origin entry                   */
   tSORT      *n           = NULL;          /* main index                     */
   int         x_match     =    0;
   char        x_flag      =  '-';
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   h = *s_head;
   DEBUG_YSORT   yLOG_info    ("h"         , h->sort);
   /*---(walk list)----------------------*/
   for (i = 0; i < s_count; ++i) {
      DEBUG_YSORT   yLOG_note    (ySORT_btree_list ('u'));
      o = *s_head;
      DEBUG_YSORT   yLOG_info    ("o"         , o->sort);
      while (o != NULL) {
         ++s_loops;
         /*---(check towards end)--------*/
         n = o->next;
         if (n == NULL) {
            DEBUG_YSORT   yLOG_note    ("hit the end, bounce");
            break;
         }
         /*---(compare)------------------*/
         ++s_comps;
         x_match = strcmp (o->sort, n->sort);
         x_flag  = (x_match <= 0) ? '<' : '>';
         DEBUG_YSORT   yLOG_complex ("compare"   , "(%4d) %-20.20s   %-20.20s v %-20.20s   %c %4d   %4d %4d %4d", s_loops, h->sort, o->sort, n->sort, x_flag, x_match, s_comps, s_swaps, s_teles);
         /*---(swap)------------------------*/
         if (x_flag == '>') {
            DEBUG_YSORT   yLOG_note    ("swap");
            ysort_intern_swap (o, n);
         }
         /*---(next)---------------------*/
         else {
            o = o->next;
         }
         /*---(done)---------------------*/
      }
      /*---(next)------------------------*/
      /*> h = h->next;                                                                <*/
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort__intern_select    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tSORT      *h           = NULL;          /* head of each wave              */
   tSORT      *o           = NULL;          /* origin entry                   */
   tSORT      *m           = NULL;          /* min entry                      */
   int         x_match     =    0;
   char        x_flag      =  '-';
   tSORT      *x_min       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   h = *s_head;
   DEBUG_YSORT   yLOG_info    ("h"         , h->sort);
   /*---(walk list)----------------------*/
   while (h != NULL && h->next != NULL) {
      DEBUG_YSORT   yLOG_note    (ySORT_btree_list ('u'));
      m = h;
      DEBUG_YSORT   yLOG_info    ("m"         , m->sort);
      o = m->next;
      DEBUG_YSORT   yLOG_info    ("o"         , o->sort);
      while (o != NULL) {
         ++s_loops;
         /*---(compare)------------------*/
         ++s_comps;
         x_match = strcmp (m->sort, o->sort);
         x_flag  = (x_match <= 0) ? '<' : '>';
         DEBUG_YSORT   yLOG_complex ("compare"   , "(%4d) %-20.20s   %-20.20s v %-20.20s   %c %4d   %4d %4d %4d", s_loops, h->sort, m->sort, o->sort, x_flag, x_match, s_comps, s_swaps, s_teles);
         /*---(save min)--------------------*/
         if (x_flag == '>') {
            DEBUG_YSORT   yLOG_note    ("new min");
            m = o;
         }
         /*---(next)---------------------*/
         o = o->next;
         /*---(done)---------------------*/
      }
      /*---(move min)--------------------*/
      if (m != h) {
         DEBUG_YSORT   yLOG_note    ("swap");
         ysort_intern_swap (h, m);
         h = m;
      }
      /*---(next)------------------------*/
      h = h->next;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort__intern_check     (char a_dir, char a_path [LEN_TITLE], int a_lvl, int a_max, tSORT **a_beg, tSORT **a_end, int a_slots, char r_path [LEN_TITLE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_path      [LEN_TITLE] = "";
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(statistic)----------------------*/
   ++s_calls;
   /*---(check depth)--------------------*/
   DEBUG_YSORT   yLOG_value   ("a_lvl"     , a_lvl);
   DEBUG_YSORT   yLOG_value   ("a_max"     , a_max);
   if (a_lvl >= a_max) {
      DEBUG_YSORT   yLOG_note    ("hit max recursion");
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(defense on ends)----------------*/
   DEBUG_YSORT   yLOG_point   ("a_beg"     , a_beg);
   --rce;  if (a_beg  == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("*a_beg"    , *a_beg);
   --rce;  if (*a_beg == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_info    ("*a_beg"    , (*a_beg)->sort);
   DEBUG_YSORT   yLOG_point   ("a_end"     , a_end);
   --rce;  if (a_end  == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_point   ("*a_end"    , *a_end);
   --rce;  if (*a_end == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_info    ("*a_end"    , (*a_end)->sort);
   /*---(slots)--------------------------*/
   DEBUG_YSORT   yLOG_value   ("a_slots"   , a_slots);
   --rce;  if (a_slots < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update path)--------------------*/
   DEBUG_YSORT   yLOG_point   ("a_path"    , a_path);
   --rce;  if (a_path == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   snprintf (x_path, LEN_TITLE, "%s%c", a_path, a_dir);
   DEBUG_YSORT   yLOG_info    ("x_path"    , x_path);
   /*---(save-back)----------------------*/
   if (r_path  != NULL)  strlcpy (r_path, x_path, LEN_TITLE);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort__intern_quicksub  (char a_dir, char a_path [LEN_TITLE], int a_lvl, int a_max, tSORT *a_beg, tSORT *a_end)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT      *p           = NULL;          /* pivot entry                    */
   tSORT      *c           = NULL;          /* current entry                  */
   tSORT      *n           = NULL;          /* next entry                     */
   int         x_match     =    0;
   char        x_flag      =  '-';
   tSORT      *x_beg       = NULL;
   tSORT      *x_end       = NULL;
   char        x_path      [LEN_TITLE] = "";
   tSORT      *x_max       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(statistic)----------------------*/
   ++s_calls;
   /*---(defense)------------------------*/
   rc = ysort__intern_check   (a_dir, a_path, a_lvl, a_max, &a_beg, &a_end, 1, x_path);
   DEBUG_YSORT   yLOG_value   ("div_chk"   , rc);
   if (rc <= 0) {
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(start at head)------------------*/
   p = a_end;
   c = a_beg;
   x_max = p->prev;
   DEBUG_YSORT   yLOG_complex ("ends"      , "beg=%-20.20s end=%-20.20s max=%-20.20s", (x_beg == NULL) ? "(null)" : x_beg->sort, (x_end == NULL) ? "(null)" : x_end->sort, (x_max == NULL) ? "(null)" : x_max->sort);
   /*---(prepare)------------------------*/
   ysort_intern_swap (c, p);
   c = p->next;
   /*---(walk the elements)--------------*/
   while (c != NULL) {
      /*---(beginning)-------------------*/
      DEBUG_YSORT   yLOG_note    (ySORT_btree_list ('u'));
      ++s_loops;
      n = c->next;
      /*---(compare)---------------------*/
      ++s_comps;
      x_match = strcmp (c->sort, p->sort);
      x_flag  = (x_match <= 0) ? '<' : '>';
      DEBUG_YSORT   yLOG_complex ("compare"   , "(%4d) %-20.20s v %-20.20s   %c %4d   %4d %4d %4d", s_loops, c->sort, p->sort, x_flag, x_match, s_comps, s_swaps, s_teles);
      /*---(swap)------------------------*/
      if (x_flag == '<') {
         DEBUG_YSORT   yLOG_note    ("swap");
         ysort_intern_swap (p, c);
         if (x_beg == NULL) {
            x_beg = c;
         }
      } else {
         x_end = c;
      }
      DEBUG_YSORT   yLOG_complex ("ends"      , "beg=%-20.20s end=%-20.20s max=%-20.20s", (x_beg == NULL) ? "(null)" : x_beg->sort, (x_end == NULL) ? "(null)" : x_end->sort, (x_max == NULL) ? "(null)" : x_max->sort);
      /*---(next)---------------------*/
      if (c == x_max)  break;
      c = n;
      /*---(done)---------------------*/
   }
   DEBUG_YSORT   yLOG_info    ("x_beg"     , (x_beg == NULL) ? "(null)" : x_beg->sort);
   DEBUG_YSORT   yLOG_info    ("p->prev"   , (p->prev == NULL) ? "(null)" : p->prev->sort);
   DEBUG_YSORT   yLOG_info    ("p"         , (p     == NULL) ? "(null)" : p->sort);
   DEBUG_YSORT   yLOG_info    ("p->next"   , (p->next == NULL) ? "(null)" : p->next->sort);
   DEBUG_YSORT   yLOG_info    ("x_end"     , (x_end == NULL) ? "(null)" : x_end->sort);
   /*---(tail recurse)-------------------*/
   ysort__intern_quicksub ('L', x_path, a_lvl + 1, a_max, x_beg, p->prev);
   ysort__intern_quicksub ('R', x_path, a_lvl + 1, a_max, p->next, x_end);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort__intern_quick     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(call initial)-------------------*/
   strcpy (s_path, "");
   rc = ysort__intern_quicksub ('*', s_path, 0, 50, *s_head, *s_tail);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysort__intern_unzip     (char c_cutoff, tSORT *a_beg, tSORT **r_mid1, tSORT **r_mid2, tSORT *a_end, int a_slots, int *r_cnt1, int *r_cnt2)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(get sub-counts)-----------------*/
   DEBUG_YSORT   yLOG_value   ("c_cutoff"  , c_cutoff);
   DEBUG_YSORT   yLOG_value   ("a_slots"   , a_slots);
   if (a_slots < c_cutoff) {
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_YSORT   yLOG_point   ("r_cnt1"    , r_cnt1);
   --rce;  if (r_cnt1 == NULL)  {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *r_cnt1 = a_slots / 2;
   DEBUG_YSORT   yLOG_value   ("*r_cnt1"   , *r_cnt1);
   DEBUG_YSORT   yLOG_point   ("r_cnt2"    , r_cnt2);
   --rce;  if (r_cnt2 == NULL)  {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *r_cnt2 = a_slots - *r_cnt1;
   DEBUG_YSORT   yLOG_value   ("*r_cnt2"   , *r_cnt2);
   /*---(get mid-points)-----------------*/
   DEBUG_YSORT   yLOG_point   ("r_mid1"    , r_mid1);
   --rce;  if (r_mid1 == NULL)  {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *r_mid1 = a_beg;
   DEBUG_YSORT   yLOG_point   ("*r_mid1"   , *r_mid1);
   for (i = 1; i < *r_cnt1; ++i)   *r_mid1 = (*r_mid1)->next;
   DEBUG_YSORT   yLOG_point   ("r_mid2"    , r_mid2);
   --rce;  if (r_mid2 == NULL)  {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *r_mid2 = (*r_mid1)->next;
   DEBUG_YSORT   yLOG_point   ("*r_mid2"   , *r_mid2);
   /*---(display)------------------------*/
   DEBUG_YSORT   yLOG_info    ("a_beg"     , (a_beg     == NULL) ? "(null)" : a_beg->sort);
   DEBUG_YSORT   yLOG_info    ("*r_mid1"   , ((*r_mid1) == NULL) ? "(null)" : (*r_mid1)->sort);
   DEBUG_YSORT   yLOG_info    ("*r_mid2"   , ((*r_mid2) == NULL) ? "(null)" : (*r_mid2)->sort);
   DEBUG_YSORT   yLOG_info    ("a_end"     , (a_end     == NULL) ? "(null)" : a_end->sort);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort__intern_zip       (tSORT *a_beg, tSORT *a_mid1, tSORT *a_mid2, tSORT *a_end)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT      *l           = NULL;
   tSORT      *r           = NULL;
   tSORT      *n           = NULL;
   int         x_match     =    0;
   char        x_flag      =  '-';
   tSORT      *x_lend      = NULL;
   tSORT      *x_rend      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   l = a_beg;
   r = a_mid2;
   x_rend = a_end->next;
   /*---(zip-up)-------------------------*/
   while (l != NULL && r != NULL) {
      ++s_loops;
      n = r->next;
      /*---(check ends)------------------*/
      if (l == a_mid1->next) {
         DEBUG_YSORT   yLOG_note    ("hit left end, done");
         break;
      }
      if (n == x_rend) {
         DEBUG_YSORT   yLOG_note    ("hit right end, done");
         break;
      }
      /*---(compare)---------------------*/
      ++s_comps;
      x_match = strcmp (l->sort, r->sort);
      x_flag  = (x_match <= 0) ? '<' : '>';
      DEBUG_YSORT   yLOG_complex ("compare"   , "(%4d) %-20.20s v %-20.20s   %c %4d   %4d %4d %4d   %-20.20s", s_loops, l->sort, r->sort, x_flag, x_match, s_comps, s_swaps, s_teles, (n == NULL) ? "(null)" : n->sort);
      /*---(swap)------------------------*/
      if (x_flag == '<') {
         DEBUG_YSORT   yLOG_note    ("all good, move left forward");
         l = l->next;
      } else {
         DEBUG_YSORT   yLOG_note    ("swap, and move right forward");
         x_rend = r->next;
         ysort_intern_swap (l, r);
         r = n;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort__intern_troller   (char c_cutoff, char a_dir, char a_path [LEN_TITLE], int a_lvl, int a_max, tSORT **b_beg, tSORT **b_end, int a_slots)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_div       =    0;
   char        x_path      [LEN_TITLE] = "";
   tSORT      *x_beg       = NULL;
   int         x_cnt1      =    0;
   tSORT      *x_mid1      = NULL;
   int         x_cnt2      =    0;
   tSORT      *x_mid2      = NULL;
   tSORT      *x_end       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(statistic)----------------------*/
   ++s_calls;
   /*---(defense)------------------------*/
   rc = ysort__intern_check     (a_dir, a_path, a_lvl, a_max, b_beg, b_end, a_slots, x_path);
   DEBUG_YSORT   yLOG_value   ("check"     , rc);
   --rce;  if (rc <  0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unzip/divide)-------------------*/
   x_div = ysort__intern_unzip     (c_cutoff, *b_beg, &x_mid1, &x_mid2, *b_end, a_slots, &x_cnt1, &x_cnt2);
   DEBUG_YSORT   yLOG_value   ("unzip"     , x_div);
   --rce;  if (x_div < 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(sort small group)---------------*/
   --rce;  if (x_div == 0) {
      rc = ysort__intern_ugnome    (b_beg, b_end);
      DEBUG_YSORT   yLOG_value   ("ugnome"    , rc);
      if (rc < 0) {
         DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(recurse to sort)----------------*/
   --rce;  if (x_div == 1) {
      rc = ysort__intern_troller  (c_cutoff, 'L', x_path, a_lvl + 1, a_max, b_beg , &x_mid1, x_cnt1);
      DEBUG_YSORT   yLOG_value   ("left"      , rc);
      if (rc < 0) {
         DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   --rce;  if (x_div == 1) {
      rc = ysort__intern_troller  (c_cutoff, 'R', x_path, a_lvl + 1, a_max, &x_mid2, b_end , x_cnt2);
      DEBUG_YSORT   yLOG_value   ("right"     , rc);
      if (rc < 0) {
         DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(zip/merge)----------------------*/
   --rce;  if (x_div == 1) {
      DEBUG_YSORT   yLOG_complex ("division"  , "b=%-12.12s  m=%-12.12s     m=%-12.12s  e=%-12.12s", (*b_beg == NULL) ? "(null)" : (*b_beg)->sort, (x_mid1 == NULL) ? "(null)" : x_mid1->sort, (x_mid2 == NULL) ? "(null)" : x_mid2->sort, (*b_end == NULL) ? "(null)" : (*b_end)->sort);
      rc    = ysort__intern_zip       (*b_beg, x_mid1, x_mid2, *b_end);
      DEBUG_YSORT   yLOG_value   ("zip"       , rc);
      if (rc < 0) {
         DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort__intern_troll     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(call initial)-------------------*/
   strcpy (s_path, "");
   rc = ysort__intern_troller  (20, '*', s_path, 0, 50, s_head, s_tail, s_count);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return rc;
}


/*
 *  classic gnome > any, always, anywhere gnome
 *  -- mods  : any       any number of additions could be present
 *  -- swap  : always    swaps performed immediately after failed comparision
 *  -- adds  : anywhere  no assumpions on additions, so it starts at the head
 *
 */

char
ysort__intern_cgnome    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tSORT      *p           = NULL;          /* main index                     */
   tSORT      *o           = NULL;          /* origin entry                   */
   tSORT      *n           = NULL;          /* next entry (helps after swap   */
   int         x_match     =    0;
   char        x_flag      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(start at head)------------------*/
   o = (*s_head)->next;
   /*---(do the gnome walk)--------------*/
   while (o != NULL) {
      /*---(beginning)-------------------*/
      ++s_loops;
      p = o->prev;
      n = o->next;
      /*---(check bounce)----------------*/
      if (p == NULL) {
         DEBUG_YSORT   yLOG_note    ("bounce off beginning");
         o = n;
         continue;
      }
      /*---(compare)---------------------*/
      ++s_comps;
      x_match = strcmp (p->sort, o->sort);
      x_flag  = (x_match <= 0) ? '<' : '>';
      DEBUG_YSORT   yLOG_complex ("compare"   , "(%4d) %-20.20s v %-20.20s   %c %4d   %4d %4d %4d   n=%-20.20s", s_loops, p->sort, o->sort, x_flag, x_match, s_comps, s_swaps, s_teles, (n == NULL) ? "(null)" : n->sort);
      /*---(handle good)-----------------*/
      if (x_flag == '<') {
         if (o->next == NULL) {
            DEBUG_YSORT   yLOG_note    ("hit end, all done");
            break;
         }
         DEBUG_YSORT   yLOG_note    ("all good, move forward");
         o = n;
      }
      /*---(handle good)-----------------*/
      else {
         DEBUG_YSORT   yLOG_note    ("swap and move backward");
         ysort_intern_swap (p, o);
      }
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort__intern_tgnome    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT      *p           = NULL;          /* main entry                     */
   tSORT      *o           = NULL;          /* origin entry                   */
   tSORT      *t           = NULL;          /* teleport entry                 */
   int         x_match     =    0;
   char        x_flag      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(start at head)------------------*/
   o = (*s_head)->next;
   t = o->next;
   /*---(do the gnome walk)--------------*/
   while (o != NULL) {
      /*---(beginning)-------------------*/
      ++s_loops;
      p = o->prev;
      /*---(teleport)--------------------*/
      if (p == NULL) {
         DEBUG_YSORT   yLOG_note    ("hit head, teleport back to next run");
         ++s_teles;
         o = t;
         if (o != NULL)  t = o->next;
         continue;
      }
      /*---(compare)---------------------*/
      ++s_comps;
      x_match = strcmp (p->sort, o->sort);
      x_flag  = (x_match <= 0) ? '<' : '>';
      DEBUG_YSORT   yLOG_complex ("compare"   , "(%4d) %-20.20s v %-20.20s   %c %4d   %4d %4d %4d   t=%-20.20s", s_loops, p->sort, o->sort, x_flag, x_match, s_comps, s_swaps, s_teles, (t == NULL) ? "(null)" : t->sort);
      /*---(handle good)-----------------*/
      if (x_flag == '<') {
         if (o->next == NULL) {
            DEBUG_YSORT   yLOG_note    ("hit end, all done");
            break;
         }
         DEBUG_YSORT   yLOG_note    ("good match, teleport back to next run");
         ++s_teles;
         o = t;
         if (o != NULL)  t = o->next;
      }
      /*---(handle good)-----------------*/
      else {
         DEBUG_YSORT   yLOG_note    ("swap and move backward");
         ysort_intern_swap (p, o);
      }
      /*---(done)------------------------*/
   }
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
ysort__intern_dgnome    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT      *p           = NULL;          /* main entry                     */
   tSORT      *o           = NULL;          /* origin entry                   */
   tSORT      *c           = NULL;          /* current target                 */
   tSORT      *t           = NULL;          /* teleport entry                 */
   int         x_match     =    0;
   char        x_flag      =  '-';
   char        x_swap      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(start at head)------------------*/
   o = (*s_head)->next;
   c = o;
   t = o->next;
   /*---(do the gnome walk)--------------*/
   while (o != NULL) {
      /*---(beginning)-------------------*/
      DEBUG_YSORT   yLOG_note    (ySORT_btree_list ('u'));
      ++s_loops;
      p = o->prev;
      DEBUG_YSORT   yLOG_complex ("current"   , "(%4d) p=%-20.20s vs o=%-20.20s   %c   c=%-20.20s   t=%-20.20s", s_loops, (p == NULL) ? "(null)" : p->sort, (o == NULL) ? "(null)" : o->sort, x_swap, (c == NULL) ? "(null)" : c->sort, (t == NULL) ? "(null)" : t->sort);
      /*---(teleport)--------------------*/
      if (p == NULL) {
         DEBUG_YSORT   yLOG_note    ("hit head, swap if necessary, teleport back to next run");
         if (x_swap == 'y') {
            ysort_intern_swap (o, c);
            x_swap = '-';
         }
         ++s_teles;
         c = o = t;
         if (t != NULL) t = t->next;
         else           t = NULL;;
         continue;
      }
      /*---(compare)---------------------*/
      ++s_comps;
      x_match = strcmp (p->sort, c->sort);
      x_flag  = (x_match <= 0) ? '<' : '>';
      DEBUG_YSORT   yLOG_complex ("compare"   , "%c %4d   %4d  %4d  %4d", x_flag, x_match, s_comps, s_swaps, s_teles);
      /*---(handle good)-----------------*/
      if (x_flag == '<') {
         if (o->next == NULL) {
            DEBUG_YSORT   yLOG_note    ("hit end, all done");
            break;
         }
         DEBUG_YSORT   yLOG_note    ("good match, swap if necessary, teleport back to next run");
         if (x_swap == 'y') {
            ysort_intern_swap (o, c);
            x_swap = '-';
         }
         ++s_teles;
         c = o = t;
         if (t != NULL) t = t->next;
         else           t = NULL;
      }
      /*---(handle good)-----------------*/
      else {
         DEBUG_YSORT   yLOG_note    ("mark swap and move backward");
         x_swap = 'y';
         o = p;
      }
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char         /*-> used within merge-style sorts ------------------------------*/
ysort__intern_ugnome    (tSORT **b_beg, tSORT **b_end)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT      *p           = NULL;          /* main entry                     */
   tSORT      *o           = NULL;          /* origin entry                   */
   tSORT      *c           = NULL;          /* current target                 */
   tSORT      *t           = NULL;          /* teleport entry                 */
   int         x_match     =    0;
   char        x_flag      =  '-';
   char        x_swap      =  '-';
   tSORT      *x_beg       = NULL;
   tSORT      *x_end       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(save positions)-----------------*/
   x_beg = *b_beg;
   DEBUG_YSORT   yLOG_point   ("x_beg"     , x_beg);
   x_end = *b_end;
   DEBUG_YSORT   yLOG_point   ("x_end"     , x_end);
   /*---(start at head)------------------*/
   o = x_beg->next;
   DEBUG_YSORT   yLOG_point   ("o"         , o);
   c = o;
   DEBUG_YSORT   yLOG_point   ("c"         , c);
   t = o->next;
   DEBUG_YSORT   yLOG_point   ("t"         , t);
   /*---(do the gnome walk)--------------*/
   while (o != NULL) {
      /*---(beginning)-------------------*/
      DEBUG_YSORT   yLOG_note    (ySORT_btree_list ('u'));
      ++s_loops;
      p = o->prev;
      DEBUG_YSORT   yLOG_complex ("current"   , "(%d) p=%-12.12s  o=%-12.12s  %c  å  c=%-12.12s  æ  t=%-12.12s  b=%-12.12s  e=%-12.12s", s_loops, (p == NULL) ? "(null)" : p->sort, (o == NULL) ? "(null)" : o->sort, x_swap, (c == NULL) ? "(null)" : c->sort, (t == NULL) ? "(null)" : t->sort, (x_beg == NULL) ? "(null)" : x_beg->sort, (x_end == NULL) ? "(null)" : x_end->sort);
      /*---(teleport)--------------------*/
      if (o == x_beg) {
         DEBUG_YSORT   yLOG_note    ("hit head, swap if necessary, teleport back to next run");
         if (x_swap == 'y') {
            if (c == x_end)  x_end = c->prev;
            ysort_intern_swap (o, c);
            x_beg = c;
            DEBUG_YSORT   yLOG_info    ("x_beg"     , x_beg->sort);
            x_swap = '-';
         }
         ++s_teles;
         c = o = t;
         if (t != NULL) t = o->next;
         else           t = NULL;;
         if (c == x_end->next) {
            DEBUG_YSORT   yLOG_note    ("hit end, all done");
            break;
         }
         continue;
      }
      /*---(compare)---------------------*/
      ++s_comps;
      x_match = strcmp (p->sort, c->sort);
      x_flag  = (x_match <= 0) ? '<' : '>';
      DEBUG_YSORT   yLOG_complex ("compare"   , "%c %4d   %4d  %4d  %4d", x_flag, x_match, s_comps, s_swaps, s_teles);
      /*---(handle good)-----------------*/
      if (x_flag == '<') {
         DEBUG_YSORT   yLOG_note    ("good match, swap if necessary, teleport back to next run");
         if (x_swap == 'y') {
            if (c == x_end)  x_end = c->prev;
            ysort_intern_swap (o, c);
            DEBUG_YSORT   yLOG_info    ("x_end"     , x_end->sort);
            x_swap = '-';
         }
         ++s_teles;
         c = o = t;
         if (t != NULL) t = o->next;
         else           t = NULL;
         if (c == x_end->next) {
            DEBUG_YSORT   yLOG_note    ("hit end, all done");
            break;
         }
      }
      /*---(handle good)-----------------*/
      else {
         DEBUG_YSORT   yLOG_note    ("mark swap and move backward");
         x_swap = 'y';
         o = p;
      }
      /*---(done)------------------------*/
   }
   /*---(save-back)----------------------*/
   if (b_beg  != NULL)  *b_beg = x_beg;
   if (b_end  != NULL)  *b_end = x_end;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

/*
 *  single, delayed, tail gnome
 *  -- single  means sorts after every single addition (no group additions)
 *  -- delayed means swaps are only performed after right position found
 *  -- tail    means additions are always added to the tail
 *
 */

char
ysort__intern_sgnome    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSORT      *p           = NULL;          /* main entry                     */
   tSORT      *o           = NULL;          /* origin entry                   */
   tSORT      *c           = NULL;          /* current target                 */
   int         x_match     =    0;
   char        x_flag      =  '-';
   char        x_swap      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(start at head)------------------*/
   o = *s_tail;
   c = o;
   /*---(do the gnome walk)--------------*/
   while (o != NULL) {
      /*---(beginning)-------------------*/
      DEBUG_YSORT   yLOG_note    (ySORT_btree_list ('u'));
      ++s_loops;
      p = o->prev;
      DEBUG_YSORT   yLOG_complex ("current"   , "(%4d) p=%-20.20s vs o=%-20.20s   %c   c=%-20.20s", s_loops, (p == NULL) ? "(null)" : p->sort, (o == NULL) ? "(null)" : o->sort, x_swap, (c == NULL) ? "(null)" : c->sort);
      /*---(teleport)--------------------*/
      if (p == NULL) {
         DEBUG_YSORT   yLOG_note    ("hit head, swap if necessary, done");
         if (x_swap == 'y') {
            ysort_intern_swap (o, c);
            x_swap = '-';
         }
         break;
      }
      /*---(compare)---------------------*/
      ++s_comps;
      x_match = strcmp (p->sort, c->sort);
      x_flag  = (x_match <= 0) ? '<' : '>';
      DEBUG_YSORT   yLOG_complex ("compare"   , "%c %4d   %4d  %4d  %4d", x_flag, x_match, s_comps, s_swaps, s_teles);
      /*---(handle good)-----------------*/
      if (x_flag == '<') {
         if (o->next == NULL) {
            DEBUG_YSORT   yLOG_note    ("hit end, all done");
            break;
         }
         DEBUG_YSORT   yLOG_note    ("good match, swap if necessary, done");
         if (x_swap == 'y') {
            ysort_intern_swap (o, c);
            x_swap = '-';
         }
         break;
      }
      /*---(handle good)-----------------*/
      else {
         DEBUG_YSORT   yLOG_note    ("mark swap and move backward");
         x_swap = 'y';
         o = p;
      }
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysort_intern            (char a_type, char a_abbr, int *r_loops, int *r_comps, int *r_swaps, int *r_teles, int *r_calls, llong *r_dur)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        rc_final    =    0;
   char       *x_head      = NULL;
   char       *x_tail      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(start clock)--------------------*/
   ysort__intern_beg ();
   /*---(preparation)--------------------*/
   rc = ysort_intern_prep (a_abbr, r_loops, r_comps, r_swaps, r_teles, r_calls);
   DEBUG_YSORT   yLOG_value   ("prep"      , rc);
   --rce;  if (rc <= 0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   x_head = *s_head;
   x_tail = *s_tail;
   /*---(launch gnome type)--------------*/
   DEBUG_YSORT   yLOG_char    ("a_type"    , a_type);
   --rce;  switch (a_type) {
   case YSORT_CGNOME    :
      rc_final = ysort__intern_cgnome   ();
      break;
   case YSORT_TGNOME    :
      rc_final = ysort__intern_tgnome   ();
      break;
   case YSORT_DGNOME    :
      rc_final = ysort__intern_dgnome   ();
      break;
   case YSORT_SGNOME    :
      rc_final = ysort__intern_sgnome   ();
      break;
   case YSORT_UGNOME    :
      rc_final = ysort__intern_ugnome   (&x_head, &x_tail);
      break;
   case YSORT_BUBBLE    :
      rc_final = ysort__intern_bubble   ();
      break;
   case YSORT_SELECT    :
      rc_final = ysort__intern_select   ();
      break;
   case YSORT_QUICK     :
      rc_final = ysort__intern_quick    ();
      break;
   case YSORT_TROLL     :
      rc_final = ysort__intern_troll    ();
      break;
   case '-'             :
      /* just to check overhead */
      break;
   default              :
      DEBUG_YSORT   yLOG_note    ("unknown sort type");
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rc);
      return rce;
   }
   DEBUG_YSORT   yLOG_value   ("rc_final"  , rc_final);
   /*---(statistics)---------------------*/
   rc = ysort_intern_done (r_loops, r_comps, r_swaps, r_teles, r_calls);
   DEBUG_YSORT   yLOG_value   ("done"      , rc);
   --rce;  if (rc <  0) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(end clock)----------------------*/
   s_dur = ysort__intern_end ();
   if (r_dur  != NULL)  *r_dur = s_dur;
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return rc_final;
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

llong        /*-> duration in microeconds ------------------------------------*/
ysort__intern_dur       (char a_type)
{
   /*---(locals)-------------------------*/
   char        rce         = -10;
   llong       a           =   0;
   tTSPEC      t;
   static llong s_beg      =   0;
   llong        x_end      =   0;
   llong        x_dur      =   0;
   /*---(header)-------------------------*/
   DEBUG_YENV  yLOG_enter   (__FUNCTION__);
   /*---(current time)-------------------*/
   clock_gettime (CLOCK_MONOTONIC, &t);
   /*---(convert to millisec)------------*/
   a += (llong) t.tv_sec  * 1000000;
   a += (llong) t.tv_nsec / 1000;
   /*---(handle stage)-------------------*/
   switch (a_type) {
   case 'b'  :  s_beg = a;                          break;
   case 'e'  :  x_end = a;  x_dur = x_end - s_beg;  break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YENV  yLOG_exit    (__FUNCTION__);
   return x_dur;
}

llong ysort__intern_beg  (void) { return ysort__intern_dur ('b'); }
llong ysort__intern_end  (void) { return ysort__intern_dur ('e'); }


