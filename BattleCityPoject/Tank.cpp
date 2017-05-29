#include "HEAD.h"
#include "MultiBattle.h"
#include"stdio.h"  
#include"string.h"  

int  WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	appInstance = hInstance;
	WNDCLASS wndcls;
	//定义窗口	
	//窗口扩展
	wndcls.cbClsExtra = 0;
	//窗口实例扩展
	wndcls.cbWndExtra = 0;
	//窗口背景色
	wndcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//窗口鼠标光标
	wndcls.hCursor = LoadCursor(NULL, IDC_CROSS);
	//窗口的最小化图标
	wndcls.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	//实例句柄
	wndcls.hInstance = hInstance;
	//窗口处理函数
	wndcls.lpfnWndProc = WndProc;
	//窗口类名
	wndcls.lpszClassName = L"CC-Tank";
	//窗口菜单
	wndcls.lpszMenuName = NULL;
	//窗口类型
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	//注册窗口
	RegisterClass(&wndcls);
	//屏幕宽和高
	int width, height;
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	//在屏幕中央创建窗口
	hwnd = CreateWindow(L"CC-Tank", L"BattleWorld V1.0", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
		(width - (GAME_W + 300)) / 2, (height - (GAME_H + 28)) / 2 - 10, GAME_W + 300, GAME_H + 28, NULL, NULL, hInstance, NULL);
	//显示窗口
	ShowWindow(hwnd, nCmdShow);
	//更新窗口
	UpdateWindow(hwnd);
	//获得窗口设备
	winDC = GetDC(hwnd);
	//创建缓冲内存
	writeDC = CreateCompatibleDC(winDC);
	//创建背景内存
	backgroundDC = CreateCompatibleDC(winDC);
	//创建玩家坦克内存
	player_tankDC = CreateCompatibleDC(winDC);
	//创建炮弹内存
	bulletDC = CreateCompatibleDC(winDC);
	//创建图块内存
	blockDC = CreateCompatibleDC(winDC);
	//创建火花内存
	fireDC = CreateCompatibleDC(winDC);
	//创建缓冲位图
	writeBMP = CreateCompatibleBitmap(winDC, GAME_W, GAME_H);
	//载入背景位图
	backgroundBMP = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(BACK_GROUND), IMAGE_BITMAP, GAME_W, GAME_H, LR_DEFAULTCOLOR);
	//载入坦克位图
	player_tankBMP = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(PLAYER_TANK), IMAGE_BITMAP, 256, 10240, LR_DEFAULTCOLOR);
	//载入炮弹位图
	bulletBMP = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(BULLET), IMAGE_BITMAP, 160, 400, LR_DEFAULTCOLOR);
	//载入图块位图
	blockBMP = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(MAP_BLOCK), IMAGE_BITMAP, 256, 896, LR_DEFAULTCOLOR);
	//载入火花位图
	fireBMP = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(FIRE), IMAGE_BITMAP, 528, 132, LR_DEFAULTCOLOR);
	//选入到对应内存
	SelectObject(writeDC, writeBMP);
	SelectObject(backgroundDC, backgroundBMP);
	SelectObject(player_tankDC, player_tankBMP);
	SelectObject(bulletDC, bulletBMP);
	SelectObject(blockDC, blockBMP);
	SelectObject(fireDC, fireBMP);
	//用系统时间初始化随机数种子
	srand((unsigned)time(NULL));
	//读入预置地图
	MapInit();
	//消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return  (int)msg.wParam;
}
//窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HFONT hFont;
	switch (message)
	{
	case WM_CREATE:
		//登录界面编辑框按钮	
		hButton_login = (HWND)CreateWindow(TEXT("Button"),
			TEXT("登                 录"),
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
			380, 400, 240, 32,
			hWnd,
			(HMENU)Btn_login,  //(重点)这里设置按钮id,但是 原本是设置菜单的 所以需要HMENU 
			appInstance,
			NULL);
		Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(Login_BMP));
		SendMessage(hButton_login, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);

		hButton_Web = (HWND)CreateWindow(TEXT("Button"),
			TEXT("官方网站"),
			WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			720, 380, 218, 28,
			hWnd,
			(HMENU)Btn_Web,
			appInstance,
			NULL);
		Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(WEBSITE_BMP));
		SendMessage(hButton_Web, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);

		hButton_Music = (HWND)CreateWindow(TEXT("Button"),
			TEXT("音效：关"),
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			800, 100, 80, 40,
			hWnd,
			(HMENU)Btn_Music,
			appInstance,
			NULL);

		hButton_MakePeople = (HWND)CreateWindow(TEXT("Button"),
			TEXT("制作人员"),
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
			720, 430, 218, 28,
			hWnd,
			(HMENU)Btn_MakePeople,
			appInstance,
			NULL);
		Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(People_BMP));
		SendMessage(hButton_MakePeople, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);

		hButton_Service = (HWND)CreateWindow(TEXT("Button"),
			TEXT("服务条款"),
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
			720, 480, 218, 28,
			hWnd,
			(HMENU)Btn_Service,
			appInstance,
			NULL);
		Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(Service_BMP));
		SendMessage(hButton_Service, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);

		hButton_Quit = (HWND)CreateWindow(TEXT("Button"),
			TEXT("退出游戏"),
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
			720, 600, 218, 28,
			hWnd,
			(HMENU)Btn_Quit,
			appInstance,
			NULL);
		Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(Quit_BMP));
		SendMessage(hButton_Quit, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);

		hEdit_username = (HWND)CreateWindow(TEXT("EDIT"),
			TEXT("输入用户名"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			350, 200, 300, 30,
			hWnd,
			(HMENU)Edt_username,
			hinst,
			NULL);

		hEdit_password = (HWND)CreateWindow(TEXT("EDIT"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_PASSWORD,
			350, 300, 300, 30,
			hWnd,
			(HMENU)Edt_password,
			hinst,
			NULL);

		break;

	case WM_COMMAND:
		//按钮——登录事件
		if (LOWORD(wParam) == Btn_login && HIWORD(wParam) == BN_CLICKED)
		{
			wchar_t szcaption[32];
			GetDlgItemText(hWnd, Edt_username, szcaption, 32);
			wcharTochar(szcaption, username, sizeof(username));
			GetDlgItemText(hWnd, Edt_password, szcaption, 32);
			wcharTochar(szcaption, password, sizeof(password));
			string t = md5(password);  //控件获得的密码以md5方式加密
			const char *passwd = t.data(); //格式转换
			//连接数据库,检测账号密码以及提取关卡
			MYSQL mysql;
			MYSQL_RES *resultset;
			MYSQL_ROW row;
			mysql_init(&mysql);
			if (mysql_real_connect(&mysql, db_host, db_user, db_passwd, db_name, db_port, NULL, 0))
			{
				if (mysql_query(&mysql, "select * from account"))
				{
					MessageBox(NULL, L"数据库查询发生错误", L"", NULL);
				}
				else{
					resultset = mysql_store_result(&mysql);// 获得结果集
					if (mysql_num_rows(resultset) != NULL){
						while (row = mysql_fetch_row(resultset)){
							if (strcmp(row[1], username) == 0){
								if (strcmp(row[2], passwd) == 0){
									Admit_Login = true;
									stage = atoi(row[4]) - 1;
									grade_db = atoi(row[5]);
									strcpy(nickname, row[3]);
								}
							}
						}
					}
					else{
						//printf("\n无查询结果!");
					}
					mysql_free_result(resultset);  // 释放结果集  
				}
				//准许登录
				if (Admit_Login)
				{
					MessageBox(hWnd, TEXT("登录成功"), TEXT(""), MB_OK);
					first = false;
					//销毁菜单按钮
					DestroyWindow(hButton_login);
					DestroyWindow(hButton_Web);
					DestroyWindow(hEdit_username);
					DestroyWindow(hEdit_password);
					DestroyWindow(hButton_Music);
					DestroyWindow(hButton_MakePeople);
					DestroyWindow(hButton_Service);
					DestroyWindow(hButton_Quit);
					//背景清除
					winlogin_DC = GetDC(hwnd);
					backgroundBMP_login = (HBITMAP)LoadImage(GetModuleHandleW(NULL), MAKEINTRESOURCE(BACK_GROUND), IMAGE_BITMAP, GAME_W, GAME_H, LR_DEFAULTCOLOR);
					background_loginDC = CreateCompatibleDC(winlogin_DC);
					SelectObject(background_loginDC, backgroundBMP_login);
					BitBlt(winlogin_DC, 0, 0, GAME_W, GAME_H, background_loginDC, 0, 0, SRCCOPY);
					ReleaseDC(0, background_loginDC);
					DeleteDC(winlogin_DC);
					//背景清除end
					//游戏模式
					hButton_Single = (HWND)CreateWindow(TEXT("Button"),
						TEXT("单人游戏"),
						WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
						150, 200, 250, 30,
						hWnd,
						(HMENU)Btn_Single,
						appInstance,
						NULL);
					Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(SINGLE_BMP));
					SendMessage(hButton_Single, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);

					hButton_Multi = (HWND)CreateWindow(TEXT("Button"),
						TEXT("多人对战"),
						WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
						150, 450, 250, 30,
						hWnd,
						(HMENU)Btn_Multi,
						appInstance,
						NULL);
					Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(Multi_BMP));
					SendMessage(hButton_Multi, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);
				}
				else {
					start_flag = false;
					MessageBox(NULL, L"    账号密码有误", L"登录错误", MB_OK);
				}
			}
			else{
				MessageBox(NULL, L"与服务器断开连接!", L"连接失败", MB_OK);
			}
			mysql_close(&mysql);
		}
		//按钮--音效事件
		if (LOWORD(wParam) == Btn_Music && HIWORD(wParam) == BN_CLICKED)
		{
			if (sound){
				sound = false; PlaySound(NULL, NULL, SND_FILENAME);
				SetWindowText(hButton_Music, L"音效：关");
			}
			else {
				sound = true; PLAYA(LOGIN);
				SetWindowText(hButton_Music, L"音效：开");
			}
		}
		//按钮--制作人员事件
		if (LOWORD(wParam) == Btn_MakePeople && HIWORD(wParam) == BN_CLICKED){
			MessageBox(hwnd, L"      Cc、橙汁", L"制作人员", MB_OK);
		}
		//按钮--服务条款事件
		if (LOWORD(wParam) == Btn_Service && HIWORD(wParam) == BN_CLICKED){
			MessageBox(hwnd, L"本站部分内容均来自互联网,仅作学习之用.", L"服务条款", MB_OK);
		}
		//按钮--官方网站事件
		if (LOWORD(wParam) == Btn_Web && HIWORD(wParam) == BN_CLICKED){
			ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("http://www.baidu.com"), NULL, SW_SHOW);
		}
		//按钮--退出游戏事件
		if (LOWORD(wParam) == Btn_Quit && HIWORD(wParam) == BN_CLICKED){
			MessageBox(hwnd, L"感谢使用！再见！", L"", MB_OK);
			PostQuitMessage(0);
		}
		//按钮--单人游戏事件
		if (LOWORD(wParam) == Btn_Single && HIWORD(wParam) == BN_CLICKED)
		{

			//修改clientORserver为0，表示单人游戏
			MYSQL mysql;
			mysql_init(&mysql);
			if (mysql_real_connect(&mysql, db_host, db_user, db_passwd, db_name, db_port, NULL, 0))
			{
				char *buffer = (char*)malloc(sizeof(char) * 128);
				sprintf(buffer, "UPDATE `battleworld`.`account` SET `cORs`='0'  WHERE `email`='%s';",  username);
				mysql_query(&mysql, buffer);
			}
			mysql_close(&mysql);

			DestroyWindow(hButton_Createroom);
			DestroyWindow(hButton_Joinroon);
			hButton_Goon = (HWND)CreateWindow(TEXT("Button"),
				TEXT("继续游戏"),
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
				450, 150, 250, 30,
				hWnd,
				(HMENU)Btn_Goon,
				appInstance,
				NULL);
			Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(Goon_BMP));
			SendMessage(hButton_Goon, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);

			hButton_Newgame = (HWND)CreateWindow(TEXT("Button"),
				TEXT("新的旅途"),
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
				450, 250, 250, 30,
				hWnd,
				(HMENU)Btn_Newgame,
				appInstance,
				NULL);
			Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(Newgame_BMP));
			SendMessage(hButton_Newgame, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);
		}
		//按钮--单人继续游戏事件
		if (LOWORD(wParam) == Btn_Goon && HIWORD(wParam) == BN_CLICKED)
		{
			DestroyWindow(hButton_Single);
			DestroyWindow(hButton_Goon);
			DestroyWindow(hButton_Newgame);
			DestroyWindow(hButton_Multi);
			DestroyWindow(hButton_Createroom);
			DestroyWindow(hButton_Joinroon);
			//游戏开始
			start_flag = true;
			Start();
		}
		//按钮--单人新的旅途事件
		if (LOWORD(wParam) == Btn_Newgame && HIWORD(wParam) == BN_CLICKED)
		{
			DestroyWindow(hButton_Single);
			DestroyWindow(hButton_Goon);
			DestroyWindow(hButton_Newgame);
			DestroyWindow(hButton_Multi);
			DestroyWindow(hButton_Createroom);
			DestroyWindow(hButton_Joinroon);
			//游戏开始
			start_flag = true;
			stage = 0; grade_db = 0;
			Start();
		}
		//按钮--多人对战事件
		if (LOWORD(wParam) == Btn_Multi && HIWORD(wParam) == BN_CLICKED)
		{
			DestroyWindow(hButton_Goon);
			DestroyWindow(hButton_Newgame);

			hButton_Createroom = (HWND)CreateWindow(TEXT("Button"),
				TEXT("创建房间"),
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
				450, 400, 250, 30,
				hWnd,
				(HMENU)Btn_Createroom,
				appInstance,
				NULL);
			Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(Createroom_BMP));
			SendMessage(hButton_Createroom, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);

			hButton_Joinroon = (HWND)CreateWindow(TEXT("Button"),
				TEXT("加入房间"),
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
				450, 500, 250, 30,
				hWnd,
				(HMENU)Btn_Joinroom,
				appInstance,
				NULL);
			Hbmp = LoadBitmap(appInstance, MAKEINTRESOURCE(Joinroom_BMP));
			SendMessage(hButton_Joinroon, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp);
		}
		//按钮--多人创建房间事件
		if (LOWORD(wParam) == Btn_Createroom && HIWORD(wParam) == BN_CLICKED)
		{
			//获取本机ip
			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
			{
				exit(0);
			}
			int nLen = 256;
			char hostname[20];
			gethostname(hostname, nLen);
			hostent *pHost = gethostbyname(hostname);
			LPSTR lpAddr = pHost->h_addr_list[0];
			struct in_addr inAddr;
			struct in_addr inAddr2;
			memmove(&inAddr, lpAddr, 4);
			strcpy(WiredIP, inet_ntoa(inAddr));
			//printf("有  线  IP地址：%s\n", inet_ntoa(inAddr));
			memmove(&inAddr2, lpAddr + 4, 4);
			strcpy(WirelessIP, inet_ntoa(inAddr2));
			//printf("无  线  IP地址：%s\n", inet_ntoa(inAddr2));
			//修改clientORserver为1，表示多人对战创建房间者,同时上传本机ip至服务器
			MYSQL mysql;
			mysql_init(&mysql);
			if (mysql_real_connect(&mysql, db_host, db_user, db_passwd, db_name, db_port, NULL, 0))
			{
				char *buffer = (char*)malloc(sizeof(char) * 128);
				sprintf(buffer, "UPDATE `battleworld`.`account` SET `cORs`='1', `WiredIP`='%s', `WirelessIP`='%s' WHERE `email`='%s';", WiredIP,WirelessIP,username);
				mysql_query(&mysql, buffer);
			}
			mysql_close(&mysql);
			::WSACleanup();

			DestroyWindow(hButton_Single);
			DestroyWindow(hButton_Goon);
			DestroyWindow(hButton_Newgame);
			DestroyWindow(hButton_Multi);
			DestroyWindow(hButton_Createroom);
			DestroyWindow(hButton_Joinroon);
			clientORserver = 1;
			Start_Server();
			start_flag = true;
			MapInit();
			Start_2();
		}
		//按钮--多人加入房间事件
		if (LOWORD(wParam) == Btn_Joinroom && HIWORD(wParam) == BN_CLICKED)
		{	//查询数据库，获取创建房间者的ip
			MYSQL mysql;
			MYSQL_RES *resultset = NULL;
			MYSQL_ROW row;
			mysql_init(&mysql);
			if (mysql_real_connect(&mysql, db_host, db_user, db_passwd, db_name, db_port, NULL, 0))
			{
				char *buffer = (char*)malloc(sizeof(char) * 128);
				//修改cORs 为2，表示加入房间者
				sprintf(buffer, "UPDATE `battleworld`.`account` SET `cORs`='2'  WHERE `email`='%s';", username);
				mysql_query(&mysql, buffer);
				//查询ip
				mysql_query(&mysql, "SELECT * from account WHERE cORs = '1' ");
				resultset = mysql_store_result(&mysql);
				if (mysql_num_rows(resultset) != NULL) {
					while (row = mysql_fetch_row(resultset))
					{
						strcpy(TargetIP, row[7]);
						strcpy(TargetIP2, row[8]);
					}
				}
				else {
					MessageBoxA(NULL, "当前无人创建房间", "", MB_OK);
				}
				mysql_free_result(resultset);
			}
			mysql_close(&mysql);

			DestroyWindow(hButton_Single);
			DestroyWindow(hButton_Goon);
			DestroyWindow(hButton_Newgame);
			DestroyWindow(hButton_Multi);
			DestroyWindow(hButton_Createroom);
			DestroyWindow(hButton_Joinroon);
			clientORserver = 2;
			Start_Client();
			start_flag = true;
			MapInit();
			Start_2();			
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		//登录界面背景
		if (first)
		{
			//背景图片加载
			winlogin_DC = GetDC(hwnd);
			backgroundBMP_login = (HBITMAP)LoadImage(GetModuleHandleW(NULL), MAKEINTRESOURCE(Client_BMP), IMAGE_BITMAP, GAME_W, GAME_H, LR_DEFAULTCOLOR);
			background_loginDC = CreateCompatibleDC(winlogin_DC);
			SelectObject(background_loginDC, backgroundBMP_login);
			BitBlt(winlogin_DC, 0, 0, GAME_W, GAME_H, background_loginDC, 0, 0, SRCCOPY);
			ReleaseDC(0, background_loginDC);
			DeleteDC(winlogin_DC);

			//静态文本用户名密码
			hFont = CreateFont(
				27, 0,    //高度, 宽取0表示由系统选择最佳值
				0, 0,    //文本倾斜，与字体倾斜都为0
				FW_HEAVY,    //粗体
				0, 0, 0,        //非斜体，无下划线，无中划线
				GB2312_CHARSET,    //字符集
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,        //一系列的默认值
				DEFAULT_PITCH | FF_DONTCARE,
				_T("宋体")    //字体名称
			);
			SetBkMode(hdc, TRANSPARENT);
			SelectObject(hdc, hFont);
			TextOut(hdc, 260, 300, str_password, (int)(_tcsclen(str_password)));
			TextOut(hdc, 260, 200, str_username, (int)(_tcsclen(str_username)));
		}
		else {
			HBRUSH hBrush = (HBRUSH)CreateSolidBrush(COLORREF RGB(200, 255, 230));
			hFont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
			SelectObject(hdc, hBrush);
			SelectObject(hdc, hFont);
			//背景颜色区域
			Rectangle(hdc, GAME_W, 0, GAME_W + 300, GAME_H + 28);
			//屏幕右侧数据信息
			LPCTSTR str1 = _T("敌军数量：");
			//变量转字符
			char  ch1[10];
			_itoa(enemy_total, ch1, 10);
			int num1 = MultiByteToWideChar(0, 0, ch1, -1, NULL, 0);
			wchar_t *wide1 = new wchar_t[num1];
			MultiByteToWideChar(0, 0, ch1, -1, wide1, num1);
			LPCTSTR str3 = wide1;

			SetMapMode(hdc, MM_TEXT);
			SetBkColor(hdc, RGB(200, 255, 230));
			TextOut(hdc, GAME_W + 10, 550, str1, (int)(_tcsclen(str1)));
			TextOut(hdc, GAME_W + 150, 550, str3, (int)(_tcsclen(str3)));

			LPCTSTR str2 = _T("我军生命：");
			char  ch[10];
			_itoa(player_num, ch, 10);

			int num = MultiByteToWideChar(0, 0, ch, -1, NULL, 0);
			wchar_t *wide = new wchar_t[num];
			MultiByteToWideChar(0, 0, ch, -1, wide, num);
			LPCTSTR str4 = wide;

			SetMapMode(hdc, MM_TEXT);
			TextOut(hdc, GAME_W + 10, 600, str2, (int)(_tcsclen(str2)));
			TextOut(hdc, GAME_W + 100, 600, str4, (int)(_tcsclen(str4)));

			//游戏说明信息
			LPCTSTR Game_Guide[22] = { L"游戏说明",
				L"  游戏模式:",
				L"    上下左右方向键移动坦克",
				L"    空格键开火",
				L"    P键暂停",
				L"    F1键切换到地图编辑模式",
				L"    F2键载入自制地图并游戏",
				L"  道具说明：",
				L"    绿色：提升坦克护甲，最多3次",
				L"    红色：提升炮弹射速，最多4次",
				L"    蓝色：提升坦克速度，最多4次",
				L"    黄色：提升炮弹数量，最多1次",
				L"  地图编辑模式：",
				L"    鼠标左键添加地图块",
				L"    1-7数字键切换地图块",
				L"    上下方向键切换关卡",
				L"    F1键切换到游戏模式",
				L"    F2键载入自制地图并编辑",
				L"    F3键保存地图",
			};
			for (int i = 0; i < 19; ++i){
				TextOut(hdc, GAME_W + 10, (i + 1) * 22, Game_Guide[i], (int)(_tcsclen(Game_Guide[i])));
			}
			EndPaint(hWnd, &ps);
		}
	}
	break;
	//窗口是否激活

	case WM_ACTIVATE:
		if (wParam != WA_INACTIVE)
			SetTimer(hwnd, GAME_ID, GAME_TIME, NULL);
		else
			KillTimer(hwnd, GAME_ID);
		break;

		//窗口最小化和还原
	case WM_SIZE:
		if (wParam == SIZE_RESTORED)
			SetTimer(hwnd, GAME_ID, GAME_TIME, NULL);
		else if (wParam == SIZE_MINIMIZED)
			KillTimer(hwnd, GAME_ID);
		break;

	case WM_MOVE:
		InvalidateRect(hWnd, NULL, false);
		break;

		//定时器响应	
	case WM_TIMER:
		if (start_flag)
		{
			switch (wParam)
			{
			case GAME_ID:
				//判断游戏状态	
				switch (game_state)
				{
				case GAME:
					if (clientORserver == 0)
						Game();
					else{
						Game_2();
					}
					break;
				case EDIT:
					Edit();
					break;
				case PAUSE:
					MessageBox(hwnd, L"游戏暂停！休息一下！", L"", MB_OK);
					game_state = GAME;
					break;
				case GAMEOVER:
					MessageBox(hwnd, L"游戏结束！再接再厉！", L"", MB_OK);
					PostQuitMessage(0);
					break;
				case NEXTSTAGE:
					if (++stage == stage_max)
						game_state = WIN;
					else
					{
						game_state = GAME;
						Init();
					}
					break;
				case WIN:
					MessageBox(hwnd, L"太厉害了，祝贺通关！", L"", MB_OK);
					Start();
					break;
				}
			}
		}
		break;

	case WM_MOUSEMOVE:
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);

		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			if (game_state == EDIT && stage > 0)
			{
				--stage;
				Init();
			}
			break;
		case VK_DOWN:
			if (game_state == EDIT && stage < stage_max - 1)
			{
				++stage;
				Init();
			}
			break;
		case 'P':
			if (game_state == GAME)
				game_state = PAUSE;
			break;
		case VK_F1:
			if (game_state == GAME)
			{
				if (MessageBox(hwnd, L"是否切换到地图编辑模式？", L"", MB_YESNO) == IDYES)
				{
					game_state = EDIT;
					stage = 0;
					Load();
					Init();
				}
			}
			else if (game_state == EDIT)
			{
				if (MessageBox(hwnd, L"是否切换到游戏模式？", L"", MB_YESNO) == IDYES)
				{
					MapInit();
					Start();
				}
			}
			break;
		case VK_F2:
			if (game_state == GAME || game_state == EDIT)
			{
				if (MessageBox(hwnd, L"是否载入自制地图？", L"", MB_YESNO) == IDYES)
				{
					Load();
					if (game_state == GAME)
						Start();
					else
					{
						stage = 0;
						Init();
					}
				}
			}
			break;
		case VK_F3:
			if (game_state == EDIT)
			{
				if (MessageBox(hwnd, L"是否保存自制地图？", L"", MB_YESNO) == IDYES)
				{
					Save();
				}
			}
		case VK_ESCAPE:
			//存储得分,排行榜
			char *rank = (char*)malloc(sizeof(char) * 250);
			memset(rank, NULL, 250);
			sprintf(rank, "   本次得分：%d\t 上次成绩：%d\n\n\n", grade, grade_db);
			strcat(rank, "\t  全球玩家排行榜\n\n");
			char sort[11][3] = { "0","1","2","3","4","5","6","7","8","9","10" };
			int j = 1;

			MYSQL mysql;
			MYSQL_RES *resultset = NULL;
			MYSQL_ROW row;
			mysql_init(&mysql);
			if (mysql_real_connect(&mysql, db_host, db_user, db_passwd, db_name, db_port, NULL, 0))
			{
				char *buffer = (char*)malloc(sizeof(char) * 128);
				sprintf(buffer, "UPDATE `battleworld`.`account` SET `gamestage`='%d', `grade`='%d'  WHERE `email`='%s';", stage + 1, grade + grade_db, username);
				//存储数据
				mysql_query(&mysql, buffer);
				//获取排行榜
				mysql_query(&mysql, "SELECT * from account ORDER BY grade DESC");
				resultset = mysql_store_result(&mysql);
				strcat(rank, "名次\t玩家\t关卡\t得分\n");
				while (row = mysql_fetch_row(resultset))
				{
					if (j <= 10)
					{
						strcat(rank, sort[j]);strcat(rank, "\t");
						strcat(rank, row[3]);strcat(rank, "\t");
						strcat(rank, row[4]);strcat(rank, "\t");
						strcat(rank, row[5]);strcat(rank, "\n");
						++j;
					}
					else
					{
						break;
					}
				}
				strcat(rank, "\n\n");
				mysql_free_result(resultset);
			}
			mysql_close(&mysql);

			if (MessageBoxA(hWnd, rank, "是否退出", MB_YESNO) == IDYES)
				PostQuitMessage(0);

		}
		break;
		//关闭按钮
	case WM_CLOSE:
		if (game_state == EDIT && MessageBox(hwnd, L"是否保存自制地图？", L"", MB_YESNO) == IDYES)
			Save();
		MessageBox(hwnd, L"感谢使用！再见！", L"", MB_OK);
		break;
		//销毁窗口
	case WM_DESTROY:
		//退出  
		closesocket(sServer);   //关闭套接字  
		closesocket(sClient);   //关闭套接字  
		closesocket(sHost); //关闭套接字  
		WSACleanup();           //释放套接字资源; 		
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//游戏开始
void Start()
{
	SetTimer(hwnd, GAME_ID, GAME_TIME, NULL);
	player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
	player_num = player_max;
	game_state = GAME;
	if (start_flag == true) {
		Init();
	}
}

//游戏过程
void Game()
{
	//玩家操作按键响应
	if (player_death == false)
		Key();
	//绘制游戏画面，区别对待单人和多人游戏
	if (clientORserver == 0)
		DrawGame();
	else
		DrawGame_2();
	//基地毁灭
	if (base_time > 0){
		if (--base_time == 0)
			game_state = GAMEOVER;
	}
	//过关
	if (wintime > 0){
		if (--wintime == 0)
			game_state = NEXTSTAGE;
	}
	//玩家死亡
	if (player_death == true)
	{
		if (--player_time == 0)
		{
			if (player_num == 0)
				game_state = GAMEOVER;
			else{
				//玩家复活
				player_death = false;
				player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
			}
			InvalidateRect(hwnd, NULL, false);
		}
	}
	//少于屏幕最大敌人数并且剩余敌人数大于零添加敌人
	if (enemy_num < enemy_maxscr && enemy_time == 0 && enemy_rest>0)
	{
		enemy_time = enemy_timemax;
	}
	if (enemy_time > 0)
	{
		--enemy_time;
		if (enemy_time == 0)
		{
			++enemy_num;
			--enemy_rest;
			int r = rand() % 10;
			int x = rand() % 3 * 7 * 64;
			//蓝色坦克
			if (r == 0)
				enemy_tank.push_back(new BaseTank(BLUE, 20, 0, DOWN, x, -64, DOWN, false, 8, 8, 4, 2, 1, 10, 12));
			//红色坦克
			else if (r == 1)
				enemy_tank.push_back(new BaseTank(RED, 50, 0, DOWN, x, -64, DOWN, true, 32, 2, 4, 3, 2, 10, 6));
			//金色坦克
			else if (r == 2)
				enemy_tank.push_back(new BaseTank(GOLD, 70, 0, DOWN, x, -64, DOWN, false, 16, 4, 4, 0, 2, 10, 8));
			//普通坦克
			else
				enemy_tank.push_back(new BaseTank(GREEN, 30, 0, DOWN, x, -64, DOWN, false, 32, 2, 4, 1, 1, 10, 6));
			//添加子弹容器
			enemy_bullet.push_back(new list<Bullet*>);
			//刷新剩余量绘图
			InvalidateRect(hwnd, NULL, false);
		}
	}
}

//按键判断
void Key()
{
	int temp_face = -1;//临时记录方向
	//秘籍
	if (KEYDOWN('Z'))
	{
		if (KEYDOWN(VK_UP))	{
			if (stage > 0)
				--stage;
			Init();
		}
		else if (KEYDOWN(VK_DOWN))	{
			if (stage < stage_max - 1)
				++stage;
			Init();
		}
		else if (KEYDOWN('F'))	{
			player_tank.life = 40;
			player_tank.id = player_tank.life / 10 - 1;
			player_tank.speed = 8;
			player_tank.fire_speed = 16;
			player_tank.bullet_max = 2;
			player_tank.bullet_speed = 24;
		}
	}
	//坦克行走
	else if (player_tank.move == 0)
	{		
		//行走
		if (KEYDOWN(VK_DOWN)){
			player_tank.Change(DOWN);
			player_tank.move = 64 / player_tank.speed;
			if(clientORserver)Send_Client(DOWN,player_tank.x,player_tank.y);//发送信号
		}
		else if (KEYDOWN(VK_LEFT))	{
			player_tank.Change(LEFT);
			player_tank.move = 64 / player_tank.speed;
			if(clientORserver)Send_Client(LEFT,player_tank.x,player_tank.y);//发送信号
		}
		else if (KEYDOWN(VK_UP))	{
			player_tank.Change(UP);
			player_tank.move = 64 / player_tank.speed;
			if(clientORserver)Send_Client(UP,player_tank.x,player_tank.y);//发送信号
		}
		else if (KEYDOWN(VK_RIGHT))	{
			player_tank.Change(RIGHT);
			player_tank.move = 64 / player_tank.speed;
			if(clientORserver)Send_Client(RIGHT,player_tank.x,player_tank.y);//发送信号
		}
		if (player_tank.move > 0)
		{
			bool hit = TankHitBlock(player_tank);
			player_tank.move = 64 / player_tank.speed_real;
			if (hit == true)
			{
				player_tank.move = 0;
				if (++player_tank.move_step > 2)
					player_tank.move_step = 0;
			}
			else	{
				for (list<BaseTank*>::iterator iter = enemy_tank.begin(); iter != enemy_tank.end(); ++iter)
				{
					BaseTank& tank = **iter;
					if (TankHitTank(player_tank, tank) == true)
					{
						player_tank.move = 0;
						if (++player_tank.move_step > 2)
							player_tank.move_step = 0;
						break;
					}
				}
			}
			if (sound)PLAYB(MOTOR);
		}
	}
	if (player_tank.move > 0)
	{
		--player_tank.move;
		if (++player_tank.move_step > 2)
			player_tank.move_step = 0;
		player_tank.Move();
		if (player_tank.move == 0)	{
			player_tank.x = (player_tank.x + 32) / 64 * 64;
			player_tank.y = (player_tank.y + 32) / 64 * 64;
		}
		
	}
	//转动炮台
	if (KEYDOWN('S')){
		if (sound)PLAYB(TURRET);
		player_tank.gun_face = DOWN;
	}
	else if (KEYDOWN('A'))	{
		if (sound)PLAYB(TURRET);
		player_tank.gun_face = LEFT;
	}
	else if (KEYDOWN('W'))	{
		if (sound)PLAYB(TURRET);
		player_tank.gun_face = UP;
	}
	else if (KEYDOWN('D'))	{
		if (sound)PLAYB(TURRET);
		player_tank.gun_face = RIGHT;
	}

	//锁定炮台
	if (keyup_L == true && KEYDOWN('L'))	{
		keyup_L = false;
		if (player_tank.gun_lock == false)player_tank.gun_lock = true;
		else player_tank.gun_lock = false;
	}
	else if (KEYUP('L'))keyup_L = true;
	//开火
	if (KEYDOWN(VK_SPACE)) {
		if (!player_tank.gun_step && player_tank.bullet_num < player_tank.bullet_max)
		{
			if (sound)PLAYA(SHOOT);
			int x = player_tank.x + (64 - 40) / 2;
			int y = player_tank.y + (64 - 40) / 2;
			switch (player_tank.gun_face)
			{
			case DOWN:
				y += 47;
				break;
			case LEFT:
				x -= 47;
				break;
			case UP:
				y -= 47;
				break;
			case RIGHT:
				x += 47;
			}
			player_bullet.push_back(new Bullet(player_tank.bullet_id, 100, player_tank.gun_face, x, y,
				player_tank.bullet_speed, player_tank.bullet_power));
			++player_tank.bullet_num;
			player_tank.Fire();
		}
	}
}

//画背景
void DrawBG(){
	BitBlt(writeDC, 0, 0, GAME_W, GAME_H, backgroundDC, 0, 0, SRCCOPY);
}

//输出缓冲区到屏幕
void Print(){
	BitBlt(winDC, 0, 0, GAME_W, GAME_H, writeDC, 0, 0, SRCCOPY);
}

//游戏单位绘制
void DrawGame()
{
	//绘制背景
	DrawBG();
	//绘制地图块
	DrawBlock();
	//处理和绘制道具
	Item();
	//处理和绘制敌人坦克
	EnemyTank();
	//处理和绘制玩家坦克
	if (player_death == false)
		player_tank.Draw(writeDC, player_tankDC, false);
	//绘制草块
	DrawGrass();
	//处理和绘制敌人炮弹
	EnemyBullet();
	//处理和绘制玩家炮弹
	PlayerBullet();
	//处理和绘制开火闪光
	player_tank.Draw(writeDC, player_tankDC, true);
	//绘制火花
	DrawFire();
	//输出到屏幕
	Print();
}

//初始化
void Init()
{
	//显示关卡
	char s[100];
	if (game_state == GAME)	{
		sprintf(s, "准备进入第%d关", stage + 1);
		if (sound)PLAYA(START);
	}
	else if (game_state == EDIT)
		sprintf(s, "第%d关地图编辑", stage + 1);
	MessageBoxA(hwnd, s, "", MB_OK);

	//存储关卡和得分
	MYSQL mysql;
	mysql_init(&mysql);
	if (mysql_real_connect(&mysql, db_host, db_user, db_passwd, db_name, db_port, NULL, 0))
	{
		char *buffer = (char*)malloc(sizeof(char) * 128);
		sprintf(buffer, "UPDATE `battleworld`.`account` SET `gamestage`='%d', `grade`='%d'  WHERE `email`='%s';", stage + 1, grade + grade_db, username);
		mysql_query(&mysql, buffer);
	}
	mysql_close(&mysql);
	//BOSS模式
	boss_mode = false;
	//玩家坦克
	player_tank = BaseTank(
		player_tank.id,
		player_tank.life,
		player_tank.armor,
		UP,
		5 * 64,
		10 * 64 + 64,
		UP,
		player_tank.gun_lock,
		64 / player_tank.speed,
		player_tank.speed,
		player_tank.fire_speed,
		player_tank.bullet_id,
		player_tank.bullet_max,
		player_tank.bullet_power,
		player_tank.bullet_speed);
	//以下为动态资源的释放
	//玩家子弹清空
	for (list<Bullet*>::iterator i = player_bullet.begin(); i != player_bullet.end(); ++i)
		delete *i;
	player_bullet.clear();
	//火花容器清空
	for (list<Fire*>::iterator i = fire.begin(); i != fire.end(); ++i)
		delete *i;
	fire.clear();
	//敌人容器清空
	for (list<BaseTank*>::iterator i = enemy_tank.begin(); i != enemy_tank.end(); ++i)
		delete *i;
	enemy_tank.clear();
	//敌人炮弹容器清空
	for (list<list<Bullet*>*>::iterator i = enemy_bullet.begin(); i != enemy_bullet.end(); ++i)
	{
		for (list<Bullet*>::iterator j = (**i).begin(); j != (**i).end(); ++j)
			delete *j;
		(**i).clear();
		delete *i;
	}
	enemy_bullet.clear();
	//关卡
	wintime = 0;
	//基地
	base_time = 0;
	//玩家恢复
	player_death = false;
	player_time = 0;
	//敌人数量
	enemy_num = 0;
	enemy_total = stage + 1;
	enemy_rest = enemy_total;
	enemy_time = 1;
	//砖块和水循环延时清零
	wall_time = 0;
	water_time = 0;
	//道具清零
	item = NONE;
	invin_time = 0;
	LoadMap();
}

//绘制地图块
void DrawBlock()
{
	int i;
	int x;
	int y;
	int x1;
	int y1;
	for (vector<int>::iterator iter = block.begin(); iter != block.end(); ++iter)
	{
		int &id = *iter;
		if (id == GRASS || (id == FLOOR && boss_mode == false))continue;
		//砖块循环
		if (wall_time == wall_timemax && id >= wall_min && id <= wall_max)	{
			if (++id > wall_max)id = wall_min;
		}
		else if (wall_time == wall_timemax && id >= wall1_min && id <= wall1_max)	{
			if (--id == wall1_min)id = wall_min;
		}
		//水块循环
		else if (water_time == water_timemax && id >= water_min && id <= water_max)	{
			if (++id >= water_max)id = water1_max;
		}
		else if (water_time == water_timemax && id >= water1_min && id <= water1_max)	{
			if (--id == water1_min)id = water_min;
		}
		//不等于雪块和草块就进行绘制
		i = (int)(iter - block.begin());
		x = i%block_wide * 64;
		y = i / block_wide * 64;
		x1 = id % 100 % 4 * 64;
		y1 = id % 100 / 4 * 128;
		BitBlt(writeDC, x, y, 64, 64, blockDC, x1, y1 + 64, SRCPAINT);
		BitBlt(writeDC, x, y, 64, 64, blockDC, x1, y1, SRCAND);
	}
	if (++wall_time > wall_timemax)wall_time = 0;
	if (++water_time > water_timemax)water_time = 0;
}
//处理玩家炮弹
void PlayerBullet()
{
	for (list<Bullet*>::iterator iter_bullet = player_bullet.begin(); iter_bullet != player_bullet.end();)
	{
		Bullet& bullet = **iter_bullet;
		bullet.Move();
		bool erase_bullet = false;
		//炮弹是否越界
		if (bullet.x + 76 - 1 < 0 || bullet.x - 36 > GAME_W - 1 || bullet.y + 76 - 1 < 0 || bullet.y - 36 > GAME_H - 1)
		{
			delete *iter_bullet;
			--player_tank.bullet_num;
			iter_bullet = player_bullet.erase(iter_bullet);
			erase_bullet = true;
		}
		//是否击中砖块
		else if (BulletHitBlock(bullet) == true)
		{
			if (sound)PLAYA(HIT);
			fire.push_back(new Fire(bullet.x - (66 - 40) / 2, bullet.y - (66 - 40) / 2));
			delete *iter_bullet;
			--player_tank.bullet_num;
			iter_bullet = player_bullet.erase(iter_bullet);
			erase_bullet = true;
		}
		else{
			list<BaseTank*>::iterator iter_tank;
			list<list<Bullet*>*>::iterator iter_enemy_bullet;
			//是否击中敌人
			for (iter_tank = enemy_tank.begin(), iter_enemy_bullet = enemy_bullet.begin();
				iter_tank != enemy_tank.end(); ++iter_tank, ++iter_enemy_bullet)
			{
				BaseTank& tank = **iter_tank;
				list<Bullet*>& b = **iter_enemy_bullet;
				if (BulletHitTank(bullet, tank) == true)
				{
					fire.push_back(new Fire(bullet.x - (66 - 40) / 2, bullet.y - (66 - 40) / 2));
					tank.life -= bullet.power - tank.armor;
					if (tank.life <= 0)
					{
						//敌方数量-1
						--enemy_total;
						//刷新剩余量绘图
						InvalidateRect(hwnd, NULL, false);
						//爆炸
						if (sound)PLAYA(BOMB);
						fire.push_back(new Fire(tank.x - 1, tank.y - 1));
						//添加道具
						switch (tank.id)
						{
						case GREEN:
							if (rand() % 7 == 0)
							{
								item = LIFE;
								item_x = tank.x;
								item_y = tank.y;
							}
							grade += 10;//加分
							break;
						case BLUE:
							if (rand() % 2 == 0)
							{
								item = SPEED;
								item_x = tank.x;
								item_y = tank.y;
							}
							grade += 20;
							break;
						case RED:
							if (rand() % 2 == 0)
							{
								item = POWER;
								item_x = tank.x;
								item_y = tank.y;
							}
							grade += 30;
							break;
						case GOLD:
							if (rand() % 2 == 0)
							{
								item = WEAPON;
								item_x = tank.x;
								item_y = tank.y;
							}
							grade += 40;
							break;
						}
						delete *iter_tank;
						//先释放空间
						for (list<Bullet*>::iterator i = b.begin(); i != b.end(); ++i)
							delete *i;
						b.clear();
						delete *iter_enemy_bullet;
						iter_tank = enemy_tank.erase(iter_tank);
						iter_enemy_bullet = enemy_bullet.erase(iter_enemy_bullet);
						if (--enemy_num == 0 && enemy_rest == 0)
							wintime = wintimemax;
					}
					else if (sound)PLAYA(HIT);
					delete *iter_bullet;
					--player_tank.bullet_num;
					iter_bullet = player_bullet.erase(iter_bullet);
					erase_bullet = true;
					break;
				}
				//是否击中敌人炮弹
				else{
					for (list<Bullet*>::iterator i = b.begin(); i != b.end(); ++i)
					{
						Bullet& bullet1 = **i;
						if (BulletHitBullet(bullet, bullet1) == true)
						{
							if (sound)PLAYA(HIT);
							fire.push_back(new Fire(bullet.x - (66 - 40) / 2, bullet.y - (66 - 40) / 2));
							delete *iter_bullet;
							--player_tank.bullet_num;
							iter_bullet = player_bullet.erase(iter_bullet);
							erase_bullet = true;
							fire.push_back(new Fire(bullet1.x - (66 - 40) / 2, bullet1.y - (66 - 40) / 2));
							delete *i;
							--tank.bullet_num;
							i = b.erase(i);
							break;
						}
					}
					if (erase_bullet == true)
						break;
				}
			}
		}
		//若炮弹没有爆炸就绘制出来
		if (erase_bullet == false)
			(*(iter_bullet++))->Draw(writeDC, bulletDC);
	}
}

//绘制草块
void DrawGrass()
{
	int i;
	int x;
	int y;
	int x1;
	int y1;
	for (vector<int>::iterator iter = block.begin(); iter != block.end(); ++iter)
		if (*iter == GRASS)
		{
			//草块绘制
			i = (int)(iter - block.begin());
			x = i%block_wide * 64;
			y = i / block_wide * 64;
			x1 = GRASS % 4 * 64;
			y1 = GRASS / 4 * 128;
			BitBlt(writeDC, x, y, 64, 64, blockDC, x1, y1 + 64, SRCPAINT);
			BitBlt(writeDC, x, y, 64, 64, blockDC, x1, y1, SRCAND);
		}
}
//绘制火花
void DrawFire()
{
	for (list<Fire*>::iterator iter = fire.begin(); iter != fire.end();)
	{
		(*iter)->Draw(writeDC, fireDC);
		bool erase = false;
		if ((*iter)->end == true)
		{
			delete (*iter);
			iter = fire.erase(iter);
			erase = true;
		}
		if (erase == false)++iter;
	}
}
//炮弹击中砖块
bool BulletHitBlock(Bullet& bullet)
{
	int x = bullet.x;
	int y = bullet.y;
	x = (x + 20) / 64;
	y = (y + 20) / 64;
	if (x < 0 || y < 0 || x >= block_wide || y >= block_high)
		return false;
	int& id = *(block.begin() + y*block_wide + x);
	int id1 = id % 100;
	if (id1 == STONE)
	{
		//if(bullet.power==max_power)
		//	id=FLOOR;
		return true;
	}
	if (id1 == BASE)
	{
		if (sound)PLAYA(BOMB);
		fire.push_back(new Fire(x * 64, y * 64));
		id = FLOOR;
		base_time = base_timemax;
		return true;
	}
	if (id1 >= WALL + 3 && id1 <= WALLMAX)
	{
		id = FLOOR;
		return true;
	}
	if (id1 >= WALL && id1 <= WALL + 2)
	{
		id += bullet.power / 10;
		if (id > WALL + 3)id = FLOOR;
		return true;
	}
	return false;
}
//坦克碰撞砖块
bool TankHitBlock(BaseTank& tank)
{
	int x = tank.x / 64;
	int y = tank.y / 64;
	switch (tank.face)
	{
	case DOWN:
		++y;
		break;
	case LEFT:
		--x;
		break;
	case UP:
		--y;
		break;
	case RIGHT:
		++x;
	}
	if (x < 0 || y < 0 || x >= block_wide || y >= block_high)
		return true;
	int& id = *(block.begin() + y*block_wide + x);
	int id1 = id % 100;
	tank.speed_real = tank.speed;
	switch (id1)
	{
	case ICE:
		tank.speed_real *= 2;
		break;
	case SAND:
		tank.speed_real = 1;
	}
	if (id1 == FLOOR || id1 == GRASS || id1 == ICE || id1 == SAND)
		return false;
	return true;
}
//炮弹击中坦克
bool BulletHitTank(Bullet& bullet, BaseTank& tank)
{
	int x1 = bullet.x + 20;
	int y1 = bullet.y + 20;
	int x2 = tank.x + 32;
	int y2 = tank.y + 32;
	if (abs(x1 - x2) < 32 && abs(y1 - y2) < 32)
		return true;
	return false;
}
//坦克碰撞坦克
bool TankHitTank(BaseTank& tank1, BaseTank& tank2)
{
	int x1 = tank1.x;
	int y1 = tank1.y;
	int x2 = tank2.x;
	int y2 = tank2.y;
	switch (tank1.face)
	{
	case DOWN:
		if ((x1 == x2 && y1 < y2 && y2 - y1 < 128 && y2 - y1 >= 64) || (y2 - y1 == 64 && abs(x1 - x2) < 64))
			return true;
		break;
	case LEFT:
		if ((y1 == y2 && x1 > x2 && x1 - x2 < 128 && x1 - x2 >= 64) || (x1 - x2 == 64 && abs(y1 - y2) < 64))
			return true;
		break;
	case UP:
		if ((x1 == x2 && y1 > y2 && y1 - y2 < 128 && y1 - y2 >= 64) || (y1 - y2 == 64 && abs(x1 - x2) < 64))
			return true;
		break;
	case RIGHT:
		if ((y1 == y2 && x1 < x2 && x2 - x1 < 128 && x2 - x1 >= 64) || (x2 - x1 == 64 && abs(y1 - y2) < 64))
			return true;
	}
	return false;
}
//炮弹碰撞炮弹
bool BulletHitBullet(Bullet& bullet1, Bullet& bullet2)
{
	int x1 = bullet1.x + 20;
	int y1 = bullet1.y + 20;
	int x2 = bullet2.x + 20;
	int y2 = bullet2.y + 20;
	switch (bullet1.face)
	{
	case DOWN:
	case UP:
		switch (bullet2.face)
		{
		case DOWN:
		case UP:
			if (abs(x1 - x2) < 20 && abs(y1 - y2) < 40)
				return true;
			break;
		case LEFT:
		case RIGHT:
			if (abs(x1 - x2) < 30 && abs(y1 - y2) < 30)
				return true;
			break;
		}
		break;
	case LEFT:
	case RIGHT:
		switch (bullet2.face)
		{
		case DOWN:
		case UP:
			if (abs(x1 - x2) < 30 && abs(y1 - y2) < 30)
				return true;
			break;
		case LEFT:
		case RIGHT:
			if (abs(x1 - x2) < 40 && abs(y1 - y2) < 20)
				return true;
		}
	}
	return false;
}
//处理和绘制敌人坦克
void EnemyTank()
{
	list<BaseTank*>::iterator iter_tank;
	list<list<Bullet*>*>::iterator iter_bullet;
	//按顺序处理所有敌方坦克
	for (iter_tank = enemy_tank.begin(),
		iter_bullet = enemy_bullet.begin();
		iter_tank != enemy_tank.end();
		++iter_tank,
		++iter_bullet)
	{
		BaseTank& t = **iter_tank;
		//随机转动炮台
		int r = t.gun_face;
		if (t.gun_lock == true && t.fire_step == 0 && rand() % 50 == 0 || (((r == DOWN && GAME_H - t.y == 64) || (r == LEFT && t.x == 0) ||
			(r == UP && t.y == 0) || (r == RIGHT && GAME_W - t.x == 64)) && t.fire_step == 0))
		{
			r = rand() % 4;
			t.gun_face = r;
			while ((r == DOWN && GAME_H - t.y == 64) || (r == LEFT && t.x == 0) ||
				(r == UP && t.y == 0) || (r == RIGHT && GAME_W - t.x == 64))
			{
				r = rand() % 4;
				t.gun_face = r;
			}
		}
		//开火
		if (t.gun_step == 0 && t.bullet_num < t.bullet_max && rand() % 20 == 0)
		{
			//PLAYA(SHOOT)
			int x = t.x + (64 - 40) / 2;
			int y = t.y + (64 - 40) / 2;
			switch (t.gun_face)
			{
			case DOWN:
				y += 47;
				break;
			case LEFT:
				x -= 47;
				break;
			case UP:
				y -= 47;
				break;
			case RIGHT:
				x += 47;
			}
			(**iter_bullet).push_back(new Bullet(t.bullet_id, 100, t.gun_face, x, y,
				t.bullet_speed, t.bullet_power));
			++t.bullet_num;
			t.Fire();
		}
		int oldface = t.face;
		//当步数减为零并且炮口闪光消失时随机生成移动方向和步数
		if (t.delay > 0)
		{
			if (++t.move_step > 2)
				t.move_step = 0;
			--t.delay;
		}
		if (t.move == 0 && t.delay == 0)
		{
			r = t.face;
			if (t.fire_step == 0 && rand() % 4 == 0 || (((r == DOWN && GAME_H - t.y == 64) || (r == LEFT && t.x == 0) ||
				(r == UP && t.y == 0) || (r == RIGHT && GAME_W - t.x == 64)) && t.fire_step == 0))
			{
				r = rand() % 4;
				t.Change(r);
				while ((r == DOWN && GAME_H - t.y == 64) || (r == LEFT && t.x == 0) ||
					(r == UP && t.y == 0) || (r == RIGHT && GAME_W - t.x == 64))
				{
					r = rand() % 4;
					t.Change(r);
				}

			}
			t.move = 64 / t.speed;
			//撞到砖块
			if (TankHitBlock(t) == true)
			{
				t.move = 0;
				t.delay = 5;
			}
			//撞到玩家坦克
			else if (player_death == false && TankHitTank(t, player_tank) == true)
			{
				//t.Change(oldface);
				t.move = 0;
				t.delay = 5;
			}
			//如果撞到敌方任意非己坦克……
			else
			{
				for (list<BaseTank*>::iterator i = enemy_tank.begin(); i != enemy_tank.end(); ++i)
					//如果不是自己本身并且撞到对方
					if (i != iter_tank && TankHitTank(t, **i) == true)
					{
						//t.Change(oldface);
						t.move = 0;
						break;
					}
			}
			if (t.move > 0)
				t.move = 64 / t.speed_real;
		}
		if (t.move > 0)
		{
			--t.move;
			if (++t.move_step > 2)
				t.move_step = 0;
			t.Move();
		}
		t.Draw(writeDC, player_tankDC, false);
	}
}
//处理和绘制敌人炮弹和炮口闪光
void EnemyBullet()
{
	list<BaseTank*>::iterator iter_tank;
	list<list<Bullet*>*>::iterator iter_bullet;
	//按顺序处理所有敌方坦克炮弹
	for (iter_tank = enemy_tank.begin(), iter_bullet = enemy_bullet.begin();
		iter_tank != enemy_tank.end(); ++iter_tank, ++iter_bullet)
	{
		BaseTank& tank = **iter_tank;
		list<Bullet*>& b = **iter_bullet;
		for (list<Bullet*>::iterator i = b.begin(); i != b.end();)
		{
			Bullet& bullet = **i;
			bullet.Move();
			int x = bullet.x;
			int y = bullet.y;
			bool erase = false;
			//炮弹是否越界
			if (bullet.x + 76 - 1 < 0 || bullet.x - 36 > GAME_W - 1 || bullet.y + 76 - 1 < 0 || bullet.y - 36 > GAME_H - 1)
			{
				delete *i;
				--tank.bullet_num;
				i = b.erase(i);
				erase = true;
			}
			//击中砖块
			else if (BulletHitBlock(bullet) == true)
			{
				//PLAYA(HIT)
				fire.push_back(new Fire(bullet.x - (66 - 40) / 2, bullet.y - (66 - 40) / 2));
				delete *i;
				--tank.bullet_num;
				i = b.erase(i);
				erase = true;
			}
			//击中玩家
			else if (player_death == false && BulletHitTank(bullet, player_tank) == true)
			{
				fire.push_back(new Fire(bullet.x - (66 - 40) / 2, bullet.y - (66 - 40) / 2));
				player_tank.life -= bullet.power - player_tank.armor;
				//玩家颜色改变									
				if (player_tank.life <= 0)
				{
					//玩家死亡
					if (sound)PLAYA(BOMB);
					fire.push_back(new Fire(player_tank.x - 1, player_tank.y - 1));
					player_death = true;
					--player_num;
					player_time = player_timemax;
				}
				else
					player_tank.id = player_tank.life / 10 - 1;
				delete *i;
				--tank.bullet_num;
				i = b.erase(i);
				erase = true;
			}
			//若炮弹没有爆炸就绘制出来
			if (erase == false)
				(*i++)->Draw(writeDC, bulletDC);
		}
		//绘制炮口闪光
		tank.Draw(writeDC, player_tankDC, true);
	}
}
//处理和绘制道具
void Item()
{
	if (item != NONE)
	{
		BitBlt(writeDC, item_x, item_y, 64, 64, blockDC, item * 64, 768 + 64, SRCPAINT);
		BitBlt(writeDC, item_x, item_y, 64, 64, blockDC, item * 64, 768, SRCAND);
		if (player_death == false && abs(player_tank.x - item_x) <= 32 && abs(player_tank.y - item_y) <= 32)
		{
			switch (item)
			{
				//坦克生命
			case LIFE:
				if (player_tank.life < max_life){
					player_tank.life += 10;
					player_tank.id = player_tank.life / 10 - 1;
				}
				break;
				//子弹射速
			case POWER:
				if (player_tank.bullet_speed < max_bullet_speed){
					player_tank.fire_speed += 2;
					player_tank.bullet_speed += 4;
				}
				break;
				//坦克速度
			case SPEED:
				if (player_tank.speed < max_speed){
					player_tank.speed += 1;
				}
				break;
				//武器弹数
			case WEAPON:
				if (player_tank.bullet_max < max_num)
					++player_tank.bullet_max;
			}
			item = NONE;
		}
	}
}
//地图编辑
void Edit()
{
	//绘制画面
	DrawBG();
	DrawBlock();
	DrawGrass();
	//在鼠标当前位置上绘制图块
	BitBlt(writeDC, mouse_x - 32, mouse_y - 32, 64, 64, blockDC, mouse_id % 4 * 64, mouse_id / 4 * 128 + 64, SRCPAINT);
	BitBlt(writeDC, mouse_x - 32, mouse_y - 32, 64, 64, blockDC, mouse_id % 4 * 64, mouse_id / 4 * 128, SRCAND);
	//输出画面
	Print();
	//编辑按键判断
	if (KEYDOWN(VK_LBUTTON)){
		blockmap[stage*block_wide*block_high + mouse_y / 64 * block_wide + mouse_x / 64] = mouse_id;
		block[mouse_y / 64 * block_wide + mouse_x / 64] = mouse_id;
	}
	else if (KEYDOWN(VK_RBUTTON)){
		blockmap[stage*block_wide*block_high + mouse_y / 64 * block_wide + mouse_x / 64] = FLOOR;
		block[mouse_y / 64 * block_wide + mouse_x / 64] = FLOOR;
	}
	else if (KEYDOWN('1'))
		mouse_id = WALL;
	else if (KEYDOWN('2'))
		mouse_id = STONE;
	else if (KEYDOWN('3'))
		mouse_id = GRASS;
	else if (KEYDOWN('4'))
		mouse_id = ICE;
	else if (KEYDOWN('5'))
		mouse_id = BASE;
	else if (KEYDOWN('6'))
		mouse_id = SAND;
	else if (KEYDOWN('7'))
		mouse_id = WATER;
}
//保存地图
void Save()
{
	map_file.open(map_name, fstream::out);
	if (!map_file){
		MessageBox(hwnd, L"地图保存失败！", L"错误", MB_OK | MB_ICONWARNING);
	}
	else{
		for (vector<int>::iterator i = blockmap.begin(); i != blockmap.end(); ++i)
		{
			map_file << *i;
			if (i != blockmap.end() - 1)
			{
				if (*i < 10)
					map_file << " ";
				map_file << " , ";
			}
			if ((i - blockmap.begin() + 1) % block_wide == 0)
				map_file << endl;
			if ((i - blockmap.begin() + 1) % (block_wide*block_high) == 0)
				map_file << endl;
		}
		map_file.clear();
		map_file.close();
		MessageBox(hwnd, L"地图保存成功！", L"", MB_OK);
	}
}
//载入地图
void Load()
{
	map_file.open(map_name, fstream::in);
	if (!map_file || map_file.eof())
	{
		MessageBox(hwnd, L"载入地图失败！", L"错误", MB_OK | MB_ICONWARNING);
	}
	else{
		string s;
		for (vector<int>::iterator i = blockmap.begin(); i != blockmap.end(); ++i)
		{
			map_file >> *i;
			if (i != blockmap.end() - 1)
				map_file >> s;
		}
	}
	map_file.clear();
	map_file.close();
}
//载入预置地图
void LoadMap(){
	block.assign(blockmap.begin() + block_wide*block_high*stage,
		blockmap.begin() + block_wide*block_high*stage + block_wide*block_high);
}
//全地图初始化
void MapInit(){
	if (clientORserver == 0) {
		blockmap.assign(map, map + block_wide*block_high*map_max);
	}
	else {
		blockmap.assign(map_2, map_2 + block_wide*block_high*map_max_2);
	}
}
