#ifndef IDEICONPROVIDER_H
#define IDEICONPROVIDER_H

#include <QFileIconProvider>

class IdeIconProvider : public QFileIconProvider
{
public:
    IdeIconProvider();
    ~IdeIconProvider();

    QIcon icon(const QFileInfo &info) const override;

private:
    QIcon m_folder;
    QIcon m_hdd;
    QIcon m_asm;
    QIcon m_prg;
    QIcon m_lib;
    QIcon m_txt;
    QIcon m_bin;
    QIcon m_sid;
    QIcon m_dbg;
    QIcon m_sym;
    QIcon m_d64;
    QIcon m_d71;
    QIcon m_d81;
    QIcon m_vs;
    QIcon m_crt;
    QIcon m_undefined;
};

#endif // IDEICONPROVIDER_H
