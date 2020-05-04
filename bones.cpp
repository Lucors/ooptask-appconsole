#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "bones.h"
#include "funcs.h"
#include <fstream>
using namespace std;


//Class implementation

/////////////////////
//BASE CLASS
/////////////////////
    Base::Base (){
        code = new int(-1);
        name = new string("Неизвестно");
    }
    Base::~Base(){
    }
    void Base::setName (string name){
        *this->name = name;
    }
    void Base::setCode (int code){
        *this->code = code;
    }
    string *Base::getName (){ return name; }
    int *Base::getCode (){ return code; }


/////////////////////
//GPU CLASS
/////////////////////
    GPU::GPU(string name, int code, int nup, int freq,
             string mrer){
        defPointers();
        setGPUInfo(name, code, nup, freq, mrer);
    }
    GPU::GPU (){
        defPointers();
    }
    void GPU::defPointers(){
        this->name = new string("Неизвестно");
        this->code = new int(-1);
        this->nup = new int(-1);
        this->freq = new int(-1);
        this->mrer = new string("Неизвестно");
        this->image = new string("blank.png");
    }
    GPU::~GPU(){
        delete this->name;
        delete this->code;
        delete this->nup;
        delete this->freq;
        delete this->mrer;
        delete this->image;
    }
    void GPU::setGPUInfo (string name, int code, int nup, int freq,
                    string mrer){
        *this->name = name;
        *this->code = code;
        *this->nup = nup;
        *this->freq = freq;
        *this->mrer = mrer;
        setImage();
    }
    void GPU::setNUP (int nup){
        *this->nup = nup;
    }
    void GPU::setFreq (int freq){
        *this->freq = freq;
    }
    void GPU::setImage (){
        string tmpMRER = *mrer;
        wordToLow(tmpMRER);
        *this->image = "gpuBLANK.png";
        if (tmpMRER == "amd"){
            *this->image = "gpuAMD.png";
        }
        else {
            if (tmpMRER == "nvidia"){
                *this->image = "gpuNVIDIA.png";
            }
            else {
                if (tmpMRER == "intel"){
                    *this->image = "gpuINTEL.png";
                }
            }
        }
    }
    void GPU::setMRER (string mrer){
        *this->mrer = mrer;
        setImage();
    }
    string GPU::getInfo (){
        string tmpStr;
        tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
        tmpStr += fillTab(6,' ') + "Название процессора: " + *name + "\n";
        tmpStr += fillTab(6,' ') + "Код Процессора: " + to_string(*code) + "\n";
        tmpStr += fillTab(6,' ') + "Кол-во универсальных пр-сов: " + to_string(*nup) + "\n";
        tmpStr += fillTab(6,' ') + "Штатная частота (МГц): " + to_string(*freq) + "\n";
        tmpStr += fillTab(6,' ') + "Производитель: " + *mrer + "\n";
        tmpStr += fillTab(6,' ') + "Путь к картинке: " + *image + "\n";
        tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
        return tmpStr;
    }
    int *GPU::getNUP (){ return nup; }
    int *GPU::getFreq (){ return freq; }
    string *GPU::getMRER (){ return mrer; }
    string *GPU::getImagePath (){ return image; }
    string GPU::writeInfoToXML (){
        string strOut;

        strOut += "\t\t";
        strOut += "<gpu name=\"";
        strOut += *this->name + "\" code='";
        strOut += to_string(*this->code) + "'>\n";

        strOut += "\t\t\t";
        strOut += "<gpuInfo ";
        strOut += "nup=\"" + to_string(*this->nup) + "\" ";
        strOut += "freq=\"" + to_string(*this->freq) + "\" ";
        strOut += "manf=\"" + *this->mrer + "\"/>\n";

        strOut += "\t\t";
        strOut += "</gpu>\n";

        return strOut;
    }
    string GPU::printSQLquery (){
        string strOut = "INSERT INTO gpus (name, code, nup, freq, manf) VALUES (";
        strOut += "'" + *this->name + "', " + to_string(*this->code) + ", " + to_string(*this->nup);
        strOut += ", " + to_string(*this->freq) + ", '" + *this->mrer + "');";

        return strOut;
    }



/////////////////////
//MRER CLASS
/////////////////////
    MRER::MRER(string name, int code, unsigned short year, string site){
        defPointers();
        setMRERInfo(name, code, year, site);
    }
    MRER::MRER (){
        defPointers();
    }
    void MRER::defPointers(){
        this->name = new string("Неизвестно");
        this->site = new string("blank.com");
        this->code = new int(-1);
        this->fYear = new unsigned short(0);
        this->image = new string("blank.png");
    }
    MRER::~MRER (){
        delete this->name;
        delete this->code;
        delete this->fYear;
        delete this->site;
        delete this->image;
    }
    void MRER::setMRERInfo (string name, int code, unsigned short year, string site){
        *this->name = name;
        *this->code = code;
        *this->fYear = year;
        *this->site = site;
        setImage();
    }
    void MRER::setFYear (unsigned short year){
        *this->fYear = year;
    }
    void MRER::setImage (){
        string tmpMR = *name;
        wordToLow(tmpMR);
        *this->image = tmpMR + ".png";
    }
    void MRER::setName(string name){
        *this->name = name;
        setImage();
    }
    void MRER::setSite (string site){
        *this->site = site;
    }
    string MRER::getInfo (){
        string tmpStr;
        tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
        tmpStr += fillTab(6,' ') + "Название компании: " + *name + "\n";
        tmpStr += fillTab(6,' ') + "Код Производителя: " + to_string(*code) + "\n";
        tmpStr += fillTab(6,' ') + "Год основания: " + to_string(*fYear) + "\n";
        tmpStr += fillTab(6,' ') + "Сайт: " + *site + "\n";
        tmpStr += fillTab(6,' ') + "Путь к картинке: " + *image + "\n";
        tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
        return tmpStr;
    }
    unsigned short *MRER::getFYear (){ return fYear; }
    string *MRER::getSite (){ return site; }
    string *MRER::getImagePath (){ return image; }
    string MRER::writeInfoToXML(){
        string strOut;

        strOut += "\t\t";
        strOut += "<manf name=\"";
        strOut += *this->name + "\" code=\"";
        strOut += to_string(*this->code) + "\">\n";

        strOut += "\t\t\t";
        strOut += "<manfInfo ";
        strOut += "year=\"" + to_string(*this->fYear) + "\" ";
        strOut += "site=\"" + *this->site + "\"/>\n";

        strOut += "\t\t";
        strOut += "</manf>\n";
        return strOut;
    }
    string MRER::printSQLquery (){
        string strOut = "INSERT INTO mrers (name, code, year, site) VALUES (";
        strOut += "'" + *this->name + "', " + to_string(*this->code) + ", " + to_string(*this->fYear);
        strOut += ", '" + *this->site + "');";

        return strOut;
    }


/////////////////////
//MMR CLASS
/////////////////////
        MMR::MMR(int code, unsigned short memory, string type, double bandwidth){
            defPointers();
            setMMRInfo(code, memory, type, bandwidth);
        }
        MMR::MMR (){
            defPointers();
        }
        MMR::~MMR (){
            delete this->name;
            delete this->code;
            delete this->memory;
            delete this->bandwidth;
            delete this->freq;
            delete this->type;
        }
        void MMR::defPointers(){
            this->name = new string("Неизвестно");
            this->type = new string("Неизвестно");
            this->code = new int(-1);
            this->memory = new unsigned short(0);
            this->bandwidth = new double (0.0);
        }
        void MMR::setMMRInfo (int code, unsigned short memory, string type,
                               double bandwidth){
            *this->name = "Неизвестно";
            *this->code = code;
            *this->memory = memory;
            *this->bandwidth = bandwidth;
            *this->type = type;
        }
        void MMR::setMemory (unsigned short memory){
            *this->memory = memory;
        }
        void MMR::setBandW (double bandwidth){
            *this->bandwidth = bandwidth;
        }
        void MMR::setType (string type){
            *this->type = type;
        }
        string MMR::getInfo (){
            string tmpStr;
            tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
            tmpStr += fillTab(6,' ') + "Код Памяти: " + to_string(*code) + "\n";
            tmpStr += fillTab(6,' ') + "Тип памяти: " + *type + "\n";
            tmpStr += fillTab(6,' ') + "Объем (Гб): " + to_string(*memory) + "\n";
            tmpStr += fillTab(6,' ') + "Пропуск. способность (Гбайт/сек): " + to_string(*bandwidth) + "\n";
            tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
            return tmpStr;
        }
        unsigned short *MMR::getMemory (){ return memory; }
        double *MMR::getBandW () {return bandwidth; }
        string *MMR::getType (){ return type; }
        string MMR::writeInfoToXML(){
            string strOut;

            strOut += "\t\t";
            strOut += "<mem code=\"";
            strOut += to_string(*this->code) + "\">\n";

            strOut += "\t\t\t";
            strOut += "<memInfo ";
            strOut += "memory=\"" + to_string(*this->memory) + "\" ";
            strOut += "type=\"" + *this->type + "\" ";
            strOut += "bandwidth=\"" + to_string(*this->bandwidth) + "\"/>\n";

            strOut += "\t\t";
            strOut += "</mem>\n";
            return strOut;
        }
        string MMR::printSQLquery (){
            string strOut = "INSERT INTO mmrs (code, memory, type, band) VALUES (";
            strOut += to_string(*this->code) + ", " + to_string(*this->memory);
            strOut += ", '" + *this->type + "', " + to_string(*this->bandwidth) + ");";

            return strOut;
        }


/////////////////////
//GCARD CLASS
/////////////////////
//obsolete *_*
//    GCard::GCard (string name, int code, int codeMre,
//                  int codeGP, int codeMry, BaseList <GPU> *GPUList,
//                  BaseList <MRER> *MRERList, BaseList <MMR> *MMRList){
//        setGCInfo(name, code, codeMre, codeGP, codeMry,
//                  GPUList, MRERList, MMRList);
//	}
    GCard::GCard (string name, int code, GPU *gpu, MRER *mrer, MMR *mmr){
        defPointers();
        setGCInfo(name, code, gpu, mrer, mmr);
    }
    GCard::GCard (){
        defPointers();
    }
    void GCard::defPointers(){
        this->name = new string("Неизвестно");
        this->correctName = new string("Неизвестно");
        this->code = new int(-1);
    }
    void GCard::setGCInfo (string name, int code, GPU* tmpGPU, MRER* tmpMRER, MMR* tmpMMR){
        *this->code = code;
        this->currGPU = tmpGPU;
        this->currMRER = tmpMRER;
        this->currMMR = tmpMMR;

        *this->correctName = name;
        *this->name = *currMRER->getName() + ' ' + *currGPU->getName() + ' ' + name;
        this->codeGP = currGPU->getCode();
        this->codeMre = currMRER->getCode();
        this->codeMry = currMMR->getCode();
    }
    GCard::~GCard (){
        delete this->name;
        delete this->code;
        delete this->correctName;
        delete this->image;
    }
    void GCard::findGPU (BaseList <GPU> *GPUList){
        try {
            GPU *tmp = GPUList->findByCode(*codeGP);
            currGPU = tmp;
        }
        catch (string errLog){
            cout << endl;
            cout << "<> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Привязка не произведена" << endl;
            cout << endl;
        }
    }
    void GCard::findMRER (BaseList <MRER> *MRERList){
        try {
            MRER *tmp = MRERList->findByCode(*codeMre);
            currMRER = tmp;
        }
        catch (string errLog){
            cout << endl;
            cout << "<> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Привязка не произведена" << endl;
            cout << endl;
        }
    }
    void GCard::findMMR (BaseList <MMR> *MMRList){
        try {
            MMR *tmp = MMRList->findByCode(*codeMry);
            currMMR = tmp;
        }
        catch (string errLog){
            cout << endl;
            cout << "<> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Привязка не произведена" << endl;
            cout << endl;
        }
    }
    void GCard::setImg (std::string demoImg){
        *this->image = demoImg;
    }
    void GCard::setCodeManre (int codeMre, BaseList <MRER> *MRERList){
        *this->codeMre = codeMre;
        findMRER(MRERList);
    }
    void GCard::setCodeGPU (int codeGP, BaseList <GPU> *GPUList){
        *this->codeGP = codeGP;
        findGPU(GPUList);
    }
    void GCard::setCodeMemory (int codeMry, BaseList <MMR> *MMRList){
        *this->codeMry = codeMry;
        findMMR(MMRList);
    }
    string GCard::getInfo (){
        string tmpStr;
        tmpStr += fillTab(44,'-') + "\n";
        tmpStr += "Название: " + *name + "\n";
        tmpStr += "Код Видеокарты: " + to_string(*code) + "\n";
        tmpStr += "Код Производ.: " + to_string(*codeMre) + "\n";
            tmpStr += currMRER->getInfo();
        tmpStr += "Код Граф. процессора: " + to_string(*codeGP) + "\n";
            tmpStr += currGPU->getInfo();
        tmpStr += "Код Памяти: " + to_string(*codeMry) + "\n";
            tmpStr += currMMR->getInfo();
        tmpStr += fillTab(44,'-') + "\n\n";
        return tmpStr;
    }
    string *GCard::getImg (){ return image; }
    int *GCard::getCodeManre (){ return codeMre; }
    int *GCard::getCodeGPU (){ return codeGP; }
    int *GCard::getCodeMemory (){ return codeMry; }
    string GCard::writeInfoToXML(){
        string strOut;

        strOut += "\t\t";
        strOut += "<gc name=\"";
        strOut += *this->correctName + "\" code=\"";
        strOut += to_string(*this->code) + "\">\n";

        strOut += "\t\t\t";
        strOut += "<gcInfo ";
        strOut += "manCode=\"" + to_string(*this->codeMre) + "\" ";
        strOut += "gpuCode=\"" + to_string(*this->codeGP) + "\" ";
        strOut += "memCode=\"" + to_string(*this->codeMry) + "\"/>\n";

        strOut += "\t\t";
        strOut += "</gc>\n";
        return strOut;
    }
    string GCard::printSQLquery (){
        string strOut = "INSERT INTO gcs (name, code, manCode, gpuCode, memCode) VALUES (";
        strOut += "'" + *this->correctName + "', " + to_string(*this->code) + ", " + to_string(*this->codeMre);
        strOut += ", " + to_string(*this->codeGP) + ", " + to_string(*this->codeMry) + ");";

        return strOut;
    }


/////////////////////
//GCARDLIST CLASS
/////////////////////
    GCardList::GCardList (){
    }
    GCardList::~GCardList(){
    }
//obsolete *_*
//    void GCardList::refresh (BaseList <GPU> *GPUList, BaseList <MRER> *MRERList, BaseList <MMR> *MMRList){
//        for (int i = 0; i < static_cast<int>(list.size()); i++){
//            try {
//                list[i].findGPU(GPUList);
//                list[i].findMRER(MRERList);
//                list[i].findMMR(MMRList);
//            }
//            catch (string errLog){
//                cout << endl;
//                cout << errLog << endl;
//                cout << " <> Ошибка обновления привязок видеокарты с кодом " << list[i].getCode() << endl;
//                cout << endl;
//            }
//        }
//    }
    GCard *GCardList::gpuBindCheck (int code){
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            if (*list[i]->getCodeGPU() == code){
                return list[i];
            }
        }
        return nullptr;
    }
    GCard *GCardList::mrerBindCheck (int code){
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            if (*list[i]->getCodeManre() == code){
                return list[i];
            }
        }
        return nullptr;
    }
    GCard *GCardList::mmrBindCheck (int code){
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            if (*list[i]->getCodeMemory() == code){
                return list[i];
            }
        }
        return nullptr;
    }



/////////////////////
//CATALOG CLASS
////////////////////
    Catalog::Catalog (){
        listGC->setObjName("Видеокарта");
        listGP->setObjName("Граф. процессор");
        listMRER->setObjName("Производитель");
        listMMR->setObjName("Память");
    }
    Catalog::~Catalog(){
    }
//CHECK
    bool Catalog::checkListGPU (){
        if (listGP == nullptr){
            return false;
        }
        else { return true;}
    }
    bool Catalog::checkListMRER (){
        if (listMRER == nullptr){
            return false;
        }
        else { return true;}
    }
    bool Catalog::checkListMMR (){
        if (listMMR == nullptr){
            return false;
        }
        else { return true;}
    }
    bool Catalog::checkListGC (){
        if (listGC == nullptr){
            return false;
        }
        else { return true;}
    }
    bool Catalog::getSettingStat (){
        if (!checkListGPU() && !checkListMMR() && !checkListMRER() && !checkListGC()){
            return false;
        }
        else {
            return true;
        }
    }
//ADD NEW ELEMENT
    void Catalog::addNewGC (string name = "Неизвестно", int code = -1, int codeMre = -1,
                             int codeGP = -1, int codeMry = -1){
        try {
            this->listGC->findByCode(code);
            cout << " <> Ошибка! Видеокарта с кодом " << code << " есть в списке" << endl;
            cout << endl;
        }
        catch (...){
            try {
                GPU *tmpGPU = listGP->findByCode(codeGP);
                MRER *tmpMRER = listMRER->findByCode(codeMre);
                MMR *tmpMMR = listMMR->findByCode(codeMry);
    //            cout << tmpGPU->getInfo() << endl;
    //            cout << tmpMRER->getInfo() << endl;
    //            cout << tmpMMR->getInfo() << endl;
                GCard *tmpCard = new GCard(name, code, tmpGPU, tmpMRER, tmpMMR);
                this->listGC->addNew(tmpCard);
            }
            catch (string errLog){
                cout << " <> Ошибка! ";
                cout << errLog << endl;
                cout << " <> Видеокарта с кодом " << code << " не добавлена" << endl;
                cout << endl;
            }
        }

    }
    void Catalog::addNewGPU (string name = "Неизвестно", int code = -1, int nup = -1, int freq = -1,
                             string manRer = "Неизвестно"){
        try {
            this->listGP->findByCode(code);
            cout << " <> Ошибка! Граф. процессор с кодом " << code << " есть в списке" << endl;
            cout << endl;
        }
        catch (...){
            GPU *tmpGPU = new GPU(name, code, nup, freq, manRer);
            this->listGP->addNew(tmpGPU);
        }
    }
    void Catalog::addNewMRER (string name = "Неизвестно", int code = -1, unsigned short year = 0,
                            string site = "Неизвестно"){
        try {
            this->listMRER->findByCode(code);
            cout << " <> Ошибка! Производитель с кодом " << code << " есть в списке" << endl;
            cout << endl;
        }
        catch (...){
            MRER *tmpMRER = new MRER(name, code, year, site);
            this->listMRER->addNew(tmpMRER);
        }
    }
    void Catalog::addNewMMR (int code = -1, unsigned short memory = 0, string type = "Неизвестно",
                             double bandwidth = 0.0){
        try {
            this->listMMR->findByCode(code);
            cout << " <> Ошибка! Память с кодом " << code << " есть в списке" << endl;
            cout << endl;
        }
        catch (...){
            MMR *tmpMMR = new MMR(code, memory, type, bandwidth);
            this->listMMR->addNew(tmpMMR);
        }
    }
//SET LIST
    void Catalog::setListGPU (BaseList<GPU> *tmpList){
        this->listGP = tmpList;
    }
    void Catalog::setListMRER (BaseList<MRER> *tmpList){
        this->listMRER = tmpList;
    }
    void Catalog::setListMMR (BaseList<MMR> *tmpList){
        this->listMMR = tmpList;
    }
    void Catalog::setListGC (GCardList *tmpList){
        this->listGC = tmpList;
    }
//SET CODE FOR ANY
    void Catalog::setGCcode(int oldCode, int newCode){
        try {
            this->listGC->findByCode(newCode);
            cout << " <> Ошибка! Видеокарта с кодом " << newCode << " уже есть в списке" << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
        }
        catch (...){
            try {
                this->listGC->findByCode(oldCode)->setCode(newCode);
                cout << " <> Успешно сменен код " << oldCode << " у видеокарты на код " << newCode << endl;
            }
            catch (string errLog){
                cout << " <> Ошибка! ";
                cout << errLog << endl;
                cout << " <> Смена кода не произведена" << endl;
                cout << endl;
            }
        }
    }
    void Catalog::setGPcode(int oldCode, int newCode){
        try {
            this->listGP->findByCode(newCode);
            cout << " <> Ошибка! Граф. процессор с кодом " << newCode << " уже есть в списке" << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
        }
        catch (...){
            try {
                this->listGP->findByCode(oldCode)->setCode(newCode);
                cout << " <> Успешно сменен код " << oldCode << " у граф. процессора на код " << newCode << endl;
            }
            catch (string errLog){
                cout << " <> Ошибка! ";
                cout << errLog << endl;
                cout << " <> Смена кода не произведена" << endl;
                cout << endl;
            }
        }
    }
    void Catalog::setMRERcode(int oldCode, int newCode){
        try {
            this->listMRER->findByCode(newCode);
            cout << " <> Ошибка! Производитель с кодом " << newCode << " уже есть в списке" << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
        }
        catch (...){
            try {
                this->listMRER->findByCode(oldCode)->setCode(newCode);
                cout << " <> Успешно сменен код " << oldCode << " у производителя на код " << newCode << endl;
            }
            catch (string errLog){
                cout << " <> Ошибка! ";
                cout << errLog << endl;
                cout << " <> Смена кода не произведена" << endl;
                cout << endl;
            }
        }
    }
    void Catalog::setMMRcode(int oldCode, int newCode){
        try {
            this->listMMR->findByCode(newCode);
            cout << " <> Ошибка! Память с кодом " << newCode << " уже есть в списке" << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
        }
        catch (...){
            try {
                this->listMMR->findByCode(oldCode)->setCode(newCode);
                cout << " <> Успешно сменен код " << oldCode << " у памяти на код " << newCode << endl;
                cout << endl;
            }
            catch (string errLog){
                cout << " <> Ошибка! ";
                cout << errLog << endl;
                cout << " <> Смена кода не произведена" << endl;
                cout << endl;
            }
        }
    }
//SET CODE FOR GC
    void Catalog::setGCcodeGPU (int i, int gpCode){
        try {
            this->listGC->findByCode(i)->setCodeGPU(gpCode, this->listGP);
        }
        catch (string errLog){
            cout << errLog << endl;
            cout << " <> Привязка не произведена" << endl;
            cout << endl;
        }
    }
    void Catalog::setGCcodeMRER (int i, int mrerCode){
        try {
            this->listGC->findByCode(i)->setCodeManre(mrerCode, this->listMRER);
        }
        catch (string errLog){
            cout << errLog << endl;
            cout << " <> Привязка не произведена" << endl;
            cout << endl;
        }
    }
    void Catalog::setGCcodeMMR (int i, int mmrCode){
        try {
            this->listGC->findByCode(i)->setCodeMemory(mmrCode, this->listMMR);
        }
        catch (string errLog){
            cout << errLog << endl;
            cout << " <> Привязка не произведена" << endl;
            cout << endl;
        }
    }
//CLEAR
    void Catalog::listGPclear (){
        this->listGP->clearAll();
    }
    void Catalog::listGCclear (){
        this->listGC->clearAll();
    }
    void Catalog::listMRERclear (){
        this->listMRER->clearAll();
    }
    void Catalog::listMMRclear (){
        this->listMMR->clearAll();
    }
    void Catalog::clearAll (){
        listGCclear();
        listGPclear();
        listMMRclear();
        listMRERclear();
    }
//DELETE ELEMENT
    void Catalog::deleteGC (int code){
        try {
            listGC->deleteElement(code);
            cout << " <> Успешно удалена видеокарта с кодом " << code << endl;
            cout << endl;
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Удаление не произведено" << endl;
            cout << endl;
        }
    }
    void Catalog::deleteGP (int code){
        try {
            GCard *tmpGC = listGC->gpuBindCheck(code);
            if (tmpGC != nullptr){
                throw "Граф. процессор с кодом " + to_string(code) +
                        " привязан к видеокарте с кодом " + std::to_string(*tmpGC->getCode());
            }
            listGP->deleteElement(code);
            cout << " <> Успешно удален граф. процессор с кодом " << code << endl;
            cout << endl;
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Удаление не произведено" << endl;
            cout << endl;
        }
    }
    void Catalog::deleteMRER (int code){
        try {
            GCard *tmpGC = listGC->mrerBindCheck(code);
            if (tmpGC != nullptr){
                throw "Производитель с кодом " + std::to_string(code) +
                        " привязан к видеокарте с кодом " + std::to_string(*tmpGC->getCode());
            }
            listMRER->deleteElement(code);
            cout << " <> Успешно удален производитель с кодом " << code << endl;
            cout << endl;
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Удаление не произведено" << endl;
            cout << endl;
        }
    }
    void Catalog::deleteMMR (int code){
        try {
            GCard *tmpGC = listGC->mmrBindCheck(code);
            if (tmpGC != nullptr){
                throw "Память с кодом " + std::to_string(code) +
                        " привязан к видеокарте с кодом " + std::to_string(*tmpGC->getCode());
            }
            listMMR->deleteElement(code);
            cout << " <> Успешно удалена память с кодом " << code << endl;
            cout << endl;
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Удаление не произведено" << endl;
            cout << endl;
        }
    }
//GET SIZE
    int Catalog::getListGCsize (){
        return this->listGC->getSize();
    }
    int Catalog::getListGPsize (){
        return this->listGP->getSize();
    }
    int Catalog::getListMRERsize (){
        return this->listMRER->getSize();
    }
    int Catalog::getListMMRsize (){
        return this->listMMR->getSize();
    }
//GET LIST
    BaseList <GPU>* Catalog::getListGPU (){
        return listGP;
    }
    BaseList <MRER>* Catalog::getListMRER (){
        return listMRER;
    }
    BaseList <MMR>* Catalog::getListMMR (){
        return listMMR;
    }
    GCardList* Catalog::getListGC (){
        return listGC;
    }
//GET INFO
    string Catalog::getGPUInfo (int code){
        return this->listGP->findByCode(code)->getInfo();
    }
    string Catalog::getGCInfo (int code){
        return this->listGC->findByCode(code)->getInfo();
    }
    string Catalog::getMRERInfo (int code){
        return this->listMRER->findByCode(code)->getInfo();
    }
    string Catalog::getMMRInfo (int code){
        return this->listMMR->findByCode(code)->getInfo();
    }
//PRINT
    string Catalog::printListGC (){
        return this->listGC->printAll();
    }
    string Catalog::printListGP (){
        return this->listGP->printAll();
    }
    string Catalog::printListMRER (){
        return this->listMRER->printAll();
    }
    string Catalog::printListMMR (){
        return this->listMMR->printAll();
    }
    string Catalog::printAllInfo (){
        string tmpOut;
        tmpOut += "\n";
        tmpOut += "GPU[" + to_string(this->getListGPsize()) + "]:\n";
        tmpOut += printListGP();
        tmpOut += "Manufacturer[" + to_string(this->getListMRERsize()) + "]:\n";
        tmpOut += printListMRER();
        tmpOut += "Memory[" + to_string(this->getListMMRsize()) + "]:\n";
        tmpOut += printListMMR();
        tmpOut += "VIDEOCARDS[" + to_string(this->getListGCsize()) + "]:\n";
        tmpOut += printListGC();
        return tmpOut;
    }
