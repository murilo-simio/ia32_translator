#include "preprocessor.hpp"

void PreProcessor(std::string file_name, std::vector<std::string>* pr_content) {
    std::map <std::string, std::string>* equ_table = new std::map<std::string, std::string>();
    std::vector<std::string>* file_content = new std::vector<std::string>();
    std::ifstream input_file;
    std::string line;
    std::string str;
    bool lab = false;
    
    // Abre o arquivo de entrada
    input_file.open(file_name);
    if(!input_file) {
        std::cout << "Nao foi possivel abrir o arquivo!" << std::endl;
        return ;
    }

    while(!input_file.eof()) {
        getline(input_file, line);

        // caso a linha contenha apenas rotulo, salva o rotulo para concatenar com o resto da instr
        // que deve estar na prox linha
        if(line[line.length() - 1] == ':') {
            lab = true;
            str = line;
            continue;
        }

        line = line.substr(0, line.find(";"));  // retira comentario

        if(line.length() > 0) {
            if(lab) {    // para o caso da linha conter apenas rotulo
                line = str + " " + line;
                lab = false;
            }

            file_content->push_back(line);
        }

    }

    input_file.close();

    FindEQU(file_content, equ_table);
    ProcessEquIf(file_content, equ_table, pr_content);

}

void ProcessEquIf(std::vector<std::string>* file_content, std::map <std::string, std::string>* equ_table, std::vector<std::string>* pr_content) {
    std::string token[10];
    std::string temp_str;
    char* line_token;
    char str[100];

    for(long unsigned int i = 0; i < file_content->size(); i++) {
        strcpy(str, (*file_content)[i].c_str());
        line_token = strtok(str, "\t ");

        if(!line_token) {
            continue;
        }

        // criacao dos tokens da linha
        int count_token = 0;
        while(line_token) {
            token[count_token] = line_token;
            count_token++; // armazena qtd de tokens criados
            line_token = strtok(NULL, "\t "); // aponta para proximo token da linha

        }

        // para qnd tiver espaco separando rotulo e ":", tipo "rot   :"
        if(token[1] == ":") {
            for(int j = 0; j < count_token; j++) {
                if(j == 0)
                    token[j] += token[j + 1];
                else
                    token[j] = token[j + 1];
            }
            count_token--;
        }

        temp_str = token[0];
        std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), ::toupper);
        if(temp_str == "IF") {
            // Desconsidera prox linha se a flag for 0
            if(equ_table->find(token[1])->second == "0")
                i++;
            continue;
        }

        std::string s;
        // garante que instr do tipo copy tenha 2 tokens => |copy| |end1,end2|
        if(temp_str == "COPY" || temp_str == "INPUT_S" || temp_str == "OUTPUT_S") {
            for (int j = 1; j < count_token; j++) {
                s += token[j];
            }
            token[1] = s;
            count_token = 2;
        }

        temp_str = token[1];
        std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), ::toupper);
        // trata instr com space e const que possua como arg EQU       
        if (temp_str == "SPACE" || temp_str == "CONST")
            if((*equ_table).find(token[2]) != (*equ_table).end())
                token[2] = (*equ_table).find(token[2])->second;

        // para instr de copy que tem rotulo, garante 3 tokens => |rotulo:| |copy| |end1,end2|
        if(temp_str == "COPY" || temp_str == "INPUT_S" || temp_str == "OUTPUT_S") {
            for (int j = 2; j < count_token; j++) {
                s += token[j];
            }
            token[2] = s;
            count_token = 3;
        }

        pr_content->push_back(WriteInstruction(count_token, token));

    }

    std::cout << "Fim do pre-processamento\n\n";

}

void FindEQU(std::vector<std::string>* file_content, std::map <std::string, std::string>* equ_table) {
    std::string label;
    std::string value;
    std::string token[4];
    std::string temp_str;
    char* line_token;
    char str[100];

    for(long unsigned int i = 0; i < file_content->size(); i++) {

        strcpy(str, (*file_content)[i].c_str());
        line_token = strtok(str, "\t ");

        // criacao dos tokens da linha
        int count_token = 0;
        while(line_token) {
            token[count_token] = line_token;
            count_token++; // qtd de tokens criados
            line_token = strtok(NULL, "\t "); // aponta para proximo token da linha
        }

        temp_str = token[1];
        std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), ::toupper);
        if(temp_str != "EQU") {
            // Remove as instrucoes de definicao de EQU do vetor
            (*file_content).erase((*file_content).begin(), (*file_content).begin() + i);
            return ;
        }

        temp_str = token[1];
        std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), ::toupper);
        if(temp_str == "EQU") {
            token[0] = token[0].substr(0, token[0].find(":"));  // desconsidera ":" no final do rotulo
            label = token[0];
            value = token[2];
            (*equ_table)[label] = value;
        }

    }

}


std::string WriteInstruction(int j, std::string* token) {
    std::string inst;

    switch (j) {
        case 1:
            inst = token[0] + NEWLINE;
            break;
        case 2:
            inst = token[0] + WHITESPACE + token[1] + NEWLINE;
            break;
        case 3:
            inst = token[0] + WHITESPACE + token[1] + WHITESPACE + token[2] + NEWLINE;
            break;
    }

    return inst;

}