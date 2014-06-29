#include ".\TextWnd.h"
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
CDrawGraphics cdg;
CTextWnd::CTextWnd(void)
{
	InitFile();
	char buff[256];
	//id初期化
	strcpy(id,"");
	//ファイルポインタ位置初期化
	fseek(frp,0,SEEK_SET);
	//メッセージ初期化
	strcpy(TexWndMsg,"");
	rt.top=12+TexTop;
	rt.left=13+TexLeft;
	rt.right=620;
	rt.bottom=480;
	CanRead=true;
	SelectedBunki=0;
	Find=FALSE;
	q_top=0;
	q_end=0;
	q_next=0;
	q_isFull=FALSE;
	//分岐用フラグ
	BFlag=0;
	lastB=0;
	LogNum=BLogSize;
	inBLog=FALSE;
	alBN=0;
	mode=0;
	isBGMon=false;
}

CTextWnd::~CTextWnd(void)
{
	//MessageBox(NULL,"","dest",0);
	//SAFE_DELETE(cdg);
	SelectObject(hdc,hFontOld);
	DeleteObject(hFont);
}

CGSM gsm;

CTextWnd::Init(HWND hWnd){
	//cdg=NULL;
	//cdg=new CDrawGraphics;
	static char *szChildName="child";
	hWndP=hWnd;
	CtrlOn=FALSE;

	//すでにクリアしているか
	if(LoadEnv()){
		cdg.InitFlag=TRUE;
	}
	cdg.InitCDG(hWnd);

	hdc=GetDC(hWnd);

	SetBkMode(hdc,TRANSPARENT);
	hFont=SetMyFont((LPCTSTR)"ＭＳ 明朝",18);
	hFontOld=(HFONT)SelectObject(hdc,hFont);
	SetTextColor(hdc, RGB(255,255,255));

	SelectObject(hdc,CreateSolidBrush(0X000099));
	//use R_operation
	SetROP2(hdc,R2_MERGEPENNOT);

//-----------------非表示状態でウィンドウを作成----------------------------
	TexWndVisible=TRUE;
//-------------------------------------------------------------------------	
	for(int i=0;i<4;i++){
			resIDBuf[i]=IDR_INV;
		}

	Hide();
	gsm.loadSeg(0,"data/sound/hazimari.mid",true);
	gsm.Play(TRUE,0);
}

HRESULT CTextWnd::OnPaint()
{
	if(TexWndVisible==TRUE){
		if(inBLog==TRUE){
			SetTextColor(hdc,RGB(255,255,0));
			MyDrawText(BLog[LogNum]);
		}else{
			SetTextColor(hdc,RGB(255,255,255));
			MyDrawText(TexWndMsg);
		}
	}
	return 0;
}

BOOL CTextWnd::Hide(void)
{
	if(TexWndVisible){
		for(int i=0;i<4;i++){
			cdg.TextWnd->VertexDataTbl[i].color=D3DCOLOR_RGBA(255,255,255,0);
		}
		cdg.Update();
		TexWndVisible=FALSE;
	}else{
		for(int i=0;i<4;i++){
			cdg.TextWnd->VertexDataTbl[i].color=D3DCOLOR_RGBA(255,255,255,255);
		}
		cdg.Update();
		TexWndVisible=TRUE;
		OnPaint();
	}
		
	return 0;
}

BOOL CTextWnd::MyDrawText(char text[])
{
	BOOL A=DrawText(hdc,text,-1,&(rt),DT_WORDBREAK);
	if(A==0)MessageBox(NULL,"failed draw text","",MB_OK);

	return 0;
}


HFONT CTextWnd::SetMyFont(LPCTSTR face, int h,int angle)
{
	HFONT hFont;
	hFont=CreateFont(
		h,0,
		angle,0,
		FW_HEAVY,
		FALSE,
		FALSE,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		DEFAULT_PITCH|FF_MODERN,
		face
		);
	return hFont;
}

char* CTextWnd::Decode(char *pcIn)
{
	char pcOut[256];
	bool rflag;
	rflag=false;
	for(int i=0;i<strlen(pcIn);i++){
		if(rflag==true&&pcIn[i]=='\n'){
    			pcOut[i-1]='\r';
    			pcOut[i]='\n';
    			rflag=false;
    	}else if(pcIn[i]=='\r'){
    			if(rflag==true){
    				pcOut[i-1] = ~pcIn[i-1];
    			}
    			rflag=true;
    	}else{
			if(pcIn[i]==('\n'+1)){
				pcOut[i]=~pcIn[i]+1;
			}else{
    			pcOut[i] = ~pcIn[i];
			}
    			if(rflag==true){
    				rflag=false;
    				pcOut[i-1] = ~pcIn[i-1];
    			}
    	}
	}
	return pcOut;
}




void CTextWnd::InitFile(void)
{
	ScenarioTex="data/scenario.dat";
	frp=fopen(ScenarioTex,"rb");
	char *pcInBuffer = new char[iSize];
	char *pcOutBuffer = new char[iSize];
	fseek(frp,0,SEEK_SET);
}
void CTextWnd::Destroy(void)
{
	DeleteObject(SelectObject(hdc , CreateSolidBrush(0xFF)));
	fclose(frp);
}


BOOL CTextWnd::Save(char place)
{
	FILE *fp;
	char szBuf[20];
	MENUITEMINFO mii;
	HMENU hm;
	hm=GetMenu(hWndP);

	if(place=='0'){
		fp=fopen("data/save.dat","wb+");
	}else{
		strcpy(szBuf,"data/save");
		strcat(szBuf,&place);
		strcat(szBuf,".dat");
		fp=fopen(szBuf,"wb+");
	}

	//日付の書き込み
	SYSTEMTIME st;
	GetLocalTime(&st);
	wsprintf(szBuf,"%04d/%02d/%02d,%02d:%02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	fseek(fp,0,SEEK_SET);
	fputs(szBuf,fp);
	fputs("\n",fp);

	mii.cbSize=sizeof(MENUITEMINFO);
	mii.fMask=MIIM_TYPE;
	mii.fType=MFT_STRING;
	mii.fState=MFS_ENABLED;
	mii.dwTypeData=(LPTSTR)szBuf;
	mii.cch=strlen(szBuf);
	EnableMenuItem(hm,ID_LOAD_1+int(place)-49,MF_ENABLED);
	if(SetMenuItemInfo(hm,ID_LOAD_1+int(place)-49,FALSE,&mii)==0)MessageBox(NULL,"set menu item error","",0);
	if(SetMenuItemInfo(hm,ID_SAVE_1+int(place)-49,FALSE,&mii)==0)MessageBox(NULL,"set menu item error","",0);

	
	//ファイルポインタの書き込み
	sprintf(szBuf,"%d",S);
	char szBufOut[20];
	for(int i=0;i<20;i++)
	{
		szBufOut[i] = ~szBuf[i];
	}
	fputs(szBufOut,fp);
	fputs("\n",fp);
	
	//中の表示レベルは？
	sprintf(szBuf,"%d",CZBuf);
	fputs(szBuf,fp);
	fputs("\n",fp);
	
	//キャラリソースIDの書き込み
	for(int i=0;i<4;i++){
		sprintf(szBuf,"%d",resIDBuf[i]);
		fputs(szBuf,fp);
		fputs("\n",fp);
	}

	//何番の選択肢を選んだか
	sprintf(szBuf,"%d",BFlag);
	fputs(szBuf,fp);
	fputs("\n",fp);
	//直前の選択肢
	sprintf(szBuf,"%d",lastB);
	fputs(szBuf,fp);
	fputs("\n",fp);
	//BGM
	if(isBGMon==true){
		fputs("1",fp);
		fputs("\n",fp);
		sprintf(szBuf,"%s",path);
		fputs(szBuf,fp);
	}else{
		fputs("0",fp);
		fputs("\n",fp);
	}

	fclose(fp);
	MessageBox(NULL,"セーブされました","",MB_OK);
	return 0;
}

BOOL CTextWnd::Load(char place)
{
	FILE *fp;
	char Buff[80];
	isEnd=FALSE;

	gsm.Stop(1,1);
	if(place=='0'){
		fp=fopen("data/save.dat","rb+");
	}else{
		strcpy(Buff,"data/save");
		strcat(Buff,&place);
		strcat(Buff,".dat");
		fp=fopen(Buff,"rb");
	}
	if(fp==NULL){
		MessageBox(NULL,"セーブファイルの展開に失敗","file open error_TextWnd->cpp",MB_OK);
		return FALSE;
	}

	int fpNum=0;
	fseek(fp,0,SEEK_SET);
	fgets(Buff,20,fp);
	fgets(Buff,2,fp);

	//ファイルポインタの読み込み
	fgets(Buff,80,fp);
	char BufOut[20];
	for(int i=0;i<20;i++)
	{
		BufOut[i] = ~Buff[i];
	}

	fpNum=atoi(BufOut)-1;

	
	//真ん中キャラの表示レベルは？
    CZ=atoi(fgets(Buff,2,fp));
	fgets(Buff,2,fp);
	cdg.SetCharaCZ(CZ);

	int inv;
	//キャラリソースIDの読み込み
	cdg.CtrlOn=TRUE;
	for(int i=0;i<4;i++){
		inv=atoi(fgets(Buff,4,fp));
		switch(inv){
			case IDR_INV:
				cdg.DelChar(i,1);
				break;
			default:
				cdg.Disp(inv,i,1);
				break;
		}
		fgets(Buff,2,fp);
	}
	cdg.CtrlOn=FALSE;

	//選択肢番号の読み込み
	strcat(Buff,"");
	BFlag=atoi(fgets(Buff,2,fp));
	fgets(Buff,2,fp);

	Bunki=BFlag;
	strcat(Buff,"");
	lastB=atoi(fgets(Buff,2,fp));
	fgets(Buff,2,fp);
	if(atoi(fgets(Buff,2,fp))==1){
		fgets(Buff,2,fp);
		strcpy(path,fgets(Buff,272,fp));
		int n=strlen(path);
		gsm.loadSeg(0,path,TRUE);
		gsm.Play(TRUE,0);
		isBGMon=true;
	}else if(atoi(fgets(Buff,2,fp))==0){
		isBGMon=false;
	}
	int rel;
	rel=fseek(frp,fpNum,SEEK_SET);
	if(rel!=0)MessageBox(NULL,"load error","",MB_OK);
	fclose(fp);
	Update();
	return TRUE;
}

BOOL CTextWnd::ReDraw(void)
{
	cdg.Update();
	OnPaint();
	return 0;
}

BOOL CTextWnd::Update(void)
{	
	//描画順をリセット
	cdg.OutFlag=0;
	if(isEnd==TRUE){
		MyDrawText("おわり");
		return 0;
	}
	//フェードフラグが立っている間Updateしない
	if(cdg.FadeFlag==TRUE){
		MessageBox(NULL,"FF on","",MB_OK);
		return 0;
	}
	if(CtrlOn==TRUE){
		cdg.CtrlOn=TRUE;
	}else{
		cdg.CtrlOn=FALSE;
	}

	static char Message[256]="";
	char DispMsg[256]="";
	char resName[12]="";
	int tmp;
	
	resID=-1;
	resPos=-1;
	
	CZ=1;

	strcpy(Message,"");
	fgets(id,sizeof(char)+1,frp);
	strncpy(id,Decode(id),1);


	if(strcmp(id,"#")==0){
		//選択肢モード
		mode=0;
		//選択肢総数リセット
		alBN=0;
		Bunki=-1;
		fgetpos(frp,&pos);
		sprintf(Tmp,"%d",pos);
		S=atoi(Tmp);
		
		while(strcmp(id,"%")!=0){
			strcpy(Message,"");
			fgets(Message,256,frp);
			strncpy(Message,Decode(Message),strlen(Message));
			if(strcmp(id,"M")==0){
				isBGMon=true;
				int n=strlen(Message);
				Message[n-1]='\0';
				Message[n-2]='\0';
				if(strcmp(Message,"stop")==0){
					gsm.Stop(TRUE,1);
					isBGMon=false;
				}else{
					strcpy(path,"data/sound/");
					strcat(path,Message);
					gsm.loadSeg(0,path,true);
					gsm.Play(TRUE,0);
				}
			}
			//WAV
			if(strcmp(id,"W")==0){
				char path[276];
				int n=strlen(Message);
				Message[n-1]='\0';
				Message[n-2]='\0';
				strcpy(path,"data/sound/");
				strcat(path,Message);
				gsm.loadSeg(1,path,false);
				gsm.Play(FALSE,1);
			}

			//台詞
			if(strcmp(id,"T")==0&&CanRead==true){
				strcat(DispMsg,Message);
			}
			//中央のレベル
			if(strcmp(id,"L")==0){
				CZ=atoi(Message);
				CZBuf=CZ;
				cdg.SetCharaCZ(CZ);
			}
			//キャラポジション
			if(strcmp(id,"P")==0){
				resPos=atoi(Message);
			}

			//キャラセット
			if(strcmp(id,"C")==0){
				resID=atoi(Message);

				resIDBuf[resPos]=resID;
				

				if(resID==IDR_INV){
					cdg.DelChar(resPos);
				}else{
					if(resPos==-1){
						//MessageBox(NULL,"シナリオファイルにおけるリソース表示位置指定不足","",MB_OK);
						cdg.Disp(resID,1);
					}else{
						cdg.Disp(resID,resPos,1);
						
						//MessageBox(NULL,"Disp-position","resID",MB_OK);
					}
					//MessageBox(NULL,Message,"resID",MB_OK);
				}
				
			}
			//背景セット
			if(strcmp(id,"H")==0){
				resID=atoi(Message);
				resIDBuf[PosBG]=resID;
				cdg.Disp(resID,PosBG);
			}
			//明・暗転
			if(strcmp(id,"O")==0){
				//描画順を変更
				cdg.OutFlag=1;
				resID=resID=atoi(Message);
				cdg.DispOut(resID);
			}
			//振動
			if(strcmp(id,"S")==0&&CtrlOn==FALSE){
				if(atoi(Message)==1){
					cdg.Shake(9,TRUE);
				}else{
					cdg.Shake(9,FALSE);
				}
			}
			//選択肢
			if(strcmp(id,"Q")==0&&CanRead==true){
				alBN++;
				mode=1;
				strcat(DispMsg,Message);
			}				

			//分岐
			if(strcmp(id,"B")==0){
			//分岐はどうなんのか
			Bunki=atoi(Message);
			if((Bunki/10)!=0){
				BFlag=lastB*10+BFlag;
			}
			lastB=Bunki;
				while(Bunki!=BFlag){
					fgets(id,2,frp);
					strncpy(id,Decode(id),1);
					fgets(Message,256,frp);
					strncpy(Message,Decode(Message),256);
					if(strcmp(id,"B")==0){
						Bunki=atoi(Message);
						if(Bunki==BFlag){
							CanRead=true;
							BFlag=0;
							Find=TRUE;
							break;
						}
						if(Bunki<BFlag&&Bunki!=0){
							//チェック用
							//if(BFlag==2)MessageBox(NULL,"bf is 3","",MB_OK);
							//MessageBox(NULL,"find !2","",MB_OK);
						}
						if(Bunki==0)BFlag=0;
					}
				}
			}
			//終了判定
			if(strcmp(id,"E")==0){
				SaveEnv();
				isEnd=TRUE;
				//MessageBox(NULL,"find E","",MB_OK);
			}
			
			fgets(id,sizeof(char)+1,frp);
			strncpy(id,Decode(id),1);
		}
	}
	fgets(id,sizeof(char)+2,frp);
	strncpy(id,Decode(id),1);

	strcpy(TexWndMsg,DispMsg);
	Queue(TexWndMsg);

	ReDraw();
	return 0;
}
BOOL CTextWnd::Queue(char* Message)
{
	//バックログをキューに蓄積
	strcpy(BLog[q_next],Message);
	if(q_isFull==FALSE){
		q_top=0;
	}else{
		q_top+=1;
		q_top=q_top%BLogSize;
	}
	q_end=q_next;
	q_next+=1;
	//環状キュー
	if(q_next>=BLogSize)q_isFull=TRUE;
	q_next=q_next%BLogSize;

	return 0;
}

BOOL CTextWnd::DispLog(void)
{
	//バックログ戻る
	if(inBLog==FALSE){
		LogNum=q_end;
		if(LogNum==-1)LogNum=BLogSize-1;
		inBLog=TRUE;
	}
	if(LogNum!=q_top){
		LogNum--;
		if(LogNum==-1)LogNum=BLogSize-1;
	}
	ReDraw();	
	return 0;
}

BOOL CTextWnd::DispLogFw(void)
{
	//バックログ進む
	if(inBLog==TRUE){
		if(LogNum!=q_end){
			LogNum++;
		if(LogNum==BLogSize)LogNum=0;
		}
	}
	ReDraw();
	return 0;
}

BOOL CTextWnd::CurSel(int pos)
{
	//選択肢時
	MSG msg;
	if(pos>alBN)pos=alBN;
	SelectedBunki=pos;
	//ReDraw();
	SendMessage(hWndP,WM_PAINT,NULL,NULL);
	Rectangle(hdc,TexLeft+10,TexTop+11+(pos-1)*18,TexLeft+580,TexTop+12+pos*18);
	return 0;
}
bool CTextWnd::SaveEnv(void)
{
	FILE *fp;
	char szBuf[80];
	fp=fopen("data/env.dat","wb+");
	sprintf(szBuf,"%s","is End flag is true dayomon");
	strncpy(szBuf,Decode(szBuf),80);
	fputs(szBuf,fp);
	fclose(fp);
	return true;
}

bool CTextWnd::LoadEnv(void)
{
	FILE *fp;
	char Buff[80];
	fp=fopen("data/env.dat","rb+");
	fgets(Buff,80,fp);
	strncpy(Buff,Decode(Buff),80);
	Buff[28]='\0';
	Buff[27]='\0';
	if(strcmp(Buff,"is End flag is true dayomon")==0){
		return true;
	}
	fclose(fp);
	return false;
}
