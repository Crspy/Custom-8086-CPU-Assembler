#pragma once

struct tMemAddress
{
    union
    {
        uint32_t m_Address;
        struct
        {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        };
    };
    bool m_bNeedLoading;

    bool InsureAddress()
    {
        if (this->m_Address > 0xFFFF)   return false;

        this->m_bNeedLoading = (this->m_Address > 0xFF) ? true : false;

        return true;
    }
    
};
static_assert(sizeof(tMemAddress) == 8, "tMemAddress size is incorrect");


struct tInstBlock
{
public:
    uint16_t reg_id : 2;
    uint16_t dir_flag : 1; // 0 - in  ,  1 - out
    uint16_t opcode : 5;
    uint16_t address : 8;
    tInstBlock()
    {
        memset(this, 0, sizeof(tInstBlock));
    }
};
static_assert(sizeof(tInstBlock) == 2, "tInstBlock size is incorrect");

class CROMBlock
{
public:
    union
    {
        struct
        {
            union
            {

                struct
                {
                    union
                    {
                        struct
                        {
                            uint16_t reg_id : 2;
                            uint16_t dir_flag : 1; // 0 - in  ,  1 - out
                            uint16_t opcode : 5;
                            uint16_t address : 8;
                        };
                        uint16_t InstShort;
                    };
                } Inst[16384]; // 32768 byte

                struct
                {
                    uint16_t data_flag : 1;  // always 1 since we will treat the Inst segment as data segment
                    uint16_t value : 15;
                } InstSeg[16384]; // 32768 byte

            };

            struct
            {
                uint16_t data_flag : 1;  // always 1
                uint16_t value : 15;
            } DataSeg[16384]; // 32768 byte
        };


        // just to access all possible memory addresses directly
        struct
        {
            uint16_t data_flag : 1;  // always 1
            uint16_t value : 15;
        } RomSeg[32768]; // 65536 byte

    };

    CROMBlock()
    {
        memset(this, 0, sizeof(CROMBlock)); // fill the RomBlock with zeroes
    }

    static void SetRomInst(tInstBlock* dst, tInstBlock* src)
    {
        dst->opcode = src->opcode;
        dst->dir_flag = src->dir_flag;
        dst->reg_id = src->reg_id;
        dst->address = src->address;
    }

};
static_assert(sizeof(CROMBlock) == 65536, "CROMBlock size is incorrect");

enum eRegID : int8_t
{
    AX,
    BX,
    CX,
    DX
};


enum eOpcode
{
    MOVE_IN = 1,
    MOVE_OUT = 2,
    LOAD = 3,
    INDIRECT_IN = 4,
    INDIRECT_OUT = 5
};
/*
int8_t GetRegID(std::string& lineReg)
{
    if (lineReg == "ax")
        return eRegID::AX;
    else if (lineReg == "bx")
        return eRegID::BX;
    else if (lineReg == "cx")
        return eRegID::CX;
    else if (lineReg == "dx")
        return eRegID::DX;
    else
        return -1;
}
*/

int8_t GetRegID(const char* lineReg)
{
    if (strcmp(lineReg,"ax") == 0)
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