#ifdef MATRIX_H_INCLUDED

#include <algorithm>
#include <iostream>
#include "matrix.h"

template<typename dataType>
    matrix<dataType>::matrix()
:
rows( 0 ),
cols( 0 ),
data()
{

}



template<typename dataType>
    matrix<dataType>::matrix( int newRows, int newCols, const dataType & value )
:
rows( newRows ),
cols( newCols ),
data( numRows() * numCols(), value )
{

}



template<typename dataType>
    matrix<dataType>::~matrix()
{

}



template<typename dataType>
    void matrix<dataType>::resize( int newRows, int newCols, const dataType & value )
{
    if ( newCols < cols )
    {
        iterator source = data.begin();
        iterator dest = data.begin();

        // for each row, copy overlap region and skip difference
        while ( source != data.end() )
        {
            std::copy( source, source + newCols, dest );
            dest += newCols;
            source += cols;
        }

        std::fill( dest, source, value );
        data.resize( newRows * newCols, value);
    }
    else if ( newCols > cols )
    {
        data.resize( newRows * newCols, value );
        if ( newRows > rows ) std::fill( data.end() - ( newRows - rows ) * newCols, data.end(), value );

        iterator source = data.begin() + std::min( rows, newRows ) * cols;
        iterator dest = data.begin() + std::min( rows, newRows ) * newCols;

        // for each row, copy overlap region and fill difference
        while(source != data.begin())
        {
            std::fill( dest - ( newCols - cols ), dest, value );
            dest -= newCols - cols;
            std::copy_backward( source - cols, source, dest );
            source -= cols;
            dest -= cols;
        }
    }
    else
    {
        data.resize( newRows * newCols, value );
    }

    rows = newRows;
    cols = newCols;
}



template<typename dataType>
    int matrix<dataType>::numRows() const
{
    return rows;
}



template<typename dataType>
    int matrix<dataType>::numCols() const
{
    return cols;
}



template<typename dataType>
    typename matrix<dataType>::iterator matrix<dataType>::begin()
{
    return data.begin();
}



template<typename dataType>
    typename matrix<dataType>::iterator matrix<dataType>::end()
{
    return data.end();
}



template<typename dataType>
    typename matrix<dataType>::iterator matrix<dataType>::at( int r, int c )
{
    return data.begin() + r * numCols() + c;
}



template<typename dataType>
    typename matrix<dataType>::const_iterator matrix<dataType>::begin() const
{
    return data.begin();
}



template<typename dataType>
    typename matrix<dataType>::const_iterator matrix<dataType>::end() const
{
    return data.end();
}



template<typename dataType>
    typename matrix<dataType>::const_iterator matrix<dataType>::at( int r, int c ) const
{
    return data.begin() + r * numCols() + c;
}



template<typename dataType>
    dataType & matrix<dataType>::operator () ( int r, int c )
{
    return data[ r * numCols() + c ];
}



template<typename dataType>
    const dataType & matrix<dataType>::operator () ( int r, int c ) const
{
    return data[ r * numCols() + c ];
}



template <typename dataType>
    const matrix<dataType> & matrix<dataType>::operator = ( const dataType & value )
{
    for ( iterator current = begin() ; current != end() ; ++current )
    {
        *current = value;
    }

    return *this;
}



template<typename dataType>
    const matrix<dataType> & matrix<dataType>::operator += ( const matrix<dataType> & other )
{
    if ( ( numRows() == other.numRows() ) && ( numCols() == other.numCols() ) )
    {
        const_iterator source = other.begin();
        iterator destination = begin();

        while ( destination != end() )
        {
            *destination++ += *source++;
        }
    }

    return *this;
}



template<typename dataType>
    std::vector<dataType> matrix<dataType>::row( int rowIndex )
{
    std::vector<dataType> result;

    if ( rowIndex >= 0 && rowIndex < numRows() )
    {
        for ( int colIndex = 0 ; colIndex < numCols() ; colIndex++ )
        {
            result.push_back( (*this)( rowIndex, colIndex ) );
        }
    }

    return result;
}



template<typename dataType>
    std::vector<dataType> matrix<dataType>::col( int colIndex )
{
    std::vector<dataType> result;

    if ( colIndex >= 0 && colIndex < numCols() )
    {
        for ( int rowIndex = 0 ; rowIndex < numRows() ; rowIndex++ )
        {
            result.push_back( (*this)( rowIndex, colIndex ) );
        }
    }

    return result;
}



template<typename dataType>
    matrix<dataType> matrix<dataType>::Transpose() const
{
    matrix result( numCols(), numRows() );

    for ( int y = 0 ; y < numRows() ; ++y )
    {
        for ( int x = 0 ; x < numCols() ; ++x )
        {
            result( x, y ) = (*this)( y, x );
        }
    }

    return result;
}



template<typename dataType>
    void matrix<dataType>::simpleDisplay()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j =0; j < cols; j++)
        {
            std::cout<< data[i * numCols() + j]<< " ";
        }
        std::cout<<std::endl;
    }   
}

#endif // MATRIX_H_INCLUDED
