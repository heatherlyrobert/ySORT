#include    "sort_bm.h"

char
main                    (void)
{
   char       *x_main      = "\n##······· ct ·quan ····loops ····comps ··swaps ·teles ·calls ··usecs ···umin ···umax range ··pct dur/qua ·vbench  ·resort ratio  ·single ratio  btree vfull vsing   Ï  ";
   char       *x_sample    = "id ····loops ····comps ··swaps ·teles ·calls ··usecs   Ï  ";
   char        x_header    [LEN_RECD]  = "";
   int         x_ten, x_fifty, x_1hund, x_3hund, x_5hund, x_thou;
   int         i           =    0;
   int         n           =    5;

   printf ("## ySORT benchmarking of sort algorithms\n");
   printf ("##   -- all use exactly the same doubly-linked list structure and data\n");
   printf ("##   -- all share a defensvie swapping function\n");
   printf ("##   -- all use the same comparision technique\n");
   printf ("##   -- all share a wrapper to track time and report statistics\n");
   printf ("##   -- all are equally poorly written by me ;)\n");

   /*---(prepare)------------------------*/
   ystrlcpy (x_header, x_main, LEN_RECD);
   for (i = 0; i < n; ++i)  ystrlcat (x_header, x_sample, LEN_RECD);
   ystrlcat (x_header, "\n"  , LEN_RECD);

   /*---(pallet cleanser)----------------*/
   run_average (YSORT_DGNOME,   10,    1,   10, NULL);

   /*---(benchmark)----------------------*/
   printf (x_header);
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME,   10,  n, 0      , &x_ten  ));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME,   50,  n, 0      , &x_fifty));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME,  100,  n, 0      , &x_1hund));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME,  300,  n, 0      , &x_3hund));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME,  500,  n, 0      , &x_5hund));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME, 1000,  n, 0      , &x_thou ));

   /*---(gnome variations)---------------*/
   printf (x_header);
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME,   10,  n, x_ten  , NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME,   50,  n, x_fifty, NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME,  100,  n, x_1hund, NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME,  300,  n, x_3hund, NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME,  500,  n, x_5hund, NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME, 1000,  n, x_thou , NULL));

   printf (x_header);
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME,   10,  n, x_ten  , NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME,   50,  n, x_fifty, NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME,  100,  n, x_1hund, NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME,  300,  n, x_3hund, NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME,  500,  n, x_5hund, NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME, 1000,  n, x_thou , NULL));

   printf (x_header);
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME,   10,  n, x_ten  , NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME,   50,  n, x_fifty, NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME,  100,  n, x_1hund, NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME,  300,  n, x_3hund, NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME,  500,  n, x_5hund, NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME, 1000,  n, x_thou , NULL));

   /*---(common)-------------------------*/
   printf (x_header);
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE,   10,  n, x_ten  , NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE,   50,  n, x_fifty, NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE,  100,  n, x_1hund, NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE,  300,  n, x_3hund, NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE,  500,  n, x_5hund, NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE, 1000,  n, x_thou , NULL));

   printf (x_header);
   printf ("select   %s\n", run_average (YSORT_SELECT,   10,  n, x_ten  , NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT,   50,  n, x_fifty, NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT,  100,  n, x_1hund, NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT,  300,  n, x_3hund, NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT,  500,  n, x_5hund, NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT, 1000,  n, x_thou , NULL));

   printf (x_header);
   printf ("quick    %s\n", run_average (YSORT_QUICK ,   10,  n, x_ten  , NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK ,   50,  n, x_fifty, NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK ,  100,  n, x_1hund, NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK ,  300,  n, x_3hund, NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK ,  500,  n, x_5hund, NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK , 1000,  n, x_thou , NULL));

   /*---(monster)------------------------*/
   printf (x_header);
   printf ("troll    %s\n", run_average (YSORT_TROLL ,   10,  n, x_ten  , NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL ,   50,  n, x_fifty, NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL ,  100,  n, x_1hund, NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL ,  300,  n, x_3hund, NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL ,  500,  n, x_5hund, NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL , 1000,  n, x_thou , NULL));

   /*> printf (x_header);                                                               <* 
    *> printf ("troll    %s\n", run_average (YSORT_TROLL , 1000,  n, x_thou , NULL));   <* 
    *> printf ("troll    %s\n", run_average (YSORT_TROLL , 1000,  n, x_thou , NULL));   <* 
    *> printf ("troll    %s\n", run_average (YSORT_TROLL , 1000,  n, x_thou , NULL));   <* 
    *> printf ("troll    %s\n", run_average (YSORT_TROLL , 1000,  n, x_thou , NULL));   <* 
    *> printf ("troll    %s\n", run_average (YSORT_TROLL , 1000,  n, x_thou , NULL));   <* 
    *> printf ("troll    %s\n", run_average (YSORT_TROLL , 1000,  n, x_thou , NULL));   <*/

   printf (x_header);
   printf ("\n## end-of-data.  finito, completare, done.\n");
   return 0;
}
