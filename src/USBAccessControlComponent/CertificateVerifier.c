#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/x509_vfy.h>
#include <stdio.h>

int certgen_verify_cert_with_ca(const char *cert_path, const char *ca_cert_path) {
    FILE *fp_cert = fopen(cert_path, "r");
    FILE *fp_ca   = fopen(ca_cert_path, "r");
    if (!fp_cert || !fp_ca) {
        perror("File open failed");
        return 0;
    }

    X509 *cert = PEM_read_X509(fp_cert, NULL, NULL, NULL);
    X509 *ca_cert = PEM_read_X509(fp_ca, NULL, NULL, NULL);
    fclose(fp_cert);
    fclose(fp_ca);
    if (!cert || !ca_cert) {
        fprintf(stderr, "Failed to load certs\n");
        return 0;
    }

    X509_STORE *store = X509_STORE_new();
    X509_STORE_add_cert(store, ca_cert);

    X509_STORE_CTX *ctx = X509_STORE_CTX_new();
    X509_STORE_CTX_init(ctx, store, cert, NULL);

    int ret = X509_verify_cert(ctx);

    if (ret == 1) {
        printf("✅ Certificate is valid and signed by CA.\n");
    } else {
        printf("❌ Verification failed: %s\n",
               X509_verify_cert_error_string(X509_STORE_CTX_get_error(ctx)));
    }

    X509_free(cert);
    X509_free(ca_cert);
    X509_STORE_free(store);
    X509_STORE_CTX_free(ctx);

    return ret;
}