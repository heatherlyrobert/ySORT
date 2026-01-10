/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YSORT_HGUARD
#define YSORT_HGUARD loaded



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
   "if you knowingly violate the spirit of these ideas, i suspect you might "    \
   "find any number of freedom-minded hackers may take it quite personally ;)"

/*´´·········1·········2·········3·········4·········5·········6·········7·········8  */
/*===[[ GNU GENERAL PUBLIC LICENSE (GPL) ]]===================================*/


#include    <ySTR_solo.h>

typedef  const    char cchar;
typedef  unsigned char uchar;
typedef  long long     llong;

typedef     struct      cSORT      tSORT;


#define    YSORT_PIXIE      'x'
#define    YSORT_GNOME      'g'
#define    YSORT_TROLL      't'
#define    YSORT_PURE       'p'
#define    YSORT_TROLLS     "tp"
#define    YSORT_ENGINES    "xgtp"

#define    YSORT_CGNOME     'C'
#define    YSORT_DGNOME     'D'
#define    YSORT_TGNOME     'T'
#define    YSORT_SGNOME     'S'
#define    YSORT_UGNOME     'U'
#define    YSORT_BHOOK      'B'

#define    YSORT_BUBBLE     'b'
#define    YSORT_SELECT     'l'
#define    YSORT_QUICK      'q'


#define    YSORT_NONE       'n'
#define    YSORT_ASCEND     'a'
#define    YSORT_DESCEND    'd'
#define    YSORT_ORIGINAL   'o'
#define    YSORT_REVERSE    'r'
#define    YSORT_SEARCH     's'
#define    YSORT_ORDERS     "nadors"



char*       ySORT_version           (void);

char        ySORT_config            (uchar a_mode, void *p_cursor, void *p_checker, void *p_unlinker, void *p_linker, void *p_slotter, void *p_joiner, void *p_forker);

char        ySORT_pixie             (void *p_move, void *p_key, void *p_unhook, void *p_hook);

char        ySORT_gnome             (uchar a_type, uchar a_order, void **a_head, void **a_tail);
char        ySORT_gnome_stats       (int *a_sorts, int *a_comps, int *a_swaps, int *a_teles);

char        ySORT_troll             (uchar a_type, uchar a_order, void **a_head, void **a_tail);
char        ySORT_troll_stats       (int *a_buckets, int *a_slots, int *a_max, int *a_sorts, int *a_comps, int *a_swaps, int *a_teles);

char        ySORT_treeify           (uchar a_type, void *a_head, void *a_tail, int a_count, void **a_root);
char        ySORT_treeify_stats     (int *a_count, int *a_span, int *a_levels, float *a_filled);

char        ySORT_search            (uchar a_type, void *a_root, void *a_key, void **a_found);
char        ySORT_search_stats      (int *a_result, void **a_last, int *a_depth, char *a_path);

char        ySORT_walk              (uchar a_type, void *a_root, void *p_callback);


/*===[[ ySORT_btree.c ]]======================================================*/
/*········´ ´···············trees·´ ´·········································*/
char        ySORT_btree             (uchar a_abbr, cchar *a_sort);
/*········´ ´·············hooking·´ ´·········································*/
char        ySORT_hook              (uchar a_abbr, void *a_data, char a_sort [LEN_TITLE], tSORT **r_link);
char        ySORT_bhook             (uchar a_abbr, void *a_data, char a_sort [LEN_TITLE], tSORT **r_link, int *r_dur);
char        ySORT_unhook            (tSORT **r_link);
char        ySORT_purge             (uchar a_abbr);
char        ySORT_purge_all         (void);
/*········´ ´·············sorting·´ ´·········································*/
char        ySORT_list              (uchar a_abbr);
char        ySORT_push              (uchar a_abbr);
char        ySORT_pop               (uchar a_abbr);
/*········´ ´·············treeify·´ ´·········································*/
int         ySORT_btree_depth       (int a_size);
int         ySORT_btree_span        (int a_levels);
char        ySORT_prepare           (uchar a_abbr);
char        ySORT_treeform          (uchar a_abbr);
/*········´ ´··············search·´ ´·········································*/
int         ySORT_count             (uchar a_abbr);
char        ySORT_by_name           (uchar a_abbr, char a_name [LEN_TITLE], void **r_data, int *r_tries);
char        ySORT_by_cursor         (uchar a_abbr, char a_dir             , void **r_data, int *r_tries);
char        ySORT_by_index          (uchar a_abbr, int  a_index           , void **r_data, int *r_tries);
char        ySORT_by_tree           (uchar a_abbr, char a_key  [LEN_TITLE], void **r_data, int *r_tries);
char*       ySORT_validate          (uchar a_abbr, char a_dir);
/*········´ ´·············testing·´· ´·········································*/
char*       ySORT_btree_list        (uchar a_abbr);
/*········´ ´················DONE·´ ´·········································*/




char        ySORT_labels            (char *a_list);

char        ySORT_string            (char a_list [LEN_MASS]);
int         ySORT_string_count      (void);
char        ySORT_string_cursor     (char a_dir, char r_item [LEN_HUND]);
char        ySORT_string_index      (int i, char r_item [LEN_HUND]);


#endif
/*===[[ END ]]================================================================*/
