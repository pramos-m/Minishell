/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:40:07 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/30 19:14:45 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<msh.h>

int	check_rd(t_token *tk)
{
	t_token	*next;
	t_token	*prev;
	int		n_type;
	int		p_type;

	if (!tk)
		return (1);
	next = tk->next;
	prev = tk->prev;
	(next && (n_type = tk->next->type));
	(prev && (n_type = tk->prev->type));
	return ((next && n_type != ARG));
}

int	check_and_or(t_token *tk)
{
	t_token	*next;
	t_token	*prev;
	int		n_type;
	int		p_type;

	if (!tk)
		return (1);
	next = tk->next;
	prev = tk->prev;
	(next && (n_type = tk->next->type));
	(prev && (n_type = tk->prev->type));
	return ((next && (n_type == PIPE || n_type == CP || n_type == AND \
			|| n_type == OR || n_type == EOCL)) || \
			(prev && (p_type != ARG && prev->type != CP)));
}

int	check_cp(t_token *tk)
{
	t_token	*next;
	t_token	*prev;
	int		n_type;
	int		p_type;

	if (!tk)
		return (1);
	next = tk->next;
	prev = tk->prev;
	(next && (n_type = tk->next->type));
	(prev && (n_type = tk->prev->type));
	return ((next && (n_type == ARG || n_type == OP)) || \
			(prev && (p_type != ARG && prev->type != CP)));
}

int	check_op(t_token *tk)
{
	t_token	*next;
	t_token	*prev;
	int		n_type;
	int		p_type;

	if (!tk)
		return (1);
	next = tk->next;
	prev = tk->prev;
	(next && (n_type = tk->next->type));
	(prev && (n_type = tk->prev->type));
	return ((next && (n_type == PIPE || n_type == CP || n_type == AND \
			|| n_type == OR || n_type == EOCL)) || \
			(prev && (p_type != PIPE && prev->type != OP \
			&& prev->type != AND && prev->type != OR)));
}

int	check_pipe(t_token *tk)
{
	t_token	*next;
	t_token	*prev;
	int		n_type;
	int		p_type;

	if (!tk)
		return (1);
	next = tk->next;
	prev = tk->prev;
	(next && (n_type = tk->next->type));
	(prev && (n_type = tk->prev->type));
	return ((next && (n_type == PIPE || n_type == CP || n_type == AND \
			|| n_type == OR || n_type == EOCL)) || \
			(prev && (p_type != ARG && prev->type != CP)));
}

int	chek_arg(t_token *tk)
{
	return ((tk->next && tk->next->type == OP) || \
			(tk->prev && tk->prev->type == CP));
}

int	check_tokens(t_tokens **tk)
{
	t_token	*tmp;
	int		err;

	(1 && (tmp = *tk) && (err = 0));
	((tmp->type == AND || tmp->type == OR || tmp->type == PIPE \
	|| tmp->type == CP) && (err = 1));
	while (tmp->next && tmp->type != EOCL && err == 0)
	{
		(((tmp->type == RDI || tmp->type == RDO || tmp->type == RDAP || \
	tmp->type == RDHD) && (!tmp->next || tmp->next->type != ARG) && (err = 1)) \
	|| (tmp->type == PIPE && (!tmp->next || tmp->next->type == OR) && \
	(err = 1)) || ((tmp->type == AND || tmp->type == OR) && (!tmp->next || \
	(tmp->next->type == AND || tmp->next->type == OR)) && (err = 1)) || \
	((tmp->type == ) && (err = 1)));
		tmp = tmp->next;
	}
	((tmp->type != CP && tmp->type != ARG) && (err = 1))
	return (err);
}

int	catch_args(char *str, int *i)
{
	int		len;
	t_kof	fok;

	(1 && (len = -1) && init_kof(&fok) && ft_isspace(str[*i]) && (*i)++);
	while (str[*i + (++len)])
	{
		check_qp(&fok, str[(*i) + len]);
		if (fok.sq < 0 && fok.dq < 0 && ft_strchr("|&<>() ", str[*i + (len)]))
			break ;
	}
	return (len);
}

t_token	*tk_analyzer(char *str, int *i, int *shlvl)
{
	t_token	*node;
	int		type;
	int		len;

	type = ARG;
	len = 1;
	(str[(*i)] == '|' && str[(*i) + 1] != '|' && (type = PIPE));
	((str[(*i)] == '|') && (str[(*i) + 1] == '|') && (type = OR));
	((str[(*i)] == '&') && (str[(*i) + 1] == '&') && (type = AND));
	((str[(*i)] == '(') && (type = OP));
	((str[(*i)] == ')') && (type = CP));
	((str[(*i)] == '<') && (str[(*i) + 1] != '<') && (type = RDI));
	((str[(*i)] == '<') && (str[(*i) + 1] == '<') && (type = RDHD));
	((str[(*i)] == '>') && (str[(*i) + 1] != '>') && (type = RDO));
	((str[(*i)] == '>') && (str[(*i) + 1] == '>') && (type = RDAP));
	(type == ARG && (len = catch_args(str, i)));
	(type != ARG && (len += (type % 2)));
	(type == CP && ((*shlvl)--));
	node = tk_create(&str[(*i)], type, len, *shlvl);
	(type == OP && ((*shlvl)++));
	*i += len;
	return (node);
}

t_token	*tokenizer(char *str)
{
	int		i;
	int		shlvl;
	t_token	*tk;
	t_token	*node;

	(0 || (i = 0) || (shlvl = 0) || (tk = NULL));
	while (str[i] && i != -1)
	{
		node = tk_analyzer(str, &i, &shlvl);
		(str[i] && ft_isspace(str[i]) && i++);
		if (!node)
			return (tk_clean(&tk));
		tk_bk_addback((void **)&tk, (void *)node, TK);
	}
	node = tk_create(NULL, 0);
	if (check_tokens(&tk))
		return (tk_clean(&tk));
	return (tk);
}
