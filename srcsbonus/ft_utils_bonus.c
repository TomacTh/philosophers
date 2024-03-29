/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 13:48:35 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/03 15:19:54 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

void	sleep_if_active(useconds_t time, int *active)
{
	if (*active)
		usleep(time);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
		write(fd, s, ft_strlen(s));
}

int	ft_return_error(int code, char *str)
{
	if (*str)
		ft_putstr_fd(str, 2);
	return (code);
}

size_t	convert_in_ms(struct timeval current_time)
{
	size_t	res;

	res = current_time.tv_usec / 1000 + (current_time.tv_sec * 1000);
	return (res);
}
