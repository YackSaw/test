#pragma once
#include "windows.h"
#include "resource.h"
#include "DrawGraphics.h"


#define WM_HIDE		0x99
#define WM_SHOW		0x98

#define BLogSize 21

#include <stdio.h>
#include <string.h>

class CTextWnd
{

	HWND hWndP;	//親ウィンドウのハンドル
	HDC hdc;
	HRGN WndShape[2];
	HFONT hFont,hFontOld;
	RECT rt;
	int iSize;
	FILE *fp,*frp;

	char *ScenarioTex;
	char id[1];

	char TexWndMsg[256];
	char path[276];
	BOOL isEnd;

	//リソースID
	int resID;
	//リソースの表示位置
	int resPos;
	//真ん中の画像の表示レベル
	int CZ;
	//分岐は何番か
	int Bunki;
	//分岐は何個あるか
	int alBN;
	//分岐関連///
	//直前の分岐番号
	int lastB;

	//BGM on
	bool isBGMon;

	BOOL Find;

	bool CanRead;

	//resPositionを指定
	//0->左
	//1->中
	//2->右
	//3->背景
	int resIDBuf[4];
	int CZBuf;
	fpos_t pos;
	char Tmp[100];
	int S,h;

	//バックログ用配列
	char BLog[BLogSize][256];
	//queueのトップ
	int q_top;
	//queueの最後
	int q_end;
	//次にqueueに入れる場所
	int q_next;
	//queue満杯フラグ
	int q_isFull;

	int LogNum;
public:
	//選択肢か？
	int mode;
	//何番が選択されたの？
	int SelectedBunki;
	//選択肢比較
	int BFlag;

	BOOL TexWndVisible;
	//CtrlはOnか
	BOOL CtrlOn;
	//バックログ表示中フラグ
	BOOL inBLog;

	CTextWnd(void);
	~CTextWnd(void);
	Init(HWND hWnd);
	HRESULT OnPaint();
	BOOL MyDrawText(char text[]);
	HFONT SetMyFont(LPCTSTR face, int h,int angle=0);
	void InitFile(void);
	void Destroy(void);
	BOOL Update(void);
	BOOL Save(char place='0');
	BOOL ReDraw(void);
	BOOL Hide(void);
	BOOL Load(char place='0');
	BOOL Queue(char* Message);
	BOOL DispLog(void);
	BOOL DispLogFw(void);
	BOOL CurSel(int pos);
	char* Decode(char *pcIn);
	bool SaveEnv(void);
	bool LoadEnv(void);
};
