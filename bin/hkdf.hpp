#ifndef HKDF_HPP
#define HKDF_HPP

#include <string>

#ifndef byte
	#define byte uint8_t
#endif

byte* HKDF(const std::string& text, const std::string& salt, const std::string& info);

#endif // HKDF_HPP
