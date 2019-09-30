/*
 * Copyright 2019 Matteo Di Pirro
 */

#include "btmacaddressagent.h"

#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QDBusReply>
#include <QDebug>
#include <QVariant>

BTMACAddressAgent::BTMACAddressAgent(QObject *parent): PairingAgentWithValidation<QDBusObjectPath, QList<QString>>(
    [](const QDBusObjectPath& device, const QList<QString>& authDevices) { // <-- validation function

        auto getDeviceMACAddress = [](const QString& devicePath) { // <-- local function to get the Bluetooth MAC address given an object path
            QDBusInterface bluezInterface{"org.bluez", devicePath, "org.freedesktop.DBus.Properties", QDBusConnection::systemBus()};
            if (bluezInterface.isValid()) { // checks if the D-Bus interface is valid
                QDBusReply<QVariant> reply{bluezInterface.call("Get", "org.bluez.Device1", "Address")};
                if (reply.isValid()) { // checks if the reply is valid
                    return reply.value().toString();
                } else {
                    qCritical() << reply.error().message(); // halts the execution if the reply was unsuccessful
                }
            } else {
                qCritical() << bluezInterface.lastError().message();
            }
            return QString(); // empty string is returned if no such device path is found
        };

        QString deviceMACAddress{getDeviceMACAddress(device.path())};
        return !deviceMACAddress.isEmpty() && authDevices.contains(deviceMACAddress); // also checks for the empty default value
    },
    parent
) {}

BTMACAddressAgent::BTMACAddressAgent(const std::initializer_list<QString>& authDevices): BTMACAddressAgent() { // always invoke the default constructor first
    this->authDevices = authDevices;
}

void BTMACAddressAgent::Release() const {
    sendErrorReply(REJECTED, "Pairing rejected");
}

// empty implementation of unsupported methods
// warning should be given in case such methods are invoked
QString BTMACAddressAgent::RequestPinCode(const QDBusObjectPath&) const {return "";}

void BTMACAddressAgent::DisplayPinCode(const QDBusObjectPath&, const QString&) const {}

uint BTMACAddressAgent::RequestPasskey(const QDBusObjectPath&) const {return 0;}

void BTMACAddressAgent::DisplayPasskey(const QDBusObjectPath&, uint, ushort) const {}

/*
 * Just check the validation function and halt the pairing if the device is not authorized.
 * Note that RequestConfirmation does not need to know the actual validation logic, it just
 * has to check the outcome of the valdiation flow.
 */
void BTMACAddressAgent::RequestConfirmation(const QDBusObjectPath& device, uint) const {
    qInfo() << device.path() << "is requesting authorization to pair";
    if(!validatePairing(device, authDevices)) Release();
}

void BTMACAddressAgent::RequestAuthorization(const QDBusObjectPath&) const {}

void BTMACAddressAgent::AuthorizeService(const QDBusObjectPath&, const QString&) const {}

void BTMACAddressAgent::Cancel() const {
    sendErrorReply(CANCELED, "Pairing canceled");
}
