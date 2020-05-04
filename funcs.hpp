#ifndef FUNCS_H
#define FUNCS_H
#define FPATH "" //Путь к папке с файлами. По-умолчанию, файлы в папке с исполняемым файлом.

#include <iostream>
#include <string>
#include <bones.h>
#include <fstream>
//Tinyxml lib
#include "tinyxml/tinystr.h"
#include "tinyxml/tinystr.cpp"
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinyxml.cpp"
#include "tinyxml/tinyxmlerror.cpp"
#include "tinyxml/tinyxmlparser.cpp"
//SQLite lib
#include <sqlite3.h>

using namespace std;

string fillTab (int count, char sim){
    string str;
    for (int i = 0; i < count; i++){
        str += sim;
    }
    return str;
}
void wordToLow (string &word){
    for (size_t i = 0; i < word.size(); i++){
        word[i] = static_cast<char>(tolower(word[i]));
    }
}
bool isWordDigit (string word){
    for (size_t i = 0; i < word.size(); i++){
        if (!isdigit(word[i])){
            return false;
        }
    }
    return true;
}
string fileNameCheck (string fileName, string extension){
    size_t tmpSize = extension.size() + 1;
    if (fileName.size() > tmpSize){
        string tmpStr = fileName.substr(fileName.size()-tmpSize, tmpSize);
        if (tmpStr != ("." + extension)){
            fileName += ("." + extension);
        }
    }
    else {
        fileName += ("." + extension);
    }
    fileName = FPATH + fileName;

    return fileName;
}

bool isEmpty (const char * word) {
    string tmp = static_cast<const char*>(word);
    return (tmp.size() == 0 ? true : false);
}

//Reading from XML file
bool readFromXML (string fileName, Catalog lib){
    fileName = fileNameCheck(fileName, "xml");
    cout << "Файл: " << fileName << endl;
    TiXmlDocument document(fileName.c_str());
    document.LoadFile(TIXML_ENCODING_UTF8);
    if (!document.LoadFile()){
        cout << " <> Ошибка, файл " << fileName << " не найден" << endl;
        return false;
    }
    else {
        TiXmlElement *database = document.FirstChildElement("database");
        if (database){
            //GPU
            for (TiXmlElement *gpulist = database->FirstChildElement("gpuList");
                 gpulist;
                 gpulist = gpulist->NextSiblingElement("gpuList")){
                for (TiXmlElement *gpu = gpulist->FirstChildElement("gpu");
                     gpu;
                     gpu = gpu->NextSiblingElement("gpu")){
                    int intCode = -1, intNup = -1, intFreq = -1;
                    const char *mrer = "Неизвестно";
                    const char *name = gpu->Attribute("name");
                    if (isEmpty(name)){
                        name = "Неизвестно";
                    }
                    const char *code = gpu->Attribute("code");
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *gpuInfo = gpu->FirstChildElement("gpuInfo");
                    if (gpuInfo){
                        const char *nup = gpuInfo->Attribute("nup");
                        if (!isEmpty(nup)){
                            intNup = atoi(nup);
                        }
                        const char *freq = gpuInfo->Attribute("freq");
                        if (!isEmpty(freq)){
                            intFreq = atoi(freq);
                        }
                        if (!isEmpty(gpuInfo->Attribute("mrer"))){
                            mrer = gpuInfo->Attribute("mrer");
                        }
                    }

                    lib.addNewGPU(static_cast<const char*>(name), intCode, intNup, intFreq, static_cast<const char*>(mrer));
                }
            }

            //MANUFACTURE
            for (TiXmlElement *manlist = database->FirstChildElement("mrerList");
                 manlist;
                 manlist = manlist->NextSiblingElement("mrerList")){
                for (TiXmlElement *manf = manlist->FirstChildElement("mrer");
                     manf;
                     manf = manf->NextSiblingElement("mrer")){
                    const char *site = "Неизвестно", *year;
                    unsigned short shortYear = 0;
                    int intCode = -1;
                    const char *name = manf->Attribute("name");
                    if (isEmpty(name)){
                        name = "Неизвестно";
                    }
                    const char *code = manf->Attribute("code");
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *manfInfo = manf->FirstChildElement("mrerInfo");
                    if (manfInfo){
                        year = manfInfo->Attribute("fYear");
                        if (!isEmpty(year)){
                            shortYear = static_cast<unsigned short>(atoi(year));
                        }
                        if (!isEmpty(manfInfo->Attribute("site"))){
                            site = manfInfo->Attribute("site");
                        }
                    }

                    lib.addNewMRER(static_cast<const char*>(name), intCode, shortYear, static_cast<const char*>(site));
                }
            }

            //MEMORY
            for (TiXmlElement *memlist = database->FirstChildElement("mmrList");
                 memlist;
                 memlist = memlist->NextSiblingElement("mmrList")){
                for (TiXmlElement *mem = memlist->FirstChildElement("mmr");
                     mem;
                     mem = mem->NextSiblingElement("mmr")){
                    const char *type = "Неизвестно";
                    double dbBand = 0.0;
                    const char *code = mem->Attribute("code");
                    int intCode = -1, intFreq = 0;
                    unsigned short shortMem = 0;
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *memInfo = mem->FirstChildElement("mmrInfo");
                    if (memInfo){
                        const char *memory = memInfo->Attribute("memory");
                        if (!isEmpty(memory)){
                            shortMem = static_cast<unsigned short>(atoi(memory));
                        }
                        if (!isEmpty(memInfo->Attribute("type"))){
                            type = memInfo->Attribute("type");
                        }
                        const char *bandwidth = memInfo->Attribute("bandwidth");
                        if (!isEmpty(bandwidth)){
                            dbBand = atof(bandwidth);
                        }
                        const char *freq = memInfo->Attribute("freq");
                        if (!isEmpty(freq)){
                            intFreq = atoi(freq);
                        }
                    }

                    lib.addNewMMR(intCode, shortMem, static_cast<const char*>(type), dbBand, intFreq);
                }
            }

            //VIDEOCARDS
            for (TiXmlElement *gclist = database->FirstChildElement("gcList");
                 gclist;
                 gclist = gclist->NextSiblingElement("gcList")){
                for (TiXmlElement *gc = gclist->FirstChildElement("gc");
                     gc;
                     gc = gc->NextSiblingElement("gc")){
                    int intCode = -1, intManCode = -1,  intGPUCode = -1, intMemCode = -1;
                    const char *name = gc->Attribute("name");
                    if (isEmpty(name)){
                        name = "Неизвестно";
                    }
                    const char *code = gc->Attribute("code");
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *gcInfo = gc->FirstChildElement("gcInfo");
                    if (gcInfo){
                        const char *manCode = gcInfo->Attribute("mrerCode");
                        if (!isEmpty(manCode)){
                            intManCode = atoi(manCode);
                        }
                        const char *gpuCode = gcInfo->Attribute("gpuCode");
                        if (!isEmpty(gpuCode)){
                            intGPUCode = atoi(gpuCode);
                        }
                        const char *memCode = gcInfo->Attribute("mmrCode");
                        if (!isEmpty(memCode)){
                            intMemCode = atoi(memCode);
                        }
                    }
                    lib.addNewGC(static_cast<const char*>(name), intCode, intManCode, intGPUCode, intMemCode);
                }
            }
        }
        else {
            cout << "Неккоретный файл" << endl;
            return false;
        }
//obsolete *_*
//        listGC->refresh(*&listGPU, *&listMR, *&listMMR);
        return true;
    }
}

//Writing to XML file
void writeToXML (string fileName, Catalog lib){
    fileName = fileNameCheck(fileName, "xml");
    ofstream fileOUT;
    fileOUT.open(fileName.c_str());
    cout << "Файл: " << fileName << endl;

    fileOUT << "<?xml version='1.0' encoding=\"utf-8\" standalone=\"yes\"?>" << endl;
    fileOUT << endl;
    fileOUT << "<database>" << endl;

    if(lib.getListGPU()->getSize() != 0){
        fileOUT << "\t<gpuList>" << endl;
        fileOUT << lib.getListGPU()->printXML();
        fileOUT << "\t</gpuList>" << endl;
    }
    if(lib.getListMMR()->getSize() != 0){
        fileOUT << "\t<memList>" << endl;
        fileOUT << lib.getListMMR()->printXML();
        fileOUT << "\t</memList>" << endl;
    }
    if(lib.getListMRER()->getSize() != 0){
        fileOUT << "\t<manfList>" << endl;
        fileOUT << lib.getListMRER()->printXML();
        fileOUT << "\t</manfList>" << endl;
    }
    if(lib.getListGC()->getSize() != 0){
        fileOUT << "\t<gcList>" << endl;
        fileOUT << lib.getListGC()->printXML();
        fileOUT << "\t</gcList>" << endl;
    }

    fileOUT << "</database>" << endl;
    fileOUT.close();
}

//GLOBAL Catalog var
static Catalog *tmp;

//CALLBACK for sqlite3_exec
//GPU HANDLER
//Предупреждение о неизбежной устарелой переменной
int callbackGPU(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        string name = colArg[0];
        int code = atoi(colArg[1]);
        int nup = atoi(colArg[2]);
        int freq = atoi(colArg[3]);
        string mrer = colArg[4];
        tmp->addNewGPU(name, code, nup, freq, mrer);
    }
    else {
        cout << "<> Ошибка! Запрос вернул пустую строку" << endl;
    }
    return 0;
}
//MRER HANDLER
//Предупреждение о неизбежной устарелой переменной
int callbackMRER(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        string name = colArg[0];
        int code = atoi(colArg[1]);
        int year = atoi(colArg[2]);
        string site = colArg[3];
        tmp->addNewMRER(name, code, year, site);
    }
    else {
        cout << "<> Ошибка! Запрос вернул пустую строку" << endl;
    }
    return 0;
}
//MMR HANDLER
//Предупреждение о неизбежной устарелой переменной
int callbackMMR(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        int code = atoi(colArg[0]);
        unsigned short memory = static_cast<unsigned short>(atoi(colArg[1]));
        string type = colArg[2];
        double band = atof(colArg[3]);
        int freq = atoi(colArg[4]);
        tmp->addNewMMR(code, memory, type, band, freq);
    }
    else {
        cout << "<> Ошибка! Запрос вернул пустую строку" << endl;
    }
    return 0;
}
//GC HANDLER
// <> warning: unused parameter in static function
int callbackGC(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        string name = colArg[0];
        int code = atoi(colArg[1]);
        int mrerCode = atoi(colArg[2]);
        int gpuCode = atoi(colArg[3]);
        int mmrCode = atoi(colArg[4]);
        tmp->addNewGC(name, code, mrerCode, gpuCode, mmrCode);
    }
    else {
        cout << "<> Ошибка! Запрос вернул пустую строку" << endl;
    }
    return 0;
}

//Reading from SQLite db
bool readFromSQL (string fileName, Catalog lib){
    fileName = fileNameCheck(fileName, "db");

    cout << "Файл: " << fileName << endl;
    ifstream fileCheck(fileName.c_str());
    if (!fileCheck.is_open()){
        cout << " <> Ошибка, файл " << fileName << " не найден" << endl;
        fileCheck.close();
        return false;
    }
    else {
        fileCheck.close();
    }

    sqlite3 *database;
    char *errMsg = nullptr;
    const char *sql;
    int tmpExec;
    tmp = &lib;

    tmpExec = sqlite3_open(fileName.c_str(), &database);
    if (tmpExec){
        cout << " <> Невозможно открыть базу данных: " << sqlite3_errmsg(database) << endl;
        return false;
    }
    else {
        cout << "База данных открыта успешно" << endl;
        try {
            sql = "SELECT * FROM GPU;";
            tmpExec = sqlite3_exec(database, sql, callbackGPU, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << lib.getListGPU()->getObjName() << " OK" << endl;
            sql = "SELECT * FROM MRER;";
            tmpExec = sqlite3_exec(database, sql, callbackMRER, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << lib.getListMRER()->getObjName() << " OK" << endl;
            sql = "SELECT * FROM MMR;";
            tmpExec = sqlite3_exec(database, sql, callbackMMR, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << lib.getListMMR()->getObjName() << " OK" << endl;
            sql = "SELECT * FROM GC;";
            tmpExec = sqlite3_exec(database, sql, callbackGC, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << lib.getListGC()->getObjName() << " OK" << endl;
        }
        catch (char *errLog){
            cout << " <> Ошибка! Не выполнен запрос: " << sql << endl;
            cout << " <> Ошибка SQL: " << errLog << endl;
            cout << " <> Чтение прервано" << endl;
            sqlite3_free(errMsg);
        }
    }
    sqlite3_close(database);
    return true;
}

//Шаблон sql запроса
template < typename T >
void execLibSQLquery (T inputLib, sqlite3 *database){
    string sql;
    char *errMsg;
    int tmpExec;

    for (int i = 0; i < inputLib->getSize(); i++){
        sql = inputLib->createSQLquery(i);
        try {
            tmpExec = sqlite3_exec(database, sql.c_str(), nullptr, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
        }
        catch (char *errLog){
            cout << " <> Ошибка! Не выполнен запрос: " << sql << endl;
            cout << " <> Ошибка SQL: " << errLog << endl;
            sqlite3_free(errMsg);
        }
    }
    cout << "\t" << inputLib->getObjName() << " OK" << endl;
}

//Запись в бд SQLite
bool writeToSQL (string fileName, Catalog lib){
    fileName = fileNameCheck(fileName, "db");
    cout << "Файл: " << fileName << endl;
    sqlite3 *database;
    char *errMsg = nullptr;
    int tmpExec;

    tmpExec = sqlite3_open(fileName.c_str(), &database);
    if (tmpExec){
        cout << " <> Невозможно открыть базу данных: " << sqlite3_errmsg(database) << endl;
        return false;
    }
    else {
        cout << "База данных открыта успешно" << endl;
        const char *sql = "\
        DROP TABLE IF EXISTS GPU;    \
        DROP TABLE IF EXISTS MRER;    \
        DROP TABLE IF EXISTS MMR;    \
        DROP TABLE IF EXISTS GC;    \
        PRAGMA foreign_keys = ON;\
        CREATE TABLE IF NOT EXISTS GPU (\
            code INTEGER PRIMARY KEY,\
            name TEXT UNIQUE,\
            nup INTEGER,\
            freq INTEGER,\
            mrer TEXT\
        );\
        CREATE TABLE IF NOT EXISTS MRER (\
            code INTEGER PRIMARY KEY,\
            name TEXT UNIQUE,\
            fYear SMALLINT UNSIGNED,\
            site TEXT\
        );\
        CREATE TABLE IF NOT EXISTS MMR (\
            code INTEGER PRIMARY KEY,\
            memory SMALLINT UNSIGNED,\
            bandwidth DOUBLE,\
            freq INTEGER,\
            type TEXT\
        );\
        CREATE TABLE IF NOT EXISTS GC (\
            code INTEGER PRIMARY KEY,\
            name TEXT,\
            mrerCode INTEGER REFERENCES MRER(code),\
            gpuCode INTEGER REFERENCES GPU(code),\
            mmrCode INTEGER REFERENCES MMR(code)\
        );";

        try {
            tmpExec = sqlite3_exec(database, sql, nullptr, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            else {
                cout << "\tСтруктуры таблиц OK" << endl;
                execLibSQLquery(lib.getListGPU(), database);
                execLibSQLquery(lib.getListMMR(), database);
                execLibSQLquery(lib.getListMRER(), database);
                execLibSQLquery(lib.getListGC(), database);
            }
        }
        catch (char *errLog){
            cout << " <> Ошибка! Не выполнен запрос: " << sql << endl;
            cout << " <> Ошибка SQL: " << errLog << endl;
            sqlite3_free(errMsg);
            return false;
        }
    }
    sqlite3_close(database);
    return true;
}

#endif // FUNCS_H
