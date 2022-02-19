/*===[[ START ]]==============================================================*/
#include    "ySORT.h"
#include    "ySORT_priv.h"



char    (*g_cursor)   (uchar a_type, void *a_head, void *a_tail, void *a_one, void **a_two, char a_action);
char    (*g_checker)  (uchar a_type, uchar a_lvl, void *a_one, void *a_two, uchar a_order);
char    (*g_unlinker) (uchar a_type, void **a_head, void **a_tail, void *a_two);
char    (*g_linker)   (uchar a_type, void **a_head, void **a_tail, void *a_one, void *a_two);

char    (*g_slotter)  (uchar a_lvl, void *a_two, uchar a_order);
char    (*g_joiner)   (void **a_bighead, void **a_bigtail, int *a_bigcount, void **a_subhead, void **a_subtail, int *a_subcount);

char    (*g_forker)   (uchar a_type, void *a_node, void **a_left, void **a_right);



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

char
ysort__reinit           (void)
{
   g_ready       = '-';
   g_cursor      = NULL;
   g_checker     = NULL;
   g_unlinker    = NULL;
   g_linker      = NULL;
   g_slotter     = NULL;
   g_joiner      = NULL;
   g_forker       = NULL;
   g_order       = YSORT_ASCEND;
   g_pure_troll  = '-';
   return 0;
}

char         /*-> check sort arguments ---------------[ leaf   [gn.530.341.50]*/ /*-[02.0000.000.!]-*/ /*-[--.---.---.--]-*/
ysort_defense           (uchar a_mode, uchar a_order, void *a_head, void *a_tail)
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
   case YSORT_SEARCH :
      if (strchr ("gtp", g_ready) == NULL) {
         DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_SORT   yLOG_point   ("g_forker"   , g_forker);
      if (g_forker == NULL) {
         DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      break;
   default  :
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
      break;
   }
   /*---(check order)--------------------*/
   DEBUG_SORT   yLOG_char    ("a_order"   , a_order);
   --rce;  if (a_order == 0 || strchr (YSORT_ORDERS, a_order) == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_order = a_order;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySORT_config            (uchar a_mode, void *p_cursor, void *p_checker, void *p_unlinker, void *p_linker, void *p_slotter, void *p_joiner, void *p_forker)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   ysort__reinit ();
   /*---(check mode)---------------------*/
   DEBUG_SORT   yLOG_char    ("a_mode"    , a_mode);
   --rce;  if (strchr (YSORT_ENGINES, a_mode) == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
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
   if (strchr (YSORT_TROLLS, a_mode) != NULL) {
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
   /*---(check troll functions)----------*/
   DEBUG_SORT   yLOG_point   ("p_forker"  , p_forker);
   if (p_forker != NULL)    g_forker     = p_forker;
   /*---(check troll uses gnome)---------*/
   if (a_mode == YSORT_PURE)    g_pure_troll = 'y';
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

char       /*----: set up program urgents/debugging --------------------------*/
ysort__unit_quiet       (void)
{
   yLOGS_begin ("ySORT", YLOG_SYS, YLOG_QUIET);
   ySORT_purge_all ();
   ysort_mock_init    ();
   ysort__reinit ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ysort__unit_loud        (void)
{
   yLOGS_begin ("ySORT", YLOG_SYS, YLOG_NOISE);
   yURG_name  ("kitchen"      , YURG_ON);
   DEBUG_YSTR   yLOG_info     ("ySORT"   , ySORT_version   ());
   ysort_mock_init    ();
   ysort__reinit ();
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ysort__unit_end    (void)
{
   ySORT_purge_all ();
   ysort_mock_wrap    ();
   ysort__reinit ();
   yLOGS_end     ();
   return 0;
}
