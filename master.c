


char       /*----: set up program urgents/debugging --------------------------*/
ysort__unit_quiet       (void)
{
   yLOGS_begin ("ySORT", 's', 'Q');
   ysort_mock_init ();
   ysort__reinit   ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ysort__unit_loud        (void)
{
   yLOGS_begin ("ySORT", 's', 'N');
   yURG_by_name  ("kitchen"      , 'y');
   ysort_mock_init ();
   ysort__reinit   ();
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ysort__unit_end    (void)
{
   ySORT_purge_all ();
   ysort_mock_wrap ();
   ysort__reinit   ();
   yLOGS_end       ();
   return 0;
}
