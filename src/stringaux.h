#ifndef STRINGAUX_H_INCLUDED
#define STRINGAUX_H_INCLUDED

#include <string>
#include <sstream>
#include <vector>

//! Converts a value of any type to a string.
template <typename T>
std::string ToString( const T & value )
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

//! Reads a value of any type from a string.
template <typename T>
T FromString( const std::string & theString )
{
	std::stringstream ss( theString );
	T value;
	ss >> value;
	return value;
}

//! Returns true if a start is a prefix of str.
bool StartsWith( std::string str, std::string start );

//! Returns true if a start is a postfix of str.
bool EndsWith( std::string str, std::string end );

namespace std
{
	//! A list of strings
	typedef vector<string> stringlist;
}

//! Splits a string into a list of string, breaking wherever a character in
//! separators occurs.
std::stringlist split( const std::string& text, const std::string& separators );

#endif // STRINGAUX_H_INCLUDED
