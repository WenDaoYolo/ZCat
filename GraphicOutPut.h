#include<SDL.h>
#include<iostream>
#include<SDL_ttf.h>

class RectButton
{
    public:
        SDL_Rect rect;
        SDL_Rect text_rect;
        SDL_Color border_color;
        SDL_Color fill_color;
        SDL_Color font_color;
        int font_size;
        char text[128];
        
        RectButton();
        void Draw(SDL_Renderer* rdr);
        void InteractResponse(SDL_Point& pt,SDL_Rect& rect,SDL_Color bg_color,SDL_Color ft_color);
        RectButton(SDL_Rect& rect,SDL_Color& border_color,SDL_Color& fill_color,int font_size,SDL_Color& font_color,const char* text);
        void SetValue(SDL_Rect& rect,SDL_Color& border_color,SDL_Color& fill_color,int font_size,SDL_Color& font_color,const char* text);
        ~RectButton();
};

class Text
{
    public:
        SDL_Rect text_rect;
        SDL_Color font_color;
        int font_size;
        char text[256];

        Text(int x,int y,SDL_Color& font_color,int font_size,const char* text);
        void Draw(SDL_Renderer* rdr);
        ~Text();
};

void graphic_init();