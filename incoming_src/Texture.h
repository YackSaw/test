#define FVF_TLVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define PosL	0
#define PosC	1
#define PosR	2
#define PosBG	3
#define PosText 4

#pragma once
#include "d3d9.h"
#include "d3dx9.h"
#include "d3dx9tex.h"
#include "dxerr9.h"
#include "windows.h"
#include "resource.h"

#define TexLeft 10
#define TexTop 380

class CDrawGraphics;
class CTextWnd;
class CTexture
{
	LPDIRECT3DTEXTURE9 gl_Texture;
protected:
	typedef struct _TLVERTEX {
    float           x, y, z;                //位置情報
    float           rhw;                    //頂点変換値
    D3DCOLOR        color;                  //頂点カラー
    float           tu, tv;                 //テクスチャ座標
	} TLVERTX,*LPTLVERTEX;
	TLVERTX VertexDataTbl[4];
public:
	friend CDrawGraphics;
	friend CTextWnd;
	int LastBG;
	int LastChar;
	char LastBGName[40];
	BOOL Visible;
	CTexture(void);
	~CTexture(void);
	void InitVertex(LPTLVERTEX v,float x1,float y1,float x2,float y2,int Alpha=255);
	BOOL SetHaikei(LPDIRECT3DDEVICE9 g_lpD3DDevice,int resID);
	BOOL SetChar(LPDIRECT3DDEVICE9 g_lpD3DDevice,int resID);
	BOOL Draw(LPDIRECT3DDEVICE9 g_lpD3DDevice);

	BOOL Destroy(LPDIRECT3DDEVICE9 g_lpD3DDevice,int Pos);
	BOOL InitTexture(int Position);
	BOOL SetTexWnd(LPDIRECT3DDEVICE9 g_lpD3DDevice);
};
