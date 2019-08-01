#ifndef CAMERACLIENT_H
#define CAMERACLIENT_H

#include "h264decoder.h"
typedef void (*UpdateVideo2GUI_Callback)(RGBData_Define * rgbData, unsigned long userData);
class cameraclient
{
public:
    cameraclient();
    ~cameraclient();

    void setUpdateGUICallback(UpdateVideo2GUI_Callback callback, unsigned long userData);

private:
    unsigned long u_userData;
    UpdateVideo2GUI_Callback m_updateVideoCallback;
};

#endif // CAMERACLIENT_H
