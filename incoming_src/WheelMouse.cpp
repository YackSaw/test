#include "windows.h"
#include "windowsx.h"
#include "wheelmouse.h"

//�}�E�X�z�C�[���ɑΉ�
UINT CWheelMouse::m_wm_message=(((GetVersion()&0x80000000)&&
	LOBYTE(LOWORD(GetVersion())==4))||(!(GetVersion()&0x80000000)&&LOBYTE(LOWORD(GetVersion())==3)))
	?RegisterWindowMessage(MSH_MOUSEWHEEL):0;
