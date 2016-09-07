#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define TYPE10 6
#define TYPE11 9
#define TYPE20 2
#define TYPE21 5
#define TYPE30 7
#define TYPE31 7
#define TYPE4 2
#define MAX 16
#define REGN 32
#define LABELN 128

typedef unsigned int instruction; //The instruction has 32 bits 

struct labelStruct {
  char labelName[64];
  int memPosition;
};

struct _REG {
  char regDef[4];
  int regNum;
};

struct _INSTR {
  char *instrName;
  int opCode;
};

struct _INSTRCYCLE {
  int opCode, cycle;
};

typedef struct reservationStation {
  instruction opCode;
  int Qi, Qj, Vi, Vj, mAddress;
  int busy, refInstrCount;
} reservationStation;

int instrRef[REGN];

int regBank[REGN];

typedef struct decodStruct {
  instruction OpCode, operand1, operand2, operand3;
} decodStruct;

typedef struct functionalUnit {
  instruction opCode;
  int busy, currentCycle, operand1, operand2, address, result, refInstrCount;
}	fUnit;

typedef struct resultList {
  instruction opResult, idInstr, opCode;		
} resultList;

const struct _INSTR instrList[REGN] = {
  {"LD", 0}, {"ST", 1}, {"MOVE", 2}, {"NEG", 3},
  {"NOT", 4}, {"ADD", 5}, {"SUB", 6}, {"MULT", 7}, 
  {"DIV", 8},	{"SSL", 11}, {"SLR", 12}, {"AND", 9},
  {"OR", 10}, {"LI", 16}, {"BEQZ", 17}, {"BNEZ", 18},
  {"BGTZ", 19},	{"BLEZ", 20}, {"ADDI", 32}, {"SUBI", 33},
  {"MULTI", 34}, {"DIVI", 35}, {"ANDI", 36}, {"ORI", 37},
  {"BEQ", 38}, {"BNE", 39}, {"BGT", 40}, {"BGE", 41},
  {"BLT", 42}, {"BLE", 43}, {"B", 48}, {"NULL", -1}
};

const struct _INSTR instrType10[TYPE10] = { //2 registers
  {"LD", 0}, {"ST", 1}, {"MOVE", 2}, {"NEG", 3},
  {"NOT", 4}, {"NULL", -1}
};

const struct _INSTR instrType11[TYPE11] = { //3 registers
  {"ADD", 5}, {"SUB", 6}, {"MULT", 7}, {"DIV", 8}, 
  {"SSL", 11}, {"SLR", 12}, {"AND", 9}, {"OR", 10},
  {"NULL", -1}
};

const struct _INSTR instrType20[TYPE20] = { //Register and imediate
  {"LI", 16}, {"NULL", -1}
};

const struct _INSTR instrType21[TYPE21] = { //Register and label
  {"BEQZ", 17}, {"BNEZ", 18}, {"BGTZ", 19},
  {"BLEZ", 20}, {"NULL", -1}
};

const struct _INSTR instrType30[TYPE30] = {
  {"ADDI", 32}, {"SUBI", 33}, {"MULTI", 34}, {"DIVI", 35},
  {"ANDI", 36}, {"ORI", 37}, {"NULL", -1}
};

const struct _INSTR instrType31[TYPE31] = {
  {"BEQ", 38}, {"BNE", 39}, {"BGT", 40}, {"BGE", 41},
  {"BLT", 42}, {"BLE", 43}, {"NULL", -1}
};

const struct _INSTR instrType4[TYPE4] = {
  {"B", 48}, {"NULL", -1}
};

const struct _REG regMap[REGN] = {
  {"R1", 0}, {"R2", 1}, {"R3", 2}, {"R4", 3},
  {"R5", 4}, {"R6", 5}, {"R7", 6}, {"R8", 7},
  {"R9", 8}, {"R10", 9}, {"R11", 10}, {"R12", 11},
  {"R13", 12}, {"R14", 13}, {"R15", 14}, {"R16", 15},
  {"R17", 16}, {"R18", 17}, {"R19", 18}, {"R20", 19},
  {"R21", 20}, {"R22", 21}, {"R23", 22}, {"R24", 23},
  {"R25", 24}, {"R26", 25}, {"R27", 26}, {"R28", 27},
  {"R29", 28}, {"R30", 29}, {"R31", 30}, {"R32", 31}
};

struct _INSTRCYCLE instrCycles[REGN] = {
  {0, 0}, {1, 0}, {2, 0}, {3, 0},
  {4, 0}, {9, 0}, {10, 0}, {5, 0},
  {6, 0}, {7, 0}, {8, 0}, {11, 0},
  {12, 0}, {16, 0}, {17, 0}, {18, 0},
  {19, 0}, {20, 0}, {32, 0}, {33, 0},
  {34, 0}, {35, 0}, {36, 0}, {37, 0},
  {38, 0}, {39, 0}, {40, 0}, {41, 0},
  {42, 0}, {43, 0}, {48, 0}, {-1, -1}
};

struct labelStruct labelList[LABELN];  //Labels list
int nextLabel = 0; //Next label position in the list

//INSTRUCTIONS
int notInstruction(int i) {
  int mask = 0xffffffff ^ i;
  return mask;
}

int negInstruction(int i) {
  return (-1) * i;
}

int andInstruction(int i, int j) {
  int mask = i & j;
  return mask;
}

int orInstruction(int i, int j) {
  int mask = i | j;
  return mask;
}

int beqInstruction(int i, int j) {
  return (i == j) ? 1 : 0;
}

int beqzInstruction(int i) {
  return (i == 0) ? 1 : 0; 
}

int bneInstruction(int i, int j) {
  return (i != j) ? 1 : 0;
}

int bnezInstruction(int i) {
  return (i != 0) ? 1 : 0;
}

int bgtInstruction(int i, int j) {
  return (i > j) ? 1 : 0;
}

int bgeInstruction(int i, int j) {
  return (i >= j) ? 1 : 0;
}

int bgtzInstruction(int i) {
  return (i > 0) ? 1 : 0;
}

int bltInstruction(int i, int j) {
  return (i < j) ? 1 : 0;
}

int bleInstruction(int i, int j) {
  return (i <= j) ? 1 : 0;
}

int blezInstruction(int i) { 
  return (i <= 0) ? 1 : 0;
}

//MODULES

//Seek the instruction by its opcode
int seekBinInstr10(instruction instr) {
  int count;
  for (count = 0; instrType10[count].opCode != -1; count++)
    if (instrType10[count].opCode == instr) {
      return 1;
    }
  return -1;
}

int seekBinInstr11(instruction instr) {
  int count;
  for (count = 0; instrType11[count].opCode != -1; count++)
    if (instrType11[count].opCode == instr) {
      return 1;
    }
  return -1;
}

int seekBinInstr20(instruction instr) {
  int count, i;
  for (count = 0; instrType20[count].opCode != -1; count++)
    if (instrType20[count].opCode == instr) {
      return 1;
    }
  return -1;
}

int seekBinInstr21(instruction instr) {
  int count, i;
  for (count = 0; instrType21[count].opCode != -1; count++)
    if (instrType21[count].opCode == instr) {
      return 1;
    }
  return -1;
}

int seekBinInstr30(instruction instr) {
  int count, i;
  for (count = 0; instrType30[count].opCode != -1; count++)
    if (instrType30[count].opCode == instr) {
      return 1;
    }
  return -1;
}

int seekBinInstr31(instruction instr) {
  int count, i;
  for (count = 0; instrType31[count].opCode != -1; count++)
    if (instrType31[count].opCode == instr) {
      return 1;
    }
  return -1;
}

//Seek the instructions and return its opcode
int seekInstr10(char *instr) {
  int count, i;
  for (count = 0; instrType10[count].opCode != -1; count++)
    if (!strcmp(instr, instrType10[count].instrName)) {
      return instrType10[count].opCode;
    }
  return -1;
}

int seekInstr11(char *instr) {
  int count, i;
  for (count = 0; instrType11[count].opCode != -1; count++)
    if (!strcmp(instr, instrType11[count].instrName)) {
      return instrType11[count].opCode;
    }
  return -1;
}

int seekInstr20(char *instr) {
  int count, i;
  for (count = 0; instrType20[count].opCode != -1; count++)
    if (!strcmp(instr, instrType20[count].instrName)) {
      return instrType20[count].opCode;
    }
  return -1;
}

int seekInstr21(char *instr) {
  int count, i;
  for (count = 0; instrType21[count].opCode != -1; count++)
    if (!strcmp(instr, instrType21[count].instrName)) {
      return instrType21[count].opCode;
    }
  return -1;
}

int seekInstr30(char *instr) {
  int count, i;
  for (count = 0; instrType30[count].opCode != -1; count++) 
    if (!strcmp(instr, instrType30[count].instrName)) 
      return instrType30[count].opCode;
  return -1;
}

int seekInstr31(char *instr) {
  int count, i;
  for (count = 0; instrType31[count].opCode != -1; count++) 
    if (!strcmp(instr, instrType31[count].instrName)) 
      return instrType31[count].opCode;
  return -1;
}

int seekInstr4(char *instr) {
  int count, i;
  for (count = 0; instrType4[count].opCode != -1; count++)
    if (!strcmp(instr, instrType4[count].instrName)) 
      return instrType4[count].opCode;
  return -1;
}

//Seek the register name and return the number of the register
int seekReg(char reg[]) {
  int count;
  for (count = 0; regMap[count].regNum < REGN; count++) {	
    if (!(strcmp(reg, regMap[count].regDef)))
      return regMap[count].regNum;	
  }
  return -1;
}

void printArchitecture(int a, int b, int c, int e, int f, int g) {
  printf("ARCHITECTURE: \n");
  printf("ADD =  %i\n", a);
  printf("MULTI = %i\n", b);
  printf("DIV = %i\n", c);
  printf("BUFFER_READ = %i\n", e);
  printf("BUFFER_WRITE = %i\n", f);
  printf("MEMORY_SIZE = %i\n", g);
}

//Print the binary instruction
void printBinInstr(instruction instr) {
  int count, aux;
  for (count = 31; count >= 0; count--) {
    aux = instr >> count;
    aux = aux & 1;
    printf("%i", aux);
  }
  printf("\n");
}

void printBinData(int data) {
  int count, aux;
  for (count = 31; count >= 0; count--) {
    aux = data >> count;
    aux = aux & 1;
    printf("%i", aux);
  }
  printf("\n");
}

//Memory functions
int storeInstruction(instruction instr, unsigned char *memory, int last) {
  int count, final = last + 4, aux = 0;
  for (count = last; count < final; count++) {
    memory[count] = instr >> (8*aux);
    aux++;
  }
  return final;
}

int storeData(int data, unsigned char *memory, int last) {
  int count, final = last + 4, aux = 0;
  for (count = last; count < final; count++) {
    memory[count] = data >> (8*aux);
    printf("STORING %i AT %i POSITION.\n", memory[count], count);
    aux++;
  }
  return final;
}

instruction loadMemory(unsigned char memory[], int pos) {
  int instr;
  instruction aux1 = memory[pos], aux2 = memory[pos-1], aux3 = memory[pos-2], aux4 = memory[pos-3];
  aux1 = aux1 << 24;
  aux2 = aux2 << 16;
  aux3 = aux3 << 8;
  instruction binInstr = aux1 + aux2 + aux3 + aux4;
  return binInstr;
}

void startMemory(unsigned char *memory, int memSize) {
  int count;
  for (count = 0; count < memSize; count++) 
    memory[count] = 0;
}

void printMemory(unsigned char memory[], int memSize, int quant) {
  int count;
  for (count = memSize - 1; count > quant; count = count - 4) {
    printf("MEMORY POS[%04i]: ", count);
    instruction binInstr = loadMemory(memory, count);
    printBinInstr(binInstr);
  }
}

//Label functions
int isLabel(char *label) { //0 = not label, 1 = is a label
  int count;
  for (count = 0; instrCycles[count].cycle != -1; count++) {
    if (!strcmp(label, instrList[count].instrName)) 
      return 0;
  }
  return -1;
}

int checkLabel(char label[], int lastLabel) {
  int count = 0;
  for (count = 0; count < lastLabel; count++) {
    if (!strcmp(labelList[count].labelName, label)) 
      return labelList[count].memPosition;
  }
  return -1;
}

//Instruction list
typedef struct circularList {
  instruction *list;
  int firstPos, lastPos, listSize, currentSize;
} circularList;

void startList(circularList *listInstr, int listSize) {
  int count;
  listInstr->list = malloc(sizeof(instruction)*listSize);
  listInstr->firstPos = 0;
  listInstr->lastPos = listInstr->firstPos;
  listInstr->listSize = listSize;
  listInstr->currentSize = 0;
}

int insertList(circularList *listInstr, instruction instr) {
  if (listInstr->currentSize < listInstr->listSize) {
    listInstr->list[listInstr->lastPos] = instr;
    listInstr->lastPos++;
    listInstr->currentSize++;
    if (listInstr->lastPos == listInstr->listSize)
      listInstr->lastPos = 0;
    return 1;
  }
  else 
    return 0;
}

void removeList(circularList *listInstr) {
  if (listInstr->currentSize != 0) {
    listInstr->firstPos++;  
    listInstr->currentSize--;
    if (listInstr->firstPos == listInstr->listSize)
      listInstr->firstPos = 0;
  }
}

void flushList(circularList *listInstr) {
  while (listInstr->currentSize != 0) 
    removeList(listInstr);
}
//Pipeline

int pipelineInstructionFetch(unsigned char memory[], int programCounter, circularList *instrList, int numInstrFetch, int mLastPos) {
  int count;
  for (count = 0; count < numInstrFetch; count++) {
    if (programCounter < mLastPos) {
      instruction binInstr = loadMemory(memory, programCounter);
      printf("Print Binary Instruction %i\n", programCounter);
      printBinInstr(binInstr);
      insertList(instrList, binInstr);
      programCounter += 4;
    }
  }
  return programCounter - 3;
}

void initReservStation(reservationStation *reservStation, int nReservStation) {
  int count;
  for (count = 0; count < nReservStation; count++) {
    reservStation[count].busy = 0;
    reservStation[count].Qi = -1;
    reservStation[count].Qj = -1;
    reservStation[count].opCode = -1;
    reservStation[count].Vi = -1;
    reservStation[count].Vj = -1;
    reservStation[count].mAddress = -1;
  }
}

fUnit *fAdder, *fMulti, *fDivi, *fLoad, *fStore;
int fAdderN, fMultiN, fDiviN, fLoadN, fStoreN;

void initFunctUnits(fUnit **functionalUnit, int nUnits) {
  int count;
  *functionalUnit = malloc(sizeof(fUnit)*nUnits);
  for (count = 0; count < nUnits; count++) {
    (*functionalUnit)[count].busy = 0;
  }
}

int findFunctUnit(fUnit *functionalUnit, int fUnitSize) {
  int count;
  for (count = 0; count < fUnitSize; count++) {
    if (functionalUnit[count].busy != 1) 
      return count;
  }
  return -1;
}

void initWritebackList(resultList *rList, int nSize) {
  rList = malloc(sizeof(resultList) * nSize);
}

resultList *writeBackList;

int instrCounter = 0;

int flagLoad = 0, flagStore = 0, flagBranch = 0;

int isBranch(instruction opCode) {
  int count;
  if (seekBinInstr21(opCode) > 0 || seekBinInstr31(opCode) > 0 || opCode == 48)
    return 1;
  else
    return 0;
}

instruction regShift(instruction reg, int shift) {
  instruction aux = reg >> shift;
  aux = aux & 31;
  return aux;
}

int pipelineInstructionDecod(instruction instr, reservationStation *reservStation, int nReservStation) {
  int count;
  instruction reg1, reg2, reg3, opeImediate, opeLabel;
  if (flagBranch == 0) {
    for (count = 0; count < nReservStation; count++) {
      if (reservStation[count].busy != 1 && flagBranch == 0) {
        instruction binInstr = instr >> 26;
        printf("binInstr %i\n", binInstr);
        switch (binInstr) {
          case 0: //LOAD
            if (flagStore > 0)
              return -1;
            flagLoad++;
          case 2: //MOVE
          case 3: //NEG
          case 4: //NOT
            reg1 = regShift(instr, 21);
            reg2 = regShift(instr, 16);
            if (instrRef[reg2] != -1) 
              reservStation[count].Qi = instrRef[reg2];
            else {
              reservStation[count].Vi = regBank[reg2];
            }
            instrRef[reg1] = instrCounter;
            reservStation[count].opCode = binInstr;
            break;

          case 1: //STORE
            if (flagLoad > 0)
              return -1;
            flagStore++;
            reg1 = regShift(instr, 21);
            reg2 = regShift(instr, 16);
            if (instrRef[reg1] != -1)
              reservStation[count].Qi = instrRef[reg1];
            else
              reservStation[count].Vi = regBank[reg1];

            if (instrRef[reg2] != -1) 
              reservStation[count].Qj = instrRef[reg2];
            else
              reservStation[count].Vj = regBank[reg2];
            reservStation[count].opCode = binInstr;
            break;

          case 5: //ADD
          case 6: //SUB
          case 7: //MULT
          case 8: //DIV
          case 9: //AND
          case 10: //OR
          case 11: //SLL
          case 12: //SLR
            reg1 = regShift(instr, 21);
            reg2 = regShift(instr, 16);
            reg3 = regShift(instr, 11);
            if (instrRef[reg2] != -1)
              reservStation[count].Qi = instrRef[reg2];
            else
              reservStation[count].Vi = regBank[reg2];

            if (instrRef[reg3] != -1) 
              reservStation[count].Qj = instrRef[reg3]; 
            else
              reservStation[count].Vj = regBank[reg3];
            reservStation[count].opCode = binInstr;
            instrRef[reg1] = instrCounter;
            reservStation[count].opCode = binInstr;
            break;

          case 16: //LI
            reg1 = regShift(instr, 21);
            opeImediate = instr << 11;
            opeImediate = opeImediate >> 11;
            reservStation[count].Vi = opeImediate;
            reservStation[count].opCode = binInstr;
            instrRef[reg1] = instrCounter;
            reservStation[count].opCode = binInstr;
            break;

          case 32: //ADDI
          case 33: //SUBI
          case 34: //MULTI
          case 35: //DIVI
          case 36: //ANDI
          case 37: //ORI
            reg1 = regShift(instr, 21);
            reg2 = regShift(instr, 16);
            opeImediate = instr << 16;
            opeImediate = opeImediate >> 16;
            if (instrRef[reg2] != -1)
              reservStation[count].Qi = instrRef[reg2];
            else
              reservStation[count].Vi = regBank[reg2];
            reservStation[count].Vj = opeImediate;
            instrRef[reg1] = instrCounter;
            reservStation[count].opCode = binInstr;
            break;

          case 17: //BEQZ 
          case 18: //BNEZ
          case 19: //BGTZ
          case 20: //BLEZ
            reg1 = regShift(instr, 21);
            opeLabel = instr << 16;
            opeLabel = opeLabel >> 16;
            if (instrRef[reg1] != -1) 
              reservStation[count].Qi = instrRef[reg1];
            else
              reservStation[count].Vi = regBank[reg1];
            reservStation[count].mAddress = opeLabel;
            reservStation[count].opCode = binInstr;
            flagBranch++;
            break;

          case 38: //BEQ
          case 39: //BNE
          case 40: //BGT
          case 41: //BGE
          case 42: //BLT
          case 43: //BLE
            reg1 = regShift(instr, 21);
            reg2 = regShift(instr, 16);
            opeLabel = instr << 16;
            opeLabel = opeLabel >> 16;
            if (instrRef[reg1] != -1)
              reservStation[count].Qi = instrRef[reg1];
            else
              reservStation[count].Vi = regBank[reg1];

            if (instrRef[reg2] != -1) 
              reservStation[count].Qj = instrRef[reg2]; 
            else
              reservStation[count].Vj = regBank[reg2];
            reservStation[count].mAddress = opeLabel;
            reservStation[count].opCode = binInstr;
            flagBranch++;
            break;

          case 48: //BEQ
            opeLabel = instr << 6;
            opeLabel = opeLabel >> 6;
            reservStation[count].mAddress = opeLabel;
            reservStation[count].opCode = binInstr;
            flagBranch++;
            break;
        }
        reservStation[count].refInstrCount = instrCounter;
        reservStation[count].busy = 1;
        instrCounter++;
        printf("opCode Issued: %i, Reserv Station: %i, InstrID: %i.\n", reservStation[count].opCode, count, reservStation[count].refInstrCount);
        return 1;
      }
    }
  }
  return -1;
}

int findCycle(instruction opCode) {
  int i;
  for (i = 0; i < REGN; i++) {
    if (opCode == instrCycles[i].opCode)
      return instrCycles[i].cycle;
  }
  return -1;
}

int rListCurretPos = 0;

int convertMemoryIndex(int index, int memorySize) {
  return (memorySize - (index * 4)) - 1;
}

void executeInstr(fUnit *funUnit, int fUnitPos, unsigned char mainMemory[], int memorySize) {
  switch(funUnit[fUnitPos].opCode) {
    case 0: //LOAD
      funUnit[fUnitPos].result = loadMemory(mainMemory, convertMemoryIndex(funUnit[fUnitPos].operand1, memorySize));
      break;
    case 1: //STORE
      funUnit[fUnitPos].result = convertMemoryIndex(funUnit[fUnitPos].operand2, memorySize) - 3; //Operand1 is the value to store
      break;
    case 2: //MOVE
      funUnit[fUnitPos].result = funUnit[fUnitPos].operand1;
      break;
    case 3: //NEG
      funUnit[fUnitPos].result = negInstruction(funUnit[fUnitPos].operand1);
      break;
    case 4: //NOT
      funUnit[fUnitPos].result = notInstruction(funUnit[fUnitPos].operand1);
      break;
    case 5: //ADD
    case 32: //ADDI
      funUnit[fUnitPos].result = funUnit[fUnitPos].operand1 + funUnit[fUnitPos].operand2;
      break;
    case 6: //SUB
    case 33: //SUBI
      funUnit[fUnitPos].result = funUnit[fUnitPos].operand1 - funUnit[fUnitPos].operand2;
      break;
    case 7: //MULT
    case 34: //MULTI
      funUnit[fUnitPos].result = funUnit[fUnitPos].operand1 * funUnit[fUnitPos].operand2;
      break;
    case 8: //DIV
    case 35: //DIVI
      funUnit[fUnitPos].result = funUnit[fUnitPos].operand1 / funUnit[fUnitPos].operand2;
      break;
    case 9: //AND
    case 36: //ANDI
      funUnit[fUnitPos].result = andInstruction(funUnit[fUnitPos].operand1, funUnit[fUnitPos].operand2);
      break;
    case 10: //OR
    case 37: //ORI
      funUnit[fUnitPos].result = orInstruction(funUnit[fUnitPos].operand1, funUnit[fUnitPos].operand2);
      break;
    case 11: //SLL
      funUnit[fUnitPos].result = funUnit[fUnitPos].operand1 << funUnit[fUnitPos].operand2;
      break;
    case 12: //SLR
      funUnit[fUnitPos].result = funUnit[fUnitPos].operand1 >> funUnit[fUnitPos].operand2;
      break;
    case 16: //LI
      funUnit[fUnitPos].result = funUnit[fUnitPos].operand1;
      break;
    case 17: //BEQZ 
      funUnit[fUnitPos].result = beqzInstruction(funUnit[fUnitPos].operand1); //operand1 is the address
      break;
    case 18: //BNEZ
      funUnit[fUnitPos].result = bnezInstruction(funUnit[fUnitPos].operand1); //operand1 is the address
      break;
    case 19: //BGTZ
      funUnit[fUnitPos].result = bgtzInstruction(funUnit[fUnitPos].operand1); //operand1 is the address
      break;
    case 20: //BLEZ
      funUnit[fUnitPos].result = blezInstruction(funUnit[fUnitPos].operand1);	//operand1 is the address
      break;
    case 38: //BEQ
      funUnit[fUnitPos].result = beqInstruction(funUnit[fUnitPos].operand1, funUnit[fUnitPos].operand2); //address is the address
      break;
    case 39: //BNE
      funUnit[fUnitPos].result = bneInstruction(funUnit[fUnitPos].operand1, funUnit[fUnitPos].operand2); //address is the address
      break;
    case 40: //BGT
      funUnit[fUnitPos].result = bgtInstruction(funUnit[fUnitPos].operand1, funUnit[fUnitPos].operand2); //address is the address
      break;
    case 41: //BGE
      funUnit[fUnitPos].result = bgeInstruction(funUnit[fUnitPos].operand1, funUnit[fUnitPos].operand2); //address is the address
      break;
    case 42: //BLT
      funUnit[fUnitPos].result = bltInstruction(funUnit[fUnitPos].operand1, funUnit[fUnitPos].operand2); //address is the address
      break;
    case 43: //BLE
      funUnit[fUnitPos].result = bleInstruction(funUnit[fUnitPos].operand1, funUnit[fUnitPos].operand2); //address is the address
    case 48: //B
      funUnit[fUnitPos].result = 1;
  }
}

void pipelineExecute(reservationStation *reservStation, int nReservStation, unsigned char mainMemory[], int memorySize) {
  int count, freeUnit;
  for (count = 0; count < fLoadN; count++) {
    if (fLoad[count].busy == 1) { 
      fLoad[count].currentCycle--;
      if (fLoad[count].currentCycle == 0) 
        executeInstr(fLoad, count, mainMemory, memorySize);
    }
  } 

  for (count = 0; count < fStoreN; count++) { 
    if (fStore[count].busy == 1)  {
      fStore[count].currentCycle--;
      if (fStore[count].currentCycle == 0) 
        executeInstr(fStore, count, mainMemory, memorySize);
    }
  }

  for (count = 0; count < fAdderN; count++) { 
    if (fAdder[count].busy == 1)  {
      fAdder[count].currentCycle--;
      if (fAdder[count].currentCycle == 0) 
        executeInstr(fAdder, count, mainMemory, memorySize);
    }
  }

  for (count = 0; count < fMultiN; count++) { 
    if (fMulti[count].busy == 1) {
      fMulti[count].currentCycle--;
      if (fMulti[count].currentCycle == 0) 
        executeInstr(fMulti, count, mainMemory, memorySize);
    }
  }

  for (count = 0; count < fDiviN; count++) { 
    if (fDivi[count].busy == 1) {
      fDivi[count].currentCycle--;
      if (fDivi[count].currentCycle == 0) 
        executeInstr(fDivi, count, mainMemory, memorySize);
    }
  }

  for (count = 0; count < nReservStation; count++) {
    if ((reservStation[count].Qi == -1) && (reservStation[count].Qj == -1) && reservStation[count].busy == 1) {
      switch(reservStation[count].opCode) {
        case 0: //LOAD
          freeUnit = findFunctUnit(fLoad, fLoadN);
          if ((freeUnit >= 0) && reservStation[count].busy == 1) {
            fLoad[freeUnit].opCode = reservStation[count].opCode;
            fLoad[freeUnit].currentCycle = findCycle(reservStation[count].opCode);
            fLoad[freeUnit].operand1 = reservStation[count].Vi;
            fLoad[freeUnit].refInstrCount = reservStation[count].refInstrCount;
            fLoad[freeUnit].busy = 1;
            reservStation[count].busy = 0;
          }
          break;

        case 1: //STORE
          freeUnit = findFunctUnit(fStore, fStoreN);
          if ((freeUnit >= 0) && reservStation[count].busy == 1) {
            fStore[freeUnit].opCode = reservStation[count].opCode;
            fStore[freeUnit].currentCycle = findCycle(reservStation[count].opCode);
            fStore[freeUnit].operand1 = reservStation[count].Vi;
            fStore[freeUnit].operand2 = reservStation[count].Vj;
            fStore[freeUnit].refInstrCount = reservStation[count].refInstrCount;
            fStore[freeUnit].busy = 1;
            reservStation[count].busy = 0;
          }
          break;

        case 2: //MOVE
        case 3:	//NEG
        case 4: //NOT
        case 9: //AND
        case 10: //OR
        case 16: //LI
        case 5: //ADD
        case 6: //SUB
        case 11: //SLL
        case 12: //SLR
        case 17: //BEQZ
        case 18: //BNEZ
        case 19: //BGTZ
        case 20: //BLEZ
        case 32: //ADDI
        case 33: //SUBI
        case 36: //ANDI
        case 37: //ORI
        case 38: //BEQ
        case 39: //BNE
        case 40: //BGT
        case 41: //BGE
        case 42: //BLT
        case 43: //BLE
        case 48: //B
          freeUnit = findFunctUnit(fAdder, fAdderN);
          if ((freeUnit >= 0) && reservStation[count].busy == 1) {
            fAdder[freeUnit].currentCycle = findCycle(reservStation[count].opCode);
            fAdder[freeUnit].opCode = reservStation[count].opCode;
            fAdder[freeUnit].operand1 = reservStation[count].Vi;
            fAdder[freeUnit].operand2 = reservStation[count].Vj;
            fAdder[freeUnit].address = reservStation[count].mAddress;
            fAdder[freeUnit].refInstrCount = reservStation[count].refInstrCount;
            fAdder[freeUnit].busy = 1;
            reservStation[count].busy = 0;
            printf("SETTING RS %i TO UNOCCUPIED.\n", count);
          }
          break;

        case 7:	//MULT
        case 34: //MULTI
          freeUnit = findFunctUnit(fMulti, fMultiN); 
          if ((freeUnit >= 0) && reservStation[count].busy == 1) {
            fMulti[freeUnit].opCode = reservStation[count].opCode;
            fMulti[freeUnit].currentCycle = findCycle(reservStation[count].opCode);
            fMulti[freeUnit].operand1 = reservStation[count].Vi;
            fMulti[freeUnit].operand2 = reservStation[count].Vj;
            fMulti[freeUnit].refInstrCount = reservStation[count].refInstrCount;
            fMulti[freeUnit].busy = 1;
            reservStation[count].busy = 0;
          }
          break;

        case 8:	//DIV
        case 35: //DIVI
          freeUnit = findFunctUnit(fDivi, fDiviN);
          if ((freeUnit >= 0) && reservStation[count].busy == 1) {
            fDivi[freeUnit].opCode = reservStation[count].opCode;
            fDivi[freeUnit].currentCycle = findCycle(reservStation[count].opCode);
            fDivi[freeUnit].operand1 = reservStation[count].Vi;
            fDivi[freeUnit].operand2 = reservStation[count].Vj;
            fDivi[freeUnit].refInstrCount = reservStation[count].refInstrCount;
            fDivi[freeUnit].busy = 1;
            reservStation[count].busy = 0;
          }
          break;	
      }
      printf("From RS %i To fUnit %i with ID %i.\n", count, freeUnit, reservStation[count].refInstrCount);
    }
  }
}

int broadcastReservStation(fUnit *funUnit, int funUnitSize, reservationStation *reservStation, int nReservStation) {
  int count, reservStationPos, regPos, nFlags = 0;
  for (count = 0; count < funUnitSize; count++) {
    if ((funUnit[count].currentCycle == 0) && (funUnit[count].busy == 1)) {
      nFlags++;
      for (reservStationPos = 0; reservStationPos < nReservStation; reservStationPos++) {
        if (reservStation[reservStationPos].Qi == funUnit[count].refInstrCount) {
          reservStation[reservStationPos].Qi = -1;
          reservStation[reservStationPos].Vi = funUnit[count].result;
        }
        if (reservStation[reservStationPos].Qj == funUnit[count].refInstrCount) {
          reservStation[reservStationPos].Qj = -1;
          reservStation[reservStationPos].Vj = funUnit[count].result;
        }
      }

      for (regPos = 0; regPos < REGN; regPos++) {
        if (instrRef[regPos] == funUnit[count].refInstrCount ) {
          printf("OPCODE %i\n", funUnit[count].opCode);
          printf("REGISTER R%i.\n", regPos);
          printf("REF INSTR COUNT %i\n", funUnit[count].refInstrCount);
          regBank[regPos] = funUnit[count].result;
          instrRef[regPos] = -1;
        }
      }
      funUnit[count].busy = 0;
    }
  }
  return nFlags;
}

int programCounter = 0;

void pipelineWriteBack(reservationStation *reservStation, int nReservStation, unsigned char *mainMemory, int memorySize, circularList *listInstr) {
  int nFlags = broadcastReservStation(fLoad, fLoadN, reservStation, nReservStation);
  int count;
  flagLoad -= nFlags;

  for (count = 0; count < fStoreN; count++) {
    if ((fStore[count].currentCycle == 0) && (fStore[count].busy == 1)) {
      storeData(fStore[count].operand1, mainMemory, fStore[count].result);
      printf("STORING %i AT %i POSITION\n", fStore[count].operand1, fStore[count].result);
      instruction binData = loadMemory(mainMemory, fStore[count].result);
      printf("BINARY DATA AT %i POSITION\n", fStore[count].result);
      printBinInstr(binData);
      flagStore--;
      fStore[count].busy = 0;
    } 
  }

  for (count = 0; count < fAdderN; count++) {
    if ((fAdder[count].currentCycle == 0) && (fAdder[count].busy == 1)) {
      if (isBranch(fAdder[count].opCode)) { 
        if (fAdder[count].result == 1) {
          programCounter = fAdder[count].address;
          flushList(listInstr);
        }
        flagBranch = 0;
        fAdder[count].busy = 0;
      } 
      else {
        int reservStationPos;
        for (reservStationPos = 0; reservStationPos < nReservStation; reservStationPos++) {
          if (reservStation[reservStationPos].Qi == fAdder[count].refInstrCount) {
            reservStation[reservStationPos].Qi = -1;
            reservStation[reservStationPos].Vi = fAdder[count].result;
          }
          if (reservStation[reservStationPos].Qj == fAdder[count].refInstrCount) {
            reservStation[reservStationPos].Qj = -1;
            reservStation[reservStationPos].Vj = fAdder[count].result;
          }
        }
        int regPos;
        for (regPos = 0; regPos < REGN; regPos++) {
          if (instrRef[regPos] == fAdder[count].refInstrCount) {
            printf("OPCODE %i\n", fAdder[count].opCode);
            printf("REGISTER R%i.\n", regPos);
            printf("REF INSTR COUNT %i\n", fAdder[count].refInstrCount);
            regBank[regPos] = fAdder[count].result;
            instrRef[regPos] = -1;
          }
        }
        fAdder[count].busy = 0;
      }
    }
  }
  broadcastReservStation(fMulti, fMultiN, reservStation, nReservStation);
  broadcastReservStation(fDivi, fDiviN, reservStation, nReservStation);
}

int emptyReservStations(reservationStation *reservStation, int nReservStation) {
  int count = 0;
  for (count = 0; count < nReservStation; count++) {
    if (reservStation[count].busy != 0) 
      return 0;
  }
  return 1;
}

int emptyFuncUnits(fUnit *funUnit, int nFunUnit) {
  int i;
  for (i = 0; i < nFunUnit; i++) {
    if (funUnit[i].busy != 0) {
      return 0;
    }
  }
  return 1;
}

void printFuncUnits(fUnit *funUnit, int nFunUnit, const char name[]) {
  int count;
  for (count = 0; count < nFunUnit; count++) {
    if (funUnit[count].busy != 0)
      printf("%s: %i, opCode: %i, op1: %i, op2: %i, result: %i, busy: %i, cycle: %i, id: %i\n", name, count, funUnit[count].opCode, funUnit[count].operand1, funUnit[count].operand2, funUnit[count].result, funUnit[count].busy, funUnit[count].currentCycle, funUnit[count].refInstrCount);
  }
}

//Output functions
void printRegisters() {
  int count;
  printf("REGISTER MAP\n");
  for (count = 0; count < REGN; count++) {
    printf("R%i\t", count + 1);
    printBinData(regBank[count]);
  }
  printf("\n\n");
}

void printReservStation(reservationStation *rs, int nReservStations) {
  int count;
  for (count = 0; count < nReservStations; count++) {
    if (rs[count].busy == 0)
      continue;
    printf("RS\tOp\tQi\tQj\tVi\tVj\tAd\tB\tID\n");
    printf("%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n", count, rs[count].opCode, rs[count].Qi, rs[count].Qj, rs[count].Vi, rs[count].Vj, rs[count].mAddress, rs[count].busy, rs[count].refInstrCount);
    printf("------------------------------------------------------------\n\n");
  }
}

int main() {
  //Opcode, operands and the input file
  char instr[6], operand1[4], operand2[4], operand3[4];
  int imediate, clock = 0;;
  FILE *inputFile;
  inputFile = fopen("Instructions.txt", "r");
  if (!inputFile) {
    printf("ERROR -01: FILE (Instructions.txt) NOT FOUND. PROGRAM HALTED.\n");
    return 0;
  }

  //Reading the parameters of the architecture
  int archAdd, archMulti, archDiv, archInstrFetch, archWriteBuffer, archLoadBuffer, archMemorySize;
  int count;
  char header[15], aux[20];
  fscanf(inputFile, "%s", header);	
  fscanf(inputFile, "%s = %i", aux, &archAdd);
  fscanf(inputFile, "%s = %i", aux, &archMulti);
  fscanf(inputFile, "%s = %i", aux, &archDiv);
  fscanf(inputFile, "%s = %i", aux, &archLoadBuffer);
  fscanf(inputFile, "%s = %i", aux, &archWriteBuffer);
  fscanf(inputFile, "%s = %i", aux, &archMemorySize);
  printArchitecture(archAdd, archMulti, archDiv, archWriteBuffer, archLoadBuffer, archMemorySize);
  printf("\n");

  fAdderN = archAdd;
  fMultiN = archMulti;
  fDiviN = archDiv;
  fLoadN = archLoadBuffer;
  fStoreN = archWriteBuffer;

  initFunctUnits(&fAdder, archAdd);
  initFunctUnits(&fMulti, archMulti);
  initFunctUnits(&fDivi, archDiv);
  initFunctUnits(&fLoad, archLoadBuffer);
  initFunctUnits(&fStore, archWriteBuffer);

  //Reading cycles of each instructions	
  fscanf(inputFile, "%s", header);
  printf("%s:\n", header); 	
  for (count = 0; count < 32; count++) {
    fscanf(inputFile, "%s = %i", aux, &instrCycles[count].cycle);
  }
  printf("\n");

  unsigned char mainMemory[archMemorySize]; //Each block of the memory has 1 byte (8 bits)
  startMemory(mainMemory, archMemorySize);

  //Reading the parameters of the pipeline
  int instrFetchLimit, instrQueueSize, nReservStation;
  printf("PIPELINE:\n");
  fscanf(inputFile, "%s = %i", aux, &instrFetchLimit);
  printf("%s = %i\n", aux, instrFetchLimit);
  fscanf(inputFile, "%s = %i", aux, &instrQueueSize);
  printf("%s = %i\n", aux, instrQueueSize);
  fscanf(inputFile, "%s = %i", aux, &nReservStation);
  printf("%s = %i\n", aux, nReservStation);

  circularList instrFetchList;
  startList(&instrFetchList, instrQueueSize);
  reservationStation reservStations[nReservStation];
  decodStruct decodedInstr[nReservStation];

  resultList *writeBackList;
  initWritebackList(writeBackList, fAdderN + fMultiN + fDiviN + fLoadN + fStoreN);

  //Reading text (.text and .data) 
  char data[10];
  fscanf(inputFile, "%s", header);
  printf("\nLOADING .DATA TO MEMORY.\n");
  int i, p = archMemorySize - 4, lastDataPos = p + 3; //p = last data memory position
  fscanf(inputFile, "%s", data);
  while(fscanf(inputFile, "%s", data) != EOF && (strcmp(data, ".text"))) {
    int count;
    i = atoi(data);
    if (i < 0) {
      i *= -1;
      i = i + (1 << 31); 
    }
    lastDataPos = storeInstruction(i, mainMemory, lastDataPos - 3);
    lastDataPos -= 5;
  }

  instruction binaryData;
  while (lastDataPos < p) {
    binaryData = loadMemory(mainMemory, p + 3);
    printBinInstr(binaryData); 
    p -= 4;
  }
  int textPointer = ftell(inputFile), labelMemory = 4;
  char line[128],label[64], readNewLine[4];
  fscanf(inputFile, "%c", readNewLine);
  while (fscanf(inputFile, "%[^\n]", line) != EOF) { //This read the file to find labels
    fscanf(inputFile, "%c", readNewLine);
    sscanf(line, "%s", label);
    if (isLabel(label) < 0) {
      strcpy(labelList[nextLabel].labelName, label);
      labelList[nextLabel].memPosition = labelMemory - 4;
      nextLabel++;
      labelMemory -= 4;
    }
    labelMemory += 4;
  }
  printf("\n-----DONE-----\n");
  printf("\nLOADING .TEXT TO MEMORY.\n");

  printf("LABELS DEFINED:\n");
  for (count = 0; count < nextLabel; count++) {
    printf("%s ", labelList[count].labelName);
  }
  printf("\n");

  fseek(inputFile, textPointer, SEEK_SET);

  int lastPos = 0;

  while (fscanf(inputFile, "%s", instr) != EOF) {
    if (seekInstr10(instr) >= 0) {
      instruction opCode = seekInstr10(instr) << 26;
      fscanf(inputFile, "%s %s", operand1, operand2);
      instruction reg1 = seekReg(operand1) << 21;
      if (seekReg(operand1) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand1);
        return 0;
      }
      instruction reg2 = seekReg(operand2) << 16;
      if (seekReg(operand2) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand2);
        return 0;
      }
      instruction binaryInstr = opCode + reg1 + reg2;
      lastPos = storeInstruction(binaryInstr, mainMemory, lastPos); //Note: It updates the memory position
      binaryInstr = loadMemory(mainMemory, lastPos - 1);
      printBinInstr(binaryInstr);
    }

    else if (seekInstr11(instr) >= 0) {	
      instruction opCode = seekInstr11(instr) << 26;
      fscanf(inputFile, "%s %s %s", operand1, operand2, operand3);
      instruction reg1 = seekReg(operand1) << 21;
      if (seekReg(operand1) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand1);
        return 0;
      }
      instruction reg2 = seekReg(operand2) << 16;
      if (seekReg(operand2) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand2);
        return 0;
      }
      instruction reg3 = seekReg(operand3) << 11;
      if (seekReg(operand3) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand3);
        return 0;
      }
      instruction binaryInstr = opCode + reg1 + reg2 + reg3;
      lastPos = storeInstruction(binaryInstr, mainMemory, lastPos);			
      binaryInstr = loadMemory(mainMemory, lastPos - 1);
      printBinInstr(binaryInstr);
    }		

    else if (seekInstr20(instr) >= 0) {
      instruction opCode = seekInstr20(instr) << 26;
      fscanf(inputFile, "%s %i", operand1, &imediate);
      instruction reg1 = seekReg(operand1) << 21;
      if (seekReg(operand1) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand1);
        return 0;
      }
      if (imediate < 0) {
        imediate *= -1;
        imediate = imediate + (1 << 15);
      }
      instruction binaryInstr = opCode + reg1 + imediate;
      lastPos = storeInstruction(binaryInstr, mainMemory, lastPos);			
      binaryInstr = loadMemory(mainMemory, lastPos - 1);
      printBinInstr(binaryInstr);
    }
    else if (seekInstr21(instr) >= 0) {
      instruction opCode = seekInstr21(instr) << 26;
      fscanf(inputFile, "%s %s", operand1, operand2); //operand2 is the label
      instruction reg1 = seekReg(operand1) << 21;	
      if (seekReg(operand1) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand1);
        return 0;
      }
      int labelAddress = checkLabel(operand2, nextLabel);
      if (labelAddress < 0) {
        printf("LABEL |%s| NOT DEFINED. PROGRAM HALTED.\n", operand2);
        return 0;
      }
      instruction binaryInstr = opCode + reg1 + labelAddress;
      lastPos = storeInstruction(binaryInstr, mainMemory, lastPos);
      binaryInstr = loadMemory(mainMemory, lastPos - 1);
      printBinInstr(binaryInstr);
    }

    else if (seekInstr30(instr) >= 0) {
      instruction opCode = seekInstr30(instr) << 26;
      fscanf(inputFile, "%s %s %i", operand1, operand2, &imediate);
      instruction reg1 = seekReg(operand1) << 21;
      if (seekReg(operand1) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand1);
        return 0;
      }
      instruction reg2 = seekReg(operand2) << 16;
      if (seekReg(operand2) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand2);
        return 0;
      }
      if (imediate < 0) {
        imediate *= -1;
        imediate = imediate + (1 << 15);
      }
      instruction binaryInstr = opCode + reg1 + reg2 + imediate;	
      lastPos = storeInstruction(binaryInstr, mainMemory, lastPos);			
      binaryInstr = loadMemory(mainMemory, lastPos - 1);
      printBinInstr(binaryInstr);
    }
    else if (seekInstr31(instr) >= 0) {
      instruction opCode = seekInstr31(instr) << 26;
      fscanf(inputFile, "%s %s %s", operand1, operand2, operand3);
      instruction reg1 = seekReg(operand1) << 21;
      if (seekReg(operand1) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand1);
        return 0;
      }
      instruction reg2 = seekReg(operand2) << 16;
      if (seekReg(operand2) < 0) {
        printf("ERROR: REGISTER |%s| NOT FOUND. PROGRAM HALTED.\n", operand2);
        return 0;
      }
      instruction labelAddress = checkLabel(operand3, nextLabel);
      if (labelAddress < 0) {
        printf("LABEL |%s| NOT DEFINED. PROGRAM HALTED.\n", operand3);
        return 0;
      }
      instruction binaryInstr = opCode + reg1 + reg2 + labelAddress;
      lastPos = storeInstruction(binaryInstr, mainMemory, lastPos);
      binaryInstr = loadMemory(mainMemory, lastPos - 1);
      printBinInstr(binaryInstr);
    }
    else if (seekInstr4(instr) >= 0) {
      instruction opCode = seekInstr4(instr) << 26;
      fscanf(inputFile, "%s", operand1);
      instruction labelAddress = checkLabel(operand1, nextLabel);
      if (labelAddress < 0) {
        printf("LABEL |%s| NOT DEFINED. PROGRAM HALTED.\n", operand1);
        return 0;
      }
      instruction binaryInstr = opCode + labelAddress;
      lastPos = storeInstruction(binaryInstr, mainMemory, lastPos);
      binaryInstr = loadMemory(mainMemory, lastPos - 1);
      printBinInstr(binaryInstr);
    }
    else {
      strcpy(labelList[nextLabel].labelName, instr);
      labelList[nextLabel].memPosition = lastPos;
      nextLabel++;
      int count;
    }
  }
  printf("\n-----DONE-----\n");
  fclose(inputFile); //End of reading file

  for (i = 0; i < REGN; i++) 
    instrRef[i] = -1;

  //Tomasulo
  for (i = 0; i < REGN; i++)
    regBank[i] = 0;

  initReservStation(reservStations, nReservStation);

  int emptyAdder, emptyLoad, emptyStore, emptyMulti, emptyDivi;
  int option = 1;

  while (1) {
    //Writeback
    printf("BEGIN WRITEBACK.\n");
    pipelineWriteBack(reservStations, nReservStation, mainMemory, archMemorySize, &instrFetchList);
    printf("END WRITEBACK\n");

    //Execute
    printf("BEGIN EXECUTE\n");
    pipelineExecute(reservStations, nReservStation, mainMemory, archMemorySize);
    printf("END EXECUTE.\n");

    //Decodification
    printf("BEGIN DECOD\n");
    printf("\n\n\n");

    //while (1) {
    //if (flagBranch != 0) {
    //break;
    // else {
    if (instrFetchList.currentSize != 0) {
      int listPosRef = instrFetchList.firstPos;
      for (i = 0; i < instrFetchLimit; i++) {
        printf("FIRST POS %i\n", instrFetchList.firstPos);
        printf("QUEUE SIZE %i\n", instrFetchList.listSize);
        int issued = pipelineInstructionDecod(instrFetchList.list[instrFetchList.firstPos], reservStations, nReservStation);
        if (issued == 1) {
          removeList(&instrFetchList);
        }
        else
          break;
      }
      //}
  } 
  //}
  //}
  printf("END DECOD\n");
  //Instruction Fetch
  printf("BEGIN INSTR FETCH\n");
  int intTemp = pipelineInstructionFetch(mainMemory, programCounter + 3, &instrFetchList, instrFetchLimit, lastPos);
  clock++;
  programCounter = intTemp;

  printf("END INSTR FETCH\n");

  printReservStation(reservStations, nReservStation);
  printRegisters();


  /*printf("Print queue\n");
  int a = instrFetchList.firstPos;
  while (a != instrFetchList.lastPos) {
    printBinInstr(instrFetchList.list[a]);
    a++;

    if (a == instrFetchList.listSize)
      a = 0;
  }*/
  
  for (i = 0; i < REGN; i++) {
    printf("REG%i -> instrRef = %i.\n", i+1, instrRef[i]);
  }

  printFuncUnits(fAdder, fAdderN, "fAdder");
  printFuncUnits(fStore, fStoreN, "fStore");
  printFuncUnits(fLoad, fLoadN, "fLoad");
  printFuncUnits(fMulti, fMultiN, "fMulti");
  printFuncUnits(fDivi, fDiviN, "fDivi");
  printf("STORE FLAG: %i\n", flagStore);
  printf("LOAD FLAG: %i\n", flagLoad);

  printf("PROGRAM COUNTER: %i\n\n", programCounter);
  emptyAdder = emptyFuncUnits(fAdder, fAdderN);
  emptyLoad = emptyFuncUnits(fLoad, fLoadN);
  emptyStore = emptyFuncUnits(fStore, fStoreN);
  emptyMulti = emptyFuncUnits(fMulti, fMultiN);
  emptyDivi = emptyFuncUnits(fDivi, fDiviN);
  if (emptyReservStations(reservStations, nReservStation) && emptyAdder && emptyLoad && emptyStore && emptyMulti && emptyDivi && instrFetchList.currentSize == 0) {
    printf("CLOCK: %i\n", clock);
    printMemory(mainMemory, archMemorySize, lastDataPos);
    printf("PROGRAM END\n");
    return 0;
  }
  }
}
