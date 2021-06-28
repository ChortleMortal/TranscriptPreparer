#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>

QString processName(QString aline)
{
    static QString lastName;

    int i = aline.indexOf(':');
    QString name = aline.left(i);
    if (name != lastName)
    {
        lastName = name;
        return aline;
    }
    else
    {
        QString str = aline.right(aline.length()-i-2);
        return str;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString filename = argv[1];
    qDebug() << filename;


    QFile afile(filename);
    bool rv = afile.open(QFile::ReadOnly);
    if (!rv)
    {
        qDebug() << "Error opening file";
        exit(0);
    }

    QFileInfo info(afile);
    QString base = info.baseName();
    QString outname = "C:/logs/" + base + ".txt";
    //QString outname = filename.replace(".vtt",".txt")
    //QFile outfile("C:/logs/outifle.txt");
    QFile outfile(outname);
    bool rv2 = outfile.open(QFile::WriteOnly | QFile::Truncate);
    if (!rv2)
    {
        qDebug() << "could not open outfile";
        exit(0);
    }
    QTextStream outstr(&outfile);


    QTextStream str(&afile);
    QString aline;
    do
    {
        aline = str.readLine();

        if (aline.contains("WEBVTT"))
            continue;

        if (aline.isEmpty())
            continue;

        bool ok;
        int num = aline.toInt(&ok);
        if (ok)
            continue;

        if (aline.contains("-->"))
            continue;

        if (aline.contains(":"))
            aline = processName(aline);

        outstr << aline << Qt::endl;
    } while (!aline.isNull());


    afile.close();
    outfile.close();
    //return a.exec();
    exit(0);
}
