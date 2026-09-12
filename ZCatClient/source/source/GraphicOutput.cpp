#include "GraphicOutput.h"


void graphic_init()
{
    SDL_SetHint(SDL_HINT_IME_SHOW_UI,"1");
    TTF_Init();
}

/************************************************-----function-----************************************************/

RectButton::RectButton()
{
    ;
}

RectButton::RectButton(SDL_Rect& rect,SDL_Color& border_color,SDL_Color& fill_color,int font_size,SDL_Color& font_color,const char* text)
{
    this->rect=rect;
    this->font_size=font_size;
    this->font_color=font_color;
    this->fill_color=fill_color;
    this->border_color=border_color;
    
    if(text!=NULL)
        strcpy(this->text,text);
    else
        strcpy(this->text,"");

    this->text_rect=rect;
    this->text_rect.x+=5;
    this->text_rect.y+=5;
}

void RectButton::SetValue(SDL_Rect& rect,SDL_Color& border_color,SDL_Color& fill_color,int font_size,SDL_Color& font_color,const char* text)
{
    this->rect=rect;
    this->font_size=font_size;
    this->font_color=font_color;
    this->fill_color=fill_color;
    this->border_color=border_color;
    
    if(text!=NULL)
        strcpy(this->text,text);
    else
        strcpy(this->text,"");

    this->text_rect=rect;
    this->text_rect.x+=5;
    this->text_rect.y+=5;
}

void RectButton::Draw(SDL_Renderer* rdr)  //have no text: NULL or ""
{
    SDL_SetRenderDrawColor(rdr,this->fill_color.r,this->fill_color.g,this->fill_color.b,this->fill_color.a);
    SDL_RenderFillRect(rdr,&(this->rect));

    SDL_SetRenderDrawColor(rdr,this->border_color.r,this->border_color.g,this->border_color.b,this->border_color.a);
    SDL_RenderDrawRect(rdr,&(this->rect));

    if(strcmp(this->text,""))
    {   
        /*
            notice:
            program file in bin
            current terminal -> ./
            exe file -> ../
        */
        TTF_Font* font=TTF_OpenFont("../resource/font/simyou.ttf",font_size);
        if(font==NULL)
        {
            SDL_Log("TTF_OpenFont:%s",TTF_GetError());
            return;
        }

        SDL_Surface* text_surf=TTF_RenderUTF8_Blended(font,text,this->font_color);
        SDL_Texture* text_tt=SDL_CreateTextureFromSurface(rdr,text_surf);
        this->text_rect.w=text_surf->w,this->text_rect.h=text_surf->h;
        this->text_rect.x=this->rect.x+5,this->text_rect.y=this->rect.y+5;
        SDL_RenderCopy(rdr,text_tt,NULL,&(this->text_rect));

        TTF_CloseFont(font);
        SDL_FreeSurface(text_surf);
        SDL_DestroyTexture(text_tt);
    }
}

//bg_color base color,ft_color Response color
void RectButton::InteractResponse(SDL_Point& pt,SDL_Rect& rect,SDL_Color bg_color,SDL_Color ft_color)
{
    if(SDL_PointInRect(&pt,&rect))
    {
        this->fill_color.r=ft_color.r;
        this->fill_color.g=ft_color.g;
        this->fill_color.b=ft_color.b;
    }
    else
    {
        this->fill_color.r=bg_color.r;
        this->fill_color.g=bg_color.g;
        this->fill_color.b=bg_color.b;
    }
}

RectButton::~RectButton()
{
    ;
}

/************************************************-----function-----************************************************/

Text::Text(int x,int y,SDL_Color& font_color,int font_size,const char* text)
{
    this->text_rect.x=x;
    this->text_rect.y=y;
    this->font_color=font_color;
    this->font_size=font_size;

    if(text!=NULL)
        strcpy(this->text,text);
    else
        strcpy(this->text,"");
}

void Text::Draw(SDL_Renderer* rdr)
{
    if(strcmp(this->text,""))
    {   
        /*
            notice:
            program file in bin
            current terminal -> ./
            exe file -> ../
        */
        TTF_Font* font=TTF_OpenFont("../resource/font/simyou.ttf",font_size);
        if(font==NULL)
        {
            SDL_Log("TTF_OpenFont:%s",TTF_GetError());
            return;
        }

        SDL_Surface* text_surf=TTF_RenderUTF8_Blended(font,text,this->font_color);
        SDL_Texture* text_tt=SDL_CreateTextureFromSurface(rdr,text_surf);
        this->text_rect.w=text_surf->w,this->text_rect.h=text_surf->h;
        SDL_RenderCopy(rdr,text_tt,NULL,&(this->text_rect));

        TTF_CloseFont(font);
        SDL_FreeSurface(text_surf);
        SDL_DestroyTexture(text_tt);
    }
}

Text::~Text()
{
    ;
}

/************************************************-----function-----************************************************/

Message::Message()
{
    ;
}

Message::Message(SDL_Rect& rect,SDL_Color& border_color,SDL_Color& fill_color,int font_size,SDL_Color& font_color,const char* text)
{
    this->rect=rect;
    this->font_size=font_size;
    this->font_color=font_color;
    this->fill_color=fill_color;
    this->border_color=border_color;
    
    if(text!=NULL)
        strcpy(this->text,text);
    else
        strcpy(this->text,"");

    this->text_rect=rect;
    this->text_rect.x+=5;
    this->text_rect.y+=5;
}

void Message::SetValue(SDL_Rect& rect,SDL_Color& border_color,SDL_Color& fill_color,int font_size,SDL_Color& font_color,const char* text)
{
    this->rect=rect;
    this->font_size=font_size;
    this->font_color=font_color;
    this->fill_color=fill_color;
    this->border_color=border_color;
    
    if(text!=NULL)
        strcpy(this->text,text);
    else
        strcpy(this->text,"");

    this->text_rect=rect;
    this->text_rect.x+=5;
    this->text_rect.y+=5;
}

void Message::Draw(SDL_Renderer* rdr,int line_width_px)  //have no text: NULL or ""
{
    SDL_SetRenderDrawColor(rdr,this->fill_color.r,this->fill_color.g,this->fill_color.b,this->fill_color.a);
    SDL_RenderFillRect(rdr,&(this->rect));

    SDL_SetRenderDrawColor(rdr,this->border_color.r,this->border_color.g,this->border_color.b,this->border_color.a);
    SDL_RenderDrawRect(rdr,&(this->rect));

    if(strcmp(this->text,""))
    {   
        /*
            notice:
            program file in bin
            current terminal -> ./
            exe file -> ../
        */
        TTF_Font* font=TTF_OpenFont("../resource/font/simyou.ttf",font_size);
        if(font==NULL)
        {
            SDL_Log("TTF_OpenFont:%s",TTF_GetError());
            return;
        }

        SDL_Surface* text_surf=TTF_RenderUTF8_Blended_Wrapped(font,text,this->font_color,line_width_px);
        SDL_Texture* text_tt=SDL_CreateTextureFromSurface(rdr,text_surf);
        this->text_rect.w=text_surf->w,this->text_rect.h=text_surf->h;
        this->text_rect.x=this->rect.x+5,this->text_rect.y=this->rect.y+5;
        SDL_RenderCopy(rdr,text_tt,NULL,&(this->text_rect));

        TTF_CloseFont(font);
        SDL_FreeSurface(text_surf);
        SDL_DestroyTexture(text_tt);
    }
}

Message::~Message()
{
    ;
}

/************************************************-----function-----************************************************/


MessageText::MessageText()
{
    ;
}

MessageText::MessageText(int x,int y,SDL_Color& font_color,int font_size,const char* text)
{
    this->text_rect.x=x;
    this->text_rect.y=y;
    this->font_color=font_color;
    this->font_size=font_size;

    if(text!=NULL)
        strcpy(this->text,text);
    else
        strcpy(this->text,"");
}

void MessageText::Draw(SDL_Renderer* rdr,int line_width_px)
{
    if(strcmp(this->text,""))
    {   
        /*
            notice:
            program file in bin
            current terminal -> ./
            exe file -> ../
        */
        TTF_Font* font=TTF_OpenFont("../resource/font/simyou.ttf",font_size);
        if(font==NULL)
        {
            SDL_Log("TTF_OpenFont:%s",TTF_GetError());
            return;
        }

        SDL_Surface* text_surf=TTF_RenderUTF8_Blended_Wrapped(font,text,this->font_color,line_width_px);
        SDL_Texture* text_tt=SDL_CreateTextureFromSurface(rdr,text_surf);
        this->text_rect.w=text_surf->w,this->text_rect.h=text_surf->h;
        SDL_RenderCopy(rdr,text_tt,NULL,&(this->text_rect));

        TTF_CloseFont(font);
        SDL_FreeSurface(text_surf);
        SDL_DestroyTexture(text_tt);
    }
}

MessageText::~MessageText()
{
    ;
}