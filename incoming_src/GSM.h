#pragma once
#include <dmusici.h>
#include <windows.h>
#define MB(c) MessageBox(NULL,c,"",0);
#define segNum 2
#define GSM_P 300

//ƒƒ‚ƒŠ‰ð•ú
#define RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
class CGSM
{
	IDirectMusicLoader8*  m_pLoader;
	IDirectMusicPerformance8* m_pPerformance;
	IDirectMusicSegment8*   m_pSegment[segNum];
	MUSIC_TIME m_pSegmentLength[segNum];
public:
	CGSM(void);
	~CGSM(void);
	void DeleteObj(void);
	HRESULT Init(void);
	HRESULT fileLoad(WCHAR* Filename);
	HRESULT Play(BOOL bRepeats);
	HRESULT Play(BOOL bRepeats,int seg);
	HRESULT Stop(BOOL All=FALSE,int seg=1);
	bool loadSeg(int seg, char* fileName, bool flgMIDI);
};
