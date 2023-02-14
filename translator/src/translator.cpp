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
                    instr = tokens.at(0) + " add eax, [" + tokens.at(2) + "]\n";
                } else {
                    instr = "add eax, [" + tokens.at(1) + "]\n";
                }
                break;
            case 2: // SUB
                // sub eax, [op1]
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " sub eax, [" + tokens.at(2) + "]\n";
                } else {
                    instr = "sub eax, [" + tokens.at(1) + "]\n";
                }
                break;
            case 3: // MUL
                // mov ebx, [op1]
	            // imul ebx
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " mov ebx, [" + tokens.at(2) + "]\nimul ebx\n";
                } else {
                    instr = "mov ebx, [" + tokens.at(1) + "]\nimul ebx\n";
                }
                break;
            case 4: // DIV
                // mov ebx, [op1]
	            // idiv ebx
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " mov ebx, [" + tokens.at(2) + "]\nidiv ebx\n";
                } else {
                    instr = "mov ebx, [" + tokens.at(1) + "]\nidiv ebx\n";
                }
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
                    instr = tokens.at(0) + " cmp eax, 0\n";
                    instr += "jl " + tokens.at(2) + "\n";
                } else {
                    instr = "cmp eax, 0\n";
                    instr += "jl " + tokens.at(1) + "\n";
                }
                break;
            case 7: // JMPP
                // cmp eax, 0
	            // jg label
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " cmp eax, 0\n";
                    instr += "jg " + tokens.at(2) + "\n";
                } else {
                    instr = "cmp eax, 0\n";
                    instr += "jg " + tokens.at(1) + "\n";
                }
                break;
            case 8: // JMPZ
                // cmp eax, 0
	            // jz label
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " cmp eax, 0\n";
                    instr += "jz " + tokens.at(2) + "\n";
                } else {
                    instr = "cmp eax, 0\n";
                    instr = "jz " + tokens.at(1) + "\n";
                }
                break;
            case 9: // COPY
                // mov ebx, [op1]
	            // mov [op2], ebx
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " mov ebx, [" + tokens.at(2) + "]\n";
                    instr += "[" + tokens.at(3) + "], ebx\n";
                } else {
                    instr = "mov ebx, [" + tokens.at(1) + "]\n";
                    instr += "mov [" + tokens.at(2) + "], ebx\n";
                }
                break;
            case 10: // LOAD
                // mov eax, [op1]
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " mov eax, [" + tokens.at(2) + "]\n";
                } else {
                    instr = "mov eax, [" + tokens.at(1) + "]\n";
                }
                break;
            case 11: // STORE
                // mov dword [op1], eax
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " mov dword [" + tokens.at(2) + "], eax\n";
                } else {
                    instr = "mov dword [" + tokens.at(1) + "], eax\n";
                }
                break;
            case 12: // INPUT
                instr = "push eax\npush dword " + tokens.at(1) + "\ncall INPUT\npop eax\n";
                break;
            case 13: // OUTPUT
                if(tokens.at(0).back() == ':') { // nao funciona
                    instr = tokens.at(0) + " push eax\npush dword " + tokens.at(2) + "\ncall OUTPUT\npop eax\n";
                } else {
                    instr = "push eax\npush dword [" + tokens.at(1) + "]\ncall OUTPUT\npop eax\n";
                }
                break;
            case 14: // STOP
                // mov eax, 1
	            // mov ebx, 0
	            // int 80h
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " mov eax, 1\n";
                } else {
                    instr = "mov eax, 1\n";
                }
                instr += "mov ebx, 0\n";
                instr += "int 80h\n";
                break;
            case 15: // INPUT_C
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " push eax\npush dword " + tokens.at(2) + "\ncall INPUT_C\npop eax\n";
                } else {
                    instr = "push eax\npush dword " + tokens.at(1) + "\ncall INPUT_C\npop eax\n";
                }
                break;
            case 16: // OUTPUT_C
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " push eax\npush dword " + tokens.at(2) + "\ncall OUTPUT_C\npop eax\n";
                } else {
                    instr = "push eax\npush dword " + tokens.at(1) + "\ncall OUTPUT_C\npop eax\n";
                }
                break;
            case 17: // INPUT_S LABEL,QTD_BYTES
            {
                int token_size = tokens.size() - 1;
                string label = processa_primeiro_arg(tokens.at(token_size), ',');
                string bytes = processa_segundo_arg(tokens.at(token_size), ',');
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " push eax\npush word " + bytes + "\npush dword " + label + "\ncall INPUT_S\npop eax\n";
                } else {
                    instr = "push eax\npush word " + bytes + "\npush dword " + label + "\ncall INPUT_S\npop eax\n";
                }
                break;
            }
            case 18: // OUTPUT_S LABEL,QTD_BYTES
            {
                int token_size = tokens.size() - 1;
                string label = processa_primeiro_arg(tokens.at(token_size), ',');
                string bytes = processa_segundo_arg(tokens.at(token_size), ',');
                if(tokens.at(0).back() == ':') {
                    instr = tokens.at(0) + " push eax\npush word " + bytes + "\npush dword " + label + "\ncall OUTPUT_S\npop eax\n";
                } else {
                    instr = "push eax\npush word " + bytes + "\npush dword " + label + "\ncall OUTPUT_S\npop eax\n";
                }
                break;
            }
            case -1:
                //  Trata space e const
                tokens.at(0).pop_back();
                if(tokens.at(1) == "SPACE"){
                    if(tokens.size() == 3) { // com arg
                        string::size_type pos = tokens.at(2).find('+');
                        if(pos != string::npos){ // arg com rotulo
                            string size = processa_segundo_arg(tokens.at(2), '+');
                            instr = tokens.at(0) + " resd " + size + "+1\n";
                        } else { // so arg
                            instr = tokens.at(0) + " resd " + tokens.at(2) + "\n";
                        }
                    } else { // sem arg
                        instr = tokens.at(0) + " resd 1\n";
                    }
                    section_bss->push_back(instr);

                } else if (tokens.at(1) == "CONST") {
                    instr = tokens.at(0) + " db " + tokens.at(2) + "\n";
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

    string instr;
    instr = "mensagem1 db 'Quantidade de Bytes lidos/escritos = '";
    instr += "\nSIZEM1 EQU $-mensagem1";
    instr += "\nmensagem2 db '!', 0Dh, 0Ah";
    instr += "\nSIZEM2 EQU $-mensagem2";
    instr += "\nnewLine db 0Dh, 0Ah";
    instr += "\nSIZENEWLINE EQU $-newLine\n";
    section_data->push_back(instr);

    output_file << "section .data\n";
    for(long unsigned int i = 0; i < section_data->size(); i++) {
        output_file << (*section_data)[i];
    }

    // Criacao de Section .BSS
    output_file << "\nsection .bss\n";
    for(long unsigned int i = 0; i < section_bss->size(); i++) {
        output_file << (*section_bss)[i];
    }

    // MSGOUT
    instr = "\nMSGOUT:\npush ebp\nmov ebp, esp\npush ebx\npush ecx\npush edx\npush esi\nmov eax, 4\nmov ebx, 1\nmov ecx, mensagem1\nmov edx, SIZEM1\nint 80h\nmov eax, [ebp+8]\nsub esp, 12\n";
    instr += "mov ecx, esp\npush eax\npush ecx\ncall DEC2HEX\nadd esp, 12\nmov edx, eax\nmov eax, 4\nmov ebx, 1\nint 80h\nmov eax, 4\nmov ebx, 1\nmov ecx, mensagem2\nmov edx, SIZEM2\nint 80h\n";
    instr += "pop esi\npop edx\npop ecx\npop ebx\npop ebp\nret 4\n\n";
    section_text->push_back(instr);
    // CONTA
    instr = "DEC2HEX:\npush ebp\nmov ebp, esp\npush ebx\npush ecx\npush edx\npush esi\nmov eax, [ebp+12]\nmov esi, [ebp+8]\nmov byte [esi], 0x0\npush esi\ncmp eax, 0\njge DEC2S\npop ebx\nmov byte [esi], 0x2d\n";
    instr += "inc esi\npush esi\nmov byte [esi], 0x0\nneg eax\n\n";
    section_text->push_back(instr);
    // DEC2S
    instr = "DEC2S:\ncdq\nmov ebx, 10\nidiv ebx\nadd dl, 0x30\nCONTALOOP:\nmov cl, [esi]\ncmp cl, 0\njz CONTAFIM\nmov byte [esi], dl\nmov dl, cl\ninc esi\njmp CONTALOOP\n\n";
    instr += "CONTAFIM:\nmov byte [esi], dl\ninc esi\nmov byte [esi], 0x0\npop esi\npush esi\ncmp eax, 0\njz CONTA3\njmp DEC2S\nCONTA3:\npop esi\nmov eax, 0\nmov esi, [ebp+8]\n\n";
    instr += "CONTA3LOOP:\ncmp byte [esi], 0x0\njz CONTA3FIM\ninc eax\ninc esi\njmp CONTA3LOOP\n\nCONTA3FIM:\npop esi\npop edx\npop ecx\npop ebx\npop ebp\nret 8\n\n";
    section_text->push_back(instr);
    // INPUT
    instr = "INPUT:\npush ebp\nmov ebp, esp\nsub esp, 12\nmov ecx, esp\nmov eax, 3\nmov ebx, 0\nmov edx, 12\nint 80h\npush eax\npush eax\ncall MSGOUT";
    instr += "\npop eax\npush eax\nsub ebx, ebx\nmov edx, ecx\nmov ecx, eax\nxor esi, esi\nxor eax, eax\nmov bl, [edx]\ndec ecx\ncmp bl, 0x2d\njne IN_COUNT\nmov esi, 1\ninc edx\ndec ecx\n";
    instr += "\nIN_COUNT:\nmov bl, [edx]\ninc edx\nsub bl, 0x30\npush ebx\nshl eax, 1\nmov ebx, eax\nshl eax, 2\nadd eax, ebx\npop ebx\nadd eax, ebx\nloop IN_COUNT\ncmp esi, 0\njz IN_FIM\nneg eax\n";
    instr += "\nIN_FIM:\nmov ebx, [ebp+8]\nmov [ebx],eax\npop eax\nadd esp,12\npop ebp\nret 4\n\n";
    section_text->push_back(instr);
    // INPUT C
    instr = "INPUT_C:\npush ebp\nmov ebp, esp\nmov eax, 3\nmov ebx, 0\nmov ecx, [ebp+8]\nmov edx, 1\nint 80h\npush eax\npush eax\ncall MSGOUT\npop eax\npop ebp\nret\n\n";
    section_text->push_back(instr);
    //INPUT STRING
    instr = "INPUT_S:\npush ebp\nmov ebp, esp\nxor edx, edx\nmov eax, 3\nmov ebx, 0\nmov ecx, [ebp+10]\nmov edx, [ebp+8]\nint 80h\npush eax\npush eax\ncall MSGOUT\npop eax\npop ebp\nret 10\n\n";
    section_text->push_back(instr);
    // OUTPUT_S
    instr = "OUTPUT_S:\npush ebp\nmov ebp, esp\nxor edx, edx\nmov eax, 4\nmov ebx, 1\nmov ecx, [ebp+10]\nmov edx, [ebp+8]\nint 80h\npush eax\npush eax\ncall MSGOUT\npop eax\npop ebp\nret 10\n\n";
    section_text->push_back(instr);
    // OUTPUT
    instr = "OUTPUT:\npush ebp\nmov ebp, esp\nmov eax, [ebp+8]\nsub esp, 12\nmov ecx, esp\npush eax\npush ecx\ncall DEC2HEX\npush eax\npush eax\nmov edx, eax\nmov eax, 4\nmov ebx, 1\nint 80h\nmov eax, 4\n";
    instr += "mov ebx, 1\nmov ecx, newLine\nmov edx, SIZENEWLINE\nint 80h\ncall MSGOUT\npop eax\nadd esp, 12\npop ebp\nret 4\n\n";
    section_text->push_back(instr);
    // OUTPUT_C
    instr = "OUTPUT_C:\npush ebp\nmov ebp, esp\nxor edx, edx\nmov eax, 4\nmov ebx, 1\nmov ecx, [ebp+8]\nmov edx, 1\nint 80h\nmov eax, 4\nmov ebx, 1\nmov ecx, newLine\nmov edx, SIZENEWLINE\nint 80h\npush eax\npush eax\ncall MSGOUT\npop eax\npop ebp\nret 4\n\n";
    section_text->push_back(instr);

    // Criacao de Section .Text
    output_file << "\nsection .text\nglobal _start\n_start:\n";
    for(long unsigned int i = 0; i < section_text->size(); i++) {
        output_file << (*section_text)[i];
    }

    cout << "Conversao finalizada!\n\n";

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