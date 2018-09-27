#ifndef ZIP_H
#define ZIP_H

class QIODevice;
class QString;

class ArchiveModel;

class Zip
{

public:
  static bool write(const ArchiveModel* data, const QString& outFile);
  //static bool read(ArchiveModel* data, const QString& inFile);

private:
  static bool copyData(QIODevice& inFile, QIODevice& outFile);

};

#endif // ZIP_H
