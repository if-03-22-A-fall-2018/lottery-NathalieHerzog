/*-----------------------------------------------------------------------------
 *				HTBLA-Leonding / Class: <your class name here>
 *-----------------------------------------------------------------------------
 * Exercise Number: #exercise_number#
 * File:			lottery.c
 * Author(s):		Peter Bauer
 * Due Date:		#due#
 *-----------------------------------------------------------------------------
 * Description:
 * Implementation of a library for analyzing lottery tips.
 *-----------------------------------------------------------------------------
*/

#include <stdio.h>
#include "lottery.h"

#define UUID_LEN 40
#define MAX_TIP_LEN 17
#define MAX_LINE_LEN (UUID_LEN + 1 + MAX_TIP_LEN + 1)

FILE *fp;
char sep;
int drawing[TIP_SIZE];

bool init_lottery(const char *csv_file, char csv_separator)
{
  fp = fopen(csv_file, "r");
  sep = csv_separator;

  if (fp != 0)
  {
    return true;
  }

  return false;
}

bool get_tip(int tip_number, int tip[TIP_SIZE])
{
  if (tip_number < 0)
  {
    return false;
  }

  char line[MAX_LINE_LEN];
  int num = 0;
  int tipIndex = 0;
  int start = 0;
  int count = 0;

  fseek(fp, 0, SEEK_SET);

  for (int i = 0; i <= tip_number; i++)
  {
    if (fgets(line, MAX_LINE_LEN, fp) == 0)
    {
      return false;
    }
  }

  for (int i = 0; i < MAX_LINE_LEN; i++)
  {
    if (count == 0 && (line[i] == sep || line[i] == '\0' || line[i] == '\n'))
    {
      start = i;
      count++;
    }
  }

  for (int i = start; i < MAX_LINE_LEN; i++)
  {
    if (line[i] != sep && line[i] != '\0' && line[i] != '\n')
    {
      if (line[i + 1] == sep || line[i + 1] == '\0' || line[i + 1] == '\n')
      {
        tip[tipIndex] = num + line[i] - '0';
        num = 0;
        tipIndex++;
      }
      else
      {
        num += line[i] - '0';
        num *= 10;
      }
    }
  }

  return true;
}

bool set_drawing(int drawing_numbers[TIP_SIZE])
{
  for (int i = 0; i < TIP_SIZE; i++)
  {
    if (drawing_numbers[i] < 1 || drawing_numbers[i] > 45)
    {
      return false;
    }

    drawing[i] = drawing_numbers[i];
  }

  return true;
}

int get_tip_result(int tip_number)
{
  if (drawing[0] == 0)
  {
    return -1;
  }

  int tip[TIP_SIZE];

  if (!get_tip(tip_number, tip))
  {
    return -2;
  }

  int count = 0;

  for (int u = 0; u < TIP_SIZE; u++)
  {
    for (int i = 0; i < TIP_SIZE; i++)
    {
      if (tip[u] == drawing[i])
      {
        count++;
      }
    }
  }

  return count;
}

int get_right_tips_count(int right_digits_count)
{
  if (right_digits_count > TIP_SIZE || right_digits_count < 0 || drawing[0] == 0)
  {
    return -1;
  }

  int count = 0;
  char line[MAX_LINE_LEN];

  fseek(fp, 0, SEEK_SET);

  for (int i = 0; i < 1000000; i++)
  {
    if (right_digits_count == get_tip_result(i))
    {
      count++;
    }
    if (fgets(line, MAX_LINE_LEN, fp) == 0)
    {
      return count;
    }
  }

  return count;
}