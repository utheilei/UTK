#ifndef UZIPREADER_H
#define UZIPREADER_H

#include <QScopedPointer>
#include <QFile>
#include <QDateTime>

class UZipReaderPrivate;
class UZipReader
{
public:
    explicit UZipReader(const QString &fileName);
    ~UZipReader();

    enum ReaderStatus
    {
        NoError,
        FileReadError,
        FileOpenError,
        FilePermissionsError,
        FileError,
        FileSaveEmpty
    };

    struct ZipFileInfo
    {
        bool isValid() const noexcept
        {
            return isDir || isFile || isSymLink;
        }

        QString filePath = "";
        uint isDir : 1;
        uint isFile : 1;
        uint isSymLink : 1;
        qint64 size = 0;
        QDateTime lastModified;
    };

    void setFileName(const QString &fileName, const QString &savePath);

    bool exists() const;

    QVector<ZipFileInfo> fileInfoList() const;

    int count() const;

    QByteArray fileData(const QString &fileName) const;

    ReaderStatus extractAll() const;

    ReaderStatus extractAll(const QString &savepath);

    ReaderStatus status() const;

    void close();

private:
    QScopedPointer<UZipReaderPrivate> d_ptr;
    Q_DECLARE_PRIVATE(UZipReader)
};

#endif // UZIPREADER_H
