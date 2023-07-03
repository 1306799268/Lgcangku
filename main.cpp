#include "Message.h" 
#include <windows.h>
#define BOOKTXT "book.mjson"
#define USERTXT "user.mjson"
HWND hwndList;HMENU g_hPopupMenu;
HWND D_qujian;
HWND g_hLoginWnd,g_hMainWnd,g_modeWnd,g_wntWnd;  // 登录窗口句柄
HWND D_name,D_publisher,D_ISDN,D_type,D_laber,D_author,D_begin,D_end,D_last;
HWND u_name,u_level,u_num1,u_num2;
int state = 0;
int level = 1,show = 1; 
MJson TBK(BOOKTXT);
MJson USK(USERTXT); 
char Username[256];
// 登录窗口的窗口过程
LRESULT CALLBACK LoginWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            // 创建用户名和密码的输入框
            CreateWindow("STATIC", "用户名：", WS_VISIBLE | WS_CHILD, 30, 40, 65, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("STATIC", "密码：", WS_VISIBLE | WS_CHILD, 45, 75, 50, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 95, 40, 200, 20, hwnd, (HMENU)100, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 95, 75, 200, 20, hwnd, (HMENU)101, NULL, NULL);
            CreateWindow("BUTTON", "登录", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 55, 115, 100, 30, hwnd, (HMENU)102, NULL, NULL);
            CreateWindow("BUTTON", "注册", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 185, 115, 100, 30, hwnd, (HMENU)103, NULL, NULL);
            break;
        }

        case WM_COMMAND:
        {
            // 检查按钮点击事件
            if (LOWORD(wParam) == 102){
                // 获取用户名和密码输入框的文本内容
                char username[256];
                char password[256];
                GetWindowText(GetDlgItem(hwnd, 100), username, sizeof(username));
                GetWindowText(GetDlgItem(hwnd, 101), password, sizeof(password));
                UserO O1(USERTXT);
                // 进行用户名和密码的验证
                if(O1.Log_check(username,password))
                {
                    // 登录成功，关闭登录窗口
                    GetWindowText(GetDlgItem(hwnd, 100), Username, sizeof(Username));
                    MessageBox(hwnd, "欢迎您，登录成功！", username, MB_OK | MB_ICONINFORMATION);
                    ShowWindow(hwnd, SW_HIDE);state = 1;
                    MJson T(BOOKTXT);
				T.MJsGet();
				mtree* top = T.oVal();
				while(top != NULL){
				SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)top->name);
				top = top->Brt;}
				CreateWindow("STATIC", username, WS_VISIBLE | WS_CHILD, 95, 35, 65, 20, g_hMainWnd, NULL, NULL, NULL);
				top = O1.GetVal(username);
				top = top->Chl->Brt;
				level =  top->data._intval;
				CreateWindow("STATIC", " 图书操作", WS_VISIBLE | WS_CHILD, 95, 85, 85, 20, g_hMainWnd, NULL, NULL, NULL);
				if(level == 1) {CreateWindow("STATIC", "普通用户", WS_VISIBLE | WS_CHILD, 95, 60, 85, 20, g_hMainWnd, NULL, NULL, NULL);
       	        }else {CreateWindow("STATIC", " 管理员", WS_VISIBLE | WS_CHILD, 95, 60, 85, 20, g_hMainWnd, NULL, NULL, NULL);
				}
				
				MJson Lbk(BOOKTXT);Lbk.MJsGet();
        		MJson Lus(USERTXT);Lus.MJsGet();
       		 	MTree_Cp lo;
       		 	mtree* user = Lus.oVal();
				user = lo.MJsFind(user,Username,2); 
        		user = user->Chl;
        		user = user->Brt->Brt->Brt->Brt;
        		mtree* Top = Lbk.oVal();
        		while(Top != NULL){
        		if(Top->Chl->Brt->Brt->Brt->Brt->data._intval == user->data._intval)	break;
        		Top = Top->Brt;
				}
				if(Top){
					SetWindowText(g_wntWnd, Top->name); 
				}else{
					SetWindowText(g_wntWnd, "暂不知您喜好"); 
				}
				 
				}
                else
                {
                    // 登录失败，显示错误消息
                    MessageBox(hwnd, "用户名或密码错误！", "失败", MB_OK | MB_ICONERROR); 
                }
            }
       
            if (LOWORD(wParam) == 103){
            HWND mess = CreateWindow("RegisterWndClass", "输入您的信息", WS_OVERLAPPEDWINDOW|WS_OVERLAPPED , CW_USEDEFAULT, CW_USEDEFAULT, 380, 380, NULL, NULL, NULL, NULL);
            SetWindowLong(mess, GWL_STYLE, GetWindowLong(mess, GWL_STYLE) & ~WS_THICKFRAME);
			ShowWindow(mess, SW_SHOW);
		}
            break;
        }
        
        case WM_DESTROY:
            
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
LRESULT CALLBACK RegistWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        	CreateWindow("STATIC", "用户名：", WS_VISIBLE | WS_CHILD, 35, 50, 65, 20, hwnd, NULL, NULL, NULL);
        	CreateWindow("STATIC", "密码：", WS_VISIBLE | WS_CHILD, 50, 100, 50, 20, hwnd, NULL, NULL, NULL);
        	CreateWindow("STATIC", "联系方式：", WS_VISIBLE | WS_CHILD, 25, 150, 75, 20, hwnd, NULL, NULL, NULL);
        	CreateWindow("STATIC", "学生卡ID：", WS_VISIBLE | WS_CHILD, 25, 200, 75, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 50, 200, 20, hwnd, (HMENU)1001, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER , 100, 100, 200, 20, hwnd, (HMENU)1002, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100,150, 200, 20, hwnd, (HMENU)1003, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER , 100, 200, 200, 20, hwnd, (HMENU)1004, NULL, NULL);
            CreateWindow("BUTTON", "注册", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 200, 250, 100, 30, hwnd, (HMENU)1005, NULL, NULL);
            
            break;
		case WM_COMMAND:
        {
            // 检查按钮点击事件
            if (LOWORD(wParam) == 1005){
            	char username[256];
                char password[256];
                char number[50],stunumber[50];
                GetWindowText(GetDlgItem(hwnd, 1001), username, sizeof(username));
                GetWindowText(GetDlgItem(hwnd, 1002), password, sizeof(password));
                GetWindowText(GetDlgItem(hwnd, 1003), number, sizeof(number));
                GetWindowText(GetDlgItem(hwnd, 1004), stunumber, sizeof(stunumber));
                UserO O1(USERTXT);
                if(O1.Regist_check(username)){
                	GetWindowText(GetDlgItem(hwnd, 100), Username, sizeof(Username));
                	MessageBox(hwnd, "欢迎您，注册成功！", username, MB_OK | MB_ICONINFORMATION);
                	O1.Regist(username,password,number,stunumber);
                	
                MJson T(BOOKTXT);
				T.MJsGet();
				mtree* top = T.oVal();
				while(top != NULL){
				SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)top->name);
				top = top->Brt;}
				ShowWindow(g_hLoginWnd, SW_HIDE);
				
				
				CreateWindow("STATIC", username, WS_VISIBLE | WS_CHILD, 95, 35, 65, 20, g_hMainWnd, NULL, NULL, NULL);
				CreateWindow("STATIC", " 普通用户", WS_VISIBLE | WS_CHILD, 95, 60, 85, 20, g_hMainWnd, NULL, NULL, NULL);
				SetWindowText(g_modeWnd, "图书操作"); 
				state = 1;
				DestroyWindow(hwnd);
				/*MJson Lbk(BOOKTXT);Lbk.MJsGet();
        		MJson Lus(USERTXT);Lus.MJsGet();
       		 	MTree_Cp lo;
       		 	
       		 	mtree* user = Lus.oVal();
				user = lo.MJsFind(user,Username,2); 
				DestroyWindow(hwnd);
				user = user->Chl;
        		user = user->Brt->Brt->Brt->Brt;
        		mtree* Top = Lbk.oVal();
        		while(Top != NULL){
        		if(Top->Chl->Brt->Brt->Brt->Brt->data._intval == user->data._intval)	break;
        		Top = Top->Brt;
				}
				if(Top){
					SetWindowText(g_wntWnd, Top->name); 
				}else{
					SetWindowText(g_wntWnd, "暂不知您喜好"); 
				} */
			 
				
				
                }else{
                	MessageBox(hwnd, "该用户名已经被注册了！", "失败", MB_OK | MB_ICONERROR); 
				}
            }
            
		}
            break;
        
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// 主窗口的窗口过程
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:{
            // 创建登录窗口
            ShowWindow(g_hLoginWnd, SW_SHOW);
            CreateWindow("STATIC", "菜单", WS_VISIBLE | WS_CHILD, 550, 25, 65, 20, hwnd, NULL, NULL, NULL);
            hwndList = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD, 550, 45, 200, 495, hwnd, NULL, GetModuleHandle(NULL), NULL);
            g_hPopupMenu = CreatePopupMenu();
            AppendMenu(g_hPopupMenu, MF_STRING, 2001, "操作");
            AppendMenu(g_hPopupMenu, MF_STRING, 2002, "查看");
            AppendMenu(g_hPopupMenu, MF_SEPARATOR, 0, NULL);
            AppendMenu(g_hPopupMenu, MF_STRING, 2003, "修改(管理员功能)");
            AppendMenu(g_hPopupMenu, MF_STRING, 2004, "增加(管理员功能)");
            AppendMenu(g_hPopupMenu, MF_STRING, 2005, "删除(管理员功能)");
            CreateWindow("STATIC", "用户名：", WS_VISIBLE | WS_CHILD, 30, 35, 65, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("STATIC", "您的等级：", WS_VISIBLE | WS_CHILD, 30, 60, 85, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("STATIC", "操作模式：", WS_VISIBLE | WS_CHILD, 30, 85, 85, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("STATIC", "当前日期：", WS_VISIBLE | WS_CHILD, 30, 110, 85, 20, hwnd, NULL, NULL, NULL);
            Time KE("2000-01-01");
            CreateWindow("STATIC", KE.NowDay().GetVal(), WS_VISIBLE | WS_CHILD, 115, 110, 85, 20, hwnd, NULL, NULL, NULL);
            g_modeWnd = CreateWindow("STATIC", " 图书操作", WS_VISIBLE | WS_CHILD, 95, 85, 85, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("STATIC", "向您推荐：", WS_VISIBLE | WS_CHILD, 30, 135, 85, 20, hwnd, NULL, NULL, NULL);
            g_wntWnd = CreateWindow("STATIC", "暂不知您喜好", WS_VISIBLE | WS_CHILD, 100, 135, 100, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("BUTTON", "切换模式", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 30, 220, 100, 30, hwnd, (HMENU)2006, NULL, NULL);
            CreateWindow("BUTTON", "刷新菜单", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 135, 220, 100, 30, hwnd, (HMENU)2010, NULL, NULL);
            //HWND D_name,D_publisher,D_ISDN,D_type,D_laber,D_author,D_begin,D_end; 
			CreateWindow("STATIC", "名称：", WS_VISIBLE | WS_CHILD, 30, 265, 65, 20, hwnd, NULL, NULL, NULL);
			D_name = CreateWindow("EDIT", "书名", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 265, 200, 20, hwnd, (HMENU)11, NULL, NULL);
			CreateWindow("STATIC", "作者：", WS_VISIBLE | WS_CHILD, 30, 290, 65, 20, hwnd, NULL, NULL, NULL);
			D_author = CreateWindow("EDIT", "作者", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 290, 200, 20, hwnd, (HMENU)12, NULL, NULL);
			CreateWindow("STATIC", "出版社：", WS_VISIBLE | WS_CHILD, 30, 315, 65, 20, hwnd, NULL, NULL, NULL);
			D_publisher = CreateWindow("EDIT", "出版社", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 315, 200, 20, hwnd,(HMENU)13, NULL, NULL);
			CreateWindow("STATIC", "ISBN：", WS_VISIBLE | WS_CHILD, 30, 340, 65, 20, hwnd, NULL, NULL, NULL);
			D_ISDN = CreateWindow("EDIT", "ISBN", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 340, 200, 20, hwnd, (HMENU)14, NULL,NULL);
			CreateWindow("STATIC", "借阅起止时间：", WS_VISIBLE | WS_CHILD, 10, 365, 105, 20, hwnd, NULL, NULL, NULL);
			D_begin = CreateWindow("EDIT", "开始时间", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 365, 95, 20, hwnd, NULL, NULL, NULL);
			D_end = CreateWindow("EDIT", "结束时间", WS_VISIBLE | WS_CHILD | WS_BORDER, 205, 365, 95, 20, hwnd, NULL, NULL, NULL);
			CreateWindow("STATIC", "最后借阅：", WS_VISIBLE | WS_CHILD, 20, 390, 75, 20, hwnd, NULL, NULL, NULL);
			D_last = CreateWindow("EDIT", "NULL", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 390, 95, 20, hwnd, NULL, NULL, NULL);
			CreateWindow("STATIC", "书籍分类：", WS_VISIBLE | WS_CHILD, 20, 415, 75, 20,  hwnd, NULL, NULL, NULL);
			D_type = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 415, 200, 200, hwnd, NULL, NULL, NULL);
			SendMessage(D_type, CB_ADDSTRING, 0, (LPARAM)"期刊");
    		SendMessage(D_type, CB_ADDSTRING, 0, (LPARAM)"杂志");
  		    SendMessage(D_type, CB_ADDSTRING, 0, (LPARAM)"书籍");
			CreateWindow("STATIC", "书籍类型：", WS_VISIBLE | WS_CHILD, 20, 440, 75, 20,  hwnd, NULL, NULL, NULL);
			//1.小说 2.漫画绘本 3.青春 4.推理 5.短篇集 6.历史 7.国风文化 8.兴趣培养 9.经典 10.精品课 11.专业 12.理财 13.外语 14.恋爱 15.技能
		//16.艺术 17.哲学 18.心理 19. 传记 20.诗歌戏剧 21散文 22科普 23影视原著 24互联网 25亲子 26儿童文学 27励志 28理想国 
			char laber_[28][25] = {"小说","漫画绘本","青春","推理","短集篇","历史","国风文化","兴趣培养","经典","精品课","专业","理财","外语",\
			"恋爱","技能","艺术","哲学","心理","传记","诗歌戏剧","散文","科普","影视原著","互联网","亲子","儿童文学","励志","理想国"}; 
			D_laber = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 440, 200, 500, hwnd, NULL, NULL, NULL);
			for(int i=0;i<28;i++){
				SendMessage(D_laber, CB_ADDSTRING, 0, (LPARAM)laber_[i]);
			}
			CreateWindow("STATIC", "借阅时长", WS_VISIBLE | WS_CHILD, 345, 260, 75, 20, hwnd, NULL, NULL, NULL);
			//HWND u_name,u_level,u_num1,u_num2;
			CreateWindow("STATIC", "姓名：", WS_VISIBLE | WS_CHILD, 20, 470, 75, 20,  hwnd, NULL, NULL, NULL);
			u_name = CreateWindow("EDIT", "姓名", WS_VISIBLE | WS_CHILD | WS_BORDER, 95, 470, 150, 20, hwnd, (HMENU)15, NULL, NULL);
			CreateWindow("STATIC", "等级：", WS_VISIBLE | WS_CHILD, 20, 500, 75, 20,  hwnd, NULL, NULL, NULL);
			u_level = CreateWindow("EDIT", "等级", WS_VISIBLE | WS_CHILD | WS_BORDER, 95, 500, 150, 20, hwnd, (HMENU)16, NULL, NULL);
			CreateWindow("STATIC", "联系方式：", WS_VISIBLE | WS_CHILD, 255, 470, 75, 20,  hwnd, NULL, NULL, NULL);
			u_num1 = CreateWindow("EDIT", "联系方式", WS_VISIBLE | WS_CHILD | WS_BORDER, 330, 470, 200, 20, hwnd, (HMENU)17, NULL, NULL);
			CreateWindow("STATIC", "学生卡ID：", WS_VISIBLE | WS_CHILD, 255, 500, 75, 20,  hwnd, NULL, NULL, NULL);
			u_num2 = CreateWindow("EDIT", "学生卡ID", WS_VISIBLE | WS_CHILD | WS_BORDER, 330, 500, 200, 20, hwnd, (HMENU)18, NULL, NULL);
			D_qujian= CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 345, 285, 100, 200, hwnd, NULL, NULL, NULL);
			SendMessage(D_qujian, CB_ADDSTRING, 0, (LPARAM)"30天");
			SendMessage(D_qujian, CB_ADDSTRING, 0, (LPARAM)"60天");
			SendMessage(D_qujian, CB_ADDSTRING, 0, (LPARAM)"90天");
			CreateWindow("BUTTON", "借阅", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 345, 315, 100, 30, hwnd, (HMENU)2007, NULL, NULL);
			CreateWindow("BUTTON", "续借", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 345, 350, 100, 30, hwnd, (HMENU)2008, NULL, NULL);
			CreateWindow("BUTTON", "还书", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 345, 385, 100, 30, hwnd, (HMENU)2009, NULL, NULL);
			break;
			}
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_COMMAND:
		{
        	switch (LOWORD(wParam)){
        		case 2001:{
        			
					break;
				} 
				case 2002:{
        			int selectedIndex = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
      				if (selectedIndex != LB_ERR){
            		// 获取选择框当前选中项的内容
            		int textLength = SendMessage(hwndList, LB_GETTEXTLEN, selectedIndex, 0);
            		char* buffer = new char[textLength + 1];
            		SendMessage(hwndList, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
            		//HWND D_name,D_publisher,D_ISDN,D_type,D_laber,D_author,D_begin,D_end;
            		mtree* Top = NULL; 
            		if(show == 1){
            			MJson T(BOOKTXT);
            			T.MJsGet();
            			Top = T.oVal();
            			MTree_Cp k;
            			while(Top != NULL){ if(strcmp(Top->name,buffer) == 0 ){break;}Top = Top->Brt;}
						if(Top && strcmp(Top->name,"NULL")){
							SetWindowText(D_name, Top->name);
							Top = Top->Chl;
							SetWindowText(D_author, Top->data._Strval);
							Top = Top->Brt;
							SetWindowText(D_ISDN, Top->data._Strval);
							Top = Top->Brt;
							SetWindowText(D_publisher, Top->data._Strval);
							Top = Top->Brt;
							SendMessage(D_type, CB_SETCURSEL, Top->data._intval - 1, 0);
							Top = Top->Brt;
							SendMessage(D_laber, CB_SETCURSEL, Top->data._intval - 1, 0);
							Top = Top->Brt;
							SetWindowText(D_begin, Top->data._Strval);
							Top = Top->Brt;
							SetWindowText(D_end, Top->data._Strval);
							Top = Top->Brt;
							SetWindowText(D_last, Top->data._Strval);
						} 
					}else if(show == 2){
						MJson T(USERTXT);
						T.MJsGet();
						T.oVal();
						Top = T.oVal();
						MTree_Cp k;
						while(Top != NULL){ if(strcmp(Top->name,buffer) == 0 ){break;}Top = Top->Brt;}
						if(Top && strcmp(Top->name,"NULL")){
							//HWND u_name,u_level,u_num1,u_num2;
							SetWindowText(u_name, Top->name);
							Top = Top->Chl->Brt;
							if(Top->data._intval == 1) SetWindowText(u_level, "普通用户");
							else SetWindowText(u_level, "管理员");
							Top = Top->Brt;
							SetWindowText(u_num1, Top->data._Strval);
							Top = Top->Brt;
							SetWindowText(u_num2, Top->data._Strval);
						} 
					}

            		delete []buffer;}
					break;
				} 
				case 2003:{
					if(level == 1) {
        				MessageBox(hwnd, "您并非管理员，您无权操作", "抱歉！ID:2004", MB_OK|MB_ICONERROR);
        				break;
					}
					int selectedIndex = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
					if (selectedIndex != CB_ERR){
					int textLength = SendMessage(hwndList, LB_GETTEXTLEN, selectedIndex, 0);
            		char* buffer = new char[textLength + 1];
            		SendMessage(hwndList, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
            		cout<<"go";
					if(show == 1){
						char name[40],author[40],ISBN[40],publish[40];
						GetWindowText(GetDlgItem(hwnd, 11), name, sizeof(name));
						GetWindowText(GetDlgItem(hwnd, 12), author, sizeof(author));
						GetWindowText(GetDlgItem(hwnd, 13), ISBN, sizeof(ISBN));
						GetWindowText(GetDlgItem(hwnd, 14), publish, sizeof(publish));
						int selectedIndex1 = SendMessage(D_type, CB_GETCURSEL, 0, 0);
						int selectedIndex2 = SendMessage(D_laber, CB_GETCURSEL, 0, 0);
						cout<<name;
						Book bk(BOOKTXT);
						MJson b(BOOKTXT);b.MJsGet();
						MTree_Cp K;
						mtree* Top = b.oVal();
						while(Top != NULL){if(strcmp(Top->name,buffer) == 0 ){break;}Top = Top->Brt;}
						cout<<Top->name; 
						if(Top){
							Top->name = name;
							Top = Top->Chl;
							Top->data._Strval = author;
							Top = Top->Brt;
							Top->data._Strval = ISBN;
							Top = Top->Brt;
							Top->data._Strval = publish;
							Top = Top->Brt;
							Top->data._intval = selectedIndex1 + 1;
							Top = Top->Brt;
							Top->data._intval = selectedIndex2 + 2;
						}
						b.MJsSave();
						SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)name);
						SendMessage(hwndList, LB_DELETESTRING, selectedIndex, 0);
					
					}else if(show == 2){
						char name[40],level[40],number[40],stunumber[40];
						GetWindowText(GetDlgItem(hwnd, 15), name, sizeof(name));
						GetWindowText(GetDlgItem(hwnd, 16), level, sizeof(level));
						GetWindowText(GetDlgItem(hwnd, 17), number, sizeof(number));
						GetWindowText(GetDlgItem(hwnd, 18), stunumber, sizeof(stunumber));
						MJson b(USERTXT);b.MJsGet();
						MTree_Cp K;
						mtree* Top = b.oVal();
						if(Top){
							Top->name = name;
							Top = Top->Chl->Brt;
							if(strcmp(level,"管理员") == 0){
								Top->data._intval = 2;
							}else if(strcmp(level,"普通用户") == 0){
								Top->data._intval = 1;
							}
							Top = Top->Brt;
							Top->data._Strval = number;
							Top = Top->Brt;
							Top->data._Strval = stunumber;
						}
						SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)name);
						SendMessage(hwndList, LB_DELETESTRING, selectedIndex, 0);
					}
					}
					break;
				}
				 
				case 2004:{
        			if(level == 1) {
        				MessageBox(hwnd, "您并非管理员，您无权操作", "抱歉！ID:2004", MB_OK|MB_ICONERROR);
        				break;
					}
					if(show == 1){
					char name[40],author[40],ISBN[40],publish[40];
					GetWindowText(GetDlgItem(hwnd, 11), name, sizeof(name));
					GetWindowText(GetDlgItem(hwnd, 12), author, sizeof(author));
					GetWindowText(GetDlgItem(hwnd, 13), ISBN, sizeof(ISBN));
					GetWindowText(GetDlgItem(hwnd, 14), publish, sizeof(publish));
					
					Book bk(BOOKTXT);
					MJson b(BOOKTXT);b.MJsGet();
					MTree_Cp K;
					mtree* Top = b.oVal();
					if(K.MJsFind(Top,name,2)){
						MessageBox(hwnd, "书名重复了！", "抱歉！ID:2004", MB_OK|MB_ICONERROR);
						break;
					}
					
					int selectedIndex1 = SendMessage(D_type, CB_GETCURSEL, 0, 0);
					int selectedIndex2 = SendMessage(D_laber, CB_GETCURSEL, 0, 0);
					bk.LaberSet(selectedIndex1+1,selectedIndex2+1);
					bk.Setmessage(name,author,ISBN,publish);
					
					bk.BookSaves();
					SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)name);
					MessageBox(hwnd, "已加入该书", "成功导入", MB_OK);
					}else{
						HWND mess = CreateWindow("RegisterWndClass", "输入添加的用户名", WS_OVERLAPPEDWINDOW|WS_OVERLAPPED , CW_USEDEFAULT, CW_USEDEFAULT, 380, 380, NULL, NULL, NULL, NULL);
           				SetWindowLong(mess, GWL_STYLE, GetWindowLong(mess, GWL_STYLE) & ~WS_THICKFRAME);
						ShowWindow(mess, SW_SHOW);
					}
					
					break;
				} 
				
				case 2005:{
					if(level != 1){
        			int selectedIndex = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
        			if (selectedIndex != LB_ERR){
            		// 删除选择框当前选中项
            		int textLength = SendMessage(hwndList, LB_GETTEXTLEN, selectedIndex, 0);
            		char* buffer = new char[textLength + 1];
            		SendMessage(hwndList, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
            		mtree* Top = NULL;
					if(show == 1){
            			MJson T(BOOKTXT);
            			T.MJsGet();
            			Top = T.oVal();
            			MTree_Cp k;
						while(Top != NULL){ if(strcmp(Top->name,buffer) == 0 ){break;}else Top = Top->Brt;}
						if(Top){
							Top = k.MJsDel(Top,T.oVal());
							T.setoval(*Top);
						}
						T.MJsSave();
					}else if(show == 2){
						MJson T(USERTXT);
						T.MJsGet();
						T.oVal();
						Top = T.oVal();
						MTree_Cp k;
						while(Top != NULL){ if(strcmp(Top->name,buffer) == 0 ){break;}Top = Top->Brt;}
						mtree* Root = Top->Rt;
						if(Top->Lst == NULL && Top->Brt != NULL){Root = Top->Brt;}
						if(Top){
							Top = k.MJsDel(Top,T.oVal());
							T.setoval(*Top);
						}
						T.MJsSave();
					}
            		SendMessage(hwndList, LB_DELETESTRING, selectedIndex, 0);}
					
				}else{
					MessageBox(hwnd, "您并非管理员，您无权操作", "抱歉！ID:2005", MB_OK|MB_ICONERROR);
				}
				break;
				} 
				case 2006:{
					if(state == 0) break;
					if(level == 1){
						MessageBox(hwnd, "您并非管理员，您无权操作", "抱歉！ID:2006", MB_OK|MB_ICONERROR);
						break;
					}
					if(show == 1) {
						SetWindowText(g_modeWnd, " 用户管理");	show = 2; 
						SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
						MJson T(USERTXT);
						T.MJsGet();
						mtree* top = T.oVal();
						while(top != NULL){
						SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)top->name);
						top = top->Brt;}
					}
					else{
						show = 1;
						SetWindowText(g_modeWnd, " 图书操作"); 
						SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
						MJson T(BOOKTXT);
						T.MJsGet();
						mtree* top = T.oVal();
						while(top != NULL){
						SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)top->name);
						top = top->Brt;}
					}
					break;
				}
				case 2007:{
					if(state == 0) break;
					UserS O2(USERTXT,BOOKTXT,Username,1);
					char bookname[50];
					int val;
					int selectedIndex = SendMessage(D_qujian, CB_GETCURSEL, 0, 0);
					if (selectedIndex != CB_ERR){
						val = 30 * (selectedIndex + 1); 
					}else val = 30;
					selectedIndex = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
      				if (selectedIndex != LB_ERR){
            		SendMessage(hwndList, LB_GETTEXT, selectedIndex, (LPARAM)bookname);}else{
            			break;
					} 
					if(O2.BorrowBook(bookname,val,2)==2) MessageBox(hwnd, "您已成功借走该书，请注意按时间归还", "操作成功", MB_OK);
					else MessageBox(hwnd, "借书失败，请查看该书是否还未被归还。", "抱歉！ID:2007", MB_OK|MB_ICONERROR); 
					break;
				}
				case 2008:{
					if(state == 0) break;
					UserS O2(USERTXT,BOOKTXT,Username,1);
					char bookname[50];
					int val;
					int selectedIndex = SendMessage(D_qujian, CB_GETCURSEL, 0, 0);
					if (selectedIndex != CB_ERR){
						val = 30 * (selectedIndex + 1); 
					}else val = 30;
					selectedIndex = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
      				if (selectedIndex != LB_ERR){
            		SendMessage(hwndList, LB_GETTEXT, selectedIndex, (LPARAM)bookname);}else{
            			break;
					} 
					if(O2.StlBorrowBook(bookname,val,3) != 5) MessageBox(hwnd, "您已成功续借该书，请注意按时间归还", "操作成功", MB_OK);
					else MessageBox(hwnd, "续借失败，请查看原因。", "抱歉！ID:2008", MB_OK|MB_ICONERROR); 
					break;
				}
				case 2009:{
					if(state == 0) break;
					UserS O2(USERTXT,BOOKTXT,Username,1);
					char bookname[50];
					int val;
					int selectedIndex = SendMessage(D_qujian, CB_GETCURSEL, 0, 0);
					if (selectedIndex != CB_ERR){
						val = 30 * (selectedIndex + 1); 
					}else val = 30;
					selectedIndex = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
      				if (selectedIndex != LB_ERR){
            		SendMessage(hwndList, LB_GETTEXT, selectedIndex, (LPARAM)bookname);}else{
            			break;
					} 
					int error = O2.GbBook(bookname);
					if(error == 1) MessageBox(hwnd, "您已成功归还该书", "操作成功", MB_OK);
					else if(error == 4) MessageBox(hwnd, "归还成功，鉴于您已逾期归还，已被记录档案", "抱歉！ID:2009", MB_OK|MB_ICONERROR); 
					else if(error == 6) MessageBox(hwnd, "归还失败，请查看原因", "抱歉！ID:2009", MB_OK|MB_ICONERROR); 
					break;
				} 
				case 2010:{
					if(state == 0) break;
					if(show == 2) {
						SetWindowText(g_modeWnd, " 用户管理");	show = 2; 
						SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
						MJson T(USERTXT);
						T.MJsGet();
						mtree* top = T.oVal();
						while(top != NULL){
						SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)top->name);
						top = top->Brt;}
					}
					else{
						show = 1;
						SetWindowText(g_modeWnd, " 图书操作"); 
						SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
						MJson T(BOOKTXT);
						T.MJsGet();
						mtree* top = T.oVal();
						while(top != NULL){
						SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)top->name);
						top = top->Brt;}
					}
					break;
				}
			} 
			break;
		}
		
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        case WM_CONTEXTMENU:
        {
            // 获取鼠标位置
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);

            // 将鼠标位置转换为列表框客户区坐标
            ScreenToClient(hwnd, &pt);

            // 判断鼠标位置是否在列表框内
            RECT rc;
            GetClientRect(hwnd, &rc);
            if (PtInRect(&rc, pt) && state == 1)
            {
                // 弹出右键菜单
                TrackPopupMenu(g_hPopupMenu, TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam), 0, hwnd, NULL);
            }
            break;
        }
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
LRESULT CALLBACK ComplWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        	CreateWindow("STATIC", "用户名：", WS_VISIBLE | WS_CHILD, 35, 50, 65, 20, hwnd, NULL, NULL, NULL);
        	CreateWindow("STATIC", "密码：", WS_VISIBLE | WS_CHILD, 50, 100, 50, 20, hwnd, NULL, NULL, NULL);
        	CreateWindow("STATIC", "联系方式：", WS_VISIBLE | WS_CHILD, 25, 150, 75, 20, hwnd, NULL, NULL, NULL);
        	CreateWindow("STATIC", "学生卡ID：", WS_VISIBLE | WS_CHILD, 25, 200, 75, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 50, 200, 20, hwnd, (HMENU)1001, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER , 100, 100, 200, 20, hwnd, (HMENU)1002, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100,150, 200, 20, hwnd, (HMENU)1003, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER , 100, 200, 200, 20, hwnd, (HMENU)1004, NULL, NULL);
            CreateWindow("BUTTON", "借阅", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 200, 250, 100, 30, hwnd, (HMENU)1005, NULL, NULL);
            
            break;
		case WM_COMMAND:
        {
            // 检查按钮点击事件
            if (LOWORD(wParam) == 1005){
            }
            
		}
            break;
        
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 注册主窗口类
    WNDCLASS wcMain = {0};
    wcMain.lpfnWndProc = MainWndProc;
    wcMain.hInstance = hInstance;
    wcMain.hIcon = (HICON)LoadImage(hInstance, "favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    wcMain.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcMain.lpszClassName = "MainWndClass";
    RegisterClass(&wcMain);

    // 创建主窗口
    HWND hwndMain = CreateWindow("MainWndClass", "图书馆管理系统", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
    ShowWindow(hwndMain, nCmdShow);

    // 注册登录窗口类
    WNDCLASS wcLogin = {0};
    wcLogin.lpfnWndProc = LoginWndProc;
    wcLogin.hInstance = hInstance;
    wcLogin.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcLogin.lpszClassName = "LoginWndClass";
    RegisterClass(&wcLogin);
    // 注册注册窗口类 
    WNDCLASS wcRegister = {0};
    wcRegister.lpfnWndProc = RegistWndProc;
    wcRegister.hInstance = hInstance;
    wcRegister.hIcon = (HICON)LoadImage(hInstance, "favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    wcRegister.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcRegister.lpszClassName = "RegisterWndClass";
    RegisterClass(&wcRegister);
    // 注册注册窗口类 
    WNDCLASS wcCompl = {0};
    wcCompl.lpfnWndProc = ComplWndProc;
    wcCompl.hInstance = hInstance;
    wcCompl.hIcon = (HICON)LoadImage(hInstance, "favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    wcCompl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcCompl.lpszClassName = "wcComplWndClass";
    RegisterClass(&wcCompl);
    
    // 创建登录窗口作为主窗口的子窗口
    HWND hwndLogin = CreateWindow("LoginWndClass", "", WS_CHILD | WS_BORDER, 25, 25, 330, 170 ,hwndMain, NULL, hInstance, NULL);
    g_hLoginWnd = hwndLogin;
    g_hMainWnd = hwndMain;
    ShowWindow(hwndLogin, SW_SHOW);
    SetWindowLong(hwndMain, GWL_STYLE, GetWindowLong(hwndMain, GWL_STYLE) & ~WS_THICKFRAME);
	SetWindowLong(hwndLogin, GWL_STYLE, GetWindowLong(hwndLogin, GWL_STYLE) & ~WS_THICKFRAME);
    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
