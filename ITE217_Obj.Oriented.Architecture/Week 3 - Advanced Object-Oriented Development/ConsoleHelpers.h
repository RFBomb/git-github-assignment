// Robert Brenckman
// CSC-2214_003 ( Spring 2025 )
// 3/12/2025
//

#pragma once

#include<string>

const std::string ConsoleColor_Normal = "\033[0m";
const std::string ConsoleColor_Red = "\033[31m";
const std::string ConsoleColor_Green = "\033[32m";

void setColorError();

void setColorNormal();

void printErr(std::string err);

void ClearInputBuffer();

int getInt(std::string question, int min, int max);