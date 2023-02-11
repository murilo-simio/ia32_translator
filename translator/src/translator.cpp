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

void Translator(vector<string>* pr_content, string file_name) {
    vector<string> tokens;
    vector<string>* section_bss  = new vector<string>();
    vector<string>* section_data = new vector<string>();
    vector<string>* section_text = new vector<string>();

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
        
        // realiza conversao para ia32
        string instr;
        switch(op) {
            case 1: // ADD
                // add eax, [op1]
                if(tokens.at(0).back() == ':') {  // quando tem rotulo
                    instr = tokens.at(0) + " " + tokens.at(1) + " EAX, [" + tokens.at(2) + "]\n";
                } else {
                    instr = tokens.at(0) + " EAX, [" + tokens.at(1) + "]\n";
                }
                break;
            case 2: // SUB
                // sub eax, [op1]
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " " + tokens.at(1) + " EAX, [" + tokens.at(2) + "]\n";
                } else {
                    instr = tokens.at(0) + " EAX, [" + tokens.at(1) + "]\n";
                }
                break;
            case 3: // MUL
                break;
            case 4: // DIV
                break;
            case 5: // JMP
                // jmp label
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " " + tokens.at(1) + " " + tokens.at(2) + "\n";
                } else {
                    instr = tokens.at(0) + " " + tokens.at(1) + "\n";
                }
                break;
            case 6: // JMPN
                // cmp eax, 0
	            // jl label
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " CMP EAX, 0\n";
                    instr += "jl " + tokens.at(2) + "\n";
                } else {
                    instr = "CMP EAX, 0\n";
                    instr += "JL " + tokens.at(1) + "\n";
                }
                break;
            case 7: // JMPP
                // cmp eax, 0
	            // jg label
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " CMP EAX, 0\n";
                    instr += "JG " + tokens.at(2) + "\n";
                } else {
                    instr = "CMP EAX, 0\n";
                    instr += "JG " + tokens.at(1) + "\n";
                }
                break;
            case 8: // JMPZ
                // cmp eax, 0
	            // jz label
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " CMP EAX, 0\n";
                    instr += "JZ " + tokens.at(2) + "\n";
                } else {
                    instr = "CMP EAX, 0\n";
                    instr = "JZ " + tokens.at(1) + "\n";
                }
                break;
            case 9: // COPY
                // mov ebx, [op1]
	            // mov [op2], ebx
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " MOV EBX, [" + tokens.at(2) + "]\n";
                    instr += "[" + tokens.at(3) + "], ebx\n";
                } else {
                    instr = "MOV EBX, [" + tokens.at(1) + "]\n";
                    instr += "MOV [" + tokens.at(2) + "], EBX\n";
                }
                break;
            case 10: // LOAD
                // mov eax, [op1]
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " MOV EAX, [" + tokens.at(2) + "]\n";
                } else {
                    instr = "MOV EAX, [" + tokens.at(1) + "]\n";
                }
                break;
            case 11: // STORE
                // mov dword [op1], eax
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " MOV DWORD [" + tokens.at(2) + "], EAX\n";
                } else {
                    instr = "MOV DWORD [" + tokens.at(1) + "], EAX\n";
                }
                break;
            case 12: // INPUT
                break;
            case 13: // OUTPUT
                break;
            case 14: // STOP
                // mov eax, 1
	            // mov ebx, 0
	            // int 80h
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " MOV EAX, 1\n";
                } else {
                    instr = "MOV EAX, 1\n";
                }
                instr += "MOV EBX, 0\n";
                instr += "INT 80H\n";
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
                tokens.at(0).pop_back();
                if(tokens.at(1) == "SPACE"){
                    if(tokens.size() == 3) { // com arg
                        string::size_type pos = tokens.at(2).find('+');
                        if(pos != string::npos){ // arg com rotulo
                            string size = processa_segundo_arg(tokens.at(2), '+'); // falta somar 1
                            instr = tokens.at(0) + " RESD " + size + "\n";
                        } else { // so arg
                            instr = tokens.at(0) + " RESD " + tokens.at(2) + "\n";
                        }
                    } else { // sem arg
                        instr = tokens.at(0) + " RESD 1\n";
                    }
                    section_bss->push_back(instr);

                } else if (tokens.at(1) == "CONST") {
                    instr = tokens.at(0) + " DB " + tokens.at(2) + "\n";
                    section_data->push_back(instr);
                }
                break;
        }

        if(op >= 1 && op <= 18) {
            section_text->push_back(instr);
        }

    }

    // Criacao do arquivo de saida .s
    ofstream output_file(file_name.c_str(), ofstream::out);

    if(!output_file.good()){
        cout << "Problema ao ler arquivo de saida!" << endl;
        return;
    }

    // Criacao de Section .Data
    output_file << "SECTION .DATA\
                    \nMSG1 DB 'Quantidade de Bytes lidos/escritos = '\
                    \nMSG1_SIZE EQU $-MSG1\
                    \nNWLN DB 0DH, 0AH\
                    \nNWLN_SIZE EQU $-NWLN\n";
    for(long unsigned int i = 0; i < section_data->size(); i++) {
        output_file << (*section_data)[i];
    }

    // Criacao de Section .BSS
    output_file << "\nSECTION .BSS\n";
    for(long unsigned int i = 0; i < section_bss->size(); i++) {
        output_file << (*section_bss)[i];
    }

    // Criacao de Section .Text
    output_file << "\nSECTION .TEXT\nGLOBAL _START\n_START: ";
    for(long unsigned int i = 0; i < section_text->size(); i++) {
        output_file << (*section_text)[i];
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