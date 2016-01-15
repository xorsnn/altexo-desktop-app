#ifndef WIDGETSTREAMER_H
#define WIDGETSTREAMER_H

#include <QObject>
#include "alconductor/alconductor.h"

class WidgetStreamer : public QObject
{
    Q_OBJECT
public:
    WidgetStreamer(QWidget *pRenderWidget, AlConductor *pStreamWidget);
    ~WidgetStreamer();
private Q_SLOTS:
    void onTimeout();
private:
    QWidget *m_pRenderWidget;
    AlConductor *m_pStreamWidget;

};

#endif // WIDGETSTREAMER_H
