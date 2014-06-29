#define AppWidth 640
#define AppHeight 480
#define FVF_TLVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define Title	1001
#define Loop	1002
#define FadeIn	1
#define FadeOut 0
#define AFade 51
#define BLACKOUT 0
#define WHITEOUT 1
#define FadeTime 10
//#define DIRECTX_VERSION 0x00040009

#pragma once
#include "Texture.h"
#include "GSM.h"

class CDrawGraphics
{
	HWND hWnd;
	LPDIRECT3D9 g_lpD3D;
	D3DPRESENT_PARAMETERS param;
	LPDIRECT3DDEVICE9 g_lpD3DDevice;
	LPDIRECT3DSURFACE9 g_lpBackBuffer;

	LPD3DXSPRITE g_lpSprite;
	LPDIRECT3DTEXTURE9 g_lpTexture;

	

	RECT rt;
public:
	CTexture *CharL,*Haikei,*CharR,*CharC,*OldHaikei;
	CTexture *TextWnd;
	BOOL InitFlag;
	BOOL FadeFlag;
	BOOL CtrlOn;
	int CharCZ;
	//ï`âÊÇÃèáî‘
	int OutFlag;
	CDrawGraphics(void);
	InitCDG(HWND g_hWnd);
	~CDrawGraphics(void);

BOOL Update(void);

BOOL DelChar(int Position,int FadeOr=0);
BOOL Disp(int resID,int Position,int loadFlag=0);
BOOL SetCharaCZ(int Z);
BOOL Fade(CTexture *Tex,int Fade=1);


BOOL DispOut(int Flag);
BOOL Shake(int kaisuu=9,BOOL ch=FALSE);

};
