#ifndef _INC_WHEELMOUSE_H_
#define _INC_WHEELMOUSE_H_

#ifndef WM_MOUSEWHEEL
#include "zmouse.h"
#endif

class CWheelMouse{
private:
    static UINT m_wm_message;
public:
    UINT TranslateMessage(UINT msg){
        if(m_wm_message==msg)msg=WM_MOUSEWHEEL;
        return msg;}
    BOOL IsForward(WPARAM wparam){return ((short)HIWORD(wparam)<0);}
};

#endif
