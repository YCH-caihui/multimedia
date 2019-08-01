#include "cameraclient.h"

cameraclient::cameraclient()
{

}


void cameraclient::setUpdateGUICallback(UpdateVideo2GUI_Callback callback, unsigned long userData)
{
    m_updateVideoCallback = callback;
    u_userData = userData;
}
