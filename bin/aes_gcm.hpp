#ifndef AES_GCM_HPP
#define AES_GCM_HPP

#include <string>

void AES_GCM_ENCRYPT(const std::string& passphrase, FILE* input,
		FILE* output);

void AES_GCM_DECRYPT(const std::string& passphrase, FILE* input,
		FILE* output);

#endif // AES_GCM_HPP
