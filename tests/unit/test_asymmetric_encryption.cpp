/*
 * #%L
 * %%
 * Copyright (C) 2018 BMW Car IT GmbH
 * %%
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * #L%
 */
#include <algorithm>
#include <fstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mococrw/private/IOUtils.h"

#include "asymmetric_crypto_ctx.cpp"

using namespace std::string_literals;

using namespace mococrw;

/// \brief Structure to hold a private/public key pair
struct KeyPair
{
    KeyPair(const std::string& publicKey, const std::string& privateKey)
            : pubKey(AsymmetricPublicKey::readPublicKeyFromPEM(publicKey))
            , privKey(AsymmetricPrivateKey::readPrivateKeyFromPEM(privateKey, ""))
    {
    }

    AsymmetricPublicKey pubKey;
    AsymmetricPrivateKey privKey;
};

/// \brief Structure to hold the input data set for the test cases
struct NominalDataSet
{
    NominalDataSet(const std::string& message,
                   const std::vector<uint8_t>& encrypted,
                   const KeyPair& keyPair,
                   std::shared_ptr<RSAEncryptionPadding> padding)
            : message(message)
            , encrypted(encrypted)
            , encryptionCtx(std::make_shared<RSAEncryptionPublicKeyCtx>(keyPair.pubKey, padding))
            , decryptionCtx(std::make_shared<RSAEncryptionPrivateKeyCtx>(keyPair.privKey, padding))
    {
    }

    // inputs
    std::string message;
    std::vector<uint8_t> encrypted;
    std::shared_ptr<EncryptionCtx> encryptionCtx;
    std::shared_ptr<DecryptionCtx> decryptionCtx;
};

class AsymmetricEncryptionTest : public ::testing::Test,
                                 public ::testing::WithParamInterface<NominalDataSet>
{
public:
    static const std::vector<NominalDataSet> nominalDataSet;

    static const NominalDataSet& OAEPPaddingDataSet;
};

/// \brief RSA 1024-bit Pair public/private key
const KeyPair keyPairs1024bit{{"-----BEGIN PUBLIC KEY-----\n"
                               "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC92JisPLVTD00GOavAR0ryUbi4\n"
                               "De/kjnLF/vPTbOEqw1RmsZSgl43uP1Hjj7vRn4/GPXUh8Oh9dqY4NE71nDN+8pgK\n"
                               "wp5xFQF1eikNx1CWkeO3iWgbTVGeIsL8OYIVQ/SZWITnhuMu9bJZaeYgE4p20qJC\n"
                               "kIGpY6r5gFr0PPdYcwIDAQAB\n"
                               "-----END PUBLIC KEY-----\n"},
                              {"-----BEGIN PRIVATE KEY-----\n"
                               "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAL3YmKw8tVMPTQY5\n"
                               "q8BHSvJRuLgN7+SOcsX+89Ns4SrDVGaxlKCXje4/UeOPu9Gfj8Y9dSHw6H12pjg0\n"
                               "TvWcM37ymArCnnEVAXV6KQ3HUJaR47eJaBtNUZ4iwvw5ghVD9JlYhOeG4y71sllp\n"
                               "5iATinbSokKQgaljqvmAWvQ891hzAgMBAAECgYAWYuQd60q2cKd0hHhsQ/vidyCB\n"
                               "E6OmH+YI0veNhtlGYaIiOe58wWJLJcxSOTC8l0Qs7DJTvkSsBsGy/vgQnkq0CV9z\n"
                               "VKL5MEiF5ogNH+vwFcxx71L4/U9Fd6xp2WlMDCmn3+M5Hk9brZUX/w5QeXIN/al0\n"
                               "R27Pn/tiDPxagYRtgQJBAO6jQXwt1IDW54FGgh9Av99fwVEraQ1cw+GCsadJe9rx\n"
                               "mdc9SwgRe9SkKmN+bkDHqWEmYewQki17gotj6/mp0i8CQQDLqJKMMZx4jGdP4E3o\n"
                               "9EcBEs+rFsoXZrcLIHCKKqNMu0/Ag7w+75R9QOGBKSrw42rea+mfkbfZES+oKjI0\n"
                               "f2D9AkEA5gtZEGV5yi3hBB09SPV5QdMkH6RSZAA+EEGWEqbjD8ABxp0enIuGLQmI\n"
                               "l1sE28vdu3ptwUyELfY3dToA/C86HwJBAMYlTefuwPEckcvKTaMQQCEQP1KTiq43\n"
                               "9nFx72zOTTFzsQV6ZmXJTWyL+ptjBW93pFuhHJ1u9jPQn9B6tXwUTrUCQCKUo5VZ\n"
                               "b38OTcPebdJGrGHXOvebKzvBxHTSf0NVKpSg9h6AGQwrmrNN12LgjoL+CGOVmQVH\n"
                               "PV+atCNhiPZi7Ts=\n"
                               "-----END PRIVATE KEY-----\n"}};

/// \brief RSA 2048-bit Pair public/private key
const KeyPair keyPairs2048bit{{"-----BEGIN PUBLIC KEY-----\n"
                               "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAotPqMar+G/+YhO7wTKbe\n"
                               "iA6HuYgNIc+4uYwpBbHV+4vZUQnauVUe7ey5YA9qPtyJMNbEmnhEJi5ZwMowyeOP\n"
                               "s0zaxuPNJLMhd5TS9/nfWQ1nzdEchGRiTGrT7EHDzCMm/Ebk/iDaHZqR0roY1s/V\n"
                               "udwyVMEaGfQsTRAVPMPvJztkMENy9k663ou0Pq/svspEditSdEumPHLQER2FQVX6\n"
                               "4T7YuWS6MkR9KKzZPZn70+2CTcaS5E0E5At4b8AzSbvhbR4FsnSKwsRiHH1BE0Yu\n"
                               "xjrVfYzxh/En6FRxolwr/0g8BCx1mnm3OXC7yT3FTFqVtm9RZw8Fr+UpZtIXsk7T\n"
                               "KQIDAQAB\n"
                               "-----END PUBLIC KEY-----\n"},
                              {"-----BEGIN PRIVATE KEY-----\n"
                               "MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQCi0+oxqv4b/5iE\n"
                               "7vBMpt6IDoe5iA0hz7i5jCkFsdX7i9lRCdq5VR7t7LlgD2o+3Ikw1sSaeEQmLlnA\n"
                               "yjDJ44+zTNrG480ksyF3lNL3+d9ZDWfN0RyEZGJMatPsQcPMIyb8RuT+INodmpHS\n"
                               "uhjWz9W53DJUwRoZ9CxNEBU8w+8nO2QwQ3L2Trrei7Q+r+y+ykR2K1J0S6Y8ctAR\n"
                               "HYVBVfrhPti5ZLoyRH0orNk9mfvT7YJNxpLkTQTkC3hvwDNJu+FtHgWydIrCxGIc\n"
                               "fUETRi7GOtV9jPGH8SfoVHGiXCv/SDwELHWaebc5cLvJPcVMWpW2b1FnDwWv5Slm\n"
                               "0heyTtMpAgMBAAECggEAQF0obC29eO/WKGYTExm7SWkE+a/wEPd9e8sNxDSB84da\n"
                               "hhAxJ6WgLkivFmvBgcvHZ0jeOnlj5vvXer/x/Tw455VvvcL6qpEsn4pywpdBVIBa\n"
                               "Eh+g+Cx+Ntrbnr54WU9j78FOb64oyh75maT6gJ9zSp7iPmJ+oNfGU0gSPQzukkDk\n"
                               "Vf9N118EMh8KUe35pT01zhO4dL9mf/vKrkY/W3r4hEhu66GUH5aI+wbh7zGcCW7L\n"
                               "RWsZcA3xA7zTzI7Tkoe7cTrxOdU3xei2XDUcCFB0bgMcm8Me9GlVAAvI+8BvntCO\n"
                               "KsIi/ennQeMrwIgyoUl7xVGzewnTQ+Vrz94wNfKNAQKBgQDUaFYPJ2TSMg0TnBAE\n"
                               "LUm/poU8e6a80CqheJrnOry6d7spbgTqxWiaAp3stnsmEHRLBmqhrLH3g6B8X2Pl\n"
                               "FM3cMcJir82qUh37vjaWYEPKrJ5pbn2rAMQYyXau7rYICIMW0B9XrlKS9sWS7E3v\n"
                               "4dV/7Sz3CfEpl/w/G0UjzC1SsQKBgQDEPrYIfjW1CiSUVSXU+HFoscA6XxnXdrvu\n"
                               "im3jhJHSCfeMO54bqYwELaERRJSTSalGQfCyFJIWtVMYRdraAJALRSMgrjJpqyBM\n"
                               "0wurapwgPxZv+7GgsecFqO2Z1bo6Bxfd4fgmZo+P0R8d/vNmQAHDo9oDHoMMFSLY\n"
                               "frgOhPb1+QKBgCPWCpb57X+tJBmn3efmgCfOf3Ip5oK6mEav42qZtVe8dZ5Syo+V\n"
                               "iMVbJavXScWtRwGVbo4iZdA9q+jv6xmuPeEloAcoZAmVWyuq9ui+CxH5t6xQEPG1\n"
                               "6l/INbbj3WBTl1JVAMS5S4MyETltluSe9Hdl240+Zyq+UreO8UZN32OxAoGAS7nL\n"
                               "MmHJRa/BR7/HlnO8B2M3ww55gUwe3sBqAPZh8oPIE0PaEeCN1/rngStOjbtJ54OA\n"
                               "ZqdLbBnSE7a46ZB4BPFLycuORnozP5HcmevMjSDq/pFIDLM5a1SxQsBXWRbtncAZ\n"
                               "qm+/DAbtE/0SXS1rwNuUP8nhVaM+S/4x9eBXD7kCgYAQmsS/L5ZZEDTVtqJhiyGj\n"
                               "tnpq6kJKVSsa3WItN47HCE0nuPpjtH3Df/w8CQg2AUA/IjvYLcpTYgJlYt9mm+bF\n"
                               "ytMgYE9OF7bDJzqfAqolINa5KQIXZhmdE5vdDWogxZzy0SsaYCo/+0pR3Q0PCuQa\n"
                               "UwMtJkDrJSjst498ZuyGpg==\n"
                               "-----END PRIVATE KEY-----\n"}};

/// \brief Data set used to test successful (nominal) use-cases, both encryption and decryption
const std::vector<NominalDataSet> AsymmetricEncryptionTest::nominalDataSet{
        // PKCS1 1024-bit RSA key data set
        {"Lorem ipsum dolor sit amet, consectetur",
         {0x59, 0x80, 0x2F, 0x2E, 0xEC, 0x14, 0xA2, 0xA7, 0x7D, 0x0A, 0xB8, 0xF1, 0x1A, 0xF5, 0x5A,
          0x1B, 0x1E, 0x06, 0x75, 0x85, 0x68, 0x2A, 0x6A, 0x19, 0x70, 0x33, 0xD6, 0x3B, 0x76, 0x78,
          0x28, 0x73, 0x41, 0xB6, 0xBC, 0x85, 0xB9, 0x9F, 0x99, 0x7E, 0x83, 0xEB, 0x20, 0x54, 0xBA,
          0x8A, 0x3D, 0xB3, 0x17, 0xA6, 0x62, 0x28, 0xB3, 0x2E, 0x45, 0x01, 0x42, 0x5F, 0x00, 0x53,
          0x86, 0xB4, 0x74, 0xE6, 0x78, 0x62, 0x0F, 0x26, 0x6A, 0xE2, 0x62, 0x1E, 0x5B, 0x45, 0x4A,
          0xE4, 0xD6, 0x7D, 0x1B, 0x4D, 0xC3, 0x6E, 0xE1, 0x36, 0xD9, 0xC7, 0x45, 0x57, 0x45, 0x60,
          0xF8, 0xF3, 0xAC, 0x3D, 0xC1, 0x18, 0x95, 0xF4, 0x3C, 0x70, 0x50, 0x84, 0x53, 0xD2, 0x74,
          0xDA, 0x18, 0x2E, 0x62, 0xE8, 0x3A, 0xC2, 0x53, 0x20, 0xB7, 0x32, 0xE5, 0xF8, 0x77, 0xCE,
          0xF3, 0x2C, 0xB0, 0xAC, 0xAC, 0x67, 0x9B, 0xFE},
         keyPairs1024bit,
         std::make_shared<PKCSPadding>()},
        // PKCS1 2048-bit RSA key data set
        {"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor "
         "incididunt ut labore et dolore magna",
         {0x85, 0x36, 0x57, 0x1E, 0xDA, 0xF6, 0xF8, 0x19, 0x13, 0xA5, 0x2E, 0x27, 0xE1, 0x50, 0x46,
          0x70, 0x2C, 0x43, 0xC3, 0xF5, 0xC9, 0xF6, 0xAD, 0x5E, 0xDE, 0xDF, 0x65, 0xA9, 0x90, 0x25,
          0xDD, 0x46, 0xFC, 0xE6, 0x50, 0x6F, 0x3F, 0xFD, 0xC4, 0xC4, 0x5A, 0xD7, 0x40, 0xEC, 0x9D,
          0x22, 0x28, 0x85, 0xE9, 0x1F, 0x7E, 0xA4, 0xFD, 0xB8, 0x20, 0x69, 0x80, 0x8D, 0xE6, 0x4A,
          0xAD, 0x21, 0xDC, 0xD9, 0xE1, 0x1D, 0xA0, 0x56, 0x78, 0x3A, 0xBC, 0x19, 0x83, 0x2D, 0x9F,
          0xF6, 0x57, 0x5E, 0xB9, 0xAD, 0xEA, 0xB4, 0xBA, 0x1D, 0x1D, 0x7D, 0x73, 0x46, 0xFE, 0x57,
          0x29, 0xF5, 0xC2, 0xBE, 0xEE, 0x6B, 0x99, 0xCB, 0x81, 0x26, 0x1C, 0xF9, 0xAA, 0x4E, 0x7C,
          0x60, 0x56, 0xD1, 0xC0, 0x02, 0xFB, 0x1E, 0x98, 0xEA, 0x04, 0xD2, 0x6C, 0xE0, 0xE2, 0xE0,
          0x79, 0x2C, 0xC6, 0xB6, 0x62, 0x6F, 0xA5, 0xF5, 0xEB, 0x5F, 0x70, 0xEF, 0xA8, 0xC6, 0x3A,
          0x41, 0xB3, 0x8F, 0xA6, 0x04, 0x9E, 0x53, 0x4F, 0x61, 0x3A, 0x70, 0x22, 0x33, 0x2D, 0xC6,
          0x4E, 0x78, 0xA6, 0xE8, 0xD6, 0x5F, 0xD7, 0x2F, 0xAA, 0x9A, 0xE0, 0xD3, 0xC1, 0x77, 0x02,
          0x2F, 0x80, 0x23, 0x69, 0x87, 0x69, 0x3D, 0x25, 0x08, 0x70, 0xC3, 0xFA, 0xE1, 0xAD, 0x6D,
          0xA4, 0xE8, 0x8C, 0x40, 0xD1, 0x7E, 0x7D, 0x02, 0x95, 0x85, 0x46, 0x2C, 0x87, 0x95, 0x3F,
          0xE0, 0x18, 0x68, 0x28, 0x39, 0xE0, 0x1F, 0x79, 0x2D, 0x02, 0x32, 0x67, 0xF4, 0xF0, 0xD6,
          0xFA, 0x43, 0x04, 0xC3, 0xE0, 0x63, 0xE5, 0xB2, 0x53, 0x3E, 0x50, 0x9D, 0xCA, 0xE8, 0x01,
          0x61, 0x20, 0x4C, 0x3C, 0xC0, 0x49, 0xFD, 0x16, 0x0D, 0x46, 0x55, 0x11, 0xDD, 0x71, 0x8F,
          0x6E, 0xEA, 0xE4, 0xE7, 0x22, 0x9B, 0x3B, 0xDB, 0x42, 0xD0, 0x56, 0x74, 0x48, 0x42, 0x0F,
          0xA7},
         keyPairs2048bit,
         std::make_shared<PKCSPadding>()},
        // OAEP 1024-bit RSA key data set, hashing SHA256, masking SHA256
        {"Lorem ipsum dolor sit amet, consectetur",
         {0x40, 0x6B, 0x15, 0x44, 0xDF, 0x62, 0xCD, 0x33, 0x80, 0x13, 0x24, 0xFC, 0xC2, 0xAE, 0x18,
          0x4D, 0x2F, 0xD6, 0x32, 0x16, 0xA3, 0x86, 0x1F, 0x66, 0x15, 0xF8, 0x09, 0xE0, 0xAB, 0x1E,
          0x56, 0xC0, 0x4E, 0x35, 0x37, 0x11, 0x82, 0x07, 0xBD, 0xBF, 0xC8, 0x68, 0xBF, 0xF6, 0xF2,
          0x61, 0xCC, 0x71, 0xBC, 0x38, 0x5A, 0x88, 0x1D, 0xFB, 0x1D, 0x98, 0x97, 0xAE, 0xBE, 0xF8,
          0xE3, 0xDB, 0x02, 0x05, 0x8C, 0xE9, 0x16, 0x16, 0x32, 0x7E, 0x8D, 0x99, 0x4F, 0x85, 0x63,
          0x8B, 0x36, 0xA0, 0xBD, 0xD0, 0xA4, 0x99, 0xA8, 0x01, 0x22, 0xB4, 0x21, 0xD3, 0x0B, 0xC6,
          0x63, 0xE0, 0xA9, 0x4C, 0xA8, 0x7F, 0xB3, 0x36, 0x47, 0x64, 0x2F, 0x25, 0xB3, 0xF9, 0xBC,
          0x51, 0xA7, 0x7D, 0x91, 0x4F, 0xA6, 0xDE, 0x57, 0x52, 0xF9, 0x24, 0x9C, 0x18, 0x76, 0xFC,
          0xE4, 0xC0, 0x86, 0x67, 0x30, 0xA6, 0x60, 0x1B},
         keyPairs1024bit,
         std::make_shared<OAEPPadding>()},
        // OAEP 1024-bit RSA key data set, hashing SHA256, masking SHA256, label = CAFEBEEF
        {"Lorem ipsum dolor sit amet, consectetur",
         {0x7F, 0x42, 0x27, 0x9F, 0x88, 0xF4, 0x13, 0xAE, 0x09, 0x01, 0x94, 0x78, 0xDE, 0x2C, 0xAB,
          0x30, 0xED, 0x0F, 0xC2, 0x07, 0x78, 0x2C, 0x8E, 0x58, 0x19, 0xDB, 0x3C, 0x74, 0x26, 0x73,
          0x8E, 0xA0, 0x6F, 0x59, 0x13, 0xAD, 0x59, 0xFD, 0xD8, 0x65, 0x3E, 0x96, 0x09, 0x41, 0x7B,
          0xE1, 0xDF, 0xC9, 0x94, 0xAA, 0xCB, 0x00, 0x9B, 0x56, 0x57, 0x62, 0x04, 0xD0, 0x57, 0xD9,
          0x61, 0x44, 0x3C, 0xDE, 0x95, 0x50, 0xC8, 0x94, 0xBE, 0x45, 0xF5, 0xE5, 0xA7, 0x5F, 0xAD,
          0xD3, 0x7E, 0x60, 0x5F, 0x6E, 0x38, 0x43, 0xD7, 0x7D, 0x0C, 0x2D, 0xA8, 0x23, 0x07, 0xD8,
          0xB2, 0x54, 0x6E, 0x5F, 0x85, 0xAD, 0xBB, 0x6D, 0x8C, 0x87, 0x07, 0xB3, 0x59, 0x7E, 0x45,
          0xFD, 0x8E, 0x9B, 0x45, 0xAC, 0x42, 0x00, 0xE5, 0xE6, 0x7C, 0xEA, 0x97, 0xC6, 0xC6, 0x1D,
          0x7F, 0xEE, 0x6E, 0xEF, 0x93, 0x25, 0xE2, 0xFF},
         keyPairs1024bit,
         std::make_shared<OAEPPadding>(
                 openssl::DigestTypes::SHA256, std::make_shared<MGF1>(), "\xca\xfe\xbe\xef")},
        // OAEP 2048-bit RSA key data set, hashing SHA256, masking SHA256
        {"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor "
         "incididunt ut labore et dolore magna",
         {0x23, 0x99, 0x95, 0x95, 0x94, 0x1E, 0xB5, 0x23, 0x3E, 0x64, 0xC2, 0x6B, 0xEC, 0xE3, 0x2A,
          0x53, 0x88, 0xC9, 0xD5, 0x81, 0x7A, 0xDD, 0x15, 0xDD, 0x03, 0xAE, 0x2B, 0x84, 0x96, 0xA6,
          0xE6, 0xF9, 0x0D, 0x73, 0xF0, 0xE9, 0xA4, 0x66, 0x6F, 0x8C, 0xCB, 0xF9, 0xAD, 0xF6, 0xE5,
          0xF6, 0x5F, 0x23, 0x8A, 0x1D, 0x42, 0xB3, 0x69, 0x72, 0x8F, 0xA0, 0x59, 0x94, 0x29, 0xF4,
          0xBC, 0x0D, 0x2D, 0xF4, 0x19, 0x14, 0xE0, 0x9E, 0x73, 0x08, 0xC8, 0xA5, 0xF8, 0x48, 0x49,
          0x4B, 0xB3, 0x69, 0x75, 0xE8, 0x3A, 0x99, 0xAE, 0x70, 0xFC, 0xE5, 0x56, 0xFF, 0xB7, 0x05,
          0x52, 0x68, 0x95, 0xDA, 0x76, 0x1B, 0xF1, 0x5D, 0x11, 0xE4, 0x57, 0xE2, 0x39, 0x8D, 0x29,
          0xEF, 0x39, 0x18, 0xBA, 0x19, 0xA0, 0xF1, 0x3C, 0x93, 0x30, 0x60, 0x23, 0x0E, 0x28, 0x73,
          0x7C, 0x93, 0x46, 0x5C, 0x4A, 0xE0, 0xDE, 0xBC, 0xB8, 0x40, 0x21, 0x4C, 0x15, 0xB4, 0xF4,
          0xEB, 0x4A, 0x1B, 0x21, 0x0F, 0xF3, 0x54, 0x1F, 0xF4, 0xDA, 0xD2, 0x13, 0xB4, 0x7E, 0xE7,
          0x95, 0x34, 0xCF, 0x5A, 0x9A, 0xDB, 0x07, 0x7D, 0x27, 0x9E, 0x87, 0xDF, 0x56, 0xD7, 0x2D,
          0xF2, 0x84, 0x87, 0xC0, 0xB1, 0x22, 0xF4, 0x9A, 0x09, 0x76, 0x3D, 0x45, 0x32, 0xF8, 0x25,
          0xC3, 0x5C, 0x7D, 0xA0, 0x4D, 0x23, 0xDE, 0x93, 0x85, 0xC4, 0x93, 0x46, 0x69, 0xA2, 0x59,
          0x16, 0x67, 0x9E, 0x49, 0x5E, 0x01, 0xE3, 0x8A, 0x8E, 0x0E, 0x5D, 0xD7, 0xD6, 0x55, 0x12,
          0xFC, 0xEC, 0x31, 0x49, 0xED, 0xB5, 0xA9, 0xAA, 0x82, 0x3F, 0x87, 0x33, 0xA2, 0xC9, 0x9F,
          0x5B, 0xA0, 0xA1, 0x60, 0xCC, 0x5B, 0x2B, 0x18, 0x91, 0xCB, 0xA3, 0xAC, 0x2E, 0x87, 0x3E,
          0x33, 0x39, 0x0F, 0x05, 0x20, 0xBE, 0x87, 0xB1, 0xDB, 0xAB, 0x28, 0x60, 0x2E, 0xE2, 0x8F,
          0x6C},
         keyPairs2048bit,
         std::make_shared<OAEPPadding>()},
        // OAEP 2048-bit RSA key data set, hashing SHA512, masking SHA512
        {"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor "
         "incididunt ut labore et dolore magna",
         {0x66, 0xD3, 0x71, 0x1F, 0xC9, 0x7A, 0x69, 0x32, 0xAC, 0x43, 0xDC, 0x41, 0xEE, 0x84, 0x0F,
          0xB9, 0x7E, 0x95, 0xB9, 0xEF, 0xA9, 0xA3, 0xD5, 0xEE, 0x85, 0x36, 0xDC, 0x2A, 0x03, 0x87,
          0x94, 0xE5, 0x6F, 0x39, 0x96, 0x24, 0x84, 0xEF, 0x5F, 0x3D, 0x30, 0x6A, 0x87, 0x5F, 0xF7,
          0xBD, 0x12, 0xC6, 0x2C, 0xAD, 0x15, 0x23, 0x44, 0x3E, 0x26, 0x03, 0xDB, 0x34, 0xED, 0x4F,
          0xDC, 0xF5, 0x29, 0x5A, 0x46, 0x07, 0xD7, 0x3D, 0xD6, 0x16, 0xAB, 0x7B, 0x50, 0x7B, 0x0C,
          0xE3, 0x05, 0xC6, 0x59, 0x09, 0xC3, 0x5F, 0x84, 0x2B, 0x7B, 0xD3, 0xA3, 0xC5, 0x1B, 0x73,
          0x95, 0x54, 0xE3, 0xC2, 0x06, 0x79, 0xE9, 0x3E, 0x02, 0x7D, 0x3B, 0xCF, 0x49, 0xB9, 0xCF,
          0xBB, 0xDD, 0xE0, 0x67, 0xE4, 0xC7, 0xA4, 0x49, 0x94, 0x7C, 0xBA, 0xE2, 0x99, 0x2D, 0x6C,
          0x64, 0x8D, 0x68, 0x80, 0x21, 0x28, 0x15, 0x4F, 0x7D, 0x38, 0x77, 0xDF, 0x27, 0x7B, 0x94,
          0xA6, 0x11, 0xE4, 0xC2, 0x3A, 0x0F, 0x2B, 0x23, 0x3F, 0x22, 0xAD, 0x97, 0x72, 0xC6, 0x9B,
          0x74, 0xE3, 0x27, 0x4E, 0x96, 0xA6, 0xAB, 0x26, 0x77, 0x3F, 0x7F, 0x6B, 0xD9, 0xEA, 0x91,
          0x0D, 0x7B, 0x95, 0x96, 0x57, 0x2C, 0x0B, 0x45, 0x23, 0x1A, 0xC5, 0x4D, 0xA5, 0xAD, 0xE8,
          0xC1, 0xE8, 0x2D, 0x7F, 0x85, 0x76, 0xE3, 0xDB, 0x8E, 0x19, 0x64, 0xB2, 0x67, 0xFB, 0xE2,
          0xEA, 0x05, 0x78, 0x7B, 0xD2, 0xE5, 0xAE, 0xEB, 0x18, 0x26, 0x9E, 0xBE, 0x02, 0x37, 0x32,
          0x1C, 0xD4, 0x91, 0xCC, 0xE3, 0xCA, 0xF3, 0x07, 0x08, 0x71, 0x94, 0x87, 0xE0, 0x6F, 0x9F,
          0xF8, 0x19, 0x9A, 0xB2, 0xF7, 0x83, 0x5A, 0x2F, 0x9F, 0x9F, 0x3B, 0xA3, 0x61, 0x70, 0x10,
          0x22, 0xE1, 0x43, 0x8A, 0x62, 0xDE, 0x1D, 0x3A, 0x48, 0x0B, 0x73, 0x76, 0xFE, 0x2D, 0x7D,
          0x21},
         keyPairs2048bit,
         std::make_shared<OAEPPadding>(openssl::DigestTypes::SHA512)},
        // OAEP 2048-bit RSA key data set, hashing SHA256, masking SHA512
        {"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor "
         "incididunt ut labore et dolore magna",
         {0x60, 0x41, 0x91, 0x35, 0xC1, 0xD7, 0x62, 0xE7, 0x92, 0xFF, 0x83, 0xA6, 0x3D, 0x5B, 0x48,
          0x10, 0x62, 0xC9, 0xD8, 0x87, 0x59, 0x9E, 0x6F, 0xCC, 0xC0, 0xD0, 0x9C, 0x50, 0xED, 0x53,
          0x6D, 0xB1, 0xB7, 0xF6, 0xF1, 0xB6, 0x88, 0xB6, 0x60, 0x51, 0x25, 0xBD, 0x57, 0xC5, 0xD8,
          0x0B, 0xCF, 0xDC, 0x7F, 0xF8, 0x1B, 0xDD, 0x96, 0x2D, 0xC5, 0x61, 0x1C, 0xD6, 0x74, 0xDD,
          0x9A, 0xF3, 0x17, 0xB6, 0x72, 0x83, 0x78, 0x87, 0xDD, 0x30, 0x6E, 0x29, 0x6A, 0x7B, 0x58,
          0xB4, 0xB0, 0x6B, 0x7C, 0x6E, 0x54, 0xA5, 0x17, 0xD7, 0x6F, 0xD3, 0xA9, 0xB4, 0xD8, 0xB4,
          0x40, 0x65, 0xA8, 0x9E, 0x50, 0x64, 0xC5, 0xBC, 0x66, 0x38, 0x06, 0x3B, 0x99, 0xA3, 0x11,
          0x21, 0xBD, 0x71, 0x1F, 0x2A, 0x46, 0xD0, 0xFF, 0xCC, 0x63, 0x97, 0xD8, 0xFF, 0xFF, 0x57,
          0xB9, 0x1E, 0x8B, 0xF8, 0x74, 0x58, 0x03, 0xCC, 0xE1, 0x90, 0xDA, 0x94, 0x45, 0x84, 0x4B,
          0x55, 0xD6, 0x3A, 0x14, 0x1E, 0xD8, 0xF3, 0xF0, 0x37, 0xB5, 0x08, 0x38, 0x1C, 0x38, 0x1E,
          0x5C, 0x1C, 0x77, 0xF8, 0xCC, 0x61, 0x91, 0xA9, 0x76, 0xDF, 0xDB, 0xB5, 0x06, 0xFD, 0x06,
          0xC7, 0xC4, 0xE4, 0x11, 0xCB, 0xC0, 0x88, 0xFF, 0x18, 0x5C, 0x6F, 0x64, 0x57, 0x5B, 0x8D,
          0x65, 0xD1, 0x90, 0x1B, 0x8A, 0xE0, 0x28, 0x63, 0x60, 0xAB, 0x3D, 0x8D, 0xEC, 0xD8, 0x21,
          0x19, 0x21, 0x86, 0x8B, 0xA5, 0x32, 0x5A, 0xA4, 0xE5, 0x25, 0x26, 0x85, 0x7B, 0x1B, 0xB8,
          0x85, 0x9B, 0x24, 0x55, 0x46, 0x96, 0xA4, 0xC1, 0x88, 0x30, 0xE9, 0xFD, 0x76, 0x7C, 0xCC,
          0x93, 0xE2, 0x4E, 0x6F, 0xE1, 0xCD, 0x35, 0xF1, 0xE5, 0x1C, 0x43, 0xB7, 0xA7, 0xD5, 0x32,
          0x3F, 0x11, 0xF7, 0x5F, 0x74, 0x81, 0xC1, 0xD6, 0x9A, 0x15, 0x5E, 0x03, 0x31, 0x3B, 0xC4,
          0x09},
         keyPairs2048bit,
         std::make_shared<OAEPPadding>(openssl::DigestTypes::SHA256,
                                       std::make_shared<MGF1>(openssl::DigestTypes::SHA512))},
        // No Padding, 1024-bit RSA key data set
        {"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce iaculis fermentum ante "
         "non aliquet. Fusce molestie mauris nullam.",
         {0x38, 0x9F, 0x7E, 0x46, 0xB1, 0x21, 0x69, 0x64, 0xF3, 0x4E, 0xD8, 0xCF, 0xF0, 0xB7, 0x2A,
          0x38, 0xE8, 0xC6, 0x6B, 0x40, 0x1A, 0x7E, 0x0C, 0xB1, 0x1E, 0x91, 0xCC, 0x73, 0xA5, 0xE9,
          0x6D, 0xEC, 0x85, 0xA4, 0xE3, 0x99, 0xAC, 0xF4, 0x75, 0x21, 0x77, 0x10, 0xBD, 0x42, 0xA6,
          0x45, 0x32, 0x67, 0x41, 0xCE, 0xEE, 0xAA, 0x65, 0xED, 0xD2, 0xE7, 0x38, 0x52, 0x45, 0x2B,
          0x28, 0x3A, 0xAC, 0x43, 0x67, 0x22, 0x1E, 0xEA, 0x71, 0x47, 0xE5, 0xB2, 0x0E, 0xB4, 0x8E,
          0x49, 0x6E, 0x19, 0x0F, 0xCA, 0xED, 0x56, 0x42, 0x10, 0x7C, 0xDE, 0x54, 0x72, 0x48, 0x98,
          0x5F, 0x67, 0x01, 0xF2, 0x81, 0xC3, 0x4F, 0x93, 0xF5, 0xE5, 0x03, 0x21, 0xC2, 0xCA, 0x6D,
          0xA3, 0x69, 0x30, 0x5A, 0xB3, 0x2E, 0xD3, 0xA2, 0xFA, 0x9A, 0xEE, 0x9F, 0x8E, 0x83, 0x23,
          0xAD, 0x46, 0xE2, 0x72, 0x52, 0x22, 0x47, 0x20},
         keyPairs1024bit,
         std::make_shared<NoPadding>()},
        // No Padding, 2048-bit RSA key data set
        {"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin arcu urna, cursus vel "
         "urna sed, viverra feugiat nulla. Nullam rutrum mi a purus posuere, id scelerisque massa "
         "commodo. Aenean egestas quis sapien ultricies tempor. Aenean magna quam massa nunc.",
         {0x8C, 0x4E, 0x9F, 0x2D, 0x41, 0x93, 0xA4, 0x7C, 0x4F, 0xCE, 0x94, 0xF1, 0x76, 0xA2, 0x93,
          0x8E, 0xA2, 0xD7, 0x58, 0x88, 0x4F, 0x10, 0x66, 0xBC, 0x63, 0x24, 0xA8, 0xB5, 0x05, 0x7F,
          0x29, 0x1B, 0xA6, 0xD1, 0x58, 0x2F, 0xB1, 0xD4, 0x3D, 0x9B, 0xA4, 0x0D, 0x21, 0xB7, 0xEF,
          0xA2, 0x61, 0xCA, 0xDD, 0x1A, 0xED, 0xC6, 0x02, 0x54, 0x1C, 0x25, 0xFE, 0xED, 0xCE, 0x21,
          0x10, 0xE0, 0xF8, 0x0E, 0x00, 0xB6, 0x68, 0x2D, 0x91, 0x9E, 0x8D, 0x64, 0xE6, 0xAC, 0x66,
          0xF2, 0x84, 0x47, 0xC8, 0xAF, 0x1F, 0xFF, 0xF9, 0x71, 0x1C, 0x21, 0x6B, 0x50, 0xB2, 0xC7,
          0x3A, 0x2A, 0xC5, 0x50, 0x2D, 0xE7, 0x8F, 0x90, 0xF5, 0xE4, 0x6F, 0x17, 0x9F, 0x4F, 0x32,
          0x76, 0xEC, 0xDA, 0x9D, 0xE4, 0xFC, 0x4A, 0x12, 0xE4, 0x9B, 0x41, 0xD6, 0xA9, 0x25, 0x37,
          0x38, 0x47, 0x2A, 0x4B, 0xE9, 0xD0, 0x7A, 0xF6, 0xE8, 0x0F, 0x10, 0x07, 0x77, 0x04, 0x7F,
          0xCB, 0xB9, 0x55, 0x58, 0xD1, 0xC2, 0x4C, 0xF1, 0xC6, 0xA5, 0x71, 0x8E, 0x75, 0x73, 0x22,
          0x8E, 0xC8, 0x62, 0x2A, 0xB0, 0x4D, 0x80, 0x9A, 0x16, 0x6D, 0xED, 0x5A, 0xAF, 0x29, 0xB3,
          0x60, 0x29, 0x82, 0xF2, 0xEE, 0xA2, 0xB5, 0x3F, 0x75, 0x18, 0x4B, 0x36, 0x40, 0x56, 0x23,
          0xC4, 0x7A, 0xC3, 0xDB, 0x75, 0x2D, 0xCC, 0xBE, 0xE5, 0xF7, 0x74, 0x41, 0xB3, 0x62, 0x74,
          0xF5, 0x94, 0x6E, 0x3E, 0x78, 0x01, 0x40, 0xCC, 0x24, 0x36, 0x06, 0x92, 0x00, 0x77, 0x08,
          0x9F, 0x96, 0x89, 0xB5, 0x25, 0xD2, 0x34, 0xD2, 0xC8, 0xBB, 0x78, 0x4E, 0x2C, 0x10, 0x7D,
          0x19, 0x89, 0xDD, 0xDF, 0x1E, 0x59, 0x96, 0x9A, 0xEF, 0x82, 0x82, 0xFB, 0x75, 0xBE, 0x94,
          0xE7, 0xC8, 0x8A, 0xE2, 0xB8, 0x44, 0x7D, 0x85, 0x35, 0x57, 0x08, 0x8F, 0x49, 0xAB, 0x79,
          0x2A},
         keyPairs2048bit,
         std::make_shared<NoPadding>()}};

const NominalDataSet& AsymmetricEncryptionTest::OAEPPaddingDataSet = nominalDataSet[2];

/**
 * @brief Tests the nominal scenarios of the decryption functionality. The nominal encryption data
 * set was generated using the openssl pkeyutl cli, thus here we decrypt a previous known and
 * encrypted message and check if we get the original known message.
 */
TEST_P(AsymmetricEncryptionTest, testSuccessfulDecryption)
{
    auto dataSet = GetParam();

    auto message = dataSet.decryptionCtx->decrypt(dataSet.encrypted);
    std::string str(message.begin(), message.end());
    EXPECT_EQ(dataSet.message, str);
}

/**
 * @brief Tests the nominal scenarios of the encryption functionality. This test relies on the
 * fully tested and working decryption functionality. It encrypts a known message, decrypts it and
 * checks if the output matches the original known message.
 */
TEST_P(AsymmetricEncryptionTest, testSuccessfulEncryption)
{
    auto dataSet = GetParam();

    std::vector<uint8_t> message(dataSet.message.begin(), dataSet.message.end());
    auto encrypted = dataSet.encryptionCtx->encrypt(message);
    auto decrypted = dataSet.decryptionCtx->decrypt(encrypted);

    EXPECT_EQ(decrypted, message);
}

INSTANTIATE_TEST_CASE_P(testSuccessfulEncryptionDecryption,
                        AsymmetricEncryptionTest,
                        testing::ValuesIn(AsymmetricEncryptionTest::nominalDataSet));

/**
 * @brief Tests that decryption is unsuccessful if a wrong padding mode is used.
 */
TEST_F(AsymmetricEncryptionTest, testUnsuccessfulDecryptionPKCSInsteadOfOEAP)
{
    auto ctxDecrypt =
            RSAEncryptionPrivateKeyCtx(keyPairs1024bit.privKey, std::make_shared<PKCSPadding>());
    EXPECT_THROW(ctxDecrypt.decrypt(OAEPPaddingDataSet.encrypted), MoCOCrWException);
}

/**
 * @brief Tests the decryption functionality with wrong key.
 */
TEST_F(AsymmetricEncryptionTest, testUnsuccessfulDecryptionWrongKey)
{
    auto ctxDecrypt =
            RSAEncryptionPrivateKeyCtx(keyPairs2048bit.privKey, std::make_shared<OAEPPadding>());
    EXPECT_THROW(ctxDecrypt.decrypt(OAEPPaddingDataSet.encrypted), MoCOCrWException);
}

/**
 * @brief Tests the decryption functionality with OEAP that uses a wrong hash function.
 */
TEST_F(AsymmetricEncryptionTest, testUnsuccessfulDecryptionWrongHashFunctionOEAP)
{
    auto padding = std::make_shared<OAEPPadding>(openssl::DigestTypes::SHA512, nullptr);
    auto ctxDecrypt = RSAEncryptionPrivateKeyCtx(keyPairs2048bit.privKey, padding);
    EXPECT_THROW(ctxDecrypt.decrypt(OAEPPaddingDataSet.encrypted), MoCOCrWException);
}

/**
 * @brief Tests the decryption functionality with OEAP that uses a wrongly configured MGF1.
 */
TEST_F(AsymmetricEncryptionTest, testUnsuccessfulDecryptionWrongMGF1OEAP)
{
    auto mgf = std::make_shared<MGF1>(openssl::DigestTypes::SHA384);
    auto padding = std::make_shared<OAEPPadding>(openssl::DigestTypes::SHA256, mgf);
    auto ctxDecrypt = RSAEncryptionPrivateKeyCtx(keyPairs2048bit.privKey, padding);
    EXPECT_THROW(ctxDecrypt.decrypt(OAEPPaddingDataSet.encrypted), MoCOCrWException);
}

/**
 * @brief Tests the decryption functionality with OEAP that uses a wrong label.
 */
TEST_F(AsymmetricEncryptionTest, testUnsuccessfulDecryptionWrongLabelOEAP)
{
    auto padding = std::make_shared<OAEPPadding>(
            openssl::DigestTypes::SHA256, nullptr, "\xaa\xbb\xcc\xdd");
    auto ctxDecrypt = RSAEncryptionPrivateKeyCtx(keyPairs2048bit.privKey, padding);
    EXPECT_THROW(ctxDecrypt.decrypt(OAEPPaddingDataSet.encrypted), MoCOCrWException);
}

TEST(AsymmetricPublicKeyTest, ecdhSmokeTest)
{
    ECCSpec spec;
    AsymmetricKey key(spec.generate());
    AsymmetricPublicKey pubKey = spec.generate().internal();
    /* Just check if the function returns without indicating a failure */
    EXPECT_NO_THROW(openssl::_EVP_derive_key(pubKey.internal(), key.internal().get()));
}
