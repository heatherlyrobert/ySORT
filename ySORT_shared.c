/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"



char    (*g_cursor)   (uchar a_type, void *a_head, void *a_tail, void *a_one, void **a_two, char a_action);
char    (*g_checker)  (uchar a_type, uchar a_lvl, void *a_one, void *a_two);
char    (*g_unlinker) (uchar a_type, void **a_head, void **a_tail, void *a_two);
char    (*g_linker)   (uchar a_type, void **a_head, void **a_tail, void *a_one, void *a_two);
char    (*g_slotter)  (uchar a_lvl, void *a_two);
char    (*g_joiner)   (void **a_bighead, void **a_bigtail, int *a_bigcount, void **a_subhead, void **a_subtail, int *a_subcount);



char    g_ready      = '-';
char    g_pure_troll = '-';



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char        ySORT_ver    [500];

char*      /* ---- : return library versioning information -------------------*/
ySORT_version           (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strlcpy (t, "[gnu gcc    ]", 15);
#elif  __HEPH__  > 0
   strlcpy (t, "[hephaestus ]", 15);
#else
   strlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (ySORT_ver, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return ySORT_ver;
}



/*====================------------------------------------====================*/
/*===----                     shared functions                         ----===*/
/*====================------------------------------------====================*/
static void      o___SHARED__________________o (void) {;}

char         /*-> check sort arguments ---------------[ leaf   [gn.530.341.50]*/ /*-[02.0000.000.!]-*/ /*-[--.---.---.--]-*/
ysort_defense           (char a_mode, void *a_head, void *a_tail)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(check endpoints)----------------*/
   DEBUG_SORT   yLOG_point   ("a_head"    , a_head);
   --rce;  if (a_head == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_point   ("a_tail"    , a_tail);
   --rce;  if (a_tail == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check gnome functions)----------*/
   DEBUG_SORT   yLOG_char    ("g_ready"   , g_ready);
   DEBUG_SORT   yLOG_char    ("a_mode"    , a_mode);
   --rce;  switch (a_mode) {
   case YSORT_GNOME :
      if (strchr ("gtp", g_ready) == NULL) {
         DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      break;
   case YSORT_TROLL :
      if (strchr ("tp" , g_ready) == NULL) {
         DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      break;
   default  :
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   }
   /*---(check gnome functions)----------*/
   DEBUG_SORT   yLOG_point   ("p_cursor"  , p_cursor);
   --rce;  if (p_cursor == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_cursor    = p_cursor;
   DEBUG_SORT   yLOG_point   ("p_checker" , p_checker);
   --rce;  if (p_checker == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_checker   = p_checker;
   DEBUG_SORT   yLOG_point   ("p_unlinker", p_unlinker);
   --rce;  if (p_unlinker == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_unlinker  = p_unlinker;
   DEBUG_SORT   yLOG_point   ("p_linker"  , p_linker);
   --rce;  if (p_linker == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_linker    = p_linker;
   /*---(check troll functions)----------*/
   if (strchr ("tp", a_mode) != NULL) {
      DEBUG_SORT   yLOG_point   ("p_slotter" , p_slotter);
      --rce;  if (p_slotter == NULL) {
         DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      g_slotter   = p_slotter;
      DEBUG_SORT   yLOG_point   ("p_joiner"  , p_joiner);
      --rce;  if (p_joiner == NULL) {
         DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      g_joiner    = p_joiner;
   }
   /*---(check troll uses gnome)---------*/
   if (a_mode == 'p') {
      g_pure_troll = '-';
   }
   /*---(set ready)----------------------*/
   g_ready = a_mode;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char
ySORT__reinit           (void)
{
   g_ready       = '-';
   g_cursor      = NULL;
   g_checker     = NULL;
   g_unlinker    = NULL;
   g_linker      = NULL;
   g_slotter     = NULL;
   g_joiner      = NULL;
   g_reverse     = '-';
   g_pure_troll  = '-';
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ySORT__unit_quiet       (void)
{
   yLOGS_begin ("ySORT", YLOG_SYS, YLOG_QUIET);
   MOCK__init    ();
   ySORT__reinit ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ySORT__unit_loud        (void)
{
   yLOGS_begin ("ySORT", YLOG_SYS, YLOG_NOISE);
   yURG_name  ("kitchen"      , YURG_ON);
   DEBUG_YSTR   yLOG_info     ("ySORT"   , ySORT_version   ());
   MOCK__init    ();
   ySORT__reinit ();
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ySORT__unit_end    (void)
{
   MOCK__wrap    ();
   ySORT__reinit ();
   yLOGS_end     ();
   return 0;
}
