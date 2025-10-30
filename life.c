#include "life.h"

int	readInput(char *input)
{
	int	nbytes;

	nbytes = read(0, input, BUFFSIZE);
	input[BUFFSIZE - 1] = '\0'; // NULL terminate just in case
	if (nbytes < 0)
		return (0);
	return (1);
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		putchar(str[i]);
		i++;
	}
}

char	**allocateTab(int width, int height)
{
	char	**tab;
	int		i;

	tab = malloc((height + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (i < height)
	{
		tab[i] = calloc(width + 1, sizeof(char));
		if (!tab[i])
			return (NULL);
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

void	blankTab(char **tab, int width, int height)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (i < height)
	{
		while (y < width)
		{
			tab[i][y] = ' ';
			y++;
		}
		y = 0;
		i++;
	}
}

void	printTab(char **tab, int height)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (i < height)
	{
		ft_putstr(tab[i]);
		putchar('\n');
		i++;
	}
}

void	initTab(char *input, char **tab, int width, int height)
{
	int	i;
	int	y;
	int	j;
	int	isWriting;

	i = 0;
	y = 0;
	j = 0;
	isWriting = 0;
	blankTab(tab, width, height);
	while (input[j])
	{
		if (input[j] == 'x')
			isWriting = !isWriting;
		if (input[j] == 'w' && (i - 1) >= 0) // UP == i - 1
			i = -1;
		if (input[j] == 's' && (i + 1) < height) // DOWN == i + 1
			i += 1;
		if (input[j] == 'a' && (y - 1) >= 0) // LEFT == y - 1
			y -= 1;
		if (input[j] == 'd' && (y + 1) < width) // RIGHT == y + 1
			y += 1;
		if (isWriting)
			tab[i][y] = 'O';
		j++;
	}
}

int	isAlive(char c)
{
	return (c == 'O');
}

void	copyTab(char **src, char **dest, int width, int height)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (i < height)
	{
		while (y < width)
		{
			dest[i][y] = src[i][y];
			y++;
		}
		y = 0;
		i++;
	}
}

void	freeTab(char **tab, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	canGoUp(int i)
{
	return (i - 1 >= 0);
}

int	canGoDown(int i, int height)
{
	return (i + 1 <= (height - 1));
}
int	canGoLeft(int y)
{
	return (y - 1 >= 0);
}
int	canGoRight(int y, int width)
{
	return (y + 1 <= (width - 1));
}
int	countNeighbors(char **tab, int width, int height, int i, int y)
{
	int	count = 0;

	if (canGoUp(i) && isAlive(tab[i - 1][y])) // UP
		count++;
	if (canGoDown(i, height) && isAlive(tab[i + 1][y])) // DOWN
		count++;
	if (canGoLeft(y) && isAlive(tab[i][y - 1])) // LEFT
		count++;
	if (canGoRight(y, width) && isAlive(tab[i][y + 1])) // RIGHT
		count++;
	if (canGoUp(i) && canGoLeft(y) && isAlive(tab[i - 1][y - 1])) // UP LEFT
		count++;
	if (canGoUp(i) && canGoRight(y, width) && isAlive(tab[i - 1][y + 1]))// UP RIGHT
		count++;
	if (canGoDown(i, height) && canGoLeft(y) && isAlive(tab[i + 1][y - 1]))// DOWN LEFT
		count++;
	if (canGoDown(i, height) && canGoRight(y, width) && isAlive(tab[i + 1][y + 1])) // DOWN RIGHT
		count++;
	return (count);
}

void	startLifeSimulation(char **tab, int width, int height, int iterations)
{
	int		i;
	int		y;
	int		j;
	char	**temp;
	int		neighbors;

	j = 0;
	temp = allocateTab(width, height);
	copyTab(tab, temp, width, height);
	while (j < iterations)
	{
		i = 0;
		while (i < height)
		{
			y = 0;
			while (y < width)
			{
				neighbors = countNeighbors(tab, width, height, i, y);
				if (isAlive(tab[i][y]) && !(neighbors == 2 || neighbors == 3))// if cell is alive and doesnt have 2 or 3 neighbors
					temp[i][y] = ' '; // kill cell
				if (!isAlive(tab[i][y]) && neighbors == 3) // if cell is dead and has exactly 3 neighbors
					temp[i][y] = 'O'; // recussitate cell
				y++;
			}
			i++;
		}
		copyTab(temp, tab, width, height);
		j++;
	}
	freeTab(temp, height);
}

int	main(int argc, char **argv)
{
	if (argc != 4)
		return (1);

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int iterations = atoi(argv[3]);
	if (width <= 0 || height <= 0)
		return (1);
	char **tab = allocateTab(width, height);
	if (!tab)
		return (1);
	// choix de la facilite car j'ai la flemme de malloc
	char input[BUFFSIZE] = {0}; // initialise with 0s
	if (!readInput(input))
		return (1);
	initTab(input, tab, width, height);
	startLifeSimulation(tab, width, height, iterations);
	printTab(tab, height);
	return (0);
}