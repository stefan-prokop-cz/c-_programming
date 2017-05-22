#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;

class CDate
{
public:
    //---------------------------------------------------------------------------------------------
    CDate                         ( int               y,
                                    int               m,
                                    int               d )
            : m_Year ( y ),
              m_Month ( m ),
              m_Day ( d )
    {
    }
    //---------------------------------------------------------------------------------------------
    int                      Compare                       ( const CDate     & x ) const
    {
        if ( m_Year != x . m_Year )
            return m_Year - x . m_Year;
        if ( m_Month != x . m_Month )
            return m_Month - x . m_Month;
        return m_Day - x . m_Day;
    }
    //---------------------------------------------------------------------------------------------
    int                      Year                          ( void ) const
    {
        return m_Year;
    }
    //---------------------------------------------------------------------------------------------
    int                      Month                         ( void ) const
    {
        return m_Month;
    }
    //---------------------------------------------------------------------------------------------
    int                      Day                           ( void ) const
    {
        return m_Day;
    }
    //---------------------------------------------------------------------------------------------
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CDate     & x )
    {
        char oldFill = os . fill ();
        return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-"
                  << setw ( 2 ) << (int) x . m_Month << "-"
                  << setw ( 2 ) << (int) x . m_Day
                  << setfill ( oldFill );
    }
    //---------------------------------------------------------------------------------------------
private:
    int16_t                  m_Year;
    int8_t                   m_Month;
    int8_t                   m_Day;
};
#endif /* __PROGTEST__ */

class CInvoice
{
public:
    string lower_seller, lower_buyer, low_seller, low_buyer;

    CInvoice                      ( const CDate     & date,
                                    const string    & seller,
                                    const string    & buyer,
                                    unsigned int      amount,
                                    double            VAT )
            : date(date), seller(seller), buyer(buyer), amount(amount), vat(VAT)
    {
        lower_buyer = "";
        lower_seller = "";
        low_buyer = "";
        low_seller = "";
        id = -1;
    }
    CInvoice                      ( const CDate     & date,
                                    const string    & seller,
                                    const string    & buyer,
                                    unsigned int      amount,
                                    double            VAT,
                                    const string & l_seller, const string & l_buyer,
                                    const string & lower_a, const string & lower_b)
            : date(date), seller(seller), buyer(buyer), amount(amount), vat(VAT)
    {
        lower_seller = l_seller;
        lower_buyer = l_buyer;
        low_seller = lower_a;
        low_buyer = lower_b;
        id = -1;
    }
    CDate                    Date                          ( void ) const
    {
        return date;
    }
    string                   Buyer                         ( void ) const
    {
        return buyer;
    }
    string                   Seller                        ( void ) const
    {
        return seller;
    }
    int                      Amount                        ( void ) const
    {
        return amount;
    }
    double                   VAT                           ( void ) const{
        return vat;
    }
    bool operator == (const CInvoice & invoice) const
    {
        return ((date.Compare(invoice.date) == 0) && (amount == invoice.amount) && (vat == invoice.vat) &&
                (lower_buyer == invoice.lower_buyer) && (lower_seller == invoice.lower_seller));
    }
    bool operator < (const CInvoice & invoice) const
    {
        if(date.Compare(invoice.Date()) == 0) {
            if (amount == invoice.amount) {
                if (vat == invoice.vat) {
                    if (lower_seller == invoice.lower_seller) {
                        return lower_buyer < invoice.lower_buyer;
                    }

                    return lower_seller < invoice.lower_seller;
                }

                return vat < invoice.vat;
            }

            return amount < invoice.amount;
        }

        return date.Compare(invoice.Date()) < invoice.Date().Compare(date);
    }

    void setSeller(const string &seller) {
        CInvoice::seller = seller;
    }

    void setBuyer(const string &buyer) {
        CInvoice::buyer = buyer;
    }

    void setId(int id) {
        CInvoice::id = id;
    }

    int getId() const {
        return id;
    }

private:
    CDate date;
    string seller, buyer;
    unsigned int amount;
    double vat;
    int id;
};

class CSortOpt
{
public:
    bool default_sort;
    vector<int> sort_params;
    vector<bool> sort_params_ascending;

    static const int         BY_DATE                       = 0;
    static const int         BY_BUYER                      = 1;
    static const int         BY_SELLER                     = 2;
    static const int         BY_AMOUNT                     = 3;
    static const int         BY_VAT                        = 4;
    CSortOpt                      ( void )
    {
        default_sort = true;
    }
    CSortOpt               & AddKey                        ( int               key,
                                                             bool              ascending = true )
    {
        switch(key) {
            case BY_DATE:
                default_sort = false;
                sort_params.push_back(key);
                sort_params_ascending.push_back(ascending);
                break;
            case BY_BUYER:
                default_sort = false;
                sort_params.push_back(key);
                sort_params_ascending.push_back(ascending);
                break;
            case BY_SELLER:
                default_sort = false;
                sort_params.push_back(key);
                sort_params_ascending.push_back(ascending);
                break;
            case BY_AMOUNT:
                default_sort = false;
                sort_params.push_back(key);
                sort_params_ascending.push_back(ascending);
                break;
            case BY_VAT:
                default_sort = false;
                sort_params.push_back(key);
                sort_params_ascending.push_back(ascending);
                break;
            default:
                break;
        }

        return *this;
    }
private:

};

class CCompany
{
public:
    string l_name;

    CCompany(string c_name)
    {
        string tmp = c_name;
        string tmp2 = c_name;
        name = tmp;
        lower_name = toLower(trim(c_name));
        l_name = toLower(tmp2);
    }
    bool operator < (const CCompany & company) const
    {
        return lower_name < company.lower_name;
    }
    string getLowerName() const
    {
        return lower_name;
    }
    string getName() const
    {
        return name;
    }
private:
    string name, lower_name;

    /**
     * Metoda vytvarejici vhodny klic slozeny ze jmena a adresy firmy
     *
     * @param name
     * @param addr
     * @return
     */
    string & toLower(string & name) const
    {
        string new_name = "", new_addr = "";
        transform(name.begin(), name.end(), name.begin(), ::tolower);

        return name;
    }
    /**
     * Metoda odstranujici prebytecne mezery z daneho slova
     *
     * @param value
     * @return
     */
    string & trim(string & text)
    {
        string value = text;
        text = "";
        bool white_space = true;
        for(int i = 0; i < (int) value.size(); i++) {
            if((value[i] == ' ' || value[i] == '\n' || value[i] == '\t') && !white_space) {
                text.push_back(value[i]);
                white_space = true;
            } else if((value[i] == ' ' || value[i] == '\n' || value[i] == '\t') && white_space) {

            } else {
                text.push_back(value[i]);
                white_space = false;
            }
        }

        if(white_space) {
            text.erase(text.end() - 1);
        }

        return text;
    }
};

class CVATRegister
{
public:
    CVATRegister                  ( void ) : companies_count(0), invoice_count(0) {}
    bool                     RegisterCompany               ( const string    & name )
    {
        CCompany company(name);
        set<CInvoice> invoices;
        company_issued.insert(pair<CCompany, set<CInvoice>>(company, invoices));

        return company_inbox.insert(pair<CCompany, set<CInvoice>>(company, invoices)).second;
    }
    bool                     AddIssued                     ( const CInvoice  & x )
    {
        CCompany seller(x.Seller());
        CCompany buyer(x.Buyer());

        auto it_seller = company_issued.find(seller);
        auto it_buyer = company_issued.find(buyer);

        CInvoice invoice = x;
        invoice.lower_seller = seller.getLowerName();
        invoice.lower_buyer = buyer.getLowerName();

        if(it_buyer == company_issued.end() || it_seller == company_issued.end() || invoice.lower_buyer == invoice.lower_seller) {
            return false;
        }

        invoice.setSeller((*it_seller).first.getName());
        invoice.setBuyer((*it_buyer).first.getName());
        invoice.low_buyer = (*it_buyer).first.l_name;
        invoice.low_seller = (*it_seller).first.l_name;
        invoice.setId(invoice_count);

        if((*it_seller).second.insert(invoice).second) {
            (*it_buyer).second.insert(invoice);
            invoice_count++;

            return true;
        }

        return false;
    }
    bool                     AddAccepted                   ( const CInvoice  & x )
    {
        CCompany seller(x.Seller());
        CCompany buyer(x.Buyer());

        auto it_seller = company_inbox.find(seller);
        auto it_buyer = company_inbox.find(buyer);

        CInvoice invoice = x;
        invoice.lower_seller = seller.getLowerName();
        invoice.lower_buyer = buyer.getLowerName();

        if(it_buyer == company_inbox.end() || it_seller == company_inbox.end() || invoice.lower_buyer == invoice.lower_seller) {
            return false;
        }

        invoice.setSeller((*it_seller).first.getName());
        invoice.setBuyer((*it_buyer).first.getName());
        invoice.low_seller = (*it_seller).first.l_name;
        invoice.low_buyer = (*it_buyer).first.l_name;
        invoice.setId(invoice_count);

        if((*it_buyer).second.insert(invoice).second) {
            (*it_seller).second.insert(invoice);
            invoice_count++;

            return true;
        }

        return false;
    }
    bool                     DelIssued                     ( const CInvoice  & x )
    {
        CCompany seller(x.Seller()), buyer(x.Buyer());
        auto it_seller = company_issued.find(seller);
        auto it_buyer = company_issued.find(buyer);

        if(it_buyer != company_issued.end()) {
            (*it_buyer).second.erase(
                    CInvoice(x.Date(), x.Seller(), x.Buyer(), (unsigned int) x.Amount(), x.VAT(), seller.getLowerName(),
                             buyer.getLowerName(), seller.l_name, buyer.l_name));
        }

        if(it_seller != company_issued.end()) {
            return (bool) (*it_seller).second.erase(CInvoice(x.Date(), x.Seller(), x.Buyer(), (unsigned int) x.Amount(), x.VAT(), seller.getLowerName(), buyer.getLowerName(), seller.l_name, buyer.l_name));
        }

        return false;
    }
    bool                     DelAccepted                   ( const CInvoice  & x )
    {
        CCompany seller(x.Seller()), buyer(x.Buyer());
        auto it_seller = company_inbox.find(seller);
        auto it_buyer = company_inbox.find(buyer);

        if(it_seller != company_inbox.end()) {
            (*it_seller).second.erase(
                    CInvoice(x.Date(), x.Seller(), x.Buyer(), (unsigned int) x.Amount(), x.VAT(), seller.getLowerName(),
                             buyer.getLowerName(), seller.l_name, buyer.l_name));
        }

        if(it_buyer != company_inbox.end()) {
            return (bool) (*it_buyer).second.erase(CInvoice(x.Date(), x.Seller(), x.Buyer(), (unsigned int) x.Amount(), x.VAT(), seller.getLowerName(), buyer.getLowerName(), seller.l_name, buyer.l_name));
        }

        return false;
    }
    list<CInvoice>           Unmatched                     ( const string    & company,
                                                             const CSortOpt  & sortBy ) const
    {
        list<CInvoice> invoices;
        invoices = findUnmatched(company);

        return getSortedInvoices(invoices, sortBy);
    }
private:
    int companies_count, invoice_count;
    map<CCompany, set<CInvoice>> company_issued;
    map<CCompany, set<CInvoice>> company_inbox;

    /**
     * Metoda vracejici pocatecni list faktur, ktere nebyly sparovane
     *
     * @param company
     * @return
     */
    list<CInvoice> findUnmatched(const CCompany & company) const
    {
        auto it_issued = company_issued.find(company);
        auto it_inbox = company_inbox.find(company);

        list<CInvoice> invoices;

        if(it_issued != company_issued.end() && it_inbox != company_inbox.end()) {
            std::set_symmetric_difference((*it_issued).second.begin(), (*it_issued).second.end(),
                                          (*it_inbox).second.begin(), (*it_inbox).second.end(), std::inserter(invoices, invoices.begin()));
        }

        return invoices;
    }
    /**
     * Metoda serazujici list faktur podle defaultnich parametru - ID
     *
     * @param invoices
     * @return
     */
    bool sortDefault(const CInvoice & a, const CInvoice & b) const
    {
        return a.getId() < b.getId();
    }
    /**
     * Metoda serazujici list faktur podle datumu
     *
     * @param invoices
     * @param ascenging
     * @return
     */
    bool sortByDate(const CInvoice & a, const CInvoice & b, const bool ascending) const
    {
        if(ascending) {
            if (a.Date().Year() == b.Date().Year()) {
                if (a.Date().Month() == b.Date().Month()) {
                    return a.Date().Day() < b.Date().Day();
                }

                return a.Date().Month() < b.Date().Month();
            }

            return a.Date().Year() < b.Date().Year();
        } else {
            if (a.Date().Year() == b.Date().Year()) {
                if (a.Date().Month() == b.Date().Month()) {
                    return a.Date().Day() > b.Date().Day();
                }

                return a.Date().Month() > b.Date().Month();
            }

            return a.Date().Year() > b.Date().Year();
        }
    }
    /**
     * Metoda serazujici list faktur podle kupujiciho
     *
     * @param invoices
     * @param ascending
     * @return
     */
    bool sortByBuyer(const CInvoice & a, const CInvoice & b, const bool ascending) const
    {
        if(ascending) {
            return a.low_buyer < b.low_buyer;
        } else {
            return a.low_buyer > b.low_buyer;
        }
    }
    /**
     * Metoda serazujici list faktur podle prodavajiciho
     *
     * @param invoices
     * @param ascending
     * @return
     */
    bool sortBySeller(const CInvoice & a, const CInvoice & b, const bool ascending) const
    {
        if(ascending) {
            return a.low_seller < b.low_seller;
        } else {
            return a.low_seller > b.low_seller;
        }
    }
    /**
     * Metoda serazujici list faktur podle castky
     *
     * @param invoices
     * @param ascending
     * @return
     */
    bool sortByAmount(const CInvoice & a, const CInvoice & b, const bool ascending) const
    {
        if(ascending) {
            return a.Amount() < b.Amount();
        } else {
            return a.Amount() > b.Amount();
        }
    }
    /**
     * Metoda serazujici list faktur podle DPH
     *
     * @param invoices
     * @param ascenging
     * @return
     */
    bool sortByVat(const CInvoice & a, const CInvoice & b, const bool ascending) const
    {
        if(ascending) {
            return a.VAT() < b.VAT();
        } else {
            return a.VAT() > b.VAT();
        }
    }
    /**
     * Metoda tridici list faktur na zaklade zadaneho porovnavaciho klice
     *
     * @param invoices
     * @param key
     * @return
     */
    list<CInvoice> getSortedInvoices(list<CInvoice> & invoices, const CSortOpt & sortBy) const
    {
        invoices.sort([&](const CInvoice &a, const CInvoice &b) {
            for (int i = 0; i < (int) sortBy.sort_params.size(); i++) {
                switch (sortBy.sort_params[i]) {
                    case CSortOpt::BY_DATE:
                        if(a.Date().Compare(b.Date()) != 0) {
                            return this->sortByDate(a, b, sortBy.sort_params_ascending[i]);
                        }
                        break;
                    case CSortOpt::BY_BUYER:
                        if(a.low_buyer != b.low_buyer) {
                            return this->sortByBuyer(a, b, sortBy.sort_params_ascending[i]);
                        }
                        break;
                    case CSortOpt::BY_SELLER:
                        if(a.low_seller != b.low_seller) {
                            return this->sortBySeller(a, b, sortBy.sort_params_ascending[i]);
                        }
                        break;
                    case CSortOpt::BY_AMOUNT:
                        if(a.Amount() != b.Amount()) {
                            return this->sortByAmount(a, b, sortBy.sort_params_ascending[i]);
                        }
                        break;
                    case CSortOpt::BY_VAT:
                        if(a.VAT() != b.VAT()) {
                            return this->sortByVat(a, b, sortBy.sort_params_ascending[i]);
                        }
                        break;
                    default:
                        return this->sortDefault(a, b);
                }
            }

            return this->sortDefault(a, b);
        });

        return invoices;
    }
};

#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{
    if(a.size() != b.size()) {
        return false;
    }

    for(auto i = a.cbegin(), i2 = b.cbegin(); i != a.cend(), i2 != b.cend(); i++, i2++) {
        if(i->Seller() != i2->Seller() || i->Date().Compare(i2->Date()) != 0 || i->VAT() != i2->VAT() || i->Amount() != i2->Amount() || i->Buyer() != i2->Buyer()) {
            return false;
        }
    }

    return true;
}

int main ( void )
{
    CVATRegister r;
    assert ( r . RegisterCompany ( "first Company" ) );
    assert ( r . RegisterCompany ( "Second     Company" ) );
    assert ( r . RegisterCompany ( "ThirdCompany, Ltd." ) );
    assert ( r . RegisterCompany ( "Third Company, Ltd." ) );
    assert ( !r . RegisterCompany ( "Third Company, Ltd." ) );
    assert ( !r . RegisterCompany ( " Third  Company,  Ltd.  " ) );

    assert ( r . AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
    assert ( r . AddIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
    assert ( r . AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
    assert ( r . AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( r . AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
    assert ( r . AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
    assert ( !r . AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( !r . AddIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
    assert ( !r . AddIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );

    assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, false ) . AddKey ( CSortOpt::BY_DATE, false ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
                                  } ) );


    assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_DATE, true ) . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
                                  } ) );

    assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_VAT, true ) . AddKey ( CSortOpt::BY_AMOUNT, true ) . AddKey ( CSortOpt::BY_DATE, true ) . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
                                  } ) );

    assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
                                  } ) );

    assert ( equalLists ( r . Unmatched ( "second company", CSortOpt () . AddKey ( CSortOpt::BY_DATE, false ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
                                  } ) );
    assert ( equalLists ( r . Unmatched ( "last company", CSortOpt () . AddKey ( CSortOpt::BY_VAT, true ) ),
                          list<CInvoice>
                                  {

                                  } ) );

    assert ( r . AddAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( r . AddAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . AddAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );

    assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) . AddKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );

    assert ( !r . DelIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
    assert ( !r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
    assert ( !r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
    assert ( r . DelIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );

    assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) . AddKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );

    assert ( r . DelAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );

    assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) . AddKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );

    assert ( r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );

    assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) . AddKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );

    return 0;
}
#endif /* __PROGTEST__ */