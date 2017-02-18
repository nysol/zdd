
int CtoI_Lcm1_ub ( char *fname1, char *fname2, int th, int param, int len_ub ) { //ZDD
ITEMSET *II = &LCM_PP.II; //ZDD
TRSACT *TT = &LCM_PP.TT; //ZDD
SGRAPH *SG = &LCM_PP.SG; //ZDD
  
ERROR_MES = 0; //ZDD 
PROBLEM_init ( &LCM_PP ); //ZDD 
switch(param) //ZDD 
{ //ZDD 
case 12: //ZDD 
LCM_PP.problem |= PROBLEM_MAXIMAL; //ZDD 
II->flag |= ITEMSET_PRE_FREQ; //ZDD 
break; //ZDD 
case 11: //ZDD 
LCM_PP.problem |= PROBLEM_CLOSED; //ZDD 
II->flag |= ITEMSET_PRE_FREQ; //ZDD 
break; //ZDD 
case 10: //ZDD 
LCM_PP.problem |= PROBLEM_FREQSET; //ZDD 
II->flag |= ITEMSET_PRE_FREQ; //ZDD 
II->flag |= ITEMSET_ALL; //ZDD 
break; //ZDD 
case 2: //ZDD 
LCM_PP.problem |= PROBLEM_MAXIMAL; //ZDD 
break; //ZDD 
case 1: //ZDD 
LCM_PP.problem |= PROBLEM_CLOSED; //ZDD 
break; //ZDD 
case 0: //ZDD 
default: //ZDD 
LCM_PP.problem |= PROBLEM_FREQSET; //ZDD 
II->flag |= ITEMSET_ALL; //ZDD 
} //ZDD 
LCM_PP.trsact_fname = fname1; //ZDD 
LCM_PP.trsact_pfname = fname2; //ZDD 
II->frq_lb = (WEIGHT)th; //ZDD

II->ub = len_ub; // ham

if ( ERROR_MES ) return (1);
  TT->flag |= LOAD_PERM +TRSACT_FRQSORT +LOAD_DECSORT +LOAD_RM_DUP +TRSACT_MAKE_NEW +TRSACT_DELIV_SC +TRSACT_ALLOC_OCC + ((II->flag & ITEMSET_TRSACT_ID)?0: TRSACT_SHRINK) ;
  if ( II->flag&ITEMSET_RULE ) TT->w_lb = -WEIGHTHUGE; else TT->w_lb = II->frq_lb;
  SG->flag =  LOAD_EDGE;
PROBLEM_init2 ( &LCM_PP, PROBLEM_PRINT_SHRINK + PROBLEM_PRINT_FRQ ); //ZDD 

  if ( !ERROR_MES ){
LCM_init(&LCM_PP); //ZDD 
} //ZDD 
return 0; //ZDD 
} //ZDD 

