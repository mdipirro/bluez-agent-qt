#ifndef PAIRINGAGENT_H
#define PAIRINGAGENT_H

#include <functional>
#include <QObject>
#include <QDBusContext>

class QDBusObjectPath;

template <class... Args>
class PairingAgentWithValidation : public QObject {
public:
    PairingAgentWithValidation(const std::function<bool(Args...)>& validationFunction, QObject *parent = nullptr): validationFunction(validationFunction), QObject(parent) {}
    virtual ~PairingAgentWithValidation() = default;

    virtual void Release() const = 0;
    virtual QString RequestPinCode(const QDBusObjectPath& device) const = 0;
    virtual void DisplayPinCode(const QDBusObjectPath& device, const QString& pincode) const = 0;
    virtual uint RequestPasskey(const QDBusObjectPath& device) const = 0;
    virtual void DisplayPasskey(const QDBusObjectPath& device, uint passkey, ushort entered) const = 0;
    virtual void RequestConfirmation(const QDBusObjectPath& device, uint passkey) const = 0;
    virtual void RequestAuthorization(const QDBusObjectPath& device) const = 0;
    virtual void AuthorizeService(const QDBusObjectPath& device, const QString& uuid) const = 0;
    virtual void Cancel() const = 0;

protected:
    static const inline QString REJECTED{"org.bluez.Error.Rejected"};
    static const inline QString CANCELED{"org.bluez.Error.Canceled"};

    bool validatePairing(const Args&... args) const {
        return validationFunction(args...);
    }

private:
    std::function<bool(Args...  )> validationFunction;
};

#endif // PAIRINGAGENT_H
