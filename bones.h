#ifndef BONES_H
#define BONES_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
//Библиотека SQLite из пакета sqlite3
#include <sqlite3.h>

using std::string;
using std::vector;
using std::to_string;

class Catalog;


//Путь к папке с файлами. По-умолчанию, файлы в папке с исполняемым файлом.
const string FPATH = "";
//Предупреждение о неиспользуемой переменной
static Catalog *currentCatalog;


namespace funcs {
    //Заполнения вывода n-нным кол-во символов char
    string fillTab (int, char);
    //Преобр. строки в нижний регистр
    void wordToLow (string &);
    //Проверка строка == число
    bool isWordDigit (string);
    //Проверка имени файла (имя, расширение)
    string fileNameCheck (string, string);

    bool isEmpty (const char*);
}


/////////////////////
//Родительский класс
/////////////////////
class Base {
protected:
    int *code;
    string *name;
public:
    Base();
    virtual ~Base() = 0;
    void setName(string);
    void setCode(int);
    string *getName();
    int *getCode();
    virtual string writeInfoToXML() = 0;
    virtual string printSQLquery() = 0;
};

/////////////////////
//Класс GPU
/////////////////////
class GPU : public Base {
private:
    //Кол-во универсальных процессов? (Number of universal processors)
    int *nup;
    //Штат. частота (MHz)
    int *freq;
    //Производитель (AMD, NVIDIA, INTEL, OTHER)
    string *mrer;
    //Далее служебные поля:
    string *image;
public:
    GPU(string, int, int, int, string);
    GPU();
    ~GPU();
    void defPointers();
    void setGPUInfo(string, int, int, int, string);
    void setNUP(int);
    void setFreq(int);
    void setMRER(string);
    void setImage();
    string getInfo();
    int *getNUP();
    int *getFreq();
    string *getMRER();
    string *getImagePath();
    //XML
    string writeInfoToXML();
    //SQL
    string printSQLquery();
};

/////////////////////
//Класс MRER
/////////////////////
class MRER : public Base {
private:
    //Год основания (Надеемся, что smallint sql равносилен short)
    unsigned short *fYear;
    //Строка сайт производителя
    string *site;
    //Далее служебные поля:
    string *image;
public:
    MRER(string, int, unsigned short, string);
    MRER();
    ~MRER();
    void defPointers();
    void setMRERInfo(string, int, unsigned short, string);
    void setFYear(unsigned short);
    void setName(string);
    void setSite(string);
    void setImage();
    string getInfo();
    unsigned short *getFYear();
    string *getSite();
    string *getImagePath();
    //XML
    string writeInfoToXML();
    //SQL
    string printSQLquery();
};

/////////////////////
//Класс MMR
/////////////////////
class MMR : public Base {
private:
    //Объём памяти
    unsigned short *memory;
    //Пропуск. способность
    double *bandwidth;
    //Штат. частота (MHz)
    int *freq;
    //Тип памяти
    string *type;
public:
    MMR(int, unsigned short, string, double, int);
    MMR();
    ~MMR();
    void defPointers();
    void setMMRInfo(int, unsigned short, string, double, int);
    void setMemory(unsigned short);
    void setBandW(double);
    void setType(string);
    void setFreq(int);
    string getInfo();
    unsigned short *getMemory();
    double *getBandW();
    string *getType();
    int *getFreq();
    //XML
    string writeInfoToXML();
    //SQL
    string printSQLquery();
};


/////////////////////
//BASELIST TEMPLATE
/////////////////////
//Шаблон базового списка
//используется на простых классах
//Manufacturer(MRER), Memory(MMR), GPU
//Родитель для GCardList
////////////////////
template < typename T >
class BaseList {
protected:
    vector < T* > list;
    string objName = "Объект";
public:
    BaseList (){
    }
    ~BaseList (){
    }
    T *operator [] (const int index){
        if(index <= list.size()){
            return list[index];
        }
        else {
            return nullptr;
        }
    }
    T *findByCode (int code) {
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            if (*list[i]->getCode() == code){
                return list[i];
            }
        }
        throw this->objName + " с кодом " + to_string(code) + " отсутствует в списке";
    }
    void addNew (T *tmpCard){
        list.push_back(tmpCard);
    }
    void clearAll () {
        for (T *iter : list){
            delete iter;
        }
        //lol here is list of nullptr
        list.clear();
    }
    int getSize (){
        return static_cast<int>(list.size());
    }
    void deleteElement (int code){
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            if (*list[i]->getCode() == code){
                delete list[i];
                list.erase(list.begin() + i);
                return;
            }
        }
        throw this->objName + " с кодом " + to_string(code) + " отсутствует в списке";
    }
    string printAll(){
        string tmpStr;
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            tmpStr += list[i]->getInfo();
            tmpStr += "\n";
        }
        return tmpStr;
    }
    string printXML(){
        string strOut;
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            strOut += list[i]->writeInfoToXML();
        }
        return strOut;
    }
    string createSQLquery(int position){
        string strOut = "";
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            if (i == position){
                strOut = list[i]->printSQLquery();
            }
        }
        return strOut;
    }
    bool checkCode(int code){
        for (int i = 0; i < list.size(); i++){
            if (*list[i]->getCode() == code){
                return false;
            }
        }
        return true;
    }
    void setObjName (string objName){
        this->objName = objName;
    }
    string getObjName (){
        return this->objName;
    }
};

/////////////////////
//Класс GCARD
/////////////////////
class GCard : public Base {
private:
    string *correctName;
    string *image;
    int *codeMRER, *codeGPU, *codeMMR;
    GPU *currGPU;
    MRER *currMRER;
    MMR *currMMR;
public:
    GCard (string, int, GPU*, MRER*, MMR*);
    //Устарело~
    //        GCard (string, int, int, int, int,
    //               BaseList <GPU>*, BaseList <MRER>*,
    //               BaseList <MMR>*);
    GCard ();
    ~GCard();
    void defPointers();
    //SET Функции
    void setGCInfo (string, int, GPU*, MRER*, MMR*);
    void setImg (string);
    void setPtrMRER (MRER*);
    void setPtrGPU (GPU*);
    void setPtrMMR (MMR*);
    //GET Функции
    string getInfo();
    string *getImg();
    int *getCodeMRER();
    int *getCodeGPU();
    int *getCodeMMR();
//    //FIND Функции
//    void findGPU(BaseList <GPU>*);
//    void findMRER(BaseList <MRER>*);
//    void findMMR(BaseList <MMR>*);
    //XML
    string writeInfoToXML ();
    //SQL
    string printSQLquery ();
};


/////////////////////
//Класс GCARDLIST
/////////////////////
class GCardList : public BaseList <GCard> {
private:
public:
    GCardList ();
    ~GCardList ();
    //Устарело~
    //Обновление связей
    //        void refresh (BaseList <GPU>*, BaseList <MRER>*, BaseList <MMR>*);
    GCard *gpuBindCheck (int);
    GCard *mrerBindCheck (int);
    GCard *mmrBindCheck (int);

};



/////////////////////
//Класс CATALOG
/////////////////////
class Catalog {
private:
    BaseList <GPU> *listGPU = new BaseList <GPU>;
    BaseList <MRER> *listMRER = new BaseList <MRER>;
    BaseList <MMR> *listMMR = new BaseList <MMR>;
    GCardList *listGC = new GCardList;
public:
    Catalog ();
    ~Catalog ();
    //Проверки
    bool checkListGPU ();
    bool checkListMRER ();
    bool checkListMMR ();
    bool checkListGC ();
    bool getSettingStat ();
    //Добавление элемента
    void addNewGC (string = "Неизвестно", int = 0, int = 0, int = 0, int = 0);
    void addNewGPU (string = "Неизвестно", int = 0, int = 0, int = 0, string = "Неизвестно");
    void addNewMRER (string = "Неизвестно", int = 0, unsigned short = 0, string = "Неизвестно");
    void addNewMMR (int = 0, unsigned short = 0, string = "Неизвестно", double = 0.0, int = 0);
    //SET LIST
    void setListGPU (BaseList <GPU>*);
    void setListMRER (BaseList <MRER>*);
    void setListMMR (BaseList <MMR>*);
    void setListGC (GCardList*);
    //Установка поля CODE для ANY
    void setGCcode (int, int);
    void setGPUcode (int, int);
    void setMRERcode (int, int);
    void setMMRcode (int, int);
    //Установка поля CODEANY для GC
    void setGCcodeGPU (int, int);
    void setGCcodeMRER (int, int);
    void setGCcodeMMR (int, int);
    //Очистка
    void listGPclear ();
    void listGCclear ();
    void listMRERclear ();
    void listMMRclear ();
    void clearAll ();
    //Удаление
    void deleteGC (int);
    void deleteGP (int);
    void deleteMRER (int);
    void deleteMMR (int);
    //Запрос размера
    int getListGCsize ();
    int getListGPsize ();
    int getListMRERsize ();
    int getListMMRsize ();
    //Запрос списков
    BaseList <GPU> *getListGPU ();
    BaseList <MRER> *getListMRER();
    BaseList <MMR> *getListMMR ();
    GCardList *getListGC();
    //Запрос информации
    string getGPUInfo(int);
    string getGCInfo(int);
    string getMRERInfo(int);
    string getMMRInfo(int);
    //Вывод
    string printListGC ();
    string printListGP ();
    string printListMRER ();
    string printListMMR ();
    string printAllInfo ();
    //XML
    bool readFromXML(string);
    void writeToXML(string);
    //SQL
    bool readFromSQL(string);
    template < typename T >
    void execLibSQLquery (T inputLib, sqlite3 *);
    bool writeToSQL(string);
};

#endif
