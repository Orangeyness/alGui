#include "Control.h"

using namespace alGui;

Panel::Panel(double width, double height)
	: Control(width, height) { }

Panel::Panel(double x, double y, double width, double height)
	: Control(x, y, width, height) { }

Panel::Panel(std::string title, double width, double height)
	: Control(width, height), m_Title(title) { }

Panel::Panel(std::string title, double x, double y, double width, double height)
	: Control(x, y, width, height), m_Title(title) { }

void Panel::draw(double x, double y)
{
	double drawX = m_X + x;
	double drawY = m_Y + y;

	al_draw_filled_rectangle(drawX, drawY, drawX + m_Width, drawY + m_Height, Theme::CurrentTheme->colourBackground);
	Theme::CurrentTheme->drawRectangleBorder(drawX, drawY, drawX + m_Width, drawY + m_Height);

	al_draw_text(	Theme::CurrentTheme->fontTitle, 
					Theme::CurrentTheme->colourFont,
					drawX + 5, drawY + 5, ALLEGRO_ALIGN_LEFT, 
					m_Title.c_str()
				);

	Control::draw(x, y);
}
