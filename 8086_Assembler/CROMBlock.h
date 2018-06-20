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
                        uint8_t InstHigh;
                        uint8_t InstLow;
                    };
                } Inst[32768]; // 32768 byte low  and 32768 high

                struct
                {
                    uint16_t data_flag : 1;  // always 1 since we will treat the Inst segment as data segment
                    uint16_t value : 15;
                } InstSeg[32768]; // 32768 byte low  and 32768 high

            };

            struct
            {
                uint16_t data_flag : 1;  // always 1
                uint16_t value : 15;
            } DataSeg[32768]; // 32768 byte low  and 32768 high
        };


        // just to access all possible memory addresses directly
        struct
        {
            union
            {
                struct
                {
                    uint16_t data_flag : 1;  // always 1
                    uint16_t value : 15;
                };
                struct
                {
                    uint8_t RomSegHigh;
                    uint8_t RomSegLow;
                };
            };
        } RomSeg[32768 * 2]; // 65536 byte low and 65536 high

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
static_assert(sizeof(CROMBlock) == 65536*2 , "CROMBlock size is incorrect");



class CROMBlockHigh
{
public:
    union
    {
        struct
        {
            struct
            {
                union
                {
                    struct
                    {
                        uint8_t reg_id : 2;
                        uint8_t dir_flag : 1; // 0 - in  ,  1 - out
                        uint8_t opcode : 5;
                    };
                    uint8_t InstHighByte;
                };
            } Inst[32768]; // 32768 byte high

            struct
            {
                union
                {
                    struct
                    {
                        uint8_t data_flag : 1;  // always 1
                        uint8_t value_high : 7;
                    };
                    uint8_t DataHighByte;
                };

            } DataSeg[32768]; // 32768 byte high
        };


        // just to access all possible memory addresses directly
        struct
        {
            union
            {
                struct
                {
                    uint8_t data_flag : 1;  // always 1
                    uint8_t value_high : 7;
                };
                uint8_t RomHighByte;
            };

        } RomSeg[32768 * 2]; // 65536 byte high

    };

    CROMBlockHigh()
    {
        memset(this, 0, sizeof(CROMBlockHigh)); // fill the RomBlock with zeroes
    }

};
static_assert(sizeof(CROMBlockHigh) == 65536 , "CROMBlockHigh size is incorrect");


class CROMBlockLow
{
public:
    union
    {
        struct
        {

            struct
            {
                union
                {
                    struct
                    {
                        uint8_t address;
                    };
                    uint8_t InstLowByte;
                };
            } Inst[32768]; // 32768 byte low

            struct
            {
                union 
                {
                    struct 
                    {
                        uint8_t value_low;
                    };
                    uint8_t DataLowByte;
                };
                
            } DataSeg[32768]; // 32768 byte low
        };


        // just to access all possible memory addresses directly
        struct
        {
            union 
            {
                struct
                {
                    uint8_t value_low;
                };
                uint8_t RomLowByte;
            };
            
        } RomSeg[32768 * 2]; // 65536 byte low

    };

    CROMBlockLow()
    {
        memset(this, 0, sizeof(CROMBlockLow)); // fill the RomBlock with zeroes
    }

};
static_assert(sizeof(CROMBlockLow) == 65536, "CROMBlockHigh size is incorrect");


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