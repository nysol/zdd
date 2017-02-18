#!/usr/bin/env ruby
# encoding: utf-8

#パターン&ソースの登録
func = Hash.new()

func['| expr QUESTION expr COLON expr'] =<<'SCP_EOF'
"
/*##vsop_iif##*/
VALUE vsop_iif(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v1;
	VALUE v2;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"20\",&v1,&v2);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v1);
	CtoI *ctoi_mode = value2ctoi(v2);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| expr MEET expr'] =<<'SCP_EOF'
"
/*##vsop_meet##*/
VALUE vsop_meet(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| expr NE expr'] =<<'SCP_EOF'
"
/*##vsop_ne##*/
VALUE vsop_ne(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| print QUOTIENT IDVAR expr'] =<<'SCP_EOF'
"
VALUE vsop_print_arg1(VALUE self,char *arg){
	Vsop_Ruby* rmod;
//	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	CtoI *ctoi_modd =new CtoI(*rmod->cmod);
	int len = strlen(arg);
	char *str_c;
	str_c = new char[len+1];
	strcpy(str_c,arg);		
	#{pgm_src}
	return self;
}

"
SCP_EOF


func['| print QUOTIENT IDVAR FNAME expr'] =<<'SCP_EOF'
"
VALUE vsop_print_arg2(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v1,v2;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	CtoI *ctoi_mode =new CtoI(*rmod->cmod);
	rb_scan_args(argc, argv,\"20\",&v1,&v2);
	if(TYPE(v1)!=T_STRING){
		rb_raise(rb_eRuntimeError,\"argument type error (1st argument must be STRING)\");
	}
	if(TYPE(v2)!=T_STRING){
		rb_raise(rb_eRuntimeError,\"argument type error (2nd argument must be STRING)\");
	}
	char *arg1=RSTRING_PTR(v1);
	char *arg2=RSTRING_PTR(v2);

	int len;
	len = strlen(arg1);
	char *str_c;
	if(*arg1=='/'){
		str_c = new char[len];
		strcpy(str_c,arg1+1);
	}
	else{
		str_c = new char[len+1];
		strcpy(str_c,arg1);		
	}

	len = strlen(arg2);
	char *str_d = new char[len+3];
	sprintf(str_d,\"\\\"%s\\\"\",arg2);
	int len_d = len+2;

	#{pgm_src}
	return self;
}
"
SCP_EOF


func['| IMPORT LPAREN FNAME RPAREN'] =<<'SCP_EOF'
"
/*##vsop_import##*/
VALUE vsop_import(int argc, VALUE *argv, VALUE self){
	VALUE v;
	CtoI *ctoi_fin; 
/*
	VALUE vz = rb_cv_get(self,\"@@init_cnt\");
	int init_cnt_v =  NUM2INT(vz);
	if(init_cnt_v==0){ BDDV_Init(256, env_nmax);}
	init_cnt_v++;
	rb_cv_set(self,\"@@init_cnt\",INT2NUM(init_cnt_v));
*/
	if(init_cnt==0){ BDDV_Init(256, env_nmax);}
	init_cnt++;

	rb_scan_args(argc, argv,\"10\",&v);
	if(TYPE(v)!=T_STRING){
		rb_raise(rb_eRuntimeError,\"argument type error (1st argument must be STRING)\");
	}
	char *argtmp = RSTRING_PTR(v);
	int len;
	len = strlen(argtmp);
	char *str_c = new char[len+3];
	sprintf(str_c,\"\\\"%s\\\"\",argtmp);
	int len_c = len+2;
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF


func['| expr LE expr'] =<<'SCP_EOF'
"
/*##vsop_le##*/
VALUE vsop_le(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| expr LT expr'] =<<'SCP_EOF'
"
/*##vsop_lt##*/
VALUE vsop_lt(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| expr GE expr'] =<<'SCP_EOF'
"
/*##vsop_ge##*/
VALUE vsop_ge(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| expr GT expr'] =<<'SCP_EOF'
"
/*##vsop_gt##*/
VALUE vsop_gt(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF


func['| expr EQ expr'] =<<'SCP_EOF'
"
/*##vsop_eq##*/
VALUE vsop_eq(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr TLE LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_termsLE##*/
VALUE vsop_termsLE(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr TLT LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_termsLT##*/
VALUE vsop_termsLT(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr TGE LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_termsGE##*/
VALUE vsop_termsGE(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr TGT LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_termsGT##*/
VALUE vsop_termsGT(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr TNE LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_termsNE##*/
VALUE vsop_termsNE(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr TEQ LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_termsEQ##*/
VALUE vsop_termsEQ(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr FPC LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_freqpatC##*/
VALUE vsop_freqpatC(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr FPM LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_freqpatM##*/
VALUE vsop_freqpatM(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr FPA LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_freqpatA##*/
VALUE vsop_freqpatA(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr SYMGRP'] =<<'SCP_EOF'
"
/*##vsop_symgrp##*/
VALUE vsop_symgrp(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| MINUS expr %prec UMINUS'] =<<'SCP_EOF'
"
/*##vsop_minus_op##*/
VALUE vsop_minus_op(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	CtoI *ctoi_modb =new CtoI(*rmod->cmod);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| PLUS expr %prec UPLUS'] =<<'SCP_EOF'
"
/*##vsop_plus_op##*/
VALUE vsop_plus_op(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	CtoI *ctoi_modb =new CtoI(*rmod->cmod);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr PERMITSYM LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_permitsym##*/
VALUE vsop_permitsym(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr RESTRICT LPAREN expr RPAREN'] =<<'SCP_EOF'
"
/*##vsop_restrict##*/
VALUE vsop_restrict(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF


func['| u_expr ITEMS'] =<<'SCP_EOF'
"
/*##vsop_items##*/
VALUE vsop_items(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr TOTALVAL'] =<<'SCP_EOF'
"
/*##vsop_totalval##*/
VALUE vsop_totalval(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);

	CtoI a = rmod->cmod->TotalVal();	
	if(a.IsConst())
	{	
		if(a.TopItem() > 0) a = a.MaxVal();
		int d = a.TopDigit() / 3 + 14;
		kgAutoPtr2<char> a_ptr;
		char *s;
		try{
			a_ptr.set(new char[d]);
			s = a_ptr.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,\"memory allocation error\");
		}
	  int err = a.StrNum10(s);
	  if (err) { rb_raise(rb_eRuntimeError,\"StrNum10 error\"); }
		return rb_cstr2inum(s,10);
	}
	return 0;
}
"
SCP_EOF

func['| u_expr MINVAL'] =<<'SCP_EOF'
"
/*##vsop_minval##*/
VALUE vsop_minval(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr MAXVAL'] =<<'SCP_EOF'
"
/*##vsop_maxval##*/
VALUE vsop_maxval(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| expr REMAINDER expr'] =<<'SCP_EOF'
"
/*##vsop_remainder##*/
VALUE vsop_remainder(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| expr QUOTIENT expr'] =<<'SCP_EOF'
"
/*##vsop_quotiment##*/
VALUE vsop_quotiment(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| expr MULTIPLY expr'] =<<'SCP_EOF'
"
/*##vsop_multiply##*/
VALUE vsop_multiply(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| LCM LPAREN FNAME FNAME NUMBER FNAME RPAREN'] =<<'SCP_EOF'
"
VALUE vsop_lcm_order(int argc, VALUE *argv, VALUE self){
	if(init_cnt==0){ BDDV_Init(256, env_nmax);}
	init_cnt++;

	VALUE v1,v2,v3,v4;
	CtoI *ctoi_fin; 
	rb_scan_args(argc, argv,\"40\",&v1,&v2,&v3,&v4);
	if(TYPE(v1)!=T_STRING){
		rb_raise(rb_eRuntimeError,\"argument type error (1st argument must be STRING)\");
	}
	if(TYPE(v2)!=T_STRING){
		rb_raise(rb_eRuntimeError,\"argument type error (1st argument must be STRING)\");
	}
	if(TYPE(v3)!=T_FIXNUM){
		rb_raise(rb_eRuntimeError,\"argument type error (3rd argument must be FIXNUM)\");
	}
	if(TYPE(v4)!=T_STRING){
		rb_raise(rb_eRuntimeError,\"argument type error (4th argument must be STRING)\");
	}
	char *arg1 = RSTRING_PTR(v1);
	char *arg2 = RSTRING_PTR(v2);
	int arg3_fix = FIX2INT(v3);
	char *arg4 = RSTRING_PTR(v4);

	int len;

	len = strlen(arg1);
	char *str_c = new char[len+3];
	sprintf(str_c,\"\\\"%s\\\"\",arg1);
	int len_c = len+2;

	len = strlen(arg2);
	char *str_d = new char[len+3];
	sprintf(str_d,\"\\\"%s\\\"\",arg2);
	int len_d = len+2;

	char *str_e = new char[64];
	sprintf(str_e,\"%d\",arg3_fix);

	len = strlen(arg4);
	char *str_f = new char[len+3];
	sprintf(str_f,\"\\\"%s\\\"\",arg4);
	int len_f = len+2;

	#{pgm_src}

	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF



func['| LCM LPAREN FNAME FNAME NUMBER RPAREN'] =<<'SCP_EOF'
"
VALUE vsop_lcm_nomal(int argc, VALUE *argv, VALUE self){
	if(init_cnt==0){ BDDV_Init(256, env_nmax);}
	init_cnt++;

	VALUE v1,v2,v3;
	CtoI *ctoi_fin; 
	rb_scan_args(argc, argv,\"30\",&v1,&v2,&v3);
	if(TYPE(v1)!=T_STRING){
		rb_raise(rb_eRuntimeError,\"argument type error (1st argument must be STRING)\");
	}
	if(TYPE(v2)!=T_STRING){
		rb_raise(rb_eRuntimeError,\"argument type error (1st argument must be STRING)\");
	}
	if(TYPE(v3)!=T_FIXNUM){
		rb_raise(rb_eRuntimeError,\"argument type error (3rd argument must be FIXNUM)\");
	}
	char *arg1 = RSTRING_PTR(v1);
	char *arg2 = RSTRING_PTR(v2);
	int arg3_fix = FIX2INT(v3);

	int len;

	len = strlen(arg1);
	char *str_c = new char[len+3];
	sprintf(str_c,\"\\\"%s\\\"\",arg1);
	int len_c = len+2;

	len = strlen(arg2);
	char *str_d = new char[len+3];
	sprintf(str_d,\"\\\"%s\\\"\",arg2);
	int len_d = len+2;

	char *str_e = new char[64];
	sprintf(str_e,\"%d\",arg3_fix);
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| u_expr PERMIT LPAREN expr RPAREN']=<<'SCP_EOF'
"
/*##vsop_permit##*/
VALUE vsop_permit(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modd = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func[': print expr']=<<'SCP_EOF'
"
/*
 *  : print expr
 */

VALUE vsop_print(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	CtoI *ctoi_modb =new CtoI(*rmod->cmod);
	#{pgm_src}
	return self;
}
"
SCP_EOF

func['| expr PLUS expr']=<<'SCP_EOF'
"
/*##vsop_plus##*/
VALUE vsop_plus(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin;
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF

func['| expr MINUS expr']=<<'SCP_EOF'
"
/*##vsop_minus##*/
VALUE vsop_minus(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,\"10\",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	#{pgm_src}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}
"
SCP_EOF




def extract_pgm(src)
	start_char =0
	end_char =0
	writeflg=1
	pgmsrc = ""

	src.each{|line|
		writeflg =1
		if "#{line}" =~ /\{/ then
			if(start_char==0) then 
				writeflg= 0
			end
			start_char= start_char + 1
		elsif "#{line}" =~ /\}/ then
			end_char = end_char + 1
			if(start_char==end_char) then
				pgmsrc =pgmsrc.gsub("$1.str", "str_a")
				pgmsrc =pgmsrc.gsub("$2.str", "str_b")
				pgmsrc =pgmsrc.gsub("$3.str", "str_c")
				pgmsrc =pgmsrc.gsub("$4.str", "str_d")
				pgmsrc =pgmsrc.gsub("$5.str", "str_e")
				pgmsrc =pgmsrc.gsub("$6.str", "str_f")
				pgmsrc =pgmsrc.gsub("$1.len", "len_a")
				pgmsrc =pgmsrc.gsub("$2.len", "len_b")
				pgmsrc =pgmsrc.gsub("$3.len", "len_c")
				pgmsrc =pgmsrc.gsub("$4.len", "len_d")
				pgmsrc =pgmsrc.gsub("$5.len", "len_e")
				pgmsrc =pgmsrc.gsub("$6.len", "len_f")
				pgmsrc =pgmsrc.gsub("$$", "ctoi_fin")
				pgmsrc =pgmsrc.gsub("$1", "ctoi_moda")
				pgmsrc =pgmsrc.gsub("$2", "ctoi_modb")
				pgmsrc =pgmsrc.gsub("$3", "ctoi_modc")
				pgmsrc =pgmsrc.gsub("$4", "ctoi_modd")
				pgmsrc =pgmsrc.gsub("$5", "ctoi_mode")
				pgmsrc =pgmsrc.gsub("$6", "ctoi_modf")
		 		return pgmsrc
			end
		end
		if(writeflg == 1)then
			pgmsrc = pgmsrc + "#{line}"
		end
	}
end

def func_set(func)
	##最初に%{から%}までint main以外をすべて抜き出す
	File.open("SAPPOROBDD/app/VSOP/vsopyacc.y++","r"){|src|
		writeflg=0
		mainflg=0
		main_s=0
		main_e=0
		src.each{|line| 
			if "#{line}" =~ /^%\{/ then
				writeflg=1
			elsif "#{line}" =~ /^%\}/ then
				break 
			elsif "#{line}" =~ /^int main/ then
				mainflg=1
				writeflg=0
			elsif "#{line}" =~ /\{|\}/ and mainflg == 1 then
				if "#{line}" =~ /\{/ then
					main_s = main_s+1
				end 
				if "#{line}" =~ /\}/ then
					main_e = main_e+1
					if main_s == main_e then
						writeflg =1
						mainflg = 0
					end 
				end 
			else
				if writeflg==1 then
					File.open("../zdd_so.cpp","a"){|file|
						file.puts line
					}
				end				
			end
		}
	}
	#パターンごとの記述を抜き出す
	File.open("SAPPOROBDD/app/VSOP/vsopyacc.y++","r"){|src|
		src.each{|line| 
			if not func[line.squeeze(" ").chomp.lstrip.rstrip].nil?then
			key = line.squeeze(" ").chomp.lstrip.rstrip
			pgm_src = extract_pgm(src)
			File.open("../zdd_so.cpp","a"){|file|
				eval "file.puts " + func[key]
			}
			end
		}
	}
end		


if(File.exist?("../zdd_so.cpp"))then
	File.delete("../zdd_so.cpp")
end


#zdd.cxxにsetfunc.cppを入れ込み、zdd_so.cppを作成
File.open("zdd_so.cxx","r:utf-8"){|src|
	src.each{|line| 
		if "#{line}" =~ /^#include \"setfunc\.cpp\"/ then
			func_set(func)
		else
			File.open("../zdd_so.cpp","a"){|file|
				file.puts line
			}
		end
	}
}










