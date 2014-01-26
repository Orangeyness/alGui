#include "Control.h"

using namespace alGui;

Button::Button(std::string label)
	: Control(0, 0), m_Label(label) 
{
	m_Width = al_get_text_width(Theme::CurrentTheme->fontNormal, label.c_str())
			+ 2 * Theme::CurrentTheme->borderPaddingHorizontal;

	m_Height = al_get_font_line_height(Theme::CurrentTheme->fontNormal)
			+ 2 * Theme::CurrentTheme->borderPaddingVertical;
}

Button::Button(std::string label, double x, double y)
	: Control(0, 0, x, y), m_Label(label) 
{
	m_Width = al_get_text_width(Theme::CurrentTheme->fontNormal, label.c_str())
			+ 2 * Theme::CurrentTheme->borderPaddingHorizontal;

	m_Height = al_get_font_line_height(Theme::CurrentTheme->fontNormal)
			+ 2 * Theme::CurrentTheme->borderPaddingVertical;
}


void Button::draw(double x, double y)
{
	double drawX = m_X + x;
	double drawY = m_Y + y;

	ALLEGRO_COLOR fontColor = m_MouseOver ? Theme::CurrentTheme->colourHighlight :
											Theme::CurrentTheme->colourFont;

	al_draw_filled_rectangle(drawX, drawY, drawX + m_Width, drawY + m_Height, Theme::CurrentTheme->colourBackground);

	if (!m_MouseDown)
	{	
		Theme::CurrentTheme->drawHorizontalBorder(drawX, drawX + m_Width, drawY, true);
		Theme::CurrentTheme->drawHorizontalBorder(drawX, drawX + m_Width, drawY + m_Height, false);
		Theme::CurrentTheme->drawVerticalBorder(drawX, drawY, drawY + m_Height, true);
		Theme::CurrentTheme->drawVerticalBorder(drawX + m_Width, drawY, drawY + m_Height, false);
	}
	else
	{
		Theme::CurrentTheme->drawHorizontalBorder(drawX, drawX + m_Width, drawY, false);
		Theme::CurrentTheme->drawHorizontalBorder(drawX, drawX + m_Width, drawY + m_Height, true);
		Theme::CurrentTheme->drawVerticalBorder(drawX, drawY, drawY + m_Height, false);
		Theme::CurrentTheme->drawVerticalBorder(drawX + m_Width, drawY, drawY + m_Height, true);
	}

	al_draw_text(	Theme::CurrentTheme->fontNormal, 
					fontColor,
					drawX + Theme::CurrentTheme->borderPaddingHorizontal, 
					drawY + Theme::CurrentTheme->borderPaddingVertical, 
					ALLEGRO_ALIGN_LEFT, 
					m_Label.c_str()
				);

	Control::draw(x, y);
}
