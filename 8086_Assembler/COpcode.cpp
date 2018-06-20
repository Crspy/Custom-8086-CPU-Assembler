#include "COpcode.h"




int8_t COpcode::GetRegID(const char* lineReg)
{
    if (strcmp(lineReg, "ax") == 0)
        return eRegID::AX;
    else if (strcmp(lineReg, "bx") == 0)
        return eRegID::BX;
    else if (strcmp(lineReg, "cx") == 0)
        return eRegID::CX;
    else if (strcmp(lineReg, "dx") == 0)
        return eRegID::DX;
    else
        return -1;
}

void COpcode::EliminateComments(char* line)
{
    char keys[] = "/;";
    char* pch = NULL;
    pch = strpbrk(line, keys);
    while (pch != NULL)
    {
        //std::cout << "FOUND THE COMMENT START " << "\n";
        strcpy(pch, "\0");
        pch = strpbrk(pch + 1, keys);
    }
}

eOpcodeDir COpcode::GetOpcodeDir(std::string line)
{
    std::stringstream ss(line);
    std::getline(ss, line, ',');
    if (line.find('[') != std::string::npos)
        return eOpcodeDir::OUT;
    else
        return eOpcodeDir::IN;
}


eErrorType COpcode::ProcessMoveIN(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer)
{
    int8_t reg;
    char * token;
    currentInst[0].opcode = eOpcode::MOVE_IN;
    //std::cout << "MOVE IN" << '\n';
    currentInst[0].dir_flag = eOpcodeDir::IN;


    token = strtok(NULL, " ,[]/");
    logger(token);

    reg = GetRegID(token);

    token = strtok(NULL, " ,[]/");
    logger(token);

    if (GetRegID(token) >= 0)
    {
        return eErrorType::USING_REGNAME_INSTEAD;
    }

    memadd->m_Address = strtol(token, NULL, 0);

    if (!memadd->InsureAddress())
    {
        return eErrorType::MEM_ADDRESS_EXCEEDS;
    }
    if (memadd->m_bNeedLoading)
    {
        currentInst[1].address = memadd->byte0; // lowbyte for mov
        currentInst[1].opcode = currentInst[0].opcode;
        currentInst[1].dir_flag = currentInst[0].dir_flag;
        currentInst[0].opcode = eOpcode::LOAD;
        currentInst[0].dir_flag = eOpcodeDir::IN; // DirFlag for Loading is 0
        currentInst[0].address = memadd->byte1; // highbyte for load
    }

    else
    {
        currentInst[0].address = memadd->byte0;
    }

    if (reg == -1)
    {
        return eErrorType::UNKNOWN_REG_NAME;
    }

    if (memadd->m_bNeedLoading)
        currentInst[1].reg_id = reg;
    else
        currentInst[0].reg_id = reg;
    return eErrorType::NO_ERROR;
}


eErrorType COpcode::ProcessMoveOUT(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer,
    bool* bMovingData, CROMBlock* myrom)
{
    int8_t reg;

    char* token;
    currentInst[0].opcode = eOpcode::MOVE_OUT;
    //std::cout << "MOVE OUT" << '\n';
    currentInst[0].dir_flag = eOpcodeDir::OUT;

    token = strtok(NULL, " [], \t");
    logger(token);
    if (GetRegID(token) >= 0)
    {
        return eErrorType::USING_REGNAME_INSTEAD;

    }

    memadd->m_Address = strtol(token, NULL, 0);
    logger(memadd->m_Address);
    if (!memadd->InsureAddress())
    {
        return eErrorType::MEM_ADDRESS_EXCEEDS;
    }
    if (memadd->m_bNeedLoading)
    {
        currentInst[1].address = memadd->byte0; // lowbyte for mov
        currentInst[1].opcode = currentInst[0].opcode;
        currentInst[1].dir_flag = currentInst[0].dir_flag;
        currentInst[0].opcode = eOpcode::LOAD;
        currentInst[0].dir_flag = 0;
        currentInst[0].address = memadd->byte1; // highbyte for load
    }
    else
    {
        currentInst[0].address = memadd->byte0;
    }


    token = strtok(NULL, " [], \t");
    
    EliminateComments(token);

    reg = GetRegID(token);
    if (reg == -1) // then it's moving value to dataSeg
    {

        long lvalue;
        try
        {
            lvalue = strtol(token, NULL, 0);
            logger("VALUE");
            logger(lvalue);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << "\n";
            return eErrorType::UNKNOWN_REG_NAME;

        }

        if (lvalue > 32767 || lvalue < -32768)
        {
            return eErrorType::DATA_VALUE_OUTOFBOUNDS;

        }
        else
        {
            *bMovingData = true;
            memadd->m_bNeedLoading = false;
            int16_t _value = lvalue;
            //std::cout << "THE SIGNED  " << _value << "\n";

            // swapping endianess
            *(((char*)&myrom->DataSeg[memadd->m_Address].value + 1)) = *(((char*)&_value));
            *(((char*)&myrom->DataSeg[memadd->m_Address].value)) = *(((char*)& _value + 1));
        }

    }
    if (memadd->m_bNeedLoading)
        currentInst[1].reg_id = reg;
    else
        currentInst[0].reg_id = reg;
    return eErrorType::NO_ERROR;
}

eErrorType COpcode::ProcessIndirectMoveOUT(tMemAddress* memadd, tInstBlock* currentInst,
    char* linebuffer)
{
    int8_t reg;
    int8_t reg2;
    char* token;

    currentInst[0].opcode = eOpcode::INDIRECT_OUT;

    currentInst[0].dir_flag = eOpcodeDir::OUT;

    token = strtok(NULL, " [], \t");
    logger(token);

    reg2 = GetRegID(token);
    if (reg2 == -1 || reg2 != eRegID::BX) // then it's moving value to dataSeg
    {
        return eErrorType::UNKNOWN_REG_NAME_OR_NOT_USING_BX_IN_LEFT_OPERAND;
    }

    token = strtok(NULL, " [], \t");
    logger(token);
    EliminateComments(token);
    reg = GetRegID(token);
    if (reg == -1 || reg == eRegID::BX) // then it's moving value to dataSeg
    {
        return eErrorType::UNKNOWN_REG_NAME_OR_USING_BX_IN_RIGHT_OPERAND;
    }
    currentInst[0].reg_id = reg;

    return eErrorType::NO_ERROR;
}

eErrorType COpcode::ProcessIndirectMoveIN(tMemAddress* memadd, tInstBlock* currentInst,
    char* linebuffer)
{
    int8_t reg;
    int8_t reg2;
    char* token;

    currentInst[0].opcode = eOpcode::INDIRECT_IN;
    currentInst[0].dir_flag = eOpcodeDir::IN;

    token = strtok(NULL, " [], \t");
    logger(token);

    reg = GetRegID(token);
    if (reg == -1 || reg == eRegID::BX) // then it's moving value to dataSeg
    {
        return eErrorType::UNKNOWN_REG_NAME_OR_USING_BX_IN_LEFT_OPERAND;
    }

    token = strtok(NULL, " [], \t");
    logger(token);

    reg2 = GetRegID(token);
    if (reg2 == -1 || reg2 != eRegID::BX) // then it's moving value to dataSeg
    {
        return eErrorType::UNKNOWN_REG_NAME_OR_NOT_USING_BX_IN_RIGHT_OPERAND;
    }
    currentInst[0].reg_id = reg;

    return eErrorType::NO_ERROR;
}