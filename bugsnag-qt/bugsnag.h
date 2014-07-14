#ifndef BUGSNAG_H
#define BUGSNAG_H

#include "bugsnag-qt_global.h"

#include <QJsonObject>
#include <QObject>
#include <QEvent>
#include <QList>
#include <QJsonArray>
#include <QHash>
#include <QStringList>

class Notifier
{
public:
    Notifier()
        : name("Bugsnag QT")
    , version("1.0.0")
    , url("https://github.com/tanel/bugsnag-qt") {}

    void write(QJsonObject &json) const {
        json["name"] = name;
        json["version"] = version;
        json["url"] = url;
    }

    QString name;
    QString version;
    QString url;
};

class StackTrace {
public:
    StackTrace()
        : file("")
    , lineNumber(0)
    , columnNumber(0)
    , method("")
    , inProject(false) {}

    void write(QJsonObject &json) const {
        json["file"] = file;
        json["lineNumber"] = lineNumber;
        json["columnNumber"] = columnNumber;
        json["method"] = method;
        json["inProject"] = inProject;
    }

    QString file;
    int lineNumber;
    int columnNumber;
    QString method;
    bool inProject;
};

class Exception
{
public:
    Exception()
        : errorClass("")
    , message("") {}

    void write(QJsonObject &json) const {
        json["errorClass"] = errorClass;
        json["message"] = message;

        QJsonArray stacktraceJSON;
        foreach(StackTrace trace, stacktrace) {
            QJsonObject lineJSON;
            trace.write(lineJSON);
            stacktraceJSON.append(lineJSON);
        }
        json["stacktrace"] = stacktraceJSON;
    }

    QString errorClass;
    QString message;
    QList<StackTrace> stacktrace;
};

class User
{
public:
    User()
        : id("")
    , name("")
    , email("") {}

    void write(QJsonObject &json) const {
        json["id"] = id;
        json["name"] = name;
        json["email"] = email;
    }

    QString id;
    QString name;
    QString email;
};

class App
{
public:
    App()
        : version("")
    , releaseStage("production") {}

    void write(QJsonObject &json) const {
        json["version"] = version;
        json["releaseStage"] = releaseStage;
    }

    QString version;
    QString releaseStage;
};

class Device
{
public:
    Device()
        : osVersion("")
    , hostname("") {}

    void write(QJsonObject &json) const {
        json["osVersion"] = osVersion;
        json["hostname"] = hostname;
    }

    QString osVersion;
    QString hostname;
};

class Event
{
public:
    Event()
        : context("")
    , groupingHash("")
    , severity("error") {}

    void write(QJsonObject &json) const {
        json["payloadVersion"] = QString("2");

        QJsonArray exceptionsJSON;
        foreach(Exception e, exceptions) {
            QJsonObject exceptionJSON;
            e.write(exceptionJSON);
            exceptionsJSON.append(exceptionJSON);
        }
        json["exceptions"] = exceptionsJSON;

        json["context"] = context;
        json["groupingHash"] = groupingHash;
        json["severity"] = severity;

        QJsonObject userJSON;
        user.write(userJSON);
        json["user"] = userJSON;

        QJsonObject appJSON;
        app.write(appJSON);
        json["app"] = appJSON;

        QJsonObject deviceJSON;
        app.write(deviceJSON);
        json["device"] = deviceJSON;

        QJsonObject metaDataJSON;
        QHash<QString, QHash<QString, QString> >::const_iterator i = metaData.begin();
        while (i != metaData.constEnd()) {
            QJsonObject valuesJSON;
            QHash<QString, QString>::const_iterator j = i.value().constBegin();
            while (j != i.value().constEnd()) {
                valuesJSON[j.key()] = j.value();
                ++j;
            }
            metaDataJSON[i.key()] = valuesJSON;
            ++i;
        }
    }

    QList<Exception> exceptions;
    QString context;
    QString groupingHash;
    QString severity;
    User user;
    App app;
    Device device;
    QHash<QString, QHash<QString, QString> > metaData;
};

class Payload
{
public:
    Payload() {}

    void write(QJsonObject &json) const {
        json["apiKey"] = apiKey;

        QJsonObject notifierJSON;
        notifier.write(notifierJSON);
        json["notifier"] = notifierJSON;
    }

    QString apiKey;
    Notifier notifier;
    QList<Event> events;
};

class BUGSNAGQTSHARED_EXPORT Bugsnag
{

public:
    Bugsnag();

    static bool notify(QObject *receiver,
                       QEvent *evt,
                       std::exception &e,
                       User *user = 0,
                       QString context = QString(""),
                       App *app = 0,
                       Device *device = 0,
                       QHash<QString, QHash<QString, QString> > *metadata = 0) {
        Payload payload;
        payload.apiKey = Bugsnag::apiKey;
        payload.notifier = Notifier();
        Exception exception;
        exception.message = e.what();
        exception.errorClass = "std::exception";
        // FIXME: exception.stacktrace
        Event event;
        event.context = context;
        event.groupingHash = receiver->objectName();
        event.exceptions << exception;
        if (user) {
            event.user = *user;
        }
        if (app) {
            event.app = *app;
        }
        if (device) {
            event.device = *device;
        }
        if (metadata) {
            event.metaData = *metadata;
        }
        payload.events << event;
        return true;
    }

    static QString apiKey;
    static QString releaseStage;
    static QStringList notifyReleaseStages;
    static bool autoNotify;
    static bool useSSL;
};

QString Bugsnag::releaseStage = QString("production");
QStringList Bugsnag::notifyReleaseStages = QStringList() << "production" << "development";
bool Bugsnag::autoNotify = true;
bool Bugsnag::useSSL = true;

#endif // BUGSNAG_H
