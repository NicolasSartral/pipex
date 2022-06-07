#include "pipex.h"

void	first_command(t_struct *data)
{
	data->c = 2;
	parse_arguments(data);
	if (data->argz == NULL)
		p_exiting(data, "wrong command");
	if (data->path == NULL)
		p_exiting(data, "wrong first argument");
	data->fd_entry = open(data->argv[1], O_RDONLY);
	if (data->fd_entry == -1)
		p_exiting(data, "can't open input file");
	if (pipe(data->fd[data->c - 2]) == -1)
		p_exiting(data, "can't pipe");
	data->pid[data->c - 2] = fork();
	if (data->pid[data->c - 2] == 0)
	{
		dup2(data->fd_entry, STDIN_FILENO);
		//if ==-1 failed
		dup2(data->fd[data->c - 2][1], STDOUT_FILENO);
		close(data->fd_entry);
		close(data->fd[data->c - 2][1]);
		close(data->fd[data->c - 2][0]); //no close ?
		status(data);
		execve(data->path, data->argz, data->envp);
	}
	else 
	{
		close(data->fd_entry);
		close(data->fd[data->c - 2][1]);
		freeing_parsing(data);
	}
}

void	loop_command(t_struct *data)
{
	while (data->c < data->argc - 2)
	{
		data->c++;
		parse_arguments(data);
		if (data->argz == NULL)
			p_exiting(data, "wrong command");
		if (data->path == NULL)
			p_exiting(data, "wrong first argument");
		if (pipe(data->fd[data->c - 2]) == -1)
			p_exiting(data, "can't pipe");
		data->pid[data->c - 2] = fork();
		if (data->pid[data->c - 2] == 0)
		{
			dup2(data->fd[data->c - 3][0], STDIN_FILENO);
			dup2(data->fd[data->c - 2][1], STDOUT_FILENO);
			close(data->fd[data->c - 2][0]);
			close(data->fd[data->c - 2][1]);
			close(data->fd[data->c - 3][0]);
			execve(data->path, data->argz, data->envp);
			//if == -1 problems
		}
		else
		{
			close(data->fd[data->c - 2][1]);
			freeing_parsing(data);		
		}
	}
	//le tout dans une fonction qui retourne le fd actuel
}

void	last_command(t_struct *data)
{
	data->c = data->argc - 2;
	parse_arguments(data);
	if (data->argz == NULL)
		p_exiting(data, "wrong command");
	if (data->path == NULL)
		p_exiting(data, "wrong first argument");
	if (pipe(data->fd[data->c - 2]) == -1)
		p_exiting(data, "can't pipe");
	data->pid[data->c - 2] = fork();
	if (data->pid[data->c - 2] == 0)
	{
		dup2(data->fd[data->c - 3][0], STDIN_FILENO);
		dup2(data->fd[data->c - 2][1], STDOUT_FILENO);
		close(data->fd[data->c - 2][1]);
		close(data->fd[data->c - 2][0]);
		close(data->fd[data->c - 3][0]);
		execve(data->path, data->argz, data->envp);

	}
	else
	{
		outfiling(data);
		close(data->fd[data->c - 3][0]);
		close(data->fd[data->c - 2][1]);
		close(data->fd[data->c - 2][0]);
		close(data->fd[0][0]);
		close(data->fd[1][0]);
		close(data->fd[2][0]);
		close(data->fd[3][0]);
		close(data->fd[4][0]);
		close(data->fd[5][0]);
		close(data->fd_end);
		freeing_parsing(data);
	}
}
