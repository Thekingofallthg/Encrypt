#include <emscripten/bind.h>
#include <string>

const int shifts[20] = {5, 2, 1, 7, 3, 6, 7, 2, 2, 4, 5, 9, 2, 5, 5, 6, 8, 3, 4, 2};

std::string encryptText(std::string text) {
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = ((text[i] - 'a' + shifts[i % 20]) % 26) + 'a';
        }
    }
    return text;
}

std::string decryptText(std::string text) {
    for (size_t j = 0; j < text.length(); j++) {
        if (text[j] >= 'a' && text[j] <= 'z') {
            text[j] = ((text[j] - 'a' - shifts[j % 20] + 26) % 26) + 'a';
        }
    }
    return text;
}

EMSCRIPTEN_BINDINGS(cipher_module) {
    emscripten::function("encryptText", &encryptText);
    emscripten::function("decryptText", &decryptText);
}
