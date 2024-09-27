/*
 * Metodología de la Programación: Language5
 * Curso 2022/2023
 */

/**
 * @file LEARN.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 *
 * Created on 29 January 2023, 11:00
 */
#include "../include/BigramCounter.h"
#include <cstring>
#include <fstream>

/**
 * Shows help about the use of this program in the given output stream
 * @param outputStream The output stream where the help will be shown (for example,
 * cout, cerr, etc)
 */
void showEnglishHelp(std::ostream& outputStream) {
    outputStream << "Error, run with the following parameters:" << std::endl;
    outputStream << "LEARN [-t|-b] [-l languageId] [-o outputFilename] <text1.txt> [<text2.txt> <text3.txt> .... ]" << std::endl;
    outputStream << "           learn the model for the language languageId from the text files <text1.txt> <text2.txt> <text3.txt> ..." << std::endl;
    outputStream << std::endl;
    outputStream << "Parameters:" << std::endl;
    outputStream << "-t|-b: text mode or binary mode for the output file (-t by default)" << std::endl;
    outputStream << "-l languageId: language identifier (unknown by default)" << std::endl;
    outputStream << "-o outputFilename: name of the output file (output.bgr by default)" << std::endl;
    outputStream << "<text1.txt> <text2.txt> <text3.txt> ....: names of the input files (at least one is mandatory)" << std::endl;
}

/**
 * This program learns a Language model from a set of input tex files (tex1.txt,
 * tex2.txt, ...
 * Running example:
 * > LEARN [-t|-b] [-l languageId] [-o outputFilename] <text1.txt> [<text2.txt> <text3.txt> ....]
 * @param argc The number of command line parameters
 * @param argv The vector of command line parameters (cstrings)
 * @return 0 If there is no error; a value > 0 if error
 */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        showEnglishHelp(std::cerr);
        return 1;
    }

    std::string languageId = "unknown";
    std::string outputName = "output.bgr";
    bool binario = false;
    int posicionId = 0, posicion_output = 0, posicionFicheros = 1;
    bool salir = false;

    for (int i = 0; i < argc; i++)
        if (argv[i][0] == '-' && argv[i][1] == 'b' && strlen(argv[i])) {
            binario = true;
            posicionFicheros++;
        }

    for (int i = 1; i < argc; i++)
        if (argv[i][0] == '-')
            if (strlen(argv[i]) != 2 || (argv[i][1] != 't' && argv[i][1] != 'b' && argv[i][1] != 'o' && argv[i][1] != 'l')) {
                showEnglishHelp(std::cerr);
                return 1;
            }


    for (posicionId = 0; posicionId < argc && salir == false; posicionId++)
        if (argv[posicionId][0] == '-' && argv[posicionId][1] == 'l' && strlen(argv[posicionId]) == 2) {
            languageId = argv[posicionId + 1];
            salir = true;
            posicionFicheros += 2;
        }

    salir = false;

    for (posicion_output = 0; posicion_output < argc && salir == false; posicion_output++)
        if (argv[posicion_output][0] == '-' && argv[posicion_output][1] == 'o' && strlen(argv[posicion_output]) == 2) {
            outputName = argv[posicion_output + 1];
            salir = true;
            posicionFicheros += 2;
        }


    BigramCounter resultado;
    for (int i = posicionFicheros; i < argc; i++) {
        resultado.calculateFrequencies(argv[i]);
    }

    Language salida = resultado.toLanguage();
    salida.setLanguageId(languageId);

    char output_file[outputName.length() + 1];
    for (int i = 0; i < outputName.length(); i++)
        output_file[i] = outputName[i];

    output_file[outputName.length()] = '\0';
    salida.sort();

    if (binario)
        salida.saveBinary(output_file);
    else
        salida.save(output_file);

    return 0;
}