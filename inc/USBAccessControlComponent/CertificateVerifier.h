#ifndef CERTIFICATE_VERIFIER_H
#define CERTIFICATE_VERIFIER_H

int certgen_verify_cert_with_ca(const char *cert_path, const char *ca_cert_path);

#endif // CERTIFICATE_VERIFIER_H