/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YSORT_priv_hguard
#define YSORT_priv_hguard loaded



/*===[[ HEADER ]]=============================================================*/
/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/

#define     P_FOCUS     "PS (programming support)"
#define     P_NICHE     "so (sort/search)"
#define     P_PURPOSE   "provide strong link-list sorting and searching capabilities"

#define     P_NAMESAKE  ""
#define     P_HERITAGE  ""
#define     P_IMAGERY   ""

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "small       (appoximately 1,000 slocl)"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2020-04"
#define     P_DEPENDS   "none"

#define     P_VERMAJOR  "0.Xx pre-production"
#define     P_VERMINOR  "0.5x main sorts tested and working"
#define     P_VERNUM    "0.5a"
#define     P_VERTXT    "base gnome and troll sorts passed basic unit testing"



/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
#include    <stdio.h>
#include    <stdlib.h>                  /* getenv()                            */
#include    <string.h>

#include    <yURG.h>                    /* heatherly program logger            */
#include    <yLOG.h>                    /* heatherly program logger            */
#include    "ySTR.h"



typedef struct cSORT_DATA tSORT_DATA;
struct cSORT_DATA {
   char        seq;
   char        label       [LEN_LABEL];
   tSORT_DATA *prev;
   tSORT_DATA *next;
};
extern void   *g_head;
extern void   *g_tail;
extern int     g_count;



extern char    g_ready;
extern char    g_reverse;
extern char    g_pure_troll;



typedef  struct cSLOT tSLOT;
struct cSLOT {
   int         count;
   void       *head;
   void       *tail;
};
#define     SEVENBIT      128



extern char    (*g_cursor)   (uchar a_type, void *a_head, void *a_tail, void *a_one, void **a_two, char a_action);
extern char    (*g_checker)  (uchar a_type, uchar a_lvl, void *a_one, void *a_two);
extern char    (*g_unlinker) (uchar a_type, void **a_head, void **a_tail, void *a_two);
extern char    (*g_linker)   (uchar a_type, void **a_head, void **a_tail, void *a_one, void *a_two);

extern char    (*g_slotter)  (uchar a_lvl, void *a_two);
extern char    (*g_joiner)   (void **a_bighead, void **a_bigtail, int *a_bigcount, void **a_subhead, void **a_subtail, int *a_subcount);



extern char   unit_answer [LEN_RECD];


char        ySORT__reinit           (void);
char        ySORT__unit_quiet       (void);
char        ySORT__unit_loud        (void);
char        ySORT__unit_end         (void);


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        GNOME_driver            (uchar a_type, uchar a_lvl, void **a_head, void **a_tail);
char        GNOME__reverse          (void);
char        GNOME_stats             (int *a_comps, int *a_swaps, int *a_teles);
char*       GNOME__unit             (char *a_question, int a_num);


char        TROLL__prepare          (void *p_slotter, void *p_joiner);
char        TROLL__scatter          (uchar a_type, uchar a_lvl, void **a_head, void **a_tail, int *a_count, tSLOT a_slots [SEVENBIT]);
char*       SCATTER__unit           (char *a_question, tSLOT a_slots [SEVENBIT], char a_num);
char*       TROLL__unit             (char *a_question, int a_num);


char        MOCK__init              (void);
char        MOCK__wrap              (void);
char        MOCK__cursor            (uchar a_type, void *a_head, void *a_tail, void *a_beg, void **a_new, char a_action);
char        MOCK__checker           (uchar a_type, uchar a_lvl, void *a_one, void *a_two);
char        MOCK__unlinker          (uchar a_type, void **a_head, void **a_tail, void *a_two);
char        MOCK__linker            (uchar a_type, void **a_head, void **a_tail, void *a_one, void *a_two);
char        MOCK__slotter           (uchar a_lvl, void *a_two);
char        MOCK__joiner            (void **a_bighead, void **a_bigtail, int *a_bigcount, void **a_subhead, void **a_subtail, int *a_subcount);
char        MOCK__creator           (char *a_name);
char        MOCK__printer           (tSORT_DATA *a_head);
char        ysort_defense           (char a_mode, void *a_head, void *a_tail);

#endif
