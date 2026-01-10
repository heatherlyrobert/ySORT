

#include    <stdio.h>
#include    <stdlib.h>                  /* getenv()                            */
#include    <string.h>

#include    <ySORT.h>
#include    <ySTR.h>


/*===[[ sort_bm_code ]]=======================================================*/
/*········´ ´·····················´ ´·········································*/
char        load_data               (int a_quantity);
char*       run_print               (char a_prefix [LEN_LABEL], int a_loops, int a_comps, int a_swaps, int a_teles, int a_calls, int a_udur);
char*       run_single              (char a_run, char a_sort, int a_quantity, int *r_count, int *r_loops, int *r_comps, int *r_swaps, int *r_teles, int *r_calls, int *r_udur);
char*       run_average             (char a_sort, int a_quantity, int a_iterations, int a_bench, int *r_bench);

