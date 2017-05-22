/**
 * 03 - progtest - Polynomy
 *
 * https://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B
 */

#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <complex>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

string p_variable = "x";

ios_base & dummy_polynomial_manipulator ( ios_base & x )
{
    return x;
}

ios_base & ( * ( polynomial_variable ( const string & varName ) ) ) ( ios_base & x )
{
    p_variable = varName;
    return dummy_polynomial_manipulator;
}

class CPolynomial
{
public:
    // default constructor
    CPolynomial() : degree(0) {};
    CPolynomial(vector<double> v) : degree(0)
    {
        values = v;
    };
    // operator +
    CPolynomial operator + (const CPolynomial & polynomial) const
    {
        int degree = (polynomial.Degree() > this->Degree()) ? polynomial.Degree() : this->Degree();
        vector<double> v;

        if(values.size() > 0 || polynomial.values.size() > 0) {
            for(int i = 0; i <= degree; i++) {
                double a, b;

                if(i >= (int) values.size()) {
                    a = 0;
                } else {
                    a = values[i];
                }

                if(i >= (int) polynomial.values.size()) {
                    b = 0;
                } else {
                    b = polynomial.values[i];
                }

                v.push_back(a + b);
            }
        }

        return CPolynomial(v);
    }
    // operator -
    CPolynomial operator - (const CPolynomial & polynomial) const
    {
        int degree = (polynomial.Degree() > this->Degree()) ? polynomial.Degree() : this->Degree();
        vector<double> v;

        if(values.size() > 0 || polynomial.values.size() > 0) {
            for(int i = 0; i <= degree; i++) {
                double a, b;

                if(i >= (int) values.size()) {
                    a = 0;
                } else {
                    a = values[i];
                }

                if(i >= (int) polynomial.values.size()) {
                    b = 0;
                } else {
                    b = polynomial.values[i];
                }

                v.push_back(a - b);
            }
        }

        return CPolynomial(v);
    }
    // operator * (polynomial, double)
    CPolynomial operator * (double value) const
    {
        vector<double> v = values;

        for(int i = 0; i < (int) values.size(); i++) {
            v[i] *= value;
        }

        return CPolynomial(v);
    }
    CPolynomial operator * (const CPolynomial & polynomial) const
    {
        /*
        int initialization = this->Degree() + polynom.Degree();
        vector<double> v (initialization, 0);
        v = FFTMultiplication();
        values = v;
        */

        int a = this->Degree(), b = polynomial.Degree(), initialization = a + b + 1;
        vector<double> v (initialization, 0);

        for(int i = 0; i < (int) values.size(); i++) {
            for(int j = 0; j < (int) polynomial.values.size(); j++) {
                v[i + j] += (values[i] * polynomial.values[j]);
            }
        }

        return CPolynomial(v);
    }
    // operator ==
    bool operator == (const CPolynomial & polynom) const
    {
        if(polynom.Degree() != this->Degree()) {
            return false;
        }

        for(int i = 0; i < (int) values.size(); i++) {
            if(values[i] != polynom.values[i]) {
                return false;
            }
        }

        return true;
    }
    // operator !=
    bool operator != (const CPolynomial & polynom) const
    {
        return !(*this == polynom);
    }
    // operator []
    double & operator [] (int key)
    {
        // pokud uz jsem iteroval, vracim pouze odkaz na prvek pole
        if((int) values.size() > key) {
            return *(values.begin() + key);
        }

        // pokud jsem neinicializoval hodnoty, nejdrive je nainicializuji
        for(int i = (int) values.size(); i <= key; i++) {
            values.push_back(0);
        }

        return *(values.end() - 1);
    }
    const double & operator [] (unsigned int key) const
    {
        return values[key];
    }
    // operator ()
    double operator () (double value) const
    {
        double result = 0;

        for(int i = 0; i < (int) values.size(); i++) {
            double r = values[i] * pow(value, i + 0.0);
            result += r;
        }

        return result;
    }
    CPolynomial & operator = (CPolynomial polynomial)
    {
        if(this == &polynomial) {
            return *this;
        }

        swap(values, polynomial.values);

        return *this;
    }
    // operator <<
    friend ostream & operator << (ostream & stream, const CPolynomial & polynomial)
    {
        if(polynomial.values.size() > 0) {
            string result = "", multiple = "";
            bool first = true;
            int degree = polynomial.Degree();

            for (int i = degree; i >= 0; i--) {
                multiple = "";

                if (polynomial.values[i] != 0) {
                    if (first) {
                        if (polynomial.values[i] < 0) {
                            stream << "- ";
                        }

                        first = false;
                    } else {
                        stream << " ";

                        if (polynomial.values[i] > 0) {
                            stream << "+ ";
                        } else {
                            stream << "- ";
                        }
                    }

                    if (fabs(polynomial.values[i]) != 1) {
                        multiple = "*";
                        stream << fabs(polynomial.values[i]);
                    }

                    if (i != 0) {
                        stream << multiple << p_variable;
                    }

                    if (i > 0) {
                        stream << "^" << i;
                    }
                }

                if(degree == 0 && polynomial.values[i] == 0) {
                    stream << "0";
                }
            }
        } else {
            stream << "0";
        }

        return stream;
    }
    // Degree (), returns unsigned value
    unsigned int Degree() const
    {
        int degree = (int) values.size() - 1;

        for(int i = (int) values.size() - 1; i > 0; i--) {
            if(values[i] == 0) {
                degree--;
            } else {
                break;
            }
        }

        return (unsigned int) degree;
    }

    const vector<double> & getValues() const {
        return values;
    }
private:
    vector<double> values;
    int degree;

    /**
     * Algoritmus nasobeni polynomu - inspirace na webu: http://www.cs.cmu.edu/afs/cs/academic/class/15451-s10/www/lectures/lect0423.txt (26. 3. 2017, 17:55)
     * http://stackoverflow.com/questions/8801158/fft-in-a-single-c-file (27. 3. 2017, 17:27)
     *
     * @return
     */
    /*
    vector<double> & FFTMultiplication()
    {
        vector<double> v;

        FFT(A, m, w)
        {
          if (m==1) return vector (a_0)
          else {
            A_even = (a_0, a_2, ..., a_{m-2})
            A_odd  = (a_1, a_3, ..., a_{m-1})
            F_even = FFT(A_even, m/2, w^2)    //w^2 is a primitive m/2-th root of unity
            F_odd = FFT(A_odd, m/2, w^2)
            F = new vector of length m
            x = 1
            for (j=0; j < m/2; ++j) {
              F[j] = F_even[j] + x*F_odd[j]
              F[j+m/2] = F_even[j] - x*F_odd[j]
              x = x * w
          }
          return F
        }

        return v;
    }
     */
};

#ifndef __PROGTEST__
bool               smallDiff                               ( double            a,
                                                             double            b )
{
    return a == b;
}

bool               dumpMatch                               ( const CPolynomial & x,
                                                             const vector<double> & ref )
{
    for(int i = 0; i < (int) ref.size(); i++) {
        if(ref[i] != x.getValues()[i]) {
            return false;
        }
    }

    return true;
}

int                main                                    ( void )
{
    CPolynomial a, b, c;
    ostringstream out;

    a[0] = -10;
    a[1] = 3.5;
    a[3] = 1;
    assert ( smallDiff ( a ( 2 ), 5 ) );
    out . str ("");
    out << a;
    assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
    a = a * -2;
    assert ( a . Degree () == 3
             && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

    out . str ("");
    out << a;
    assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
    out . str ("");
    out << b;
    assert ( out . str () == "0" );
    b[5] = -1;
    out . str ("");
    out << b;
    assert ( out . str () == "- x^5" );
    c = a + b;
    assert ( c . Degree () == 5
             && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );

    out . str ("");
    out << c;
    assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
    c = a - b;
    assert ( c . Degree () == 5
             && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

    out . str ("");
    out << c;
    assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );
    c = a * b;
    assert ( c . Degree () == 8
             && dumpMatch ( c, vector<double>{ 0.0, -0.0, 0.0, -0.0, -0.0, -20.0, 7.0, 0.0, 2.0 } ) );

    out . str ("");
    out << c;
    assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );
    assert ( a != b );
    b[5] = 0;
    assert ( !(a == b) );
    a = a * 0;
    assert ( a . Degree () == 0
             && dumpMatch ( a, vector<double>{ 0.0 } ) );

    assert ( a == b );

    // bonus
    a[2] = 4;
    a[1] = -3;
    b[3] = 7;
    out . str ("");
    out << polynomial_variable ( "y" ) << "a=" << a << ", b=" << b;
    assert ( out . str () == "a=4*y^2 - 3*y^1, b=7*y^3" );

    out . str ("");
    out << polynomial_variable ( "test" ) << c;
    assert ( out . str () == "2*test^8 + 7*test^6 - 20*test^5" );

    return 0;
}
#endif /* __PROGTEST__ */
