#include ".\GSM.h"

//const
CGSM::CGSM(void)
{
	m_pLoader = NULL;
	m_pPerformance = NULL;
//	m_pSegment = NULL;
	Init();
}

//dest
CGSM::~CGSM(void)
{
	Stop(TRUE);
	DeleteObj();
}

//�폜�֐�
void CGSM::DeleteObj(void)
{
	for (int i=0 ; i<segNum ; i++) {
		RELEASE(m_pSegment[i]);
	}

	RELEASE(m_pLoader);
	RELEASE(m_pPerformance);

	CoUninitialize();
}

HRESULT CGSM::Init(void)
{
	HRESULT hr = S_OK;
	//COM�̏�����
	CoInitialize(NULL);
	//���[�_�[�̍쐬
	hr = CoCreateInstance(CLSID_DirectMusicLoader, NULL, 
			CLSCTX_INPROC, IID_IDirectMusicLoader8, (void**)&m_pLoader);
	//�p�t�H�[�}���X�̍쐬
	hr = CoCreateInstance(CLSID_DirectMusicPerformance, NULL, 
			CLSCTX_INPROC, IID_IDirectMusicPerformance8, (void**)&m_pPerformance);
	//�p�t�H�[�}���X�ƃV���Z�T�C�U�[�̏�����
	hr = m_pPerformance->InitAudio(NULL, NULL, NULL,  
			DMUS_APATH_SHARED_STEREOPLUSREVERB, 64, DMUS_AUDIOF_ALL, NULL);
	//���[�_�[�̏������i�����p�X���J�����g�f�B���N�g���ɐݒ�j
	CHAR strPath[GSM_P];
	DWORD cntPath = GetCurrentDirectory(GSM_P, strPath);
	if (cntPath==0 || GSM_P<cntPath) {
		MB("�J�����g�E�f�B���N�g���̎擾�Ɏ��s");
		return false;
	}
	// �}���`�E�o�C�g������UNICODE�ɕϊ�
	WCHAR wstrSearchPath[GSM_P];
	MultiByteToWideChar(CP_ACP, 0, strPath, -1, wstrSearchPath, GSM_P);

	// ���[�_�[�Ɍ����p�X��ݒ�
	hr = m_pLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wstrSearchPath, FALSE);
	if (FAILED(hr)) {
		MB("�����p�X�̐ݒ�Ɏ��s");
		return false;
	}

	if(FAILED(hr)){
		MB("Music class initialize error");
	}
	return hr;
}

HRESULT CGSM::fileLoad(WCHAR* Filename)
{
	//���̃Q�[�����ł͖��g�p
	//HRESULT hr = S_OK;

	////�t�@�C���̃��[�h
	//if(FAILED(m_pLoader->LoadObjectFromFile(
	//	CLSID_DirectMusicSegment,
	//	IID_IDirectMusicSegment8,
	//	Filename,
	//	(LPVOID*) &m_pSegment
	//	)))
	//{
	//	m_pPerformance->CloseDown();
	//	DeleteObj();

	//	return 0;
	//}

	////�o���h���p�t�H�[�}���X�Ƀ_�E�����[�h����
	//m_pSegment->Download(m_pPerformance);
	//if(FAILED(hr)){
	//	MB("File load error");
	//}

	//return hr;
	return 0;
}

bool CGSM::loadSeg(int seg, char* fileName, bool flgMIDI)
{
	HRESULT	hr;
	WCHAR wstrFileName[GSM_P];

	//�Z�O�����g�̏�����
	if(m_pSegment[seg] != NULL){
		Stop(FALSE,seg);
		m_pSegment[seg] = NULL;
	}
	
	MultiByteToWideChar(CP_ACP,0,fileName,-1,wstrFileName,GSM_P);

	//�Z�O�����g�Ƀt�@�C�������w�肵�ă��[�h����
	hr = m_pLoader->LoadObjectFromFile(
		CLSID_DirectMusicSegment,
		IID_IDirectMusicSegment8,
		wstrFileName,
		(LPVOID*)&m_pSegment[seg]);
	if (FAILED(hr)) {
		switch(hr){
			case DMUS_E_LOADER_FAILEDCREATE:
				MB("DMUS_E_LOADER_FAILEDCREATE");
				break;
			case DMUS_E_LOADER_FAILEDOPEN:
				MB("DMUS_E_LOADER_FAILEDOPEN");
				break;
			case DMUS_E_LOADER_FORMATNOTSUPPORTED:
				MB("DMUS_E_LOADER_FORMATNOTSUPPORTED");
				break;
			case DMUS_E_LOADER_NOCLASSID:
				MB("DMUS_E_LOADER_NOCLASSID");
				break;
			case E_FAIL:
				MB("E_FAIL");
				break;
			case E_INVALIDARG:
				MB("E_INVALIDARG");
				break;
			case E_OUTOFMEMORY:
				MB("E_OUTOFMEMORY");
				break;
			case E_POINTER:
				MB("E_POINTER");
				break;
			case REGDB_E_CLASSNOTREG:
				MB("REGDB_E_CLASSNOTREG");
				break;
			default:
				MB("FLB error case 0");
				break;
		}
		MB(fileName);
		return false;
	}
	//MB(fileName);

	//�p�����[�^��ݒ肷��
	if (flgMIDI) {
		hr = m_pSegment[seg]->SetParam(GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL);
		if (FAILED(hr)) {
			MB("segment parameter set error");
			return false;
		}

		//�Z�O�����g�̒������擾
		hr = m_pSegment[seg]->GetLength(&m_pSegmentLength[seg]);
		if (FAILED(hr))	MB("get segment  length error");

		//���[�v�񐔐ݒ�
		hr = m_pSegment[seg]->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
		if (FAILED(hr))	MB("segment loop error");
	}

	//�o���h�̃_�E�����[�h
	hr = m_pSegment[seg]->Download(m_pPerformance);
	if (FAILED(hr)) {
		MB("segment band download error");
		return false;
	}

	return true;
}


HRESULT CGSM::Play(BOOL bRepeats)
{
	//���̃Q�[�����ł͖��g�p
	//HRESULT hr = S_OK;
	//if(bRepeats)
	//	m_pSegment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
	//m_pPerformance->PlaySegment(m_pSegment, 0, 0, NULL);
	//if(FAILED(hr)){
	//	MB("sound play error");
	//}
	return 0;
}
HRESULT CGSM::Play(BOOL bRepeats,int seg)
{
	HRESULT hr = S_OK;
	switch(seg){
		case 0:
            m_pPerformance->PlaySegmentEx(m_pSegment[0], NULL, NULL, 0, 0, NULL, NULL, NULL);
			break;
		default:
			m_pPerformance->PlaySegmentEx(m_pSegment[seg], NULL, NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL);
			break;
	}
	if(FAILED(hr)){
		MB("sound play error");
	}
	return 0;
}


HRESULT CGSM::Stop(BOOL All,int seg)
{
	HRESULT hr = S_OK;
	if(All){
		m_pPerformance->Stop(NULL, NULL, 0, 0);
	}else{
		m_pPerformance->StopEx(m_pSegment[seg], 0, 0);
	}
	if(FAILED(hr)){
		MB("stop error");
	}

	return hr;
}

