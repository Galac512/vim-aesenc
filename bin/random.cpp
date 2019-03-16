#include "random.hpp"

#include <crypto++/osrng.h>

int utf8size(const char *str)
{
    int utf8charcount = 0;
    int size = strlen(str);

    for (int i = 0; i < size; utf8charcount++ )
    {
        if ( ~(str[i]) & 0x80 ) 
        {
            i+=1;
        } 
        else if ( ~(str[i] & 0xE0) & 0x20 )
        {
            if ( ~(str[i + 1] & 0xB0) & 0x40 )
                i+=2;
            else
                break; /* bad character */
        }
        else if ( ~(str[i] & 0xF0) & 0x10 )
        {
            if ( i + 2 < size )
            {
                if ( ( ~(str[i + 1] & 0xB0) & 0x40 ) && ( ~(str[i + 2] & 0xB0) & 0x40 ) )
                    i+=3;
                else
                    break; /* bad char */
            }
            else
            {
                break; /* past \x0 */
            }
        }
        else if ( ~(str[i] & 0xF8) & 0x08 )
        {
            if ( i + 3 < size )
            {
                if ( ( ~(str[i + 1] & 0xB0) & 0x40 ) && ( ~(str[i + 2] & 0xB0) & 0x40 ) && ( ~(str[i + 3] & 0xB0) & 0x40 ))
                    i+=4;
                else
                    break; /* bad char */
            }
            else
            {
                break; /* past \x0 */
            }
        }
    }

    return utf8charcount;
}

char *utf8get(const char *str, const int& index)
{
    int utf8charcount=0;
    int size = strlen(str);
    int i = 0;

    for (; utf8charcount < index; utf8charcount++)
    {
        if ( ~(str[i]) & 0x80 ) 
        {
            i+=1;
        }
        else if ( ~(str[i] & 0xE0) & 0x20 )
        {
            if ( ~(str[i + 1] & 0xB0) & 0x40 )
                i+=2;
            else
                break; /* bad character */
        }
        else if ( ~(str[i] & 0xF0) & 0x10 )
        {
            if ( i + 2 < size )
            {
                if ( ( ~(str[i + 1] & 0xB0) & 0x40 ) && ( ~(str[i + 2] & 0xB0) & 0x40 ) )
                    i+=3;
                else
                    break; /* bad char */
            }
            else
            {
                break; /* past \x0 */
            }
        }
        else if ( ~(str[i] & 0xF8) & 0x08 )
        {
            if ( i + 3 < size )
            {
                if ( ( ~(str[i + 1] & 0xB0) & 0x40 ) && ( ~(str[i + 2] & 0xB0) & 0x40 ) && ( ~(str[i + 3] & 0xB0) & 0x40 ) )
                    i+=4;
                else
                    break; /* bad char */
            }
            else
            {
                break; /* past \x0 */
            }
        }
    }

    if ( ~(str[i]) & 0x80 ) 
    {
        char * ret = (char*)malloc(2);
        ret[0] = str[i];
        ret[1] = 0x0; //Null terminator
        return ret;
    }
    else if ( ~(str[i] & 0xE0) & 0x20 )
    {
        if ( ~(str[i + 1] & 0xB0) & 0x40 )
        {
            char * ret = (char*)malloc(3);
            ret[0] = str[i];
            ret[1] = str[i+1];
            ret[2] = 0x0; //Null terminator
            return ret;
        }
        else {  }
    }
    else if ( ~(str[i] & 0xF0) & 0x10 )
    {
        if ( i + 2 < size )
        {
            if ( ( ~(str[i + 1] & 0xB0) & 0x40 ) && ( ~(str[i + 2] & 0xB0) & 0x40 ) )
            {
                char * ret = (char*)malloc(4);
                ret[0] = str[i];
                ret[1] = str[i+1];
                ret[2] = str[i+2];
                ret[3] = 0x0; //Null terminator
                return ret;
            }
            else { }
        }
        else { }
    }
    else if ( ~(str[i] & 0xF8) & 0x08 )
    {
        if ( i + 3 < size )
        {
            if ( ( ~(str[i + 1] & 0xB0) & 0x40 ) && ( ~(str[i + 2] & 0xB0) & 0x40 ) && ( ~(str[i + 3] & 0xB0) & 0x40 ))
            {
                char * ret = (char*)malloc(5);
                ret[0] = str[i];
                ret[1] = str[i+1];
                ret[2] = str[i+2];
                ret[3] = str[i+3];
                ret[4] = 0x0; //Null terminator
                return ret;
            }
            else { }
        }
        else { }
    }

    return (char*)"0";
}

std::string GENERATE_PASSWORD(const int& length, const std::string& charset)
{
	CryptoPP::AutoSeededRandomPool prng(false, 32);

	std::string password = "";

	int max = utf8size(charset.c_str())-1;
	for (int i = 0; i < length; i++)
		password += utf8get(charset.c_str(), prng.GenerateWord32(0, max));

	return password;
}
