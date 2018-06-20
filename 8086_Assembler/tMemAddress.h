#pragma once
#include "stdafx.h"

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

    tMemAddress()
    {
        this->m_Address = 0;
        this->m_bNeedLoading = false;
    }

    bool InsureAddress()
    {
        if (this->m_Address > 0xFFFF)   return false;

        this->m_bNeedLoading = (this->m_Address > 0xFF) ? true : false;

        return true;
    }

};
static_assert(sizeof(tMemAddress) == 8, "tMemAddress size is incorrect");
