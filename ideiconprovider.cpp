#include "ideiconprovider.h"

IdeIconProvider::IdeIconProvider() :
    m_folder(QIcon(":/res/exticon/folder.png")),
    m_hdd(QIcon(":/res/exticon/hdd.png")),
    m_asm(QIcon(":/res/exticon/asm.png")),
    m_prg(QIcon(":/res/exticon/prg.png")),
    m_lib(QIcon(":/res/exticon/lib.png")),
    m_txt(QIcon(":/res/exticon/txt.png")),
    m_bin(QIcon(":/res/exticon/bin.png")),
    m_sid(QIcon(":/res/exticon/sid.png")),
    m_dbg(QIcon(":/res/exticon/dbg.png")),
    m_sym(QIcon(":/res/exticon/sym.png")),
    m_d64(QIcon(":/res/exticon/d64.png")),
    m_d71(QIcon(":/res/exticon/d71.png")),
    m_d81(QIcon(":/res/exticon/d81.png")),
    m_vs(QIcon(":/res/exticon/vs.png")),
    m_undefined(QIcon(":/res/exticon/undefined.png"))
{

}

IdeIconProvider::~IdeIconProvider()
{

}

QIcon IdeIconProvider::icon(const QFileInfo &info) const
{
    if(info.isRoot())
    {
        return m_hdd;   // hdd
    }
    else if(info.isDir())
    {
        return m_folder;    // folder
    }
    else if(info.isFile())
    {
        if((info.suffix()=="asm")||(info.suffix()=="s")||(info.suffix()=="inc")) return m_asm;  // asm, s, inc
        if(info.suffix()=="prg") return m_prg;  // prg
        if((info.suffix()=="lib")||(info.suffix()=="a")) return m_lib;  // lib, a
        if(info.suffix()=="txt") return m_txt;  // txt
        if(info.suffix()=="bin") return m_bin;  // bin
        if(info.suffix()=="sid") return m_sid;  // sid
        if(info.suffix()=="dbg") return m_dbg;  // dbg
        if(info.suffix()=="sym") return m_sym;  // sym
        if(info.suffix()=="d64") return m_d64;  // d64
        if(info.suffix()=="d71") return m_d71;  // d71
        if(info.suffix()=="d81") return m_d81;  // d81
        if(info.suffix()=="vs") return m_vs;  // vs
    }

    return m_undefined; // undefined or others
}
