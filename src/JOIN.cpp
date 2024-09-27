/*
 * Metodología de la Programación: Language5
 * Curso 2022/2023
 */

/** 
 * @file JOIN.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * 
 * Created on 29 January 2023, 11:00
 */
#include "BigramCounter.h"
#include <cstring>
#include <fstream>

/**
 * Shows help about the use of this program in the given output stream
 * @param outputStream The output stream where the help will be shown (for example,
 * cout, cerr, etc) 
 */
void showEnglishHelp(std::ostream& outputStream) {
    outputStream << "Error, run with the following parameters:" << std::endl;
    outputStream << "JOIN [-t|-b] [-o <outputFile.bgr>] <file1.bgr> [<file2.bgr> ... <filen.bgr>] " << std::endl;
    outputStream << "       join the Language files <file1.bgr> <file2.bgr> ... into <outputFile.bgr>" << std::endl;
    outputStream << std::endl;
    outputStream << "Parameters:" << std::endl;
    outputStream << "-t|-b: text mode or binary mode for the output file (-t by default)" << std::endl;
    outputStream << "-o <outputFile.bgr>: name of the output file (output.bgr by default)" << std::endl;
    outputStream << "<file*.bgr>: each one of the files to be joined" << std::endl;
}

/**
 * This program reads an undefined number of Language objects from the files
 * passed as parameters to main(). It obtains as result the union of all the 
 * input Language objects. The result is then sorted by decreasing order of 
 * frequency and alphabetical order of bigrams when there is any tie in 
 * frequencies. Finally, the resulting Language is saved in an output file. The 
 * program must have at least an output file. 
 * Running example:
 * >  JOIN [-t|-b] [-o <outputFile.bgr>] <file1.bgr> [<file2.bgr> ... <filen.bgr>]
 * @param argc The number of command line parameters
 * @param argv The vector of command line parameters (cstrings)
 * @return 0 If there is no error; a value > 0 if error
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        showEnglishHelp(std::cerr);
        return 1;
    }
    std::string name_salida = "output.bgr";


    Language first;
    int primer_fichero = 1;
    bool binario = false;

    if (argv[1][0] == '-' && argv[1][1] == 'b' && strlen(argv[1])) {
        binario = true;
        primer_fichero++;
    } else if (argv[1][0] == '-' && argv[1][1] == 't' && strlen(argv[1]) == 2)
        primer_fichero++;

    else if ((argv[1][0] == '-' && argv[1][1] != 't' && argv[1][1] != 'b' && argv[1][1] != 'o')) {
        showEnglishHelp(std::cerr);
        return 1;
    }

    for (int i = 0; i < argc; i++)
        if (argv[i][0] == '-' && argv[i][1] == 'o' && strlen(argv[1]) == 2) {
            primer_fichero += 2;
            name_salida = argv[i + 1];
        }

    first.load(argv[primer_fichero]);
    for (int i = primer_fichero + 1; i < argc; i++) {
        Language aux;
        aux.load(argv[i]);
        first += aux;
    }

    char output_file[name_salida.length() + 1];
    for (int i = 0; i < name_salida.length(); i++)
        output_file[i] = name_salida[i];
    output_file[name_salida.length()] = '\0';
    first.sort();

    if (binario)
        first.saveBinary(output_file);
    else
        first.save(output_file);

    return 0;
}

