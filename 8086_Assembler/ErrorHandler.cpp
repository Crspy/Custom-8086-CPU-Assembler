#include "ErrorHandler.h"


void CErrorHandler::PrintErrorMessage(eErrorType errortype, long& linecount)
{
    const char* message = NULL;
    switch (errortype)
    {

    case USING_REGNAME_INSTEAD:
        message = "Compiler Error , using Reg name instead of a memory address... in line : ";
        break;
    case MEM_ADDRESS_EXCEEDS:
        message = "Compiler Error , Memory Address exceeds 2 bytes... in line : ";
        break;
    case UNKNOWN_REG_NAME:
        message = "Compiler Error , unknown reg name....in line : ";
        break;
    case DATA_VALUE_OUTOFBOUNDS:
        message = "Compiler Error , data value is Out Of Bounds signed short integer....in line : ";
        break;
    case UNKNOWN_REG_NAME_OR_NOT_USING_BX_IN_LEFT_OPERAND:
        message = "Compiler Error , unknown reg name or not using bx in left operand....in line : ";
        break;
    case UNKNOWN_REG_NAME_OR_USING_BX_IN_RIGHT_OPERAND:
        message = "Compiler Error , unknown reg name or using bx in right operand....in line : ";
        break;
    case UNKNOWN_REG_NAME_OR_USING_BX_IN_LEFT_OPERAND:
        message = "Compiler Error , unknown reg name or using bx in left operand....in line : ";
        break;
    case UNKNOWN_REG_NAME_OR_NOT_USING_BX_IN_RIGHT_OPERAND:
        message = "Compiler Error , unknown reg name or not using bx in right operand....in line : ";
        break;
    case UNKNOWN_OPCODE:
        message = "Compiler Error, Unknown opcode in line : ";
        break;
    case ROM_INSTRUCTION_SEGMENT_OVERFLOW:
        message = "Compiler Error , Rom Instruction Segment overflow \nPossible Solution: Reduce your code lines";
        break;
    default:
        break;
    }
    if (message)
    {
        std::cout << message << linecount << '\n';
        std::cout << "Press Any Key to exit..." << '\n';
        std::cin.get();
    }
}