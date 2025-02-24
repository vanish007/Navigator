#pragma once
#include <cinttypes>
#include <cstring>
#include <iostream>


struct uint239_t {
    uint8_t data[35];
};


enum bits{
    BitsInByte = 8,
    TotalBytes = 35,
    TotalBits = 245,
    MaxValueInByte = 128,
};


static_assert(sizeof(uint239_t) == 35, "Size of uint239_t must be no higher than 35 bytes");

uint239_t FromInt(uint32_t value, uint32_t shift);

uint239_t FromString(const char* str, uint32_t shift);

uint239_t operator+(const uint239_t& lhs, const uint239_t& rhs);

uint239_t operator-(const uint239_t& lhs, const uint239_t& rhs);

uint239_t operator*(const uint239_t& lhs, const uint239_t& rhs);

uint239_t operator/(const uint239_t& lhs, const uint239_t& rhs);

bool operator==(const uint239_t& lhs, const uint239_t& rhs);

bool operator!=(const uint239_t& lhs, const uint239_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint239_t& value);

uint64_t GetShift(const uint239_t& value);

void PlaceBit(uint239_t& value, const int bit);

void PrintBinaryF(uint239_t value, int size = 35);

void PrintBinaryS(uint239_t value, int size = 35);

void PrintBinaryN(uint239_t value, int size = 35);

int StringToInt10(const char* str);

void StringToInt128(uint239_t& value, int shift, const char* str, int idx, 
                    const int i, const int j, int second = 0);

uint239_t ShiftToZero(const uint239_t& value);

uint239_t MakeShift(uint239_t& val, uint64_t shift);

uint239_t operator%(const uint239_t& lhs, const uint239_t& rhs);

void Multiply2(uint239_t& value);

void Divide2(uint239_t& value);

bool operator>(const uint239_t& lhs, const uint239_t& rhs);

bool operator<(const uint239_t& lhs, const uint239_t& rhs);

bool operator>=(const uint239_t& lhs, const uint239_t& rhs);