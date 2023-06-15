/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramos-m <pramos-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:47:59 by pramos-m          #+#    #+#             */
/*   Updated: 2023/06/12 12:15:17 by pramos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include    <msh.h>

int	exec_pwd(void)
{
	if (print_one_env("PWD") == -1)
		return (1);
	return (0);
}
