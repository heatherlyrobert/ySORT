/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YSORT_priv_hguard
#define YSORT_priv_hguard loaded



/*===[[ HEADER ]]=============================================================*/
/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/

#define     P_FOCUS     "PS (programming support)"
#define     P_NICHE     "so (sort/search)"
#define     P_SUBJECT   "sorting and searching"
#define     P_PURPOSE   "provide strong link-list sorting and searching capabilities"

#define     P_NAMESAKE  "sisyphos-katergaris (trickster)"
#define     P_HERITAGE  ""
#define     P_IMAGERY   ""
#define     P_REASON    ""

#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT

#define     P_BASENAME  ""
#define     P_FULLPATH  ""
#define     P_SUFFIX    ""
#define     P_CONTENT   ""

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "small       (appoximately 1,000 slocl)"
#define     P_DEPENDS   "none"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2020-04"

#define     P_VERMAJOR  "0.Xx pre-production"
#define     P_VERMINOR  "0.5x main sorts tested and working"
#define     P_VERNUM    "0.5g"
#define     P_VERTXT    "drew in polymnia btree interface and unit tested"


/*
 *  i use sorts to maintain data in real-time.  batch sorting is a different
 *  animal altogether.
 *
 *  i have four major use cases in mind...
 *
 *  1) gyges (moderate sized lists -- thousands)
 *     -- cells are added to and removed from master list continuously
 *     -- list must always be sorted and btree accessible
 *     -- could use special, self-balancing tree, but do not need to
 *     -- by handling in real-time, list is always nearly sorted
 *     -- just need a sort that can sort NEARLY SORTED LISTS very fast ;)
 *     -- complicated sorts have low max-times, but high min-times (not useful)
 *
 *  2) yCALC (modest sized lists -- hundreds)
 *     -- keeps dependency graphs of all calculatable cells
 *     -- individual cells list of requires, provides, and likes are valuable
 *     -- tokenized string lists used for unit testing and real-time highlighting
 *     -- they need to be sorted to be predictable and testable
 *
 *  3) helios (modest sized lists -- hundreds)
 *     -- reads entire file-systems and creates a lookup database
 *     -- sets of entries are sorted within each parent directory
 *     -- access is command-line and needs to be reasonalble, not lightning
 *     -- need a fast, good sort for MODEST SIZED LISTS (directories)
 *     -- since it runs in batch, it does not need to be lightning
 *
 *  4) gregg (moderate sized lists -- thousands)
 *     -- large gregg shorthand library of outlines and translations
 *     -- read from database and then sorted into a useable data structure
 *     -- need to reliably sort LARGISH LISTS (not business class)
 *     -- this is done during program start-up so needs to be snappy
 *
 *  GNOME sort is one of the the simplest sorts possible.
 *
 *  linked-list gnome...
 *     -- just needs to swap pointers rather than move all the data
 *     -- this greatly reduces the swap effort
 *
 *  linked-list, teleporting gnome...
 *     -- uses a teleport (quick jump) back to unsorted data after a swap
 *     -- this greatly reduces the number of camparisons
 *
 *  linked-list, delayed-swap, teleporting gnome...
 *     -- waits to find the ideal location before swapping (insertion sort-like)
 *     -- this greatly reduces the number of swaps
 *
 *  when i say GNOME sort, i mean a linked-list, delayed-swap, teleporting gnome
 *     -- this baby is fast on nearly sorted data and not bad any moderate list
 *     -- the sort order is also stable (useful)
 *     -- its as fast as insertion, selection, ...
 *
 *  BUT, for really big stuff, i built TROLL...
 *     -- recursive, bucketing, merge sort
 *     -- uses GNOME whenever a bucket gets to 10 or less items
 *     -- its a monster and quite blood fast for me
 *  
 *  AND, i will never build a large-data sort as that has been very well
 *  covered by theoticians and packaged libraries
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */





/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
#include    <stdio.h>
#include    <stdlib.h>                  /* getenv()                            */
#include    <string.h>

#include    <yURG.h>                    /* heatherly program logger            */
#include    <yLOG.h>                    /* heatherly program logger            */
#include    <ySTR.h>
#include    <yDLST_solo.h>


typedef     struct      cSORT       tSORT;

/*----------+-----------+-----------+-----------+-----------+-----------+-----*/
struct      cSORT {
   /*---(information)-------*/
   char        n;
   char       *sort;
   /*---(linked list)-------*/
   tSORT      *prev;
   tSORT      *next;
   /*---(searching)---------*/
   tSORT      *left;
   tSORT      *right;
   /*---(data)--------------*/
   void       *data;
   /*---(done)--------------*/
};

typedef struct cMOCK tMOCK;
struct cMOCK {
   char        seq;
   char        label       [LEN_LABEL];
   tMOCK *prev;
   tMOCK *next;
   tMOCK *left;
   tMOCK *right;
};
extern tMOCK   *g_head;
extern tMOCK   *g_tail;
extern int           g_count;


extern int   g_depth;
extern char  g_path    [LEN_HUND];

extern char    g_ready;
extern uchar   g_order;
extern char    g_pure_troll;



typedef  struct cSLOT tSLOT;
struct cSLOT {
   int         count;
   void       *head;
   void       *tail;
};
#define     SEVENBIT      255



extern char    (*g_walk)      (void **a_cur, uchar a_mov);
extern char*   (*g_key)       (void *a_cur);
extern char    (*g_unhook)    (void *a_cur);
extern char    (*g_hook)      (void *a_ref, void *a_cur);


extern char    (*g_cursor)   (uchar a_type, void *a_head, void *a_tail, void *a_one, void **a_two, char a_action);
extern char    (*g_checker)  (uchar a_type, uchar a_lvl, void *a_one, void *a_two, uchar a_order);
extern char    (*g_unlinker) (uchar a_type, void **a_head, void **a_tail, void *a_two);
extern char    (*g_linker)   (uchar a_type, void **a_head, void **a_tail, void *a_one, void *a_two);

extern char    (*g_slotter)  (uchar a_lvl, void *a_two, uchar a_order);
extern char    (*g_joiner)   (void **a_bighead, void **a_bigtail, int *a_bigcount, void **a_subhead, void **a_subtail, int *a_subcount);

extern char    (*g_forker)   (uchar a_type, void *a_node, void **a_left, void **a_right);


extern char   unit_answer [LEN_RECD];



/*===[[ ySORT_btree.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(trees)----------------*/
char        ySORT_btree             (char a_abbr, char *a_sort);
char        ysort_btree_by_abbr     (char a_abbr);
/*---(hooking)--------------*/
char        ySORT_hook              (char a_abbr, void *a_data, char *a_sort, tSORT **r_link);
char        ySORT_unhook            (tSORT **r_link);
char        ysort_btree__remove     (char n, tSORT *a_old);
char        ySORT_purge             (char a_abbr);
char        ySORT_purge_all         (void);
/*---(sort)-----------------*/
char        ysort_btree_swap        (char n, tSORT *a_one, tSORT *a_two);
char        ysort_btree_dgnome      (char a_abbr);
/*---(sequence)-------------*/
char        ySORT_by_cursor         (char a_abbr, char a_dir, void **r_data);
char        ySORT_by_index          (char a_abbr, int i, void **r_data);
int         ySORT_count             (char a_abbr);
/*---(build)----------------*/
int         ysort_btree_depth       (int a_size);
int         ysort_btree_span        (int a_levels);
tSORT*      ysort_btree_nextlevel   (int n, int a_lvl, int a_pos, int a_dist, char a_dir, tSORT *a_node);
char        ysort_btree_build       (char a_abbr);
/*---(search)---------------*/
char        ysort_btree_display     (int a_lvl, tSORT *a_node);
char        ySORT_list              (char a_abbr);
tSORT*      ysort_btree_searchdown  (tSORT *a_node, char *a_dir, char *a_key);
char        ySORT_by_name           (char a_abbr, char *a_key, void **r_data);
/*---(full)-----------------*/
char        ySORT_prepare           (char a_abbr);
/*---(unittest)-------------*/
char*       ysort_btree__unit       (char a_btree, char *a_question, int i);
/*---(done)-----------------*/





char        ysort_defense           (uchar a_mode, uchar a_order, void *a_head, void *a_tail);
char        ysort__reinit           (void);
char        ysort__unit_quiet       (void);
char        ysort__unit_loud        (void);
char        ysort__unit_end         (void);

char        ysort_gnome_driver      (uchar a_type, uchar a_lvl, void **a_head, void **a_tail);
char*       ysort_gnome__unit       (char *a_question, int a_num);


char        ysort_troll__scatter    (uchar a_type, uchar a_lvl, void **a_head, void **a_tail, int *a_count, tSLOT a_slots [SEVENBIT]);
char        ysort_troll__gather     (void **a_head, void **a_tail, int *a_count, tSLOT a_slots [SEVENBIT]);
char*       ysort_scatter__unit     (char *a_question, tSLOT a_slots [SEVENBIT], char a_num);


char        ysort_mock_init         (void);
char        ysort_mock_wrap         (void);

char        ysort_mock__move        (tMOCK **a_cur, char a_move);
char*       ysort_mock__key         (tMOCK *a_cur);
char        ysort_mock__unhook      (tMOCK *a_cur);
char        ysort_mock__hook        (tMOCK *a_ref, tMOCK *a_cur);

char        ysort_mock__cursor      (uchar a_type, void *a_head, void *a_tail, void *a_beg, void **a_new, char a_action);
char        ysort_mock__checker     (uchar a_type, uchar a_lvl, void *a_one, void *a_two, uchar a_order);
char        ysort_mock__unlinker    (uchar a_type, void **a_head, void **a_tail, void *a_two);
char        ysort_mock__linker      (uchar a_type, void **a_head, void **a_tail, void *a_one, void *a_two);
char        ysort_mock__slotter     (uchar a_lvl, void *a_two, uchar a_order);
char        ysort_mock__joiner      (void **a_bighead, void **a_bigtail, int *a_bigcount, void **a_subhead, void **a_subtail, int *a_subcount);
char        ysort_mock__forker      (uchar a_type, void *a_node, void **a_left, void **a_right);
char        ysort_mock_creator      (char *a_name);
char        ysort_mock_printer      (tMOCK *a_head);



char*       ysort_treeify__unit     (char *a_question, int n);


#endif
