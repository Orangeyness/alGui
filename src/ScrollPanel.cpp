#include "Control.h"

using namespace alGui;

ScrollPanel::ScrollPanel(double width, double displayHeight, double scrollHeight)
	: 	Control(width, displayHeight), 
		m_UpButton("^"),
		m_DownButton("v")
{
	m_PaneHeight = scrollHeight;

	m_PaneWidth = m_Width
				- 2 * Theme::CurrentTheme->borderPaddingHorizontal
				+ al_get_text_width(Theme::CurrentTheme->fontNormal, "v");

	init();
}

ScrollPanel::ScrollPanel(double x, double y, double width, double displayHeight, double scrollHeight)
	: 	Control(x, y, width, displayHeight),
		m_UpButton("^"),
		m_DownButton("v")
{
	m_PaneHeight = scrollHeight;

	m_PaneWidth = m_Width
				- 2 * Theme::CurrentTheme->borderPaddingHorizontal
				+ al_get_text_width(Theme::CurrentTheme->fontNormal, "v");

	init();
}

ScrollPanel::~ScrollPanel()
{
	al_destroy_bitmap(m_PaneBuffer);
}

void ScrollPanel::init()
{
	Control::attachControl(&m_UpButton, PLACE_RELATIVE_TOPRIGHT, 0, 0);
	Control::attachControl(&m_DownButton, PLACE_RELATIVE_BOTRIGHT, 0, 0);

	m_PaneBuffer = al_create_bitmap(m_PaneWidth, m_PaneHeight);
	m_ScrollPosition = 0;
	
	m_UpButton.onClick(
		[&](const ALLEGRO_EVENT& e) { scroll(-10); return false; }
		);
	m_DownButton.onClick(
		[&](const ALLEGRO_EVENT& e) { scroll(+10); return false; }
		);
}

void ScrollPanel::scroll(double scrollAmount)
{
	m_ScrollPosition = std::max(0.0, std::min(m_PaneHeight - m_Height, m_ScrollPosition + scrollAmount));
}

void ScrollPanel::attachControl(Control* child, int placement, double x, double y)
{
	Control::attachControl(child);

	switch(placement)
	{
		case PLACE_RELATIVE_TOPLEFT:
			child->reposition(x, y);
			break;
		
		case PLACE_RELATIVE_TOPRIGHT:
			child->reposition(m_PaneWidth -(x + child->width()), y);
			break;

		case PLACE_RELATIVE_BOTLEFT:
			child->reposition(x, m_PaneHeight - (y + child->height()));
			break;

		case PLACE_RELATIVE_BOTRIGHT:
			child->reposition(m_PaneWidth - (x + child->width()), m_PaneHeight - (y + child->height()));
			break;
	}
}

void ScrollPanel::draw(double x, double y)
{
	double drawX = m_X + x;
	double drawY = m_Y + y;

	ALLEGRO_STATE state;
	al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);

	al_set_target_bitmap(m_PaneBuffer);
	al_clear_to_color(Theme::CurrentTheme->colourBackground);

	for(auto child : m_ChildrenControls)
	{
		if (child == &m_UpButton || child == &m_DownButton) continue;
		child->draw(0, 0);
	}

	al_restore_state(&state);

	al_draw_bitmap_region(m_PaneBuffer, 0, m_ScrollPosition, m_PaneWidth, m_Height, drawX, drawY, 0);

	m_UpButton.draw(drawX, drawY);
	m_DownButton.draw(drawX, drawY);

	Theme::CurrentTheme->drawRectangleBorder(drawX, drawY, drawX + m_Width, drawY + m_Height);
}

std::pair<double, double> ScrollPanel::getChildOffset(Control* child, double offsetX, double offsetY)
{
	if (child == &m_UpButton || child == &m_DownButton)
	{
		return std::pair<double, double>(offsetX + m_X, offsetY + m_Y);
	}

	return std::pair<double, double>(offsetX + m_X, offsetX + m_Y - m_ScrollPosition);
}
