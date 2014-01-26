#include "Control.h"

#include <iostream>

using namespace alGui;

Control::Control(double width, double height)
	: m_X(0), m_Y(0), m_Width(width), m_Height(height)
{
	m_MouseOver = false;
	m_MouseDown = false;
}

Control::Control(double x, double y, double width, double height)
	: m_X(x), m_Y(y), m_Width(width), m_Height(height) 
{
	m_MouseOver = false;
	m_MouseDown = false;
}

Control::~Control()
{
	/*auto end = m_ChildrenControls.end();
	for(auto iter = m_ChildrenControls.begin(); iter != end; iter ++)
	{
		delete *iter;
	}*/
}

void Control::handleEvent(const ALLEGRO_EVENT& event)
{
	switch(event.type)
	{
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			propergateClickEvent(event, 0, 0);
			break;

		case ALLEGRO_EVENT_MOUSE_AXES:
		case ALLEGRO_EVENT_MOUSE_WARPED:
		case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
		case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
			propergateMouseMoveEvent(event, 0, 0);
			break;
	}
}

bool Control::propergateClickEvent(const ALLEGRO_EVENT& event, double offsetX, double offsetY)
{
	if (!isPointInside(event.mouse.x, event.mouse.y, offsetX, offsetY)) return true;

	for(auto child : m_ChildrenControls)
	{
		std::pair<double, double> childOffset = getChildOffset(child, offsetX, offsetY);
		bool finished = !child->propergateClickEvent(event, childOffset.first, childOffset.second);
		
		if (finished) return false;
	}

	if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		return doClick(event);
	}
	
	return doMouseDown(event);
}

bool Control::propergateMouseMoveEvent(const ALLEGRO_EVENT& event, double offsetX, double offsetY)
{
	if (!isPointInside(event.mouse.x, event.mouse.y, offsetX, offsetY) && m_MouseOver)
	{
		propergateMouseLeaveEvent(event);
		return true;
	}

	for(auto child : m_ChildrenControls)
	{
		std::pair<double, double> childOffset = getChildOffset(child, offsetX, offsetY);
		bool finished = !child->propergateMouseMoveEvent(event, childOffset.first, childOffset.second);
		
		if (finished) return false;
	}

	if (isPointInside(event.mouse.x, event.mouse.y, offsetX, offsetY) == m_MouseOver) return true;

	return (m_MouseOver ? doMouseLeave(event) : doMouseEnter(event));
}

void Control::propergateMouseLeaveEvent(const ALLEGRO_EVENT& event)
{
	for(auto child : m_ChildrenControls)
	{
		child->propergateMouseLeaveEvent(event);
	}

	if (m_MouseOver) doMouseLeave(event);
}

std::pair<double, double> Control::getChildOffset(Control* child, double offsetX, double offsetY)
{
	return std::pair<double, double>(m_X + offsetX, m_Y + offsetY);
}

bool Control::doClick(const ALLEGRO_EVENT& event)
{
	m_MouseDown = false;

	if (m_ClickDelegate)
	{
		return m_ClickDelegate(event);
	}

	return true;
}

bool Control::doMouseDown(const ALLEGRO_EVENT& event)
{
	m_MouseDown = true;

	if (m_MouseDownDelegate)
	{
		return m_MouseDownDelegate(event);
	}

	return true;
}

bool Control::doMouseEnter(const ALLEGRO_EVENT& event)
{
	m_MouseOver = true;

	if (m_MouseEnterDelegate)
	{
		return m_MouseEnterDelegate(event);
	}
	
	return true;
}

bool Control::doMouseLeave(const ALLEGRO_EVENT& event)
{
	m_MouseOver = false;
	m_MouseDown = false;

	if (m_MouseLeaveDelegate)
	{
		return m_MouseLeaveDelegate(event);
	}
	
	return true;
}

bool Control::isPointInside(double x, double y)
{
	return !(x < m_X || x > m_X + m_Width || y < m_Y || y > m_Y + m_Height);
}

bool Control::isPointInside(double x, double y, double offsetX, double offsetY)
{
	return 	!(x < (m_X + offsetX) 
			|| x > (m_X + offsetX + m_Width)
			|| y < (m_Y + offsetY) 
			|| y > (m_Y + offsetY + m_Height)
			);
}

void Control::attachControl(Control* child) 
{
	m_ChildrenControls.push_front(child);
}

void Control::attachControl(Control* child, double x, double y)
{
	attachControl(child, PLACE_RELATIVE_TOPLEFT, x, y);
}

void Control::attachControl(Control* child, int placement, double x, double y)
{
	attachControl(child);

	switch(placement)
	{
		case PLACE_RELATIVE_TOPLEFT:
			child->reposition(x, y);
			break;
		
		case PLACE_RELATIVE_TOPRIGHT:
			child->reposition(m_Width - (x + child->width()), y);
			break;

		case PLACE_RELATIVE_BOTLEFT:
			child->reposition(x, m_Height - (y + child->height()));
			break;

		case PLACE_RELATIVE_BOTRIGHT:
			child->reposition(m_Width - (x + child->width()), m_Height - (y + child->height()));
			break;
	}
}

void Control::reposition(double x, double y)
{
	m_X = x;
	m_Y = y;
}

double Control::width() { return m_Width; }
double Control::height() { return m_Height; }

void Control::onClick(std::function<bool (const ALLEGRO_EVENT&)> handlerFunc)
{
	m_ClickDelegate = handlerFunc;
}

void Control::onMouseDown(std::function<bool (const ALLEGRO_EVENT&)> handlerFunc)
{
	m_MouseDownDelegate = handlerFunc;
}

void Control::onMouseEnter(std::function<bool (const ALLEGRO_EVENT&)> handlerFunc)
{
	m_MouseEnterDelegate = handlerFunc;
}

void Control::onMouseLeave(std::function<bool (const ALLEGRO_EVENT&)> handlerFunc)
{
	m_MouseLeaveDelegate = handlerFunc;
}

void Control::draw()
{
	ALLEGRO_STATE state;
	al_store_state(&state, ALLEGRO_STATE_ALL);

	draw(0, 0);
		
	al_restore_state(&state);
}

void Control::draw(double x, double y)
{
	auto end = m_ChildrenControls.end();
	for(auto iter = m_ChildrenControls.begin(); iter != end; iter ++)
	{
		(*iter)->draw(x + m_X, y + m_Y);
	}
}
