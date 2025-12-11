//
// Created by tetram26 on 20/11/2025.
//

#include "utils/UTF16String.h"

#include <utils/ConsoleUtils.h>
#include <string>

utils::UTF16String::UTF16String() : characters(NULL), length(0) {}

utils::UTF16String::UTF16String(const std::string &str) : characters(NULL), length((int)str.size()) {
    this->characters = new unsigned short[this->length];
    for (unsigned int i = 0; i < this->length; i++) {
        this->characters[i] = (unsigned char)str[i];
    }
}

utils::UTF16String::UTF16String(const unsigned short * utf16String, int length): characters(NULL), length(length) {
    this->characters = new unsigned short[this->length];
    for (unsigned int i = 0; i < this->length; i++) {
        this->characters[i] = utf16String[i];
    }
}

utils::UTF16String::UTF16String(const UTF16String & str) : characters(NULL), length(str.size()) {
    this->characters = new unsigned short[this->length];
    for (unsigned int i = 0; i < this->length; i++) {
        this->characters[i] = str[i];
    }
}

utils::UTF16String::~UTF16String() {
    if (this->characters != NULL) {
        delete [] this->characters;
        this->characters = NULL;
    }
}

int utils::UTF16String::size() const {
    return length;
}

utils::UTF16String & utils::UTF16String::operator+=(const UTF16String& string) {
    this->append(string);
    return (*this);
}

void utils::UTF16String::append(const utils::UTF16String& string) {
    if (string.length != 0) {
        unsigned short * new_string = new unsigned short[this->length + string.length];
        for (unsigned int i = 0; i < this->length; i++) {
            new_string[i] = this->characters[i];
        }
        delete [] this->characters;
        for (unsigned int i = 0; i < string.length; i++) {
            new_string[i + this->length] = string.characters[i]; 
        }  
        this->characters = new_string;
        this->length = this->length + string.length;
    }
}

void utils::UTF16String::append(int nbr) {
    unsigned short * new_string = new unsigned short[this->length + 1];
    if (this->characters != NULL) {
        for (unsigned int i = 0; i < this->length; i++) {
            new_string[i] = this->characters[i];
        }
        delete [] this->characters;
    }
    new_string[this->length] = nbr;

    this->characters = new_string;
    this->length++;
}

utils::UTF16String utils::UTF16String::operator+(const UTF16String &string) const {
    UTF16String new_string(*this);
    new_string += string;
    return new_string;
}

unsigned short utils::UTF16String::operator[](unsigned int index) {
    return this->characters[index];
}

const unsigned short utils::UTF16String::operator[](unsigned int index) const {
    return this->characters[index];
}

std::string utils::UTF16String::toString() const {
    std::string result;
    for (unsigned int i = 0; i < this->length; i++) {
        result += static_cast<char>(this->characters[i]);
    }
    return result;
}

std::ostream & utils::UTF16String::display(std::ostream & outputStream) const {
    outputStream << this->toString();
    return outputStream;
}

std::ostream & utils::operator<<(std::ostream & outputStream, const UTF16String & string) {
    string.display(outputStream);
    return outputStream;
}

utils::UTF16String utils::operator+(const UTF16String &utf16_string, const UTF16String &utf16_string1) {
    UTF16String utf16_string2(utf16_string);
    return utf16_string2 += utf16_string1;
}

utils::UTF16String& utils::UTF16String::operator=(const UTF16String& string) {
    if (this->length != 0) {
        delete [] this->characters;
        this->characters = NULL;
    }
    this->length = string.length;
    if (string.length != 0 && string.characters != NULL) {
        this->characters = new unsigned short[string.length];
        for (unsigned int i = 0; i < this->length; i++) {
            this->characters[i] = string.characters[i];
        } 
    }
    return *this;
}
