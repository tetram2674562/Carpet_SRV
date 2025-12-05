//
// Created by tetram26 on 31/07/25.
//


#ifndef BUFFER_H
#define BUFFER_H
#include <string>
#include <vector>

#include "utils/UTF16String.h"

namespace packet {
    class Buffer {
        public:

            void clearBuffer();
            std::vector<unsigned char>& getDataBuffer();

            unsigned char readByte();
            void writeByte(unsigned char);

            short readShort();
            void writeShort(short);

            char readChar();
            void writeChar(char);

            std::string readString(int);
            void writeString(const std::string &);

            double readDouble();
            void writeDouble(double);


            void writeUTF16Char(unsigned short);
            void writeUTF16String(const utils::UTF16String&);

            std::vector<unsigned char> readBytes();
            void writeBytes(const std::vector<unsigned char> &);

            void writeInt(int);

            Buffer(std::vector<unsigned char> &);
            Buffer();
            int readInt();

            bool readBool();


        private:
            std::vector<unsigned char> data;
            int index;
    };
}
#endif //BUFFER_H