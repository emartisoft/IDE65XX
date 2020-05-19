#include "common.h"

QString Common::appLocalDir()
{
    return QCoreApplication::applicationDirPath();
}

QString Common::appConfigDir()
{
    return QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QDir::separator() + qApp->applicationName());
}
