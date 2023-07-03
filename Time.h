#include<time.h>
class Time{
	private:
		char* _val;
		int year,month,day;
	public:
		Time(char* val):_val(val){
			char* ptr = _val;
			int tag = 0;
			int a  = 0;
			while(*ptr != '\0'){
				for(;*ptr >= '0' && *ptr <= '9';ptr++) a = a*10 + *ptr - '0';
				if(tag == 0){
					year = a;
					a = 0;
					tag++;
				}else if(tag == 1){
					month = a;
					a = 0;
					tag++;
				}else{
					day = a;
				}
				ptr++;
			}
		}
		void reset(){
			char* ptr = _val;
			int tag = 0;
			int a  = 0;
			while(*ptr != '\0'){
				for(;*ptr >= '0' && *ptr <= '9';ptr++) a = a*10 + *ptr - '0';
				if(tag == 0){
					year = a;
					a = 0;
					tag++;
				}else if(tag == 1){
					month = a;
					a = 0;
					tag++;
				}else{
					day = a;
				}
				ptr++;
			} 
		}
		bool operator== (const Time& s2){
			if(year != s2.year) return false;
			if(month != s2.month) return false;
			if(day != s2.day) return false;
			return true;
		}
		bool operator>(const Time& s2){
			if(year > s2.year) return true;
			if(year < s2.year) return false;
			if(month > s2.month) return true;
			if(month < s2.month) return false;
			if(day > s2.day) return true;
			else return false;
		}
		Time Add(int Yval,int Mval,int Dval){
			int year_ = year,month_=month,day_=day;
			year_+= Yval;
			month_+= Mval;
			year_+= month/12;
			month_ = month_%12;
			day_+= Dval;
			while(day_>Monthday(year_,month_)){
				day_-= Monthday(year_,month_);
				month_++;
				if(month_ ==13) month_ = 1,year_++;
			}
			return Time(ToVal(year_,month_,day_));
		}
		int Monthday(int year,int month){
			if(month == 1 ||month == 3||month == 5||month == 7||month == 8||month == 10||month == 12){
				return 31;
			}
			if(month == 4 || month ==6 || month == 9 ||month ==11){
				return 30;
			}
			if(month == 2){
				if(year % 4 == 0 && year%100 != 0) return 29;
				else if(year % 400 == 0) return 29;
				else return 28;
			}
		}
		int Yearday(int year){
			if(month == 2){
				if(year % 4 == 0 && year%100 != 0) return 366;
				else if(year % 400 == 0) return 366;
				else return 365;
			}
		}
		void SetYear(int val){
			year =val;
		}
		void SetMonth(int val){
			month =val;
		}
		void SetDay(int val){
			day = val;
		}
		int GetYear(){
			return year;
		}
		char* GetVal(){
			return _val;
		}
		int GetMonth(){
			return month;
		}
		int GetDay(){
			return day;
		}
		char* ToVal(int year_,int month_,int day_){
			char* data = new char[10];
			data[0] ='0',data[1]= '0',data[2]='0',data[3]='0',data[4]='-',data[5]='0',data[6]='0',data[7]='-',data[8]='0',data[9]='0';
			int cnt = 3;
			while(year_>0){
				data[cnt] = (year_%10)+ '0';
				//cout<<data[cnt]<<endl;
				year_/= 10;
				cnt--;
			}
			cnt = 6;
			while(month_>0){
				data[cnt--] = (month_%10) +'0';
				month_/= 10;
			}
			cnt = 9;
			while(day_>0){
				data[cnt--] = (day_%10)+ '0';
				day_/= 10;
			}
			data[10] = '\0';
			return data;
		}
		void SetNow(){
			time_t current;
			time(&current);
			struct tm *Nowtime = localtime(&current);
			year = Nowtime->tm_year + 1900;
			month = Nowtime->tm_mon + 1;
			day = Nowtime->tm_mday;
		}
		Time NowDay(){
			time_t current;
			time(&current);
			struct tm *Nowtime = localtime(&current);
			int year = Nowtime->tm_year + 1900;
			int month = Nowtime->tm_mon + 1;
			int day = Nowtime->tm_mday;
			char* data = ToVal(year,month,day);
			return Time(data);
		}
};
