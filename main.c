/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkellum <nkellum@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 17:29:43 by nkellum           #+#    #+#             */
/*   Updated: 2019/09/21 22:18:40 by nkellum          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	plot(int x, int y, t_mlx *mlx, double alpha, double specular, t_vector3 *color)
{
	int index;

	index = 4 * (y * WIDTH) + 4 * x;
	mlx->img_str[index] = (char)ft_constrain(alpha * color->z + specular * 100, 0, 255);
	mlx->img_str[index + 1] = (char)ft_constrain(alpha * color->y + specular * 100, 0, 255);
	mlx->img_str[index + 2] = (char)ft_constrain(alpha * color->x + specular * 100, 0, 255);
}

void	initialize_mlx(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, WIDTH, HEIGHT, "RTv1");
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, WIDTH, HEIGHT);
	mlx->img_str = mlx_get_data_addr(mlx->img_ptr, &(mlx->bpp),
	&(mlx->size_line), &(mlx->endian));
}

void	normalize(t_vector3 *vec)
{
	double mag;

	mag = sqrt(pow(vec->x, 2) + pow(vec->y, 2) + pow(vec->z, 2));
	vec->x /= mag;
	vec->y /= mag;
	vec->z /= mag;
}

double intersect(t_ray *ray, t_sphere *sphere)
{
	double t0, t1;
	t_vector3 *L = sub_vector3(sphere->pos, ray->pos, 0);
	float tca = scal_vector3(L, ray->dir, 0);
	if (tca < 0)
		return (0);
	float d2 = scal_vector3(L, L, 0) - tca * tca;
	if (d2 > pow(sphere->radius, 2))
		return (0);
	float thc = sqrt(pow(sphere->radius, 2) - d2);
	t0 = tca - thc;
	t1 = tca + thc;
	if(t0 <= 0 && t1 <= 0)
		return (0);
	if (t0 < t1)
		return (t0);
	else
		return (t1);
}

double intersect_plane(t_ray *ray, t_ray *plane)
{
    // assuming vectors are all normalized
    double denom = scal_vector3(plane->dir, ray->dir, 0);
    if (denom > 1e-6) {
        t_vector3 *p0l0 = sub_vector3(plane->pos, ray->pos, 0);
        double t = scal_vector3(p0l0, plane->dir, 0) / denom;
        return (t >= 0);
    }
    return 0;
}

t_vector3 *reflect(t_vector3 *light, t_vector3 *normal)
{
		float dotprod = 2 * scal_vector3(normal, light, 0);
		normal->x *= dotprod;
		normal->y *= dotprod;
		normal->z *= dotprod;

		return sub_vector3(normal, light, 0);
}

int	main(int argc, char **argv)
{
	t_ray	*eye;
	t_ray	*plane;
	t_vector3 *screen;
	t_vector3 *hit_point;
	t_vector3 *sphere_normal;
	t_vector3 *hit_line;
	t_sphere *sphere;
	t_mlx	*mlx;

	if ((mlx = malloc(sizeof(t_mlx))) == NULL)
		return (0);
	initialize_mlx(mlx);
	if ((eye = malloc(sizeof(t_ray))) == NULL)
		return (0);
	if ((plane = malloc(sizeof(t_ray))) == NULL)
		return (0);
	if ((sphere = malloc(sizeof(t_sphere))) == NULL)
		return (0);
	sphere->pos = new_vector3(0, 0, 0);
	sphere->radius = 150.0;
	eye->pos = new_vector3(0, 0, -300);
	screen = new_vector3(-WIDTH / 2, -HEIGHT / 2, -100);
	plane->pos = new_vector3(0, 0, 0);
	plane->dir = new_vector3(0, 1, 0);


	// printf("%f %f %f\normalizing...\n", vec->x, vec->y, vec->z);
	// normalize(vec);
	// printf("%f %f %f\n", vec->x, vec->y, vec->z);
	int hitnum = 0;
	double intersectdist = 0.0;

	while(screen->y < HEIGHT/ 2)
	{
		screen->x = -WIDTH / 2;
		while(screen->x < WIDTH / 2)
		{
			eye->dir = sub_vector3(screen, eye->pos, 0);
			normalize(eye->dir);
			double t = intersect_plane(eye, plane);
			if(t)
			{
				//plot(screen->x + WIDTH / 2, screen->y + HEIGHT/ 2, mlx, 1);
				t_vector3 *view_normal = new_vector3(-eye->dir->x, -eye->dir->y,
				-eye->dir->z);
				double facing_ratio = scal_vector3(plane->dir, view_normal, 0);
				//printf("facing_ratio is %f\n", facing_ratio);
				facing_ratio = -facing_ratio;
				if(facing_ratio > 1)
					facing_ratio = 1;
				if(facing_ratio > 0.0)
					plot(screen->x + WIDTH / 2, screen->y + HEIGHT/ 2, mlx, facing_ratio, 0,  new_vector3(255,255,255));
			}
			intersectdist = intersect(eye, sphere);
			if(intersectdist)
			{
				hit_line = new_vector3(eye->dir->x * intersectdist,
				eye->dir->y * intersectdist, eye->dir->z * intersectdist);
				hit_point = add_vector3(eye->pos, hit_line, 0);
				sphere_normal = sub_vector3(hit_point, sphere->pos, 0);
				normalize(sphere_normal);
				t_vector3 *light_dir = new_vector3(1, -1, -0.8);
				double facing_ratio = scal_vector3(sphere_normal, light_dir, 0);
				facing_ratio *= 0.5; // can be edited with light intensity

				t_vector3 *r = reflect(light_dir, sphere_normal);
				normalize(r);
				t_vector3 *v = new_vector3(-eye->dir->x,
				-eye->dir->y, -eye->dir->z);
				double specular = pow(scal_vector3(r, v, 0), 400);

				if(facing_ratio > 1)
					facing_ratio = 1;
				if(facing_ratio > 0.0)
					plot(screen->x + WIDTH / 2, screen->y + HEIGHT/ 2, mlx, facing_ratio, specular, new_vector3(147,233,190));
				else
					plot(screen->x + WIDTH / 2, screen->y + HEIGHT/ 2, mlx, 0, 0, new_vector3(0,0,0));
			}
			screen->x++;
		}
		screen->y++;
	}
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
	mlx_hook(mlx->win_ptr, 2, 0, deal_key, mlx);
	mlx_hook(mlx->win_ptr, 17, 0, win_close, mlx);
	mlx_loop(mlx->mlx_ptr);

	return (0);
}
