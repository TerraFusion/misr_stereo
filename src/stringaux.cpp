#include "stringaux.h"

bool StartsWith( std::string str, std::string start )
{
	return str.substr( 0, start.length() ) == start;
}

bool EndsWith( std::string str, std::string end )
{
	return str.substr( str.length() - end.length(), str.length() ) == end;
}

std::stringlist split( const std::string& text, const std::string& separators )
{
	std::vector<std::string> output;

    size_t first = text.find_first_not_of( separators );

    while( first < text.length() )
	{
        size_t last = text.find_first_of( separators, first );
        if ( last > text.length() ) last = text.length();
        output.push_back( text.substr( first, last - first ) );
        first = text.find_first_not_of( separators, last + 1 );
    }

	return output;
}
