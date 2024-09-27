/*
 * Metodología de la Programación: Language5
 * Curso 2022/2023
 */

/** 
 * @file BigramCounter.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * 
 * Created on 12 February 2023, 11:00
 */

#include "BigramCounter.h"
#include <fstream>

/**
 * DEFAULT_VALID_CHARACTERS is a c-string that contains the set of characters
 * that will be considered as part of a word (valid chars). 
 * The characters not in the c-string will be considered as separator of words.
 * The constructor of the class BigramCounter uses this c-string as a 
 * default parameter. It is possible to use a different c-string if that
 * constructor is used with a different c-string
 */
const char* const BigramCounter::DEFAULT_VALID_CHARACTERS = "abcdefghijklmnopqrstuvwxyz\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF";

BigramCounter::BigramCounter(const std::string &validChars) {
    _validCharacters = validChars;
    int tamanio = _validCharacters.size();
    _frequency = nullptr;
    asignarM();
}

BigramCounter::BigramCounter(const BigramCounter &orig) {
    _validCharacters = orig._validCharacters;
    int tamanio = orig.getSize();
    _frequency = nullptr;
    asignarM();

    for (int i = 0; i < tamanio; i++)
        for (int j = 0; j < tamanio; j++)
            _frequency[i][j] = orig._frequency[i][j];
}

BigramCounter::~BigramCounter() {
    liberarM();
}

int BigramCounter::getSize() const {
    return _validCharacters.size();
}

int BigramCounter::getNumberActiveBigrams() const {
    int resultado = 0;
    for (int i = 0; i < getSize(); i++)
        for (int j = 0; j < getSize(); j++)
            if (_frequency[i][j] > 0)
                resultado++;

    return resultado;
}

bool BigramCounter::setFrequency(const Bigram &bigram, const int &frequency) {
    if (frequency < 0)
        throw std::out_of_range("se intento meter una frecuencia negativa en una matriz");

    bool salida = false;

    int pos1 = findCharacter(bigram.at(0), _validCharacters);
    int pos2 = findCharacter(bigram.at(1), _validCharacters);

    if (!(pos1 == -1 || pos2 == -1)) {
        salida = true;
        _frequency[pos1][pos2] = frequency;
    }

    return salida;
}

void BigramCounter::increaseFrequency(const Bigram &bigram, const int &frequency) {
    if (findCharacter(bigram.at(0), _validCharacters) == -1 && findCharacter(bigram.at(0), _validCharacters) == -1)
        throw std::invalid_argument("bigrama no valido");

    int extra = 0;
    if (frequency < 0)
        throw std::out_of_range("se intento aumentar una frecuencia con un numero negativo en una matriz");

    if (frequency == 0)
        extra = 1;

    int pos1 = findCharacter(bigram.at(0), _validCharacters);
    int pos2 = findCharacter(bigram.at(1), _validCharacters);
    if (pos1 != -1 && pos2 != -1)
        _frequency[pos1][pos2] = _frequency[pos1][pos2] + extra + frequency;
}

BigramCounter& BigramCounter::operator=(const BigramCounter &orig) {
    if (this != &orig) {
        int tamanio = orig.getSize();
        liberarM();

        _validCharacters = orig._validCharacters;
        asignarM();

        for (int i = 0; i < tamanio; i++)
            for (int j = 0; j < tamanio; j++)
                _frequency[i][j] = orig._frequency[i][j];
    }
    return *this;
}

BigramCounter& BigramCounter::operator+=(const BigramCounter &rhs) {
    if (_validCharacters == rhs._validCharacters)
        for (int i = 0; i < rhs.getSize(); i++)
            for (int j = 0; j < rhs.getSize(); j++)
                _frequency[i][j] = _frequency[i][j] + rhs._frequency[i][j];
    else
        throw std::invalid_argument("Se intentaron sumar dos BigramCounters con distintos caracteres validos");

    return *this;
}

bool BigramCounter::calculateFrequencies(const char fileName[]) {
    std::string valor;
    std::ifstream fichero;
    fichero.open(fileName);
    if (!fichero)
        throw std::ios_base::failure("Error al leer el fichero");

    while (fichero >> valor) {
        valor = aMinuscula(valor);
        for (int i = 1; i < valor.size(); i++)
            if (findCharacter(valor[i - 1], _validCharacters) != -1 && findCharacter(valor[i], _validCharacters) != -1) {
                Bigram bigrama(valor[i - 1], valor[i]);
                increaseFrequency(bigrama);
            }
    }
    fichero.close();
    return true;
}

Language BigramCounter::toLanguage() const {
    Language resultado;
    for (int i = 0; i < getSize(); i++)
        for (int j = 0; j < getSize(); j++)
            if (_frequency[i][j] > 0) {
                Bigram bigrama(_validCharacters[i], _validCharacters[j]);
                BigramFreq bigramFreq;
                bigramFreq.setBigram(bigrama);
                bigramFreq.setFrequency(_frequency[i][j]);
                resultado.append(bigramFreq);
            }
    resultado.sort();
    return resultado;
}

int& BigramCounter::operator()(const int &row, const int &column) {
    if (!(row > 0 && row < getSize() && column > 0 && column < getSize()))
        throw std::out_of_range("Se intento acceder a una frecuencia no existente");

    return _frequency[row][column];
}

const int& BigramCounter::operator()(const int &row, const int &column) const {
    if (!(row > 0 && row < getSize() && column > 0 && column < getSize()))
        throw std::out_of_range("Se intento acceder a una frecuencia no existente");

    return _frequency[row][column];
}

void BigramCounter::liberarM() {
    if (_frequency != nullptr) {
        for (int i = 0; i < getSize(); i++)
            delete [] _frequency[i];

        delete [] _frequency;
        _frequency = nullptr;
    }
}

void BigramCounter::asignarM() {
    if (_frequency == nullptr) {
        int tam = getSize();
        _frequency = new int* [tam];

        for (int i = 0; i < tam; i++)
            _frequency[i] = new int [tam];

        for (int i = 0; i < tam; i++)
            for (int j = 0; j < tam; j++)
                _frequency[i][j] = 0;
    }

}

std::string aMinuscula(std::string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);

    return cadena;
}