/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YSORT_HGUARD
#define YSORT_HGUARD loaded



typedef  unsigned char uchar;



#define    YSORT_GNOME      'g'
#define    YSORT_TROLL      't'
#define    YSORT_PURE       'p'
#define    YSORT_TROLLS     "tp"
#define    YSORT_ENGINES    "gtp"



#define    YSORT_ASCEND     'a'
#define    YSORT_DESCEND    'd'
#define    YSORT_ORIGINAL   'o'
#define    YSORT_REVERSE    'r'
#define    YSORT_ORDERS     "ador"



char*       ySORT_version           (void);

char        ySORT_config            (uchar a_mode, void *p_cursor, void *p_checker, void *p_unlinker, void *p_linker, void *p_slotter, void *p_joiner);

char        ySORT_gnome             (uchar a_type, uchar a_order, void **a_head, void **a_tail);

char        ySORT_troll             (uchar a_type, uchar a_order, void **a_head, void **a_tail);



#endif
/*===[[ END ]]================================================================*/
