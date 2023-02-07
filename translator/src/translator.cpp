#include "translator.hpp"

using namespace std;

map<std::string, int> operacoes = 
{
    {"ADD",       1},   
    {"SUB",       2},
    {"MUL",       3},
    {"DIV",       4},
    {"JMP",       5},
    {"JMPN",      6},
    {"JMPP",      7},
    {"JMPZ",      8},
    {"COPY",      9},
    {"LOAD",     10},
    {"STORE",    11},
    {"INPUT",    12},
    {"OUTPUT",   13},
    {"STOP",     14},
    {"INPUT_C",  15},
    {"OUTPUT_C", 16},
    {"INPUT_S",  17},
    {"OUTPUT_S", 18}
};

void Translator(vector<string>* pr_content) {
    vector<string> tokens;

    for(long unsigned int i = 0; i < pr_content->size(); i++) {
        istringstream str((*pr_content)[i]);
        string token;
        bool is_copy_instr = false;
        int op = -1;

        tokens.clear();

        // Copia os tokens para o vetor de tokens
        while(str >> token) {
            if(operacoes.find(upper(token)) != operacoes.end()) {
                op = operacoes[upper(token)]; // captura tipo de instr de cada linha
            }

            if(upper(token) == "COPY") {   // instr do tipo copy recebe outro tratamento 
                is_copy_instr = true;
                tokens.push_back(upper(token));
                continue;         
            }
            
            if(is_copy_instr) {
                string antes = processa_primeiro_arg(token, ',');
                string depois = processa_segundo_arg(token, ',');
                tokens.push_back(upper(antes));
                tokens.push_back(upper(depois)); 
                continue;
            }
            tokens.push_back(upper(token));     // Transforma tudo em maiuscula

        }

        switch(op) {
            case 1: // ADD
                break;
            case 2: // SUB
                break;
            case 3: // MUL
                break;
            case 4: // DIV
                break;
            case 5: // JMP
                break;
            case 6: // JMPN
                break;
            case 7: // JMPP
                break;
            case 8: // JMPZ
                break;
            case 9: // COPY
                break;
            case 10: // LOAD
                break;
            case 11: // STORE
                break;
            case 12: // INPUT
                break;
            case 13: // OUTPUT
                break;
            case 14: // STOP
                break;
            case 15: // INPUT_C
                break;
            case 16: // OUTPUT_C
                break;
            case 17: // INPUT_S
                break;
            case 18: // OUTPUT_S
                break;
            case -1:
                //  Trata space e const
                if(tokens.at(0) != "SECTION") {
                    
                }
                break;
        }

    }

}

string upper(string s){
    for(unsigned i=0;i<s.length();i++){
        s[i] = toupper(s[i]);
    }
    return s;
}

string processa_primeiro_arg(string const& s, char t){
    string::size_type pos = s.find(t);
    if(pos != string::npos){
        return s.substr(0,pos);
    }else{
        return s;
    }
}

string processa_segundo_arg(string const& s, char t){
    string::size_type pos = s.find(t);
    if(pos != string::npos){
        return s.substr(pos+1);
    }else{
        return s;
    }
}