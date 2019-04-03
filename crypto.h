#ifndef CRYPTO_H
#define CRYPTO_H

#include <QString>
#include <QByteArray>

namespace Crypto
{
    const QString GenericAESKey = "a3K8Bx%2r8Y7#xDh";

    QByteArray decryptPack(const QByteArray& packBase64, const QString& key);
    QByteArray encryptPack(const QByteArray &pack, const QString& key);
    void addPKCS7Padding(QByteArray& packBase64);

    QByteArray Encrypt_CBC(QByteArray &packet,unsigned char &key, unsigned char &iv);
    QByteArray Decrypt_CBC(QByteArray &packet,unsigned char &key, unsigned char &iv);
}

#endif // CRYPTO_H
