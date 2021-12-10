#include "libui.h"

void	ui_surface_print(SDL_Surface *surface)
{
	ft_printf("[%s] : \n", __FUNCTION__);
	if (!surface)
	{
		ft_printf("surface given doesn\'t exist.\n");
		return ;
	}
	ft_printf("\tw : %d\n", surface->w);
	ft_printf("\th : %d\n", surface->h);
	ft_printf("\tpitch : %d\n", surface->pitch);
	ft_printf("\tformat : \n");
	ft_printf("\t\tformat : %d %s\n", surface->format->format,
		SDL_GetPixelFormatName(surface->format->format));
	ft_printf("\t\tBitsPerPixel : %d\n", surface->format->BitsPerPixel);
	ft_printf("\t\tBytesPerPixel : %d\n", surface->format->BytesPerPixel);
	ft_printf("\t\tRmask : %.8x\n", surface->format->Rmask);
	ft_printf("\t\tGmask : %.8x\n", surface->format->Gmask);
	ft_printf("\t\tBmask : %.8x\n", surface->format->Bmask);
	ft_printf("\t\tAmask : %.8x\n", surface->format->Amask);
}

SDL_Surface	*ui_surface_text_new(
		char *text, char *font_path, int size, Uint32 font_color)
{
	SDL_Surface	*surface;
	TTF_Font	*font;
	t_rgba		rgba;
	SDL_Color	color;

	font = TTF_OpenFont(font_path, size);
	if (!font)
	{
		ft_printf("[%s] Font path doesnt exist.\n", __FUNCTION__);
		return (NULL);
	}
	rgba = hex_to_rgba(font_color);
	color.r = rgba.r;
	color.g = rgba.g;
	color.b = rgba.b;
	color.a = rgba.a;
	surface = TTF_RenderUTF8_Blended(font, text, color);
	TTF_CloseFont(font);
	font = NULL;
	return (surface);
}

SDL_Surface	*ui_surface_image_new(char *image_path)
{
	SDL_Surface	*surface;

	surface = IMG_Load(image_path);
	if (!surface)
		ft_printf("[%s] Error loading image from path. <%s>\n",
			__FUNCTION__, image_path);
	return (surface);
}
