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
    for (int i = 0; static_cast<size_t>(i) < word.size(); i++){
        word[i] = tolower(word[i]);
    }
}
bool isWordDigit (string word){
    for (int i = 0; static_cast<size_t>(i) < word.size(); i++){
        if (!isdigit(word[i])){
            return false;
        }
    }
    return true;
}
string fileNameCheck (string fileName, string extension){
    int tmpSize = extension.size() + 1;
    if (fileName.size() > static_cast<size_t>(tmpSize)){
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
    string tmp = (const char*) word;
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
                    const char *manf = "Неизвестно";
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
                        if (!isEmpty(gpuInfo->Attribute("manf"))){
                            manf = gpuInfo->Attribute("manf");
                        }
                    }

                    lib.addNewGPU((const char*) name, intCode, intNup, intFreq, (const char*) manf);
                }
            }

            //MANUFACTURE
            for (TiXmlElement *manlist = database->FirstChildElement("manfList");
                 manlist;
                 manlist = manlist->NextSiblingElement("manfList")){
                for (TiXmlElement *manf = manlist->FirstChildElement("manf");
                     manf;
                     manf = manf->NextSiblingElement("manf")){
                    const char *site = "Неизвестно", *year;
                    int intYear = -1, intCode = -1;
                    const char *name = manf->Attribute("name");
                    if (isEmpty(name)){
                        name = "Неизвестно";
                    }
                    const char *code = manf->Attribute("code");
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *manfInfo = manf->FirstChildElement("manfInfo");
                    if (manfInfo){
                        year = manfInfo->Attribute("year");
                        if (!isEmpty(year)){
                            intYear = atoi(year);
                        }
                        if (!isEmpty(manfInfo->Attribute("site"))){
                            site = manfInfo->Attribute("site");
                        }
                    }

                    lib.addNewMRER((const char*) name, intCode, intYear, (const char*) site);
                }
            }

            //MEMORY
            for (TiXmlElement *memlist = database->FirstChildElement("memList");
                 memlist;
                 memlist = memlist->NextSiblingElement("memList")){
                for (TiXmlElement *mem = memlist->FirstChildElement("mem");
                     mem;
                     mem = mem->NextSiblingElement("mem")){
                    const char *type = "Неизвестно";
                    double dbBand = 0.0;
                    const char *code = mem->Attribute("code");
                    int intCode = -1, intMem = -1;
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *memInfo = mem->FirstChildElement("memInfo");
                    if (memInfo){
                        const char *memory = memInfo->Attribute("memory");
                        if (!isEmpty(memory)){
                            intMem = atoi(memory);
                        }
                        if (!isEmpty(memInfo->Attribute("type"))){
                            type = memInfo->Attribute("type");
                        }
                        const char *bandwidth = memInfo->Attribute("bandwidth");
                        if (!isEmpty(bandwidth)){
                            dbBand = atof(bandwidth);
                        }
                    }

                    lib.addNewMMR(intCode, intMem, (const char*) type, dbBand);
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
                        const char *manCode = gcInfo->Attribute("manCode");
                        if (!isEmpty(manCode)){
                            intManCode = atoi(manCode);
                        }
                        const char *gpuCode = gcInfo->Attribute("gpuCode");
                        if (!isEmpty(gpuCode)){
                            intGPUCode = atoi(gpuCode);
                        }
                        const char *memCode = gcInfo->Attribute("memCode");
                        if (!isEmpty(memCode)){
                            intMemCode = atoi(memCode);
                        }
                    }
                    lib.addNewGC((const char*) name, intCode, intManCode, intGPUCode, intMemCode);
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
// <> warning: unused parameter in static function
int callbackGPU(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        string name = colArg[0];
        int code = atoi(colArg[1]);
        int nup = atoi(colArg[2]);
        int freq = atoi(colArg[3]);
        string manf = colArg[4];
        tmp->addNewGPU(name, code, nup, freq, manf);
    }
    else {
        cout << "<> Ошибка! Запрос вернул пустую строку" << endl;
    }
    return 0;
}
//MRER HANDLER
// <> warning: unused parameter in static function
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
// <> warning: unused parameter in static function
int callbackMMR(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        int code = atoi(colArg[0]);
        int memory = atoi(colArg[1]);
        string type = colArg[2];
        double band = atof(colArg[3]);
        tmp->addNewMMR(code, memory, type, band);
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
        int manCode = atoi(colArg[2]);
        int gpuCode = atoi(colArg[3]);
        int memCode = atoi(colArg[4]);
        tmp->addNewGC(name, code, manCode, gpuCode, memCode);
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
    char *errMsg = 0;
    char *sql;
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
            // <> warning: ISO C++ converting string --> char *
            sql = "SELECT * FROM gpus;";
            tmpExec = sqlite3_exec(database, sql, callbackGPU, 0, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << lib.getListGPU()->getObjName() << " OK" << endl;
            // <> warning: ISO C++ converting string --> char *
            sql = "SELECT * FROM mrers;";
            tmpExec = sqlite3_exec(database, sql, callbackMRER, 0, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << lib.getListMRER()->getObjName() << " OK" << endl;
            // <> warning: ISO C++ converting string --> char *
            sql = "SELECT * FROM mmrs;";
            tmpExec = sqlite3_exec(database, sql, callbackMMR, 0, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << lib.getListMMR()->getObjName() << " OK" << endl;
            // <> warning: ISO C++ converting string --> char *
            sql = "SELECT * FROM gcs;";
            tmpExec = sqlite3_exec(database, sql, callbackGC, 0, &errMsg);
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

//Template for SQL query
template < typename T >
void execLibSQLquery (T inputLib, sqlite3 *database){
    string sql;
    char *errMsg;
    int tmpExec;

    for (int i = 0; i < inputLib->getSize(); i++){
        sql = inputLib->createSQLquery(i);
        try {
            tmpExec = sqlite3_exec(database, sql.c_str(), 0, 0, &errMsg);
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

//Writing to SQLite db
bool writeToSQL (string fileName, Catalog lib){
    fileName = fileNameCheck(fileName, "db");
    cout << "Файл: " << fileName << endl;
    sqlite3 *database;
    char *errMsg = 0;
    int tmpExec;

    tmpExec = sqlite3_open(fileName.c_str(), &database);
    if (tmpExec){
        cout << " <> Невозможно открыть базу данных: " << sqlite3_errmsg(database) << endl;
        return false;
    }
    else {
        cout << "База данных открыта успешно" << endl;
        const char *sql = "\
        DROP TABLE IF EXISTS gpus;    \
        DROP TABLE IF EXISTS mrers;    \
        DROP TABLE IF EXISTS mmrs;    \
        DROP TABLE IF EXISTS gcs;    \
        CREATE TABLE gpus (  \
            name text,  \
            code int,   \
            nup int,    \
            freq int,   \
            manf text,  \
            PRIMARY KEY (code)  \
        );  \
        CREATE TABLE mrers (  \
            name text,  \
            code int,   \
            year int,   \
            site text,  \
            PRIMARY KEY (code)  \
        );  \
        CREATE TABLE mmrs (   \
            code int,   \
            memory int, \
            type text,  \
            band float, \
            PRIMARY KEY (code)  \
        );  \
        CREATE TABLE gcs (    \
            name text,  \
            code int,   \
            manCode int,    \
            gpuCode int,    \
            memCode int,    \
            PRIMARY KEY (code)  \
        );";

        try {
            tmpExec = sqlite3_exec(database, sql, 0, 0, &errMsg);
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
