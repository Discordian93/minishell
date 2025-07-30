#include "minishell.h"

t_parser	*parser_init(t_token *tokens)
{	
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->tokens = tokens;
	parser->current = tokens;
	parser->error = false;
	parser->error_msg = NULL;
	return (parser);
}

static void	advance(t_parser *parser)
{
	if (parser && parser->current && parser->current->type != TOKEN_EOF)
		parser->current = parser->current->next;
}

static bool	check(t_parser *parser, t_token_type type)
{
	if (!parser->current)
		return (false);
	return (parser->current->type == type);
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

static void	set_error(t_parser *parser, const char *msg)
{
	parser->error = true;
	parser->error_msg = ft_strndup(msg, ft_strlen(msg));
}

static	t_ast_node	*create_ast_node(t_ast_type type)
{
	t_ast_node *node;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	return node;
}

/* Check if current token is a redirection */
static bool is_redirection(t_parser *parser)
{
	return parser->current && 
	       (check(parser, TOKEN_REDIRECT_IN) ||
	        check(parser, TOKEN_REDIRECT_OUT) ||
	        check(parser, TOKEN_HEREDOC) ||
	        check(parser, TOKEN_APPEND));
}

/* Parse redirection type and advance parser */
static t_redir_type	parse_redir_type(t_parser *parser)
{
	t_redir_type type;
	
	if (check(parser, TOKEN_REDIRECT_IN))
		type = REDIR_IN;
	else if (check(parser, TOKEN_REDIRECT_OUT))
		type = REDIR_OUT;
	else if (check(parser, TOKEN_HEREDOC))
		type = REDIR_HEREDOC;
	else if (check(parser, TOKEN_APPEND))
		type = REDIR_APPEND;
	else
		return -1;
	advance(parser);
	return type;
}

static t_redirect *parse_redirections(t_parser *parser)
{
	t_redirect *head;
	t_redirect *tail;;
	t_redirect *redir;

	while (is_redirection(parser))
	{
		redir = malloc(sizeof(t_redirect));
		if (!redir)
			return (NULL);
		redir->type = parse_redir_type(parser);
		if (!check(parser, TOKEN_WORD))
		{
			set_error(parser, "Expected filename after redirection");
			free(redir);
			return (NULL);
		}
		redir->file = ft_strndup(parser->current->value, ft_strlen(parser->current->value));
		advance(parser);
		if (!head)
			head = redir;
		else
			tail->next = redir;
		tail = redir;
	}
	return (head);
}

static int	count_args(t_parser *parser)
{
   t_token	*token;
   int		count;

   count = 0;
   token = parser->current;
   while (token && token->type == TOKEN_WORD)
   {
   	count++;
   	token = token->next;
   	if (token && (token->type == TOKEN_REDIRECT_IN ||
   				token->type == TOKEN_REDIRECT_OUT ||
   				token->type == TOKEN_HEREDOC ||
   				token->type == TOKEN_APPEND ||
   				token->type == TOKEN_PIPE ||
   				token->type == TOKEN_AND ||
   				token->type == TOKEN_OR ||
   				token->type == TOKEN_RPAREN))
   		break;
   }
   return count;
}

static t_ast_node *parse_command(t_parser *parser)
{
	t_ast_node *node;
	int argc;
	int i;

	i = 0;
	if(!check(parser, TOKEN_WORD))
		return (NULL);
	node = create_ast_node(AST_COMMAND);
	if (!node)
		return (NULL);
	argc = count_args(parser);
	node->u_data.s_command.argv = malloc((argc + 1) * sizeof(char *));
	if (!node->u_data.s_command.argv)
		return (freeandnull(node));
	while (parser->current && parser->current->type == TOKEN_WORD && i < argc)
	{
		node->u_data.s_command.argv[i++] = ft_strndup(parser->current->value, ft_strlen(parser->current->value));
		advance(parser);
	}
	node->u_data.s_command.argv[i] = NULL;
	node->u_data.s_command.redirects = parse_redirections(parser);
	return (node);
}