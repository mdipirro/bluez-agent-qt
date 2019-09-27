#include "btmacaddressagent.h"

#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QDBusReply>
#include <QDebug>
#include <QVariant>

BTMACAddressAgent::BTMACAddressAgent(QObject *parent): PairingAgentWithValidation<QDBusObjectPath, QList<QString>>(
    [](const QDBusObjectPath& device, const QList<QString>& authDevices) {
        auto getDeviceMACAddress = [](const QString& devicePath) {
            QDBusInterface bluezInterface{"org.bluez", devicePath, "org.freedesktop.DBus.Properties", QDBusConnection::systemBus()};
            if (bluezInterface.isValid()) {
                QDBusReply<QVariant> reply{bluezInterface.call("Get", "org.bluez.Device1", "Address")};
                if (reply.isValid()) {
                    return reply.value().toString();
                } else {
                    qCritical() << reply.error().message();
                }
            } else {
                qCritical() << bluezInterface.lastError().message();
            }
            return QString();
        };

        return authDevices.contains(getDeviceMACAddress(device.path()));
    },
    parent
) {}

BTMACAddressAgent::BTMACAddressAgent(const std::initializer_list<QString>& authDevices): BTMACAddressAgent() {
    this->authDevices = authDevices;
}

void BTMACAddressAgent::Release() const {
    sendErrorReply(REJECTED, "Pairing rejected");
}

QString BTMACAddressAgent::RequestPinCode(const QDBusObjectPath&) const {return "";}

void BTMACAddressAgent::DisplayPinCode(const QDBusObjectPath&, const QString&) const {}

uint BTMACAddressAgent::RequestPasskey(const QDBusObjectPath&) const {return 0;}

void BTMACAddressAgent::DisplayPasskey(const QDBusObjectPath&, uint, ushort) const {}

void BTMACAddressAgent::RequestConfirmation(const QDBusObjectPath& device, uint) const {
    qInfo() << device.path() << "is requesting authorization to pair";
    if(!validatePairing(device, authDevices)) Release();
}

void BTMACAddressAgent::RequestAuthorization(const QDBusObjectPath&) const {}

void BTMACAddressAgent::AuthorizeService(const QDBusObjectPath&, const QString&) const {}

void BTMACAddressAgent::Cancel() const {
    sendErrorReply(CANCELED, "Pairing canceled");
}
