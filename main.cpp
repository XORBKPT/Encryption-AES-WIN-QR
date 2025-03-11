#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <mbedtls/aes.h>
#include <sstream>
#include <fstream>
#include "qrcodegen.hpp"

// Constants
const size_t MAX_MESSAGE_LEN = 256;
const size_t KEY_LEN = 32;  // 256 bits for AES-256
const size_t IV_LEN = 16;   // AES block size

// Function prototypes
std::string read_line(const std::string& prompt);
bool hex_to_bytes(const std::string& hex, std::vector<uint8_t>& bytes);
std::string bytes_to_hex(const std::vector<uint8_t>& bytes);
std::vector<uint8_t> pkcs7_pad(const std::string& data, size_t block_size);
void save_qr_to_bmp(const qrcodegen::QrCode& qr, const std::string& filename, int pixel_size = 10);
void print_formatted_hex(const std::string& hex);

// Reads input safely, trims whitespace
std::string read_line(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    size_t start = line.find_first_not_of(" \t");
    size_t end = line.find_last_not_of(" \t");
    if (start == std::string::npos || end == std::string::npos) return "";
    return line.substr(start, end - start + 1);
}

// Converts hex string to bytes with validation
bool hex_to_bytes(const std::string& hex, std::vector<uint8_t>& bytes) {
    if (hex.size() % 2 != 0) return false;
    bytes.clear();
    bytes.reserve(hex.size() / 2);
    for (size_t i = 0; i < hex.size(); i += 2) {
        std::string byte_str = hex.substr(i, 2);
        if (!std::all_of(byte_str.begin(), byte_str.end(), ::isxdigit)) return false;
        uint8_t byte = static_cast<uint8_t>(std::stoi(byte_str, nullptr, 16));
        bytes.push_back(byte);
    }
    return true;
}

// Converts bytes to hex string
std::string bytes_to_hex(const std::vector<uint8_t>& bytes) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    for (uint8_t byte : bytes) {
        oss << std::setw(2) << static_cast<int>(byte);
    }
    return oss.str();
}

// Applies PKCS7 padding
std::vector<uint8_t> pkcs7_pad(const std::string& data, size_t block_size) {
    size_t pad_len = block_size - (data.size() % block_size);
    if (pad_len == 0) pad_len = block_size;
    std::vector<uint8_t> padded(data.begin(), data.end());
    padded.insert(padded.end(), pad_len, static_cast<uint8_t>(pad_len));
    return padded;
}

// Saves QR code as a 24-bit BMP file
void save_qr_to_bmp(const qrcodegen::QrCode& qr, const std::string& filename, int pixel_size) {
    int size = qr.getSize();
    int width = size * pixel_size;
    int height = size * pixel_size;
    int row_size = (width * 3 + 3) & ~3;  // Pad to 4 bytes
    int pixel_data_size = row_size * height;
    int file_size = 54 + pixel_data_size;

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // BMP Header
    file.put('B').put('M');
    file.write(reinterpret_cast<const char*>(&file_size), 4);
    file.write("\0\0\0\0", 4);  // Reserved
    int offset = 54;
    file.write(reinterpret_cast<const char*>(&offset), 4);

    // DIB Header
    int dib_size = 40;
    file.write(reinterpret_cast<const char*>(&dib_size), 4);
    file.write(reinterpret_cast<const char*>(&width), 4);
    file.write(reinterpret_cast<const char*>(&height), 4);
    short planes = 1;
    file.write(reinterpret_cast<const char*>(&planes), 2);
    short bpp = 24;
    file.write(reinterpret_cast<const char*>(&bpp), 2);
    int compression = 0;
    file.write(reinterpret_cast<const char*>(&compression), 4);
    file.write(reinterpret_cast<const char*>(&pixel_data_size), 4);
    int x_pixels_per_m = 0;
    file.write(reinterpret_cast<const char*>(&x_pixels_per_m), 4);
    int y_pixels_per_m = 0;
    file.write(reinterpret_cast<const char*>(&y_pixels_per_m), 4);
    int colors_used = 0;
    file.write(reinterpret_cast<const char*>(&colors_used), 4);
    int colors_important = 0;
    file.write(reinterpret_cast<const char*>(&colors_important), 4);

    // Pixel data (bottom-up)
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            int qr_x = x / pixel_size;
            int qr_y = y / pixel_size;
            bool is_dark = qr.getModule(qr_x, qr_y);
            if (is_dark) {
                file.put(0).put(0).put(0);  // Black
            } else {
                file.put(255).put(255).put(255);  // White
            }
        }
        // Pad to row_size
        for (int p = width * 3; p < row_size; ++p) {
            file.put(0);
        }
    }
}

// Prints formatted hex and generates QR codes per line
void print_formatted_hex(const std::string& hex) {
    size_t len = hex.size();
    size_t index = 0;
    size_t line_num = 1;
    while (index < len) {
        std::string line_text;
        std::cout << std::right << std::setw(2) << line_num << " ";
        for (int group = 0; group < 5 && index < len; ++group) {
            std::string group_str;
            for (int j = 0; j < 5 && index < len; ++j) {
                group_str += hex[index];
                std::cout << hex[index++];
            }
            line_text += group_str + " ";
            std::cout << " ";
        }
        if (!line_text.empty()) {
            line_text.pop_back();  // Remove trailing space
            qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(line_text.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
            std::string filename = "qr_line" + std::to_string(line_num) + ".bmp";
            save_qr_to_bmp(qr, filename);
        }
        std::cout << std::endl;
        if (line_num % 10 == 0) std::cout << std::endl << std::endl;
        ++line_num;
    }
    std::cout << "QR codes saved as qr_lineX.bmp files." << std::endl;
}

int main() {
    std::cout << "Aerospace Encryption Tool\n";
    std::cout << "-------------------------\n";

    // Get message
    std::string message = read_line("Enter message (max 256 chars): ");
    if (message.empty() || message.size() > MAX_MESSAGE_LEN) {
        std::cerr << "Error: Message must be 1-256 characters.\n";
        return 1;
    }

    // Get key
    std::string key_hex = read_line("Enter 256-bit key (64 hex chars): ");
    std::vector<uint8_t> key;
    if (key_hex.size() != KEY_LEN * 2 || !hex_to_bytes(key_hex, key)) {
        std::cerr << "Error: Key must be 64 hex characters.\n";
        return 1;
    }

    // Get IV
    std::string iv_hex = read_line("Enter IV (32 hex chars): ");
    std::vector<uint8_t> iv;
    if (iv_hex.size() != IV_LEN * 2 || !hex_to_bytes(iv_hex, iv)) {
        std::cerr << "Error: IV must be 32 hex characters.\n";
        return 1;
    }

    // Pad message
    std::vector<uint8_t> padded = pkcs7_pad(message, IV_LEN);

    // Initialize AES context
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    if (mbedtls_aes_setkey_enc(&aes, key.data(), KEY_LEN * 8) != 0) {
        std::cerr << "Error: Failed to set encryption key.\n";
        mbedtls_aes_free(&aes);
        return 1;
    }

    // Encrypt
    std::vector<uint8_t> ciphertext(padded.size());
    if (mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, padded.size(), iv.data(), padded.data(), ciphertext.data()) != 0) {
        std::cerr << "Error: Encryption failed.\n";
        mbedtls_aes_free(&aes);
        return 1;
    }

    // Output result
    std::string hex_output = bytes_to_hex(ciphertext);
    std::cout << "\nEncrypted Output (Hex):\n";
    print_formatted_hex(hex_output);

    // Cleanup
    mbedtls_aes_free(&aes);
    std::cout << "\nEncryption complete.\n";
    return 0;
}
