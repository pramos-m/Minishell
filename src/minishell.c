/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramos-m <pramos-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 17:36:26 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/12 17:53:30 by pramos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<msh.h>

int	main(int ac, char **av, char **env)
{
	char	*line;

	(void) av;
	if (ac > 1)
		exit(1);
	ft_env(env);
	while (42)
	{
		line = readline("PESH + 🐚 > ");
		if (!line)
			break ;
		if (validate_input(line))
			printf("ERROR\n");
		g_msh.err = 0;
		// printf("first child line: :%s:\n", g_msh.block->child->line);
		// printf("first child line: :%s:\n", g_msh.block->child->next->child->child->child->line);
		// printf("first child line: :%s:\n", g_msh.block->child->next->child->child->child->next->line);
		// printf("first child line: :%s:\n", g_msh.block->child->next->child->child->next->line);
		// printf("first child line: :%s:\n", g_msh.block->child->next->child->next->line);
		// printf("first child line: :%s:\n", g_msh.block->child->next->next->line);
		// printf("first child: :%s:\n", g_msh.block->child->line);
		// printf("next first child: :%s:\n", g_msh.block->child->next->line);
		// printf("next first child child: :%s:\n", g_msh.block->child->next->child->line);
		// printf("next first child child child: :%s:\n", g_msh.block->child->next->child->child->line);
		// printf("next first child child child: :%s:\n", g_msh.block->child->next->child->child->child->next->line);
	}
	return (0);
}
