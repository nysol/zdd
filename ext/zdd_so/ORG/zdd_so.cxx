//lcmのruby拡張
#include <ruby.h>
#include <fstream>
#include <memory>
#include <stack>
//Vsop_Rubyクラス
#include"CtoI.h"

extern int CtoI_Lcm1_ub(char *, char *, int, int, int); // by ham

// ruby 1.8.5以下対応
#ifndef RSTRING_PTR
	#define RSTRING_PTR(s) (RSTRING(s)->ptr)
#endif
#ifndef RARRAY_PTR
	#define RARRAY_PTR(s) (RARRAY(s)->ptr)
#endif

CtoI* string2ctoi(char *str);
CtoI* int2ctoi(int val);
CtoI *value2ctoi(VALUE v);

//------------------------------------------------------------------------------
// 配列型に拡張したauto_ptr
//------------------------------------------------------------------------------
template<class T>
class kgAutoPtr2 
{
  T* _ptr;
public:
  kgAutoPtr2(T* ptr=0) : _ptr(ptr) {};
  virtual ~kgAutoPtr2(void) { if(_ptr != NULL) delete[] _ptr; }
  T* get(void) const throw() { return _ptr; }
  void set(T* ptr) throw() { if(_ptr!=NULL) delete[] _ptr; _ptr=ptr; }
  void clear(void) { if(_ptr!=NULL) delete[] _ptr; _ptr=0;}
};

class Vsop_Ruby
{
public:
	CtoI *cmod;	
	~Vsop_Ruby(void){ 
		if(cmod!=0) delete cmod;
	}	
};

void free_rmod(Vsop_Ruby* rmod){
	if(rmod!=0){
		delete rmod;
	}
}

//初期化カウント
int init_cnt=0;
//ノード数
#define DEF_ENV_NMAX 40000000
#define MAX_LEN 409600
#define HASH_MAX 4096000

int env_nmax=0;
bool env_warning = false;

#include "setfunc.cpp"

extern "C"{
	void Init_zdd_so();
}

int	yylineno=1;

static int  hashcnt;
static int  Depth;
static int* S_Var;
static int  PFflag;

static int  Depth_e;
static int* S_Var_e;

static int  Depth_item;
static int* S_Var_item;

static int* S_Var_ary;

//数値チェック（アイテムが数値なら警告）
static void num_check(char *str)
{
	char *p=str;
	if(*p=='-' || *p=='+' ) p++;
		while(*p){
		if( ( *p>='0' && *p<='9' ) || *p == '.' ){
				fprintf(stderr,"chech %c\n",*p);
				fprintf(stderr,"use numbers for symbol Variable\n");
				break;
		}
		p++;
	}
}

//----------------------- vsop_csvout -------------------------------
static int PutNum(CtoI a, int base,ofstream &fs)
{
	if(a.TopItem() > 0) a = a.MaxVal();
	int d = a.TopDigit() / 3 + 14;
	kgAutoPtr2<char> a_ptr;
	char *s;
	try{
		a_ptr.set(new char[d]);
		s = a_ptr.get();
	}catch(...){
		rb_raise(rb_eRuntimeError,"memory allocation error");
	}
	int err;
	if(base == 16) err = a.StrNum16(s);
	else err = a.StrNum10(s);
	if(err == 1)		return 1;
	fs << s<< ",";
	return 0;
}

static int PF(CtoI a, int base,ofstream &fs)
{
	if(a.IsConst())
	{
		if(PFflag == 1){fs << endl;}
		if(a.TopDigit() & 1) {fs << "-" ; a = -a; }
		
		PFflag = 1;
		int c1 = (a != 1);
		if(c1 || Depth == 0)
		{
			if(PutNum(a, base,fs) == 1) return 1;
		}
		else if(!c1){
			fs <<  "1,";
		}
		for(int i=0; i<Depth; i++)
		{
			if(i==0){ fs        << VTable.GetName(S_Var[i]);}
			else    { fs << " " << VTable.GetName(S_Var[i]);}
		}
		return 0;
	}
	int v = a.TopItem();
	CtoI b = a.Factor1(v);
	if(b == CtoI_Null()) return 1;
	S_Var[Depth] = v;
	Depth++;
	if(PF(b, base,fs) == 1) return 1;
	Depth--;
	b = a.Factor0(v);
	if(b == 0) return 0;
	if(b == CtoI_Null()) return 1;
	return PF(b, base,fs);
}

int CsvoutCtoI(CtoI a,ofstream &fs)
{
	if(a == CtoI_Null()) return 1;
	if(a == 0) return 0;
	else
	{
		int lev = BDD_LevOfVar(a.TopItem());
		Depth = 0;
		kgAutoPtr2<int> a_ptr;
		try{
			a_ptr.set(new int[lev]);
			S_Var = a_ptr.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		PFflag = 0;
		int err = PF(a, 10,fs);
		if(err == 1){ 
			rb_raise(rb_eRuntimeError,"memory over flow");
			return 1; 
		}
	}
	fs << endl;
	return 0;
}
/*##vsop_csvout##*/
VALUE vsop_csvout(int argc, VALUE *argv, VALUE self) {
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,"10",&v);
	char *str = RSTRING_PTR(v);
	ofstream fs;
	fs.open(str);
	if(fs.is_open()){
		CsvoutCtoI(*rmod->cmod,fs);
		fs.close();
	}
	else{
		rb_raise(rb_eRuntimeError,"file oepn error");
	}
	return self;
}
//----------------------- vsop_csvout -------------------------------

//----------------------- vsop_hashout -------------------------------
static int PF_hash(CtoI a, int base,VALUE& hash)
{
	if(a.IsConst())
	{
		char *valstr;
		kgAutoPtr2<char> a_ptr0;
		try{
			a_ptr0.set(new char[MAX_LEN]);
			valstr = a_ptr0.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		if(a.TopDigit() & 1) {strcpy(valstr,"-") ; a = -a; }
		else{ strcpy(valstr,"");}

		PFflag = 1;
		int c1 = (a != 1);

		char *valrtn;
		kgAutoPtr2<char> a_ptr;
		if(c1 || Depth == 0)
		{
			if(a.TopItem() > 0) a = a.MaxVal();
			int d = a.TopDigit() / 3 + 14;
			try{
				a_ptr.set(new char[d]);
				valrtn = a_ptr.get();
			}catch(...){
				rb_raise(rb_eRuntimeError,"memory allocation error");
			}
			int err;
			if(base == 16) err = a.StrNum16(valrtn);
			else err = a.StrNum10(valrtn);
			if(err == 1)
			{
				rb_raise(rb_eRuntimeError,"memory over flow");
				return 1;
			}
			strcat(valstr,valrtn);
		}
		else if(!c1){
			strcat(valstr,"1");
		}
		char *keystr;
		kgAutoPtr2<char> a_ptr1;
		try{
			a_ptr1.set(new char[MAX_LEN]);
			keystr = a_ptr1.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		*keystr = '\0';
		for(int i=0; i<Depth; i++)
		{
			int size = strlen(keystr)+strlen(VTable.GetName(S_Var[i]))+2;
			if(size>MAX_LEN){
				rb_raise(rb_eRuntimeError,"string size over flow");				
			}
			if(i==0){
				strcpy(keystr,VTable.GetName(S_Var[i]));
			}
			else{
				strcat(keystr," ");
				strcat(keystr,VTable.GetName(S_Var[i]));
			}
		}
		VALUE key = rb_str_new2(keystr);
		VALUE val = INT2NUM(atoi(valstr));
		rb_hash_aset(hash, key, val);
		hashcnt++;
		if(hashcnt> HASH_MAX){return 2;}
		return 0;
	}
	int v = a.TopItem();
	CtoI b = a.Factor1(v);
	if(b == CtoI_Null()) return 1;
	S_Var[Depth] = v;
	Depth++;
	int chk=PF_hash(b, base,hash);
	if(chk > 0) return chk;
	Depth--;
	b = a.Factor0(v);
	if(b == 0) return 0;
	if(b == CtoI_Null()) return 1;

	return PF_hash(b, base,hash);
}
VALUE HashoutCtoI(CtoI a,int *rtn)
{
	hashcnt=0;
	VALUE hash = rb_hash_new();
	if(a == CtoI_Null()) return 1;
	if(a == 0) return 0;
	else
	{
		int lev = BDD_LevOfVar(a.TopItem());
		Depth = 0;
		kgAutoPtr2<int> a_ptr;
		try{
			a_ptr.set(new int[lev]);
			S_Var = a_ptr.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		PFflag = 0;
		int err = PF_hash(a, 10,hash);
		*rtn = err;
		if(err == 1){
			rb_raise(rb_eRuntimeError,"memory over flow");
			return rb_hash_new();
		}
	}
	return hash;
}

/*##vsop_hashout##*/
VALUE vsop_hashout(VALUE self){
	int rtnflg=0;
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
 	VALUE hash = HashoutCtoI(*rmod->cmod,&rtnflg);
	if(rtnflg==2){
		rb_iv_set(self,"@partly", Qtrue );
	}
	else{
		rb_iv_set(self,"@partly", Qfalse);
	}
	return hash;
}
//----------------------- vsop_hashout -------------------------------

//----------------------- vsop_each -------------------------------
static int PF_array_each(CtoI a, VALUE& self)
{
	int sign=1;
	if(a.IsConst())
	{
		if(a.TopDigit() & 1) {sign= -1 ; a = -a; }
		CtoI rtn;
		int c1 = (a != 1);
		char *valrtn;
		kgAutoPtr2<char> a_ptr;
		if(c1 || Depth_e == 0)
		{
			if(a.TopItem() > 0) a = a.MaxVal();
			int d = a.TopDigit() / 3 + 14;
			try{
				a_ptr.set(new char[d]);
				valrtn = a_ptr.get();
			}catch(...){
				rb_raise(rb_eRuntimeError,"memory allocation error");
			}
			int  err = a.StrNum10(valrtn);
			if(err == 1){
				rb_raise(rb_eRuntimeError,"memory over flow");
				return 1;
			}
			rtn = CtoI(CtoI_atoi(valrtn));
		}
		else if(!c1){
			rtn = CtoI(CtoI_atoi("1"));;
		}

		for(int i=0; i<Depth_e; i++)
		{
			char *str = VTable.GetName(S_Var_e[i]);
			int ckck = VTable.GetID(str);
			rtn =Product(rtn, CtoI(1).AffixVar( ckck));
		}
		
		Vsop_Ruby* rmod=new Vsop_Ruby;
		rmod->cmod = new CtoI (rtn) ;
		rb_yield(Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod));
		return 0;
	}
			
	int v = a.TopItem();
	CtoI b = a.Factor1(v);
	if(b == CtoI_Null()) return 1;
	S_Var_e[Depth_e] = v;
	Depth_e++;
	int chk=PF_array_each(b, self);
	if(chk > 0) return chk;
	Depth_e--;
	b = a.Factor0(v);
	if(b == 0) return 0;
	if(b == CtoI_Null()) return 1;
	return PF_array_each(b,self);
}

void CtoI2Array_each(CtoI a,VALUE &self)
{
	if(a == CtoI_Null()) return ;
	if(a == 0) return ;
	else
	{
		int lev = BDD_LevOfVar(a.TopItem());
		Depth_e = 0;
		kgAutoPtr2<int> a_ptr;
		try{
			a_ptr.set(new int[lev]);
			S_Var_e = a_ptr.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		PF_array_each(a, self);
	}
}
/*##vsop_each##*/
VALUE vsop_each(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
 	CtoI2Array_each(*rmod->cmod,self);
	return Qtrue;	
}
//----------------------- vsop_each -------------------------------

//----------------------- vsop_to_a -------------------------------
static int PF_array(CtoI a,VALUE& array)
{
	if(a.IsConst())
	{
		char *valstr;
		kgAutoPtr2<char> a_ptr0;
		try{
			a_ptr0.set(new char[MAX_LEN]);
			valstr = a_ptr0.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		if(a.TopDigit() & 1) {strcpy(valstr,"-") ; a = -a; }
		else{ strcpy(valstr,"");}

		PFflag = 1;
		int c1 = (a != 1);

		char *valrtn;
		kgAutoPtr2<char> a_ptr;
		if(c1 || Depth == 0)
		{
			if(a.TopItem() > 0) a = a.MaxVal();
			int d = a.TopDigit() / 3 + 14;
			try{
				a_ptr.set(new char[d]);
				valrtn = a_ptr.get();
			}catch(...){
				rb_raise(rb_eRuntimeError,"memory allocation error");
			}
			int err;
			err = a.StrNum10(valrtn);
			if(err == 1)
			{
				rb_raise(rb_eRuntimeError,"memory over flow");
				return 1;
			}
			strcat(valstr,valrtn);
		}
		char *keystr;
		kgAutoPtr2<char> a_ptr1;
		try{
			a_ptr1.set(new char[MAX_LEN]);
			keystr = a_ptr1.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		*keystr = '\0';
		for(int i=0; i<Depth; i++)
		{
			int size = strlen(keystr)+strlen(VTable.GetName(S_Var_ary[i]))+2;
			if(size>MAX_LEN){
				rb_raise(rb_eRuntimeError,"string size over flow");				
			}
			if(i==0){
				strcpy(keystr,VTable.GetName(S_Var_ary[i]));
			}
			else{
				strcat(keystr," ");
				strcat(keystr,VTable.GetName(S_Var_ary[i]));
			}
		}
		if(*valstr&& *keystr){ 
			strcat(valstr," ");
			strcat(valstr,keystr); 
		}
		else{strcat(valstr,keystr); }

		VALUE val = rb_str_new2(valstr);
		rb_ary_push(array,val);
		return 0;
	}
	int v = a.TopItem();
	CtoI b = a.Factor1(v);
	if(b == CtoI_Null()) return 1;
	S_Var_ary[Depth] = v;
	Depth++;
	int chk=PF_array(b, array);
	if(chk > 0) return chk;
	Depth--;
	b = a.Factor0(v);
	if(b == 0) return 0;
	if(b == CtoI_Null()) return 1;

	return PF_array(b, array);
}

VALUE CtoI2Array(CtoI a)
{
	VALUE array = rb_ary_new();
	if(a == CtoI_Null()) return array;
	if(a == 0) return array;
	else
	{
		int lev = BDD_LevOfVar(a.TopItem());
		Depth = 0;
		kgAutoPtr2<int> a_ptr;
		try{
			a_ptr.set(new int[lev]);
			S_Var_ary = a_ptr.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		int err = PF_array(a, array);
		if(err == 1){ 
			rb_raise(rb_eRuntimeError,"memory over flow");
			return array; 
		}
	}
	return array;
}
/*##vsop_to_a##*/
VALUE vsop_to_a(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
 	VALUE array = CtoI2Array(*rmod->cmod);

	return array;
}
//----------------------- vsop_to_a -------------------------------

//----------------------- vsop_to_s -------------------------------
static int PutNum_str(CtoI a, int base,VALUE &str)
{
	if(a.TopItem() > 0) a = a.MaxVal();
	int d = a.TopDigit() / 3 + 14;
	kgAutoPtr2<char> a_ptr;
	char *s;
	try{
		a_ptr.set(new char[d]);
		s = a_ptr.get();
	}catch(...){
		rb_raise(rb_eRuntimeError,"memory allocation error");
	}
	int err;
	if(base == 16) err = a.StrNum16(s);
	else err = a.StrNum10(s);
	if(err == 1)		return 1;
	rb_str_cat(str,s,strlen(s));

	return 0;
}

static int PF_str(CtoI a,VALUE str)
{
	if(a.IsConst())
	{
		if(PFflag == 1){rb_str_cat(str," + ",strlen(" + "));}
		if(a.TopDigit() & 1) {rb_str_cat(str," - ",strlen(" - ")); a = -a; }
		
		PFflag = 1;
		int c1 = (a != 1);
		if(c1 || Depth == 0)
		{
			if(PutNum_str(a, 10,str) == 1) return 1;
			rb_str_cat(str," ",strlen(" "));
		}

		for(int i=0; i<Depth; i++)
		{
			char *p = VTable.GetName(S_Var[i]);
			rb_str_cat(str,p,strlen(p));
			if( i<Depth-1) rb_str_cat(str," ",strlen(" "));
		}
		return 0;
	}
	int v = a.TopItem();
	CtoI b = a.Factor1(v);
	if(b == CtoI_Null()) return 1;
	S_Var[Depth] = v;
	Depth++;
	if(PF_str(b, str) == 1) return 1;
	Depth--;
	b = a.Factor0(v);
	if(b == 0) return 0;
	if(b == CtoI_Null()) return 1;
	return PF_str(b,str);
}

VALUE CtoI2String(CtoI a)
{
	VALUE str = rb_str_new2("");

	if(a == CtoI_Null()) return str;
	if(a == 0) return str;
	else
	{
		int lev = BDD_LevOfVar(a.TopItem());
		Depth = 0;
		kgAutoPtr2<int> a_ptr;
		try{
			a_ptr.set(new int[lev]);
			S_Var = a_ptr.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		PFflag = 0;
		int err = PF_str(a, str);
		if(err == 1){ 
			rb_raise(rb_eRuntimeError,"memory over flow");
			return str; 
		}
	}
	return str;
}

/*##vsop_to_s##*/
VALUE vsop_to_s(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
 	VALUE str = CtoI2String(*rmod->cmod);
	return str;
}
//----------------------- vsop_to_s -------------------------------


//----------------------- vsop_each_item -------------------------------
static int PF_itemarray(CtoI a, VALUE& self)
{
	int sign=1;
	if(a.IsConst())
	{
		if(a.TopDigit() & 1) {sign= -1 ; a = -a; }
		int c1 = (a != 1);
		VALUE weight;
		char *valrtn;
		kgAutoPtr2<char> a_ptr;
		if(c1 || Depth_item == 0)
		{
			if(a.TopItem() > 0) a = a.MaxVal();
			int d = a.TopDigit() / 3 + 14;
			try{
				a_ptr.set(new char[d]);
				valrtn = a_ptr.get();
			}catch(...){
				rb_raise(rb_eRuntimeError,"memory allocation error");
			}
			int  err = a.StrNum10(valrtn);
			if(err == 1){
				rb_raise(rb_eRuntimeError,"memory over flow");
				return 1;
			}
			weight = INT2NUM(atoi(valrtn));
		}
		else if(!c1){
			weight = INT2NUM(1);
		}

		VALUE top=Qtrue;
		VALUE bottom=Qfalse;
		for(int i=0; i<Depth_item; i++){
			char *str = VTable.GetName(S_Var_item[i]);
			int ckck = VTable.GetID(str);
			Vsop_Ruby* rmod=new Vsop_Ruby;
			rmod->cmod = new CtoI (CtoI(1).AffixVar(ckck)) ;
			if( i>0 ) top=Qfalse;
			if( i+1==Depth_item ) bottom=Qtrue; 
			rb_yield_values(4,weight,Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod) ,top,bottom);
		}
		return 0;
	}	
	int v = a.TopItem();
	CtoI b = a.Factor1(v);
	if(b == CtoI_Null()) return 1;
	S_Var_item[Depth_item] = v;
	Depth_item++;
	int chk=PF_itemarray(b, self);
	if(chk > 0) return chk;
	Depth_item--;
	b = a.Factor0(v);
	if(b == 0) return 0;
	if(b == CtoI_Null()) return 1;
	return PF_itemarray(b,self);
}

void CtoI2ItemArray(CtoI a,VALUE &self)
{
	VALUE ary = rb_ary_new();
	if(a == CtoI_Null()) return;
	if(a == 0) return;
	else
	{
		int lev = BDD_LevOfVar(a.TopItem());
		Depth_item = 0;
		kgAutoPtr2<int> a_ptr;
		try{
			a_ptr.set(new int[lev]);
			S_Var_item = a_ptr.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
		PF_itemarray(a, self);
	}
}

/*##vsop_each_item##*/
VALUE vsop_each_item(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
 	CtoI2ItemArray(*rmod->cmod,self);
	return Qtrue;	
}
//----------------------- vsop_each_item -------------------------------


//この値はBDDライブラリとかぶらないよう注意すること
// キャッシュがおかしくなる
static const char BC_CtoI_DELTA =  50;

CtoI CtoI_Delta(CtoI a, CtoI b)
{
  if(a == CtoI_Null())  return a;
  if(b == CtoI_Null()) return b;
  if(a == 0) return 0;
  if(b == 0) return 0;
  if(a == 1 && b==1) return 1;


  int atop = a.Top();
  int btop = b.Top();
  if(BDD_LevOfVar(atop) < BDD_LevOfVar(btop)) return CtoI_Delta(b, a);

  bddword ax = a.GetZBDD().GetID();
  bddword bx = b.GetZBDD().GetID();
  if(atop == btop && ax < bx) return CtoI_Delta(b, a);

  ZBDD z = BDD_CacheZBDD(BC_CtoI_DELTA, ax, bx);
  if(z != -1) return CtoI(z);

  CtoI a0 = a.Factor0(atop);
  CtoI a1 = a.Factor1(atop);
  CtoI c;
  if(atop != btop)
  {
		if(a.IsBool()) c =  CtoI_Union( CtoI_Delta(a0, b), CtoI_Delta(a1, b).AffixVar(atop) );
    else c = CtoI_Delta(a0, b) + CtoI_Delta(a1, b).TimesSysVar(atop);
  }
  else
  {
    CtoI b0 = b.Factor0(atop);
    CtoI b1 = b.Factor1(atop);
    if(a.IsBool())
      c = CtoI_Union( CtoI_Delta(a0, b0) + CtoI_Delta(a1, b1),
             (CtoI_Delta(a1, b0)+ CtoI_Delta(a0, b1)).AffixVar(atop) ) ;
		else if(atop > 1)
      c = CtoI_Delta(a0, b0)
        + (CtoI_Delta(a1, b0) + CtoI_Delta(a0, b1)).TimesSysVar(atop)
        + CtoI_Delta(a1, b1).TimesSysVar(atop-1);
    else BDDerr("CtoI_Delta(): SysVar overflow.");
  }
  BDD_CacheEnt(BC_CtoI_DELTA, ax, bx, c.GetZBDD().GetID());
  return c;
}

/*##vsop_delta##*/
VALUE vsop_delta(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,"10",&v);
	CtoI *ctoi_moda =new CtoI(*rmod->cmod);
	CtoI *ctoi_modc = value2ctoi(v);
	CtoI *ctoi_fin; 
	*ctoi_moda = CtoI_Delta(*ctoi_moda, *ctoi_modc);
  ctoi_fin = ctoi_moda;
  delete ctoi_modc;
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}

CtoI* string2ctoi(char *str)
{
	if(init_cnt==0){ BDDV_Init(256, env_nmax);}
	init_cnt++;
	int sep_cnt=0;
	for(char *p=str;*p;p++){
		if(*p==' '){ 
			sep_cnt++;
		}
	}
	sep_cnt++;
	kgAutoPtr2<CtoI> a_ptr;
	CtoI * ctoitmp;
	try{
		a_ptr.set(new CtoI[sep_cnt]);
		ctoitmp = a_ptr.get();
	}catch(...){
		rb_raise(rb_eRuntimeError,"memory allocation error");
	}
	int i=0;
	char *p,*q;
	p=str;
	q=str;
	while(1){
		if(*p==' '){//区切り文字
			*p='\0';
			int var = VTable.GetID(q);     
			if(var == 0){
				if(env_warning){ num_check(q); }
				VTable.SetB(q, power16/2);
				var  = VTable.GetID(q);
			}
			ctoitmp[i] = CtoI(1).AffixVar(var);
			q=p+1;
			i++;
		}
		else if(*p=='\0'){//終了時文字
			int var = VTable.GetID(q);     
			if(var == 0){
				if(env_warning){ num_check(q); }
			 	VTable.SetB(q, power16/2);
				var  = VTable.GetID(q);
			}
			ctoitmp[i] = CtoI(1).AffixVar(var);
			break;
		}
		p++;
	}
	CtoI ctmp = ctoitmp[0];
	for(int i=1;i<sep_cnt;i++){
		ctmp = Product(ctmp,ctoitmp[i]);
	}
	CtoI *rtnctoi = new CtoI(ctmp);
	return rtnctoi;
}

//VALUEを受け取り、CtoI*を返す
CtoI *value2ctoi(VALUE v)
{
 CtoI *rtnctoi;
	Vsop_Ruby* argrmod;
	if(TYPE(v)==T_STRING){
		rtnctoi=string2ctoi(RSTRING_PTR(v));
	}
	else if(TYPE(v)==T_FIXNUM){
		rtnctoi=int2ctoi(FIX2INT(v));
	}
	else if(TYPE(v)==T_BIGNUM){
		rtnctoi=int2ctoi(NUM2INT(v));
	}
	else{
		Data_Get_Struct(v,Vsop_Ruby,argrmod);
		rtnctoi =new CtoI(*argrmod->cmod);
	}
	return rtnctoi; 
}

/*##vsop_symbol##*/
VALUE vsop_symbol(int argc, VALUE *argv, VALUE self) 
{
	//引数読み込み
	VALUE rtn;
	//初回呼び出し時のみBDDの初期化	
	if(init_cnt==0){ BDDV_Init(256, env_nmax);}
	init_cnt++;

	if(argc>=1){
		int val=power16/2;
		char *to ="bottom";
		char *str;
		VALUE v1,v2,v3;
		rb_scan_args(argc, argv,"12",&v1,&v2,&v3);
		switch(argc){
			case 3:
				if(TYPE(v3)!=T_STRING){
					rb_raise(rb_eRuntimeError,"argument type error (arguments must be STRING)");
				}
				to = RSTRING_PTR(v3);				
			case 2:
				if(TYPE(v2)==T_FLOAT){
					val = (int)( NUM2DBL(v2)*power16);
				}
				else if(TYPE(v2)==T_FIXNUM){
					val =  NUM2INT(v2)*power16;
				}
				else if(TYPE(v2)==T_NIL){
				}				
				else{
					rb_raise(rb_eRuntimeError,"argument type error (arguments must be FLOAT or INT or NIL)");
				}				
			case 1:
				if(TYPE(v1)!=T_STRING){
					rb_raise(rb_eRuntimeError,"argument type error (arguments must be STRING)");
				}
				str = RSTRING_PTR(v1);
				//数値チェック（アイテムが数値なら警告）
				if(env_warning){ num_check(str); }
				break;
			default:
				rb_raise(rb_eRuntimeError,"argument type error");
				break;
		}
		if(*str!='\0'){
			int var = VTable.GetID(str);
			if(var == 0){
				if(!strcmp(to,"top"))	{ VTable.SetT(str, val);}
				else					{ VTable.SetB(str, val);}
			}
		}
		rtn = Qtrue;
	}
	else{
		int n = VTable.Used();
		string str ;
		for(int i=n; i>0; i--)
		{
			int var = BDD_VarOfLev(i);
			str += VTable.GetName(var);
			str += " ";
		}
		rtn = rb_str_new2(str.c_str());
	}
	return rtn;
}

/*##vsop_itemset##*/
VALUE vsop_itemset(int argc, VALUE *argv, VALUE self)
{
	Vsop_Ruby* rmod=new Vsop_Ruby;
	VALUE v;

	//引数読み込み
	rb_scan_args(argc, argv,"10",&v);
	if(TYPE(v)!=T_STRING){
		rb_raise(rb_eRuntimeError,"argument type error (arguments must be STRING)");
	}
	
	char *str = RSTRING_PTR(v);
	if (*str =='\0'){
	 	rmod->cmod = int2ctoi(1);
	}
	else{
		rmod->cmod = string2ctoi(str);
	}

	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod);
}

CtoI * int2ctoi(int val)
{
	//初回呼び出し時のみBDDの初期化
	if(init_cnt==0){ BDDV_Init(256, env_nmax);}
	init_cnt++;
	char wrkval[64];
	sprintf(wrkval,"%d",val);
	CtoI *rtnctoi = new CtoI(CtoI_atoi(wrkval));
	return rtnctoi;
}

/*##vsop_constant##*/
VALUE vsop_constant(int argc, VALUE *argv, VALUE self)
{
	Vsop_Ruby* rmod=new Vsop_Ruby;
	VALUE v;

	//引数読み込み
	rb_scan_args(argc, argv,"10",&v);

	CtoI ctoitmp;
	//定数FIXNUM
	if(TYPE(v)==T_FIXNUM){
		rmod->cmod = int2ctoi(FIX2INT(v));
	}
	//定数BIGNUM
	else if(TYPE(v)==T_BIGNUM){
		rmod->cmod = int2ctoi(NUM2INT(v));
	}
	else{
		rb_raise(rb_eRuntimeError,"argument type error (arguments must be FIXNUM or BIGNUM)");
	}
	VALUE rtn = Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod);
	return rtn;
}
// ------------------------------ パターン長制約を入れたlcm over zdd

VALUE vsop_lcm_nomal_ub(VALUE v1,VALUE v2,VALUE v3,VALUE v4 , VALUE self)
{
	if(init_cnt==0){ BDDV_Init(256, env_nmax);}
	init_cnt++;

	CtoI *ctoi_fin; 
	if(TYPE(v1)!=T_STRING){
		rb_raise(rb_eRuntimeError,"argument type error (1st argument must be STRING)");
	}
	if(TYPE(v2)!=T_STRING){
		rb_raise(rb_eRuntimeError,"argument type error (2nd argument must be STRING)");
	}
	if(TYPE(v3)!=T_FIXNUM){
		rb_raise(rb_eRuntimeError,"argument type error (3rd argument must be FIXNUM)");
	}
	if(TYPE(v4)!=T_FIXNUM){
		rb_raise(rb_eRuntimeError,"argument type error (5th argument must be FIXNUM)");
	}
	char *arg1 = RSTRING_PTR(v1);
	char *arg2 = RSTRING_PTR(v2);
	int arg3_fix = FIX2INT(v3);
	int len_ub = FIX2INT(v4);

	int len;

	len = strlen(arg1);
	char *str_c = new char[len+3];
	sprintf(str_c,"\"%s\"",arg1);
	int len_c = len+2;

	len = strlen(arg2);
	char *str_d = new char[len+3];
	sprintf(str_d,"\"%s\"",arg2);
	int len_d = len+2;

	char *str_e = new char[64];
	sprintf(str_e,"%d",arg3_fix);
	      str_c[len_c - 1] = 0;
      str_d[len_d - 1] = 0;
      int th = atoi(str_e);
           if(strcmp(str_c+1, "F" ) == 0) CtoI_Lcm1_ub(str_d+1, 0, th,  0, len_ub);
      else if(strcmp(str_c+1, "C" ) == 0) CtoI_Lcm1_ub(str_d+1, 0, th,  1, len_ub);
      else if(strcmp(str_c+1, "M" ) == 0) CtoI_Lcm1_ub(str_d+1, 0, th,  2, len_ub);
      else if(strcmp(str_c+1, "FQ") == 0) CtoI_Lcm1_ub(str_d+1, 0, th, 10, len_ub);
      else if(strcmp(str_c+1, "CQ") == 0) CtoI_Lcm1_ub(str_d+1, 0, th, 11, len_ub);
      else if(strcmp(str_c+1, "MQ") == 0) CtoI_Lcm1_ub(str_d+1, 0, th, 12, len_ub); //11 -> 12 by ham
      for(int i=VTable.Used(); i<CtoI_LcmItems(); i++)
      {
	int t = 1;
	char s[32];
	int x = CtoI_LcmPerm(i);
	sprintf(s, "x%d", x);
	while(VTable.GetID(s) != 0)
	{
	  t++;
	  sprintf(s, "x%d_%d", x, t);
	}
	VTable.SetT(s, power16/2);
      }
      CtoI a = CtoI_Lcm2();
      ctoi_fin = new CtoI(a);
      delete[] str_c;
      delete[] str_d;
      delete[] str_e;

	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}


VALUE vsop_lcm_order_ub(VALUE v1,VALUE v2,VALUE v3,VALUE v4 ,VALUE v5, VALUE self)
{
	if(init_cnt==0){ BDDV_Init(256, env_nmax);}
	init_cnt++;

	CtoI *ctoi_fin; 
	if(TYPE(v1)!=T_STRING){
		rb_raise(rb_eRuntimeError,"argument type error (1st argument must be STRING)");
	}
	if(TYPE(v2)!=T_STRING){
		rb_raise(rb_eRuntimeError,"argument type error (1st argument must be STRING)");
	}
	if(TYPE(v3)!=T_FIXNUM){
		rb_raise(rb_eRuntimeError,"argument type error (3rd argument must be FIXNUM)");
	}
	if(TYPE(v4)!=T_STRING){
		rb_raise(rb_eRuntimeError,"argument type error (4th argument must be STRING)");
	}
	if(TYPE(v5)!=T_FIXNUM){
		rb_raise(rb_eRuntimeError,"argument type error (5th argument must be FIXNUM)");
	}
	char *arg1 = RSTRING_PTR(v1);
	char *arg2 = RSTRING_PTR(v2);
	int arg3_fix = FIX2INT(v3);
	char *arg4 = RSTRING_PTR(v4);

	int len_ub = FIX2INT(v5);

	int len;

	len = strlen(arg1);
	char *str_c = new char[len+3];
	sprintf(str_c,"\"%s\"",arg1);
	int len_c = len+2;

	len = strlen(arg2);
	char *str_d = new char[len+3];
	sprintf(str_d,"\"%s\"",arg2);
	int len_d = len+2;

	char *str_e = new char[64];
	sprintf(str_e,"%d",arg3_fix);

	len = strlen(arg4);
	char *str_f = new char[len+3];
	sprintf(str_f,"\"%s\"",arg4);
	int len_f = len+2;

	      str_c[len_c - 1] = 0;
      str_d[len_d - 1] = 0;
      int th = atoi(str_e);
      str_f[len_f - 1] = 0;
           if(strcmp(str_c+1, "F" ) == 0) CtoI_Lcm1_ub(str_d+1, str_f+1, th,  0, len_ub);
      else if(strcmp(str_c+1, "C" ) == 0) CtoI_Lcm1_ub(str_d+1, str_f+1, th,  1, len_ub);
      else if(strcmp(str_c+1, "M" ) == 0) CtoI_Lcm1_ub(str_d+1, str_f+1, th,  2, len_ub);
      else if(strcmp(str_c+1, "FQ") == 0) CtoI_Lcm1_ub(str_d+1, str_f+1, th, 10, len_ub);
      else if(strcmp(str_c+1, "CQ") == 0) CtoI_Lcm1_ub(str_d+1, str_f+1, th, 11, len_ub);
      else if(strcmp(str_c+1, "MQ") == 0) CtoI_Lcm1_ub(str_d+1, str_f+1, th, 12, len_ub);
      for(int i=VTable.Used(); i<CtoI_LcmItems(); i++)
      {
	int t = 1;
	char s[32];
	int x = CtoI_LcmPerm(i);
	sprintf(s, "x%d", x);
	while(VTable.GetID(s) != 0)
	{
	  t++;
	  sprintf(s, "x%d_%d", x, t);
	}
	VTable.SetT(s, power16/2);
      }
      CtoI a = CtoI_Lcm2();
      ctoi_fin = new CtoI(a);
      delete[] str_c;
      delete[] str_d;
      delete[] str_e;
      delete[] str_f;


	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}

/*##vsop_lcm##*/
VALUE vsop_lcm(int argc, VALUE *argv, VALUE self)
{
	VALUE rtnval;
	if(argc==3){
		rtnval = vsop_lcm_nomal(argc,argv,self);
	}
	else if(argc==4){
		rtnval = vsop_lcm_order(argc,argv,self);
	}
	else if(argc==5){
		VALUE v1,v2,v3,v4,v5;
		rb_scan_args(argc, argv,"50",&v1,&v2,&v3,&v4,&v5);
		if(TYPE(v4)!=T_STRING || strcmp(RSTRING_PTR(v4),"")==0  ){
			rtnval = vsop_lcm_nomal_ub(v1,v2,v3,v5,self);
		}
		else{
			rtnval = vsop_lcm_order_ub(v1,v2,v3,v4,v5,self);
		}
	}
	else{
		rb_raise(rb_eRuntimeError,"argument size error ");
	}
	return rtnval;
}

// ------------------------------ パターン長制約を入れたlcm over zdd ここまで 2010/02/28

/*##vsop_same##*/
 VALUE vsop_same(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,"10",&v);
	
	CtoI *ctoi_modc = value2ctoi(v);
	if ( *rmod->cmod == *ctoi_modc){
		return Qtrue;
	} 
	else{
		return  Qfalse;
	}
}

/*##vsop_diff##*/
VALUE vsop_diff(int argc, VALUE *argv,VALUE self){
	Vsop_Ruby* rmod;
	VALUE v;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	rb_scan_args(argc, argv,"10",&v);
	
	CtoI *ctoi_modc = value2ctoi(v);
	if ( *rmod->cmod != *ctoi_modc){
		return Qtrue;
	} 
	else{
		return  Qfalse;
	}
}

/*##vsop_print_size##*/
VALUE vsop_print_size( VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	int val = rmod->cmod->Size();
	return INT2NUM(val);
}        

/*##vsop_print_totalsize##*/
VALUE vsop_print_totalsize(VALUE self){
	rb_gc();
	BDD_GC();
	VALUE rtn = INT2NUM(BDD_Used());

	return rtn;
}        

/*##vsop_print_count##*/
VALUE vsop_print_count( VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	auto_ptr<CtoI> auto_p(new CtoI(*rmod->cmod));
	CtoI *ctoitmp = auto_p.get();
	*ctoitmp = ctoitmp -> CountTerms();
	if(*ctoitmp == CtoI_Null())
	{
		*ctoitmp = 0;
		rb_raise(rb_eRuntimeError,"06Memory overflow");
	}
	int slen = ctoitmp->TopDigit() / 3 + 14;
	kgAutoPtr2<char> a_ptr;
	char *s;
	try{
		a_ptr.set(new char[slen]);
		s = a_ptr.get();
	}catch(...){
		rb_raise(rb_eRuntimeError,"memory allocation error");
	}
	ctoitmp -> StrNum10(s);
	VALUE rtn = rb_cstr_to_inum(s, 10, Qfalse);
	return rtn;
}        

/*##vsop_print_density##*/
VALUE vsop_print_density( VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	auto_ptr<CtoI> auto_p(new CtoI(*rmod->cmod));
	CtoI *ctoitmp = auto_p.get();

	*ctoitmp = ctoitmp -> NonZero();
	if(*ctoitmp == CtoI_Null())
	{
		*ctoitmp = 0;
		rb_raise(rb_eRuntimeError,"07Memory overflow");
	}
	int d = Density(ctoitmp -> GetZBDD(), VTable.Used());
	if(d == 0 && *ctoitmp != 0){
		rb_raise(rb_eRuntimeError,"Bit underflow occurred");
	}
	VALUE rtn = rb_float_new((float)d / power30);
	return rtn;
}

/*##vsop_print_value##*/
VALUE vsop_print_value( VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	auto_ptr<CtoI> auto_p(new CtoI(*rmod->cmod));
	CtoI *ctoitmp = auto_p.get();
	
	if(OVF.Check()!= 0){
		rb_raise(rb_eRuntimeError,"Bit overflow occurred");
	}
	VALUE rtn = rb_float_new((float)Value(*ctoitmp)/power16);
	return rtn;
}        

/*##vsop_print_maxcover##*/
VALUE vsop_print_maxcover(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);

	CtoI *ctoi_fin; 

	auto_ptr<CtoI> auto_p(new CtoI(*rmod->cmod));
	CtoI *ctoitmp = auto_p.get();
	
	*ctoitmp = ctoitmp -> NonZero();
	if(*ctoitmp == CtoI_Null())
	{
		*ctoitmp = 0;
		ctoi_fin = new CtoI(0);
		rb_raise(rb_eRuntimeError,"08Memory overflow");
	}
	if(*ctoitmp == 0){
		ctoi_fin = new CtoI(0);
	}
	else
	{
		ZBDD f = ctoitmp -> GetZBDD();
		if(MaxCost(f)==0){
			ctoi_fin = new CtoI(1);
		}
		else
		{
			CtoI ctmp(1);
			while(1)
			{
				int var = f.Top();
				if(var == 0) break;
				ZBDD f0 = f.OffSet(var);
				ZBDD f1 = f.OnSet0(var);
				int c1 = MaxCost(f1) + VTable.GetValue(var);
				if(MaxCost(f0) < c1)
				{
					f = f1;
					ctmp = ctmp.AffixVar(var);
				}
				else f = f0;
			}
			ctoi_fin = new CtoI(ctmp);
		}
	}

	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}        

/*##vsop_print_maxcost##*/
VALUE vsop_print_maxcost( VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	auto_ptr<CtoI> auto_p(new CtoI(*rmod->cmod));
	CtoI *ctoitmp = auto_p.get();

	*ctoitmp = ctoitmp -> NonZero();
	if(*ctoitmp == CtoI_Null())
	{
		*ctoitmp = 0;
		rb_raise(rb_eRuntimeError,"09Memory overflow");
	}
	VALUE rtn;
	if(*ctoitmp == 0){
		rtn = 	Qnil;
	}
	else
	{
		int c = MaxCost(ctoitmp -> GetZBDD());
		rtn = rb_float_new((float)c/power16);
	}
	return rtn;
}        

/*##vsop_print_mincover##*/
VALUE vsop_print_mincover( VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);

	CtoI *ctoi_fin; 

	auto_ptr<CtoI> auto_p(new CtoI(*rmod->cmod));
	CtoI *ctoitmp = auto_p.get();
	
	*ctoitmp = ctoitmp -> NonZero();
	if(*ctoitmp == CtoI_Null())
	{
		*ctoitmp = 0;
		ctoi_fin = new CtoI(0);
		rb_raise(rb_eRuntimeError,"10Memory overflow");
	}
	if(*ctoitmp == 0){
		ctoi_fin = new CtoI(0);
	}
	else
	{
		ZBDD f = ctoitmp -> GetZBDD();
		if(MinCost(f)==0){
			ctoi_fin = new CtoI(1);
		}
		else
		{
			CtoI ctmp(1);
			while(1)
			{
				int var = f.Top();
				if(var == 0) break;
				ZBDD f0 = f.OffSet(var);
				ZBDD f1 = f.OnSet0(var);
				int c1 = MinCost(f1) + VTable.GetValue(var);
				if(MinCost(f0) > c1)
				{
					f = f1;
					ctmp = ctmp.AffixVar(var);
				}
				else f = f0;
			}
			ctoi_fin = new CtoI(ctmp);
		}
	}
	Vsop_Ruby *rmod_rtn = new Vsop_Ruby;
	rmod_rtn->cmod = ctoi_fin;
	return Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod_rtn);
}        

/*##vsop_print_mincost##*/
VALUE vsop_print_mincost( VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	auto_ptr<CtoI> auto_p(new CtoI(*rmod->cmod));
	CtoI *ctoitmp = auto_p.get();
	
	*ctoitmp = ctoitmp -> NonZero();
	if(*ctoitmp == CtoI_Null())
	{
		*ctoitmp = 0;
		rb_raise(rb_eRuntimeError,"11Memory overflow");
	}
	VALUE rtn;
	if(*ctoitmp == 0){
		rtn = Qnil;
	}
	else
	{
		int c = MinCost(ctoitmp -> GetZBDD());
		rtn = rb_float_new((float)c/power16);
	}
	return rtn;
}        

/*##vsop_print_decomp##*/
VALUE vsop_print_decomp(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	VALUE *v;
	rb_scan_args(argc, argv,"01",&v);
	if(argc!=0){
		if(TYPE(v)==T_STRING){
			char *str = RSTRING_PTR(v);
			bout.Set(str);
		}
		else{
			rb_raise(rb_eRuntimeError,"argument type error (arguments must be STRING)");
		}
	}

	auto_ptr<CtoI> auto_p(new CtoI(*rmod->cmod));
	CtoI *ctoitmp = auto_p.get();

	if(PrintDecomp(*ctoitmp) == 1)
	{
		rb_raise(rb_eRuntimeError,"12Memory overflow");
	}
	bout.Unset();
	return self;
}        

/*##vsop_print_export##*/
VALUE vsop_print_export(int argc, VALUE *argv, VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);
	auto_ptr<CtoI> auto_p(new CtoI(*rmod->cmod));
	CtoI *ctoitmp = auto_p.get();
	FILE *fp=NULL;
	if(argc==1){
		VALUE v1;
		rb_scan_args(argc, argv,"0*",&v1);
		if(TYPE(RARRAY_PTR(v1)[0])==T_STRING){
				char *str = RSTRING_PTR(RARRAY_PTR(v1)[0]);
				fp = fopen(str, "w");
				if(fp == NULL){
					rb_raise(rb_eRuntimeError,"Can't open the file");		
				}
		}
		else{
			rb_raise(rb_eRuntimeError,"argument type error (1st argument must be STRING)");
		}
	}
	else if(argc!=0){
		rb_raise(rb_eRuntimeError,"number of argument is 0 or 1 ");		
	}

	int d = ctoitmp -> TopDigit();
	ZBDDV v = ZBDDV();
	for(int i=0; i<=d; i++){
		v += ZBDDV(ctoitmp -> Digit(i).GetZBDD(), i);
	}

	if(fp){
		v.Export(fp);
		fclose(fp);
	}else{
		v.Export();
	}
	return self;
}        


static char* show_use_para[][2]={
 {"hex","/hex"},
 {"bit", "/bit"}, 
 {"case", "/case"},
 {"map", "/map"},
 {"rmap", "/rmap"},
 {"decomp", "/decomp"},
 {"",""}
};

static char* show_notuse_para[]={
 "size", "count", "density", "value", "maxcover", "maxcost", "mincover",
 "mincost", "plot","decompd", "imply0","symmetry", 
 "imply", "coimply0", "coimply2", "coimply","export",
 "/size", "/count", "/density", "/value", "/maxcover",
 "/maxcost", "/mincover", "/mincost","/plot", 
  "/decompd", "/imply0", "/symmetry", "/imply",
 "/coimply0", "/coimply2", "/coimply", "/export",""
};

/*##vsop_show##*/
VALUE vsop_show(int argc, VALUE *argv, VALUE self){
	// bit,hex,map,rmap,case,decompのみ動くようにする
	VALUE rtnval;
	if(argc == 0){
		rtnval = vsop_print(self);
	}
	else if(argc == 1)	{
		VALUE v;
		rb_scan_args(argc, argv,"10",&v);
		if(TYPE(v)!=T_STRING){
			rb_raise(rb_eRuntimeError,"argument type error (1st argument must be STRING)");
		}
		char *argtmp = RSTRING_PTR(v);
		for(size_t i=0;*show_use_para[i][0]!='\0';i++){
			if( !strcmp(argtmp,show_use_para[i][0])|| 
				!strcmp(argtmp,show_use_para[i][1])){
				rtnval = vsop_print_arg1(self,show_use_para[i][0]);
				return rtnval;
			}		
		}
		bool output = true;
		for(const char *p=*show_notuse_para ;*p!='\0';p++){
			if( !strcmp(argtmp,p)){
				output=false;
				break;
			}
		}
		if(output){
			rtnval = vsop_print_arg1(self,argtmp);
			return rtnval;
		}
		rb_raise(rb_eRuntimeError,"Illegal switch(/bit,/hex,/map,/rmap,/case,/decomp)");
	}
	else	if(argc == 2)
	{
		VALUE v1,v2;
		rb_scan_args(argc, argv,"20",&v1,&v2);
		char *argtmp = RSTRING_PTR(v1);
		for(size_t i=0;*show_use_para[i][0]!='\0';i++){
			if( !strcmp(argtmp,show_use_para[i][0])|| 
				!strcmp(argtmp,show_use_para[i][1])){
				rtnval = vsop_print_arg2(argc,argv,self);
				return rtnval;
			}		
		}
		rb_raise(rb_eRuntimeError,"parameter error(/bit,/hex,/map,/rmap,/case,/decomp)");
	}
	else{
		rb_raise(rb_eRuntimeError,"number of argument is 0 or 1 or 2");
	}
	return rtnval;
}

/*##vsop_partly##*/
VALUE vsop_partly(VALUE self){
	return rb_iv_get(self,"@partly");
}

//強制変換用
VALUE vsop_coerce(int argc, VALUE *argv, VALUE self){
	VALUE v;
	Vsop_Ruby* rmod=new Vsop_Ruby;
	rb_scan_args(argc, argv,"10",&v);
	rmod->cmod =value2ctoi(v);
	VALUE rtn_v = Data_Wrap_Struct(rb_class_of(self),0,free_rmod,rmod);
	return rb_ary_new3(2,rtn_v,self);
}

/*##vsop_const_to_i##*/
VALUE vsop_const_to_i(VALUE self){
	Vsop_Ruby* rmod;
	Data_Get_Struct(self,Vsop_Ruby,rmod);

	CtoI a = *(rmod->cmod);
	
	if(a.IsConst()){
		if(a.TopItem() > 0) a = a.MaxVal();
		int d = a.TopDigit() / 3 + 14;
		kgAutoPtr2<char> a_ptr;
		char *s;
		try{
			a_ptr.set(new char[d]);
			s = a_ptr.get();
		}catch(...){
			rb_raise(rb_eRuntimeError,"memory allocation error");
		}
	  int err = a.StrNum10(s);
	  if (err) { rb_raise(rb_eRuntimeError,"StrNum10 error"); }
		return rb_cstr2inum(s,10);
	}else{
		return Qnil;
	}
}

void Init_zdd_so() {
/*##zdd##*/

	//環境変数取得
	char *envStr;
	envStr=getenv("ZDDLimitNode");
	if(envStr!=NULL){
		env_nmax=atoi(envStr);
	}else{
		env_nmax=DEF_ENV_NMAX ;
	}
	envStr=getenv("ZDDWarning");
	if(envStr!=NULL){
		env_warning=true;
	}else{
		env_warning=false;
	}
	

	VALUE vsop_main=rb_define_module("ZDD");

	rb_iv_set(vsop_main,"@partly", Qfalse );

	//モジュール変数設定
	rb_define_module_function(vsop_main,    "itemset", RUBY_METHOD_FUNC(vsop_itemset), -1);
	rb_define_module_function(vsop_main,   "constant", RUBY_METHOD_FUNC(vsop_constant), -1);
	rb_define_module_function(vsop_main,     "symbol", RUBY_METHOD_FUNC(vsop_symbol), -1);
	rb_define_module_function(vsop_main,       "each", RUBY_METHOD_FUNC(vsop_each), 0);
	rb_define_module_function(vsop_main,  "each_item", RUBY_METHOD_FUNC(vsop_each_item), 0);

	rb_define_module_function(vsop_main,       "show", RUBY_METHOD_FUNC(vsop_show), -1);
	rb_define_module_function(vsop_main,        "lcm", RUBY_METHOD_FUNC(vsop_lcm), -1);
	rb_define_module_function(vsop_main,     "permit", RUBY_METHOD_FUNC(vsop_permit), -1);
	rb_define_module_function(vsop_main,  "permitsym", RUBY_METHOD_FUNC(vsop_permitsym), -1);
	rb_define_module_function(vsop_main,   "restrict", RUBY_METHOD_FUNC(vsop_restrict), -1);
	rb_define_module_function(vsop_main,   "freqpatC", RUBY_METHOD_FUNC(vsop_freqpatC), -1);
	rb_define_module_function(vsop_main,   "freqpatM", RUBY_METHOD_FUNC(vsop_freqpatM), -1);
	rb_define_module_function(vsop_main,   "freqpatA", RUBY_METHOD_FUNC(vsop_freqpatA), -1);
	rb_define_module_function(vsop_main,    "termsLE", RUBY_METHOD_FUNC(vsop_termsLE), -1);
	rb_define_module_function(vsop_main,    "termsLT", RUBY_METHOD_FUNC(vsop_termsLT), -1);
	rb_define_module_function(vsop_main,    "termsGE", RUBY_METHOD_FUNC(vsop_termsGE), -1);
	rb_define_module_function(vsop_main,    "termsGT", RUBY_METHOD_FUNC(vsop_termsGT), -1);
	rb_define_module_function(vsop_main,    "termsNE", RUBY_METHOD_FUNC(vsop_termsNE), -1);
	rb_define_module_function(vsop_main,    "termsEQ", RUBY_METHOD_FUNC(vsop_termsEQ), -1);

	rb_define_module_function(vsop_main,         "<=", RUBY_METHOD_FUNC(vsop_le), -1);
	rb_define_module_function(vsop_main,          "<", RUBY_METHOD_FUNC(vsop_lt), -1);
	rb_define_module_function(vsop_main,         ">=", RUBY_METHOD_FUNC(vsop_ge), -1);
	rb_define_module_function(vsop_main,          ">", RUBY_METHOD_FUNC(vsop_gt), -1);
	rb_define_module_function(vsop_main,        "ne?", RUBY_METHOD_FUNC(vsop_ne), -1);
	rb_define_module_function(vsop_main,         "==", RUBY_METHOD_FUNC(vsop_eq), -1);
	rb_define_module_function(vsop_main,        "iif", RUBY_METHOD_FUNC(vsop_iif), -1);
	rb_define_module_function(vsop_main,       "meet", RUBY_METHOD_FUNC(vsop_meet), -1);
	rb_define_module_function(vsop_main,      "same?", RUBY_METHOD_FUNC(vsop_same), -1);
	rb_define_module_function(vsop_main,        "===", RUBY_METHOD_FUNC(vsop_same), -1);	
	rb_define_module_function(vsop_main,      "diff?", RUBY_METHOD_FUNC(vsop_diff), -1);
	rb_define_module_function(vsop_main,      "delta", RUBY_METHOD_FUNC(vsop_delta), -1);

	rb_define_module_function(vsop_main,          "+", RUBY_METHOD_FUNC(vsop_plus), -1);
	rb_define_module_function(vsop_main,          "-", RUBY_METHOD_FUNC(vsop_minus), -1);
	rb_define_module_function(vsop_main,         "+@", RUBY_METHOD_FUNC(vsop_plus_op), 0);
	rb_define_module_function(vsop_main,         "-@", RUBY_METHOD_FUNC(vsop_minus_op), 0);
	rb_define_module_function(vsop_main,          "*", RUBY_METHOD_FUNC(vsop_multiply), -1);
	rb_define_module_function(vsop_main,          "/", RUBY_METHOD_FUNC(vsop_quotiment), -1);
	rb_define_module_function(vsop_main,          "%", RUBY_METHOD_FUNC(vsop_remainder), -1);
	rb_define_module_function(vsop_main,     "csvout", RUBY_METHOD_FUNC(vsop_csvout), -1);
	rb_define_module_function(vsop_main,     "import", RUBY_METHOD_FUNC(vsop_import), -1);
	rb_define_module_function(vsop_main,    "hashout", RUBY_METHOD_FUNC(vsop_hashout), 0);
	rb_define_module_function(vsop_main,  "maxweight", RUBY_METHOD_FUNC(vsop_maxval), 0);
	rb_define_module_function(vsop_main,  "minweight", RUBY_METHOD_FUNC(vsop_minval), 0);
	rb_define_module_function(vsop_main,"totalweight", RUBY_METHOD_FUNC(vsop_totalval), 0);
	rb_define_module_function(vsop_main,      "items", RUBY_METHOD_FUNC(vsop_items), 0);
	rb_define_module_function(vsop_main,     "symgrp", RUBY_METHOD_FUNC(vsop_symgrp), 0);

	rb_define_module_function(vsop_main,       "size", RUBY_METHOD_FUNC(vsop_print_size), 0);
	rb_define_module_function(vsop_main,  "totalsize", RUBY_METHOD_FUNC(vsop_print_totalsize), 0);
	rb_define_module_function(vsop_main,      "count", RUBY_METHOD_FUNC(vsop_print_count), 0);
	rb_define_module_function(vsop_main,    "density", RUBY_METHOD_FUNC(vsop_print_density), 0);
	rb_define_module_function(vsop_main,       "cost", RUBY_METHOD_FUNC(vsop_print_value), 0);
	rb_define_module_function(vsop_main,   "maxcover", RUBY_METHOD_FUNC(vsop_print_maxcover), 0);
	rb_define_module_function(vsop_main,    "maxcost", RUBY_METHOD_FUNC(vsop_print_maxcost), 0);
	rb_define_module_function(vsop_main,   "mincover", RUBY_METHOD_FUNC(vsop_print_mincover), 0);
	rb_define_module_function(vsop_main,    "mincost", RUBY_METHOD_FUNC(vsop_print_mincost), 0);
	rb_define_module_function(vsop_main,     "export", RUBY_METHOD_FUNC(vsop_print_export), -1);
	rb_define_module_function(vsop_main,     "partly", RUBY_METHOD_FUNC(vsop_partly), 0);
	rb_define_module_function(vsop_main,     "coerce", RUBY_METHOD_FUNC(vsop_coerce), -1);
	rb_define_module_function(vsop_main,       "to_i", RUBY_METHOD_FUNC(vsop_const_to_i), 0);
	rb_define_module_function(vsop_main,       "to_a", RUBY_METHOD_FUNC(vsop_to_a), 0);
	rb_define_module_function(vsop_main,       "to_s", RUBY_METHOD_FUNC(vsop_to_s), 0);
	rb_define_module_function(vsop_main,    "inspect", RUBY_METHOD_FUNC(vsop_to_s), 0);

}
