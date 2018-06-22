#ifndef HELP_H_INCLUDED
#define HELP_H_INCLUDED

class help
{
public:

	help();

	//! draws a message to press h for help, or the actual help menu.
	void Draw();

	//! toggles display of the help menu.
	void Toggle();

protected:

	void DrawMenu();
	void DrawMenuLayer();

	void DrawMessage();
	void DrawMessageLayer();

	double messageShowTime;
	bool menuVisible;
};

#endif // HELP_H_INCLUDED
