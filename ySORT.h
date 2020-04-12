/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YSORT_HGUARD
#define YSORT_HGUARD loaded

typedef  unsigned char uchar;

#define    YSORT_GNOME      'g'
#define    YSORT_TROLL      't'
#define    YSORT_PURE       'p'

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char*       ySORT_version           (void);

char        ySORT_config            (char a_mode, void *p_cursor, void *p_checker, void *p_unlinker, void *p_linker, void *p_slotter, void *p_joiner);
char        ySORT_gnome             (uchar a_type, void **a_head, void **a_tail);
char        ySORT_troll             (uchar a_type, void **a_head, void **a_tail);

#endif
/*===[[ END ]]================================================================*/
