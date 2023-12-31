/*
  Copyright (c) 2010 Boris Moiseev (cyberbobs at gmail dot com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1
  as published by the Free Software Foundation and appearing in the file
  LICENSE.LGPL included in the packaging of this file.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
*/
#ifndef FILEAPPENDER_H
#define FILEAPPENDER_H

#include "abstractstringappender.h"

#include <QFile>
#include <QTextStream>

LOGGER_BEGIN_NAMESPACE

class FileAppender : public AbstractStringAppender
{
public:
    FileAppender(const QString &fileName = QString());
    ~FileAppender();

    QString fileName() const;
    void setFileName(const QString &);

    qint64 size() const;

protected:
    virtual void append(const QDateTime &timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, const QString &category, const QString &message);
    bool openFile();
    void closeFile();

private:
    QFile m_logFile;
    QTextStream m_logStream;
    mutable QMutex m_logFileMutex;
};

LOGGER_END_NAMESPACE

#endif // FILEAPPENDER_H
