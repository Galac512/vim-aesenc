#include "aes_gcm.hpp"

#include "hkdf.hpp"
#include "config.hpp"

#include <crypto++/gcm.h>
#include <crypto++/osrng.h>


void AES_GCM_ENCRYPT(const std::string& passphrase, FILE* input,
		FILE* output)
{
	//* Read the file
	fseek(input, 0L, SEEK_END);
	size_t fsize = ftell(input);
	rewind(input);
	byte *raw = new byte[fsize];
	fread(&raw[0], fsize, 1, input);

	//* Get the passphrase's hkdf
	byte *key = HKDF(passphrase, AESFILES_HKDF_SALT, AESFILES_HKDF_INFO);

	//* Random IV
	CryptoPP::SecByteBlock IV(AESFILES_AES_BLOCKSIZE);
	CryptoPP::AutoSeededRandomPool prng(false, 32);
	prng.GenerateBlock(IV, IV.size());

	//* AES GCM
	//- Set key
	CryptoPP::GCM< CryptoPP::AES >::Encryption enc;
	enc.SetKeyWithIV
	(
		key, AESFILES_AES_BLOCKSIZE,
		IV, IV.size()
	);

	//- Output
	std::string outStr;

	//- Encrypt
	CryptoPP::StringSource
	(
		&raw[0],
		fsize,
		true,
		new CryptoPP::AuthenticatedEncryptionFilter
		(
			enc,
			new CryptoPP::StringSink(outStr)
		)
	);

	//* Write output
	//- Write IV
	fwrite(&IV[0], IV.size(), sizeof(IV[0]), output);
	//- Write AES ciphered text
	fwrite(&outStr[0], outStr.size(), sizeof(outStr[0]), output);

	delete[] key;
}

void AES_GCM_DECRYPT(const std::string& passphrase, FILE* input,
		FILE* output)
{
	try
	{
		//* Read the file
		fseek(input, 0L, SEEK_END);
		size_t fsize = ftell(input);
		rewind(input);
		byte *raw = new byte[fsize];
		fread(&raw[0], fsize, 1, input);

		//* Get the passphrase's hkdf
		byte *key = HKDF(passphrase, AESFILES_HKDF_SALT, AESFILES_HKDF_INFO);

		//* Get IV
		CryptoPP::SecByteBlock IV(AESFILES_AES_BLOCKSIZE);
		memcpy(&IV[0], &raw[0], AESFILES_AES_BLOCKSIZE*sizeof(IV[0]));

		//* AES GCM
		try
		{
			//- Set key
			CryptoPP::GCM< CryptoPP::AES >::Decryption dec;
			dec.SetKeyWithIV
			(
				key, AESFILES_AES_BLOCKSIZE,
				IV, IV.size()
			);

			//- Output
			std::string outStr;

			//- Decrypt
			CryptoPP::StringSource
			(
				&raw[AESFILES_AES_BLOCKSIZE],
				fsize-AESFILES_AES_BLOCKSIZE,
				true,
				new CryptoPP::AuthenticatedDecryptionFilter
				(
					dec,
					new CryptoPP::StringSink(outStr)
				)
			);
			delete[] raw;
			delete[] key;

			//* Write output
			fwrite(&outStr[0], outStr.size(), sizeof(outStr[0]), output);
		}
		catch (CryptoPP::Exception& e)
		{
			delete[] raw;
			delete[] key;
			fprintf(stderr, "AES_GCM_DECRYPT(): Error while decrypting the text: '%s'", e.what());
			exit(1);
		}
	}
	catch (std::length_error& e)
	{
		fprintf(stderr, "AES_GCM_DECRYPT(): std::lenght_error: '%s', are you sure the input file is an encrypted file?", e.what());
		exit(1);
	}
}
