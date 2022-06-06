#include "pipex.h"

//access(..., F_OK or R_OK)

void	first_command(t_struct *dd)
{
	dd->c = 2;
	parse_arguments(dd);
	if (dd->argz == NULL)
		exiting(dd, "wrong command");
	if (dd->path == NULL)
		exiting(dd, "wrong first argument");
	dd->fd_infile = open(dd->argv[1], O_RDONLY);
	if (dd->fd_infile == -1)
		exiting(dd, "can't open input file");
	if (pipe(dd->fd_one) == -1)
		exiting(dd, "can't pipe");
	if (forking(dd, dd->fd_infile, dd->fd_one[1]) == 0)
		exiting(dd, "first fork failure");
	waitpid(dd->pid_one, 0, 0);
	close(dd->fd_infile);
	close(dd->fd_one[1]);
	freeing(dd);
}

void	last_command(t_struct *dd)
{
	dd->c = 3;
	parse_arguments(dd);
	if (dd->argz == NULL)
		exiting(dd, "wrong command");
	if (dd->path == NULL)
		exiting(dd, "wrong first argument");
	if (pipe(dd->fd_two) == -1)
		exiting(dd, "can't pipe");
	if (forking(dd, dd->fd_one[0], dd->fd_two[1]) == 0)
		exiting(dd, "second fork failure");
	waitpid(dd->pid_two, 0, 0);
	close(dd->fd_one[0]);
	close(dd->fd_two[1]);
	outfiling(dd);
	freeing(dd);
}

int	forking(t_struct *dd, int fd_in, int fd_out)
{
	dd->pid_two = fork();
	if (dd->pid_two == -1)
		return (0);
	if (dd->pid_two == 0)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
			dd->error = 1;
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			dd->error = 1;
		close(fd_in);
		close(fd_out);
		if (dd->error == 1)
			return (0);
		if (execve(dd->path, dd->argz, dd->envp) == -1)
			dd->error = 1;
	}
	if (dd->error == 1)
		return (0);
	return (1);
}
