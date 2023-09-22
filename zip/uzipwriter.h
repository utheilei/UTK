#ifndef UZIPWRITER_H
#define UZIPWRITER_H

#include <QScopedPointer>
#include <QFile>

class UZipWriterPrivate;
class UZipWriter
{
public:
    explicit UZipWriter(const QString &fileName);
    ~UZipWriter();

    enum WriterStatus
    {
        NoError,
        FileWriteError,
        FileOpenError,
        FilePermissionsError,
        FileError
    };

    enum WriterCompressionPolicy
    {
        AlwaysCompress,
        NeverCompress,
        AutoCompress
    };

    void setFileName(const QString &fileName);

    bool exists() const;

    WriterStatus status() const;

    void setCompressionPolicy(WriterCompressionPolicy policy);
    WriterCompressionPolicy compressionPolicy() const;

    void setCreationPermissions(QFile::Permissions permissions);
    QFile::Permissions creationPermissions() const;

    void addFile(const QString &fileName, const QByteArray &data);

    void addFile(const QString &fileName, QIODevice* device);

    void addDirectory(const QString &dirName);

    void addSymLink(const QString &fileName, const QString &destination);

    void close();

private:
    QScopedPointer<UZipWriterPrivate> d_ptr;
    Q_DECLARE_PRIVATE(UZipWriter)
};

#endif // UZIPWRITER_H
