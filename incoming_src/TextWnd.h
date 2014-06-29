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

	HWND hWndP;	//�e�E�B���h�E�̃n���h��
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

	//���\�[�XID
	int resID;
	//���\�[�X�̕\���ʒu
	int resPos;
	//�^�񒆂̉摜�̕\�����x��
	int CZ;
	//����͉��Ԃ�
	int Bunki;
	//����͉����邩
	int alBN;
	//����֘A///
	//���O�̕���ԍ�
	int lastB;

	//BGM on
	bool isBGMon;

	BOOL Find;

	bool CanRead;

	//resPosition���w��
	//0->��
	//1->��
	//2->�E
	//3->�w�i
	int resIDBuf[4];
	int CZBuf;
	fpos_t pos;
	char Tmp[100];
	int S,h;

	//�o�b�N���O�p�z��
	char BLog[BLogSize][256];
	//queue�̃g�b�v
	int q_top;
	//queue�̍Ō�
	int q_end;
	//����queue�ɓ����ꏊ
	int q_next;
	//queue���t�t���O
	int q_isFull;

	int LogNum;
public:
	//�I�������H
	int mode;
	//���Ԃ��I�����ꂽ�́H
	int SelectedBunki;
	//�I������r
	int BFlag;

	BOOL TexWndVisible;
	//Ctrl��On��
	BOOL CtrlOn;
	//�o�b�N���O�\�����t���O
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
