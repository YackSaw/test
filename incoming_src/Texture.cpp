#include ".\texture.h"
#define RELEASE(p) { if(p){(p)->Release();p=NULL;} };
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define Cont 0

CTexture::CTexture(void)
{
	gl_Texture=NULL;
}

BOOL CTexture::InitTexture(int Position)
{
	switch(Position){
		case PosL:
			InitVertex(			//左キャラ用頂点データ初期化
			VertexDataTbl,
			0.0f,
			0.0f+Cont,
			512.0f,
			512.0f+Cont
			);
			break;
		case PosC:
			InitVertex(			//中央キャラ用頂点データ初期化
			VertexDataTbl,
			160.0f,
			0.0f+Cont,
			160.0f+512.0f,
			512.0f+Cont
			);
			break;
		case PosR:
			InitVertex(			//右キャラ用頂点データ初期化
			VertexDataTbl,
			320.0f,
			0.0f+Cont,
			320.0f+512.0f,
			512.0f+Cont
			);
			break;
		case PosBG:
			InitVertex(			//背景用頂点データ初期化
			VertexDataTbl,
			0.0f,
			0.0f+Cont,
			1024.0f,
			1024.0f+Cont
			);
			break;
		case PosText:
			InitVertex(
				VertexDataTbl,
				10.0f,
				380.0f-2.0f+Cont,
				1034.0f,
				1404.0f-2.0f+Cont
				//,128
				);
			break;
		case -1:
			return FALSE;
		default:			//こりゃあかんで
			return FALSE;
	}
	


	return 0;
}



CTexture::~CTexture(void)
{
	RELEASE(gl_Texture);
}

BOOL CTexture::SetTexWnd(LPDIRECT3DDEVICE9 g_lpD3DDevice)
{
	RELEASE(gl_Texture);
	
	HRESULT hr;

	hr=D3DXCreateTextureFromResourceEx(
		g_lpD3DDevice,
		NULL,
		MAKEINTRESOURCE(IDR_TEXWND),
		1024,
		1024,
		0,
		0,
		D3DFMT_UNKNOWN,
		//D3DFMT_A8R3G3B2,	//Formatを色抜き可能にする
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0,255,0),	//透過色設定
		NULL,
		NULL,
		&gl_Texture);
	Visible=TRUE;
	if(hr!=D3D_OK)MessageBox(NULL,"dame","",MB_OK);
	return TRUE;
}


BOOL CTexture::SetHaikei(LPDIRECT3DDEVICE9 g_lpD3DDevice,int resID){
	RELEASE(gl_Texture);
	
	HRESULT hr;
	hr=D3DXCreateTextureFromResourceEx(
		g_lpD3DDevice,
		NULL,
		MAKEINTRESOURCE(resID),
		//D3DX_DEFAULT,
		//D3DX_DEFAULT,
		1024,
		1024,
		0,
		0,
		D3DFMT_UNKNOWN,	//Format
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		//D3DCOLOR_XRGB(0,255,0),	//透過色設定
		0,
		NULL,
		NULL,
		&gl_Texture);
	//if(hr==D3DXERR_INVALIDDATA)MessageBox(NULL,"d3dx Invaliddata","",0);
	
	LastBG=resID;

	return TRUE;
}



BOOL CTexture::SetChar(LPDIRECT3DDEVICE9 g_lpD3DDevice,int resID){
	RELEASE(gl_Texture);
	HRESULT hr;
	hr=D3DXCreateTextureFromResourceEx(
		g_lpD3DDevice,
		NULL,
		MAKEINTRESOURCE(resID),
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		0,
		D3DFMT_UNKNOWN,
		//D3DFMT_A8R8G8B8,	//Formatを色抜き可能にする
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0,255,0),	//透過色設定
		NULL,
		NULL,
		&gl_Texture);
	//if(hr!=D3D_OK){
		//if(hr==D3DXERR_INVALIDDATA)	MessageBox(NULL,"Not OK char","s",MB_OK);
	//}
	if(resID==IDR_INV){
		Visible=FALSE;
	}else{
		Visible=TRUE;
	}
	LastBG=resID;
	LastChar=resID;
    return TRUE;
}

BOOL CTexture::Draw(LPDIRECT3DDEVICE9 g_lpD3DDevice)
{
	g_lpD3DDevice->SetVertexShader(NULL);
	g_lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	g_lpD3DDevice->SetTexture(0, gl_Texture);
    // 四角形ポリゴンをバックバッファに転送
    g_lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, VertexDataTbl, sizeof(TLVERTX));
	return 0;
}

void CTexture::InitVertex(LPTLVERTEX v, float x1, float y1, float x2, float y2,int Alpha)
{
	D3DCOLOR    color;  // 頂点カラー

    // 半透明処理しまス?
	switch(Alpha){
		case 255:
			color=D3DCOLOR_XRGB(255,255,255);
			break;
		default:
			color=D3DCOLOR_RGBA(255,255,255,Alpha);
			break;
	}

    //頂点配列をゼロクリア
    ZeroMemory(v, sizeof(TLVERTX));

	v[0].x=x1;		//頂点X
    v[0].y=y1;		//頂点Y
    v[0].z=0.0f;	//頂点Z
    v[0].rhw=1.0f;
    v[0].tu=0.0f;	//テクスチャX
    v[0].tv=0.0f;	//テクスチャY
    v[0].color=color;	//頂点色

	v[1].x=x2;
    v[1].y=y1;
    v[1].z=0.0f;
    v[1].rhw=1.0f;
    v[1].tu=1.0f;
    v[1].tv=0.0f;
    v[1].color=color;

	v[2].x=x2;
    v[2].y=y2;
    v[2].z=0.0f;
    v[2].rhw=1.0f;
    v[2].tu=1.0f;
    v[2].tv=1.0f;
    v[2].color=color;

    v[3].x=x1;
    v[3].y=y2;
    v[3].z=0.0f;
    v[3].rhw=1.0f;
    v[3].tu=0.0f;
    v[3].tv=1.0f;
    v[3].color=color;
}

BOOL CTexture::Destroy(LPDIRECT3DDEVICE9 g_lpD3DDevice,int Pos)
{
	if(Visible==TRUE){											//見えているなら
		if(Pos!=PosBG){
			SetChar(g_lpD3DDevice,IDR_INV);		//消す
			//MessageBox(NULL,"hide","message",MB_OK);
		}else{
			SetHaikei(g_lpD3DDevice,IDR_INV);			//一応記述
		}
		return TRUE;
	}else{
		//MessageBox(hWnd,"invisible","message",MB_OK);//見えていないなら何もしない
		return FALSE;
	}
}
