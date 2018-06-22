#include "csscName.h"

int csscName::csscOlsen[ 1581 ];
QString csscName::olsenName[ 74 ];

class forceInit { public: forceInit() { csscName::Init(); } };
forceInit now;

csscName::csscName( QWidget *parent )
:
QLabel( parent )
{
	QPalette pal( palette() );
	pal.setColor( QPalette::Foreground, Qt::red );
    setPalette( pal );

    CsscUpdate( 0 );
}

csscName::~csscName()
{

}

void csscName::Init()
{
	SetOlsenClass(    0,    0,  0 );
	SetOlsenClass(    1,    1,  2 );
	SetOlsenClass(    2,    5,  6 );
	SetOlsenClass(    6,   48,  8 );
	SetOlsenClass(   49,   53, 17 );
	SetOlsenClass(   54,   56, 20 );
	SetOlsenClass(   57,   74, 21 );
	SetOlsenClass(   75,  112, 22 );
	SetOlsenClass(  113,  147, 23 );
	SetOlsenClass(  148,  187, 24 );
	SetOlsenClass(  188,  205, 25 );
	SetOlsenClass(  206,  227, 26 );
	SetOlsenClass(  228,  239, 27 );
	SetOlsenClass(  240,  260, 28 );
	SetOlsenClass(  261,  299, 29 );
	SetOlsenClass(  300,  334, 30 );
	SetOlsenClass(  335,  450, 31 );
	SetOlsenClass(  451,  492, 32 );
	SetOlsenClass(  493,  514, 33 );
	SetOlsenClass(  515,  538, 36 );
	SetOlsenClass(  539,  571, 37 );
	SetOlsenClass(  572,  579, 38 );
	SetOlsenClass(  580,  585, 39 );
	SetOlsenClass(  586,  646, 40 );
	SetOlsenClass(  647,  788, 41 );
	SetOlsenClass(  789,  802, 42 );
	SetOlsenClass(  803,  846, 43 );
	SetOlsenClass(  847,  874, 44 );
	SetOlsenClass(  875,  902, 45 );
	SetOlsenClass(  903,  927, 46 );
	SetOlsenClass(  928,  952, 47 );
	SetOlsenClass(  953,  963, 48 );
	SetOlsenClass(  964,  965, 49 );
	SetOlsenClass(  966, 1007, 50 );
	SetOlsenClass( 1008, 1063, 51 );
	SetOlsenClass( 1064, 1080, 52 );
	SetOlsenClass( 1081, 1169, 53 );
	SetOlsenClass( 1170, 1170, 54 );
	SetOlsenClass( 1171, 1197, 55 );
	SetOlsenClass( 1198, 1251, 56 );
	SetOlsenClass( 1252, 1294, 57 );
	SetOlsenClass( 1295, 1358, 58 );
	SetOlsenClass( 1359, 1391, 59 );
	SetOlsenClass( 1392, 1398, 60 );
	SetOlsenClass( 1399, 1408, 61 );
	SetOlsenClass( 1409, 1436, 62 );
	SetOlsenClass( 1437, 1475, 63 );
	SetOlsenClass( 1476, 1483, 64 );
	SetOlsenClass( 1484, 1494, 65 );
	SetOlsenClass( 1495, 1501, 66 );
	SetOlsenClass( 1502, 1506, 67 );
	SetOlsenClass( 1507, 1514, 68 );
	SetOlsenClass( 1515, 1545, 69 );
	SetOlsenClass( 1546, 1569, 70 );
	SetOlsenClass( 1570, 1577, 71 );
	SetOlsenClass( 1578, 1579, 72 );
	SetOlsenClass( 1580, 1580, 73 );

    olsenName[  0 ] = "Water";
    olsenName[  1 ] = "City";
    olsenName[  2 ] = "Spare Grass/Shrub";
    olsenName[  3 ] = "";
    olsenName[  4 ] = "";
    olsenName[  5 ] = "";
    olsenName[  6 ] = "African Broadleaf Evergreen";
    olsenName[  7 ] = "";
    olsenName[  8 ] = "Desert";
    olsenName[  9 ] = "";
    olsenName[ 10 ] = "";
    olsenName[ 11 ] = "";
    olsenName[ 12 ] = "";
    olsenName[ 13 ] = "";
    olsenName[ 14 ] = "";
    olsenName[ 15 ] = "";
    olsenName[ 16 ] = "Broadleaf Evergreen Scrub";
    olsenName[ 17 ] = "Antarctic Ice Cap";
    olsenName[ 18 ] = "";
    olsenName[ 19 ] = "";
    olsenName[ 20 ] = "Snowy, Rainy Coastal Conifer";
    olsenName[ 21 ] = "Main Boreal Conifer Forest";
    olsenName[ 22 ] = "Snowy non-Boreal Conifer Forest";
    olsenName[ 23 ] = "Conifer/Deciduous, Persistent Snow";
    olsenName[ 24 ] = "Temperate Broadleaf/Conifer";
    olsenName[ 25 ] = "Snowy Deciduous Forest";
    olsenName[ 26 ] = "Temperate Broad Leaf Forest";
    olsenName[ 27 ] = "Non-Snowy Conifer Forest";
    olsenName[ 28 ] = "Tropical Montane";
    olsenName[ 29 ] = "Tropical Broadleaf Seasonal";
    olsenName[ 30 ] = "Cool Farmland/Settlement";
    olsenName[ 31 ] = "Mild/Hot Farmland/Settlement";
    olsenName[ 32 ] = "Rain-Green/Seasonal Dry Evergreen Forest";
    olsenName[ 33 ] = "Tropical Rainforest";
    olsenName[ 34 ] = "";
    olsenName[ 35 ] = "";
    olsenName[ 36 ] = "Paddy Rice";
    olsenName[ 37 ] = "Warm/Hot Cropland";
    olsenName[ 38 ] = "Cool Cropland";
    olsenName[ 39 ] = "Cold Cropland";
    olsenName[ 40 ] = "Cool Grass/Shrub";
    olsenName[ 41 ] = "Mild/Warm/Hot Grass/Shrub";
    olsenName[ 42 ] = "Cold Steppe/Meadow";
    olsenName[ 43 ] = "Savanna/Grass, Seasonal Woods";
    olsenName[ 44 ] = "Mire";
    olsenName[ 45 ] = "Marsh";
    olsenName[ 46 ] = "Mediterranean Evergreen";
    olsenName[ 47 ] = "Dry/Highland Scrub, Open Woodland";
    olsenName[ 48 ] = "Dry Evergreen Woodland, Low Forest";
    olsenName[ 49 ] = "Hot/Mild Volcanic Island";
    olsenName[ 50 ] = "Sand Desert";
    olsenName[ 51 ] = "Semi-Desert/Desert Scrub/Succulent/Sparse Grass";
    olsenName[ 52 ] = "Cool/Cold Shrub Semidesert/Steppe";
    olsenName[ 53 ] = "Polar/Alpine Tundra";
    olsenName[ 54 ] = "Temperate Evergreen Forest";
    olsenName[ 55 ] = "Snowy Fields/Woods";
    olsenName[ 56 ] = "Forest/Field with Regrowth";
    olsenName[ 57 ] = "Snowy Forest/Field";
    olsenName[ 58 ] = "Field/Woods with Grass/Cropland";
    olsenName[ 59 ] = "Succulent/Thorn Woods";
    olsenName[ 60 ] = "Southern Dry Taiga";
    olsenName[ 61 ] = "Larch Taiga with Deciduous Conifer";
    olsenName[ 62 ] = "Northern/Maritime Taiga";
    olsenName[ 63 ] = "Wooded Tundra Margin, Mountain Scrub/Meadow";
    olsenName[ 64 ] = "Heath/Moorland";
    olsenName[ 65 ] = "Coastal, NorthWest Quadrant";
    olsenName[ 66 ] = "Coastal, NorthEast Quadrant";
    olsenName[ 67 ] = "Coastal, SouthEast Quadrant";
    olsenName[ 68 ] = "Coastal, SouthWest Quadrant";
    olsenName[ 69 ] = "Polar Desert with Rock Lichens";
    olsenName[ 70 ] = "Glaciers";
    olsenName[ 71 ] = "Salt/Soda Flats, Desert Playas";
    olsenName[ 72 ] = "Mangrove/Non-Saline Swamps, Tidal Mudflats";
    olsenName[ 73 ] = "Island/Shore Water";
}



// CsscToOlsen
// + maps a cssc index to an olsen index
int csscName::CsscToOlsen( int csscIndex )
{
    return csscOlsen[ csscIndex ];
}



// OlsenToName
// + maps the olsen index to its name
QString csscName::OlsenToName( int olsenIndex )
{
    return olsenName[ olsenIndex ];
}



// CsscUpdate( csscIndex )
// + call whenever the viewed cssc changes
void csscName::CsscUpdate( int newCsscIndex )
{
    if ( ( newCsscIndex >= 0 ) && ( newCsscIndex <= 1580 ) )
    {
        setText( "Olsen Class: " + OlsenToName( CsscToOlsen( newCsscIndex ) ) );
    }
}

void csscName::SetOlsenClass( int first, int last, int olsen )
{
	std::fill( csscOlsen + first, csscOlsen + last + 1, olsen );
}



