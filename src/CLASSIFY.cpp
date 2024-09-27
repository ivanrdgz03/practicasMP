/*
 * Metodología de la Programación: Language5
 * Curso 2022/2023
 */

/** 
 * @file CLASSIFY.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * 
 * Created on 29 January 2023, 11:00
 */
#include "BigramCounter.h"

/**
 * Shows help about the use of this program in the given output stream
 * @param outputStream The output stream where the help will be shown (for example,
 * cout, cerr, etc) 
 */
void showEnglishHelp(std::ostream& outputStream) {
    outputStream << "Error, run with the following parameters:" << std::endl;
    outputStream << "CLASSIFY <text.txt> <lang1.bgr> [<lang2.bgr> <lang3.bgr> ....]" << std::endl;
    outputStream << "          Obtains the identifier of the closest language to the input text file" << std::endl;
    outputStream << std::endl;
}

/**
 * This program print the language identifier of the closest language 
 * for an input text file (<text.txt>) among the set of provided models:
 * <lang1.bgr>, <lang2.bgr>, ...
 * 
 * Running example:
 * > CLASSIFY  <text.txt> <lang1.bgr> [<lang2.bgr> <lang3.bgr> ...]
 * @param argc The number of command line parameters
 * @param argv The vector of command line parameters (cstrings)
 * @return 0 If there is no error; a value > 0 if error
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        showEnglishHelp(std::cerr);
        return 1;
    }
    BigramCounter bigramcount;
    bigramcount.calculateFrequencies(argv[1]);

    Language language = bigramcount.toLanguage(), primero;
    int pos_ganador = 2;
    primero.load(argv[pos_ganador]);
    double dist_ganadora = language.getDistance(primero);
    for (int i = pos_ganador + 1; i < argc; i++) {
        Language aux;
        aux.load(argv[i]);
        if (dist_ganadora >= language.getDistance(aux)) {
            pos_ganador = i;
            dist_ganadora = language.getDistance(aux);
        }
    }
    Language ganador;
    ganador.load(argv[pos_ganador]);
    language.setLanguageId(ganador.getLanguageId());

    std::cout << "Final decision: language " << language.getLanguageId() << " with a distance of " << dist_ganadora << std::endl;
    return 0;
}

