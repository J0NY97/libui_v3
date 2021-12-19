gcc -o path_finder ui_path.c -I../libft -L../libft -L../libpf -I../libpf -lpf -lft -I../SDL_WIN/include -L../SDL_WIN/lib -lSDL2;
./path_finder;
rm path_finder;
