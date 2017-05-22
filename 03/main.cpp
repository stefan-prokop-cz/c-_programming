#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>

using namespace std;
#endif /* __PROGTEST__ */

int ALLOCATED = 100;

class BankTmp
{
public:
    int references;
    // default constructor
    BankTmp() : references(1), account_count(0), allocated(ALLOCATED), transaction_count(0), transaction_allocated(ALLOCATED), acc_id_acc(-1)
    {
        account_id = new char*[allocated];
        deposit = new int[allocated];
        count = new int[allocated];
        trans_ids = new int*[allocated];

        cred_account_id = new char**[allocated];
        deb_account_id = new char**[allocated];
        sign = new char**[allocated];
        value = new unsigned int*[allocated];
    }
    // copy constructor
    BankTmp(const BankTmp & bank)
    {
        references = 1;
        account_count = bank.account_count;
        allocated = bank.allocated;
        transaction_count = bank.transaction_count;
        transaction_allocated = bank.transaction_allocated;
        acc_id_acc = -1;

        account_id = new char*[allocated];
        deposit = new int[allocated];
        count = new int[allocated];
        trans_ids = new int*[allocated];

        cred_account_id = new char**[allocated];
        deb_account_id = new char**[allocated];
        sign = new char**[allocated];
        value = new unsigned int*[allocated];

        for(int i = 0; i < account_count; i++) {
            account_id[i] = new char[strlen(bank.account_id[i]) + 1];
            strcpy(account_id[i], bank.account_id[i]);
            deposit[i] = bank.deposit[i];
            count[i] = bank.count[i];
            trans_ids[i] = new int[2];
            trans_ids[i][0] = bank.trans_ids[i][0];
            trans_ids[i][1] = bank.trans_ids[i][1];

            cred_account_id[i] = new char*[trans_ids[i][1]];
            deb_account_id[i] = new char*[trans_ids[i][1]];
            sign[i] = new char*[trans_ids[i][1]];
            value[i] = new unsigned int[trans_ids[i][1]];

            for(int j = 0; j < trans_ids[i][0]; j++) {
                value[i][j] = bank.value[i][j];
                cred_account_id[i][j] = new char[strlen(bank.cred_account_id[i][j]) + 1];
                strcpy(cred_account_id[i][j], bank.cred_account_id[i][j]);
                deb_account_id[i][j] = new char[strlen(bank.deb_account_id[i][j]) + 1];
                strcpy(deb_account_id[i][j], bank.deb_account_id[i][j]);
                sign[i][j] = new char[strlen(bank.sign[i][j]) + 1];
                strcpy(sign[i][j], bank.sign[i][j]);
            }
        }
    }
    // destructor
    void destruct()
    {
        delete [] deposit;
        delete [] count;

        for(int i = 0; i < account_count; i++) {
            delete [] account_id[i];
            delete [] value[i];

            for(int j = 0; j < trans_ids[i][0]; j++) {
                delete[] deb_account_id[i][j];
                delete[] cred_account_id[i][j];
                delete[] sign[i][j];
            }

            delete [] deb_account_id[i];
            delete [] cred_account_id[i];
            delete [] sign[i];
            delete [] trans_ids[i];
        }

        delete [] account_id;
        delete [] deb_account_id;
        delete [] cred_account_id;
        delete [] sign;
        delete [] trans_ids;
        delete [] value;
    }
    // operator =
    BankTmp & operator = (const BankTmp & bank)
    {
        if(&bank == this) {
            return *this;
        }

        delete [] deposit;
        delete [] count;

        for(int i = 0; i < account_count; i++) {
            delete [] account_id[i];
            delete [] value[i];

            for(int j = 0; j < trans_ids[i][0]; j++) {
                delete[] deb_account_id[i][j];
                delete[] cred_account_id[i][j];
                delete[] sign[i][j];
            }

            delete [] deb_account_id[i];
            delete [] cred_account_id[i];
            delete [] sign[i];
            delete [] trans_ids[i];
        }

        delete [] account_id;
        delete [] deb_account_id;
        delete [] cred_account_id;
        delete [] sign;
        delete [] trans_ids;
        delete [] value;

        account_count = bank.account_count;
        allocated = bank.allocated;
        transaction_count = bank.transaction_count;
        transaction_allocated = bank.transaction_allocated;
        acc_id_acc = -1;

        account_id = new char*[allocated];
        deposit = new int[allocated];
        count = new int[allocated];
        trans_ids = new int*[allocated];

        cred_account_id = new char**[allocated];
        deb_account_id = new char**[allocated];
        sign = new char**[allocated];
        value = new unsigned int*[allocated];

        for(int i = 0; i < account_count; i++) {
            account_id[i] = new char[strlen(bank.account_id[i]) + 1];
            strcpy(account_id[i], bank.account_id[i]);
            deposit[i] = bank.deposit[i];
            count[i] = bank.count[i];
            trans_ids[i] = new int[2];
            trans_ids[i][0] = bank.trans_ids[i][0];
            trans_ids[i][1] = bank.trans_ids[i][1];

            cred_account_id[i] = new char*[trans_ids[i][1]];
            deb_account_id[i] = new char*[trans_ids[i][1]];
            sign[i] = new char*[trans_ids[i][1]];
            value[i] = new unsigned int[trans_ids[i][1]];

            for(int j = 0; j < trans_ids[i][0]; j++) {
                value[i][j] = bank.value[i][j];
                cred_account_id[i][j] = new char[strlen(bank.cred_account_id[i][j]) + 1];
                strcpy(cred_account_id[i][j], bank.cred_account_id[i][j]);
                deb_account_id[i][j] = new char[strlen(bank.deb_account_id[i][j]) + 1];
                strcpy(deb_account_id[i][j], bank.deb_account_id[i][j]);
                sign[i][j] = new char[strlen(bank.sign[i][j]) + 1];
                strcpy(sign[i][j], bank.sign[i][j]);
            }
        }

        return *this;
    }

    ostream & getString(ostream & stream, const BankTmp & account)
    {
        stream << account.account_id[account.acc_id_acc] << ":\n" << "   " << account.deposit[account.acc_id_acc] << "\n";

        for(int i = 0; i < account.trans_ids[account.acc_id_acc][0]; i++) {
            if(strcmp(account.account_id[account.acc_id_acc], account.deb_account_id[account.acc_id_acc][i]) == 0) {
                stream << " - " << account.value[account.acc_id_acc][i] << ", " << "to: " << account.cred_account_id[account.acc_id_acc][i] << ", ";
                stream << "sign: " << account.sign[account.acc_id_acc][i] << "\n";
            } else if(strcmp(account.account_id[account.acc_id_acc], account.cred_account_id[account.acc_id_acc][i]) == 0) {
                stream << " + " << account.value[account.acc_id_acc][i] << ", " << "from: " << account.deb_account_id[account.acc_id_acc][i] << ", ";
                stream << "sign: " << account.sign[account.acc_id_acc][i] << "\n";
            }
        }

        stream << " = " << account.count[account.acc_id_acc] << "\n";

        return stream;
    }
    bool   NewAccount    ( const char * accID,
                           int          initialBalance )
    {
        if(findAccount(accID) != -1) {
            return false;
        }

        if(account_count >= allocated) {
            realloc();
        }

        account_id[account_count] = new char[strlen(accID) + 1];
        strcpy(account_id[account_count], accID);
        deposit[account_count] = initialBalance;
        count[account_count] = initialBalance;

        cred_account_id[account_count] = new char*[ALLOCATED];
        deb_account_id[account_count] = new char*[ALLOCATED];
        sign[account_count] = new char*[ALLOCATED];
        value[account_count] = new unsigned int[ALLOCATED];
        trans_ids[account_count] = new int[2];

        trans_ids[account_count][0] = 0;
        trans_ids[account_count][1] = ALLOCATED;

        account_count++;

        return true;
    }
    bool   Transaction   ( const char * debAccID,
                           const char * credAccID,
                           int          amount,
                           const char * signature )
    {
        int debId = findAccount(debAccID), credId = findAccount(credAccID);

        if(debId == -1 || credId == -1 || amount < 0 || debAccID == credAccID) {
            return false;
        }

        if(trans_ids[debId][0] >= trans_ids[debId][1]) {
            reallocTransaction(debId);
        }

        deb_account_id[debId][trans_ids[debId][0]] = new char[strlen(debAccID) + 1];
        strcpy(deb_account_id[debId][trans_ids[debId][0]], debAccID);
        cred_account_id[debId][trans_ids[debId][0]] = new char[strlen(credAccID) + 1];
        strcpy(cred_account_id[debId][trans_ids[debId][0]], credAccID);
        sign[debId][trans_ids[debId][0]] = new char[strlen(signature) + 1];
        strcpy(sign[debId][trans_ids[debId][0]], signature);
        value[debId][trans_ids[debId][0]] = (unsigned int) amount;
        trans_ids[debId][0] = trans_ids[debId][0] + 1;

        if(trans_ids[debId][0] >= trans_ids[debId][1]) {
            reallocTransaction(credId);
        }

        deb_account_id[credId][trans_ids[credId][0]] = new char[strlen(debAccID) + 1];
        strcpy(deb_account_id[credId][trans_ids[credId][0]], debAccID);
        cred_account_id[credId][trans_ids[credId][0]] = new char[strlen(credAccID) + 1];
        strcpy(cred_account_id[credId][trans_ids[credId][0]], credAccID);
        sign[credId][trans_ids[credId][0]] = new char[strlen(signature) + 1];
        strcpy(sign[credId][trans_ids[credId][0]], signature);
        value[credId][trans_ids[credId][0]] = (unsigned int) amount;
        trans_ids[credId][0] = trans_ids[credId][0] + 1;

        count[debId] = count[debId] - amount;
        count[credId] = count[credId] + amount;

        transaction_count++;

        return true;
    }
    bool   TrimAccount   ( const char * accID )
    {
        int id = findAccount((char *) accID);

        if(id == -1) {
            return false;
        }

        deposit[id] = count[id];

        for(int j = 0; j < trans_ids[id][0]; j++) {
            delete[] deb_account_id[id][j];
            delete[] cred_account_id[id][j];
            delete[] sign[id][j];
        }

        delete [] deb_account_id[id];
        delete [] cred_account_id[id];
        delete [] sign[id];
        delete [] value[id];

        cred_account_id[id] = new char*[ALLOCATED];
        deb_account_id[id] = new char*[ALLOCATED];
        sign[id] = new char*[ALLOCATED];
        value[id] = new unsigned int[ALLOCATED];

        trans_ids[id][0] = 0;
        trans_ids[id][1] = ALLOCATED;

        return true;
    }
    // Account ( accID )
    BankTmp & Account(const char * accId)
    {
        acc_id_acc = findAccount(accId);

        if(findAccount(accId) == -1) {
            throw 500;
        }

        return *this;
    }
    int Balance()
    {
        int balance = count[acc_id_acc];
        acc_id_acc = -1;

        return balance;
    }
private:
    int account_count, allocated, transaction_count, transaction_allocated, acc_id_acc;

    char ** account_id;
    int * deposit;
    int * count;

    char *** deb_account_id;
    char *** cred_account_id;
    char *** sign;
    unsigned int ** value;

    int ** trans_ids;

    /**
     * Metoda vyhledavajici account v databazi
     *
     * @param accId
     * @return
     */
    int findAccount(const char * accId) const
    {
        for(int i = 0; i < account_count; i++) {
            if(strcmp(account_id[i], accId) == 0) {
                return i;
            }
        }

        return -1;
    }

    /**
     * Metoda reallokujici databazi
     */
    void realloc()
    {
        allocated += ALLOCATED;
        char *** tmp_sign = new char**[allocated];
        char *** tmp_deb = new char**[allocated];
        char *** tmp_cred = new char**[allocated];

        char ** tmp_account_id = new char*[allocated];
        int * tmp_deposit = new int[allocated];
        int * tmp_count = new int[allocated];
        unsigned int ** tmp_value = new unsigned int*[allocated];
        int ** tmp_trans_ids = new int*[allocated];

        for(int i = 0; i < account_count; i++) {
            tmp_value[i] = new unsigned int[trans_ids[i][1]];
            tmp_sign[i] = new char*[trans_ids[i][1]];
            tmp_deb[i] = new char*[trans_ids[i][1]];
            tmp_cred[i] = new char*[trans_ids[i][1]];
            tmp_trans_ids[i] = new int[2];
            tmp_account_id[i] = new char[strlen(account_id[i]) + 1];
            strcpy(tmp_account_id[i], account_id[i]);
            tmp_deposit[i] = deposit[i];
            tmp_count[i] = count[i];
            tmp_trans_ids[i][0] = trans_ids[i][0];
            tmp_trans_ids[i][1] = trans_ids[i][1];

            for(int j = 0; j < trans_ids[i][0]; j++) {
                tmp_sign[i][j] = new char[strlen(sign[i][j]) + 1];
                tmp_deb[i][j] = new char[strlen(deb_account_id[i][j]) + 1];
                tmp_cred[i][j] = new char[strlen(cred_account_id[i][j]) + 1];

                strcpy(tmp_sign[i][j], sign[i][j]);
                strcpy(tmp_cred[i][j], cred_account_id[i][j]);
                strcpy(tmp_deb[i][j], deb_account_id[i][j]);
                tmp_value[i][j] = value[i][j];

                delete[] sign[i][j];
                delete[] cred_account_id[i][j];
                delete[] deb_account_id[i][j];
            }

            delete [] account_id[i];
            delete [] value[i];
            delete [] trans_ids[i];
            delete [] sign[i];
            delete [] cred_account_id[i];
            delete [] deb_account_id[i];
        }

        delete [] account_id;
        delete [] deposit;
        delete [] count;
        delete [] trans_ids;
        delete [] value;

        delete [] sign;
        delete [] cred_account_id;
        delete [] deb_account_id;

        account_id = tmp_account_id;
        count = tmp_count;
        deposit = tmp_deposit;
        trans_ids = tmp_trans_ids;

        sign = tmp_sign;
        deb_account_id = tmp_deb;
        cred_account_id = tmp_cred;
        value = tmp_value;
    }
    /**
     * Metoda reallokujici databazi trasakci
     *
     * @param accId
     */
    void reallocTransaction(int accId)
    {
        trans_ids[accId][1] = trans_ids[accId][1] + ALLOCATED;

        char ** tmp_deb_account_id = new char*[trans_ids[accId][1]];
        char ** tmp_cred_account_id = new char*[trans_ids[accId][1]];
        char ** tmp_sign = new char*[trans_ids[accId][1]];
        unsigned int * tmp_value = new unsigned int[trans_ids[accId][1]];

        for(int i = 0; i < trans_ids[accId][0]; i++) {
            tmp_deb_account_id[i] = new char[strlen(deb_account_id[accId][i]) + 1];
            strcpy(tmp_deb_account_id[i], deb_account_id[accId][i]);
            delete [] deb_account_id[accId][i];

            tmp_cred_account_id[i] = new char[strlen(cred_account_id[accId][i]) + 1];
            strcpy(tmp_cred_account_id[i], cred_account_id[accId][i]);
            delete [] cred_account_id[accId][i];

            tmp_sign[i] = new char[strlen(sign[accId][i]) + 1];
            strcpy(tmp_sign[i], sign[accId][i]);
            delete [] sign[accId][i];

            tmp_value[i] = value[accId][i];
        }

        delete [] value[accId];
        delete [] deb_account_id[accId];
        delete [] cred_account_id[accId];
        delete [] sign[accId];

        value[accId] = tmp_value;
        cred_account_id[accId] = tmp_cred_account_id;
        deb_account_id[accId] = tmp_deb_account_id;
        sign[accId] = tmp_sign;
    }
};

class CBank
{
public:
    CBank()
    {
        bank = new BankTmp();
    }
    ~CBank()
    {
        if(--bank->references == 0) {
            bank->destruct();
            delete bank;
        }
    }
    CBank(const CBank & b)
    {
        bank = b.bank;
        bank->references += 1;
    }
    CBank & operator = (const CBank & b)
    {
        if(&b == this) {
            return *this;
        }

        if(--bank->references == 0) {
            bank->destruct();
            delete bank;
        }

        bank = b.bank;
        bank->references += 1;

        return *this;
    }
    bool   NewAccount    ( const char * accID,
                           int          initialBalance )
    {
        return bank->NewAccount(accID, initialBalance);
    }
    bool   Transaction   ( const char * debAccID,
                           const char * credAccID,
                           int          amount,
                           const char * signature )
    {
        if(bank->references > 1) {
            bank->references -= 1;
            bank = new BankTmp(*bank);
        }

        return bank->Transaction(debAccID, credAccID, amount, signature);
    }
    bool   TrimAccount   ( const char * accID )
    {
        return bank->TrimAccount(accID);
    }
    CBank & Account(const char * accId)
    {
        try {
            bank->Account(accId);

            return *this;
        } catch (...) {
            throw 500;
        }
    }
    friend ostream & operator << (ostream & stream, const CBank & account)
    {
        return account.bank->getString(stream, *account.bank);
    }
    int Balance()
    {
        return bank->Balance();
    }
private:
    BankTmp * bank;
};

#ifndef __PROGTEST__
int main ( void )
{
    ostringstream os;
    char accCpy[100], debCpy[100], credCpy[100], signCpy[100];
    CBank x0;
    assert ( x0 . NewAccount ( "123456", 1000 ) );
    assert ( x0 . NewAccount ( "987654", -500 ) );
    assert ( x0 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
    assert ( x0 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
    assert ( x0 . NewAccount ( "111111", 5000 ) );
    assert ( x0 . Transaction ( "111111", "987654", 290, "Okh6e+8rAiuT5=" ) );
    assert ( x0 . Account ( "123456" ). Balance ( ) ==  -2190 );
    assert ( x0 . Account ( "987654" ). Balance ( ) ==  2980 );
    assert ( x0 . Account ( "111111" ). Balance ( ) ==  4710 );
    os . str ( "" );
    os << x0 . Account ( "123456" );
    assert ( ! strcmp ( os . str () . c_str (), "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );
    os . str ( "" );
    os << x0 . Account ( "987654" );
    assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 290, from: 111111, sign: Okh6e+8rAiuT5=\n = 2980\n" ) );
    os . str ( "" );
    os << x0 . Account ( "111111" );
    assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 290, to: 987654, sign: Okh6e+8rAiuT5=\n = 4710\n" ) );
    assert ( x0 . TrimAccount ( "987654" ) );
    assert ( x0 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
    os . str ( "" );
    os << x0 . Account ( "987654" );
    assert ( ! strcmp ( os . str () . c_str (), "987654:\n   2980\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 3103\n" ) );

    CBank x2;
    strncpy ( accCpy, "123456", sizeof ( accCpy ) );
    assert ( x2 . NewAccount ( accCpy, 1000 ));
    strncpy ( accCpy, "987654", sizeof ( accCpy ) );
    assert ( x2 . NewAccount ( accCpy, -500 ));
    strncpy ( debCpy, "123456", sizeof ( debCpy ) );
    strncpy ( credCpy, "987654", sizeof ( credCpy ) );
    strncpy ( signCpy, "XAbG5uKz6E=", sizeof ( signCpy ) );
    assert ( x2 . Transaction ( debCpy, credCpy, 300, signCpy ) );
    strncpy ( debCpy, "123456", sizeof ( debCpy ) );
    strncpy ( credCpy, "987654", sizeof ( credCpy ) );
    strncpy ( signCpy, "AbG5uKz6E=", sizeof ( signCpy ) );
    assert ( x2 . Transaction ( debCpy, credCpy, 2890, signCpy ) );
    strncpy ( accCpy, "111111", sizeof ( accCpy ) );
    assert ( x2 . NewAccount ( accCpy, 5000 ));
    strncpy ( debCpy, "111111", sizeof ( debCpy ) );
    strncpy ( credCpy, "987654", sizeof ( credCpy ) );
    strncpy ( signCpy, "Okh6e+8rAiuT5=", sizeof ( signCpy ) );
    assert ( x2 . Transaction ( debCpy, credCpy, 2890, signCpy ) );
    assert ( x2 . Account ( "123456" ). Balance ( ) ==  -2190 );
    assert ( x2 . Account ( "987654" ). Balance ( ) ==  5580 );
    assert ( x2 . Account ( "111111" ). Balance ( ) ==  2110 );
    os . str ( "" );
    os << x2 . Account ( "123456" );
    assert ( ! strcmp ( os . str () . c_str (), "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );
    os . str ( "" );
    os << x2 . Account ( "987654" );
    assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n = 5580\n" ) );
    os . str ( "" );
    os << x2 . Account ( "111111" );
    assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n = 2110\n" ) );
    assert ( x2 . TrimAccount ( "987654" ) );
    strncpy ( debCpy, "111111", sizeof ( debCpy ) );
    strncpy ( credCpy, "987654", sizeof ( credCpy ) );
    strncpy ( signCpy, "asdf78wrnASDT3W", sizeof ( signCpy ) );
    assert ( x2 . Transaction ( debCpy, credCpy, 123, signCpy ) );
    os . str ( "" );
    os << x2 . Account ( "987654" );
    assert ( ! strcmp ( os . str () . c_str (), "987654:\n   5580\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );

    CBank x4;
    assert ( x4 . NewAccount ( "123456", 1000 ) );
    assert ( x4 . NewAccount ( "987654", -500 ) );
    assert ( !x4 . NewAccount ( "123456", 3000 ) );
    assert ( !x4 . Transaction ( "123456", "666", 100, "123nr6dfqkwbv5" ) );
    assert ( !x4 . Transaction ( "666", "123456", 100, "34dGD74JsdfKGH" ) );
    assert ( !x4 . Transaction ( "123456", "123456", 100, "Juaw7Jasdkjb5" ) );
    try
    {
        x4 . Account ( "666" ). Balance ( );
        assert ( "Missing exception !!" == NULL );
    }
    catch ( ... )
    {
    }
    try
    {
        os << x4 . Account ( "666" ). Balance ( );
        assert ( "Missing exception !!" == NULL );
    }
    catch ( ... )
    {
    }
    assert ( !x4 . TrimAccount ( "666" ) );

    CBank x6;
    assert ( x6 . NewAccount ( "123456", 1000 ) );
    assert ( x6 . NewAccount ( "987654", -500 ) );
    assert ( x6 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
    assert ( x6 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
    assert ( x6 . NewAccount ( "111111", 5000 ) );
    assert ( x6 . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );
    CBank x7 ( x6 );
    assert ( x6 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
    assert ( x7 . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
    assert ( x6 . NewAccount ( "99999999", 7000 ) );
    assert ( x6 . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" ) );
    assert ( x6 . TrimAccount ( "111111" ) );
    assert ( x6 . Transaction ( "123456", "111111", 221, "Q23wr234ER==" ) );
    os . str ( "" );
    os << x6 . Account ( "111111" );
    assert ( ! strcmp ( os . str () . c_str (), "111111:\n   -1802\n + 221, from: 123456, sign: Q23wr234ER==\n = -1581\n" ) );
    os . str ( "" );
    os << x6 . Account ( "99999999" );
    assert ( ! strcmp ( os . str () . c_str (), "99999999:\n   7000\n + 3789, from: 111111, sign: aher5asdVsAD\n = 10789\n" ) );
    os . str ( "" );
    os << x6 . Account ( "987654" );
    assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );
    os . str ( "" );
    os << x7 . Account ( "111111" );
    assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );
    try
    {
        os << x7 . Account ( "99999999" ). Balance ( );
        assert ( "Missing exception !!" == NULL );
    }
    catch ( ... )
    {
    }
    os . str ( "" );
    os << x7 . Account ( "987654" );
    assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 789, from: 111111, sign: SGDFTYE3sdfsd3W\n = 6369\n" ) );

    CBank x8;
    CBank x9;
    assert ( x8 . NewAccount ( "123456", 1000 ) );
    assert ( x8 . NewAccount ( "987654", -500 ) );
    assert ( x8 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
    assert ( x8 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
    assert ( x8 . NewAccount ( "111111", 5000 ) );
    assert ( x8 . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );
    x9 = x8;
    assert ( x8 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
    assert ( x9 . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
    assert ( x8 . NewAccount ( "99999999", 7000 ) );
    assert ( x8 . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" ) );
    assert ( x8 . TrimAccount ( "111111" ) );
    os . str ( "" );
    os << x8 . Account ( "111111" );
    assert ( ! strcmp ( os . str () . c_str (), "111111:\n   -1802\n = -1802\n" ) );
    os . str ( "" );
    os << x9 . Account ( "111111" );
    assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );
// */
    return 0;
}
#endif /* __PROGTEST__ */