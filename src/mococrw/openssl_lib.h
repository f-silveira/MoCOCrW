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
#pragma once

extern "C" {
#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/cmac.h>
#include <openssl/conf.h>
#include <openssl/crypto.h>
#include <openssl/dsa.h>
#include <openssl/ec.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
}

namespace mococrw
{
namespace openssl
{
namespace lib
{
/**
 * Thin wrapper around the "naked" openssl library.
 *
 * Prefix all method names with 'SSL_' to distinguish them from their
 * openssl counterparts.
 *
 * Note: If you have the idea to put include openssl \#includes into
 * a dedicated namespace to enforce proper name-resolution: Nice try,
 * but macros make that idea impossible.
 *
 */
class OpenSSLLib
{
public:
    static int SSL_ENGINE_free(ENGINE* e) noexcept;
    static int SSL_ENGINE_finish(ENGINE* e) noexcept;
    static ENGINE* SSL_ENGINE_by_id(const char* id) noexcept;
    static int SSL_ENGINE_init(ENGINE* e) noexcept;
    static int SSL_ENGINE_ctrl_cmd_string(ENGINE* e,
                                          const char* cmd_name,
                                          const char* cmd_arg,
                                          int cmd_optional) noexcept;
    static EVP_PKEY* SSL_ENGINE_load_public_key(ENGINE* e,
                                                const char* key_id,
                                                UI_METHOD* ui_method,
                                                void* callback_data) noexcept;
    static EVP_PKEY* SSL_ENGINE_load_private_key(ENGINE* e,
                                                 const char* key_id,
                                                 UI_METHOD* ui_method,
                                                 void* callback_data) noexcept;
    static ECDSA_SIG* SSL_d2i_ECDSA_SIG(ECDSA_SIG** sig,
                                        const unsigned char** pp,
                                        long len) noexcept;
    static int SSL_i2d_ECDSA_SIG(const ECDSA_SIG* sig, unsigned char** pp) noexcept;
    static int SSL_ECDSA_SIG_set0(ECDSA_SIG* sig, BIGNUM* r, BIGNUM* s) noexcept;
    static const BIGNUM* SSL_ECDSA_SIG_get0_s(const ECDSA_SIG* sig) noexcept;
    static const BIGNUM* SSL_ECDSA_SIG_get0_r(const ECDSA_SIG* sig) noexcept;
    static void SSL_ECDSA_SIG_free(ECDSA_SIG* sig) noexcept;
    static ECDSA_SIG* SSL_ECDSA_SIG_new() noexcept;
    static BIGNUM* SSL_BN_bin2bn(const unsigned char* s, int len, BIGNUM* ret) noexcept;
    static int SSL_BN_bn2binpad(const BIGNUM* a, unsigned char* to, int tolen) noexcept;
    static int SSL_i2d_X509_REQ_bio(BIO* bp, X509_REQ* req) noexcept;
    static X509_REQ* SSL_d2i_X509_REQ_bio(BIO* bp, X509_REQ** req) noexcept;
    static const char* SSL_EVP_CIPHER_name(const EVP_CIPHER* cipher) noexcept;
    static int SSL_EVP_CIPHER_key_length(const EVP_CIPHER* cipher) noexcept;
    static int SSL_EVP_CIPHER_CTX_set_padding(EVP_CIPHER_CTX* c, int pad) noexcept;
    static int SSL_EVP_CIPHER_CTX_reset(EVP_CIPHER_CTX* c) noexcept;
    static int SSL_RAND_bytes(unsigned char* buf, int num) noexcept;
    static int SSL_EVP_CIPHER_CTX_iv_length(const EVP_CIPHER_CTX* ctx) noexcept;
    static int SSL_EVP_CIPHER_CTX_key_length(const EVP_CIPHER_CTX* ctx) noexcept;
    static int SSL_EVP_CIPHER_CTX_ctrl(EVP_CIPHER_CTX* ctx, int type, int arg, void* ptr) noexcept;
    static void SSL_EVP_CIPHER_CTX_free(EVP_CIPHER_CTX* c) noexcept;
    static EVP_CIPHER_CTX* SSL_EVP_CIPHER_CTX_new() noexcept;
    static int SSL_EVP_CipherInit_ex(EVP_CIPHER_CTX* ctx,
                                     const EVP_CIPHER* cipher,
                                     ENGINE* impl,
                                     const unsigned char* key,
                                     const unsigned char* iv,
                                     int enc) noexcept;
    static int SSL_EVP_CipherFinal_ex(EVP_CIPHER_CTX* ctx, unsigned char* outm, int* outl) noexcept;
    static int SSL_EVP_CipherUpdate(EVP_CIPHER_CTX* ctx,
                                    unsigned char* out,
                                    int* outl,
                                    const unsigned char* in,
                                    int inl) noexcept;
    static int SSL_EVP_MD_CTX_reset(EVP_MD_CTX* ctx) noexcept;
    static int SSL_EVP_DigestFinal_ex(EVP_MD_CTX* ctx, unsigned char* md, unsigned int* s) noexcept;
    static int SSL_EVP_DigestUpdate(EVP_MD_CTX* ctx, const void* d, size_t cnt) noexcept;
    static int SSL_EVP_DigestInit_ex(EVP_MD_CTX* ctx, const EVP_MD* type, ENGINE* impl) noexcept;
    static void SSL_EVP_MD_CTX_init(EVP_MD_CTX* ctx) noexcept;
    static void SSL_X509_STORE_CTX_set_time(X509_STORE_CTX* ctx,
                                            unsigned long flags,
                                            time_t t) noexcept;
    static ASN1_TIME* SSL_ASN1_TIME_adj(ASN1_TIME* s,
                                        time_t t,
                                        int offset_day,
                                        long offset_sec) noexcept;
    static int SSL_sk_X509_CRL_push(STACK_OF(X509_CRL) * stack, const X509_CRL* crl) noexcept;
    static STACK_OF(X509_CRL) * SSL_sk_X509_CRL_new_null() noexcept;
    static void SSL_sk_X509_CRL_free(STACK_OF(X509_CRL) * stack) noexcept;
    static void SSL_X509_STORE_CTX_set0_crls(X509_STORE_CTX* ctx,
                                             STACK_OF(X509_CRL) * crls) noexcept;
    static X509_CRL* SSL_X509_CRL_new() noexcept;
    static void SSL_X509_CRL_free(X509_CRL* a) noexcept;
    static X509_CRL* SSL_d2i_X509_CRL_bio(BIO* bp, X509_CRL** crl) noexcept;
    static int SSL_PEM_write_bio_X509_CRL(BIO* bp, X509_CRL* x) noexcept;
    static X509_CRL* SSL_PEM_read_bio_X509_CRL(BIO* bp,
                                               X509_CRL** x,
                                               pem_password_cb* cb,
                                               void* u) noexcept;
    static const ASN1_TIME* SSL_X509_CRL_get_lastUpdate(const X509_CRL* x) noexcept;
    static const ASN1_TIME* SSL_X509_CRL_get_nextUpdate(const X509_CRL* x) noexcept;
    static int SSL_X509_CRL_verify(X509_CRL* a, EVP_PKEY* r) noexcept;
    static X509_NAME* SSL_X509_CRL_get_issuer(const X509_CRL* crl) noexcept;
    static ASN1_STRING* SSL_ASN1_STRING_dup(const ASN1_STRING* str) noexcept;
    static ASN1_TIME* SSL_ASN1_TIME_new() noexcept;
    static int SSL_ASN1_TIME_set_string(ASN1_TIME* s, const char* str) noexcept;
    static int SSL_BN_num_bytes(const BIGNUM* a) noexcept;
    static int SSL_BN_bn2bin(const BIGNUM* a, unsigned char* to) noexcept;
    static ASN1_INTEGER* SSL_ASN1_INTEGER_new() noexcept;
    static void SSL_ASN1_INTEGER_free(ASN1_INTEGER* a) noexcept;
    static void* SSL_OPENSSL_malloc(int num) noexcept;
    static void SSL_OPENSSL_free(void* addr) noexcept;
    static char* SSL_BN_bn2dec(const BIGNUM* a) noexcept;
    static void SSL_BN_free(BIGNUM* a) noexcept;
    static BIGNUM* SSL_ASN1_INTEGER_to_BN(const ASN1_INTEGER* ai, BIGNUM* bn) noexcept;
    static int SSL_ASN1_INTEGER_cmp(const ASN1_INTEGER* x, const ASN1_INTEGER* y) noexcept;
    static long SSL_ASN1_INTEGER_get(const ASN1_INTEGER* a) noexcept;
    static int SSL_ASN1_INTEGER_set(ASN1_INTEGER* a, long value) noexcept;
    static ASN1_INTEGER* SSL_X509_get_serialNumber(X509* x) noexcept;
    static int SSL_X509_set_serialNumber(X509* x, ASN1_INTEGER* serial) noexcept;
    static void SSL_X509V3_set_ctx(X509V3_CTX* ctx,
                                   X509* issuer,
                                   X509* subject,
                                   X509_REQ* req,
                                   X509_CRL* crl,
                                   int flags) noexcept;
    static void SSL_X509V3_set_ctx_nodb(X509V3_CTX* ctx) noexcept;
    static void SSL_X509_EXTENSION_free(X509_EXTENSION* a) noexcept;
    static int SSL_X509_add_ext(X509* x, X509_EXTENSION* ex, int loc) noexcept;
    static X509_EXTENSION* SSL_X509V3_EXT_conf_nid(lhash_st_CONF_VALUE* conf,
                                                   X509V3_CTX* ctx,
                                                   int ext_nid,
                                                   char* value) noexcept;

    static const EVP_CIPHER* SSL_EVP_aes_128_cbc() noexcept;
    static const EVP_CIPHER* SSL_EVP_aes_256_cbc() noexcept;

    static int SSL_BIO_write(BIO* b, const void* buf, int len) noexcept;
    static int SSL_BIO_read(BIO* b, void* buf, int len) noexcept;
    static BIO* SSL_BIO_new_file(const char* filename, const char* mode) noexcept;

    /* Initialization */
    static void SSL_ERR_load_crypto_strings() noexcept;
    static void SSL_SSL_load_error_strings() noexcept;
    static void SSL_OpenSSL_add_all_algorithms() noexcept;
    static void SSL_CRYPTO_malloc_init() noexcept;

    /* Key Generation */
    static EVP_PKEY* SSL_EVP_PKEY_new() noexcept;
    static void SSL_EVP_PKEY_free(EVP_PKEY* ptr) noexcept;
    static int SSL_EVP_PKEY_keygen(EVP_PKEY_CTX* ctx, EVP_PKEY** ppkey) noexcept;
    static int SSL_EVP_PKEY_keygen_init(EVP_PKEY_CTX* ctx) noexcept;
    static EVP_PKEY_CTX* SSL_EVP_PKEY_CTX_new(EVP_PKEY* pkey, ENGINE* engine) noexcept;
    static EVP_PKEY_CTX* SSL_EVP_PKEY_CTX_new_id(int id, ENGINE* engine) noexcept;
    static void SSL_EVP_PKEY_CTX_free(EVP_PKEY_CTX* ptr) noexcept;
    static int SSL_EVP_PKEY_CTX_set_rsa_keygen_bits(EVP_PKEY_CTX* ctx, int mbits) noexcept;
    static int SSL_EVP_PKEY_cmp(const EVP_PKEY* a, const EVP_PKEY* b) noexcept;
    static int SSL_EVP_PKEY_paramgen_init(EVP_PKEY_CTX* ctx) noexcept;
    static int SSL_EVP_PKEY_paramgen(EVP_PKEY_CTX* ctx, EVP_PKEY** ppkey) noexcept;
    static int SSL_EVP_PKEY_CTX_set_ec_paramgen_curve_nid(EVP_PKEY_CTX* ctx, int nid) noexcept;
    static int SSL_EVP_PKEY_CTX_set_ec_param_enc(EVP_PKEY_CTX* ctx, int param_enc) noexcept;
    static const EC_GROUP* SSL_EC_KEY_get0_group(const EC_KEY* key) noexcept;
    static int SSL_EC_GROUP_get_curve_name(const EC_GROUP* group) noexcept;
    static int SSL_EC_GROUP_get_degree(const EC_GROUP* group) noexcept;
    static int SSL_EVP_PKEY_type(int type) noexcept;
    static int SSL_EVP_PKEY_id(const EVP_PKEY* pkey) noexcept;
    static int SSL_EVP_PKEY_size(EVP_PKEY* pkey) noexcept;

    /* Error handling */
    static char* SSL_ERR_error_string(unsigned long error, char* buf) noexcept;
    static unsigned long SSL_ERR_get_error() noexcept;

    /* BIO Stuff */
    static const BIO_METHOD* SSL_BIO_s_mem() noexcept;
    static void SSL_BIO_free_all(BIO* ptr) noexcept;
    static BIO* SSL_BIO_new(const BIO_METHOD* method) noexcept;
    static int SSL_BIO_gets(BIO* bio, char* buf, int size) noexcept;
    static int SSL_BIO_puts(BIO* bio, char* buf) noexcept;
    static int SSL_PEM_write_bio_X509_REQ(BIO* bio, X509_REQ* req) noexcept;
    static X509_REQ* SSL_PEM_read_bio_X509_REQ(BIO* bp,
                                               X509_REQ** x,
                                               pem_password_cb* cb,
                                               void* u) noexcept;

    static int SSL_PEM_write_bio_PKCS8PrivateKey(BIO* bp,
                                                 EVP_PKEY* x,
                                                 const EVP_CIPHER* enc,
                                                 char* kstr,
                                                 int klen,
                                                 pem_password_cb* cb,
                                                 void* u) noexcept;
    static int SSL_PEM_write_bio_PUBKEY(BIO* bp, EVP_PKEY* x) noexcept;

    static EVP_PKEY* SSL_PEM_read_bio_PUBKEY(BIO* bio,
                                             EVP_PKEY** pkey,
                                             pem_password_cb* cb,
                                             void* u) noexcept;

    static EVP_PKEY* SSL_PEM_read_bio_PrivateKey(BIO* bio,
                                                 EVP_PKEY** pkey,
                                                 pem_password_cb* cb,
                                                 void* u) noexcept;
    static X509* SSL_PEM_read_bio_X509(BIO* bio, X509** x, pem_password_cb*, void* pwd) noexcept;
    static int SSL_PEM_write_bio_X509(BIO* bp, X509* x) noexcept;
    static X509* SSL_d2i_X509_bio(BIO* bp, X509** x509) noexcept;
    static int SSL_i2d_X509_bio(BIO* bp, X509* x) noexcept;

    /* X509 */
    static X509* SSL_X509_new() noexcept;
    static int SSL_X509_set_pubkey(X509* ptr, EVP_PKEY* pkey) noexcept;
    static int SSL_X509_set_issuer_name(X509* x, X509_NAME* name) noexcept;
    static int SSL_X509_set_subject_name(X509* x, X509_NAME* name) noexcept;
    static int SSL_X509_set_notBefore(X509* x, const ASN1_TIME* t) noexcept;
    static int SSL_X509_set_notAfter(X509* x, const ASN1_TIME* t) noexcept;
    static int SSL_X509_sign(X509* x, EVP_PKEY* pkey, const EVP_MD* md) noexcept;
    static void SSL_X509_free(X509* ptr) noexcept;
    static X509_NAME* SSL_X509_get_subject_name(X509* ptr) noexcept;
    static X509_NAME* SSL_X509_get_issuer_name(X509* ptr) noexcept;
    static EVP_PKEY* SSL_X509_get_pubkey(X509* x) noexcept;
    static ASN1_TIME* SSL_X509_get_notBefore(X509* x) noexcept;
    static ASN1_TIME* SSL_X509_get_notAfter(X509* x) noexcept;

    /* ASN1_TIME */
    static void SSL_ASN1_TIME_free(ASN1_TIME* x) noexcept;
    static int SSL_ASN1_TIME_diff(int* pday,
                                  int* psec,
                                  const ASN1_TIME* from,
                                  const ASN1_TIME* to) noexcept;
    static ASN1_TIME* SSL_ASN1_TIME_set(ASN1_TIME* s, time_t t) noexcept;

    /* X509_REQ */
    static int SSL_X509_REQ_sign_ctx(X509_REQ* req, EVP_MD_CTX* ctx) noexcept;
    static int SSL_X509_REQ_set_pubkey(X509_REQ* req, EVP_PKEY* pkey) noexcept;
    static int SSL_X509_REQ_set_version(X509_REQ* req, unsigned long version) noexcept;
    static int SSL_X509_REQ_set_subject_name(X509_REQ* req, X509_NAME* name) noexcept;
    static void SSL_X509_REQ_free(X509_REQ* ptr) noexcept;
    static X509_REQ* SSL_X509_REQ_new() noexcept;
    static X509_NAME* SSL_X509_REQ_get_subject_name(const X509_REQ* req) noexcept;
    static EVP_PKEY* SSL_X509_REQ_get_pubkey(X509_REQ* req) noexcept;
    static int SSL_X509_REQ_verify(X509_REQ* a, EVP_PKEY* r) noexcept;
    static const EVP_MD* SSL_EVP_sha1() noexcept;
    static const EVP_MD* SSL_EVP_sha256() noexcept;
    static const EVP_MD* SSL_EVP_sha384() noexcept;
    static const EVP_MD* SSL_EVP_sha512() noexcept;
    static const EVP_MD* SSL_EVP_sha3_256() noexcept;
    static const EVP_MD* SSL_EVP_sha3_384() noexcept;
    static const EVP_MD* SSL_EVP_sha3_512() noexcept;

    /* X509_NAME */
    static X509_NAME* SSL_X509_NAME_new() noexcept;
    static void SSL_X509_NAME_free(X509_NAME*) noexcept;
    static int SSL_X509_NAME_add_entry_by_NID(X509_NAME* name,
                                              int nid,
                                              int type,
                                              unsigned char* bytes,
                                              int len,
                                              int loc,
                                              int set) noexcept;
    static int SSL_X509_NAME_get_index_by_NID(X509_NAME* name, int nid, int lastpos) noexcept;
    static X509_NAME_ENTRY* SSL_X509_NAME_get_entry(X509_NAME* name, int loc) noexcept;

    /* X509_NAME_ENTRY */
    static ASN1_STRING* SSL_X509_NAME_ENTRY_get_data(X509_NAME_ENTRY* ne) noexcept;

    /* ASN1_STRING */
    /**
     * Returns number of characters written or -1 on error.
     *
     */
    static int SSL_ASN1_STRING_print_ex(BIO* out, ASN1_STRING* str, unsigned long flags) noexcept;

    /* X509 Certificate validation */
    static X509_STORE* SSL_X509_STORE_new() noexcept;
    static void SSL_X509_STORE_free(X509_STORE* v) noexcept;
    static int SSL_X509_STORE_add_cert(X509_STORE* ctx, X509* x) noexcept;

    static X509_STORE_CTX* SSL_X509_STORE_CTX_new() noexcept;
    static int SSL_X509_STORE_CTX_init(X509_STORE_CTX* ctx,
                                       X509_STORE* store,
                                       X509* x509,
                                       STACK_OF(X509) * chain) noexcept;
    static void SSL_X509_STORE_CTX_free(X509_STORE_CTX* ctx) noexcept;

    static X509_VERIFY_PARAM* SSL_X509_STORE_CTX_get0_param(X509_STORE_CTX* ctx) noexcept;
    static int SSL_X509_VERIFY_PARAM_set_flags(X509_VERIFY_PARAM* param,
                                               unsigned long flags) noexcept;

    static int SSL_X509_verify_cert(X509_STORE_CTX* ctx) noexcept;
    static const char* SSL_X509_verify_cert_error_string(long n) noexcept;
    static int SSL_X509_STORE_CTX_get_error(X509_STORE_CTX* ctx) noexcept;

    static int SSL_X509_check_ca(X509* cert) noexcept;

    /* stack of X509 */
    static STACK_OF(X509) * SSL_sk_X509_new_null() noexcept;
    static int SSL_sk_X509_push(STACK_OF(X509) * stack, const X509* crt) noexcept;
    static void SSL_sk_X509_free(STACK_OF(X509) * stack) noexcept;

    /* EVP_MD */
    static EVP_MD_CTX* SSL_EVP_MD_CTX_create() noexcept;
    static void SSL_EVP_MD_CTX_destroy(EVP_MD_CTX* ptr) noexcept;

    /* Signatures */
    static int SSL_EVP_PKEY_sign(EVP_PKEY_CTX* ctx,
                                 unsigned char* sig,
                                 size_t* siglen,
                                 const unsigned char* tbs,
                                 size_t tbslen) noexcept;
    static int SSL_EVP_PKEY_sign_init(EVP_PKEY_CTX* ctx) noexcept;
    static int SSL_EVP_PKEY_verify_init(EVP_PKEY_CTX* ctx) noexcept;
    static int SSL_EVP_PKEY_verify(EVP_PKEY_CTX* ctx,
                                   const unsigned char* sig,
                                   size_t siglen,
                                   const unsigned char* tbs,
                                   size_t tbslen) noexcept;
    static int SSL_EVP_PKEY_CTX_set_signature_md(EVP_PKEY_CTX* ctx, const EVP_MD* md) noexcept;
    static int SSL_EVP_PKEY_CTX_set_rsa_pss_saltlen(EVP_PKEY_CTX* ctx, int len) noexcept;
    static EC_KEY* SSL_EVP_PKEY_get0_EC_KEY(EVP_PKEY* pkey) noexcept;
    static int SSL_EVP_DigestSignInit(EVP_MD_CTX* ctx,
                                      EVP_PKEY_CTX** pctx,
                                      const EVP_MD* type,
                                      ENGINE* e,
                                      EVP_PKEY* pkey) noexcept;
    static int SSL_EVP_DigestSign(EVP_MD_CTX* ctx,
                                  unsigned char* sigret,
                                  size_t* siglen,
                                  const unsigned char* tbs,
                                  size_t tbslen) noexcept;
    static int SSL_EVP_DigestVerify(EVP_MD_CTX* ctx,
                                    const unsigned char* sigret,
                                    size_t siglen,
                                    const unsigned char* tbs,
                                    size_t tbslen) noexcept;
    static int SSL_EVP_DigestVerifyInit(EVP_MD_CTX* ctx,
                                        EVP_PKEY_CTX** pctx,
                                        const EVP_MD* type,
                                        ENGINE* e,
                                        EVP_PKEY* pkey) noexcept;

    /* Encryption */
    static int SSL_EVP_PKEY_encrypt_init(EVP_PKEY_CTX* ctx) noexcept;
    static int SSL_EVP_PKEY_encrypt(EVP_PKEY_CTX* ctx,
                                    unsigned char* out,
                                    size_t* outlen,
                                    const unsigned char* in,
                                    size_t inlen) noexcept;
    static int SSL_EVP_PKEY_decrypt_init(EVP_PKEY_CTX* ctx) noexcept;
    static int SSL_EVP_PKEY_decrypt(EVP_PKEY_CTX* ctx,
                                    unsigned char* out,
                                    size_t* outlen,
                                    const unsigned char* in,
                                    size_t inlen) noexcept;
    static int SSL_EVP_PKEY_CTX_set_rsa_oaep_md(EVP_PKEY_CTX* ctx, const EVP_MD* md) noexcept;
    static int SSL_EVP_PKEY_CTX_set_rsa_oaep_label(EVP_PKEY_CTX* ctx,
                                                   unsigned char* l,
                                                   int llen) noexcept;
    static int SSL_RSA_size(const RSA* r) noexcept;
    static int SSL_EVP_MD_size(const EVP_MD* md) noexcept;
    static int SSL_EVP_PKEY_CTX_set_rsa_mgf1_md(EVP_PKEY_CTX* ctx, const EVP_MD* md) noexcept;
    static int SSL_EVP_PKEY_CTX_set_rsa_padding(EVP_PKEY_CTX* ctx, int pad) noexcept;

    /* KDF */
    static int SSL_PKCS5_PBKDF2_HMAC(const char* pass,
                                     int passlen,
                                     const unsigned char* salt,
                                     int saltlen,
                                     int iter,
                                     const EVP_MD* digest,
                                     int keylen,
                                     unsigned char* out) noexcept;

    static int SSL_ECDH_KDF_X9_63(unsigned char* out,
                                  size_t outlen,
                                  const unsigned char* Z,
                                  size_t Zlen,
                                  const unsigned char* sinfo,
                                  size_t sinfolen,
                                  const EVP_MD* md) noexcept;

    /* HMAC */
    static void SSL_HMAC_CTX_free(HMAC_CTX* ctx) noexcept;
    static HMAC_CTX* SSL_HMAC_CTX_new() noexcept;
    static int SSL_HMAC_Final(HMAC_CTX* ctx, unsigned char* md, unsigned int* len) noexcept;
    static int SSL_HMAC_Update(HMAC_CTX* ctx, const unsigned char* data, int len) noexcept;
    static int SSL_HMAC_Init_ex(
            HMAC_CTX* ctx, const void* key, int key_len, const EVP_MD* md, ENGINE* impl) noexcept;

    /* CMAC */
    static CMAC_CTX* SSL_CMAC_CTX_new() noexcept;
    static void SSL_CMAC_CTX_cleanup(CMAC_CTX* ctx) noexcept;
    static void SSL_CMAC_CTX_free(CMAC_CTX* ctx) noexcept;
    static EVP_CIPHER_CTX* SSL_CMAC_CTX_get0_cipher_ctx(CMAC_CTX* ctx) noexcept;
    static int SSL_CMAC_CTX_copy(CMAC_CTX* out, const CMAC_CTX* in) noexcept;
    static int SSL_CMAC_Init(CMAC_CTX* ctx,
                             const void* key,
                             size_t keylen,
                             const EVP_CIPHER* cipher,
                             ENGINE* impl) noexcept;
    static int SSL_CMAC_Update(CMAC_CTX* ctx, const void* data, size_t dlen) noexcept;
    static int SSL_CMAC_Final(CMAC_CTX* ctx, unsigned char* out, size_t* poutlen) noexcept;
    static int SSL_CMAC_resume(CMAC_CTX* ctx) noexcept;

    /* EC Point import and export */
    static size_t SSL_EC_KEY_key2buf(const EC_KEY* eckey,
                                     point_conversion_form_t form,
                                     unsigned char** pbuf,
                                     BN_CTX* ctx) noexcept;
    static int SSL_EVP_PKEY_set1_EC_KEY(EVP_PKEY* pkey, EC_KEY* key) noexcept;
    static EC_KEY* SSL_EC_KEY_new_by_curve_name(int nid) noexcept;
    static void SSL_EC_KEY_free(EC_KEY* key) noexcept;
    static EC_KEY* SSL_EC_KEY_new() noexcept;
    static int SSL_EC_KEY_oct2key(EC_KEY* eckey, const unsigned char* buf, size_t len) noexcept;

    /* ECC: compute on elliptic curves */
    static int SSL_EVP_PKEY_derive(EVP_PKEY_CTX* ctx, unsigned char* key, size_t* keylen) noexcept;
    static int SSL_EVP_PKEY_derive_set_peer(EVP_PKEY_CTX* ctx, EVP_PKEY* peer) noexcept;
    static int SSL_EVP_PKEY_derive_init(EVP_PKEY_CTX* ctx) noexcept;
};
}  // namespace lib
}  // namespace openssl
}  // namespace mococrw
