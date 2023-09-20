#include "main.h"

/**
 * _strlen - Calculates the length of a string.
 *
 * @s: The input string.
 *
 * Return: The length of the string (excluding the null terminator).
 */
int _strlen(const char *s)
{
	int length = 0;

	if (!s)
		return (length);
	for (length = 0; s[length]; length++)
		;
	return (length);
}

/**
 * _strcpy - Copies the string pointed by src to dest.
 *
 * @dest: The destination buffer where the string is to be copied.
 * @src: The source string to be copied.
 *
 * Return: A pointer to the destination string (dest).
 */
char *_strcpy(char *dest, const char *src)
{
	size_t m_m;

	for (m_m = 0; src[m_m] != '\0'; m_m++)
		dest[m_m] = src[m_m];
	dest[m_m] = '\0';
	return (dest);
}

/**
 * _strcat - Concatenates the src string to the dest string.
 *
 * @dest: The destination buffer where the src string is to be appended.
 * @src: The source string to be appended.
 *
 * Return: A pointer to the destination string (dest).
 */
char *_strcat(char *dest, const char *src)
{
	char *destTemp;
	const char *srcTemp;

	destTemp = dest;
	srcTemp = src;

	while (*destTemp != '\0')
		destTemp++;

	while (*srcTemp != '\0')
		*destTemp++ = *srcTemp++;
	*destTemp = '\0';
	return (dest);
}

/**
 * _strncat - Concatenates at most n characters of src to dest.
 *
 * @dest: The destination buffer where the src string is to be appended.
 * @src: The source string to be appended.
 * @n: The maximum number of characters to append.
 *
 * Return: A pointer to the destination string (dest).
 */
char *_strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = _strlen(dest);
	size_t m_m;

	for (m_m = 0; m_m < n && src[m_m] != '\0'; m_m++)
		dest[dest_len + m_m] = src[m_m];
	dest[dest_len + m_m] = '\0';

	return (dest);
}
