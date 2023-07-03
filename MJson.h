#include<iostream>
#include<cmath>
#include<cstring>
#include<fstream>
#include<stack>
using namespace std;
typedef struct MTree{
	struct MTree *Chl,*Brt,*Rt,*Lst;//4
	int _type,tag,num_db;
	char* name; 
	union Data{
		char* _Strval;//0
		double _Dbval;//1
		long long _intval;//2
	}data;
}mtree;
int NumwriteInt(long long n,char* &ptr){
	if(n == 0){
		ptr = new char[2];
		ptr[0] = '0';
		ptr[1] = '\0';
		return 1;
	}
	int num = 0,f = 1;
	long long t = n;
	if(t <0){t*= -1;num++;f=-1;}
	while(t>0){
		t/= 10;num++;
	}
	ptr = new char[num];
	int cnt = num-1;
	if(f == -1){
		ptr[0] = '-';
		n *= -1;
	}
	while(n>0){
		ptr[cnt--] = (n%10) + '0';n/= 10;
	}
	ptr[num] = '\0';
	return num;
}
int NumwriteDouble(double n,char* &ptr,int num){
	ptr = new char[num];
	n = n*10;
	int cnt = 0;
	while(cnt < num){
		ptr[cnt++] = int(n) + '0';
		n = n - int(n);
		n*= 10;
	}
	ptr[num] = '\0';
	
	return num;
}
class Str{
	private:
		char* _val;//Chinese range from 0xB0A1 to 0xF7FE
		long long _num;
	public:
		void setNum(int num){
			_num = num;
		}
		int Getsize(){
			return _num;
		}
		char* GetVal(){
			return _val;
		}
		/*char* Print(){
			//Example 
			return _val;
		}*/
		Str(char* val="",int num=0):_val(val),_num(num){
			
		}
		void Print(){ 
			char *ptr = _val;
			int cnt = 0;
			while(*ptr != '\0' && cnt < _num){
			putchar(*ptr);
			ptr++;cnt++;
			}
		}
		void setval(char* val){
			_val = val;
		}
		operator char*(){
			return _val;
		}
		
		Str operator+(const Str& A) const;
		Str operator+(const char A) const;
};
int GetChS(char* T){ 
	char *ptr = T;
	int cnt = 0;
	while(*ptr != '\0'){
		cnt++,ptr++;
	}
	return cnt;
}
Str Str::operator+(const Str& A) const
{
	int nums = _num + A._num;
	char* T = new char[nums];
	strcpy(T,_val);
	strcat(T,A._val);
	return Str(T,nums);
}
Str Str::operator+(char A) const
{
	int nums = _num + 1;
	char* T = new char[nums];
	strcpy(T,_val);
	T[nums] = A;
	return Str(T,nums);
}
class MJson{
	private:
		mtree _oval; 
		Str _ival;
		const char* _FileName;
	public:
		MJson(const char* filename):_FileName(filename){
		}
		void setoval(mtree tag){
			_oval = tag;
		}
		Str MJsRead(){
			ifstream ifs(_FileName,ios::in);
			if(!ifs) return Str("",0);
			ifs.seekg(0,ios::end);
			int num = ifs.tellg();
			ifs.seekg(0,ios::beg);
			char *T = new char[num];
			ifs.read(T,num);
			Str target(T,num);
			ifs.close();
			return target;
		}
		int MJsRead_Char(mtree* &T,char* ptr){
			int cnt = 0;
			while(*ptr != '"' && *(ptr+1) != ',') cnt++,ptr++;
			ptr-= cnt;char* _name = new char[cnt+1];
			while(*ptr != '"' && *(ptr+1) != ','){
				*_name = *ptr;
				ptr++,_name++;
			}
			*_name = '\0';
			_name-= cnt;
			T->_type = 0;
			T->data._Strval = _name;
			return cnt+1;
		}
		int MJsRead_Struct(mtree* &T,char* ptr,stack<mtree* > &deepth){
			deepth.push(T);
			T->_type = 4;
			T->Brt = NULL;T->Chl = NULL;
			T->Chl = new mtree;
			T = T->Chl;T->Rt = T;
			T->Lst= NULL;
			T->Brt = NULL;T->Chl = NULL;
			T->tag = 1;
			return 0;
		}
		int MJsRead_Name(mtree* &T,char* ptr,stack<mtree* > &deepth){
			if(T->tag == 1){
				T->tag = 2;
				//判断是否需要新开辟空间 
			}else{
				//cout<<"rrr";
				T->Brt = new mtree;
				(T->Brt)->Lst = T;
				(T->Brt)->Rt = T->Rt;
				T = T->Brt;T->Chl = NULL;T->Brt = NULL;
				T->tag = 0;
			}
			int cnt = 0;
			while(*ptr != '"') cnt++,ptr++;
			ptr-= cnt;char* _name = new char[cnt+1];
			while(*ptr != '"'){
				*_name = *ptr;
				ptr++,_name++;
			}
			*_name = '\0';
			_name-= cnt;
			T->name = _name;
			return cnt+1;
		}
		int MJsRead_Number(mtree* &T,char* ptr){
			long long cnt = 0;
			int I = 0, f =1;
			double Db = 0;
			int tag = 0;//小数位数 
			while(*ptr != ','){
				if(*ptr == '-'){
					ptr++;
					f*= -1;
				}
				if(*ptr == '.'){
					ptr++;
					tag = 1;
					cnt++;
				}
				if(tag == 0){
					I = I*10 + *ptr - '0';
				}else{
					double KB = 1;
					KB = pow(10,tag);
					Db = Db + double(*ptr - '0')/KB;
					tag++;
					//cout<<Db<<"	";
				}
				cnt++;
				ptr++;
			}
			if(tag == 0){
				T->_type = 2;
				T->data._intval = f*I;
			}else{
				T->_type = 1;
				T->data._Dbval = f* (double(I)+Db);
				T->num_db = tag - 1;
			}
			
			return cnt+1;
		}
		bool MJsGet(){
			mtree T;mtree* Ptr;
			//if(MJsRead().Getsize() == 0) return false;
			T.Rt = &T; T.Brt = NULL;T.Chl = NULL;
			T._type = 4;
			T.num_db = 5;
			T.Lst = NULL;
			T.tag = 1;
			T.name = "NULL";
			T.data._Strval = "NULL"; 
			Ptr = &T;
			Str MSt = MJsRead();
			_ival.setval(MSt);
			_ival.setNum(MSt.Getsize());
			int State = -1;
			stack<mtree* > deepth; 
			//char NuM[4][20] = {"Char_Read","Struct_Read","Struct_name","number_Read"};
			//0:Char_Read(begin: :"  end: ", ) 1:Struct_Read(begin: :{  end:}, ) 2:Struct_name(begin: " end: ":) 3:number_Read(begin "  end ,)
			char* ptr = MSt;
			int mend = MSt.Getsize();
			int pos = 0;
			while(*ptr != '\0' && pos < mend){
				State = -1;
				int skip = 1;
				if(*ptr == ':'){
					if(*(ptr+1) == '{') State= 1;
					if(*(ptr+1) == '"') State= 0;
					skip = 2;
					if(*(ptr+1) != '{' && *(ptr+1) != '"'){ State= 3;skip = 1;}
				}
				if(*ptr == '"') State=2;
				if(*ptr == '}' && *(ptr+1) == ','){
					if(*(ptr+1) == ',' && !deepth.empty()){
					Ptr = deepth.top(); 
					deepth.pop();
					} 
					//cout<<"out"<<endl;

					skip = 2;
				}
				//cout<<pos<<": "<<*ptr<<"  ";
				//cout<<"State:"<<State<<endl; 
				ptr+= skip;
				//cout<<"skip:"<<skip<<endl;
				pos+= skip;
				skip = 0;
				switch(State){
					case 0:{
						skip = MJsRead_Char(Ptr,ptr);
						//cout<<"skip:"<<skip<<endl;
						break;
					}
					case 1:{
						skip = MJsRead_Struct(Ptr,ptr,deepth);
						//cout<<"skip:"<<skip<<endl;
						break;
					}
					case 2:{
						skip = MJsRead_Name(Ptr,ptr,deepth);
						//cout<<"skip:"<<skip<<endl;
						break;
					}
					case 3:{
						skip = MJsRead_Number(Ptr,ptr);
						//cout<<"skip:"<<skip<<endl;
						break;
					}
				}
			ptr+= skip;
			pos+= skip;
		}
		//cout<<pos<<" "<<MJsRead().Getsize()<<endl;
			_oval = T;
			return true;
		}
		mtree* oVal(){
			return &this->_oval;
		}
		bool MJsWrite(Str Ot){
			//_FileName
			ofstream ofs(_FileName,ios::out);
			if(!ofs) return false;
			ofs.write(Ot,Ot.Getsize());
			ofs.close();
			return true;
		}
		bool MJsSave(){
			ofstream ofs(_FileName,ios::out);
			if(!ofs) return false;
			bool c = ToStr(&_oval,ofs);
			ofs.close();
			return c;
		}
		bool ToStr(mtree *Top,ofstream& ofs){
			//Str Target;
			mtree *Ptr = Top;
			while(Ptr != NULL){
				//Target = Target + Str("\"",1);
				ofs.put('"');//putchar('"'); 
				//Target = Target + Str(Ptr->name,strlen(Ptr->name));
				ofs.write(Ptr->name,strlen(Ptr->name));//cout<<Ptr->name;
				//Target = Target + Str("\":",2);
				ofs.put('"');ofs.put(':');//putchar('"');putchar(':');
				switch(Ptr->_type){//0:char  1:double  2:int  4:Struct
					case 0:{
						//cout<<"0: ";
						//Target = Target + Str("\"",1);
						ofs.put('"');//putchar('"');
						//Target = Target + Str(Ptr->data._Strval,strlen(Ptr->data._Strval));
						ofs.write(Ptr->data._Strval,strlen(Ptr->data._Strval));//cout<<Ptr->data._Strval;
						//Target = Target + Str("\",\n",3);
						ofs.put('"');ofs.put(',');ofs.put('\n');//putchar('"');putchar(',');putchar('\n');
						//cout<<Target;
						break;
					}
					case 1:{
						//cout<<"1:";
						char* Int_;
						NumwriteInt(Ptr->data._Dbval,Int_);
						char* Double_;
						NumwriteDouble(Ptr->data._Dbval - int(Ptr->data._Dbval),Double_,Ptr->num_db);
						//Target = Target + Str(Int_,strlen(Int_));
						ofs.write(Int_,strlen(Int_));//cout<<Int_;
						//Target = Target + Str(".",1);
						ofs.put('.');//putchar('.');
						//Target = Target + Str(Double_,strlen(Double_));
						//Target = Target + Str(",\n",2);
						ofs.write(Double_,strlen(Double_));
						ofs.put(',');ofs.put('\n');//putchar(',');putchar('\n');
						//cout<<Target;
						break;
					}
					case 2:{
						//cout<<"2:";
						char* Int_ = new char[20];
						NumwriteInt(Ptr->data._intval,Int_);
						//Target = Target + Str(Int_,strlen(Int_));
						ofs.write(Int_,strlen(Int_));//cout<<Int_;
						//Target = Target + Str(",\n",2);
						ofs.put(',');ofs.put('\n');//putchar(',');putchar('\n');
						//cout<<Target;
						break;
					}
					case 4:{
						//cout<<"4:";
						//Target = Target + Str("{\n",2);
						ofs.put('{');ofs.put('\n');//putchar('{');putchar('\n');
						//Target = Target + ToStr(Ptr->Chl);
						ToStr(Ptr->Chl,ofs);
						//Target = Target + Str("},\n",3);
						ofs.put('}');ofs.put(',');ofs.put('\n');//putchar('}');putchar(',');putchar('\n');
						//cout<<Target;
						break;
					}
				}
				//cout<<Target;
				Ptr = Ptr->Brt;
			}
			//cout<<Target;
			return true;
		}
		char* iVal(){
			return this->_ival;
		}
};


class MTree_Cp{
	private:
		mtree _oval;
	public:
		MTree_Cp(mtree oval):_oval(oval){
			
		}
		MTree_Cp(){
			
		}
		void getnext(char* t,int* next){
			int tlen = strlen(t);
			next[0] = -1;int j=0,k = -1;
			while(j<tlen){
				if(k== -1 || t[j] == t[k]){
					if(t[j+1] == t[k+1]){
						next[j+1] = next[k+1];
					}else{
						next[j+1] = k+1;
					}
					j++,k++;
				}else k = next[k];
			}
		}
		void SetName(mtree* Ptr,char* name){
			Ptr->name = name;
		}
		int kmp(char* t,char* s){
			int next[250];
			getnext(t,next);
			int tlen = strlen(t),slen = strlen(s);
			int i =0,j = 0;
			while(i<slen && j<tlen){
				if(j==-1 || s[i] == t[j])
				i++,j++;
				else
				j = next[j];
			}
			if(j >= tlen) return i-tlen+1;
			else return -1;
		}
		template<typename T>
		mtree* MJsAdd(mtree* there,char* Mname,int mode,T data = 0,int Num = 10){
			//0 Str 1 Db 2 In 4S Struct
				mtree* temp = there->Brt;
				there->Brt = new mtree;
				if(temp)(there->Brt)->Brt = temp;
				else there->Brt->Brt = NULL;
				there = there->Brt;there->Chl = NULL;
				if(temp) temp->Lst = there;
				there->num_db = Num;
				there->name = Mname;
				there->_type = mode;
				if(mode == 0) SetSVal(there,data);
				if(mode == 1) SetDVal(there,data);
				if(mode == 2) SetIVal(there,data);
				if(mode == 4){
					there->Chl = new mtree;
					there->Chl->Brt = NULL;
					there->Chl->Rt = there->Chl;
					there->Chl->name = "NULL";
					there->Chl->data._Strval = "NULL";
					there->Chl->Lst = NULL;
					there->Chl->Chl = NULL;
				}
				return there;
		}
		mtree* GetMtr(){
			return &_oval;
		}
		void SetSVal(mtree* there,char* data){
			there->data._Strval = data;
		}
		void SetSVal(mtree* there,const char* data){
			char* T = new char[strlen(data)];
			strcpy(T,data);
			there->data._Strval = T;
		}
		void SetSVal(mtree* there,int data){
			there->data._intval = data;
		}
		void SetIVal(mtree* there,int data){
			there->data._intval = data;
		}
		void SetIVal(mtree* there,const char* data){
			char* T = new char[strlen(data)];
			strcpy(T,data);
			there->data._Strval = T;
		}
		void SetDVal(mtree* there,double data){
			there->data._Dbval = data;
		}
		void SetDVal(mtree* there,const char* data){
			char* T = new char[strlen(data)];
			strcpy(T,data);
			there->data._Strval = T;
		}
		mtree* MJsDel(mtree* there,mtree* top){
			mtree* T = top;
			MJsSwap(top,there);
			
			there->Brt->Lst = NULL;
			there->Brt->Rt = there->Brt; 
			T = there->Brt;
			return T;
		}
		template<typename T>
		mtree* MJsChange(mtree* there,char* name ,T data,int mode,int Num =10){
			if(name != "") there->name = name;
			there->num_db = Num;
			there->_type = mode;
			if(there->Chl) delete there->Chl;
			if(mode == 0) SetSVal(there,data);
			if(mode == 1) SetDVal(there,data);
			if(mode == 2) SetIVal(there,data);
			if(mode == 4){
				there->Chl = NULL;
				there->Chl = new mtree;
				there->Chl->_type = 0;
				there->Chl->name = "NULL";
				there->Chl->Brt = NULL;
				there->Chl->Rt = there->Chl;
				there->Chl->data._Strval = "NULL";
				there->Chl->Lst = NULL;
				there->Chl->Chl = NULL;
			}
			return there;
		}
		mtree* MJsFind(mtree* top,char* wtd,int mode){
			//mode1 ALL mode2 Just there
			while(top != NULL){
				if(strcmp(top->name,wtd) == 0 ){
					return top;
					break;
				}
				if(top->Chl != NULL && mode == 1){
					mtree* result = MJsFind(top->Chl,wtd,mode);
					if(result) return result;
				}
				top = top->Brt;
			}
		    return NULL;
		}
		char* MJsGetValStr(mtree* Ptr){
			if(Ptr->_type == 0)	return Ptr->data._Strval;
			else return "NULL";}
		long long MJsGetValInt(mtree* Ptr){
			if(Ptr->_type == 2) return Ptr->data._intval;
			else return 0;}
		double MJsGetValDb(mtree* Ptr){
			if(Ptr->_type == 1) return Ptr->data._Dbval;
			else return 0.0;}
		mtree* MJsGetValSt(mtree* Ptr){
			if(Ptr->_type == 4) return Ptr->Chl;
			else return NULL;}
		void MJsSwap(mtree* a,mtree* b){
			mtree Temp = *b;
			b->_type = a->_type;b->name = a->name;b->data = a->data;b->tag = a->tag;b->num_db = a->num_db;
			a->_type = Temp._type;a->data = Temp.data;a->tag = Temp.tag;a->name = Temp.name;a->num_db = Temp.tag;
		}
};
