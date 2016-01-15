#ifndef ALSTREAMERINTERFACE_H
#define ALSTREAMERINTERFACE_H
#include <QString>

//! [0]
class AlStreamerInterface
{
public:
    virtual ~AlStreamerInterface() {}
    virtual QString echo(const QString &message) = 0;
};


QT_BEGIN_NAMESPACE

#define AlStreamerInterface_iid "org.altexo.AlStreamerInterface"

Q_DECLARE_INTERFACE(AlStreamerInterface, AlStreamerInterface_iid)
QT_END_NAMESPACE

#endif // ALSTREAMERINTERFACE_H
