#pragma once

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
                    int16_t value;
                } InstSeg[32768]; // 32768 byte low  and 32768 high

            };

            struct
            {
                int16_t value;
            } DataSeg[32768]; // 32768 byte low  and 32768 high
        };


        // just to access all possible memory addresses directly
        struct
        {
            union
            {
                struct
                {
                    int16_t value;                 
                };
                struct
                {
                    int8_t RomSegHigh;
                    int8_t RomSegLow;
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
                       int8_t value_high;                      
                    };

                    int8_t DataHighByte;
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
                    int8_t value_high;
                    
                };
                int8_t RomHighByte;
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
                        int8_t value_low;
                    };
                    int8_t DataLowByte;
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
                    int8_t value_low;
                };
                int8_t RomLowByte;
            };
            
        } RomSeg[32768 * 2]; // 65536 byte low

    };

    CROMBlockLow()
    {
        memset(this, 0, sizeof(CROMBlockLow)); // fill the RomBlock with zeroes
    }

};
static_assert(sizeof(CROMBlockLow) == 65536, "CROMBlockHigh size is incorrect");


