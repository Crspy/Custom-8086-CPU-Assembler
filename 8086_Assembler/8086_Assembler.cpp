// 8086_Assembler.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CROMBlock.h"

#define VERBOSE

#ifdef VERBOSE
#define logger(x) std::cout << (x) << "\n"
#else
#define logger(x) //
#endif




int main(int argc,char* argv[])
{
    //FILE* myfile;
    //char* mybuff;
    long fileSize;
    //size_t result;
    //char* mytok;
    std::string line;
    CROMBlock myrom;
    long linecount = 0;



    int PC = 0;
    
    if (argc < 2) return 0;

    std::ifstream myfile;
    try
    {
        myfile.open(argv[1], std::ios::in);
    }
    catch (std::exception& e) { std::cout << e.what() << "\n"; }

    myfile.seekg(0, std::ios_base::end);
    fileSize = myfile.tellg();
    myfile.clear();
    myfile.seekg(0);
    //std::cout << fileSize << "\n";
    //mybuff = (char*)malloc(fileSize * sizeof(char));
    while (std::getline(myfile, line))
    {
        tInstBlock currentInst[2];
        linecount++;
        char* linebuff = (char*)malloc(line.capacity());
        strcpy(linebuff,line.c_str());
        char* token;
        bool bMovingData = false;
        tMemAddress memadd;
        int8_t reg;
        std::string linecheck = line;
        std::stringstream ss(linecheck);
        std::getline(ss, linecheck, ',');

        //std::cout << linecheck << '\n';


        if (line.find_first_not_of(' ') == std::string::npos)   continue;

        if (std::any_of(linecheck.begin(), linecheck.end(), ::isdigit))
        {
            std::stringstream ss(line);
            token = strtok(linebuff, " [], \t");
            logger(token);
            //std::getline(ss, line, ' ');          
            if (strcmp(token, "mov") == 0)
            {
                currentInst[0].opcode = eOpcode::MOVE_OUT;
                //std::cout << "MOVE OUT" << '\n';
                currentInst[0].dir_flag = 1;
            }
            else if (strcmp(token, "imov") == 0)
            {
                // not implemented yet
                return 0;
            }
            else
            {
                std::cout << "Compiler Error, Unknown opcode in line : " << linecount << '\n';
                std::cout << "Press Any Key to exit..." << '\n';
                std::cin.get();
                return 0;
            }


            token = strtok(NULL, " [], \t");
            logger(token);
            //std::getline(ss, line, ']');
            memadd.m_Address = strtol(token, NULL, 0);
            logger(memadd.m_Address);
            if (!memadd.InsureAddress())
            {
                std::cout << "Compiler Error , Memory Address exceeds 2 bytes... in line : " << linecount << '\n';
                std::cout << "Press Any Key to exit..." << '\n';
                std::cin.get();
                return 0;
            }
            if (memadd.m_bNeedLoading)
            {
                currentInst[1].address = memadd.byte0; // lowbyte for mov
                currentInst[1].opcode = currentInst[0].opcode;
                currentInst[1].dir_flag = currentInst[0].dir_flag;
                currentInst[0].opcode = eOpcode::LOAD;
                currentInst[0].dir_flag = 0;
                currentInst[0].address = memadd.byte1; // highbyte for load
            }
            else
            {
                currentInst[0].address = memadd.byte0;
            }


            token = strtok(NULL, " [], \t");
            logger(token);
            //std::getline(ss, line, '\n');
            reg = GetRegID(token);
            if (reg == -1) // then it's moving value to dataSeg
            {
                
                unsigned short _value;
                try
                {
                    _value = strtol(token,NULL,0);
                    logger("VALUE");
                    logger(_value);
                }
                catch (std::exception& e)
                {
                    std::cout << e.what() << "\n";
                    std::cout << "Compiler Error , unknown register name....in line : " << linecount << '\n';
                    std::cout << "Press Any Key to exit..." << '\n';
                    std::cin.get();
                    return 0;
                }

                if (_value > 0x7FFF)
                {
                    std::cout << "Compiler Error , data value is Out Of Bounds 15bits....in line : " << linecount << '\n';
                    std::cout << "Press Any Key to exit..." << '\n';
                    std::cin.get();
                    return 0;
                }
                else
                {
                    bMovingData = true;
                    memadd.m_bNeedLoading = false;
                    myrom.RomSeg[memadd.m_Address / 2].value = _value;
                    myrom.RomSeg[memadd.m_Address / 2].data_flag = 1;
                    continue;
                }

            }

        }
        else
        {
            std::stringstream ss(line);
            token = strtok(linebuff, " ,[]/");
            logger(token);
            // std::getline(ss, line, ' ');
            if (strcmp(token, "mov") == 0)
            {
                currentInst[0].opcode = eOpcode::MOVE_IN;
                //std::cout << "MOVE IN" << '\n';
                currentInst[0].dir_flag = 0;
            }
            else if (strcmp(token, "imov") == 0)
            {
                // not implemented yet
            }
            else
            {
                std::cout << "Compiler Error, Unknown opcode in line : " << linecount << '\n';
                std::cout << "Press Any Key to exit..." << '\n';
                std::cin.get();
                return 0;
            }
            token = strtok(NULL, " ,[]/");
            logger(token);
 
            reg = GetRegID(token);

            token = strtok(NULL, " ,[]/");
            logger(token);
            //std::getline(ss, line, ']');
            memadd.m_Address = strtol(token, NULL, 0);
            
            if (!memadd.InsureAddress())
            {
                std::cout << "Compiler Error , Memory Address exceeds 2 bytes...in line : " << linecount << '\n';
                std::cout << "Press Any Key to exit..." << '\n';
                std::cin.get();
                return 0;
            }
            if (memadd.m_bNeedLoading)
            {
                currentInst[1].address = memadd.byte0; // lowbyte for mov
                currentInst[1].opcode = currentInst[0].opcode;
                currentInst[1].dir_flag = currentInst[0].dir_flag;
                currentInst[0].opcode = eOpcode::LOAD;
                currentInst[0].dir_flag = 0;
                currentInst[0].address = memadd.byte1; // highbyte for load
            }

            else
            {
                currentInst[0].address = memadd.byte0;
            }



            if (reg == -1)
            {
                std::cout << "Compiler Error , unknown register name...in line : " << linecount << '\n';
                std::cout << "Press Any Key to exit..." << '\n';
                std::cin.get();
                return 0;
            }
        }
        //printf("PC COUNT : %d\n", PC);
        //std::cout << memadd.m_Address << '\n';
        std::cout << (memadd.m_bNeedLoading ? "true" : "false") << '\n';
        //printf("%d\n", reg);


        if (memadd.m_bNeedLoading)
        {
            currentInst[1].reg_id = reg;
            CROMBlock::SetRomInst((tInstBlock*)&myrom.Inst[PC], &currentInst[0]);
            CROMBlock::SetRomInst((tInstBlock*)&myrom.Inst[PC+1], &currentInst[1]);
            printf("%d , %d, %d, %d\n", currentInst[0].opcode, currentInst[0].dir_flag, currentInst[0].reg_id, currentInst[0].address);
            printf("%d , %d, %d, %d\n", currentInst[1].opcode, currentInst[1].dir_flag, currentInst[1].reg_id, currentInst[1].address);
            PC += 2;
        }
        else
        {
            currentInst[0].reg_id = reg;
            CROMBlock::SetRomInst((tInstBlock*)&myrom.Inst[PC], &currentInst[0]);
            printf("%d , %d, %d, %d\n", currentInst[0].opcode, currentInst[0].dir_flag, currentInst[0].reg_id, currentInst[0].address);
            PC++;
        }

        if (PC > (32767)) // 32768 - 1  
        {
            std::cout << "Compiler Error , Rom Instruction Segment overflow" << '\n';
            std::cout << "Possible Solution : Reduce your code lines" << '\n';
            std::cout << "Press Any Key to exit..." << '\n';
            std::cin.get();
            return 0;
        }
        

    }

    myfile.close();
    printf("linecount : %d\n", linecount);
    printf("PC COUNT : %d\n", PC);
    
    CROMBlockHigh highrom;
    CROMBlockLow lowrom;

    for (int i = 0; i < 65536; i++)
    {
        highrom.RomSeg[i].RomHighByte = myrom.RomSeg[i].RomSegHigh;
        lowrom.RomSeg[i].RomLowByte = myrom.RomSeg[i].RomSegLow;
    }

    
    
    std::ofstream highromfile;
    highromfile.open("high.bin", std::ios::binary | std::ios::out);
    highromfile.write((char*)&highrom, sizeof(highrom));
    highromfile.close();

    std::ofstream lowromfile;
    lowromfile.open("low.bin", std::ios::binary | std::ios::out);    
    lowromfile.write((char*)&lowrom, sizeof(lowrom));        
    lowromfile.close();
    
    myfile.close();

    std::cout << "Build Done !" << '\n';
    std::cout << "Press Any Key to exit..." << '\n';
    std::cin.get();



    /*
    if (argc == 1) return 0;

    if ((myfile = fopen(argv[1], "rb")) == NULL) {
        return -1;
    }


    fseek(myfile, 0, SEEK_END);
    fileSize = ftell(myfile);
    rewind(myfile);
    
    mybuff = (char*)malloc(sizeof(char)*fileSize);
    if (mybuff == NULL) { fputs("Memory error", stderr); exit(2); }

    // copy the file into the buffer:
    result = fread(mybuff, 1, fileSize, myfile);
    if (result != fileSize) { fputs("Reading error", stderr); exit(3); }

    getchar();
    */

    return 0;
}

