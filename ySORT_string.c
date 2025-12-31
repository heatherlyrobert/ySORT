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
 *  the purpose of this capability is to force stable list orders, meaning
 *  text content separated by commas or semi-colons.
 *
 *  many simple lists are difficult to test and use unless in a stable order,
 *  for instance directory reads which come in reverse creation time order.
 *
 *
 */



static char s_list   [LEN_MASS]  = "";      /* full list                      */
static char s_item   [LEN_THOU][LEN_HUND];  /* list parsed into items         */
static int  s_nitem  = 0;                   /* count of items                 */
static int  s_cur    = 0;                   /* current item for cursors       */
static int  s_comp   = 0;
static int  s_swap   = 0;



/*====================------------------------------------====================*/
/*===----                          sorting list                        ----===*/
/*====================------------------------------------====================*/
static void  o___SORT____________o () { return; }

char         /*-> parse the item list ----------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ysort_string__prepare   (char a_list [LEN_MASS])
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   char        x_list      [LEN_MASS];      /* copy of the list for parsing   */
   char       *p           = NULL;          /* strtok parsing pointer         */
   char       *q           = ",;";          /* strtok delimeter string        */
   char       *r           = NULL;
   int         i           =  0;            /* loop iterator -- entry         */
   /*---(header)-------------------------*/
   DEBUG_YSORT    yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   ystrlcpy (s_list, "",  LEN_MASS);
   for (i = 0; i < LEN_THOU; ++i)  strcpy (s_item [i], "");
   s_nitem = 0;
   s_cur   = 0;
   /*---(defense)------------------------*/
   DEBUG_YSORT    yLOG_point   ("a_list"    , a_list);
   --rce;  if (a_list == NULL) {
      DEBUG_YSORT    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT    yLOG_info    ("a_list"    , a_list);
   /*---(initialize)---------------------*/
   DEBUG_YSORT    yLOG_note    ("load the array");
   ystrlcpy (x_list, a_list,  LEN_MASS);
   /*---(parse/load)---------------------*/
   p = strtok_r (x_list, q, &r);
   --rce;  while (p != NULL) {
      ystrlcpy (s_item [s_nitem], p, LEN_HUND);
      p = strtok_r (NULL  , q, &r);
      ++s_nitem;
   }
   DEBUG_YSORT    yLOG_value   ("s_nitem"   , s_nitem);
   /*---(complete)-----------------------*/
   DEBUG_YSORT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> actual sort  for list --------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ysort_string__itself    (void)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   int         i           =  0;            /* loop iterator -- entry         */
   char       *a           = NULL;          /* comparison entry one           */
   char       *b           = NULL;          /* comparison entry two           */
   char        t           [LEN_HUND]  = "";
   int         tele        = -1;            /* teleport point to speed sort   */
   /*---(header)-------------------------*/
   DEBUG_YSORT    yLOG_enter   (__FUNCTION__);
   /*---(sort)---------------------------*/
   s_comp = s_swap = 0;
   i = 1;
   while (i < s_nitem) {
      /*---(load vars)-------------------*/
      a = s_item [i - 1];
      b = s_item [i];
      DEBUG_YSORT    yLOG_complex ("current"   , "compare %3d (%3d)  %-100.100s vs %-100.100s", i, tele, a, b);
      /*---(compare)---------------------*/
      ++s_comp;
      if (i == 0 || strcmp (a, b) <= 0) {
         if (tele >= 0) {
            i    = tele;
            tele = -1;
         } else {
            ++i;
         }
         continue;
      }
      /*---(swap)------------------------*/
      ++s_swap;
      ystrlcpy (t, a, LEN_HUND);
      ystrlcpy (a, b, LEN_HUND);
      ystrlcpy (b, t, LEN_HUND);
      a = s_item [i - 1];
      b = s_item [i];
      DEBUG_YSORT    yLOG_complex ("swapped"   , "now     %3d (%3d)  %-100.100s    %-100.100s", i, tele, a, b);
      /*---(update)----------------------*/
      if (tele < 0) tele = i;
      if (i > 1) --i;
   }
   DEBUG_YSORT    yLOG_complex ("stats"     , "count = %3d, comps = %5d, swaps = %5d", s_nitem, s_comp, s_swap);
   DEBUG_YSORT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> return items to list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ysort_string__wrap      (char r_list [LEN_MASS])
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         =  -10;
   char       *rc          = NULL;
   int         i           =    0;            /* loop iterator -- entry         */
   char       *p           = NULL;
   char        x_last      [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_YSORT    yLOG_enter   (__FUNCTION__);
   /*---(concatenate)--------------------*/
   ystrlcpy (r_list, ",", LEN_MASS);
   --rce;  for (i = 0; i < s_nitem; ++i) {
      p = s_item [i];
      /*---(duplicates)------------------*/
      if (strcmp (p, x_last) == 0)  continue;
      ystrlcpy (x_last, p, LEN_HUND);
      /*---(append to list)--------------*/
      ystrlcat (r_list, p  , LEN_MASS);
      ystrlcat (r_list, ",", LEN_MASS);
      DEBUG_YSORT    yLOG_info    ("r_list"  , r_list);
      /*---(duplicates)------------------*/
   }
   if (strcmp (r_list, ",") == 0)  ystrlcpy (r_list, ".", LEN_MASS);
   DEBUG_YSORT    yLOG_info    ("r_list"    , r_list);
   /*---(complete)-----------------------*/
   DEBUG_YSORT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ySORT_string       (char b_list [LEN_MASS])
{
   /*---(purpose)-------------------------------------------------------------* 
    * for unit testing to be reliable, cosmetic changes must not result in    *
    * failures or the need to update the unit test script.  strings passed    *
    * frome directory reading is  a prime example.  they are added in the     *
    * order they are encountered, but simple reordering of operations can     *
    * make them appear different even if the same cells are listed.  this     *
    * routine simply sorts a list of strings so they are more stable.         */
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   char        rc          =  0;            /* return code                    */
   /*---(header)-------------------------*/
   DEBUG_YSORT    yLOG_enter   (__FUNCTION__);
   /*---(defense: null)------------------*/
   --rce;  if (b_list         == NULL)  {
      DEBUG_YSORT    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(sort)---------------------------*/
   rc = ysort_string__prepare (b_list);
   DEBUG_YSORT    yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ysort_string__itself ();
   DEBUG_YSORT    yLOG_value   ("itself"    , rc);
   --rce;  if (rc < 0) {
      ystrlcpy (b_list, "#SORT", LEN_RECD);
      DEBUG_YSORT    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ysort_string__wrap   (b_list);
   DEBUG_YSORT    yLOG_value   ("wrap"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSORT    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSORT    yLOG_exit    (__FUNCTION__);
   return rc;
}

int ySORT_string_count  (void)  { return s_nitem; }

char
ySORT_string_cursor     (char a_dir, char r_item [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_item != NULL)  strcpy (r_item, "");
   /*---(defenses)-----------------------*/
   --rce;  if (s_nitem == 0) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(navigate)-----------------------*/
   DEBUG_YSORT   yLOG_sint    (s_cur);
   DEBUG_YSORT   yLOG_schar   (a_dir);
   --rce;  if (strchr ("[<.>]", a_dir) == NULL) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  switch (a_dir) {
   case YDLST_HEAD : s_cur = 0;              break;
   case YDLST_PREV : --s_cur;                break;
   case YDLST_CURR : s_cur;                  break;
   case YDLST_NEXT : ++s_cur;                break;
   case YDLST_TAIL : s_cur = s_nitem - 1;    break;
   }
   /*---(bounds)-------------------------*/
   DEBUG_YSORT   yLOG_sint    (s_cur);
   if (s_cur < 0)         s_cur = 0;
   if (s_cur >= s_nitem)  s_cur = s_nitem - 1;
   DEBUG_YSORT   yLOG_sint    (s_cur);
   /*---(save results)-------------------*/
   DEBUG_YSORT   yLOG_snote   (s_item [s_cur]);
   if (r_item != NULL)   ystrlcpy (r_item, s_item [s_cur], LEN_HUND);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ySORT_string_index      (int i, char r_item [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   int         c           =    0;
   tSORT      *o           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_senter  (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_item != NULL)  strcpy (r_item, "");
   /*---(defenses)-----------------------*/
   DEBUG_YSORT   yLOG_sint    (i);
   --rce;  if (s_nitem == 0) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (i < 0) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (i >= s_nitem) {
      DEBUG_YSORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save results)-------------------*/
   s_cur = i;
   DEBUG_YSORT   yLOG_snote   (s_item [s_cur]);
   if (r_item != NULL)   ystrlcpy (r_item, s_item [s_cur], LEN_HUND);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char*
ysort_string__unit      (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   /*---(initialize)---------------------*/
   ystrlcpy (unit_answer, "STRING unit      : unknown request", LEN_FULL);
   /*---(basics)-------------------------*/
   if (strcmp (a_question, "item") == 0) {
      if (a_num < 0 || a_num >= LEN_THOU) {
         ystrlcpy (unit_answer, "STRING itm (---) : item number must be (0 - 999)", LEN_FULL);
      } else if (a_num >= s_nitem) {
         snprintf (unit_answer, LEN_FULL, "STRING itm (%3d) : ··åæ", a_num);
      } else {
         snprintf (unit_answer, LEN_FULL, "STRING itm (%3d) : %2då%sæ", a_num, strlen (s_item [a_num]), s_item [a_num]);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

