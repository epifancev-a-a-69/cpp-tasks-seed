#include <gtest/gtest.h>
#include <random>
#include <string>
#include <algorithm>

#include "base85ed.h"

// Базовые тестовые случаи
const std::vector<std::pair<const char*, const char*>> short_cases = {
    {"", ""},
    {"F#", "1"},
    {"F){", "12"},
    {"F)}j", "123"},
    {"F)}kW", "1234"},
    {"F)}kWD", "12345"},
    {"G%G", "ab"},
    {"G%G*", "abc"},
    {"G%G*D", "abcd"},
    {"G%G*D+", "abcde"}
};

// Преобразование C-строки в вектор
static std::vector<uint8_t> cstr2v(const char* s) {
    return std::vector<uint8_t>(s, s + std::strlen(s));
}

// Генерация случайных данных для тестирования
static std::vector<uint8_t> generate_random_data(size_t size) {
    std::vector<uint8_t> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<uint8_t>(dis(gen));
    }
    return data;
}

// Тест encode с короткими строками
TEST(Base85ShortsEncode, TrivialShortEncodes) {
    for (const auto& p : short_cases) {
        EXPECT_EQ(base85::encode(cstr2v(p.second)), cstr2v(p.first))
            << "Failed encoding: '" << p.second << "'";
    }
}

// Тест decode с короткими строками
TEST(Base85ShortsDecode, TrivialShortDecodes) {
    for (const auto& p : short_cases) {
        EXPECT_EQ(base85::decode(cstr2v(p.first)), cstr2v(p.second))
            << "Failed decoding: '" << p.first << "'";
    }
}

// Тест round-trip для коротких строк
TEST(Base85RoundTrip, ShortStrings) {
    const std::vector<std::string> test_strings = {
        "", "a", "ab", "abc", "abcd", "abcde",
        "Hello", "World!", "Test123", "Base85",
        "Lorem ipsum", "dolor sit amet",
        "!@#$%^&*()", "[]{}|;:',.<>?`~",
        "1234567890", "abcdefghij",
        "\x00\x01\x02\x03", "\xFF\xFE\xFD\xFC",
        "\x00\xFF\x00\xFF\x00"
    };
    
    for (const auto& s : test_strings) {
        auto encoded = base85::encode(std::vector<uint8_t>(s.begin(), s.end()));
        auto decoded = base85::decode(encoded);
        EXPECT_EQ(std::vector<uint8_t>(s.begin(), s.end()), decoded)
            << "Round-trip failed for: '" << s << "'";
    }
}

// Тест encode/decode для длинных строк
TEST(Base85LongStrings, EncodeDecode) {
    std::vector<size_t> test_sizes = {
        1, 2, 3, 4, 5, 10, 50, 100, 256, 500, 1000, 10000
    };
    
    for (size_t size : test_sizes) {
        auto original = generate_random_data(size);
        auto encoded = base85::encode(original);
        auto decoded = base85::decode(encoded);
        
        EXPECT_EQ(original, decoded)
            << "Round-trip failed for size: " << size;
    }
}

// Тест специального случая с нулями
TEST(Base85SpecialCases, ZeroBlock) {
    // Четыре нуля должны кодироваться как 'z'
    std::vector<uint8_t> zeros = {0, 0, 0, 0};
    auto encoded = base85::encode(zeros);
    
    // Проверяем, что результат содержит 'z'
    EXPECT_EQ(encoded.size(), 1);
    EXPECT_EQ(encoded[0], 'z');
    
    // Проверяем декодирование
    auto decoded = base85::decode(encoded);
    EXPECT_EQ(decoded, zeros);
}

// Тест множественных нулевых блоков
TEST(Base85SpecialCases, MultipleZeroBlocks) {
    std::vector<uint8_t> data(12, 0); // 12 нулевых байт (3 блока по 4)
    auto encoded = base85::encode(data);
    auto decoded = base85::decode(encoded);
    EXPECT_EQ(decoded, data);
}

// Тест всех возможных значений байтов
TEST(Base85EdgeCases, AllByteValues) {
    std::vector<uint8_t> all_bytes(256);
    for (int i = 0; i < 256; ++i) {
        all_bytes[i] = static_cast<uint8_t>(i);
    }
    
    auto encoded = base85::encode(all_bytes);
    auto decoded = base85::decode(encoded);
    EXPECT_EQ(decoded, all_bytes);
}

// Тест последовательности нулей разной длины
TEST(Base85EdgeCases, VaryingZeroLengths) {
    for (size_t len = 0; len <= 20; ++len) {
        std::vector<uint8_t> zeros(len, 0);
        auto encoded = base85::encode(zeros);
        auto decoded = base85::decode(encoded);
        EXPECT_EQ(decoded, zeros) << "Failed for length: " << len;
    }
}

// Тест с максимальными значениями
TEST(Base85EdgeCases, MaxValues) {
    for (size_t len = 1; len <= 10; ++len) {
        std::vector<uint8_t> max_vals(len, 0xFF);
        auto encoded = base85::encode(max_vals);
        auto decoded = base85::decode(encoded);
        EXPECT_EQ(decoded, max_vals) << "Failed for max values length: " << len;
    }
}

// Тест с чередующимися значениями
TEST(Base85EdgeCases, AlternatingPatterns) {
    for (size_t len = 1; len <= 16; ++len) {
        std::vector<uint8_t> pattern(len);
        for (size_t i = 0; i < len; ++i) {
            pattern[i] = (i % 2 == 0) ? 0xAA : 0x55;
        }
        auto encoded = base85::encode(pattern);
        auto decoded = base85::decode(encoded);
        EXPECT_EQ(decoded, pattern) << "Failed for alternating pattern length: " << len;
    }
}

// Тест decode с пробелами (если поддерживается)
TEST(Base85Decode, WithWhitespace) {
    std::vector<uint8_t> with_spaces = cstr2v("F)} kW");
    std::vector<uint8_t> expected = cstr2v("1234");
    auto decoded = base85::decode(with_spaces);
    EXPECT_EQ(decoded, expected);
}

// Стресс-тест с большими данными
TEST(Base85StressTest, LargeData) {
    for (size_t size : {1000, 5000, 10000, 50000}) {
        auto original = generate_random_data(size);
        
        auto start = std::chrono::high_resolution_clock::now();
        auto encoded = base85::encode(original);
        auto decoded = base85::decode(encoded);
        auto end = std::chrono::high_resolution_clock::now();
        
        EXPECT_EQ(original, decoded) << "Stress test failed for size: " << size;
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Size: " << size << " bytes, time: " << duration.count() << "ms" << std::endl;
    }
}

// Тест производительности (только убеждаемся что нет ошибок)
TEST(Base85Performance, EncodeDecodeSpeed) {
    auto test_data = generate_random_data(100000);
    
    // Выполняем множественные операции для проверки стабильности
    for (int i = 0; i < 10; ++i) {
        auto encoded = base85::encode(test_data);
        auto decoded = base85::decode(encoded);
        EXPECT_EQ(decoded, test_data) << "Performance test failed at iteration: " << i;
    }
}

// Тест граничных условий с очень маленькими входами
TEST(Base85Boundary, VerySmallInputs) {
    for (uint8_t val : {0x00, 0x01, 0x7F, 0x80, 0xFF}) {
        std::vector<uint8_t> input = {val};
        auto encoded = base85::encode(input);
        auto decoded = base85::decode(encoded);
        EXPECT_EQ(decoded, input) << "Failed for single byte: " << (int)val;
    }
}

// Тест с поврежденными данными (ожидаем исключения)
TEST(Base85InvalidInput, InvalidCharacters) {
    // Символы вне диапазона Base85
    std::vector<uint8_t> invalid = {'~', 0x7F, 0x80, 0xFF};
    EXPECT_THROW(base85::decode(invalid), std::runtime_error);
}

// Тест с неполными группами
TEST(Base85InvalidInput, IncompleteGroup) {
    // Одиночный символ не может быть декодирован
    std::vector<uint8_t> incomplete = {'A'};
    EXPECT_THROW(base85::decode(incomplete), std::runtime_error);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}