/**
 * 06 - progtest - Vysledkova listina
 */

#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
using namespace std;
#endif /* __PROGTEST__ */

class OrderingDoesNotExistException
{
};

class DuplicateMatchException
{
};

/**
 * Trida reprezentujici dany zapas
 *
 * @tparam _M
 */
template <typename _M>
class Match
{
public:
    Match(const string & contestant1, const string & contestant2, const _M & result)
            : contestant1(contestant1), contestant2(contestant2), result(result) {}
    bool operator < (const Match<_M> & a) const
    {
        if(contestant1 == a.contestant1) {
            if(contestant2 == a.contestant2) {
                return false;
            }

            return contestant2 < a.contestant2;
        }

        return contestant1 < a.contestant1;
    }
    const string &getContestant1() const {
        return contestant1;
    }
    const string &getContestant2() const {
        return contestant2;
    }
    _M getResult() const {
        return result;
    }
private:
    string contestant1, contestant2;
    _M result;
};

template <typename _M>
class CContest
{
public:
    // default constructor
    CContest() {}
    // destructor
    // AddMatch ( contestant1, contestant2, result )
    CContest & AddMatch(const string & contestant1, const string & contestant2, const _M & result)
    {
        Match<_M> match(contestant1, contestant2, result);
        Match<_M> match2(contestant2, contestant1, result);

        auto contest1 = matches.find(match);
        auto contest2 = matches.find(match2);

        if(contest1 != matches.end() || contest2 != matches.end()) {
            throw DuplicateMatchException();
        }

        if(!matches.insert(match).second) {
            throw DuplicateMatchException();
        }

        teams.insert(contestant1);
        teams.insert(contestant2);

        return *this;
    }
    // IsOrdered ( comparator )
    template <class func>
    bool IsOrdered(const func & comparator) const
    {
        if(matches.size() == 0) {
            return false;
        }

        try {
            list<string> results = Results(comparator);
        } catch (OrderingDoesNotExistException ex) {
            return false;
        }

        return true;
    }
    // Results ( comparator )
    template <class func>
    list<string> Results(const func & comparator) const
    {
        if(matches.size() == 0) {
            throw OrderingDoesNotExistException();
        }

        list<string> results;
        vector<string> init;
        init.resize(teams.size(), "|;_<");
        map<string, list<string>> graph;

        for(auto it = matches.begin(); it != matches.end(); it++) {
            int cmp_result = comparator(it->getResult());

            if (cmp_result >= 1) {
                graph[it->getContestant1()].push_back(it->getContestant2());
            } else if(cmp_result == 0) {
                graph[it->getContestant2()].push_back(it->getContestant1());
                graph[it->getContestant1()].push_back(it->getContestant2());
            } else if(cmp_result <= -1) {
                graph[it->getContestant2()].push_back(it->getContestant1());
            }
        }

        for(auto it = teams.begin(); it != teams.end(); it++) {
            int res = BFS(graph, *it) - 1;

            if ((int) teams.size() > res && init[res] == "|;_<") {
                init[res] = *it;
            } else {
                throw OrderingDoesNotExistException();
            }
        }

        for(auto it = init.begin(); it != init.end(); it++) {
            results.push_front(*it);
        }

        return results;
    }
private:
    set<Match<_M>> matches;
    set<string> teams;

    /**
     * Algoritmus BFS - kostra algoritmu pouzita ze stranky http://www.geeksforgeeks.org/breadth-first-traversal-for-a-graph/
     * (22. 4. 2017 17:21)
     * - nasledne upraveno dle potreb tohoto programu
     *
     * @param graph
     * @param match
     * @return
     */
    int BFS(map<string, list<string>> & graph, string match) const
    {
        list<string> results;
        map<string, bool> visited_matches;

        int visited = 1;
        visited_matches[match] = true;
        results.push_back(match);

        // pocatecni inicializace navstivenych zapasu v grafu
        for(auto it = teams.begin(); it != teams.end(); it++) {
            visited_matches[*it] = false;
        }

        while(!results.empty()) {
            match = results.front();
            results.pop_front();

            for(auto i = graph[match].begin(); i != graph[match].end(); ++i) {
                if (!visited_matches[*i]) {
                    visited_matches[*i] = true;
                    visited++;
                    results.push_back(*i);
                }
            }
        }

        return visited;
    }
};

#ifndef __PROGTEST__
struct CMatch
{
public:
    CMatch                        ( int               a,
                                    int               b )
            : m_A ( a ),
              m_B ( b )
    {
    }

    int                      m_A;
    int                      m_B;
};

class HigherScoreThreshold
{
public:
    HigherScoreThreshold          ( int diffAtLeast )
            : m_DiffAtLeast ( diffAtLeast )
    {
    }
    int                      operator ()                   ( const CMatch & x ) const
    {
        return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
private:
    int            m_DiffAtLeast;
};

int                HigherScore                             ( const CMatch    & x )
{
    return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

int                main                                    ( void )
{
    CContest<CMatch>  x;

    x . AddMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
            . AddMatch ( "C++", "Java", CMatch ( 8, 1 ) )
            . AddMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
            . AddMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
            . AddMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
            . AddMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );


    assert ( ! x . IsOrdered ( HigherScore ) );
    try
    {
        list<string> res = x . Results ( HigherScore );
        assert ( "Exception missing!" == NULL );
    }
    catch ( const OrderingDoesNotExistException & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == NULL );
    }

    x . AddMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) );

    assert ( x . IsOrdered ( HigherScore ) );
    try
    {
        list<string> res = x . Results ( HigherScore );
        assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
    }
    catch ( ... )
    {
        assert ( "Unexpected exception!" == NULL );
    }


    assert ( ! x . IsOrdered ( HigherScoreThreshold ( 3 ) ) );
    try
    {
        list<string> res = x . Results ( HigherScoreThreshold ( 3 ) );
        assert ( "Exception missing!" == NULL );
    }
    catch ( const OrderingDoesNotExistException & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == NULL );
    }

    assert ( x . IsOrdered ( [] ( const CMatch & x )
                             {
                                 return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );
                             } ) );
    try
    {
        list<string> res = x . Results ( [] ( const CMatch & x )
                                         {
                                             return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );
                                         } );
        assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
    }
    catch ( ... )
    {
        assert ( "Unexpected exception!" == NULL );
    }

    CContest<bool>  y;

    y . AddMatch ( "Python", "PHP", true )
            . AddMatch ( "PHP", "Perl", true )
            . AddMatch ( "Perl", "Bash", true )
            . AddMatch ( "Bash", "JavaScript", true )
            . AddMatch ( "JavaScript", "VBScript", true );

    assert ( y . IsOrdered ( [] ( bool v )
                             {
                                 return v ? 10 : - 10;
                             } ) );
    try
    {
        list<string> res = y . Results ( [] ( bool v )
                                         {
                                             return v ? 10 : - 10;
                                         });
        assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
    }
    catch ( ... )
    {
        assert ( "Unexpected exception!" == NULL );
    }

    y . AddMatch ( "PHP", "JavaScript", false );
    assert ( !y . IsOrdered ( [] ( bool v )
                              {
                                  return v ? 10 : - 10;
                              } ) );
    try
    {
        list<string> res = y . Results ( [] ( bool v )
                                         {
                                             return v ? 10 : - 10;
                                         } );
        assert ( "Exception missing!" == NULL );
    }
    catch ( const OrderingDoesNotExistException & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == NULL );
    }

    try
    {
        y . AddMatch ( "PHP", "JavaScript", false );
        assert ( "Exception missing!" == NULL );
    }
    catch ( const DuplicateMatchException & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == NULL );
    }

    try
    {
        y . AddMatch ( "JavaScript", "PHP", true );
        assert ( "Exception missing!" == NULL );
    }
    catch ( const DuplicateMatchException & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == NULL );
    }

    return 0;
}
#endif /* __PROGTEST__ */