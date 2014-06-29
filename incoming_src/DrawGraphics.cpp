#include ".\drawgraphics.h"

#define RELEASE(p) { if(p){(p)->Release();p=NULL;} };
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define MB(c) MessageBox(NULL,c,"",0);
//�h�ꕝ
//#define border 21
#define BD 27
//���̈ړ���
//#define dbd 5

CDrawGraphics::CDrawGraphics(){
	CharL=NULL;
	CharR=NULL;
	CharC=NULL;
	Haikei=NULL;
	OldHaikei=NULL;
	TextWnd=NULL;
}

CDrawGraphics::InitCDG(HWND g_hWnd)
{
	//����������
	CharL=new CTexture;
	CharR=new CTexture;
	CharC=new CTexture;
	Haikei=new CTexture;
	OldHaikei=new CTexture;
	TextWnd=new CTexture;

	FadeFlag=FALSE;
	D3DDISPLAYMODE  DispMode;					// �f�B�X�v���C���[�h
	CharCZ=1;									//�����L�����̕\�����ʂ�ݒ�:1=�O�A0=���
	OutFlag=0;									//�u���b�N/�z���C�g�A�E�g�t���O
	HRESULT hr;
	hWnd=g_hWnd;
	g_lpD3D = NULL;
	g_lpD3D = Direct3DCreate9( D3D_SDK_VERSION);
	
	ZeroMemory( &param, sizeof(param) );
	g_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DispMode);
    param.BackBufferFormat=DispMode.Format;				//�o�b�N�o�b�t�@�t�H�[�}�b�g���f�B�X�v���C�Ɠ�����
	param.BackBufferCount=1;
	param.SwapEffect=D3DSWAPEFFECT_DISCARD;
	param.EnableAutoDepthStencil=TRUE;
    param.AutoDepthStencilFormat=D3DFMT_D16;
	param.Windowed=TRUE;                   //�E�C���h�E���[�h
	
	// �f�o�C�X�쐬
	g_lpD3DDevice=NULL;

	//�n�[�h�E�F�A�̐��\�ɂ���Đ�������f�o�C�X��ς���悤�ɏC��
	//�����\�ȃn�[�h�E�F�A�f�o�C�X�̐��������݂�
	hr=g_lpD3D->CreateDevice(
	D3DADAPTER_DEFAULT,
	D3DDEVTYPE_HAL,                      //�n�[�h�E�F�A
	g_hWnd,
	D3DCREATE_HARDWARE_VERTEXPROCESSING,
	&param,
	&g_lpD3DDevice
	);

	if(FAILED(hr)){
		    hr=g_lpD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			g_hWnd,
            D3DCREATE_MIXED_VERTEXPROCESSING,
			&param,
			&g_lpD3DDevice
			);
		if(FAILED(hr)){
			hr=g_lpD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				g_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&param,
				&g_lpD3DDevice
				);
			if(FAILED(hr)){
				hr=g_lpD3D->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					g_hWnd,
					D3DCREATE_HARDWARE_VERTEXPROCESSING,
					&param,
					&g_lpD3DDevice
					);
					if(FAILED(hr)){
						hr=g_lpD3D->CreateDevice(
							D3DADAPTER_DEFAULT,
							D3DDEVTYPE_REF,
							g_hWnd,
							D3DCREATE_MIXED_VERTEXPROCESSING,
							&param,
							&g_lpD3DDevice
							);
					if(FAILED(hr)){
						//�\�t�g�E�F�A�f�o�C�X�̐��������݂�
						hr=g_lpD3D->CreateDevice(
							D3DADAPTER_DEFAULT,
							D3DDEVTYPE_REF,
							g_hWnd,
							D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							&param,
							&g_lpD3DDevice
							);
						if(FAILED(hr)){
							//�f�o�C�X�������s
							MessageBox(g_hWnd,"DirectX�f�o�C�X�������s","�G���[",MB_OK); 
						}
					}
				}
			}
        }
	}

	g_lpD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_NONE);
	g_lpD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
	g_lpD3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);

	g_lpD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	g_lpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_lpD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//BackBuffer
	g_lpBackBuffer = NULL;
	g_lpD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &g_lpBackBuffer );


	CharL->InitTexture(PosL);
	CharR->InitTexture(PosR);
	CharC->InitTexture(PosC);
	Haikei->InitTexture(PosBG);
	OldHaikei->InitTexture(PosBG);
	TextWnd->InitTexture(PosText);
	TextWnd->SetTexWnd(g_lpD3DDevice);
	
	g_lpD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 0, 0 );
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if(InitFlag==TRUE){
		Haikei->SetHaikei(g_lpD3DDevice,184);
	}else{
		//MessageBox(NULL,"view after there,DrawGraphics.cpp","",0);
		Haikei->SetHaikei(g_lpD3DDevice,IDR_SORA);
	}
	CharL->SetChar(g_lpD3DDevice,IDR_INV);
	CharC->SetChar(g_lpD3DDevice,IDR_INV);
	CharR->SetChar(g_lpD3DDevice,181);

}

CDrawGraphics::~CDrawGraphics(void)
{
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	// ���
	RELEASE(g_lpD3D);
	RELEASE(g_lpD3DDevice);
	RELEASE(g_lpBackBuffer);
	SAFE_DELETE(CharL);
	SAFE_DELETE(CharR);
	SAFE_DELETE(CharC);
	SAFE_DELETE(Haikei);
	SAFE_DELETE(OldHaikei);
	SAFE_DELETE(TextWnd);
	RELEASE(g_lpTexture);
	RELEASE(g_lpSprite);
	//MessageBox(hWnd,"destract","test",MB_OK);
}


BOOL CDrawGraphics::Update(void)
{
	//��ʍX�V
    if( ! g_lpD3D )			return FALSE;
	if( ! g_lpD3DDevice )	return FALSE;
	g_lpD3DDevice->BeginScene();

	switch(OutFlag){
		case 0:
			OldHaikei->Draw(g_lpD3DDevice);
			Haikei->Draw(g_lpD3DDevice);
			switch(CharCZ){
				case 0:
					CharL->Draw(g_lpD3DDevice);
					CharR->Draw(g_lpD3DDevice);
					CharC->Draw(g_lpD3DDevice);
					break;
				default:
					CharC->Draw(g_lpD3DDevice);
					CharL->Draw(g_lpD3DDevice);
					CharR->Draw(g_lpD3DDevice);
					break;
			}

			break;
		case 1:
			OldHaikei->Draw(g_lpD3DDevice);
			switch(CharCZ){
				case 0:
					CharL->Draw(g_lpD3DDevice);
					CharR->Draw(g_lpD3DDevice);
					CharC->Draw(g_lpD3DDevice);
					break;
				default:
					CharC->Draw(g_lpD3DDevice);
					CharL->Draw(g_lpD3DDevice);
					CharR->Draw(g_lpD3DDevice);
					break;
			}
			Haikei->Draw(g_lpD3DDevice);
			break;
		default:
			break;
	}
	TextWnd->Draw(g_lpD3DDevice);

	g_lpD3DDevice->EndScene();
	g_lpD3DDevice->Present(NULL,NULL,NULL,NULL);
	return 0;
}

BOOL CDrawGraphics::DelChar(int Position,int FadeOr)
{
	//�L������\��
	switch(Position){
		case PosL:
			if(FadeOr==0)Fade(CharL,FadeOut);
			//CharL->SetChar(g_lpD3DDevice,IDR_INV);
			CharL->Destroy(g_lpD3DDevice,PosL);
			break;
		case PosC:
			if(FadeOr==0)Fade(CharC,FadeOut);
			//CharC->SetChar(g_lpD3DDevice,IDR_INV);
			CharC->Destroy(g_lpD3DDevice,PosC);
			break;
		case PosR:
			if(FadeOr==0)Fade(CharR,FadeOut);
			//CharR->SetChar(g_lpD3DDevice,IDR_INV);
			CharR->Destroy(g_lpD3DDevice,PosR);
			break;
		case PosBG:
			if(FadeOr==0)Fade(Haikei,FadeOut);
			Haikei->Destroy(g_lpD3DDevice,PosBG);
			break;
		default:
			return FALSE;
	}

	return 0;
}


BOOL CDrawGraphics::Disp(int resID,int Position,int loadFlag)
{
	//�L�����\��
	switch(Position){
		case PosL:
			if(CharL->Visible==FALSE||loadFlag==1){
				if(CharL->LastChar!=resID){
					CharL->SetChar(g_lpD3DDevice,resID);
					Fade(CharL,FadeIn);
				}
			}
			break;
		case PosC:
			if(CharC->Visible==FALSE||loadFlag==1){
				if(CharC->LastChar!=resID){
					CharC->SetChar(g_lpD3DDevice,resID);
					Fade(CharC,FadeIn);
				}
			}
			break;
		case PosR:
			if(CharR->Visible==FALSE||loadFlag==1){
				if(CharR->LastChar!=resID){
					CharR->SetChar(g_lpD3DDevice,resID);
					Fade(CharR,FadeIn);
				}
			}
			break;
		case PosBG:
			if(Haikei->LastBG!=resID||loadFlag==1){
				if(CtrlOn==FALSE)OldHaikei->SetHaikei(g_lpD3DDevice,Haikei->LastBG);
				Haikei->SetHaikei(g_lpD3DDevice,resID);
				if(CtrlOn==FALSE){
					Fade(Haikei,FadeIn);
					OldHaikei->Destroy(g_lpD3DDevice,PosBG);
				}
			}
			break;
		default:
			return 0;
	}
	
	return 0;
}

BOOL CDrawGraphics::SetCharaCZ(int Z)
{
	//�^�񒆃L�����̕\���D��x
	CharCZ=Z;
	return 0;
}


BOOL CDrawGraphics::Fade(CTexture *Tex,int Fade)
{
	//�t�F�[�h����
	DWORD t=0;
	DWORD tp=0;
	//Ctrl��������Ă����
	//�s�����łȂ��Ȃ�Β���
	if(CtrlOn==TRUE){
		if(Tex->VertexDataTbl[3].color==D3DCOLOR_RGBA(255,255,255,255))return 0;
		for(int i=0;i<4;i++){
			Tex->VertexDataTbl[i].color=D3DCOLOR_RGBA(255,255,255,255);
		}
		return 0;
	}
	

	FadeFlag=TRUE;
	
	switch(Fade){
		MSG msg;
		case FadeOut:
			t=GetTickCount();
			tp=t;
			for(int Alpha=255;Alpha>=0;Alpha-=AFade){
				while(tp+FadeTime>=t){
					t=GetTickCount();
				}
				tp=t;
				for(int i=0;i<4;i++){
					Tex->VertexDataTbl[i].color=D3DCOLOR_RGBA(255,255,255,Alpha);
				}
				if(CtrlOn==FALSE)PeekMessage(&msg,NULL,0,0,PM_REMOVE);//�t�F�[�h���̃N���b�N���͖���(Ctrl�L�[������������)
				Update();
			}
			break;
		case FadeIn:
			//MessageBox(NULL,"fadein","",MB_OK);
			t=GetTickCount();
			tp=t;
			for(int Alpha=0;Alpha<=255;Alpha+=AFade){
				while(tp+FadeTime>=t){
					t=GetTickCount();
				}
				tp=t;
				for(int i=0;i<4;i++){
					Tex->VertexDataTbl[i].color=D3DCOLOR_RGBA(255,255,255,Alpha);
				}
				if(CtrlOn==FALSE)PeekMessage(&msg,NULL,0,0,PM_REMOVE);//�t�F�[�h���̃N���b�N���͖���(Ctrl�L�[������������)
				Update();
			}
			break;
		default:
			return 0;
	}
	
	FadeFlag=FALSE;
	return 0;
}

//white out or black out effect
BOOL CDrawGraphics::DispOut(int Flag/*,int resID*/)
{
	if(CtrlOn==TRUE){
		//MessageBox(NULL,"ctrl","",MB_OK);
		OutFlag=0;
		return 0;
	}

	OldHaikei->SetHaikei(g_lpD3DDevice,Haikei->LastBG);
	//OutFlag=1;
	switch(Flag){
		case BLACKOUT:
            Haikei->SetHaikei(g_lpD3DDevice,IDR_BL1);
			break;
		case WHITEOUT:
			Haikei->SetHaikei(g_lpD3DDevice,IDR_WH1);
			break;
		default:
			break;
	}
	Fade(Haikei,FadeIn);
	return 0;
}


BOOL CDrawGraphics::Shake(int kaisuu,BOOL ch)
{
	DWORD t=0;
	DWORD tp=0;
	//if(CtrlOn==TRUE)return 0;
	int i=-1,j=0,k=0,l=0,border;
	MSG msg;
	border=BD;
	int dbd=border/3;
	while(k<4){
		OldHaikei->VertexDataTbl[k].color=D3DCOLOR_XRGB(0,0,0);
		k++;
	}

	t=GetTickCount();
	tp=t;
	while(j<=kaisuu){
		i=i*(-1);
		//border�܂ňړ�������
		while((dbd*l)<border){
			k=0;
			while(k<4){
				//�e���_�̈ړ�
				//�L�������h�炷���H
				switch(j){
					case 0:
						Haikei->VertexDataTbl[k].y+=(dbd*i);
						switch(ch){
							case TRUE:
								CharL->VertexDataTbl[k].y+=(dbd*i);
								CharR->VertexDataTbl[k].y+=(dbd*i);
								CharC->VertexDataTbl[k].y+=(dbd*i);
								break;
							default:
								break;
						}
						break;
					case 9:
						Haikei->VertexDataTbl[k].y+=(dbd*i);
						switch(ch){
							case TRUE:
								CharL->VertexDataTbl[k].y+=(dbd*i);
								CharR->VertexDataTbl[k].y+=(dbd*i);
								CharC->VertexDataTbl[k].y+=(dbd*i);
								break;
							default:
								break;
						}
						break;
					default:
						Haikei->VertexDataTbl[k].y+=(dbd*i)*2;
						switch(ch){
							case TRUE:
								CharL->VertexDataTbl[k].y+=(dbd*i)*2;
								CharC->VertexDataTbl[k].y+=(dbd*i)*2;
								CharR->VertexDataTbl[k].y+=(dbd*i)*2;
								break;
							default:
								break;
						}
						break;
				}
				k++;
			}
			
			//�`��
			if(CtrlOn==FALSE)PeekMessage(&msg,NULL,0,0,PM_REMOVE);
			Update();
			while(tp+(FadeTime)>=t){
				t=GetTickCount();
			}
			tp=t;
			l++;
		}
		l=0;
		j++;
	}
	k=0;
	while(k<4){
		OldHaikei->VertexDataTbl[k].color=D3DCOLOR_XRGB(255,255,255);
		k++;
	}
	return 0;
}
