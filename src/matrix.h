#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <vector>

//! A matrix.
template <typename dataType>
    class matrix
{
public:

    typedef typename std::vector<dataType>::iterator iterator;
    typedef typename std::vector<dataType>::const_iterator const_iterator;

    matrix();
    matrix( int newRows, int newCols, const dataType & value = dataType() );

    ~matrix();

    void resize( int newRows, int newCols, const dataType & value = dataType() );
    void resizeRows( int newRows, const dataType & value );
    void resizeCols( int newCols, const dataType & value );

    int numRows() const;
    int numCols() const;

    iterator begin();
    iterator end();
    iterator at( int r, int c ); 

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator at( int r, int c ) const;

    dataType & operator () ( int r, int c );
    const dataType & operator () ( int r, int c ) const;

    const matrix & operator = ( const dataType & value );
    const matrix & operator += ( const matrix & other );

    std::vector<dataType> row( int rowIndex );
    std::vector<dataType> col( int colIndex );

    matrix Transpose() const;

    void simpleDisplay();

protected:

    int rows;
    int cols;

    std::vector<dataType> data;
};


#include "matrix.cpp"

#endif // MATRIX_H_INCLUDED
