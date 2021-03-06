#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <QFile>
#include "logwriter_global.h"

class LOGWRITERSHARED_EXPORT LogWriter
{
public:
    LogWriter();
    ~LogWriter();

    qint64 write(QByteArray buffer);
    void setFileName(QString filename);
private:
    QFile* outfile;
};

#endif // LOGWRITER_H
