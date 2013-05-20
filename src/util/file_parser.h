#ifndef FILE_PARSER_H_
#define FILE_PARSER_H_

#include <cstdio>
#include <cctype>
#include <vector>
#include <string>
#include <wx/wx.h>
#include "typedefs.h"
#include "../mips/memory.h"

int parseInstFile(const wxString& filename, Memory* memory,
					std::vector<std::string>& instDesc);
bool parseInputFile(const wxString& filename, Memory* memory);

#endif /* FILE_PARSER_H_ */
