#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <cstring>
#include <algorithm>

#include "base85ed.h"

namespace base85 {

// Стандартный Base85 алфавит (ASCII85)
static const char BASE85_ALPHABET[] = 
    "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstu";

// Карта для декодирования
static uint8_t decode_char(char c) {
    if (c >= '!' && c <= 'u') {
        return static_cast<uint8_t>(c - '!');
    }
    throw std::runtime_error(std::string("Invalid Base85 character: ") + c);
}

std::vector<uint8_t> encode(std::vector<uint8_t> const& bytes) {
    std::vector<uint8_t> result;
    size_t i = 0;
    
    // Резервируем место для результата (примерно 5/4 от входного размера)
    result.reserve(bytes.size() * 5 / 4 + 10);
    
    while (i + 4 <= bytes.size()) {
        // Читаем 4 байта как 32-битное число (big-endian)
        uint32_t value = (static_cast<uint32_t>(bytes[i]) << 24) |
                        (static_cast<uint32_t>(bytes[i + 1]) << 16) |
                        (static_cast<uint32_t>(bytes[i + 2]) << 8) |
                         static_cast<uint32_t>(bytes[i + 3]);
        
        if (value == 0) {
            // Специальный случай: четыре нуля -> 'z'
            result.push_back('z');
        } else {
            // Кодируем в 5 Base85 символов
            char encoded[5];
            encoded[4] = BASE85_ALPHABET[value % 85]; value /= 85;
            encoded[3] = BASE85_ALPHABET[value % 85]; value /= 85;
            encoded[2] = BASE85_ALPHABET[value % 85]; value /= 85;
            encoded[1] = BASE85_ALPHABET[value % 85]; value /= 85;
            encoded[0] = BASE85_ALPHABET[value % 85];
            result.insert(result.end(), encoded, encoded + 5);
        }
        
        i += 4;
    }
    
    // Обрабатываем оставшиеся байты
    if (i < bytes.size()) {
        size_t remaining = bytes.size() - i;
        uint32_t value = 0;
        
        // Читаем оставшиеся байты
        for (size_t j = 0; j < remaining; ++j) {
            value = (value << 8) | bytes[i + j];
        }
        
        // Дополняем до 4 байт
        value <<= 8 * (4 - remaining);
        
        // Кодируем и берём только нужное количество символов
        char encoded[5];
        encoded[4] = BASE85_ALPHABET[value % 85]; value /= 85;
        encoded[3] = BASE85_ALPHABET[value % 85]; value /= 85;
        encoded[2] = BASE85_ALPHABET[value % 85]; value /= 85;
        encoded[1] = BASE85_ALPHABET[value % 85]; value /= 85;
        encoded[0] = BASE85_ALPHABET[value % 85];
        
        // Для n оставшихся байт нужно n+1 символов
        result.insert(result.end(), encoded, encoded + remaining + 1);
    }
    
    return result;
}

std::vector<uint8_t> decode(std::vector<uint8_t> const& b85str) {
    std::vector<uint8_t> result;
    size_t i = 0;
    
    // Удаляем пробельные символы
    std::vector<uint8_t> clean;
    clean.reserve(b85str.size());
    for (uint8_t c : b85str) {
        if (c != ' ' && c != '\n' && c != '\r' && c != '\t') {
            clean.push_back(c);
        }
    }
    
    i = 0;
    while (i < clean.size()) {
        uint32_t value = 0;
        size_t group_size = 0;
        
        // Проверяем специальный случай 'z'
        if (clean[i] == 'z') {
            result.insert(result.end(), 4, 0);
            i++;
            continue;
        }
        
        // Собираем группу до 5 символов
        size_t chunk_start = i;
        while (i < clean.size() && (i - chunk_start) < 5) {
            value = value * 85 + decode_char(static_cast<char>(clean[i]));
            group_size++;
            i++;
        }
        
        if (group_size < 2) {
            throw std::runtime_error("Invalid Base85 data: incomplete group");
        }
        
        size_t output_bytes = group_size - 1;
        
        // Дополняем недостающие символы максимальным значением
        for (size_t j = group_size; j < 5; ++j) {
            value = value * 85 + 84;
        }
        
        // Извлекаем байты (big-endian порядок)
        uint8_t decoded[4];
        decoded[3] = value & 0xFF; value >>= 8;
        decoded[2] = value & 0xFF; value >>= 8;
        decoded[1] = value & 0xFF; value >>= 8;
        decoded[0] = value & 0xFF;
        
        result.insert(result.end(), decoded, decoded + output_bytes);
    }
    
    return result;
}

} // namespace base85