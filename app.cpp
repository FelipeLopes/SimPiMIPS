#include <cstdio>

#include "memory.h"
#include "cpu.h"
#include "app.h"
#include "main_window.h"

IMPLEMENT_APP(App);

int teste[] = {
		0b00100000000000100000000000000010, // LI R2,2 ; A
		0b00100000000001010000001111101000, // LI R5, 1000 ; X
		0b00100000000001100001001110001000, // LI R6, 5000 ; Y
		0b00100000000000010000000000000000, // LI R1, 0 ; I
		0b00100000000001110000001111101000, // LI R7, 1000
											// Loop 1000 vezes
		0b10001100101011110000000000000000, // LW R15, 0(R5)
		0b00000000010011110111100000011000, // MUL R15, R15, R2
		0b10001100110100000000000000000000, // LW R16, 0(R6)
		0b00000001111100001000000000100000, // ADD R16, R15, R16
		//0b00000001111100001000000000100010, // SUB R16, R15, R16
		0b10101100110100000000000000000000, // SW R16, 0(R6)
		0b00100000001000010000000000000001, // ADDI R1, R1, 1
		0b00100000101001010000000000000100, // ADDI R5, R5, 4
		0b00100000110001100000000000000100, // ADDI R6, R6, 4
		0b00011100001001110000000000010100  // BLE R1, R7, Loop
		//0b00010000001001111111111111011100  // BNE R1, R7, Loop
};

int x[1000],y[1000];

Memory instMem,dataMem;
Register reg;
CPU cpu(&reg,&instMem,&dataMem,true);

bool App::OnInit(){

for (int i=0; i<14; i++) instMem.write(i*4,teste[i]);
reg.writePC(0);

FILE* file = fopen("input.txt","r");
if (file==NULL){
printf("Could not open file");
exit(1);
}

for (int i=0; i<1000; i++){
fscanf(file," %d %d",&x[i],&y[i]);
}

for (int i=0; i<1000; i++){
dataMem.write(1000+4*i,x[i]);
dataMem.write(5000+4*i,y[i]);
}
	MainWindow* mainWindow = new MainWindow(&cpu);
	mainWindow->Show(true);
	SetTopWindow(mainWindow);
	return true;
}
