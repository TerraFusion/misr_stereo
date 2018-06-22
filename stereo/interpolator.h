#ifndef INTERPOLATOR_H_INCLUDED
#define INTERPOLATOR_H_INCLUDED

#include <map>

template<typename key,typename value>
    class interpolator : public std::map<key,value>
{
public:

    using std::map<key,value>::begin;
    using std::map<key,value>::end;
    using std::map<key,value>::rbegin;
    using std::map<key,value>::rend;
    using std::map<key,value>::upper_bound;
    
    key KeyMin() const { return begin()->first; }

    value ValueMin() const { return begin()->second; }

    key KeyMax() const { return rbegin()->first; }

    value ValueMax() const { return rbegin()->second; }

    key KeyCenter() const { return 0.5 * ( KeyMin() + KeyMax() ); }

    value operator () ( key t ) const
    {
        // get upper bound of interpolation interval
        typename std::map<key,value>::const_iterator next = upper_bound( t );

        // get lower bound
        typename std::map<key,value>::const_iterator prev = next;
        prev--;

        // check if key is after last map point
        if ( next == end() ) return prev->second;

        // check if key is before first map point
        if ( next == begin() ) return next->second;

        // calculate distances
        double a = t - prev->first;
        double b = next->first - t;

        // check for divide by 0 conditions
        if ( a <= 0 ) return prev->second;
        if ( b <= 0 ) return next->second;

        // calculate interpolated value
        return ( b / ( a + b ) ) * prev->second + ( a / ( a + b ) ) * next->second;
    }
};

#endif // INTERPOLATOR_H_INCLUDED
