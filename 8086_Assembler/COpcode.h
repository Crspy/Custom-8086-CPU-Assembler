#pragma once
#include "stdafx.h"
#include "CROMBlock.h"
#include "tMemAddress.h"
#include "ErrorHandler.h"


enum eRegID : int8_t
{
    AX,
    BX,
    CX,
    DX
};



enum eOpcode : int8_t
{
    MOVE_IN = 1,
    MOVE_OUT = 2,
    LOAD = 3,
    INDIRECT_IN = 4,
    INDIRECT_OUT = 5
};

enum eOpcodeDir
{
    IN,
    OUT
};

class COpcode
{
public:
    static int8_t GetRegID(const char* lineReg);

    static eOpcodeDir GetOpcodeDir(std::string line);

    static eErrorType ProcessMoveOUT(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer,
        bool* bMovingData, CROMBlock* myrom);
    
    static eErrorType ProcessMoveIN(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer);

    static eErrorType ProcessIndirectMoveOUT(tMemAddress* memadd, tInstBlock* currentInst,
        char* linebuffer);

    static eErrorType ProcessIndirectMoveIN(tMemAddress* memadd, tInstBlock* currentInst,
        char* linebuffer);


};