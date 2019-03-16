#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "hkdf.hpp"
#include "aes_gcm.hpp"
#include "random.hpp"

#include "config.hpp"

#ifndef byte
	#define byte uint8_t
#endif

#define CMP(a, b) (strcmp(a, b) == 0)

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("USAGE: %s [OPTIONS...] FILE\nTry '%s --help' for help.", argv[0], argv[0]);
		exit(2);
	}

	std::string passphrase;
	bool passphraseDefined = false;

	FILE* File = NULL;
	bool fileDefined = false;

	std::string charset;
	int length;

	for (int i = 1; i < argc; i++)
	{
		//{{{ Help
		if (CMP(argv[i], "-h") || CMP(argv[i], "--help"))
		{
			printf("USAGE: %s [OPTIONS...] FILE\n\n"
"Options\n"
"  -p, --passphrase PASSPHRASE\tSet the PASSPHRASE to encrypt or decrypt\n"
"  -f, --file FILE\t\tSet the FILE to encrypt\n"
"  --only-hkdf\t\t\tJust encrypt PASSPHRASE to stdout and exits\n"
"  -d, --decrypt\t\t\tDecrypt FILE with PASSPHRASE to stdout\n"
"  -e, --encrypt\t\t\tEncrypt FILE with PASSPHRASE to stdout\n"
"  -r, --random CHARSET LENGTH\tGenerate a random string made out of CHARSET, of size LENGTH\n\n"
"Other\n"
"  -v, --version\t\t\tShow informations about AESfiles\n", argv[0]);
			if (File != NULL) fclose(File);
			exit(0);
		}
		//}}}

		//{{{ Passphrase
		else if (CMP(argv[i], "-p") || CMP(argv[i], "--passphrase"))
		{
			if (argc > i+1)
			{
				i++;
				passphrase = argv[i];
				passphraseDefined = true;
			}
			else
			{
				fprintf(stderr, "You must add the PASSPHRASE string after the option.\n");
				if (File != NULL) fclose(File);
				exit(2);
			}
		}
		//}}}

		//{{{ File
		else if (CMP(argv[i], "-f") || CMP(argv[i], "--file"))
		{
			if (argc > i+1)
			{
				i++;
				File = fopen(argv[i], "r+");
				if (File == NULL)
				{
					fprintf(stderr, "Could not open file: '%s'.\n", argv[i]);
					exit(1);
				}
				fileDefined = true;
			}
			else
			{
				fprintf(stderr, "You must add the FILE string after the option.\n");
				if (File != NULL) fclose(File); //In case the user calls this function two times
				exit(2);
			}
		}
		//}}}

		//{{{ HKDF output
		else if (CMP(argv[i], "--only-hkdf"))
		{
			if (passphraseDefined)
			{
				byte *key = HKDF(passphrase, AESFILES_HKDF_SALT, AESFILES_HKDF_INFO);
				fwrite(&key[0], AESFILES_AES_BLOCKSIZE, sizeof(key[0]), stdout);
				delete[] key;
				if (File != NULL) fclose(File);
				exit(0);
			}
			else
			{
				fprintf(stderr, "You need to define the PASSPHRASE first.\n");
				if (File != NULL) fclose(File);
				exit(2);
			}
		}
		//}}}

		//{{{ Decrypt file
		else if (CMP(argv[i], "-d") || CMP(argv[i], "--decrypt"))
		{
			if (passphraseDefined)
			{
				if (fileDefined)
				{
					AES_GCM_DECRYPT(passphrase, File, stdout);
					if (File != NULL) fclose(File);
					exit(0);
				}
				else
				{
					fprintf(stderr, "You need to define the FILE first.\n");
					if (File != NULL) fclose(File);
					exit(2);
				}
			}
			else
			{
				fprintf(stderr, "You need to define the PASSPHRASE first.\n");
				if (File != NULL) fclose(File);
				exit(2);
			}
		}
		//}}}

		//{{{ Encrypt file
		else if (CMP(argv[i], "-e") || CMP(argv[i], "--encrypt"))
		{
			if (passphraseDefined)
			{
				if (fileDefined)
				{
					AES_GCM_ENCRYPT(passphrase, File, stdout);
					if (File != NULL) fclose(File);
					exit(0);
				}
				else
				{
					fprintf(stderr, "You need to define the FILE first.\n");
					if (File != NULL) fclose(File);
					exit(2);
				}
			}
			else
			{
				fprintf(stderr, "You need to define the PASSPHRASE first.\n");
				if (File != NULL) fclose(File);
				exit(2);
			}
		}
		//}}}

		//{{{ Random
		else if (CMP(argv[i], "-r") || CMP(argv[i], "--random"))
		{
	
			if (argc > i+2)
			{
				charset = argv[i+1];
				length = atoi(argv[i+2]);
				i += 2;

				printf("%s", GENERATE_PASSWORD(length, charset).c_str());
			}
			else
			{
				fprintf(stderr, "You need to define the PASSPHRASE first.\n");
				if (File != NULL) fclose(File);
				exit(2);
			}
		}
		//}}}

		//{{{ Version
		else if (CMP(argv[i], "-v") || CMP(argv[i], "--version"))
		{
			printf("AESfiles version %s\nLicense: %s\nAuthor: %s\n", AESFILES_VERSION, AESFILES_LICENSE, AESFILES_CREATOR);
			if (File != NULL) fclose(File);
			exit(0);
		}
		//}}}

	//{{{Unknown option
	else
	{
		fprintf(stderr, "Unknown option: '%s'\n", argv[i]);
		if (File != NULL) fclose(File);
		exit(2);
	}
	//}}}
	}
	
	return 0;
}
