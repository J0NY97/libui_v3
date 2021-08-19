#ifndef UI_VEC_H
# define UI_VEC_H

# define VEC2_SIZE 2
# define VEC4_SIZE 4

typedef struct s_vec2i
{
	union {
		struct {
			int x;
			int y;
		};
		int v[VEC2_SIZE];
	};
}					t_vec2i;

typedef struct s_vec4
{
	union {
		struct {
			float x;
			float y;
			float w;
			float h;
		};
		float v[VEC4_SIZE];
	};
}					t_vec4;

typedef struct s_vec4i
{
	union {
		struct {
			int x;
			int y;
			int w;
			int h;
		};
		int v[VEC4_SIZE];
	};
}					t_vec4i;

#endif
