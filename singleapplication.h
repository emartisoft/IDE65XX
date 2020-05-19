#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QApplication>
#include <QSharedMemory>

class SingleApplication : public QApplication
{
    Q_OBJECT
public:
    SingleApplication(int &argc, char **argv);
    ~SingleApplication();
    bool lock();

private:
    QSharedMemory *_singular;
};

#endif // SINGLEAPPLICATION_H
