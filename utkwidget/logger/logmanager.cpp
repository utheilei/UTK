/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "logmanager.h"
#include "logger.h"
#include "consoleappender.h"
#include "rollingfileappender.h"

LOGGER_BEGIN_NAMESPACE

/**
 * \class DLogManager
 *
 * \brief DLogManager is the deepin user application log manager
 */

LogManager::LogManager()
{
    QString cachePath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0);
    if (!QDir(cachePath).exists())
    {
        QDir(cachePath).mkpath(cachePath);
    }
    m_logPath = joinPath(cachePath, QString("%1.log").arg(qApp->applicationName()));
    m_format = "%{time}{yyyy-MM-dd, HH:mm:ss.zzz} [%{type:-7}] [%{file:-20} %{function:-35} %{line}] %{message}\n";
}

void LogManager::initConsoleAppender()
{
    m_consoleAppender = new ConsoleAppender;
    m_consoleAppender->setFormat(m_format);
    logger->registerAppender(m_consoleAppender);
}

void LogManager::initRollingFileAppender()
{
    m_rollingFileAppender = new RollingFileAppender(m_logPath);
    m_rollingFileAppender->setFormat(m_format);
    m_rollingFileAppender->setLogFilesLimit(5);
    m_rollingFileAppender->setDatePattern(RollingFileAppender::DailyRollover);
    logger->registerAppender(m_rollingFileAppender);
}

void LogManager::setDetailsLevel(Logger::LogLevel level)
{
    if (m_consoleAppender)
    {
        m_consoleAppender->setDetailsLevel(level);
    }
    if (m_rollingFileAppender)
    {
        m_rollingFileAppender->setDetailsLevel(level);
    }
}

//! Registers the appender to write the log records to the Console
/**
 * \sa registerFileAppender
 */
void LogManager::registerConsoleAppender()
{
    LogManager::instance()->initConsoleAppender();
}

//! Registers the appender to write the log records to the file
/**
 * \sa getlogFilePath
 * \sa registerConsoleAppender
 */
void LogManager::registerFileAppender()
{
    LogManager::instance()->initRollingFileAppender();
}

//! Return the path file log storage
/**
 * \sa registerFileAppender
 */
QString LogManager::getlogFilePath()
{
    return QDir::toNativeSeparators(LogManager::instance()->m_logPath);
}

void LogManager::setlogFilePath(const QString &logFilePath)
{
    LogManager::instance()->m_logPath = logFilePath;
}

void LogManager::setLogFormat(const QString &format)
{
    //m_format = "%{time}{yyyy-MM-dd, HH:mm:ss.zzz} [%{type:-7}] [%{file:-20} %{function:-35} %{line}] %{message}\n";
    LogManager::instance()->m_format = format;
}

void LogManager::setLogLevel(Logger::LogLevel level)
{
    LogManager::instance()->setDetailsLevel(level);
}

QString LogManager::joinPath(const QString &path, const QString &fileName)
{
    QString separator(QDir::separator());
    return QString("%1%2%3").arg(path, separator, fileName);
}

LogManager::~LogManager()
{

}

LOGGER_END_NAMESPACE
