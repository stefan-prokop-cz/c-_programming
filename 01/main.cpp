#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class Identification
{
public:
    Identification() {}
    string key;
    int company_id;
};

class Company
{
public:
    Company() : active(true), total_amount(0) {}
    Company(const string & name, const string & addr, const string & id) : name(name), addr(addr), id(id), active(true), total_amount(0) {}
    ~Company() {}

    void addInvoice(const unsigned int amount) {
        total_amount += amount;
    }

    void setActive(bool active) {
        Company::active = active;
    }

    const string &getName() const {
        return name;
    }

    int getTotal_amount() const {
        return total_amount;
    }

    const string &getAddr() const {
        return addr;
    }

    const string &getId() const {
        return id;
    }

private:
    string name, addr, id;
    bool active;
    int total_amount;
};

class CVATRegister
{
public:
    CVATRegister   ( void ) {
        company_count = 0;
        invoice_count = 0;
    }
    ~CVATRegister  ( void ) {}
    bool          NewCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID ) {
        if(company_count == 0) {
            createCompany(name, addr, taxID);

            return true;
        }

        if(!isCompanyExists(name, addr, taxID)) {
            createCompany(name, addr, taxID);
        } else {
            return false;
        }

        return true;
    }
    bool          CancelCompany  ( const string    & name,
                                   const string    & addr ) {
        int id = findByNameAddrAndRemove(name, addr);

        if(id == -1) {
            return false;
        }

        companies[id].setActive(false);

        return true;
    }
    bool          CancelCompany  ( const string    & taxID ) {
        int id = findByIdAndRemove(taxID);

        if(id == -1) {
            return false;
        }

        companies[id].setActive(false);

        return true;
    }
    bool          Invoice        ( const string    & taxID,
                                   unsigned int      amount ) {
        int id = findById(taxID);

        if(id == -1) {
            return false;
        }

        int a = amount;
        invoices.push_back(a);
        companies[id].addInvoice(amount);
        invoice_count++;

        return true;
    }
    bool          Invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount ) {
        int id = findByNameAddr(name, addr);

        if(id == -1) {
            return false;
        }

        int a = amount;
        invoices.push_back(a);
        companies[id].addInvoice(amount);
        invoice_count++;

        return true;
    }
    bool          Audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const {
        if(invoice_count == 0) {
            return false;
        }

        int index = findByNameAddr(name, addr);

        if(index == -1) {
            return false;
        }

        sumIncome = (unsigned int) companies[index].getTotal_amount();

        return true;
    }
    bool          Audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const {
        if(invoice_count == 0) {
            return false;
        }

        int index = findById(taxID);

        if(index == -1) {
            return false;
        }

        sumIncome = (unsigned int) companies[index].getTotal_amount();

        return true;
    }
    unsigned int  MedianInvoice  ( void ) const {
        int index = invoice_count / 2;

        if(invoice_count == 0) {
            return 0;
        }

        const_cast<CVATRegister*>(this)->sortArray();

        return (unsigned int) invoices[index];
    }
private:
    vector<Identification> db_name_address, db_id;
    vector<Company> companies;
    vector<int> invoices;
    int invoice_count;
    int company_count;

    void sortArray() {
        std::sort(invoices.begin(), invoices.end());
    }
    /**
     * Metoda vytvarejici vhodny klic slozeny ze jmena a adresy firmy
     *
     * @param name
     * @param addr
     * @return
     */
    string createNameAddressIndex(string name, string addr) const {
        string new_name = "", new_addr = "";

        transform(name.begin(), name.end(), name.begin(), ::tolower);
        transform(addr.begin(), addr.end(), addr.begin(), ::tolower);

        return name + "#|+___++#|" + addr;
    }
    /**
     * Metoda overujici existenci firmy
     *
     * @param name
     * @param addr
     * @param id
     * @return
     */
    bool isCompanyExists(const string & name, const string & addr, const string & id) {
        int id1 = findById(id);

        if(id1 != -1) {
            return true;
        }

        int id2 = findByNameAddr(name, addr);

        return id2 != -1;

    }
    /**
     * Operator pro sort
     * - inspirace: http://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects (19. 03. 2017, 10:55)
     */
    struct cmp {
        inline bool operator () (const Identification &id1, const Identification &id2) {
            return (id1.key < id2.key);
        }

        inline bool operator () (const Identification &id1, const string &id) {
            return (id1.key < id);
        }
    };
    /**
     * Metoda vytvarejici novou firmu
     *
     * @param name
     * @param addr
     * @param id
     */
    void createCompany(const string & name, const string & addr, const string & id) {
        Identification identification;
        identification.company_id = company_count;
        identification.key = createNameAddressIndex(name, addr);

        if(company_count == 0) {
            db_name_address.push_back(identification);
            identification.key = id;
            db_id.push_back(identification);
        } else {
            auto lower = lower_bound(db_name_address.begin(), db_name_address.end(), identification, cmp());
            db_name_address.insert(lower, identification);
            identification.key = id;

            auto lower1 = lower_bound(db_id.begin(), db_id.end(), identification, cmp());
            db_id.insert(lower1, identification);
        }

        Company company(name, addr, id);
        companies.push_back(company);
        company_count++;
    }
    /**
     * Metoda vyhledavajici firmu podle taxID
     *
     * @param id
     * @return
     */
    int findById(const string & id) const {
        auto lower = lower_bound(db_id.begin(), db_id.end(), id, cmp());
        int dist = (int) distance(db_id.begin(), lower);

        if(dist >= company_count || db_id[dist].key != id) {
            return -1;
        }

        return db_id[dist].company_id;
    }
    /**
     * Metoda vyhledavajici firmu podle adresy a jmena
     *
     * @param name
     * @param addr
     * @return
     */
    int findByNameAddr(const string & name, const string & addr) const {
        string id = createNameAddressIndex(name, addr);
        auto lower = lower_bound(db_name_address.begin(), db_name_address.end(), id, cmp());
        int dist = (int) distance(db_name_address.begin(), lower);

        if(dist >= company_count || db_name_address[dist].key != id) {
            return -1;
        }

        return db_name_address[dist].company_id;
    }
    int findByNameAddrAndRemove(const string & name, const string & addr) {
        string id = createNameAddressIndex(name, addr);
        auto lower = lower_bound(db_name_address.begin(), db_name_address.end(), id, cmp());
        int dist = (int) distance(db_name_address.begin(), lower);

        if(dist >= company_count || db_name_address[dist].key != id) {
            return -1;
        }

        db_name_address[dist].key = db_name_address[dist].key + "#|+___++#|" + to_string(db_name_address[dist].company_id);

        string id1 = companies[db_name_address[dist].company_id].getId();
        auto lower1 = lower_bound(db_id.begin(), db_id.end(), id1, cmp());
        int dist1 = (int) distance(db_id.begin(), lower1);

        if(dist1 >= company_count || db_id[dist1].key != id1) {
            return -1;
        }

        db_id[dist1].key = db_id[dist1].key + "#|+___++#|" + to_string(db_id[dist1].company_id);

        return db_name_address[dist].company_id;
    }
    int findByIdAndRemove(const string & id) {
        auto lower = lower_bound(db_id.begin(), db_id.end(), id, cmp());
        int dist = (int) distance(db_id.begin(), lower);

        if(dist >= company_count || db_id[dist].key != id) {
            return -1;
        }

        db_id[dist].key = db_id[dist].key + "#|+___++#|" + to_string(db_id[dist].company_id);

        string id1 = createNameAddressIndex(companies[db_id[dist].company_id].getName(), companies[db_id[dist].company_id].getAddr());
        auto lower1 = lower_bound(db_name_address.begin(), db_name_address.end(), id1, cmp());
        int dist1 = (int) distance(db_name_address.begin(), lower1);

        if(dist1 >= company_count || db_name_address[dist1].key != id1) {
            return -1;
        }

        db_name_address[dist1].key = db_name_address[dist1].key + "#|+___++#|" + to_string(db_name_address[dist1].company_id);

        return db_id[dist].company_id;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
    unsigned int sumIncome;

    CVATRegister b1;
    assert ( b1 . NewCompany ( "ACME", "Kolejni", "666/666/666" ) );
    assert ( b1 . NewCompany ( "ACME", "Thakurova", "666/666" ) );
    assert ( b1 . NewCompany ( "Dummy", "Thakurova", "123456" ) );
    assert ( b1 . Invoice ( "666/666", 2000 ) );
    assert ( b1 . MedianInvoice () == 2000 );
    assert ( b1 . Invoice ( "666/666/666", 3000 ) );
    assert ( b1 . MedianInvoice () == 3000 );
    assert ( b1 . Invoice ( "123456", 4000 ) );
    assert ( b1 . MedianInvoice () == 3000 );
    assert ( b1 . Invoice ( "aCmE", "Kolejni", 5000 ) );
    assert ( b1 . MedianInvoice () == 4000 );
    assert ( b1 . Audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
    assert ( b1 . Audit ( "123456", sumIncome ) && sumIncome == 4000 );
    assert ( b1 . CancelCompany ( "ACME", "KoLeJnI" ) );
    assert ( b1 . MedianInvoice () == 4000 );
    assert ( b1 . CancelCompany ( "666/666" ) );
    assert ( b1 . MedianInvoice () == 4000 );
    assert ( b1 . Invoice ( "123456", 100 ) );
    assert ( b1 . MedianInvoice () == 3000 );
    assert ( b1 . Invoice ( "123456", 300 ) );
    assert ( b1 . MedianInvoice () == 3000 );
    assert ( b1 . Invoice ( "123456", 200 ) );
    assert ( b1 . MedianInvoice () == 2000 );
    assert ( b1 . Invoice ( "123456", 230 ) );
    assert ( b1 . MedianInvoice () == 2000 );
    assert ( b1 . Invoice ( "123456", 830 ) );
    assert ( b1 . MedianInvoice () == 830 );
    assert ( b1 . Invoice ( "123456", 1830 ) );
    assert ( b1 . MedianInvoice () == 1830 );
    assert ( b1 . Invoice ( "123456", 2830 ) );
    assert ( b1 . MedianInvoice () == 1830 );
    assert ( b1 . Invoice ( "123456", 2830 ) );
    assert ( b1 . MedianInvoice () == 2000 );
    assert ( b1 . Invoice ( "123456", 3200 ) );
    assert ( b1 . MedianInvoice () == 2000 );

    CVATRegister b2;
    assert ( b2 . NewCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . NewCompany ( "Dummy", "Kolejni", "123456" ) );
    assert ( ! b2 . NewCompany ( "AcMe", "kOlEjNi", "1234" ) );
    assert ( b2 . NewCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
    assert ( b2 . MedianInvoice () == 0 );
    assert ( b2 . Invoice ( "ABCDEF", 1000 ) );
    assert ( b2 . MedianInvoice () == 1000 );
    assert ( b2 . Invoice ( "abcdef", 2000 ) );
    assert ( b2 . MedianInvoice () == 2000 );
    assert ( b2 . Invoice ( "aCMe", "kOlEjNi", 3000 ) );
    assert ( b2 . MedianInvoice () == 2000 );
    assert ( ! b2 . Invoice ( "1234567", 100 ) );
    assert ( ! b2 . Invoice ( "ACE", "Kolejni", 100 ) );
    assert ( ! b2 . Invoice ( "ACME", "Thakurova", 100 ) );
    assert ( ! b2 . Audit ( "1234567", sumIncome ) );
    assert ( ! b2 . Audit ( "ACE", "Kolejni", sumIncome ) );
    assert ( ! b2 . Audit ( "ACME", "Thakurova", sumIncome ) );
    assert ( ! b2 . CancelCompany ( "1234567" ) );
    assert ( ! b2 . CancelCompany ( "ACE", "Kolejni" ) );
    assert ( ! b2 . CancelCompany ( "ACME", "Thakurova" ) );
    assert ( b2 . CancelCompany ( "abcdef" ) );
    assert ( b2 . MedianInvoice () == 2000 );
    assert ( ! b2 . CancelCompany ( "abcdef" ) );
    assert ( b2 . NewCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . CancelCompany ( "ACME", "Kolejni" ) );
    assert ( ! b2 . CancelCompany ( "ACME", "Kolejni" ) );

    assert ( b2 . NewCompany ( "Ugtimjheoooa", "|Pfiluqngqofbba", "Ibjbpgrmmaibba" ) );
    assert ( b2 . NewCompany ( "Ugtimjheoooa|", "Pfiluqngqofbba", "Ibjbpgrmmaibb" ) );
    assert ( ! b2 . NewCompany ( "Ugtimjheoooa", "|pfiluqngqofbba", "Ibjbpgrmmaibba" ) );
    assert ( ! b2 . NewCompany ( "Ugtimjheoooa", "|Pfiluqngqofbba", "ibjbpgrmmaibba" ) );
    assert ( ! b2 . NewCompany ( "Ugtimjheoooa", "|Pfuqngqofbba", "Ibjbpgrmmaibba" ) );
    assert ( b2 . NewCompany ( "Ugtimjheoooa", "pfiluqngqofbba", "ibjbpgrmmaibba" ) );
    assert ( b2 . CancelCompany ( "Ugtimjheoooa", "pfiluqngqofbbA" ) );
    assert ( ! b2 . CancelCompany ( "ibjbpgrmmaibba" ) );
    assert ( ! b2 . CancelCompany ( "Ibjbpgrmmaibba " ) );
    assert ( ! b2 . CancelCompany ( " Ibjbpgrmmaibba" ) );
    assert ( b2 . CancelCompany ( "Ibjbpgrmmaibba" ) );
    assert ( b2 . NewCompany ( "Ugtimjheoooa", "pfiluqngqofbba", "Ibjbpgrmmaibba" ) );
    assert ( ! b2 . CancelCompany ( "ibjbpgrmmaibba" ) );
    assert ( b2 . CancelCompany ( "Ibjbpgrmmaibba" ) );

    assert ( b2 . NewCompany ( "a", "a", "a" ) );
    assert ( b2 . NewCompany ( "", "", "" ) );
    assert ( ! b2 . NewCompany ( "", "", "" ) );
    assert ( ! b2 . NewCompany ( "", "", "a" ) );
    assert ( ! b2 . NewCompany ( "a", "a", "a" ) );
    assert ( b2 . NewCompany ( "|a", "a|", "A" ) );

    CVATRegister c;
    assert ( c . NewCompany ( "Dummy", "Thakurova", "123456" ) );
    assert ( c . Invoice ( "123456", 1 ) );
    assert ( c . Invoice ( "123456", 0 ) );
    assert ( c . Invoice ( "123456", 2 ) );
    assert ( c . Invoice ( "123456", 3 ) );
    assert ( c . Invoice ( "123456", 4 ) );
    assert ( c . MedianInvoice () == 2 );
    assert ( c . Invoice ( "123456", 5 ) );
    assert ( c . MedianInvoice () == 3 );

    return 0;
}
#endif /* __PROGTEST__ */