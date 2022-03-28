/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"



/*====================------------------------------------====================*/
/*===----                          sorting list                        ----===*/
/*====================------------------------------------====================*/
static void  o___SORT____________o () { return; }

typedef  long long  llong;

static llong s_array     [1000];          /* working list as integers       */
static int  s_narray    =  0;            /* working count of entries       */

static llong s_bf        = 1000000000000;
static llong s_xf        = 100000000;
static llong s_yf        = 10000;
static llong s_zf        = 1;

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ysort_labels__prep      (char *a_list)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   char        x_list      [LEN_RECD];      /* copy of the list for parsing   */
   char       *p           = NULL;          /* strtok parsing pointer         */
   char       *q           = ",;";          /* strtok delimeter string        */
   char       *r           = NULL;
   int         i           =  0;            /* loop iterator -- entry         */
   int         u           =  0;            /* col of current entry           */
   int         x           =  0;            /* col of current entry           */
   int         y           =  0;            /* row of current entry           */
   int         z           =  0;            /* tab of current entry           */
   char        rc          =  0;            /* return code                    */
   void       *x_owner     = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT    yLOG_point   ("a_list"    , a_list);
   --rce;  if (a_list == NULL) {
      DEBUG_SORT    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT    yLOG_info    ("a_list"    , a_list);
   /*---(initialize)---------------------*/
   DEBUG_SORT    yLOG_note    ("initialize");
   for (i = 0; i < 1000; ++i)  s_array [i] = 0;
   s_narray = 0;
   /*---(parse/load)---------------------*/
   DEBUG_SORT    yLOG_note    ("load the array");
   strlcpy (x_list, a_list,  LEN_RECD);
   strlcpy (a_list, "#PREP", LEN_RECD);
   p = strtok_r (x_list, q, &r);
   s_narray = 0;
   --rce;  while (p != NULL) {
      DEBUG_SORT    yLOG_info    ("parse"     , p);
      rc = str2gyges (p, &u, &x, &y, &z, 0, 0, '-');
      DEBUG_SORT    yLOG_value   ("addresser" , rc);
      if (rc < 0)  {
         DEBUG_SORT    yLOG_note    ("could not parse, EXITING");
         DEBUG_SORT    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_SORT    yLOG_complex ("parts"     , "u=%04d, x=%04d, y=%04d, z=%04d", u, x, y, z);
      ++u;
      ++x;
      ++y;
      ++z;
      DEBUG_SORT    yLOG_complex ("inserted"  , "u=%04d, x=%04d, y=%04d, z=%04d", u, x, y, z);
      s_array [s_narray] = (u * s_bf) + (x * s_xf) + (y * s_yf) + z;
      DEBUG_SORT    yLOG_llong   ("value"          , s_array [s_narray]);
      p = strtok_r (NULL  , q, &r);
      ++s_narray;
   }
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ysort_labels__itself    (void)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   int         i           =  0;            /* loop iterator -- entry         */
   llong       a           =  0;            /* comparison entry one           */
   llong       b           =  0;            /* comparison entry two           */
   int         tele        = -1;            /* teleport point to speed sort   */
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(sort)---------------------------*/
   i = 1;
   while (i < s_narray) {
      /*---(load vars)-------------------*/
      a = s_array [i - 1];
      b = s_array [i];
      DEBUG_SORT    yLOG_complex ("current"   , "compare i,%3d (t,%3d) =  a,%10lld to b,%10lld", i, tele, a, b);
      /*---(compare)---------------------*/
      if (i == 0 || a <= b) {
         if (tele >= 0) {
            i    = tele;
            tele = -1;
         } else {
            ++i;
         }
         continue;
      }
      /*---(swap)------------------------*/
      s_array [i - 1] = b;
      s_array [i]     = a;
      a = s_array [i - 1];
      b = s_array [i];
      DEBUG_SORT    yLOG_complex ("swapped"   , "now     i,%3d (t,%3d) =  a,%10lld to b,%10lld", i, tele, a, b);
      /*---(update)----------------------*/
      if (tele < 0) tele = i;
      if (i > 1) --i;
   }
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ysort_labels__wrap      (char *a_list)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;            /* loop iterator -- entry         */
   int         u           =    0;            /* col of current entry           */
   int         x           =    0;            /* col of current entry           */
   int         y           =    0;            /* row of current entry           */
   int         z           =    0;            /* tab of current entry           */
   long        x_rem       =    0;
   /*> void       *x_owner     = NULL;                                                <*/
   /*> tDEP_ROOT  *x_deproot   = NULL;                                                <*/
   char        x_label     [LEN_LABEL];     /* label for sorted entry         */
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   strlcpy (a_list, ",", LEN_RECD);
   for (i = 0; i < s_narray; ++i) {
      DEBUG_SORT    yLOG_llong   ("value"   , s_array[i]);
      x_rem  = s_array [i];
      u      = x_rem / s_bf;
      x_rem -= u * s_bf;
      x      = x_rem / s_xf;
      x_rem -= x * s_xf;
      y      = x_rem / s_yf;
      x_rem -= y * s_yf;
      z      = x_rem;
      --u;
      --x;
      --y;
      --z;
      DEBUG_SORT    yLOG_complex ("parts"     , "u=%04d, x=%04d, y=%04d, z=%04d", u, x, y, z);
      rc = str4gyges (u, x, y, z, 0, x_label, '-');
      /*> if      (x_owner   == NULL)   sprintf (x_label, "[#%d]", i);                                  <* 
       *> else if (rc        <  0   )   sprintf (x_label, "<#%d>", i);                                  <* 
       *> else if (x_deproot != NULL)   strlcpy (x_label, ycalc_call_labeler (x_deproot), LEN_LABEL);   <* 
       *> else                          sprintf (x_label, "{#%d}", i);                                  <*/
      DEBUG_SORT    yLOG_info    ("label"   , x_label);
      strlcat (a_list, x_label, LEN_RECD);
      strlcat (a_list, ","    , LEN_RECD);
      DEBUG_SORT    yLOG_info    ("a_list"  , a_list);
   }
   DEBUG_SORT    yLOG_info    ("a_list"    , a_list);
   /*---(complete)-----------------------*/
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ySORT_labels       (char *a_list)
{
   /*---(purpose)-------------------------------------------------------------* 
    * for unit testing to be reliable, cosmetic changes must not result in    *
    * failures or the need to update the unit test script.  label lists from  *
    * the dependency linked lists are a prime example.  they are added in the *
    * order they are encountered, but simple reordering of operations can     *
    * make them appear different even if the same cells are listed.  this     *
    * routine simply sorts a list of dependencies so they are more stable.    */
   /*---(design notes)--------------------------------------------------------* 
    * there are many sort algorithms which can handle vast amounts of complex *
    * data, but they are simply overkill and problematic for small, simple    *
    * lists.  the teleporting gnome sort is my personal favorite due to its   *
    * simplicity which in turn leads to maintainability and confidence.       */
   /*---(oddities)------------------------------------------------------------* 
    * one is added to tab, col, and row before storage in the array and       *
    * sorting as the all three are consolidated mathmatically into a single   *
    * fieild and zeros are just a little harder to debug ;)                   */
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   char        rc          =  0;            /* return code                    */
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(defense: null)------------------*/
   --rce;  if (a_list  == NULL)  {
      DEBUG_SORT    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(sort)---------------------------*/
   rc = ysort_labels__prep   (a_list);
   DEBUG_SORT    yLOG_value   ("prep"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_SORT    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   rc = ysort_labels__itself ();
   DEBUG_SORT    yLOG_value   ("itself"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_SORT    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   rc = ysort_labels__wrap   (a_list);
   DEBUG_SORT    yLOG_value   ("wrap"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_SORT    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
   return rc;
}
