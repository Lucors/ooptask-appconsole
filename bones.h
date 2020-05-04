#ifndef BONES_H
#define BONES_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
//Class interface

/////////////////////
//BASE CLASS
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
    MMR(int, unsigned short, std::string, double);
    MMR();
    ~MMR();
    void defPointers();
    void setMMRInfo(int, unsigned short, std::string, double);
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
////////////////////
template < typename T >
class BaseList {
protected:
    std::vector < T* > list;
    std::string objName = "Объект";
    T *operator [] (const int index){
        if(index <= list.size()){
            return list[index];
        }
        else {
            return nullptr;
        }
    }
public:
    BaseList (){
    }
    ~BaseList (){
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
//GCARD CLASS
/////////////////////
class GCard : public Base {
private:
    std::string *correctName;
    std::string *image;
    int *codeMre, *codeGP, *codeMry;
    GPU *currGPU;
    MRER *currMRER;
    MMR *currMMR;
public:
    GCard (std::string, int, GPU*, MRER*, MMR*);
    //obsolete *_*
    //        GCard (std::string, int, int, int, int,
    //               BaseList <GPU>*, BaseList <MRER>*,
    //               BaseList <MMR>*);
    GCard ();
    ~GCard();
    void defPointers();
    //SET Функции
    void setGCInfo (std::string, int, GPU*, MRER*, MMR*);
    void setImg (std::string);
    void setCodeManre (int, BaseList <MRER>*);
    void setCodeGPU (int, BaseList <GPU>*);
    void setCodeMemory (int, BaseList <MMR>*);
    //GET Функции
    std::string getInfo();
    std::string *getImg();
    int *getCodeManre();
    int *getCodeGPU();
    int *getCodeMemory();
    //FIND Функции
    void findGPU(BaseList <GPU>*);
    void findMRER(BaseList <MRER>*);
    void findMMR(BaseList <MMR>*);
    //XML
    std::string writeInfoToXML ();
    //SQL
    std::string printSQLquery ();
};


/////////////////////
//GCARDLIST CLASS
/////////////////////
class GCardList : public BaseList <GCard> {
private:
public:
    GCardList ();
    ~GCardList ();
    //obsolete *_*
    //Update bindings
    //        void refresh (BaseList <GPU>*, BaseList <MRER>*, BaseList <MMR>*);
    GCard *gpuBindCheck (int);
    GCard *mrerBindCheck (int);
    GCard *mmrBindCheck (int);

};



/////////////////////
//CATALOG CLASS
/////////////////////
class Catalog {
private:
    BaseList <GPU> *listGP = new BaseList <GPU>;
    BaseList <MRER> *listMRER = new BaseList <MRER>;
    BaseList <MMR> *listMMR = new BaseList <MMR>;
    GCardList *listGC = new GCardList;
    //        GCard tmpCard;
public:
    Catalog ();
    ~Catalog ();
    //CHECK
    bool checkListGPU ();
    bool checkListMRER ();
    bool checkListMMR ();
    bool checkListGC ();
    bool getSettingStat ();
    //ADD NEW ELEMENT
    void addNewGC (std::string, int, int, int, int);
    void addNewGPU (std::string, int, int, int, std::string);
    void addNewMRER (std::string, int, unsigned short, std::string);
    void addNewMMR (int, unsigned short, std::string, double);
    //SET LIST
    void setListGPU (BaseList <GPU>*);
    void setListMRER (BaseList <MRER>*);
    void setListMMR (BaseList <MMR>*);
    void setListGC (GCardList*);
    //SET CODE FOR ANY
    void setGCcode (int, int);
    void setGPcode (int, int);
    void setMRERcode (int, int);
    void setMMRcode (int, int);
    //SET CODE FOR GC
    void setGCcodeGPU (int, int);
    void setGCcodeMRER (int, int);
    void setGCcodeMMR (int, int);
    //CLEAR
    void listGPclear ();
    void listGCclear ();
    void listMRERclear ();
    void listMMRclear ();
    void clearAll ();
    //DELETE ELEMENT
    void deleteGC (int);
    void deleteGP (int);
    void deleteMRER (int);
    void deleteMMR (int);
    //GET SIZE
    int getListGCsize ();
    int getListGPsize ();
    int getListMRERsize ();
    int getListMMRsize ();
    //GET LIST
    BaseList <GPU> *getListGPU ();
    BaseList <MRER> *getListMRER();
    BaseList <MMR> *getListMMR ();
    GCardList *getListGC();
    //GET INFO
    std::string getGPUInfo(int);
    std::string getGCInfo(int);
    std::string getMRERInfo(int);
    std::string getMMRInfo(int);
    //PRINT
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
