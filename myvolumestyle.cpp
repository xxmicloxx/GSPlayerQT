#include "myvolumestyle.h"

int MyVolumeStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const {
    if (hint == SH_Slider_AbsoluteSetButtons)
        return Qt::LeftButton; // or "Qt::LeftButton | Qt::RightButton"
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}
