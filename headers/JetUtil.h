#ifndef __JETUTIL_H__
#define __JETUTIL_H__

class player;

namespace JetUtil
{
    void drawWarningSpeed(player& P1);
    void drawSpeed(player& P1);
    void drawHeight(player& P1);
    void drawWidth(player& P1);
    void drawProgress(float f);
};

#endif