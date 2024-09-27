/** 
 * @file:   Bigram.cpp
 * @Author: Ivan Rodriguez Chacon
 * 
 * Created on 18 de mayo de 2023, 9:38
 */

#include "Bigram.h"
#include <cstring>

Bigram::Bigram(const std::string& text) {
    if (text.size() != 2) {
        _text[0] = '_';
        _text[1] = '_';
    } else
        for (int i = 0; i < text.size(); i++)
            _text[i] = text[i];


    _text[2] = '\0';

}

Bigram::Bigram(const char first, const char second) {
    _text[0] = first;
    _text[1] = second;
    _text[2] = '\0';
}

Bigram::Bigram(const char text[]) {
    if (strlen(text) != 2)
        for (int i = 0; i < 2; i++)
            _text[i] = '_';

    else
        memcpy(_text, text, sizeof (_text));

    _text[2] = '\0';
}

std::string Bigram::getText() const {
    return _text;
}

void Bigram::editText(std::string cadena) {
    _text[0] = cadena[0];
    _text[1] = cadena[1];
    _text[2] = '\0';
}

std::string Bigram::toString() const {
    return _text;
}

const char& Bigram::at(int index) const {
    if (index < 0 || index > 1)
        throw std::out_of_range("Introduzca los dígitos 0 o 1.");
    else
        return _text[index];
}

char& Bigram::at(int index) {
    if (index < 0 || index > 1)
        throw std::out_of_range("Introduzca los dígitos 0 o 1.");
    else
        return _text[index];
}

void Bigram::toUpper() {
    const int dif_min_mayusc = 'a' - 'A';

    for (int i = 0; i < 2; i++)
        if (_text[i] >= 'a' && _text[i] <= 'z')
            _text[i] -= dif_min_mayusc;
}

bool isValidCharacter(char character, const std::string& validCharacters) {
    bool salida = false;

    for (int i = 0; i < validCharacters.size() && !salida; i++)
        if (validCharacters[i] == character)
            salida = true;

    return salida;
}

int findCharacter(const char &carac, const std::string &array) {
    int resultado = static_cast<int> (array.find(carac));
    return resultado;
}

std::ostream& operator<<(std::ostream &os, const Bigram &bigram) {
    os << bigram.toString();
    return os;
}

std::istream& operator>>(std::istream &is, Bigram &bigram) {
    std::string cadena;
    is>>cadena;
    bigram.editText(cadena);
    return is;
}