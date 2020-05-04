#ifndef BONES_H
#define BONES_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

/////////////////////
//Родительский класс
/////////////////////
class Base {
protected:
    int *code;
    std::string *name;
public:
    Base();
    virtual ~Base() = 0;
    void setName(std::string);
    void setCode(int);
    std::string *getName();
    int *getCode();
    virtual std::string writeInfoToXML() = 0;
    virtual std::string printSQLquery() = 0;
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
    std::string *mrer;
    //Далее служебные поля:
    std::string *image;
public:
    GPU(std::string, int, int, int, std::string);
    GPU();
    ~GPU();
    void defPointers();
    void setGPUInfo(std::string, int, int, int, std::string);
    void setNUP(int);
    void setFreq(int);
    void setMRER(std::string);
    void setImage();
    std::string getInfo();
    int *getNUP();
    int *getFreq();
    std::string *getMRER();
    std::string *getImagePath();
    //XML
    std::string writeInfoToXML();
    //SQL
    std::string printSQLquery();
};

/////////////////////
//Класс MRER
/////////////////////
class MRER : public Base {
private:
    //Год основания (Надеемся, что smallint sql равносилен short)
    unsigned short *fYear;
    //Строка сайт производителя
    std::string *site;
    //Далее служебные поля:
    std::string *image;
public:
    MRER(std::string, int, unsigned short, std::string);
    MRER();
    ~MRER();
    void defPointers();
    void setMRERInfo(std::string, int, unsigned short, std::string);
    void setFYear(unsigned short);
    void setName(std::string);
    void setSite(std::string);
    void setImage();
    std::string getInfo();
    unsigned short *getFYear();
    std::string *getSite();
    std::string *getImagePath();
    //XML
    std::string writeInfoToXML();
    //SQL
    std::string printSQLquery();
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
    std::string *type;
public:
    MMR(int, unsigned short, std::string, double, int);
    MMR();
    ~MMR();
    void defPointers();
    void setMMRInfo(int, unsigned short, std::string, double, int);
    void setMemory(unsigned short);
    void setBandW(double);
    void setType(std::string);
    void setFreq(int);
    std::string getInfo();
    unsigned short *getMemory();
    double *getBandW();
    std::string *getType();
    int *getFreq();
    //XML
    std::string writeInfoToXML();
    //SQL
    std::string printSQLquery();
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
    std::vector < T* > list;
    std::string objName = "Объект";
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
        throw this->objName + " с кодом " + std::to_string(code) + " отсутствует в списке";
    }
    void addNew (T *tmpCard){
        list.push_back(tmpCard);
    }
    void clearAll () {
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
        throw this->objName + " с кодом " + std::to_string(code) + " отсутствует в списке";
    }
    std::string printAll(){
        std::string tmpStr;
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            tmpStr += list[i]->getInfo();
            tmpStr += "\n";
        }
        return tmpStr;
    }
    std::string printXML(){
        std::string strOut;
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            strOut += list[i]->writeInfoToXML();
        }
        return strOut;
    }
    std::string createSQLquery(int position){
        std::string strOut = "";
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
    void setObjName (std::string objName){
        this->objName = objName;
    }
    std::string getObjName (){
        return this->objName;
    }
};

/////////////////////
//Класс GCARD
/////////////////////
class GCard : public Base {
private:
    std::string *correctName;
    std::string *image;
    int *codeMRER, *codeGPU, *codeMMR;
    GPU *currGPU;
    MRER *currMRER;
    MMR *currMMR;
public:
    GCard (std::string, int, GPU*, MRER*, MMR*);
    //Устарело~
    //        GCard (std::string, int, int, int, int,
    //               BaseList <GPU>*, BaseList <MRER>*,
    //               BaseList <MMR>*);
    GCard ();
    ~GCard();
    void defPointers();
    //SET Функции
    void setGCInfo (std::string, int, GPU*, MRER*, MMR*);
    void setImg (std::string);
    void setPtrMRER (MRER*);
    void setPtrGPU (GPU*);
    void setPtrMMR (MMR*);
    //GET Функции
    std::string getInfo();
    std::string *getImg();
    int *getCodeMRER();
    int *getCodeGPU();
    int *getCodeMMR();
//    //FIND Функции
//    void findGPU(BaseList <GPU>*);
//    void findMRER(BaseList <MRER>*);
//    void findMMR(BaseList <MMR>*);
    //XML
    std::string writeInfoToXML ();
    //SQL
    std::string printSQLquery ();
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
    void addNewGC (std::string = "Неизвестно", int = 0, int = 0, int = 0, int = 0);
    void addNewGPU (std::string = "Неизвестно", int = 0, int = 0, int = 0, std::string = "Неизвестно");
    void addNewMRER (std::string = "Неизвестно", int = 0, unsigned short = 0, std::string = "Неизвестно");
    void addNewMMR (int = 0, unsigned short = 0, std::string = "Неизвестно", double = 0.0, int = 0);
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
    std::string getGPUInfo(int);
    std::string getGCInfo(int);
    std::string getMRERInfo(int);
    std::string getMMRInfo(int);
    //Вывод
    std::string printListGC ();
    std::string printListGP ();
    std::string printListMRER ();
    std::string printListMMR ();
    std::string printAllInfo ();
    ////LIB SQL CALLBACK FUNC(S)
    //        int callbackGPU(void*, int, char**, char**);
    //        int callbackMRER(void*, int, char**, char**);
    //        int callbackMMR(void*, int, char**, char**);
    //        int callbackGC(void*, int, char**, char**);
};

#endif
