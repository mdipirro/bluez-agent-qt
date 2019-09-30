/*
 * Copyright 2019 Matteo Di Pirro
 */

#include <QCoreApplication>

#include "adaptor.h"
#include "btmacaddressagent.h"

int main(int argc, char *argv[]) {
    BTMACAddressAgent agent{"00:00:00:00:00:00", "01:01:01:01:01:01"};
    QString objectPath{"/com/mdipirro/agent"};

    new Agent1Adaptor(&agent);

    if (QDBusConnection::systemBus().registerObject(objectPath, &agent)) {
        QDBusInterface agentManager{"org.bluez", "/org/bluez", "org.bluez.AgentManager1", QDBusConnection::systemBus()};
        if (agentManager.isValid()) {
            QVariant agentPath{QVariant::fromValue(QDBusObjectPath(objectPath))};
            QDBusMessage msg{agentManager.call("RegisterAgent", agentPath, "DisplayOnly")};
            if (msg.type() != QDBusMessage::ErrorMessage) {
                msg = agentManager.call("RequestDefaultAgent", agentPath);
                if (msg.type() != QDBusMessage::ErrorMessage) {
                    qInfo() << "Agent registered as default";
                } else {
                    qWarning() << msg.errorMessage();
                }
            } else {
                qWarning() << msg.errorMessage();
            }
        } else {
            qWarning() << agentManager.lastError().message();
        }
    }

    return QCoreApplication(argc, argv).exec();
}
