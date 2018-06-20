#pragma once
#include "stdafx.h"
#include "COpcode.h"
#include "CROMBlock.h"

bool IsCommentLine(std::string& line,char* linebuff)
{
    for (int i = 0; i < line.capacity(); i++)
    {
        char first = linebuff[i];
        if (first != ' '
            && first != '/'
            && first != '\t'
            && first != '\n'
            && first != ';')
            return false;

        if (first == '/' || first == ';')
        {
            return true;
        }

    }
    return false;
}