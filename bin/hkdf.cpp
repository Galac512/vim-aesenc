#include "hkdf.hpp"

#include "config.hpp"

#include <crypto++/hkdf.h>
#include <crypto++/sha.h>

byte* HKDF(const std::string& __text, const std::string& __salt, const std::string& __info)
{
	//Text
	size_t tlen = __text.size();
	byte text[tlen];
	for (size_t i(0); i <= tlen; i++)
		text[i] = static_cast<byte>(__text[i]); //text contains a nullTerm


	//Salt
	size_t slen = __salt.size();
	byte salt[slen];
	for (size_t i(0); i <= slen; i++)
		salt[i] = static_cast<byte>(__salt[i]); //salt contains a nullTerm


	//Info
	size_t ilen = __info.size();
	byte info[ilen];
	for (size_t i(0); i <= ilen; i++)
		info[i] = static_cast<byte>(__info[i]); //info contains a nullTerm


	//Store result
	byte *key = new byte[AESFILES_AES_BLOCKSIZE];

	//Hkdf
	CryptoPP::HKDF< CryptoPP::SHA1 > hkdf;
	hkdf.DeriveKey
	(
		key, AESFILES_AES_BLOCKSIZE,
		text, tlen,
		salt, slen,
		info, ilen
	);

	return key;
}
