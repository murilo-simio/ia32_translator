#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>

#define WHITESPACE " "
#define NEWLINE "\n"

void PreProcessor(std::string, std::vector<std::string>*);
void FindEQU(std::vector<std::string>*, std::map <std::string, std::string>*);
void ProcessEquIf(std::vector<std::string>*, std::map <std::string, std::string>*, std::vector<std::string>*);
std::string WriteInstruction(int, std::string*);

#endif