#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>

void Translator(std::vector<std::string>*, std::string);
std::string processa_primeiro_arg(std::string const& s, char t);
std::string processa_segundo_arg(std::string const& s, char t);
std::string upper(std::string s);

#endif