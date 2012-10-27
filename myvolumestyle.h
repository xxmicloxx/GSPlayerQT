#ifndef MYVOLUMESTYLE_H
#define MYVOLUMESTYLE_H

#include <QProxyStyle>

class MyVolumeStyle : public QProxyStyle
{
public:
    int styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const;
    
signals:
    
public slots:
    
};

#endif // MYVOLUMESTYLE_H
