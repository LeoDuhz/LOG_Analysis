#include <QDebug>
#include "logwriter.h"


LogWriter::LogWriter() : outfile(NULL) {

}

LogWriter::~LogWriter() {
    delete outfile;
    outfile = NULL;
}

void LogWriter::setFileName(QString &filename) {
    delete outfile;
    outfile = NULL;

    outfile = new QFile(filename);
    outfile->open(QIODevice::WriteOnly);
}

qint64 LogWriter::write(QByteArray buffer) {
    return outfile->write(buffer);
}
