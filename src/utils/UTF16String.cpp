//
// Created by tetram26 on 20/11/2025.
//

#include "utils/UTF16String.h"
#include <iostream>
#include <ostream>
#include <string>
#include <utils/ConsoleUtils.h>
namespace utils {
UTF16String::UTF16String() : characters(nullptr), length(0) {}

UTF16String::UTF16String(const std::string &str)
    : characters(nullptr), length((int)str.size()) {
  this->characters = new unsigned short[this->length];
  for (unsigned int i = 0; i < this->length; i++) {
    this->characters[i] = (unsigned char)str[i];
  }
}

UTF16String::UTF16String(const unsigned short *utf16String, int length)
    : characters(nullptr), length(length) {
  this->characters = new unsigned short[this->length];
  for (unsigned int i = 0; i < this->length; i++) {
    this->characters[i] = utf16String[i];
  }
}

UTF16String::UTF16String(const UTF16String &str)
    : characters(nullptr), length(str.size()) {
  this->characters = new unsigned short[this->length];
  for (unsigned int i = 0; i < this->length; i++) {
    this->characters[i] = str[i];
  }
}

UTF16String::~UTF16String() {
  if (this->characters != nullptr) {
    delete[] this->characters;
    this->characters = nullptr;
  }
}

int UTF16String::size() const { return length; }

UTF16String &UTF16String::operator+=(const UTF16String &string) {
  this->append(string);
  return (*this);
}

void UTF16String::append(const UTF16String &string) {
  if (string.length != 0) {
    unsigned short *new_string =
        new unsigned short[this->length + string.length];
    for (unsigned int i = 0; i < this->length; i++) {
      new_string[i] = this->characters[i];
    }
    delete[] this->characters;
    for (unsigned int i = 0; i < string.length; i++) {
      new_string[i + this->length] = string.characters[i];
    }
    this->characters = new_string;
    this->length = this->length + string.length;
  }
}

void UTF16String::append(int nbr) {
  unsigned short *new_string = new unsigned short[this->length + 1];
  if (this->characters != nullptr) {
    for (unsigned int i = 0; i < this->length; i++) {
      new_string[i] = this->characters[i];
    }
    delete[] this->characters;
  }
  new_string[this->length] = nbr;

  this->characters = new_string;
  this->length++;
}

UTF16String UTF16String::operator+(const UTF16String &string) const {
  UTF16String new_string(*this);
  new_string += string;
  return new_string;
}

unsigned short UTF16String::operator[](unsigned int index) {
  return this->characters[index];
}

const unsigned short UTF16String::operator[](unsigned int index) const {
  return this->characters[index];
}

std::string UTF16String::toString() const {
  std::string result;
  for (unsigned int i = 0; i < this->length; i++) {
    result += static_cast<char>(this->characters[i]);
  }
  return result;
}

std::ostream &UTF16String::display(std::ostream &outputStream) const {
  outputStream << toString();
  return outputStream;
}

std::ostream &operator<<(std::ostream &outputStream,
                         const UTF16String &string) {
  string.display(outputStream);
  return outputStream;
}

UTF16String operator+(const UTF16String &utf16_string,
                      const UTF16String &utf16_string1) {
  UTF16String utf16_string2(utf16_string);
  return utf16_string2 += utf16_string1;
}

UTF16String &UTF16String::operator=(const UTF16String &string) {
  if (this->length != 0) {
    delete[] this->characters;
    this->characters = nullptr;
  }
  this->length = string.length;
  if (string.length != 0 && string.characters != nullptr) {
    this->characters = new unsigned short[string.length];
    for (unsigned int i = 0; i < this->length; i++) {
      this->characters[i] = string.characters[i];
    }
  }
  return *this;
}
} // namespace utils