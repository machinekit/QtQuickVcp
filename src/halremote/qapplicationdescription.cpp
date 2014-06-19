#include "qapplicationdescription.h"

/*!
    \qmltype ApplicationDescription
    \instantiates QApplicationDescription
    \inqmlmodule Machinekit.HalRemote
    \brief Application description
    \ingroup halremote

    This component loads the description of an application
    and determines the main QML file.

    \qml
    ApplicationDescription {
        sourceDir: "qrc:/AppDiscover.ServiceDisplay/"
    }
    \endqml
*/

/*! \qmlproperty string ApplicationDescription::sourceDir

    This property holds the source directory of the application.
*/

/*! \qmlproperty string ApplicationDescription::mainFile

    This property holds the main file of the application.
*/

/*! \qmlproperty string ApplicationDescription::name

    This property holds the name of the application.
*/

/*! \qmlproperty string ApplicationDescription::description

    This property holds the description of the application.
*/

/*! \qmlproperty string ApplicationDescription::valid

    This property holds wheter the application configuration is valid or not.
*/

QApplicationDescription::QApplicationDescription(QObject *parent) :
    QObject(parent),
    m_componentCompleted(false),
    m_sourceDir(""),
    m_mainFile(""),
    m_name(""),
    m_description(""),
    m_valid(false)
{
}

bool QApplicationDescription::refresh()
{
    const QString iniName = "description.ini";
    QString sourcePath;
    QStringList fileList;
    QString baseFilePath;
    QString name;
    QString description;
    QString mainFile;

    sourcePath = m_sourceDir.toString(QUrl::RemoveScheme);  // for local operation we must convert the url

    if (m_sourceDir.scheme() == "qrc")  // if qrc we must prepend the :
    {
        sourcePath.prepend(":");
    }

    QDir sourceDir(sourcePath);

    if (!sourceDir.exists())    // the source directory is wrong
    {
        return false;
    }

    baseFilePath = sourceDir.path() + "/";

    fileList = sourceDir.entryList(QDir::Files);

    if ((fileList.count() < 2) || !fileList.contains(iniName, Qt::CaseSensitive)) // minimum of 2 files and ini file
    {
        return false;
    }
    else
    {
        fileList.removeAll(iniName);   // remove the ini file from the list
    }

    for (int i = 0; i < fileList.size(); ++i)
    {
        fileList[i].prepend(baseFilePath);
    }

    QSettings settings(baseFilePath + iniName, QSettings::IniFormat);

    name = settings.value("Default/name", QString("")).toString();
    description = settings.value("Default/description", QString("")).toString();
    mainFile = settings.value("Default/mainFile", QString("")).toString();

    if (name.isEmpty() || description.isEmpty())
    {
        return false;
    }

    if (mainFile.isEmpty())
    {
        mainFile = getMainFile(fileList, baseFilePath, name);   // if no main file is specified we try to figure it out
    }

    if (mainFile.indexOf(':') != 0) // the local string must be converted back to an url
    {
        mainFile.prepend(':');
    }

    m_name = name;
    m_description = description;
    m_mainFile = QUrl(m_sourceDir.scheme() + mainFile);
    emit nameChanged(name);
    emit descriptionChanged(description);
    emit mainFileChanged(mainFile);

    return true;
}

QString QApplicationDescription::getMainFile(QStringList fileList, QString baseFilePath, QString applicationName)
{
    QString mainFileName;
    QString preferredName;

    // Number 1 priority main QML file is name main.qml
    preferredName = baseFilePath + "main.qml";
    foreach (QString fileName, fileList)
    {
        if (fileName == preferredName)
        {
            mainFileName = preferredName;
            break;
        }
    }

    // Number 2 priority main QML file is named like the config name
    if (mainFileName.isEmpty())
    {
        preferredName = baseFilePath + applicationName + ".qml";
        foreach (QString fileName, fileList)
        {
            if (fileName == preferredName)
            {
                mainFileName = preferredName;
                break;
            }
        }
    }

    // Number 3 priority main QML file is the first one of the file list
    if (mainFileName.isEmpty() && (fileList.size() > 0))
    {
        mainFileName = fileList.at(0);
    }

    return mainFileName;
}
