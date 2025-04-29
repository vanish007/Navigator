#include "number.hpp"


const int kBitsInByte = bits(BitsInByte);
const int kTotalBytes = bits(TotalBytes);
const int KTotalBits = bits(TotalBits);
const int kMaxValueInByte = bits(MaxValueInByte);
const long long kTwoToThePowerOf35 = 34359738368LL;


uint239_t FromInt(uint32_t input_value, uint32_t shift) {
    uint239_t result = {};

    int value_size = 0;
    uint32_t value_copy = input_value;

    while (value_copy > 0) {
        ++value_size;
        value_copy >>= 1;
    }

    int byte_index = ((shift % KTotalBits) + 
                      (shift % KTotalBits) / 7) / kBitsInByte;
    int bit_position = (kBitsInByte - 1) - (shift % (kBitsInByte - 1)); 
    int start_index = byte_index * (kBitsInByte - 1) + 
                      (shift % (kBitsInByte - 1));
    int temp = (start_index + value_size - KTotalBits);
    int carry = static_cast<int>(temp >= 0) * temp;
    
    value_copy = (value_size == 32 && carry == 0) ? 0 : 
                 input_value >> (value_size - carry);

    for (int k = 0; k < byte_index; ++k) {
        result.data[kTotalBytes - k] = (value_copy % kMaxValueInByte) + 
                                        ((shift >> k) & 1) * kMaxValueInByte;
        value_copy >>= (kBitsInByte - 1);
    }

    result.data[kTotalBytes - 1 - byte_index] = 
        ((input_value % (1 << bit_position)) << (shift % (kBitsInByte - 1))) + 
        ((shift >> byte_index) & 1) * kMaxValueInByte;
    
    input_value >>= bit_position;
    ++byte_index;

    for (; byte_index < kTotalBytes; ++byte_index) {
        result.data[kTotalBytes - 1 - byte_index] = 
            (input_value % kMaxValueInByte) + 
            ((shift >> byte_index) & 1) * kMaxValueInByte;
        input_value >>= (kBitsInByte - 1);
    }

    for (int i = 0; i < kTotalBytes - 32; ++i) {
        result.data[i] -= static_cast<int>(result.data[i] >= kMaxValueInByte) * 
                           kMaxValueInByte;
    }

    return result;
}

uint239_t FromString(const char* input_str, uint32_t shift) {
    uint239_t result = {};


    const int byte_index = kTotalBytes - 1 - 
                          (((shift % KTotalBits) + 
                            (shift % KTotalBits) / (kBitsInByte - 1)) / 
                           kBitsInByte);
    const int bit_position = shift % (kBitsInByte - 1); 

    StringToInt128(result, shift, input_str, byte_index, byte_index, 
                   bit_position);

    for (int i = kTotalBytes - 1; i > kTotalBytes - 33; --i) {
        result.data[i] += ((shift >> (kTotalBytes - 1 - i)) & 1) * 
                           kMaxValueInByte;
    }


    return result;
}

uint239_t operator+(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t lhs_zero = ShiftToZero(lhs); 
    uint239_t rhs_zero = ShiftToZero(rhs); 
    uint64_t lhs_shift = GetShift(lhs);
    uint64_t rhs_shift = GetShift(rhs);
    uint239_t result = {};

    for (int i = kTotalBytes - 1; i > 0; --i) {
        int sum = lhs_zero.data[i] + rhs_zero.data[i];
        result.data[i - 1] = (result.data[i] + sum) / kMaxValueInByte;
        result.data[i] = (result.data[i] + sum) % kMaxValueInByte;
    }

    int sum = lhs_zero.data[0] + rhs_zero.data[0];
    result.data[0] = (result.data[0] + sum) % kMaxValueInByte;

    return MakeShift(result, (lhs_shift + rhs_shift) % kTwoToThePowerOf35);
}

uint239_t operator-(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t neg_rhs = rhs;
    uint239_t one = {};
    one.data[kTotalBytes-1] = 1;

    for (int i = 0; i < kTotalBytes; ++i) {
        neg_rhs.data[i] = neg_rhs.data[i] ^ (kMaxValueInByte - 1);
    }

    neg_rhs = neg_rhs + one;

    long long shift = GetShift(lhs) - GetShift(rhs);
    if (shift < 0) {
        shift = kTwoToThePowerOf35 + shift;
    }

    uint239_t result = ShiftToZero(lhs) + neg_rhs;
    return MakeShift(result, shift % kTwoToThePowerOf35);
}

uint239_t operator*(const uint239_t& lhs, const uint239_t& rhs) {
    const int kMaxDigitsInNumber = 72;
    int result[kMaxDigitsInNumber] = {};
    long long shift_left = GetShift(lhs);
    uint239_t shifted_lhs = ShiftToZero(lhs);

    int lhs_index;
    for (lhs_index = 0; lhs_index < kTotalBytes; ++lhs_index) {
        if (shifted_lhs.data[lhs_index] != 0) {
            lhs_index--; 
            break;
        }
    }

    long long shift_right = GetShift(rhs);
    uint239_t shifted_rhs = ShiftToZero(rhs);

    int rhs_index;
    for (rhs_index = 0; rhs_index < kTotalBytes; ++rhs_index) {
        if (shifted_rhs.data[rhs_index] != 0) {
            rhs_index--;
            break;
        }
    }

    for (int i = kTotalBytes - 1; i > rhs_index; --i) {
        int result_index = kTotalBytes - 1 - i;
        for (int j = kTotalBytes - 1; j > lhs_index; --j) {
            int temp = shifted_lhs.data[j] * shifted_rhs.data[i];
            result[result_index] += temp;
            ++result_index;
        }
    }

    int carry = 0;
    for (int i = 0; i < kTotalBytes; ++i) {
        result[i] += carry;
        carry = result[i] / kMaxValueInByte;
        result[i] %= kMaxValueInByte;
    }

    uint239_t answer = {};
    for (int i = 0; i < kTotalBytes; ++i) {
        answer.data[kTotalBytes - 1 - i] = result[i];
    }

    return MakeShift(answer, shift_left + shift_right);
}

bool operator==(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t lhs_copy = ShiftToZero(lhs);
    uint239_t rhs_copy = ShiftToZero(rhs);

    for (int i = 0; i < kTotalBytes; ++i) {
        if (lhs_copy.data[i] != rhs_copy.data[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const uint239_t& lhs, const uint239_t& rhs) {
    return !(lhs == rhs);
}

uint239_t operator/(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t shifted_lhs = ShiftToZero(lhs);
    uint239_t zero = FromInt(0, 0);
    uint239_t result = {};

    while (shifted_lhs >= rhs) {
        uint239_t shifted_rhs = ShiftToZero(rhs);
        int count = 0;

        while (shifted_lhs >= shifted_rhs) {
            ++count;
            Multiply2(shifted_rhs);
        }
        --count;
        PlaceBit(result, count);
        Divide2(shifted_rhs);
        shifted_lhs = shifted_lhs - shifted_rhs;
    }

    long long shift = GetShift(lhs) - GetShift(rhs);
    if (shift < 0) {
        shift = kTwoToThePowerOf35 + shift;
    }

    return MakeShift(result, shift);
}

std::ostream& operator<<(std::ostream& stream, const uint239_t& value) {
    uint239_t shifted_value = ShiftToZero(value);
    uint239_t ten = FromInt(10, 0);
    const int kMaxDigitsInNumber = 90;
    int digit_array[kMaxDigitsInNumber] = {};
    
    int index = kMaxDigitsInNumber - 1;
    while (shifted_value >= ten) {
        digit_array[index] = (shifted_value % ten).data[kTotalBytes - 1];
        shifted_value = shifted_value / ten;
        --index;
    }
    
    digit_array[index] = (shifted_value % ten).data[kTotalBytes - 1];
    
    for (int i = index; i < kMaxDigitsInNumber; ++i) { // исправлено k на i
        stream << digit_array[i];
    }

    return stream;
}

uint64_t GetShift(const uint239_t& value) {
    uint64_t result = 0;

    for (int i = 0; i < kTotalBytes; ++i) {
        result += (static_cast<long long>(value.data[i]) / kMaxValueInByte) 
                  << (kTotalBytes - 1 - i);
    }

    return result;
}

void PlaceBit(uint239_t& value, const int bit) {
    value.data[34 - bit / 7] += (1 << (bit % 7));
}

void PrintBinaryF(uint239_t value, int size) {

    for (int i = 0; i < size; ++i) {
        for (int j = 7; j > -1; --j) {
            if (j == 7 && i == 0) {
                std::cout << ( (value.data[i] >> j) & 1 ) << std::endl << ' ';
            } else if (j == 7) {
                std::cout << char(27) << "[1A" << ( (value.data[i] >> j) & 1 ) << char(27) << "[1B";
            } else {
                std::cout << ( (value.data[i] >> j) & 1 );
            }
        }
    }
}

void PrintBinaryS(uint239_t value, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 7; j > -1; --j) {
            std::cout << ( (value.data[i] >> j) & 1 );
        }
        std::cout << ' ';
    }
}

void PrintBinaryN(uint239_t value, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 7; j > -1; --j) {
            std::cout << ( (value.data[i] >> j) & 1 );
        }
    }
}

int StringToInt10(const char* str) {
    long long num = 0;
    int i = 0;
    while (str[i] != '\0') {
        int t = (int)(str[i]) - 48;
        num += t;
        num *= 10;
        ++i;
    }
    num /= 10;
    return num;
}

void StringToInt128(uint239_t& value, int shift, const char* str, int idx, 
                     const int i, const int j, int second) {

    int str_size = 0;
    while (str[str_size] != '\0') {
        ++str_size;
    }

    int temp = 0;
    bool has_carry = false; 

    if (str_size < 3) {
        temp = StringToInt10(str);
        has_carry = true;
    }

    char str_buffer[str_size + 1]; 
    memset(str_buffer, '\0', sizeof(str_buffer));

    if (has_carry) {
        str_size = 0; 
    }

    for (int j = 0, k = 0; j < str_size; ++j) {
        temp *= 10;
        temp += str[j] - '0';

        if (temp >= kMaxValueInByte) {
            str_buffer[k] = (temp / kMaxValueInByte) + '0';
            ++k;
            temp %= kMaxValueInByte;
            has_carry = true;
        } else if (has_carry) {
            str_buffer[k] = '0';
            ++k;
        }
    }

    int first_digit = (temp << j) % kMaxValueInByte;
    value.data[idx] = first_digit + second;
    second = temp >> (7 - j);

    if (idx - 1 == i || str_size < 3) {
        if (idx < 1) {
            value.data[kTotalBytes] += second;
        } else {
            value.data[idx - 1] += second;
        }
        value.data[kTotalBytes - 1] %= kMaxValueInByte;
        return;
    } else if (idx == 0) {
        idx += kTotalBytes;
    }

    StringToInt128(value, shift, str_buffer, idx - 1, i, j, second);
}


uint239_t ShiftToZero(const uint239_t& value) {
    uint239_t result = {};
    uint64_t shift = GetShift(value);
    int i = 34 - (((shift % 245) + (shift % 245) / 7) / 8);
    const int j = shift % 7; 
    for (int k = 34; k > -1; --k) {
        if (i-1 == -1) {
            result.data[k] = (((value.data[i])%128>>j) + ((value.data[34])%128<<(7-j)))%128;
            i = 35;
        } else {
            result.data[k] = (((value.data[i])%128>>j) + ((value.data[i-1])%128<<(7-j)))%128; 
        }
        --i;
    }
    return result;
}

uint239_t MakeShift(uint239_t& val, uint64_t shift) {
    uint239_t value = ShiftToZero(val);
    uint239_t result = {};

    int i = 34 - (((shift % 245) + (shift % 245) / 7) / 8);
    const int j = shift % 7; 
    int k = 34;
    int second = 0;

    int m = i;
    do {
        result.data[m] = (((value.data[k] << j))%128 + second)%128;
        second = (value.data[k] >> (7-j)) % 128;
        --k;
        --m;
        if (m == -1) m = 34;
    } while (m != i);

    for (int l = 34; l > -1; --l) {
        long long temp = (shift >> (34 - l));
        result.data[l] += (temp & 1) * 128;
    }

    return result;
}

uint239_t operator%(const uint239_t& lhs, const uint239_t& rhs) {
    return lhs - ((lhs / rhs) * rhs);
}

void Multiply2(uint239_t& value) {
    value = value * FromInt(2, 0);
}

void Divide2(uint239_t& value) {
    for (int i = 34; i > 0; --i) {
        if (value.data[i] >= 128) {
            value.data[i] -= 128;
            value.data[i] >>= 1;
            value.data[i] += 128;
            value.data[i] += (value.data[i-1] & 1) * 64;
        } else {
            value.data[i] >>= 1;
            value.data[i] += (value.data[i-1] & 1) * 64;
        }
    }
    if (value.data[0] >= 128) {
            value.data[0] -= 128;
            value.data[0] >>= 1;
            value.data[0] += 128;
        } else {
            value.data[0] >>= 1;
        }
}

bool operator>(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t clhs = ShiftToZero(lhs);
    uint239_t crhs = ShiftToZero(rhs);
    for (int i = 0; i < 35; ++i) {
        if (clhs.data[i] > crhs.data[i]) return true;
        else if (clhs.data[i] < crhs.data[i]) return false;
    }
    return false;
}

bool operator<(const uint239_t& lhs, const uint239_t& rhs) {
    return !(lhs>rhs || lhs==rhs);
}

bool operator>=(const uint239_t& lhs, const uint239_t& rhs) {
    return (lhs>rhs || lhs==rhs);
}