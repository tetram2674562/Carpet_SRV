//
// Created by tetram26 on 20/11/2025.
//

#ifndef CARPET_SRV_UTF16STRING_H
#define CARPET_SRV_UTF16STRING_H
#include <iosfwd>

namespace utils {
class UTF16String {
	private:
		unsigned short* characters;
		int length;
	public:
		UTF16String();
        UTF16String(const std::string&);
        UTF16String(const unsigned short*, int length);

		UTF16String(const UTF16String&);
        UTF16String& operator=(const UTF16String&);
        ~UTF16String();

		int size() const;
		UTF16String& operator+=(const UTF16String&);
		UTF16String operator+(const UTF16String&);
        unsigned short operator[] (unsigned int);
        const unsigned short operator[] (unsigned int) const;

		std::string toString() const;
		std::ostream& display(std::ostream&) const;
		void append(const UTF16String&);
        void append(int nbr);

		utils::UTF16String operator+(const UTF16String &string) const;
};

std::ostream& operator << (std::ostream&, const UTF16String&);
UTF16String operator+(const UTF16String&, const UTF16String&);
}
#endif //CARPET_SRV_UTF16STRING_H