Summary
=======

- Use `vim file.aesf` to create a new AES protected file. It will ask for a passphrase and ask to confirm. In this file you can do:

	- `w` to save and encrypt the file
	- `q` to delete the temporary file

- Use `:Random` to create a random string (useful for passwords).

Installation
============

To install vim-aesenc, you need to compile an AES encryption tool wrote in C++ (you need `crypto++`). Go into the `./bin/` folder and run `$ ./build.sh`.
