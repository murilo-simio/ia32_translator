#include <iostream>
#include "preprocessor.hpp"
#include "translator.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    string input_file; // contem extensao
    vector<string>* pr_content = new vector<string>(); // armazena conteudo pre-processado

    if(argc != 2) {
        cout << "Quantidade invalida de argumentos!" << endl;
        cout << "Eh necessario inserir apenas o arquivo .asm" << endl;
        return 0;
    }

    input_file = argv[1];

    PreProcessor(input_file, pr_content);
    Translator(pr_content);

    return 0;
}