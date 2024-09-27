/*
 * Metodología de la Programación: Language5
 * Curso 2022/2023
 */

/** 
 * @file Language.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * 
 * Created on 29 January 2023, 11:00
 */

#include "Language.h"
#include <fstream>

const std::string Language::MAGIC_STRING_T = "MP-LANGUAGE-T-1.0";
const std::string Language::MAGIC_STRING_B = "MP-LANGUAGE-B-1.0";

Language::Language() {
    _languageId = "unknow";
    _size = 0;
    _vectorBigramFreq = nullptr;
}

Language::Language(const int &numberBigrams) {
    if (numberBigrams < 0)
        throw std::out_of_range("Se intento crear un array de un numero negativo de elementos");

    _languageId = "unknow";
    asignarM(numberBigrams);

}

Language::Language(const Language& orig) {
    _languageId = orig._languageId;
    asignarM(orig._size);

    for (int i = 0; i < _size; i++) {
        _vectorBigramFreq[i].setBigram(orig._vectorBigramFreq[i].getBigram());
        _vectorBigramFreq[i].setFrequency(orig._vectorBigramFreq[i].getFrequency());
    }
}

Language::~Language() {
    liberarM();
}

Language& Language::operator=(const Language& orig) {
    if (this != &orig) {
        liberarM();
        _languageId = orig._languageId;
        asignarM(orig._size);

        for (int i = 0; i < _size; i++) {
            _vectorBigramFreq[i].setBigram(orig._vectorBigramFreq[i].getBigram());
            _vectorBigramFreq[i].setFrequency(orig._vectorBigramFreq[i].getFrequency());
        }
    }
    return *this;
}

const std::string& Language::getLanguageId() const {
    return _languageId;
}

const std::string& Language::getMagicStringT() const {
    return MAGIC_STRING_T;
}

const std::string& Language::getMagicStringB() const {
    return MAGIC_STRING_B;
}

void Language::setLanguageId(const std::string& id) {
    _languageId = id;
}

BigramFreq& Language::at(const int &index) {
    if (index < 0 || index > _size)
        throw std::out_of_range("El numero introducido no es valido.");

    else
        return _vectorBigramFreq[index];
}

const BigramFreq& Language::at(const int &index) const {
    if (index < 0 || index > _size)
        throw std::out_of_range("El numero introducido no es valido.");

    else
        return _vectorBigramFreq[index];
}

const int& Language::getSize() const {
    return _size;
}

double Language::getDistance(const Language& otherLanguage) const {
    if (_size == 0)
        throw std::invalid_argument("Se intenta calcular la distancia a un objeto Language vacio");
    double sumatoria = 0, total = 0;

    for (int i = 0; i < _size; i++) {
        int busqueda = otherLanguage.findBigram(_vectorBigramFreq[i].getBigram());
        if (busqueda == -1)
            busqueda = _size;

        sumatoria += abs(i - busqueda);
    }
    total = sumatoria / (_size * _size);

    return total;
}

int Language::findBigram(const Bigram& bigram) const {
    int devolver = -1;
    for (int i = 0; i < _size || devolver == i; i++)
        if (bigram.getText() == _vectorBigramFreq[i].getBigram().getText())
            devolver = i;

    return devolver;
}

std::string Language::toString() const {
    std::string salida = "";
    for (int i = 0; i < _size; i++)
        salida += _vectorBigramFreq[i].toString() + '\n';

    return salida;
}

void Language::sort() {
    for (int i = 0; i < _size; i++)
        for (int k = _size - 1; k > i; k--)
            if (_vectorBigramFreq[k] > _vectorBigramFreq[k - 1]) {
                BigramFreq bigramfreq_aux;
                bigramfreq_aux = _vectorBigramFreq[k - 1];
                _vectorBigramFreq[k - 1] = _vectorBigramFreq[k];
                _vectorBigramFreq[k] = bigramfreq_aux;
            }
}

void Language::save(const char fileName[]) const {
    std::ofstream salida;
    salida.open(fileName);
    if (!salida.is_open())
        throw std::ios_base::failure("No se pudo abrir el archivo");

    salida << MAGIC_STRING_T << std::endl << _languageId << std::endl << _size << std::endl << toString();
    salida << std::endl;
    salida.close();
}

void Language::saveBinary(const char fileName[]) const {
    std::ofstream salida;
    salida.open(fileName);
    if (!salida.is_open())
        throw std::ios_base::failure("No se pudo abrir el archivo");

    salida << MAGIC_STRING_B << std::endl << _languageId << std::endl << _size << std::endl;
    for (int i = 0; i < _size; i++)
        _vectorBigramFreq[i].serialize(salida);

    salida.close();
}

void Language::load(const char fileName[]) {

    liberarM();
    std::ifstream entrada;
    std::string magicString, languageId;
    std::string cadena;
    int size, freq;
    entrada.open(fileName);
    if (!entrada)
        throw std::ios_base::failure("No se pudo abrir el archivo.");

    entrada>>magicString;


    if (magicString != MAGIC_STRING_T && magicString != MAGIC_STRING_B)
        throw std::invalid_argument("No se encontro la cadena magica.");

    entrada >> languageId>>size;

    _languageId = languageId;
    asignarM(size);

    if (magicString == MAGIC_STRING_T)
        for (int i = 0; i < _size; i++)
            entrada >> _vectorBigramFreq[i];
    else
        for (int i = 0; i < _size; i++)
            _vectorBigramFreq[i].deserialize(entrada);

    entrada.close();
}

void Language::append(const BigramFreq& bigramFreq) {
    int resultado_busqueda = findBigram(bigramFreq.getBigram());
    if (resultado_busqueda == -1) {
        BigramFreq* aux = new BigramFreq[_size + 1];
        for (int i = 0; i < _size; i++)
            aux[i] = _vectorBigramFreq[i];
        aux[_size] = bigramFreq;
        liberarM();
        _vectorBigramFreq = aux;
        _size++;
    } else
        _vectorBigramFreq[resultado_busqueda].setFrequency(_vectorBigramFreq[resultado_busqueda].getFrequency() + bigramFreq.getFrequency());

}

BigramFreq& Language::operator[](const int &index) {
    return at(index);
}

const BigramFreq& Language::operator[](const int &index) const {
    return at(index);
}

Language& Language::operator+=(const Language& language) {
    if (language._languageId == _languageId)
        for (int i = 0; i < language._size; i++)
            this->append(language._vectorBigramFreq[i]);
    return *this;
}

std::ostream& operator<<(std::ostream &os, const Language &language) {
    os << language.toString();
    return os;
}

std::istream& operator>>(std::istream &is, Language &language) {

    std::string magicString, languageId;
    BigramFreq bigramFreq;
    int size;


    is>>magicString;

    if (magicString != language.getMagicStringT() && magicString != language.getMagicStringB())
        throw std::invalid_argument("No se encontro la cadena magica.");

    is >> languageId>>size;

    if (size < 0)
        throw std::out_of_range("Se leen un numero negativo de bigramas");


    Language aux(size);
    aux.setLanguageId(languageId);
    if (magicString == language.getMagicStringT())
        for (int i = 0; i < aux.getSize(); i++)
            is >> bigramFreq;
    else
        for (int i = 0; i < aux.getSize(); i++)
            bigramFreq.deserialize(is);

    language = aux;

    return is;
}

void Language::liberarM() {
    delete []_vectorBigramFreq;
    _vectorBigramFreq = nullptr;
}

void Language::asignarM(const int &tam) {
    if (_vectorBigramFreq == nullptr) {
        _size = tam;
        _vectorBigramFreq = new BigramFreq [_size];
    }
}