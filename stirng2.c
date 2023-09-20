#include "main.h"

/**
 * _strchr - Locates the first occurrence of a character in a string.
 * @s: The string to be searched.
 * @c: The character to be located.
 *
 * Return: A pointer to the first occurrence of the character @c in the
 * string @s, or NULL if the character is not found.
 */
char *_strchr(char *s, char c)
{
	int index;

	for (index = 0; s[index]; index++)
	{
		if (s[index] == c)
			return (s + index);
	}

	return (NULL);
}

/**
 * _strspn - Gets the length of a prefix substring.
 * @s: The string to be searched.
 * @accept: The string containing acceptable characters.
 *
 * Return: The number of bytes in the initial segment of @s which
 * consist only of bytes from @accept.
 */
int _strspn(char *s, char *accept)
{
	int bytes = 0;
	int index;

	while (*s)
	{
		for (index = 0; accept[index]; index++)
		{
			if (*s == accept[index])
			{
				bytes++;
				break;
			}
		}
		s++;
	}
	return (bytes);
}

/**
 * _strcmp - Compares two strings.
 * @s1: The first string to be compared.
 * @s2: The second string to be compared.
 *
 * Return: An integer less than, equal to, or greater than zero if @s1 is found,
 * respectively, to be less than, to match, or to be greater than @s2.
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	if (*s1 != *s2)
		return (*s1 - *s2);

	return (0);
}

/**
 * _strncmp - Compares two strings up to a given number of bytes.
 * @s1: The first string to be compared.
 * @s2: The second string to be compared.
 * @n: The maximum number of bytes to compare.
 *
 * Return: An integer less than, equal to, or greater than zero if the first @n
 * bytes of @s1 is found, respectively, to be less than, to match, or to be
 * greater than the first @n bytes of @s2.
 */
int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t m_m;

	for (m_m = 0; s1[m_m] && s2[m_m] && m_m < n; m_m++)
	{
		if (s1[m_m] > s2[m_m])
			return (s1[m_m] - s2[m_m]);
		else if (s1[m_m] < s2[m_m])
			return (s1[m_m] - s2[m_m]);
	}
	if (m_m == n)
		return (0);
	else
		return (-15);
}
