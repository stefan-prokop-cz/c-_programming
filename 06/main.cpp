/**
 * 07 - progtest - Prazdniny se blizi I
 */

#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST_ */

/**
 * Trida obalujici veskere veci
 */
class CThing
{
public:
    CThing() {}
    virtual ~CThing() {}
    virtual int getWeight() const = 0;
    virtual CThing * clone() const = 0;
    virtual bool isDangerous() const = 0;
    virtual ostream & print(ostream & os) const { return os; }
    friend ostream &operator << (ostream & os, const CThing & thing)
    {
        return thing.print(os);
    }
protected:
    int weight;
};
class CKnife : public CThing
{
public:
    // CKnife ( bladeLength );
    CKnife(int blade_length) : CThing(), blade_length(blade_length), weight(100) {}
    virtual CKnife * clone() const
    {
        return new CKnife(*this);
    }
    ostream & print(ostream & os) const
    {
        os << "Knife, blade: " << blade_length << " cm";
        return CThing::print(os);
    }
    virtual bool isDangerous() const
    {
        return blade_length > 7;

    }
    int getBlade_length() const {
        return blade_length;
    }
    int getWeight() const {
        return weight;
    }
private:
    int blade_length, weight;
};

class CClothes : public CThing
{
public:
    // CClothes ( desc );
    CClothes(const string & description) : CThing(), description(description), weight(500) {}
    const string &getDescription() const {
        return description;
    }
    virtual CClothes * clone() const
    {
        return new CClothes(*this);
    }
    ostream & print(ostream & os) const
    {
        os << "Clothes (" << description << ")";

        return CThing::print(os);
    }
    virtual bool isDangerous() const
    {
        return false;
    }
    int getWeight() const {
        return weight;
    }
private:
    string description;
    int weight;
};

class CShoes : public CThing
{
public:
    // CShoes ();
    CShoes() : CThing(), weight(750) {}
    virtual CShoes * clone() const
    {
        return new CShoes(*this);
    }
    ostream & print(ostream & os) const
    {
        os << "Shoes";

        return CThing::print(os);
    }
    virtual bool isDangerous() const
    {
        return false;
    }
    int getWeight() const {
        return weight;
    }
private:
    int weight;
};

class CMobile : public CThing
{
public:
    // CMobile ( manufacturer, model );
    CMobile(const string & manufacturer, const string & model) : CThing(), manufacturer(manufacturer), model(model),
                weight(150) {}
    virtual CMobile * clone() const
    {
        return new CMobile(*this);
    }
    ostream & print(ostream & os) const
    {
        os << "Mobile " << model << " by: " << manufacturer;

        return CThing::print(os);
    }
    virtual bool isDangerous() const
    {
        return manufacturer == "Samsung" && model == "Galaxy Note S7";

    }
    const string &getManufacturer() const {
        return manufacturer;
    }
    const string &getModel() const {
        return model;
    }
    int getWeight() const {
        return weight;
    }
private:
    string manufacturer, model;
    int weight;
};
/**
 * Trida obalujici zavazadla
 */
class CLuggage
{
public:
    CLuggage() : things_count(0), max_weight(0), actual_things_weight(0), danger(0),
                 shoes_count(0) {}
    CLuggage(int max_weight) : things_count(0), max_weight(max_weight), actual_things_weight(max_weight), danger(0),
                shoes_count(0) {}
    virtual ~CLuggage()
    {
        if(things_count > 0) {
            for(int i = 0; i < things_count; i++) {
                delete things[i];
            }
        }
    }
    int getCount() const
    {
        return things_count;
    }
    int getWeight() const
    {
        return actual_things_weight;
    }
    bool getDanger() const
    {
        return (bool) danger;
    }
    CLuggage &operator = (const CLuggage & luggage)
    {
        if(this == &luggage) {
            return *this;
        }

        for(int i = 0; i < things_count; i++) {
            delete things[i];
        }
        things.clear();

        max_weight = luggage.max_weight;
        things_count = luggage.things_count;
        actual_things_weight = luggage.actual_things_weight;
        danger = luggage.danger;
        shoes_count = luggage.shoes_count;

        for (int i = 0; i < luggage.things_count; i++) {
            things.push_back(luggage.things[i]->clone());
        }

        return *this;
    }
    bool getIdenticalContents(const CLuggage & luggage) const
    {
        if(this == &luggage) {
            return true;
        }

        if(things.size() != luggage.things.size() || shoes_count != luggage.shoes_count) {
            return false;
        }

        for(int i = 0; i < things_count; i++) {
            auto it = lower_bound(luggage.things.begin(), luggage.things.end(), things[i], [&] (CThing * a, CThing * b) {
                if(CKnife * knife_a = dynamic_cast<CKnife*>(a)) {
                    if(CKnife * knife_b = dynamic_cast<CKnife*>(b)) {
                        return knife_a->getBlade_length() < knife_b->getBlade_length();
                    }

                    return false;
                } else if(CMobile * mobile_a = dynamic_cast<CMobile*>(a)) {
                    if(CMobile * mobile_b = dynamic_cast<CMobile*>(b)) {
                        if(mobile_a->getModel() == mobile_b->getModel()) {
                            return mobile_a->getManufacturer() < mobile_b->getManufacturer();
                        }

                        return false;
                    }

                    return false;
                } else if(CShoes * shoes_a = dynamic_cast<CShoes*>(a)) {
                    if(CShoes * shoes_b = dynamic_cast<CShoes*>(b)) {
                        return shoes_a->getWeight() < shoes_b->getWeight();
                    }

                    return false;
                } else if(CClothes * clothes_a = dynamic_cast<CClothes*>(a)) {
                    if(CClothes * clothes_b = dynamic_cast<CClothes*>(b)) {
                        return clothes_a->getDescription() < clothes_b->getDescription();
                    }

                    return false;
                }

                return false;
            });

            if(it == luggage.things.end()) {
                return false;
            }
        }

        return true;
    }
    /**
     * Metoda pridavajici do zavazadla dalsi vec
     *
     * @param thing
     */
    void addThing(CThing * thing)
    {
        //things.insert(pair<int, CThing*>(things_count, &thing));
        things.push_back(thing->clone());
        actual_things_weight += thing->getWeight();

        if(CShoes * shoes = dynamic_cast<CShoes*>(thing)) {
            shoes_count++;
            shoes->getWeight();
        }

        if(thing->isDangerous()) {
            danger++;
        }

        things_count++;
    }
    /**
     * Metoda vypisujici obsah zavazadla
     *
     * @param os
     * @return
     */
    virtual ostream & print(ostream & os) const
    {
        for(auto it = things.begin(); it != things.end(); it++) {
            if(it != things.end() && next(it) == things.end()) {
                os << "\\-" << **it << "\n";
            } else {
                os << "+-" << **it << "\n";
            }
        }

        return os;
    }
protected:
    int things_count, max_weight, actual_things_weight, danger, shoes_count;
    //map<int, CThing*> things;
    vector<CThing*> things;
};
class CSuitcase : public CLuggage
{
public:
    // CSuitcase ( w, h, d )
    CSuitcase(int w, int h, int d) : CLuggage(2000), w(w), h(h), d(d), weight(2000) {}
    // Add
    template <typename thing>
    CSuitcase & Add(thing t)
    {
        CThing * tmp = &t;
        addThing(tmp);

        return *this;
    }
    // Weight
    int Weight() const
    {
        return getWeight();
    }
    // Count
    int Count() const
    {
        return getCount();
    }
    // Danger
    bool Danger() const
    {
        return getDanger();
    }
    // IdenticalContents
    bool IdenticalContents(const CLuggage & luggage) const
    {
        return getIdenticalContents(luggage);
    }
    // operator <<
    friend ostream &operator << (ostream & os, const CSuitcase & suitcase)
    {
        os << "Suitcase " << suitcase.w << "x" << suitcase.h << "x" << suitcase.d << "\n";

        return suitcase.print(os);
    }
    CSuitcase(const CSuitcase & luggage)
    {
        if(this != &luggage) {
            w = luggage.w;
            h = luggage.h;
            d = luggage.d;

            things_count = luggage.things_count;
            max_weight = luggage.max_weight;
            actual_things_weight = luggage.actual_things_weight;
            danger = luggage.danger;
            shoes_count = luggage.shoes_count;

            if (luggage.things.size() > 0) {
                for (int i = 0; i < luggage.things_count; i++) {
                    things.push_back(luggage.things[i]->clone());
                }
            }
        }
    }
private:
    int w, h, d, weight;
};

class CBackpack : public CLuggage
{
public:
    // CBackpack ()
    CBackpack() : CLuggage(1000), weight(1000) {}
    // Add
    template <typename thing>
    CBackpack & Add(thing t)
    {
        CThing * tmp = &t;
        addThing(tmp);

        return *this;
    }
    // Weight
    int Weight() const
    {
        return getWeight();
    }
    // Count
    int Count() const
    {
        return getCount();
    }
    // Danger
    bool Danger() const
    {
        return getDanger();
    }
    // IdenticalContents
    bool IdenticalContents(const CLuggage & luggage) const
    {
        return getIdenticalContents(luggage);
    }
    // operator <<
    friend ostream &operator << (ostream & os, const CBackpack & backpack)
    {
        os << "Backpack\n";

        return backpack.print(os);
    }
    CBackpack(const CBackpack & luggage)
    {
        if(this != &luggage) {
            things_count = luggage.things_count;
            max_weight = luggage.max_weight;
            actual_things_weight = luggage.actual_things_weight;
            danger = luggage.danger;
            shoes_count = luggage.shoes_count;

            if (luggage.things.size() > 0) {
                for (int i = 0; i < luggage.things_count; i++) {
                    things.push_back(luggage.things[i]->clone());
                }
            }
        }
    }
private:
    int weight;
};

#ifndef __PROGTEST__
int main ( void )
{

    CSuitcase x ( 1, 2, 3 );
    CBackpack y;
    ostringstream os;

    x . Add ( CKnife ( 7 ) );
    x . Add ( CClothes ( "red T-shirt" ) );
    x . Add ( CClothes ( "black hat" ) );
    x . Add ( CShoes () );
    x . Add ( CClothes ( "green pants" ) );
    x . Add ( CClothes ( "blue jeans" ) );
    x . Add ( CMobile ( "Samsung", "J3" ) );
    x . Add ( CMobile ( "Tamtung", "Galaxy Note S7" ) );
    os . str ( "" );
    os << x;
    assert ( os . str () == "Suitcase 1x2x3\n"
            "+-Knife, blade: 7 cm\n"
            "+-Clothes (red T-shirt)\n"
            "+-Clothes (black hat)\n"
            "+-Shoes\n"
            "+-Clothes (green pants)\n"
            "+-Clothes (blue jeans)\n"
            "+-Mobile J3 by: Samsung\n"
            "\\-Mobile Galaxy Note S7 by: Tamtung\n" );

    assert ( x . Count () == 8 );
    assert ( x . Weight () == 5150 );
    assert ( !x . Danger () );
    x . Add ( CKnife ( 8 ) );
    os . str ( "" );
    os << x;
    assert ( os . str () == "Suitcase 1x2x3\n"
            "+-Knife, blade: 7 cm\n"
            "+-Clothes (red T-shirt)\n"
            "+-Clothes (black hat)\n"
            "+-Shoes\n"
            "+-Clothes (green pants)\n"
            "+-Clothes (blue jeans)\n"
            "+-Mobile J3 by: Samsung\n"
            "+-Mobile Galaxy Note S7 by: Tamtung\n"
            "\\-Knife, blade: 8 cm\n" );
    assert ( x . Count () == 9 );
    assert ( x . Weight () == 5250 );

    assert ( x . Danger () );
    y . Add ( CKnife ( 7 ) )
            . Add ( CClothes ( "red T-shirt" ) )
            . Add ( CShoes () );
    y . Add ( CMobile ( "Samsung", "Galaxy Note S7" ) );
    y . Add ( CShoes () );
    y . Add ( CClothes ( "blue jeans" ) );
    y . Add ( CClothes ( "black hat" ) );
    y . Add ( CClothes ( "green pants" ) );
    os . str ( "" );
    os << y;
    assert ( os . str () == "Backpack\n"
            "+-Knife, blade: 7 cm\n"
            "+-Clothes (red T-shirt)\n"
            "+-Shoes\n"
            "+-Mobile Galaxy Note S7 by: Samsung\n"
            "+-Shoes\n"
            "+-Clothes (blue jeans)\n"
            "+-Clothes (black hat)\n"
            "\\-Clothes (green pants)\n" );
    assert ( y . Count () == 8 );
    assert ( y . Weight () == 4750 );
    assert ( y . Danger () );
    y . Add ( CMobile ( "Samsung", "J3" ) );
    y . Add ( CMobile ( "Tamtung", "Galaxy Note S7" ) );
    y . Add ( CKnife ( 8 ) );
    os . str ( "" );
    os << y;
    assert ( os . str () == "Backpack\n"
            "+-Knife, blade: 7 cm\n"
            "+-Clothes (red T-shirt)\n"
            "+-Shoes\n"
            "+-Mobile Galaxy Note S7 by: Samsung\n"
            "+-Shoes\n"
            "+-Clothes (blue jeans)\n"
            "+-Clothes (black hat)\n"
            "+-Clothes (green pants)\n"
            "+-Mobile J3 by: Samsung\n"
            "+-Mobile Galaxy Note S7 by: Tamtung\n"
            "\\-Knife, blade: 8 cm\n" );
    assert ( y . Count () == 11 );
    assert ( y . Weight () == 5150 );
    assert ( y . Danger () );

    assert ( !x . IdenticalContents ( y ) );
    assert ( !y . IdenticalContents ( x ) );
    x . Add ( CMobile ( "Samsung", "Galaxy Note S7" ) );
    assert ( !x . IdenticalContents ( y ) );
    assert ( !y . IdenticalContents ( x ) );
    x . Add ( CShoes () );
    assert ( x . IdenticalContents ( y ) );
    assert ( y . IdenticalContents ( x ) );
    assert ( y . IdenticalContents ( y ) );
    assert ( x . IdenticalContents ( x ) );

    x = x;
    assert(x.IdenticalContents(x));
    CSuitcase z(x);
    CSuitcase z1(1,1,1);
    z1 = z;
    assert(z1.IdenticalContents(z));
    z = x;
    assert(x.IdenticalContents(z));

    return 0;
}
#endif /* __PROGTEST__ */