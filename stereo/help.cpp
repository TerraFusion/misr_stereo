#include "help.h"
#include "glutaux.h"
#include <time.h>

help::help()
:
messageShowTime( double( time( 0 ) ) ),
menuVisible( false )
{

}

void help::Draw()
{
	double messageTime = double( time( 0 ) ) - messageShowTime;

	if( menuVisible )
	{
		DrawMenu();
	}
	else if( messageTime < 8.0 )
	{
		DrawMessage();
	}
}

void help::DrawMenu()
{
	glPushMatrix();
	glTranslated( 1, 1, 0 );
	glColor3d( 0, 0, 0 );
	DrawMenuLayer();
	glPopMatrix();

	glColor3d( 1, 1, 0 );
	DrawMenuLayer();
}

void help::DrawMenuLayer()
{
	const double dy = 16.0;
	double y = -dy;
	glutDrawText( vec2d( 0, y += dy ), "Hold left mouse and drag to move" );
	glutDrawText( vec2d( 0, y += dy ), "Hold right mouse and drag to zoom" );
	glutDrawText( vec2d( 0, y += dy ), "Type a block number to go there" );
	glutDrawText( vec2d( 0, y += dy ), "    H - Toggle help display" );
	glutDrawText( vec2d( 0, y += dy ), "    S - Swap eyes ( if depth is inverted )" );
	glutDrawText( vec2d( 0, y += dy ), "   Up - Increase Brightness ( mouse wheel or keyboard )" );
	glutDrawText( vec2d( 0, y += dy ), " Down - Decrease Brightness ( mouse wheel or keyboard )" );
	glutDrawText( vec2d( 0, y += dy ), "    Q - Quit the program" );
}

void help::DrawMessage()
{
	glPushMatrix();
	glTranslated( 1, 1, 0 );
	glColor3d( 0, 0, 0 );
	DrawMessageLayer();
	glPopMatrix();

	glColor3d( 1, 1, 0 );
	DrawMessageLayer();
}

void help::DrawMessageLayer()
{
	glutDrawText( vec2d( 0, 0 ), "Press 'H' for help..." );
}

void help::Toggle()
{
	messageShowTime = 0;
	menuVisible = !menuVisible;
}
