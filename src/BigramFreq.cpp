/** 
 * @file:   BigramFreq.cpp
 * @Author: Ivan Rodriguez Chacon
 * 
 * Created on 18 de mayo de 2023, 9:38
 */

#include "BigramFreq.h"

BigramFreq::BigramFreq() {
    _bigram.editText("__");
    _frequency = 0;
}

const Bigram& BigramFreq::getBigram() const {
    return _bigram;
}

int BigramFreq::getFrequency() const {
    return _frequency;
}

void BigramFreq::setBigram(const Bigram& bigram) {
    _bigram.editText(bigram.getText());
}

void BigramFreq::setFrequency(const int &frequency) {
    if (frequency >= 0)
        _frequency = frequency;
    else
        throw std::out_of_range("Se esta poniendo una frecuencia negativa en un BigramFreq");
}

std::string BigramFreq::toString() const {
    std::string freq = std::to_string(_frequency);
    std::string salida = _bigram.toString() + ' ' + freq;
    return salida;
}

void BigramFreq::serialize(std::ostream& outputStream) const {
    outputStream.write(_bigram.getText().c_str(), sizeof (char) * 2);
    outputStream.write(reinterpret_cast<const char*> (&_frequency), sizeof (_frequency));
}

void BigramFreq::deserialize(std::istream& inputSstream) {
    char bigram[2];
    int freq;
    inputSstream.read(bigram, sizeof (char) * 2);
    inputSstream.read(reinterpret_cast<char*> (&freq), sizeof (freq));
    _bigram.editText(bigram);
    _frequency = freq;
}

std::ostream& operator<<(std::ostream &os, const BigramFreq &bigramFreq) {
    os << bigramFreq.getBigram() << bigramFreq.getFrequency();
    return os;
}

std::istream& operator>>(std::istream &is, BigramFreq &bigramFreq) {
    Bigram bigram;
    int freq;
    is >> bigram>>freq;
    bigramFreq.setBigram(bigram);
    bigramFreq.setFrequency(freq);
    return is;
}

bool operator>(const BigramFreq &bigramFreq1, const BigramFreq &bigramFreq2) {
    bool resultado = false;
    int freq1 = bigramFreq1.getFrequency();
    int freq2 = bigramFreq2.getFrequency();

    if (freq1 > freq2 || (freq1 == freq2 && bigramFreq1.getBigram().toString() < bigramFreq2.getBigram().toString()))
        resultado = true;

    return resultado;
}

bool operator<(const BigramFreq &bigramFreq1, const BigramFreq &bigramFreq2) {
    bool resultado = false;
    std::string bigram1 = bigramFreq1.toString();
    std::string bigram2 = bigramFreq2.toString();
    int freq1 = bigramFreq1.getFrequency();
    int freq2 = bigramFreq2.getFrequency();

    if (freq1 < freq2 || (freq1 == freq2 && bigram1 > bigram2))
        resultado = true;

    return resultado;
}

bool operator==(BigramFreq bigramFreq1, BigramFreq bigramFreq2) {
    bool resultado = false;
    std::string bigram1 = bigramFreq1.toString();
    std::string bigram2 = bigramFreq2.toString();
    int freq1 = bigramFreq1.getFrequency();
    int freq2 = bigramFreq2.getFrequency();

    if (freq1 == freq2 && bigram1 == bigram2)
        resultado = true;

    return resultado;
}

bool operator!=(const BigramFreq &bigramFreq1, const BigramFreq &bigramFreq2) {
    bool resultado = (bigramFreq1 == bigramFreq2);
    return !resultado;
}

bool operator<=(const BigramFreq &bigramFreq1, const BigramFreq &bigramFreq2) {
    bool es_igual = (bigramFreq1 == bigramFreq2);
    bool es_menor = (bigramFreq1 < bigramFreq2);
    bool resultado = es_igual && es_menor;

    return resultado;
}

bool operator>=(const BigramFreq &bigramFreq1, const BigramFreq &bigramFreq2) {
    bool es_igual = (bigramFreq1 == bigramFreq2);
    bool es_menor = (bigramFreq1 > bigramFreq2);
    bool resultado = es_igual && es_menor;

    return resultado;
}

