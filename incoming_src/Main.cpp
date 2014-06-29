#define MDI_CHILD TEXT("MDICHILD")
#define MDI_FRAME TEXT("MDIFRAME")
#define MUTEX_NAME "atoso,IncomingNoiz,2006-2007 unkopuu~"			//mutex名

#pragma once
#include "windows.h"
#include "resource.h"
#include "DrawGraphics.h"
#include "TextWnd.h"
#include "WheelMouse.h"


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK DlgProc(HWND,UINT,WPARAM,LPARAM);

CTextWnd *CT;

HANDLE m_hMutex;
CWheelMouse wmouse;

bool ENABLEMENU(HMENU hm){
	EnableMenuItem(hm,ID_SAVE_1,MF_ENABLED);
	EnableMenuItem(hm,ID_SAVE_2,MF_ENABLED);
	EnableMenuItem(hm,ID_SAVE_3,MF_ENABLED);
	return true;
}


int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
	WNDCLASSEX wcex;

	m_hMutex=OpenMutex(
		MUTEX_ALL_ACCESS,
		FALSE,
		MUTEX_NAME
		);
	if(m_hMutex){
		CloseHandle(m_hMutex);
		return -1;
	}else{
		m_hMutex=CreateMutex(
			NULL,
			TRUE,
			MUTEX_NAME
			);
	}

	CT=new CTextWnd;

	HWND hWnd;
	MSG msg;

	wcex.cbSize=sizeof(WNDCLASSEX);
	wcex.style=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
	wcex.lpfnWndProc=(WNDPROC)WndProc;
	wcex.cbClsExtra=0;
	wcex.cbWndExtra=0;
	wcex.hInstance=hInstance;
	wcex.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor=LoadCursor(NULL,IDC_ARROW);
	wcex.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName="ModelApp";
	wcex.hIconSm=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));


	RegisterClassEx(&wcex);

	hWnd=CreateWindow(
		wcex.lpszClassName,
		"IncomingNoiz version 1.01",
		//WS_OVERLAPPEDWINDOW,
		WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_CLIPCHILDREN,
        CW_USEDEFAULT,
		CW_USEDEFAULT,
		640+6,//CW_USEDEFAULT,//width
		480+3*SM_CYMENU-1,//CW_USEDEFAULT,//height
		NULL,
		NULL,
		hInstance,
		NULL,
		);

	CT->Init(hWnd);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	FILE *fp0;//セーブしたデータがあるかどうか調べてメニューに反映させる
	MENUITEMINFO mii;
	HMENU hm;
	hm=GetMenu(hWnd);
	if((fp0=fopen("data/save1.dat","r"))==NULL){
		EnableMenuItem(hm,ID_LOAD_1,MF_GRAYED);
	}else{
		char Buff[30];
		char *Num=" --data1--";
		fgets(Buff,20,fp0);
		strcat(Buff,Num);
		EnableMenuItem(hm,ID_LOAD_1,MF_ENABLED);
		mii.cbSize=sizeof(MENUITEMINFO);
		//if(GetMenuItemInfo(hm,ID_LOAD_1,FALSE,&mii)==0)MessageBox(NULL,"get menu item error","",0);
		mii.fMask=MIIM_TYPE;
		mii.fType=MFT_STRING;
		mii.fState=MFS_ENABLED;
		mii.dwTypeData=(LPTSTR)Buff;
		mii.cch=strlen(Buff);
		if(SetMenuItemInfo(hm,ID_LOAD_1,FALSE,&mii)==0)MessageBox(NULL,"set menu item error","LOAD1",0);
		if(SetMenuItemInfo(hm,ID_SAVE_1,FALSE,&mii)==0)MessageBox(NULL,"set menu item error","SAVE1",0);
	}

	if((fp0=fopen("data/save2.dat","r"))==NULL){
		EnableMenuItem(hm,ID_LOAD_2,MF_GRAYED);
	}else{
		char Buff[30];
		char *Num=" --data2--";
		fgets(Buff,20,fp0);
		strcat(Buff,Num);
		EnableMenuItem(hm,ID_LOAD_2,MF_ENABLED);
		mii.cbSize=sizeof(MENUITEMINFO);
		//if(GetMenuItemInfo(hm,ID_LOAD_1,FALSE,&mii)==0)MessageBox(NULL,"get menu item error","",0);
		mii.fMask=MIIM_TYPE;
		mii.fType=MFT_STRING;
		mii.fState=MFS_ENABLED;
		mii.dwTypeData=(LPTSTR)Buff;
		mii.cch=strlen(Buff);
		if(SetMenuItemInfo(hm,ID_LOAD_2,FALSE,&mii)==0)MessageBox(NULL,"set menu item error","LOAD2",0);
		if(SetMenuItemInfo(hm,ID_SAVE_2,FALSE,&mii)==0)MessageBox(NULL,"set menu item error","SAVE2",0);
	}

	if((fp0=fopen("data/save3.dat","r"))==NULL){
		EnableMenuItem(hm,ID_LOAD_3,MF_GRAYED);
	}else{
		char Buff[30];
		char *Num=" --data3--";
		fgets(Buff,20,fp0);
		strcat(Buff,Num);
		EnableMenuItem(hm,ID_LOAD_1,MF_ENABLED);
		mii.cbSize=sizeof(MENUITEMINFO);
		//if(GetMenuItemInfo(hm,ID_LOAD_1,FALSE,&mii)==0)MessageBox(NULL,"get menu item error","",0);
		mii.fMask=MIIM_TYPE;
		mii.fType=MFT_STRING;
		mii.fState=MFS_ENABLED;
		mii.dwTypeData=(LPTSTR)Buff;
		mii.cch=strlen(Buff);
		if(SetMenuItemInfo(hm,ID_LOAD_3,FALSE,&mii)==0)MessageBox(NULL,"set menu item error","LOAD3",0);
		if(SetMenuItemInfo(hm,ID_SAVE_3,FALSE,&mii)==0)MessageBox(NULL,"set menu item error","SAVE3",0);
	}
	
	while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK DlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam){
	switch(message){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
		case IDOK:
			EndDialog(hDlg,IDOK);
			return 0;
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	int id;
	HINSTANCE hInstance=(HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE);

	HDC hdc;
	PAINTSTRUCT ps;
	static  bool title;
	static	HMENU hm;
	hm=GetMenu(hWnd);
	switch(wmouse.TranslateMessage(message))
	{
	case WM_CREATE:
		title=true;
		EnableMenuItem(hm,ID_SAVE_1,MF_GRAYED);
		EnableMenuItem(hm,ID_SAVE_2,MF_GRAYED);
		EnableMenuItem(hm,ID_SAVE_3,MF_GRAYED);
		return 0;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		CT->ReDraw();
		EndPaint(hWnd,&ps);
		return 0;
	case WM_KEYDOWN:
		//MessageBox(NULL , tcStr , TEXT("Key Event") , MB_OK);
		if(GetKeyState(VK_CONTROL)<0&&CT->TexWndVisible==TRUE){
			CT->CtrlOn=TRUE;
			if(CT->inBLog==FALSE&&CT->mode==0)CT->Update();
			CT->inBLog=FALSE;
			CT->ReDraw();
			CT->CtrlOn=FALSE;
		}
		if(GetKeyState(0xD)<0){
			//MessageBox(NULL,"enter","",MB_OK);
			//Enterが押された時
			if(title==true){
				CT->Hide();
				title=false;
				ENABLEMENU(hm);
			}
			if(CT->TexWndVisible==TRUE){
				if(CT->inBLog==FALSE&&CT->mode==0)CT->Update();
				CT->inBLog=FALSE;
				CT->ReDraw();
			}
		}

		return 0;
	case WM_MOUSEMOVE:
		bool pos;
		if(CT->mode==1){
			pos=false;
			if(LOWORD(lParam)>TexLeft/*ポインタがTexWindowの左端より右*/&&
				LOWORD(lParam)<TexLeft+620/*ポインタがTexWindowの右端より左*/&&
				HIWORD(lParam)>TexTop+5/*ポインタがTexWindowの上端より下*/&&
				HIWORD(lParam)<TexTop+100/*ポインタがTexWindowの下端より上*/&&
				CT->TexWndVisible==TRUE){
				//MessageBox(NULL,"in texWnd","",MB_OK);
					pos=true;
					CT->CurSel((int)(HIWORD(lParam)-TexTop+18-5)/18);
				}else if(pos==true&&HIWORD(lParam)<TexTop+5){
					CT->ReDraw();
					pos=false;
				}
				
		}
		return 0;
	case WM_LBUTTONDOWN:
		//if(LOWORD(lParam)>TexLeft&&LOWORD(lParam)<TexLeft+620&&HIWORD(lParam)>TexTop){
			//MessageBox(NULL,"clicked in texWnd","",MB_OK);
		//}
		if(title==true){
			CT->Hide();
			title=false;
			ENABLEMENU(hm);
		}
		if(CT->TexWndVisible==TRUE){
			if(CT->inBLog==FALSE&&CT->mode==0)CT->Update();
			if(CT->mode==1&&CT->SelectedBunki>0){
				CT->BFlag=CT->SelectedBunki;
				if(LOWORD(lParam)>TexLeft/*ポインタがTexWindowの左端より右*/&&
				LOWORD(lParam)<TexLeft+620/*ポインタがTexWindowの右端より左*/&&
				HIWORD(lParam)>TexTop+5/*ポインタがTexWindowの上端より下*/&&
				HIWORD(lParam)<TexTop+100/*ポインタがTexWindowの下端より上*/){
					CT->Update();
					CT->SelectedBunki=0;
				}
			}
			CT->inBLog=FALSE;
			CT->ReDraw();
		}
		return 0;
	case WM_RBUTTONDOWN:
		//MessageBox(NULL,"R","",MB_OK);
		if(!title)CT->Hide();
		return 0;
	case WM_MOUSEWHEEL:
		if(wmouse.IsForward(wParam)){
			CT->DispLogFw();
			//バックログ進む
			//MessageBox(NULL,"Forward","",MB_OK);
		}else{
			CT->DispLog();
			//バックログ戻る
			//MessageBox(NULL,"Backward","",MB_OK);
		}
		return 0;
		
	//COMMAND_
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case ID_FILE_QUIT:
			SendMessage(hWnd,WM_CLOSE,wParam,lParam);
			return 0;
		case ID_HELP_VERSIONINFO:
			DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),hWnd,(DLGPROC)DlgProc);
			SendMessage(hWnd,WM_PAINT,wParam,lParam);
			return 0;
		case ID_SAVE_1:
			CT->Save('1');
			return 0;
		case ID_SAVE_2:
			CT->Save('2');
			return 0;
		case ID_SAVE_3:
			CT->Save('3');
			return 0;
		case ID_LOAD_1:
			if(title==true){
				title=false;
				ENABLEMENU(hm);
				CT->Hide();
			}
			CT->Load('1');
			return 0;
		case ID_LOAD_2:
			if(title==true){
				title=false;
				ENABLEMENU(hm);
				CT->Hide();
			}
			CT->Load('2');
			return 0;
		case ID_LOAD_3:
			if(title==true){
				title=false;
				ENABLEMENU(hm);
				CT->Hide();
			}
			CT->Load('3');
			return 0;
			}
		return 0;
	case WM_CLOSE:
		id=MessageBox(hWnd,"終了しますか?","確認",MB_YESNO | MB_ICONQUESTION);
		if(id==IDYES){
			CT->Destroy();
			PostQuitMessage(0);
			ReleaseMutex(m_hMutex);//mutex開放
		}
		return 0;
	default:
		return DefWindowProc(hWnd,message,wParam,lParam);
	}
}