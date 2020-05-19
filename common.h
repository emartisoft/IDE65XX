#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

namespace Common
{
    QString appLocalDir();
    QString appConfigDir();
    static const int ALREADYRUNNING = -0x40;
};

#endif // COMMON_H
