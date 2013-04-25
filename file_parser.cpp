#include "file_parser.h"

bool parseInputFile(const wxString& filename, Memory* memory){
	FILE* file = fopen(filename.mb_str(),"r");
	if (file==NULL) return false;
	int x,y;
	for (int i=0; i<1000; i++){
		if (fscanf(file," %d %d",&x,&y)<2) return false;
		memory->write(1000+4*i,x);
		memory->write(5000+4*i,y);
	}
	fclose(file);
	return true;
}

bool readLine(FILE* file, std::vector<char>& buf){
	char c;
	buf.clear();
	while ((c=fgetc(file))!=EOF){
		if (c=='\n') break;
		buf.push_back(c);
	}
	return c!=EOF;
}

bool parseInstFile(const wxString& filename, Memory* memory,
						std::vector<std::string>& instDesc){
	FILE* file = fopen(filename.mb_str(),"r");
	if (file==NULL) return false;
	instDesc.clear();
	// HACK: using vector as variable length buffer
	// to read files without C++'s broken streams
	std::vector<char> buffer;
	int instPos=0;
	while (readLine(file,buffer)){
		if (buffer.empty()) continue;
		u32 inst=0;
		int i;
		for (i=0; i<32; i++){
			if (buffer[i]!='0' && buffer[i]!='1'){
				printf("%d\n",instPos);
				return false;
			}
			inst=2*inst+buffer[i]-'0';
		}
		memory->write(instPos*4,inst);
		instDesc.push_back("");
		while (i<buffer.size() && isspace(buffer[i])) i++;
		if (i==buffer.size()) continue;
		else if (buffer[i]!=';') {
			return false;
		}
		else {
			++i;
			while (i<buffer.size() && isspace(buffer[i])) i++;
			for (;i<buffer.size(); i++) instDesc[instPos]+=buffer[i];
		}
		instPos++;
	}
	return true;
}
