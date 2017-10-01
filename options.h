#ifndef OPTIONS_H
#define OPTIONS_H
#include <QStringList>

class Options
{
public:
    Options();
    void Initialize();
    void Save();
    void deleteSegmentDirectory( QString sel );
    void deleteFileTemplate( QString sel );

    QStringList segmentdirectorylist;
    QStringList filetemplatelist;
    QStringList startdatetemplatelist;
};

#endif // OPTIONS_H
