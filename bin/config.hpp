#ifndef CONFIG_HPP
#define CONFIG_HPP

//*Edit these to change encryption
//The salt of the HKDF function
#define AESFILES_HKDF_SALT "salt"
//The info of the HKDF function
#define AESFILES_HKDF_INFO "info"
//Aes block size in byte: 32 -> AES-256-GCM. Valid values are 16, 24 & 32.
#define AESFILES_AES_BLOCKSIZE 32

#define AESFILES_VERSION "0.1"
#define AESFILES_LICENSE "GNU GPlv2"
#define AESFILES_CREATOR "Galac <glac@airmail.cc>"

#endif // CONFIG_HPP
