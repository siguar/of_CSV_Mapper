//
//  checkQrCodeInMap.h
//  csv2
//
//  Created by F on 21/11/2022.
//
#include <ctime>
#include <iostream>
#include <utility>
#include <map>
#include <algorithm>
#ifndef checkQrCodeInMap_h
#define checkQrCodeInMap_h


#endif /* checkQrCodeInMap_h */
//enum ReturnValues
//{
//    USED,
//    CORRECT,
//    ERROR
//};


struct MainArray
{
    std::string firstUseDate;
    std::string lastUseDate;
    int mainCounter = 0; //liczba użyć kodu QR
};
class Mapper
{
public:
    
//---------------------------------------------------------------------------------
    
    
    std::map <std::string/*kod QR jako klucz*/, MainArray/* ilość użyć - jeśli jest więcej niż zero znaczy że kod został wykorzystany*/> mainMap;
    Mapper()
    {
        if(xcsv.load("XX.csv", ";"))
        {
            for(int i = 0; i < xcsv.getNumRows(); ++i)
            {
                int valueofcheck = 0;
                std::vector <std::string> vec1 = xcsv.getRow(i).getData();
                
                MainArray tempMainArray;
                tempMainArray.mainCounter = std::stoi(vec1[1]);
                tempMainArray.firstUseDate = vec1[2];
                tempMainArray.lastUseDate = vec1[3];
                mainMap.insert(std::make_pair(vec1[0], tempMainArray));
            }
        }
        else
        {               //"db\qrcodes.csv"
            
            xcsv.load("db\qrcodes.csv", ";");
            for(int i = 0; i < xcsv.getNumRows(); ++i)
            {
                int valueofcheck = 0;
                MainArray tempMainArray;
                mainMap.insert(std::make_pair(xcsv.getRow(i).toString(), tempMainArray));
                
            }
           
        }
        
      
    }
    void replaceAll(std::string& str, const std::string& from, const std::string& to) //funkcja usuwająca entery generowane przez ofxcsv w pliku csv(to wyklucza problem z konwersją stringa na int)
    {
        if(from.empty())
            return;
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
    
    bool checkQrCode(const std::string &QrValue)
    {   auto clock_start_time = std::chrono::high_resolution_clock::now();
                     if(!mainMap.count(QrValue))
                     {
                         std::cout << "QR CODE NOT FOUND\n\nPLEASE CORRECT\n\n\n";
                         //return ReturnValues::ERROR ;
                         auto end_time = std::chrono::high_resolution_clock::now();
                         auto time = end_time - clock_start_time;
                         std::cout << "\nczas wykonania wyszukiwania kodu\n" << time/std::chrono::microseconds(1);
                         return false;
                     }
        
                     int c;
                     mainMap[QrValue].mainCounter++;
                     if(mainMap[QrValue].mainCounter > 1)
                     {
                         std::cout << "ITS QR CODE: " << QrValue << " " << mainMap[QrValue].mainCounter << "\n\n" << "THIS CODE IS USED";
                         //return ReturnValues::USED;
                         time_t now = time(0);
                         char* td = ctime(&now);
                         mainMap[QrValue].lastUseDate = std::string(td);
                         auto end_time = std::chrono::high_resolution_clock::now();
                         auto time = end_time - clock_start_time;
                         std::cout << "\nczas wykonania wyszukiwania kodu\n" << time/std::chrono::microseconds(1);
                         saverOfFile();
                         return false;
                     }else{
                         std::cout << "CODE   " << QrValue << "   APPROVE\n\n";
                         time_t now = time(0);
                         char* td = ctime(&now);
                         mainMap[QrValue].lastUseDate = std::string(td);
                        
                         mainMap[QrValue].firstUseDate = std::string(td);
                         //return ReturnValues::CORRECT;
                         auto end_time = std::chrono::high_resolution_clock::now();
                         auto time = end_time - clock_start_time;
                         std::cout << "\nczas wykonania wyszukiwania kodu\n" << time/std::chrono::microseconds(1);
                         saverOfFile();
                         return true;
                     }
        
                  
        
    }
    void saverOfFile()     //map saving to csv file
    {
        ofxCsv toSave;
        
        for(auto& item : mainMap)
        {
            ofxCsvRow tempRow;
            std::vector<std::string> tempVec;
            tempVec.push_back(item.first);
            tempVec.push_back(std::to_string(item.second.mainCounter));
            replaceAll(item.second.firstUseDate, std::string (1, '\n'), "");
            replaceAll(item.second.lastUseDate, std::string (1, '\n'), "");
            tempVec.push_back(item.second.firstUseDate);
            tempVec.push_back(item.second.lastUseDate);
            tempRow.load(tempVec);
            toSave.addRow(tempRow);
         }
        
        toSave.save("XX.csv", true, ";");
    }
ofxCsv xcsv;
};
