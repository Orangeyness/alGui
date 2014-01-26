#include "Control.h"

using namespace alGui;

Theme* Theme::CurrentTheme = NULL;
std::string Theme::FontDirectory = "assets/";

void Theme::SetCurrentTheme(Theme* theme)
{
	Theme::CurrentTheme = theme;
}

void Theme::SetFontDirectory(std::string fontDirectory)
{
	Theme::FontDirectory = fontDirectory;
}

Theme::Theme()
{
	colourFont = al_map_rgb(63, 57, 52);
	colourBackground = al_map_rgb(218, 218, 218);
	colourHighlight = al_map_rgb(78, 166, 234);
	colourBorder = al_map_rgba(30, 30, 30, 150);
	
	borderWidth = 4;
	borderPaddingVertical = 4;
	borderPaddingHorizontal = 10;

	fontTitle = al_load_font((FontDirectory + "segoeuil.ttf").c_str(), 20, 0);
	fontNormal = al_load_font((FontDirectory + "segoeuil.ttf").c_str(), 14, 0);
}

Theme::~Theme()
{
	al_destroy_font(fontTitle);
	al_destroy_font(fontNormal);
}

void Theme::drawRectangleBorder(double x1, double y1, double x2, double y2)
{
	drawHorizontalBorder(x1, x2, y1, true);
	drawHorizontalBorder(x1, x2, y2, false);
	drawVerticalBorder(x1, y1, y2, true);
	drawVerticalBorder(x2, y1, y2, false);
}

void Theme::drawVerticalBorder(double x, double y1, double y2, bool isLeftBorder)
{
	unsigned char r, g, b, a;

	al_draw_line(x, y1 - 1, x, y2 + 1, colourHighlight, 1);
	al_unmap_rgba(colourBorder, &r, &b, &g, &a);

	int xDir = (isLeftBorder ? -1 : 1);
	int aDelta = a / (borderWidth + 1);

	for (int i = 1; i < borderWidth;  i++)
	{
		al_draw_line(x + i * xDir, y1 - 1, x + i * xDir, y2 + 1, al_map_rgba(r, g, b, a - aDelta * i), 1);
	}
}

void Theme::drawHorizontalBorder(double x1, double x2, double y, bool isTopBorder)
{
	unsigned char r, g, b, a;

	al_draw_line(x1 - 1, y, x2 + 1, y, colourHighlight, 1);
	al_unmap_rgba(colourBorder, &r, &b, &g, &a);

	int yDir = (isTopBorder ? -1 : 1);
	int aDelta = a / (borderWidth + 1);

	for (int i = 1; i < borderWidth;  i++)
	{
		al_draw_line(x1 - 1, y + i * yDir, x2 + 1, y + i * yDir, al_map_rgba(r, g, b, a - aDelta * i), 1); 
	}
}
