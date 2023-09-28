#include "uzipwriter.h"

#include <QtGui/private/qzipwriter_p.h>

class UZipWriterPrivate
{
    Q_DECLARE_PUBLIC(UZipWriter)
public:
    UZipWriterPrivate(UZipWriter* parent) : q_ptr(parent) {}
    ~UZipWriterPrivate() {}

    UZipWriter* q_ptr = nullptr;
private:
    QScopedPointer<QZipWriter> writer;
};

UZipWriter::UZipWriter(const QString &fileName) : d_ptr(new UZipWriterPrivate(this))
{
    Q_D(UZipWriter);
    d->writer.reset(new QZipWriter(fileName));
}

UZipWriter::~UZipWriter()
{
}

void UZipWriter::setFileName(const QString &fileName)
{
    Q_D(UZipWriter);
    d->writer.reset(new QZipWriter(fileName));
}

bool UZipWriter::exists() const
{
    Q_D(const UZipWriter);
    if (d->writer.isNull())
    {
        return false;
    }

    return d->writer->exists();
}

UZipWriter::WriterStatus UZipWriter::status() const
{
    Q_D(const UZipWriter);
    if (d->writer.isNull())
    {
        return WriterStatus::FileError;
    }

    return WriterStatus(d->writer->status());
}

void UZipWriter::setCompressionPolicy(WriterCompressionPolicy policy)
{
    Q_D(UZipWriter);
    if (d->writer.isNull())
    {
        return;
    }

    d->writer->setCompressionPolicy(QZipWriter::CompressionPolicy(policy));
}

UZipWriter::WriterCompressionPolicy UZipWriter::compressionPolicy() const
{
    Q_D(const UZipWriter);
    if (d->writer.isNull())
    {
        return WriterCompressionPolicy::NeverCompress;
    }

    return WriterCompressionPolicy(d->writer->compressionPolicy());
}

void UZipWriter::setCreationPermissions(QFileDevice::Permissions permissions)
{
    Q_D(UZipWriter);
    if (d->writer.isNull())
    {
        return;
    }

    d->writer->setCreationPermissions(permissions);
}

QFileDevice::Permissions UZipWriter::creationPermissions() const
{
    Q_D(const UZipWriter);
    if (d->writer.isNull())
    {
        return QFileDevice::Permission::ExeOther;
    }

    return d->writer->creationPermissions();
}

void UZipWriter::addFile(const QString &fileName, const QByteArray &data)
{
    Q_D(UZipWriter);
    if (d->writer.isNull())
    {
        return;
    }

    d->writer->addFile(fileName, data);
}

void UZipWriter::addFile(const QString &fileName, QIODevice* device)
{
    Q_D(UZipWriter);
    if (d->writer.isNull())
    {
        return;
    }

    d->writer->addFile(fileName, device);
}

void UZipWriter::addDirectory(const QString &dirName)
{
    Q_D(UZipWriter);
    if (d->writer.isNull())
    {
        return;
    }

    d->writer->addDirectory(dirName);
}

void UZipWriter::addSymLink(const QString &fileName, const QString &destination)
{
    Q_D(UZipWriter);
    if (d->writer.isNull())
    {
        return;
    }

    d->writer->addSymLink(fileName, destination);
}

void UZipWriter::close()
{
    Q_D(UZipWriter);
    if (d->writer.isNull())
    {
        return;
    }

    d->writer->close();
}
