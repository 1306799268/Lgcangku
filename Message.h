#include"MJson.h"
#include"Time.h"
#include<set>
#include<vector> 
class Book{
	private:
		char* _FileName;//储存路径 
		char* Name,*author,*ISBN_number,*publisher;//基本信息 
		int type;//书的类型  1:期刊、2:杂志、3:书籍
		int laber;//1.小说 2.漫画绘本 3.青春 4.推理 5.短篇集 6.历史 7.国风文化 8.兴趣培养 9.经典 10.精品课 11.专业 12.理财 13.外语 14.恋爱 15.技能
		//16.艺术 17.哲学 18.心理 19. 传记 20.诗歌戏剧 21散文 22科普 23影视原著 24互联网 25亲子 26儿童文学 27励志 28理想国 
		char* begin,*end;//记录借阅时长
	public:
		 Book(char* FileName):_FileName(FileName){
		 Name = "NULL";
		 author = "NULL";
		 ISBN_number = "NULL";
		 publisher = "NULL";
		 type = 0;laber  = 0; 
		 begin = "2000-01-01",end = "2000-01-01";
		 }
		 void Setmessage(char* name,char* mauthor,char* ISBN,char* publish){
		 	Name = name,author = mauthor,ISBN_number = ISBN,publisher = publish;
		 } 
		 void TimeSet(char* beginT,char* endT){
		 	begin = beginT,end = endT;
		 }
		 void LaberSet(int val,int val2){
		 	type = val;laber = val2;
		 }
		 void BookSaves(){
		 	MJson T(_FileName);
		 	T.MJsGet();
		 	mtree* Top = T.oVal();
			MTree_Cp t;
		 	if(Top->name == "NULL"){
			t.MJsChange(Top,Name,"",4);
			}else{
			Top = t.MJsAdd(Top,Name,4,0,10);
			}
			Top = Top->Chl;
			t.MJsChange(Top,"author",author,0);
			Top = t.MJsAdd(Top,"ISBN",0,ISBN_number);
			Top = t.MJsAdd(Top,"publisher",0,publisher);
			Top = t.MJsAdd(Top,"type",2,type);
			Top = t.MJsAdd(Top,"laber",2,laber);
			Top = t.MJsAdd(Top,"begin",0,begin);
			Top = t.MJsAdd(Top,"end",0,end);
			Top = t.MJsAdd(Top,"Who",0,"NULL");
			T.MJsSave();
		 }
		 mtree* GetTop(){
		 	MJson T(_FileName);
		 	T.MJsGet();
		 	//cout<<T.oVal()->Chl->name;
		 	return T.oVal();
		 }
		 mtree* Find(char* Name){
		 	mtree* Top = GetTop();
		 	MTree_Cp k;
		 	return k.MJsFind(Top,Name,2);
		 }
};
class UserO{
	private:
		char *_FileName;
	public:
		UserO(char* Filename):_FileName(Filename){}
		mtree* GetTop(){
		 	MJson T(_FileName);
		 	T.MJsGet();
		 	mtree* Top = T.oVal();
		 	return Top;
		}
		bool Regist_check(char* name){
			MJson T(_FileName);
		 	T.MJsGet();
		 	mtree* Top = T.oVal();
			MTree_Cp k;
			if(k.MJsFind(Top,name,2)) return false;
			return true;
		}
		bool Log_check(char* name,char* password){
			MTree_Cp k;MJson T(_FileName);
		 	T.MJsGet();
		 	mtree* Top = T.oVal();
			mtree* To = k.MJsFind(Top,name,2);
			if(!To) return false; 
			To = To->Chl;
			To = k.MJsFind(To,"password",2);
			if(strcmp(To->data._Strval,password)==0) return true;
			else return false;
		}
		mtree* GetVal(char* name){
			MJson T(_FileName);MTree_Cp t;
			T.MJsGet();
			mtree* Top = T.oVal();
			return t.MJsFind(Top,name,2);
		}
		bool Regist(char* name,char* password,char* number,char* stunumber){
			MJson T(_FileName);MTree_Cp t;
			T.MJsGet();
			mtree* Top = T.oVal();
			
			if(strcmp(Top->name,"NULL") == 0){
			//cout<<"Go";
			t.MJsChange(Top,name,"",4);
			}else{
			//cout<<"Go!";
			Top = t.MJsAdd(Top,name,4,0,10);
			}
			Top = Top->Chl;
			t.MJsChange(Top,"password",password,0);
			Top = t.MJsAdd(Top,"level",2,1);
			Top = t.MJsAdd(Top,"number",0,number);
			Top = t.MJsAdd(Top,"StuNumber",0,stunumber);
			Top = t.MJsAdd(Top,"haven",2,0);
			Top = t.MJsAdd(Top,"history",4,0,0);
			return T.MJsSave();
		}
		bool LogIn(char* name,char* password){
			MJson T(_FileName);
			if(!Log_check(name,password)) return false;
			else return true;
		}
	
		mtree* GetHistory(char* name){
			MJson T(_FileName);
			T.MJsGet();
			MTree_Cp k;
			mtree* Top = T.oVal();
			Top = k.MJsFind(Top,name,2);
			Top = Top->Chl;
			Top = k.MJsFind(Top,"history",2);
			return Top->Chl;
		}
};
class UserS{
	private:
		//1 正常退订 2 正常借书成功 3 正常续借成功 4 预期退订 5正常借书失败 6退订失败  
		char* username;
		char* UserFileName,*BookFileName;//从左到右依次是用户数据库，图书数据库 
		int level;
	public:
		UserS(char* _UserFileName,char* _BookFileName,char* _username,int _level):UserFileName(_UserFileName),BookFileName(_BookFileName),username(_username){}
		int BorrowBook(char* BookName,int val,int tag){
			MJson Bk_(BookFileName);MTree_Cp k;
			Bk_.MJsGet();
			mtree* top = Bk_.oVal();
			mtree* WntBook =  k.MJsFind(top,BookName,2);
			if(!WntBook) {return 5;}
			
			mtree* Endtime = k.MJsFind(WntBook->Chl,"end",2);
			Time endTime(Endtime->data._Strval);
			if(endTime > endTime.NowDay() || (strcmp("NULL",Endtime->Brt->data._Strval) != 0 && strcmp(username,Endtime->Brt->data._Strval) !=0) ) {
			return 5;}
			if(strcmp(username,Endtime->Brt->data._Strval) ==0) tag = 2; 
			Endtime->Brt->data._Strval = username;
			Endtime->Lst->data._Strval = endTime.ToVal(endTime.NowDay().GetYear(),endTime.NowDay().GetMonth(),endTime.NowDay().GetDay());
			endTime.SetNow();
			Endtime->data._Strval = endTime.Add(0,0,val).GetVal();
			//cout<<endTime.Add(0,0,val).GetVal();
			Bk_.MJsSave();
			MJson User_(UserFileName);top = User_.oVal();
			User_.MJsGet();
			mtree* WntUser = k.MJsFind(top,username,2);
			WntUser = k.MJsFind(WntUser->Chl,"haven",2);
			WntBook = k.MJsFind(WntBook->Chl,"laber",2);
			WntUser->data._intval = WntBook->data._intval;
			
			WntUser = WntUser->Brt->Chl;
			if(strcmp(WntUser->name,"NULL")==0){
				k.MJsChange(WntUser,BookName,endTime.NowDay().GetVal(),0);
			}else{
			//	k.MJsAdd(WntUser,BookName,0,endTime.NowDay().GetVal(),0);
			}
			User_.MJsSave();
			return tag;
		}
		int StlBorrowBook(char* BookName,int val,int tag){
			MJson Bk_(BookFileName);MTree_Cp k;
			Bk_.MJsGet();
			mtree* top = Bk_.oVal();
			mtree* WntBook =  k.MJsFind(top,BookName,2);
			if(!WntBook) {return 5;}
			mtree* Endtime = k.MJsFind(WntBook->Chl,"end",2);
			Time endTime(Endtime->data._Strval);
			if(strcmp(username,Endtime->Brt->data._Strval) !=0 ) {
			return 5;}
			Endtime->Brt->data._Strval = username;
			Endtime->Lst->data._Strval = endTime.ToVal(endTime.NowDay().GetYear(),endTime.NowDay().GetMonth(),endTime.NowDay().GetDay());
			Endtime->data._Strval = endTime.Add(0,0,val).GetVal();
			//cout<<endTime.Add(0,0,val).GetVal();
			Bk_.MJsSave();
			MJson User_(UserFileName);top = User_.oVal();
			User_.MJsGet();
			mtree* WntUser = k.MJsFind(top,username,2);
			WntUser = k.MJsFind(WntUser->Chl,"haven",2);
			WntBook = k.MJsFind(WntBook->Chl,"laber",2);
			WntUser->data._intval = WntBook->data._intval;
			
			WntUser = WntUser->Brt->Chl;
			if(strcmp(WntUser->name,"NULL")==0){
				k.MJsChange(WntUser,BookName,endTime.NowDay().GetVal(),0);
			}else{
			//	k.MJsAdd(WntUser,BookName,0,endTime.NowDay().GetVal(),0);
			}
			User_.MJsSave();
			return 3;
		}
		int GbBook(char* bookname){
			MJson bk_(BookFileName);MTree_Cp k;
			bk_.MJsGet();
			mtree* top = bk_.oVal();
			mtree* WTNbook = k.MJsFind(top,bookname,2);
			if(!WTNbook) return 6;
			WTNbook = k.MJsFind(WTNbook->Chl,"end",2);
			if(strcmp(WTNbook->Brt->data._Strval,username) != 0) return 6;  
			Time EndTime(WTNbook->data._Strval);
			WTNbook->Brt->data._Strval = "NULL";
			if(EndTime > EndTime.NowDay()  || EndTime.NowDay() == EndTime) {
				WTNbook->data._Strval = EndTime.NowDay().GetVal();
				bk_.MJsSave();
				return 1;
			}else{
				bk_.MJsSave();
				return 4;
			}
		}
		mtree* Townt(){
			MJson bk_(BookFileName);MTree_Cp k;
			bk_.MJsGet();
			MJson user_(UserFileName);
			user_.MJsGet();
			mtree* used = k.MJsFind(user_.oVal(),username,2);
			used = k.MJsFind(used->Chl,"haven",2);
			int haven = used->data._intval;
			used = bk_.oVal();
			while(used != NULL){
				if(used->Chl->Brt->Brt->Brt->Brt->data._intval == haven) return used;
				used = used->Brt;
			}
			return NULL;
		}
};
class Daily{
	private:
		char* Filename;
	public:
		Daily(char* _Filename):Filename(_Filename){}
		void Add(char* username,int val){
			MJson t(Filename);MTree_Cp k;
			t.MJsGet();
			Time Now("1111-11-11");
			mtree* top = t.oVal();
			if(strcmp(top->name,"NULL") == 0){
				k.MJsChange(top,username,0,4);
				top = top->Chl;
				k.MJsChange(top,"time",Now.NowDay().GetVal(),0);
				k.MJsAdd(top,"mode",2,val);
			}else{
				top = k.MJsAdd(top,username,4,0,0);
				top = top->Chl;
				k.MJsChange(top,"time",Now.NowDay().GetVal(),0);
				k.MJsAdd(top,"mode",2,val);
			}
			t.MJsSave();
		}
		mtree* Getval(){
			MJson t(Filename);
			t.MJsGet();
			return t.oVal();
		}
};
