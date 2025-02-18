/*
 * #%L
 * %%
 * Copyright (C) 2022 BMW Car IT GmbH
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
#include <cstddef>

#include <gmock/gmock.h>

#include "mococrw/openssl_wrap.h"
#include "openssl_lib_mock.h"

namespace mococrw
{
namespace openssl
{
std::unique_ptr<::testing::NiceMock<OpenSSLLibMock>> OpenSSLLibMockManager::_mock{nullptr};
std::mutex OpenSSLLibMockManager::_mutex{};

/*
 * If there is no mock yet, we create a new one.
 */
testing::NiceMock<OpenSSLLibMock>& OpenSSLLibMockManager::getMockInterface()
{
    if (!_mock) {
        resetMock();
    }

    return *_mock;
}

void OpenSSLLibMockManager::resetMock()
{
    std::lock_guard<std::mutex> _lock(_mutex);
    _mock = std::make_unique<::testing::NiceMock<OpenSSLLibMock>>();
}

void OpenSSLLibMockManager::destroy()
{
    std::lock_guard<std::mutex> _lock(_mutex);
    // NOLINTNEXTLINE(clang-analyzer-optin.cplusplus.VirtualCall)
    _mock.reset();
}

namespace lib
{
/**
 * Provide implementations for the OpenSSLLib members that forward
 * to the mock object.
 *
 * This translation unit is inserted into the bulid process for unit-tests
 * via CMake. The library gets these definitions from a corresponding class
 * in the src-dir.
 */
// TODO unit test the initialization
void OpenSSLLib::SSL_CRYPTO_malloc_init() noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_CRYPTO_malloc_init();
}

void OpenSSLLib::SSL_ERR_load_crypto_strings() noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_ERR_load_crypto_strings();
}

void OpenSSLLib::SSL_SSL_load_error_strings() noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_SSL_load_error_strings();
}

void OpenSSLLib::SSL_OpenSSL_add_all_algorithms() noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_OpenSSL_add_all_algorithms();
}

X509_REQ* OpenSSLLib::SSL_X509_REQ_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_REQ_new();
}

X509_NAME* OpenSSLLib::SSL_X509_REQ_get_subject_name(const X509_REQ* req) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_REQ_get_subject_name(req);
}

EVP_PKEY* OpenSSLLib::SSL_X509_REQ_get_pubkey(X509_REQ* req) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_REQ_get_pubkey(req);
}

int OpenSSLLib::SSL_X509_REQ_verify(X509_REQ* a, EVP_PKEY* r) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_REQ_verify(a, r);
}

void OpenSSLLib::SSL_X509_REQ_free(X509_REQ* ptr) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509_REQ_free(ptr);
}

EVP_PKEY* OpenSSLLib::SSL_EVP_PKEY_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_new();
}

void OpenSSLLib::SSL_EVP_PKEY_free(EVP_PKEY* ptr) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_free(ptr);
}

int OpenSSLLib::SSL_EVP_PKEY_keygen(EVP_PKEY_CTX* ctx, EVP_PKEY** ppkey) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_keygen(ctx, ppkey);
}

int OpenSSLLib::SSL_EVP_PKEY_keygen_init(EVP_PKEY_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_keygen_init(ctx);
}

EVP_PKEY_CTX* OpenSSLLib::SSL_EVP_PKEY_CTX_new(EVP_PKEY* pkey, ENGINE* engine) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_new(pkey, engine);
}

EVP_PKEY_CTX* OpenSSLLib::SSL_EVP_PKEY_CTX_new_id(int id, ENGINE* engine) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_new_id(id, engine);
}

void OpenSSLLib::SSL_EVP_PKEY_CTX_free(EVP_PKEY_CTX* ptr) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_free(ptr);
}

int OpenSSLLib::SSL_EVP_PKEY_CTX_set_rsa_keygen_bits(EVP_PKEY_CTX* ctx, int mbits) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_set_rsa_keygen_bits(ctx,
                                                                                          mbits);
}

int OpenSSLLib::SSL_EVP_PKEY_cmp(const EVP_PKEY* a, const EVP_PKEY* b) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_cmp(a, b);
}

int OpenSSLLib::SSL_EVP_PKEY_paramgen_init(EVP_PKEY_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_paramgen_init(ctx);
}

int OpenSSLLib::SSL_EVP_PKEY_paramgen(EVP_PKEY_CTX* ctx, EVP_PKEY** ppkey) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_paramgen(ctx, ppkey);
}

int OpenSSLLib::SSL_EVP_PKEY_CTX_set_ec_paramgen_curve_nid(EVP_PKEY_CTX* ctx, int nid) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_set_ec_paramgen_curve_nid(
            ctx, nid);
}

int OpenSSLLib::SSL_EVP_PKEY_CTX_set_ec_param_enc(EVP_PKEY_CTX* ctx, int param_enc) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_set_ec_param_enc(ctx,
                                                                                       param_enc);
}

const EC_GROUP* OpenSSLLib::SSL_EC_KEY_get0_group(const EC_KEY* key) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EC_KEY_get0_group(key);
}

int OpenSSLLib::SSL_EC_GROUP_get_degree(const EC_GROUP* group) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EC_GROUP_get_degree(group);
}

int OpenSSLLib::SSL_EC_GROUP_get_curve_name(const EC_GROUP* group) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EC_GROUP_get_curve_name(group);
}

int OpenSSLLib::SSL_EVP_PKEY_type(int type) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_type(type);
}

int OpenSSLLib::SSL_EVP_PKEY_id(const EVP_PKEY* pkey) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_id(pkey);
}

int OpenSSLLib::SSL_EVP_PKEY_size(EVP_PKEY* pkey) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_size(pkey);
}

char* OpenSSLLib::SSL_ERR_error_string(unsigned long error, char* buf) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ERR_error_string(error, buf);
}

unsigned long OpenSSLLib::SSL_ERR_get_error() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ERR_get_error();
}

X509_NAME* OpenSSLLib::SSL_X509_NAME_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_NAME_new();
}

void OpenSSLLib::SSL_X509_NAME_free(X509_NAME* ptr) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509_NAME_free(ptr);
}

int OpenSSLLib::SSL_X509_NAME_add_entry_by_NID(X509_NAME* name,
                                               int nid,
                                               int type,
                                               unsigned char* bytes,
                                               int len,
                                               int loc,
                                               int set) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_NAME_add_entry_by_NID(
            name, nid, type, bytes, len, loc, set);
}

int OpenSSLLib::SSL_X509_REQ_set_subject_name(X509_REQ* req, X509_NAME* name) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_REQ_set_subject_name(req, name);
}

int OpenSSLLib::SSL_X509_REQ_set_pubkey(X509_REQ* req, EVP_PKEY* pkey) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_REQ_set_pubkey(req, pkey);
}

int OpenSSLLib::SSL_X509_REQ_set_version(X509_REQ* ctx, unsigned long version) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_REQ_set_version(ctx, version);
}

int OpenSSLLib::SSL_PEM_write_bio_X509_REQ(BIO* bio, X509_REQ* req) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_write_bio_X509_REQ(bio, req);
}

X509_REQ* OpenSSLLib::SSL_PEM_read_bio_X509_REQ(BIO* bp,
                                                X509_REQ** x,
                                                pem_password_cb* cb,
                                                void* u) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_read_bio_X509_REQ(bp, x, cb, u);
}

const BIO_METHOD* OpenSSLLib::SSL_BIO_s_mem() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BIO_s_mem();
}

void OpenSSLLib::SSL_BIO_free_all(BIO* ptr) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_BIO_free_all(ptr);
}

BIO* OpenSSLLib::SSL_BIO_new(const BIO_METHOD* method) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BIO_new(method);
}

int OpenSSLLib::SSL_BIO_gets(BIO* bio, char* buf, int size) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BIO_gets(bio, buf, size);
}

int OpenSSLLib::SSL_BIO_puts(BIO* bio, char* buf) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BIO_puts(bio, buf);
}

int OpenSSLLib::SSL_X509_REQ_sign_ctx(X509_REQ* req, EVP_MD_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_REQ_sign_ctx(req, ctx);
}

EVP_MD_CTX* OpenSSLLib::SSL_EVP_MD_CTX_create() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_MD_CTX_create();
}

int OpenSSLLib::SSL_EVP_DigestSignInit(EVP_MD_CTX* ctx,
                                       EVP_PKEY_CTX** pctx,
                                       const EVP_MD* type,
                                       ENGINE* e,
                                       EVP_PKEY* pkey) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_DigestSignInit(
            ctx, pctx, type, e, pkey);
}

void OpenSSLLib::SSL_EVP_MD_CTX_destroy(EVP_MD_CTX* ptr) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_EVP_MD_CTX_destroy(ptr);
}

const EVP_MD* OpenSSLLib::SSL_EVP_sha1() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_sha1();
}

const EVP_MD* OpenSSLLib::SSL_EVP_sha256() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_sha256();
}

const EVP_MD* OpenSSLLib::SSL_EVP_sha384() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_sha384();
}

const EVP_MD* OpenSSLLib::SSL_EVP_sha512() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_sha512();
}

const EVP_MD* OpenSSLLib::SSL_EVP_sha3_256() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_sha3_256();
}

const EVP_MD* OpenSSLLib::SSL_EVP_sha3_384() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_sha3_384();
}

const EVP_MD* OpenSSLLib::SSL_EVP_sha3_512() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_sha3_512();
}

int OpenSSLLib::SSL_PEM_write_bio_PKCS8PrivateKey(BIO* bp,
                                                  EVP_PKEY* x,
                                                  const EVP_CIPHER* enc,
                                                  char* kstr,
                                                  int klen,
                                                  pem_password_cb* cb,
                                                  void* u) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_write_bio_PKCS8PrivateKey(
            bp, x, enc, kstr, klen, cb, u);
}

int OpenSSLLib::SSL_PEM_write_bio_PUBKEY(BIO* bp, EVP_PKEY* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_write_bio_PUBKEY(bp, x);
}
EVP_PKEY* OpenSSLLib::SSL_PEM_read_bio_PUBKEY(BIO* bio,
                                              EVP_PKEY** pkey,
                                              pem_password_cb* cb,
                                              void* u) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_read_bio_PUBKEY(bio, pkey, cb, u);
}

EVP_PKEY* OpenSSLLib::SSL_PEM_read_bio_PrivateKey(BIO* bio,
                                                  EVP_PKEY** pkey,
                                                  pem_password_cb* cb,
                                                  void* u) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_read_bio_PrivateKey(bio, pkey, cb, u);
}

X509* OpenSSLLib::SSL_d2i_X509_bio(BIO* bp, X509** x509) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_d2i_X509_bio(bp, x509);
}

int OpenSSLLib::SSL_i2d_X509_bio(BIO* bp, X509* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_i2d_X509_bio(bp, x);
}

X509* OpenSSLLib::SSL_X509_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_new();
}

int OpenSSLLib::SSL_X509_set_pubkey(X509* ptr, EVP_PKEY* pkey) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_set_pubkey(ptr, pkey);
}

int OpenSSLLib::SSL_X509_set_issuer_name(X509* x, X509_NAME* name) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_set_issuer_name(x, name);
}

int OpenSSLLib::SSL_X509_set_subject_name(X509* x, X509_NAME* name) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_set_subject_name(x, name);
}

int OpenSSLLib::SSL_X509_set_notBefore(X509* x, const ASN1_TIME* t) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_set_notBefore(x, t);
}

int OpenSSLLib::SSL_X509_set_notAfter(X509* x, const ASN1_TIME* t) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_set_notAfter(x, t);
}

int OpenSSLLib::SSL_X509_sign(X509* x, EVP_PKEY* pkey, const EVP_MD* md) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_sign(x, pkey, md);
}

void OpenSSLLib::SSL_X509_free(X509* ptr) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509_free(ptr);
}

X509* OpenSSLLib::SSL_PEM_read_bio_X509(BIO* bio, X509** x, pem_password_cb* cb, void* pwd) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_read_bio_X509(bio, x, cb, pwd);
}

int OpenSSLLib::SSL_PEM_write_bio_X509(BIO* bp, X509* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_write_bio_X509(bp, x);
}

void OpenSSLLib::SSL_ASN1_TIME_free(ASN1_TIME* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_TIME_free(x);
}

int OpenSSLLib::SSL_ASN1_TIME_diff(int* pday,
                                   int* psec,
                                   const ASN1_TIME* from,
                                   const ASN1_TIME* to) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_TIME_diff(pday, psec, from, to);
}

ASN1_TIME* OpenSSLLib::SSL_ASN1_TIME_set(ASN1_TIME* s, time_t t) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_TIME_set(s, t);
}

/* X509 */
X509_NAME* OpenSSLLib::SSL_X509_get_subject_name(X509* ptr) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_get_subject_name(ptr);
}

X509_NAME* OpenSSLLib::SSL_X509_get_issuer_name(X509* ptr) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_get_issuer_name(ptr);
}

EVP_PKEY* OpenSSLLib::SSL_X509_get_pubkey(X509* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_get_pubkey(x);
}

ASN1_TIME* OpenSSLLib::SSL_X509_get_notBefore(X509* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_get_notBefore(x);
}

ASN1_TIME* OpenSSLLib::SSL_X509_get_notAfter(X509* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_get_notAfter(x);
}

/* X509 Certificate validation */
X509_STORE* OpenSSLLib::SSL_X509_STORE_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_new();
}

void OpenSSLLib::SSL_X509_STORE_free(X509_STORE* v) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_free(v);
}

int OpenSSLLib::SSL_X509_STORE_add_cert(X509_STORE* ctx, X509* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_add_cert(ctx, x);
}

X509_STORE_CTX* OpenSSLLib::SSL_X509_STORE_CTX_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_CTX_new();
}

void OpenSSLLib::SSL_X509_STORE_CTX_free(X509_STORE_CTX* ctx) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_CTX_free(ctx);
}
int OpenSSLLib::SSL_X509_STORE_CTX_init(X509_STORE_CTX* ctx,
                                        X509_STORE* store,
                                        X509* x509,
                                        STACK_OF(X509) * chain) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_CTX_init(
            ctx, store, x509, chain);
}

X509_VERIFY_PARAM* OpenSSLLib::SSL_X509_STORE_CTX_get0_param(X509_STORE_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_CTX_get0_param(ctx);
}

int OpenSSLLib::SSL_X509_VERIFY_PARAM_set_flags(X509_VERIFY_PARAM* param,
                                                unsigned long flags) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_VERIFY_PARAM_set_flags(param, flags);
}

int OpenSSLLib::SSL_X509_verify_cert(X509_STORE_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_verify_cert(ctx);
}

const char* OpenSSLLib::SSL_X509_verify_cert_error_string(long n) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_verify_cert_error_string(n);
}

int OpenSSLLib::SSL_X509_STORE_CTX_get_error(X509_STORE_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_CTX_get_error(ctx);
}

int OpenSSLLib::SSL_X509_check_ca(X509* cert) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_check_ca(cert);
}

/* stack of X509 */
STACK_OF(X509) * OpenSSLLib::SSL_sk_X509_new_null() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_sk_X509_new_null();
}

int OpenSSLLib::SSL_sk_X509_push(STACK_OF(X509) * stack, const X509* crt) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_sk_X509_push(stack, crt);
}
void OpenSSLLib::SSL_sk_X509_free(STACK_OF(X509) * stack) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_sk_X509_free(stack);
}

int OpenSSLLib::SSL_X509_NAME_get_index_by_NID(X509_NAME* name, int nid, int lastpos) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_NAME_get_index_by_NID(
            name, nid, lastpos);
}

X509_NAME_ENTRY* OpenSSLLib::SSL_X509_NAME_get_entry(X509_NAME* name, int loc) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_NAME_get_entry(name, loc);
}

ASN1_STRING* OpenSSLLib::SSL_X509_NAME_ENTRY_get_data(X509_NAME_ENTRY* ne) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_NAME_ENTRY_get_data(ne);
}

int OpenSSLLib::SSL_ASN1_STRING_print_ex(BIO* out, ASN1_STRING* str, unsigned long flags) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_STRING_print_ex(out, str, flags);
}

BIO* OpenSSLLib::SSL_BIO_new_file(const char* filename, const char* mode) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BIO_new_file(filename, mode);
}
int OpenSSLLib::SSL_BIO_read(BIO* b, void* buf, int len) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BIO_read(b, buf, len);
}
int OpenSSLLib::SSL_BIO_write(BIO* b, const void* buf, int len) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BIO_write(b, buf, len);
}
const EVP_CIPHER* OpenSSLLib::SSL_EVP_aes_128_cbc() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_aes_128_cbc();
}
const EVP_CIPHER* OpenSSLLib::SSL_EVP_aes_256_cbc() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_aes_256_cbc();
}
X509_EXTENSION* OpenSSLLib::SSL_X509V3_EXT_conf_nid(lhash_st_CONF_VALUE* conf,
                                                    X509V3_CTX* ctx,
                                                    int ext_nid,
                                                    char* value) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509V3_EXT_conf_nid(
            conf, ctx, ext_nid, value);
}
int OpenSSLLib::SSL_X509_add_ext(X509* x, X509_EXTENSION* ex, int loc) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_add_ext(x, ex, loc);
}
void OpenSSLLib::SSL_X509_EXTENSION_free(X509_EXTENSION* a) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509_EXTENSION_free(a);
}
void OpenSSLLib::SSL_X509V3_set_ctx_nodb(X509V3_CTX* ctx) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509V3_set_ctx_nodb(ctx);
}
void OpenSSLLib::SSL_X509V3_set_ctx(X509V3_CTX* ctx,
                                    X509* issuer,
                                    X509* subject,
                                    X509_REQ* req,
                                    X509_CRL* crl,
                                    int flags) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509V3_set_ctx(
            ctx, issuer, subject, req, crl, flags);
}
int OpenSSLLib::SSL_X509_set_serialNumber(X509* x, ASN1_INTEGER* serial) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_set_serialNumber(x, serial);
}
ASN1_INTEGER* OpenSSLLib::SSL_X509_get_serialNumber(X509* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_get_serialNumber(x);
}
int OpenSSLLib::SSL_ASN1_INTEGER_set(ASN1_INTEGER* a, long value) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_INTEGER_set(a, value);
}
long OpenSSLLib::SSL_ASN1_INTEGER_get(const ASN1_INTEGER* a) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_INTEGER_get(a);
}
int OpenSSLLib::SSL_ASN1_INTEGER_cmp(const ASN1_INTEGER* x, const ASN1_INTEGER* y) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_INTEGER_cmp(x, y);
}
BIGNUM* OpenSSLLib::SSL_ASN1_INTEGER_to_BN(const ASN1_INTEGER* ai, BIGNUM* bn) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_INTEGER_to_BN(ai, bn);
}
void OpenSSLLib::SSL_BN_free(BIGNUM* a) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_BN_free(a);
}
char* OpenSSLLib::SSL_BN_bn2dec(const BIGNUM* a) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BN_bn2dec(a);
}
void* OpenSSLLib::SSL_OPENSSL_malloc(int num) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_OPENSSL_malloc(num);
}
void OpenSSLLib::SSL_OPENSSL_free(void* addr) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_OPENSSL_free(addr);
}
void OpenSSLLib::SSL_ASN1_INTEGER_free(ASN1_INTEGER* a) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_ASN1_INTEGER_free(a);
}
ASN1_INTEGER* OpenSSLLib::SSL_ASN1_INTEGER_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_INTEGER_new();
}
int OpenSSLLib::SSL_BN_bn2bin(const BIGNUM* a, unsigned char* to) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BN_bn2bin(a, to);
}
int OpenSSLLib::SSL_BN_num_bytes(const BIGNUM* a) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BN_num_bytes(a);
}
int OpenSSLLib::SSL_ASN1_TIME_set_string(ASN1_TIME* s, const char* str) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_TIME_set_string(s, str);
}
ASN1_TIME* OpenSSLLib::SSL_ASN1_TIME_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_TIME_new();
}
ASN1_STRING* OpenSSLLib::SSL_ASN1_STRING_dup(const ASN1_STRING* str) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_STRING_dup(str);
}
X509_NAME* OpenSSLLib::SSL_X509_CRL_get_issuer(const X509_CRL* crl) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_CRL_get_issuer(crl);
}
int OpenSSLLib::SSL_X509_CRL_verify(X509_CRL* a, EVP_PKEY* r) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_CRL_verify(a, r);
}
const ASN1_TIME* OpenSSLLib::SSL_X509_CRL_get_nextUpdate(const X509_CRL* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_CRL_get_nextUpdate(x);
}
const ASN1_TIME* OpenSSLLib::SSL_X509_CRL_get_lastUpdate(const X509_CRL* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_CRL_get_lastUpdate(x);
}
X509_CRL* OpenSSLLib::SSL_PEM_read_bio_X509_CRL(BIO* bp,
                                                X509_CRL** x,
                                                pem_password_cb* cb,
                                                void* u) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_read_bio_X509_CRL(bp, x, cb, u);
}
int OpenSSLLib::SSL_PEM_write_bio_X509_CRL(BIO* bp, X509_CRL* x) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PEM_write_bio_X509_CRL(bp, x);
}
X509_CRL* OpenSSLLib::SSL_d2i_X509_CRL_bio(BIO* bp, X509_CRL** crl) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_d2i_X509_CRL_bio(bp, crl);
}
void OpenSSLLib::SSL_X509_CRL_free(X509_CRL* a) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509_CRL_free(a);
}
X509_CRL* OpenSSLLib::SSL_X509_CRL_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_X509_CRL_new();
}
void OpenSSLLib::SSL_X509_STORE_CTX_set0_crls(X509_STORE_CTX* ctx,
                                              STACK_OF(X509_CRL) * crls) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_CTX_set0_crls(ctx, crls);
}
void OpenSSLLib::SSL_sk_X509_CRL_free(STACK_OF(X509_CRL) * stack) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_sk_X509_CRL_free(stack);
}
STACK_OF(X509_CRL) * OpenSSLLib::SSL_sk_X509_CRL_new_null() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_sk_X509_CRL_new_null();
}
int OpenSSLLib::SSL_sk_X509_CRL_push(STACK_OF(X509_CRL) * stack, const X509_CRL* crl) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_sk_X509_CRL_push(stack, crl);
}
ASN1_TIME* OpenSSLLib::SSL_ASN1_TIME_adj(ASN1_TIME* s,
                                         time_t t,
                                         int offset_day,
                                         long offset_sec) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ASN1_TIME_adj(
            s, t, offset_day, offset_sec);
}
void OpenSSLLib::SSL_X509_STORE_CTX_set_time(X509_STORE_CTX* ctx,
                                             unsigned long flags,
                                             time_t t) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_X509_STORE_CTX_set_time(ctx, flags, t);
}
void OpenSSLLib::SSL_EVP_MD_CTX_init(EVP_MD_CTX* ctx) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_EVP_MD_CTX_init(ctx);
}
int OpenSSLLib::SSL_EVP_DigestInit_ex(EVP_MD_CTX* ctx, const EVP_MD* type, ENGINE* impl) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_DigestInit_ex(ctx, type, impl);
}
int OpenSSLLib::SSL_EVP_DigestUpdate(EVP_MD_CTX* ctx, const void* d, size_t cnt) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_DigestUpdate(ctx, d, cnt);
}
int OpenSSLLib::SSL_EVP_DigestFinal_ex(EVP_MD_CTX* ctx, unsigned char* md, unsigned int* s) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_DigestFinal_ex(ctx, md, s);
}
int OpenSSLLib::SSL_EVP_MD_CTX_reset(EVP_MD_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_MD_CTX_reset(ctx);
}
int OpenSSLLib::SSL_EVP_PKEY_sign(EVP_PKEY_CTX* ctx,
                                  unsigned char* sig,
                                  size_t* siglen,
                                  const unsigned char* tbs,
                                  size_t tbslen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_sign(
            ctx, sig, siglen, tbs, tbslen);
}
int OpenSSLLib::SSL_EVP_PKEY_sign_init(EVP_PKEY_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_sign_init(ctx);
}
int OpenSSLLib::SSL_EVP_PKEY_verify_init(EVP_PKEY_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_verify_init(ctx);
}
int OpenSSLLib::SSL_EVP_PKEY_verify(EVP_PKEY_CTX* ctx,
                                    const unsigned char* sig,
                                    size_t siglen,
                                    const unsigned char* tbs,
                                    size_t tbslen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_verify(
            ctx, sig, siglen, tbs, tbslen);
}
int OpenSSLLib::SSL_EVP_PKEY_CTX_set_rsa_padding(EVP_PKEY_CTX* ctx, int pad) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_set_rsa_padding(ctx, pad);
}
int OpenSSLLib::SSL_EVP_PKEY_CTX_set_signature_md(EVP_PKEY_CTX* ctx, const EVP_MD* md) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_set_signature_md(ctx, md);
}
int OpenSSLLib::SSL_EVP_PKEY_CTX_set_rsa_pss_saltlen(EVP_PKEY_CTX* ctx, int len) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_set_rsa_pss_saltlen(ctx, len);
}
int OpenSSLLib::SSL_EVP_PKEY_CTX_set_rsa_mgf1_md(EVP_PKEY_CTX* ctx, const EVP_MD* md) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_set_rsa_mgf1_md(ctx, md);
}
int OpenSSLLib::SSL_EVP_PKEY_encrypt_init(EVP_PKEY_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_encrypt_init(ctx);
}
int OpenSSLLib::SSL_EVP_PKEY_encrypt(EVP_PKEY_CTX* ctx,
                                     unsigned char* out,
                                     size_t* outlen,
                                     const unsigned char* in,
                                     size_t inlen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_encrypt(
            ctx, out, outlen, in, inlen);
}
int OpenSSLLib::SSL_EVP_PKEY_decrypt_init(EVP_PKEY_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_decrypt_init(ctx);
}
int OpenSSLLib::SSL_EVP_PKEY_decrypt(EVP_PKEY_CTX* ctx,
                                     unsigned char* out,
                                     size_t* outlen,
                                     const unsigned char* in,
                                     size_t inlen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_decrypt(
            ctx, out, outlen, in, inlen);
}
int OpenSSLLib::SSL_EVP_PKEY_CTX_set_rsa_oaep_md(EVP_PKEY_CTX* ctx, const EVP_MD* md) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_set_rsa_oaep_md(ctx, md);
}
int OpenSSLLib::SSL_EVP_PKEY_CTX_set_rsa_oaep_label(EVP_PKEY_CTX* ctx,
                                                    unsigned char* l,
                                                    int llen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_CTX_set_rsa_oaep_label(
            ctx, l, llen);
}
int OpenSSLLib::SSL_RSA_size(const RSA* r) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_RSA_size(r);
}
int OpenSSLLib::SSL_EVP_MD_size(const EVP_MD* md) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_MD_size(md);
}
EC_KEY* OpenSSLLib::SSL_EVP_PKEY_get0_EC_KEY(EVP_PKEY* pkey) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_get0_EC_KEY(pkey);
}
int OpenSSLLib::SSL_EVP_DigestSign(EVP_MD_CTX* ctx,
                                   unsigned char* sigret,
                                   size_t* siglen,
                                   const unsigned char* tbs,
                                   size_t tbslen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_DigestSign(
            ctx, sigret, siglen, tbs, tbslen);
}
int OpenSSLLib::SSL_EVP_DigestVerifyInit(EVP_MD_CTX* ctx,
                                         EVP_PKEY_CTX** pctx,
                                         const EVP_MD* type,
                                         ENGINE* e,
                                         EVP_PKEY* pkey) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_DigestVerifyInit(
            ctx, pctx, type, e, pkey);
}
int OpenSSLLib::SSL_EVP_DigestVerify(EVP_MD_CTX* ctx,
                                     const unsigned char* sigret,
                                     size_t siglen,
                                     const unsigned char* tbs,
                                     size_t tbslen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_DigestVerify(
            ctx, sigret, siglen, tbs, tbslen);
}
X509_REQ* OpenSSLLib::SSL_d2i_X509_REQ_bio(BIO* bp, X509_REQ** req) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_d2i_X509_REQ_bio(bp, req);
}
int OpenSSLLib::SSL_i2d_X509_REQ_bio(BIO* bp, X509_REQ* req) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_i2d_X509_REQ_bio(bp, req);
}
int OpenSSLLib::SSL_EVP_CipherUpdate(EVP_CIPHER_CTX* ctx,
                                     unsigned char* out,
                                     int* outl,
                                     const unsigned char* in,
                                     int inl) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CipherUpdate(ctx, out, outl, in, inl);
}
int OpenSSLLib::SSL_EVP_CipherFinal_ex(EVP_CIPHER_CTX* ctx, unsigned char* outm, int* outl) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CipherFinal_ex(ctx, outm, outl);
}
int OpenSSLLib::SSL_EVP_CipherInit_ex(EVP_CIPHER_CTX* ctx,
                                      const EVP_CIPHER* cipher,
                                      ENGINE* impl,
                                      const unsigned char* key,
                                      const unsigned char* iv,
                                      int enc) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CipherInit_ex(
            ctx, cipher, impl, key, iv, enc);
}
EVP_CIPHER_CTX* OpenSSLLib::SSL_EVP_CIPHER_CTX_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CIPHER_CTX_new();
}
void OpenSSLLib::SSL_EVP_CIPHER_CTX_free(EVP_CIPHER_CTX* c) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_EVP_CIPHER_CTX_free(c);
}
int OpenSSLLib::SSL_EVP_CIPHER_CTX_ctrl(EVP_CIPHER_CTX* ctx, int type, int arg, void* ptr) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CIPHER_CTX_ctrl(ctx, type, arg, ptr);
}
int OpenSSLLib::SSL_EVP_CIPHER_CTX_key_length(const EVP_CIPHER_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CIPHER_CTX_key_length(ctx);
}
int OpenSSLLib::SSL_EVP_CIPHER_CTX_iv_length(const EVP_CIPHER_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CIPHER_CTX_iv_length(ctx);
}
int OpenSSLLib::SSL_RAND_bytes(unsigned char* buf, int num) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_RAND_bytes(buf, num);
}
int OpenSSLLib::SSL_EVP_CIPHER_CTX_reset(EVP_CIPHER_CTX* c) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CIPHER_CTX_reset(c);
}
int OpenSSLLib::SSL_EVP_CIPHER_CTX_set_padding(EVP_CIPHER_CTX* c, int pad) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CIPHER_CTX_set_padding(c, pad);
}

int OpenSSLLib::SSL_PKCS5_PBKDF2_HMAC(const char* pass,
                                      int passlen,
                                      const unsigned char* salt,
                                      int saltlen,
                                      int iter,
                                      const EVP_MD* digest,
                                      int keylen,
                                      unsigned char* out) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_PKCS5_PBKDF2_HMAC(
            pass, passlen, salt, saltlen, iter, digest, keylen, out);
}

int OpenSSLLib::SSL_ECDH_KDF_X9_63(unsigned char* out,
                                   size_t outlen,
                                   const unsigned char* Z,
                                   size_t Zlen,
                                   const unsigned char* sinfo,
                                   size_t sinfolen,
                                   const EVP_MD* md) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ECDH_KDF_X9_63(
            out, outlen, Z, Zlen, sinfo, sinfolen, md);
}
int OpenSSLLib::SSL_HMAC_Init_ex(
        HMAC_CTX* ctx, const void* key, int key_len, const EVP_MD* md, ENGINE* impl) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_HMAC_Init_ex(ctx, key, key_len, md, impl);
}
int OpenSSLLib::SSL_HMAC_Update(HMAC_CTX* ctx, const unsigned char* data, int len) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_HMAC_Update(ctx, data, len);
}
int OpenSSLLib::SSL_HMAC_Final(HMAC_CTX* ctx, unsigned char* md, unsigned int* len) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_HMAC_Final(ctx, md, len);
}
HMAC_CTX* OpenSSLLib::SSL_HMAC_CTX_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_HMAC_CTX_new();
}
void OpenSSLLib::SSL_HMAC_CTX_free(HMAC_CTX* ctx) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_HMAC_CTX_free(ctx);
}
int OpenSSLLib::SSL_EC_KEY_oct2key(EC_KEY* eckey, const unsigned char* buf, size_t len) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EC_KEY_oct2key(eckey, buf, len);
}
EC_KEY* OpenSSLLib::SSL_EC_KEY_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EC_KEY_new();
}
void OpenSSLLib::SSL_EC_KEY_free(EC_KEY* key) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_EC_KEY_free(key);
}
EC_KEY* OpenSSLLib::SSL_EC_KEY_new_by_curve_name(int nid) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EC_KEY_new_by_curve_name(nid);
}
int OpenSSLLib::SSL_EVP_PKEY_set1_EC_KEY(EVP_PKEY* pkey, EC_KEY* key) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_set1_EC_KEY(pkey, key);
}
size_t OpenSSLLib::SSL_EC_KEY_key2buf(const EC_KEY* eckey,
                                      point_conversion_form_t form,
                                      unsigned char** pbuf,
                                      BN_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EC_KEY_key2buf(eckey, form, pbuf, ctx);
}
int OpenSSLLib::SSL_EVP_PKEY_derive_init(EVP_PKEY_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_derive_init(ctx);
}
int OpenSSLLib::SSL_EVP_PKEY_derive_set_peer(EVP_PKEY_CTX* ctx, EVP_PKEY* peer) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_derive_set_peer(ctx, peer);
}
int OpenSSLLib::SSL_EVP_PKEY_derive(EVP_PKEY_CTX* ctx, unsigned char* key, size_t* keylen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_PKEY_derive(ctx, key, keylen);
}
BIGNUM* OpenSSLLib::SSL_BN_bin2bn(const unsigned char* s, int len, BIGNUM* ret) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BN_bin2bn(s, len, ret);
}
ECDSA_SIG* OpenSSLLib::SSL_ECDSA_SIG_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ECDSA_SIG_new();
}
void OpenSSLLib::SSL_ECDSA_SIG_free(ECDSA_SIG* sig) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_ECDSA_SIG_free(sig);
}
int OpenSSLLib::SSL_ECDSA_SIG_set0(ECDSA_SIG* sig, BIGNUM* r, BIGNUM* s) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ECDSA_SIG_set0(sig, r, s);
}
int OpenSSLLib::SSL_i2d_ECDSA_SIG(const ECDSA_SIG* sig, unsigned char** pp) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_i2d_ECDSA_SIG(sig, pp);
}
ECDSA_SIG* OpenSSLLib::SSL_d2i_ECDSA_SIG(ECDSA_SIG** sig,
                                         const unsigned char** pp,
                                         long len) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_d2i_ECDSA_SIG(sig, pp, len);
}
const BIGNUM* OpenSSLLib::SSL_ECDSA_SIG_get0_r(const ECDSA_SIG* sig) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ECDSA_SIG_get0_r(sig);
}
const BIGNUM* OpenSSLLib::SSL_ECDSA_SIG_get0_s(const ECDSA_SIG* sig) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ECDSA_SIG_get0_s(sig);
}
int OpenSSLLib::SSL_BN_bn2binpad(const BIGNUM* a, unsigned char* to, int tolen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_BN_bn2binpad(a, to, tolen);
}

/* CMAC */
CMAC_CTX* OpenSSLLib::SSL_CMAC_CTX_new() noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_CMAC_CTX_new();
}
void OpenSSLLib::SSL_CMAC_CTX_cleanup(CMAC_CTX* ctx) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_CMAC_CTX_cleanup(ctx);
}
void OpenSSLLib::SSL_CMAC_CTX_free(CMAC_CTX* ctx) noexcept
{
    OpenSSLLibMockManager::getMockInterface().SSL_CMAC_CTX_free(ctx);
}
EVP_CIPHER_CTX* OpenSSLLib::SSL_CMAC_CTX_get0_cipher_ctx(CMAC_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_CMAC_CTX_get0_cipher_ctx(ctx);
}
int OpenSSLLib::SSL_CMAC_CTX_copy(CMAC_CTX* out, const CMAC_CTX* in) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_CMAC_CTX_copy(out, in);
}
int OpenSSLLib::SSL_CMAC_Init(CMAC_CTX* ctx,
                              const void* key,
                              size_t keylen,
                              const EVP_CIPHER* cipher,
                              ENGINE* impl) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_CMAC_Init(ctx, key, keylen, cipher, impl);
}
int OpenSSLLib::SSL_CMAC_Update(CMAC_CTX* ctx, const void* data, size_t dlen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_CMAC_Update(ctx, data, dlen);
}
int OpenSSLLib::SSL_CMAC_Final(CMAC_CTX* ctx, unsigned char* out, size_t* poutlen) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_CMAC_Final(ctx, out, poutlen);
}
int OpenSSLLib::SSL_CMAC_resume(CMAC_CTX* ctx) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_CMAC_resume(ctx);
}

int OpenSSLLib::SSL_EVP_CIPHER_key_length(const EVP_CIPHER* cipher) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CIPHER_key_length(cipher);
}
const char* OpenSSLLib::SSL_EVP_CIPHER_name(const EVP_CIPHER* cipher) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_EVP_CIPHER_name(cipher);
}
EVP_PKEY* OpenSSLLib::SSL_ENGINE_load_private_key(ENGINE* e,
                                                  const char* key_id,
                                                  UI_METHOD* ui_method,
                                                  void* callback_data) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ENGINE_load_private_key(
            e, key_id, ui_method, callback_data);
}
EVP_PKEY* OpenSSLLib::SSL_ENGINE_load_public_key(ENGINE* e,
                                                 const char* key_id,
                                                 UI_METHOD* ui_method,
                                                 void* callback_data) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ENGINE_load_public_key(
            e, key_id, ui_method, callback_data);
}
int OpenSSLLib::SSL_ENGINE_ctrl_cmd_string(ENGINE* e,
                                           const char* cmd_name,
                                           const char* cmd_arg,
                                           int cmd_optional) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ENGINE_ctrl_cmd_string(
            e, cmd_name, cmd_arg, cmd_optional);
}
int OpenSSLLib::SSL_ENGINE_init(ENGINE* e) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ENGINE_init(e);
}
ENGINE* OpenSSLLib::SSL_ENGINE_by_id(const char* id) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ENGINE_by_id(id);
}
int OpenSSLLib::SSL_ENGINE_finish(ENGINE* e) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ENGINE_finish(e);
}
int OpenSSLLib::SSL_ENGINE_free(ENGINE* e) noexcept
{
    return OpenSSLLibMockManager::getMockInterface().SSL_ENGINE_free(e);
}
}  // namespace lib
}  // namespace openssl
}  // namespace mococrw
