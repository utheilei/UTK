#include "uzipreader.h"

#include <QtGui/private/qzipreader_p.h>

class UZipReaderPrivate
{
    Q_DECLARE_PUBLIC(UZipReader)
public:
    UZipReaderPrivate(UZipReader* parent) : q_ptr(parent)
    {
    }

    UZipReader* q_ptr = nullptr;
private:
    QScopedPointer<QZipReader> reader;
    QString savePath = "";
};

UZipReader::UZipReader(const QString &fileName) : d_ptr(new UZipReaderPrivate(this))
{
    Q_D(UZipReader);
    d->reader.reset(new QZipReader(fileName));
}

UZipReader::~UZipReader()
{
}

void UZipReader::setFileName(const QString &fileName, const QString &savePath)
{
    Q_D(UZipReader);
    d->reader.reset(new QZipReader(fileName));
    d->savePath = savePath;
}

bool UZipReader::exists() const
{
    Q_D(const UZipReader);

    if (d->reader.isNull())
    {
        return false;
    }

    return d->reader->exists();
}

QVector<UZipReader::ZipFileInfo> UZipReader::fileInfoList() const
{
    Q_D(const UZipReader);

    QVector<UZipReader::ZipFileInfo> vector;
    if (d->reader.isNull())
    {
        return QVector<UZipReader::ZipFileInfo>();
    }

    vector.reserve(count());
    for (const auto &info : d->reader->fileInfoList())
    {
        UZipReader::ZipFileInfo zipInfo;
        zipInfo.filePath = info.filePath;
        zipInfo.isDir = info.isDir;
        zipInfo.isFile = info.isFile;
        zipInfo.isSymLink = info.isSymLink;
        zipInfo.size = info.size;
        zipInfo.lastModified = info.lastModified;
        vector.append(zipInfo);
    }

    return vector;
}

int UZipReader::count() const
{
    Q_D(const UZipReader);

    if (d->reader.isNull())
    {
        return false;
    }

    return d->reader->count();
}

QByteArray UZipReader::fileData(const QString &fileName) const
{
    Q_D(const UZipReader);

    if (d->reader.isNull())
    {
        return QByteArray();
    }

    return d->reader->fileData(fileName);
}

UZipReader::ReaderStatus UZipReader::extractAll() const
{
    Q_D(const UZipReader);

    if (d->reader.isNull())
    {
        return ReaderStatus::FileError;
    }

    if (d->savePath.isEmpty())
    {
        return ReaderStatus::FileSaveEmpty;
    }

    ReaderStatus status = d->reader->extractAll(d->savePath) ? ReaderStatus::NoError :
                       ReaderStatus::FileReadError;
    return status;
}

UZipReader::ReaderStatus UZipReader::extractAll(const QString &savepath)
{
    Q_D(UZipReader);

    if (d->reader.isNull())
    {
        return ReaderStatus::FileError;
    }

    d->savePath = savepath;
    ReaderStatus status = d->reader->extractAll(savepath) ? ReaderStatus::NoError :
                       ReaderStatus::FileReadError;
    return status;
}

UZipReader::ReaderStatus UZipReader::status() const
{
    Q_D(const UZipReader);

    if (d->reader.isNull())
    {
        return ReaderStatus::FileError;
    }

    return ReaderStatus(d->reader->status());
}

void UZipReader::close()
{
    Q_D(UZipReader);

    if (d->reader.isNull())
    {
        return;
    }

    d->reader->close();
}
