#ifndef ALGUI_CONTROL_
#define ALGUI_CONTROL_

#include <string>
#include <functional>
#include <forward_list>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <iostream>

namespace alGui 
{
	class Theme
	{
		private:
			static std::string FontDirectory;
		
		public:
			Theme();
			~Theme();

			ALLEGRO_FONT*  fontTitle;
			ALLEGRO_FONT*  fontNormal;

			ALLEGRO_COLOR colourFont;
			ALLEGRO_COLOR colourBackground;
			ALLEGRO_COLOR colourHighlight;
			ALLEGRO_COLOR colourBorder;

			int borderWidth;
			int borderPaddingHorizontal;
			int borderPaddingVertical;

			virtual void drawRectangleBorder(double x1, double y1, double x2, double y2);
			virtual void drawVerticalBorder(double x, double y1, double y2, bool isLeftBorder);
			virtual void drawHorizontalBorder(double x1, double x2, double y, bool isTopBorder);

			static Theme* CurrentTheme;
			static void SetFontDirectory(std::string fontDirectory);
			static void SetDefaultTheme();
			static void SetCurrentTheme(Theme* theme);
	};	

	const int PLACE_RELATIVE_TOPLEFT = 0;
	const int PLACE_RELATIVE_TOPRIGHT = 1;
	const int PLACE_RELATIVE_BOTLEFT = 2;
	const int PLACE_RELATIVE_BOTRIGHT = 3;

	class Control
	{
		protected:
			double m_X, m_Y, m_Width, m_Height;
			bool m_MouseOver;
			bool m_MouseDown;

			std::function<bool (const ALLEGRO_EVENT&)> m_ClickDelegate;
			std::function<bool (const ALLEGRO_EVENT&)> m_MouseDownDelegate;
			std::function<bool (const ALLEGRO_EVENT&)> m_MouseEnterDelegate;	
			std::function<bool (const ALLEGRO_EVENT&)> m_MouseLeaveDelegate;	
			std::forward_list<Control*> m_ChildrenControls;

			virtual bool doClick(const ALLEGRO_EVENT& event);
			virtual bool doMouseDown(const ALLEGRO_EVENT& event);
			virtual bool doMouseEnter(const ALLEGRO_EVENT& event);
			virtual bool doMouseLeave(const ALLEGRO_EVENT& event);

		public:
			Control(double width, double height);
			Control(double x, double y, double width, double height);
			~Control();

			void onClick(std::function<bool (const ALLEGRO_EVENT&)> handlerFunc);
			void onMouseDown(std::function<bool (const ALLEGRO_EVENT&)> handlerFunc);
			void onMouseEnter(std::function<bool (const ALLEGRO_EVENT&)> handlerFunc);
			void onMouseLeave(std::function<bool (const ALLEGRO_EVENT&)> handlerFunc);

			virtual bool propergateClickEvent(const ALLEGRO_EVENT& event, double offsetX, double offsetY);
			virtual bool propergateMouseMoveEvent(const ALLEGRO_EVENT& event, double offsetX, double offsetY);
			virtual void propergateMouseLeaveEvent(const ALLEGRO_EVENT& event);
			virtual void handleEvent(const ALLEGRO_EVENT& event);
			virtual void draw();
			virtual void draw(double x, double y);

			virtual void attachControl(Control* child);
			virtual void attachControl(Control* child, double x, double y);
			virtual void attachControl(Control* child, int placement, double x, double y);

			virtual std::pair<double, double> getChildOffset(Control* child, double offsetX, double offsetY);

			virtual bool isPointInside(double x, double y);
			virtual bool isPointInside(double x, double y, double offsetX, double offsetY);
			virtual void reposition(double x, double y);
		
			virtual double width();
			virtual double height();	
	};

	class Panel : public Control
	{
		protected:
			std::string m_Title;

		public: 
			Panel(double width, double height);
			Panel(double x, double y, double width, double height);
			Panel(std::string title, double width, double height);
			Panel(std::string title, double x, double y, double width, double height);
			virtual void draw(double x, double y);
	};
	
	class Button : public Control
	{
		private:
			std::string m_Label;

		public:
			Button(std::string label);
			Button(std::string label, double x, double y);
			virtual void draw(double x, double y);
	};

	class ScrollPanel : public Control
	{
		private: 
			Button m_UpButton;
			Button m_DownButton;
			ALLEGRO_BITMAP* m_PaneBuffer;
			double m_PaneHeight;
			double m_PaneWidth;
			double m_ScrollPosition;
			
			void init();
			void scroll(double amount);

		public:
			ScrollPanel(double width, double displayHeight, double scrollHeight);			
			ScrollPanel(double x, double y, double width, double displayHeight, double scrollHeight);
			~ScrollPanel();

			virtual std::pair<double, double> getChildOffset(Control* child, double offsetX, double offsetY);
			virtual void attachControl(Control* child, int placement, double x, double y);
			virtual void draw(double x, double y);
	};
}
#endif
