#ifndef BTMACADDRESSAGENT_H
#define BTMACADDRESSAGENT_H

#include "pairingagentwithvalidation.h"

#include <QList>
#include <QDBusContext>

class QDBusObjectPath;
class QString;

class BTMACAddressAgent : public PairingAgentWithValidation<QDBusObjectPath, QList<QString>>, protected QDBusContext {
    Q_OBJECT
public:
    BTMACAddressAgent(QObject *parent = nullptr);
    BTMACAddressAgent(const std::initializer_list<QString>& authDevices);

    Q_INVOKABLE void Release() const;
    Q_INVOKABLE QString RequestPinCode(const QDBusObjectPath& device) const;
    Q_INVOKABLE void DisplayPinCode(const QDBusObjectPath& device, const QString& pincode) const;
    Q_INVOKABLE uint RequestPasskey(const QDBusObjectPath& device) const;
    Q_INVOKABLE void DisplayPasskey(const QDBusObjectPath& device, uint passkey, ushort entered) const;
    Q_INVOKABLE void RequestConfirmation(const QDBusObjectPath& device, uint passkey) const;
    Q_INVOKABLE void RequestAuthorization(const QDBusObjectPath& device) const;
    Q_INVOKABLE void AuthorizeService(const QDBusObjectPath& device, const QString& uuid) const;
    Q_INVOKABLE void Cancel() const;

private:
    QList<QString> authDevices{};
};

#endif // BTMACADDRESSAGENT_H
