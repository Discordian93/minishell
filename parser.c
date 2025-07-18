#include "minishell.h"

static void	advance(t_parser *parser)
{
	if (parser && parser->current && parser->current->type != TOKEN_EOF)
		parser->current = parser->current->next;
}

static bool	check(t_parser *parser, t_token_type type)
{
	if (!parser || !parser->current || !(parser->current->type == type))
		return (false);
	return (true);
}

static bool	match(t_parser *parser, t_token_type type)
{
	if (check(parser, type))
	{
		advance(parser);
		return (true);
	}
	return (false);
}
