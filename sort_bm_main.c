#include    "sort_bm.h"

char
main                    (void)
{
   char       *x_main      = "\n##······· ct ·quan ····loops ····comps ··swaps ·teles ·calls ··usecs ···umin ···umax range ··pct dur/qua ·vbench  ·resort vfull vbnch  ·single vfull vbnch  btree vfull vsing   Ï  ";
   char       *x_sample    = "run ····loops ····comps ··swaps ·teles ·calls ··usecs   Ï  ";
   char        x_header    [LEN_RECD]  = "";
   int         x_ten, x_fifty, x_1hund, x_3hund, x_5hund, x_thou;
   int         x_rten, x_rfifty, x_r1hund, x_r3hund, x_r5hund, x_rthou;
   int         x_sten, x_sfifty, x_s1hund, x_s3hund, x_s5hund, x_sthou;
   int         i           =    0;
   int         n           =   10;

   printf ("#!/usr/local/bin/sort_bm\n");
   printf ("\n");
   printf ("##   %s\n", P_ONELINE);
   printf ("##   %s : %s\n", P_VERNUM, P_VERTXT);
   printf ("\n");
   printf ("##   ySORT benchmarking of various sort algorithms\n");
   printf ("##   -- all use exactly the same doubly-linked list structure and data\n");
   printf ("##   -- all share a defensvie swapping function\n");
   printf ("##   -- all use the same comparision technique\n");
   printf ("##   -- all share a wrapper to track time and report statistics\n");
   printf ("##   -- all are equally poorly written by me ;)\n");
   printf ("\n");
   printf ("#ù spacing   åÏ········ Ï· Ï···· Ï········ Ï········ Ï······ Ï····· Ï····· Ï······ Ï······ Ï······ Ï···· Ï···· Ï······ Ï······  Ï······ Ï···· Ï····  Ï······ Ï···· Ï····  Ï···· Ï···· Ï···· æ\n");
   printf ("#ú titles    ådesc····· ct quan· loops···· comps···· swaps···teles· calls· usecs·· umin··· umax··· range pct·· dur/qua vbench·  resort· vfull vbnch  single· vfull vbnch  btree vfull vsing æ\n");

   /*---(prepare)------------------------*/
   ystrlcpy (x_header, x_main, LEN_RECD);
   for (i = 0; i < n; ++i)  ystrlcat (x_header, x_sample, LEN_RECD);
   ystrlcat (x_header, "\n"  , LEN_RECD);

   /*---(pallet cleanser)----------------*/
   run_average (YSORT_DGNOME,   10,    1,    0,    0,    0, NULL, NULL, NULL);

   /*---(benchmark)----------------------*/
   printf (x_header);
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME ,   10,  n, 0      , 0       , 0       , &x_ten  , &x_rten  , &x_sten  ));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME ,   50,  n, 0      , 0       , 0       , &x_fifty, &x_rfifty, &x_sfifty));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME ,  100,  n, 0      , 0       , 0       , &x_1hund, &x_r1hund, &x_s1hund));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME ,  300,  n, 0      , 0       , 0       , &x_3hund, &x_r3hund, &x_s3hund));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME ,  500,  n, 0      , 0       , 0       , &x_5hund, &x_r5hund, &x_s5hund));
   printf ("dgnome   %s\n", run_average (YSORT_DGNOME , 1000,  n, 0      , 0       , 0       , &x_thou , &x_rthou , &x_sthou ));

   /*---(gnome variations)---------------*/
   printf (x_header);
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME ,   10,  n, x_ten  , x_rten  , x_sten  , NULL, NULL, NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME ,   50,  n, x_fifty, x_rfifty, x_sfifty, NULL, NULL, NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME ,  100,  n, x_1hund, x_r1hund, x_s1hund, NULL, NULL, NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME ,  300,  n, x_3hund, x_r3hund, x_s3hund, NULL, NULL, NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME ,  500,  n, x_5hund, x_r5hund, x_s5hund, NULL, NULL, NULL));
   printf ("cgnome   %s\n", run_average (YSORT_CGNOME , 1000,  n, x_thou , x_rthou , x_sthou , NULL, NULL, NULL));

   printf (x_header);
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME ,   10,  n, x_ten  , x_rten  , x_sten  , NULL, NULL, NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME ,   50,  n, x_fifty, x_rfifty, x_sfifty, NULL, NULL, NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME ,  100,  n, x_1hund, x_r1hund, x_s1hund, NULL, NULL, NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME ,  300,  n, x_3hund, x_r3hund, x_s3hund, NULL, NULL, NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME ,  500,  n, x_5hund, x_r5hund, x_s5hund, NULL, NULL, NULL));
   printf ("tgnome   %s\n", run_average (YSORT_TGNOME , 1000,  n, x_thou , x_rthou , x_sthou , NULL, NULL, NULL));

   printf (x_header);
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME ,   10,  n, x_ten  , x_rten  , x_sten  , NULL, NULL, NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME ,   50,  n, x_fifty, x_rfifty, x_sfifty, NULL, NULL, NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME ,  100,  n, x_1hund, x_r1hund, x_s1hund, NULL, NULL, NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME ,  300,  n, x_3hund, x_r3hund, x_s3hund, NULL, NULL, NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME ,  500,  n, x_5hund, x_r5hund, x_s5hund, NULL, NULL, NULL));
   printf ("ugnome   %s\n", run_average (YSORT_UGNOME , 1000,  n, x_thou , x_rthou , x_sthou , NULL, NULL, NULL));

   /*---(common)-------------------------*/
   printf (x_header);
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE ,   10,  n, x_ten  , x_rten  , x_sten  , NULL, NULL, NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE ,   50,  n, x_fifty, x_rfifty, x_sfifty, NULL, NULL, NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE ,  100,  n, x_1hund, x_r1hund, x_s1hund, NULL, NULL, NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE ,  300,  n, x_3hund, x_r3hund, x_s3hund, NULL, NULL, NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE ,  500,  n, x_5hund, x_r5hund, x_s5hund, NULL, NULL, NULL));
   printf ("bubble   %s\n", run_average (YSORT_BUBBLE , 1000,  n, x_thou , x_rthou , x_sthou , NULL, NULL, NULL));

   printf (x_header);
   printf ("select   %s\n", run_average (YSORT_SELECT ,   10,  n, x_ten  , x_rten  , x_sten  , NULL, NULL, NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT ,   50,  n, x_fifty, x_rfifty, x_sfifty, NULL, NULL, NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT ,  100,  n, x_1hund, x_r1hund, x_s1hund, NULL, NULL, NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT ,  300,  n, x_3hund, x_r3hund, x_s3hund, NULL, NULL, NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT ,  500,  n, x_5hund, x_r5hund, x_s5hund, NULL, NULL, NULL));
   printf ("select   %s\n", run_average (YSORT_SELECT , 1000,  n, x_thou , x_rthou , x_sthou , NULL, NULL, NULL));

   printf (x_header);
   printf ("quick    %s\n", run_average (YSORT_QUICK  ,   10,  n, x_ten  , x_rten  , x_sten  , NULL, NULL, NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK  ,   50,  n, x_fifty, x_rfifty, x_sfifty, NULL, NULL, NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK  ,  100,  n, x_1hund, x_r1hund, x_s1hund, NULL, NULL, NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK  ,  300,  n, x_3hund, x_r3hund, x_s3hund, NULL, NULL, NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK  ,  500,  n, x_5hund, x_r5hund, x_s5hund, NULL, NULL, NULL));
   printf ("quick    %s\n", run_average (YSORT_QUICK  , 1000,  n, x_thou , x_rthou , x_sthou , NULL, NULL, NULL));

   /*---(monster)------------------------*/
   printf (x_header);
   printf ("troll    %s\n", run_average (YSORT_TROLL  ,   10,  n, x_ten  , x_rten  , x_sten  , NULL, NULL, NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL  ,   50,  n, x_fifty, x_rfifty, x_sfifty, NULL, NULL, NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL  ,  100,  n, x_1hund, x_r1hund, x_s1hund, NULL, NULL, NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL  ,  300,  n, x_3hund, x_r3hund, x_s3hund, NULL, NULL, NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL  ,  500,  n, x_5hund, x_r5hund, x_s5hund, NULL, NULL, NULL));
   printf ("troll    %s\n", run_average (YSORT_TROLL  , 1000,  n, x_thou , x_rthou , x_sthou , NULL, NULL, NULL));

   printf (x_header);
   printf ("troll10  %s\n", run_average (YSORT_TROLL10,   10,  n, x_ten  , x_rten  , x_sten  , NULL, NULL, NULL));
   printf ("troll10  %s\n", run_average (YSORT_TROLL10,   50,  n, x_fifty, x_rfifty, x_sfifty, NULL, NULL, NULL));
   printf ("troll10  %s\n", run_average (YSORT_TROLL10,  100,  n, x_1hund, x_r1hund, x_s1hund, NULL, NULL, NULL));
   printf ("troll10  %s\n", run_average (YSORT_TROLL10,  300,  n, x_3hund, x_r3hund, x_s3hund, NULL, NULL, NULL));
   printf ("troll10  %s\n", run_average (YSORT_TROLL10,  500,  n, x_5hund, x_r5hund, x_s5hund, NULL, NULL, NULL));
   printf ("troll10  %s\n", run_average (YSORT_TROLL10, 1000,  n, x_thou , x_rthou , x_sthou , NULL, NULL, NULL));

   printf (x_header);
   printf ("troll30  %s\n", run_average (YSORT_TROLL30,   10,  n, x_ten  , x_rten  , x_sten  , NULL, NULL, NULL));
   printf ("troll30  %s\n", run_average (YSORT_TROLL30,   50,  n, x_fifty, x_rfifty, x_sfifty, NULL, NULL, NULL));
   printf ("troll30  %s\n", run_average (YSORT_TROLL30,  100,  n, x_1hund, x_r1hund, x_s1hund, NULL, NULL, NULL));
   printf ("troll30  %s\n", run_average (YSORT_TROLL30,  300,  n, x_3hund, x_r3hund, x_s3hund, NULL, NULL, NULL));
   printf ("troll30  %s\n", run_average (YSORT_TROLL30,  500,  n, x_5hund, x_r5hund, x_s5hund, NULL, NULL, NULL));
   printf ("troll30  %s\n", run_average (YSORT_TROLL30, 1000,  n, x_thou , x_rthou , x_sthou , NULL, NULL, NULL));

   printf (x_header);
   printf ("\n## end-of-data.  finito, completare, done.\n");
   return 0;
}
