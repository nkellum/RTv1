/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkellum <nkellum@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 16:31:29 by nkellum           #+#    #+#             */
/*   Updated: 2019/10/02 16:14:08 by nkellum          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include "minilibx_macos/mlx.h"
# include "libft/libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <stdlib.h>

#define WIDTH 1280
#define HEIGHT 900

typedef struct		s_ray
{
	t_vector3		*pos;
	t_vector3		*dir;
	int				id;
	struct s_ray	*next;
}					t_ray;

typedef struct		s_sphere
{
	t_vector3		*pos;
	double			radius;
	int				id;
	struct s_sphere	*next;
}					t_sphere;

typedef struct	s_cone
{
	t_vector3	*pos;
	t_vector3	*dir;
	double		theta;
}				t_cone;

typedef struct	s_cylinder
{
	t_vector3	*pos;
	t_vector3	*dir;
	double		radius;
}				t_cylinder;

typedef struct	s_mlx
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	char		*img_str;
	int			bpp;
	int			size_line;
	int			endian;
}				t_mlx;


int				deal_key(int key, void *param);
int				win_close(void *param);
void			initialize_mlx(t_mlx *mlx);

#endif
