#include "crypto.h"

#include <QDebug>
#include <QLoggingCategory>

#include <vector>

#include <aes.h>
#include <filters.h>
#include <modes.h>

using namespace CryptoPP;

Q_DECLARE_LOGGING_CATEGORY(CryptoLog)
Q_LOGGING_CATEGORY(CryptoLog, "Crypto")

namespace Crypto
{


unsigned char packet_bytes[] = {
  0x48, 0xb1, 0x86, 0xbe, 0x84, 0x78, 0x80, 0x82,
  0xc9, 0x68, 0xbd, 0x70, 0x80, 0x41, 0x01, 0x5c,
  0xb2, 0xc4, 0xa8, 0xd7, 0xe5, 0x95, 0x09, 0x8b,
  0x30, 0xdd, 0x4a, 0xba, 0x76, 0x33, 0x90, 0x17,
  0xb9, 0xc6, 0x9d, 0x40, 0x19, 0xe1, 0x71, 0x16,
  0xb9, 0xbd, 0x31, 0x1d, 0x8c, 0xd1, 0xff, 0x1e,
  0x10, 0xef, 0x8c, 0xf4, 0x80, 0xdd, 0x7a, 0xb5,
  0xa2, 0xa9, 0x81, 0xb5, 0x6d, 0xe4, 0xbc, 0x52,
  0xff, 0x46, 0x64, 0xcd, 0x1d, 0x1b, 0x7e, 0xb0,
  0xeb, 0xf1, 0xbe, 0x87, 0x85, 0xfa, 0x3b, 0x2c,
  0xef, 0x0c, 0x4c, 0x52, 0x7c, 0x60, 0x49, 0x29,
  0x9b, 0x3a, 0x65, 0x0b, 0x55, 0x81, 0x32, 0x76,
  0xbc, 0x99, 0xa6, 0xbd, 0xd0, 0x36, 0x67, 0x87,
  0x71, 0x59, 0x6b, 0x7e, 0x9c, 0x89, 0x1f, 0x92
};

QByteArray Decrypt_CBC(QByteArray &packet,unsigned char &key, unsigned char &iv)
{
    std::string recovered;

CBC_Mode< AES >::Decryption d;
    d.SetKeyWithIV( (byte *) &key, 16,(byte *) &iv);
    StringSource ss((byte *) packet.data(),packet.length() /*&packet_bytes[0],sizeof (packet_bytes)*/, true,
        new StreamTransformationFilter( d,
            new StringSink( recovered ),BlockPaddingSchemeDef::NO_PADDING
        ) // StreamTransformationFilter
    ); // StringSource
    return QByteArray (recovered.c_str(),recovered.length());;
}
QByteArray Encrypt_CBC(QByteArray &packet,unsigned char &key, unsigned char &iv)
{

    CBC_Mode< AES >::Encryption e;
    std::string cipher;

    e.SetKeyWithIV( (byte *) &key, 16, (byte *) &iv );

    StringSource sse((byte *) packet.data(), packet.length(),true,
            new StreamTransformationFilter( e,
                new StringSink( cipher ),BlockPaddingSchemeDef::NO_PADDING
            ) // StreamTransformationFilter
        ); // StringSource

    QByteArray encrypted(cipher.c_str(),cipher.length());
    return encrypted;
}

QByteArray decryptPack(const QByteArray &packBase64, const QString &key)
{
 //   qCDebug(CryptoLog) << "decrypting pack:" << packBase64;

    auto&& pack = QByteArray::fromBase64(packBase64);

    ECB_Mode<AES>::Decryption decryption{ reinterpret_cast<const unsigned char*>(key.toUtf8().constData()),16};// AES::DEFAULT_KEYLENGTH };
    StreamTransformationFilter decryptor{ decryption, nullptr };
    decryptor.Put(reinterpret_cast<const unsigned char *>(pack.constData()), pack.length());
    decryptor.MessageEnd();

    auto decryptedBytes = decryptor.MaxRetrievable();
//    qCDebug(CryptoLog) << "decrypted data bytes:" << decryptedBytes;

    std::vector<unsigned char> decrypted;
    decrypted.resize(decryptedBytes);
    decryptor.Get(decrypted.data(), decryptedBytes);

    auto&& json = QByteArray(reinterpret_cast<const char*>(decrypted.data()), decryptedBytes);
//    qCDebug(CryptoLog) << "decrypted JSON:" << json;

    return json;
}

QByteArray encryptPack(const QByteArray &pack, const QString &key)
{
//    qCDebug(CryptoLog) << "encrypting pack:" << pack;

    QByteArray paddedPack{ pack };
    addPKCS7Padding(paddedPack);

    ECB_Mode<AES>::Encryption encryption{ reinterpret_cast<const unsigned char*>(key.toUtf8().constData()), AES::DEFAULT_KEYLENGTH };
    StreamTransformationFilter encryptor{ encryption, nullptr };
    encryptor.Put(reinterpret_cast<const unsigned char*>(paddedPack.constData()), paddedPack.length());
    encryptor.MessageEnd();

    auto encryptedBytes = encryptor.MaxRetrievable();
//    qCDebug(CryptoLog) << "encrypted data bytes:" << encryptedBytes;

    QByteArray encrypted(encryptedBytes, 0);
    encryptor.Get(reinterpret_cast<unsigned char*>(encrypted.data()), encryptedBytes);

    auto&& packBase64 = encrypted.toBase64();
//    qCDebug(CryptoLog) << "encrypted data:" << packBase64;

    return packBase64;
}

void addPKCS7Padding(QByteArray& packBase64)
{
    auto length = 16 - (packBase64.length() % 16);
    packBase64.append(QByteArray(length, length));
}

}
