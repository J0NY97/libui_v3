gcc -o path_finder ui_path.c -I../libft -L../libft -L../libpf -I../libpf -lpf -lft  -I../SDL_Frameworks/SDL2.framework/Headers -framework SDL2 -F../SDL_Frameworks;
./path_finder;
rm path_finder;
