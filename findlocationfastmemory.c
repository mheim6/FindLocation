////////////////////////////////////////////
////  Team:Monica Heim
////	Machine: Linux-18
////	Programming Assignment 2
////	CSCE 321 - Operating Systems
////	10/7/19
////	Fall 2019
////	C. Lauter
////////////////////////////////////////////

#include <sys/mman.h> // mmap
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // open, lseek
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h> // read, write, lseek, close
#define SSIZE_MAX 2147483648 // byte size

/*
  Program: findlocationfastmemory
  Description: This program is a tool that always takes two arguments:
    (1) a filename and (2) a 6-digit numerical prefix - it opens the file
    and performs a binary search on it to find the location identified
    by the prefix specified by the second argument. It then maps it to
    the file.
  Execution: If a corresponding line from the file is found, then the
    location from the line is output to standard output, stripping the
    spaces at the end and the prefix at the beginning
  Complexity: O(log n) - binary search
*/

int checkinput(char input[])
{
  for(int i = 0; i < 6; i++)
  {
    if(input[i] < '0' || input[i] > '9')
    {
      return 1; // not numerical input from '0' to '9'
    }
  }

  return 0;
}

int checklength(char *s)
{
  int i = 0;
  char c = s[i];
  while(c != '\0')
  {
    i++;
    c = s[i];
  }

  return i;
}

int cmpstr(char s1[], char s2[])
{
  for(int i = 0; ; i++)
  {
    if(s1[i] != s2[i])
      return s1[i] < s2[i] ? -1 : 1;
    if(s1[i] == '\0')
      return 0;
  }
}

int main(int argc, char *argv[])
{
  size_t fd, end_file;
  int upper, lower, mid;
  int target = 0, end = 0;
  off_t offset;

  // checking argument conditions
  if(argc != 3)
  {
    write(2, "Error! Invalid number of arguments!\n", 0);
    exit(1);
  }

  fd = open(argv[1], O_RDONLY); // open the file

  // checking prefix argument
  if(checkinput(argv[2]) != 0)
  {
    write(2, "Error! Prefix argument not well-formed!\n", 0);
    close(fd);
    exit(1);
  }

  // checking prefix length
  if(checklength(argv[2]) != 6)
  {
    write(2, "Error! Prefix argument not correct length!\n", 0);
    close(fd);
    exit(1);
  }

  // checking if file exists
  if(fd == -1)
  {
    write(2, "Error! File not found!\n", 0);
    close(fd);
    exit(1);
  }

  // checking if file is seekable from beginning to end
  if((offset = lseek(fd, 0, SEEK_END)) == -1)
  {
    write(2, "Error! File not seekable!\n", 0);
    close(fd);
    exit(1);
  }

  end_file = (int)offset;

  // checking if end of the file is at most 2147483648 bytes
  if(end_file > SSIZE_MAX)
    end_file = SSIZE_MAX;

  lower = 0; // set lowerbound to the beginning of the file
  upper = end_file; // set upperbound to the end of the file
  offset = lseek(fd, (end_file / 2) - (offset % 32), SEEK_SET);
  mid = (int)offset; // set the middle of the file to the offset

  // map file region to memory
  char *region = mmap(NULL, offset, PROT_READ, MAP_PRIVATE, fd, 0);

  while(target == 0 && end == 0)
  {

    offset = lseek(fd, -6, SEEK_CUR);

    if(cmpstr(region[mid], argv[2]) == 0) // compare - itself
    {
      target = 1;
      break;
    }
    if(upper - lower <= 32)
    {
      end = 1;
      break;
    }
    if(cmpstr(region[mid], argv[2]) > 0) // compare - prefix is smaller
    {
      upper = offset;
      mid = (upper - lower) / 2;
      mid = mid - (mid % 32);
      offset = lseek(fd, mid, SEEK_SET);
    }
    if(cmpstr(region[mid], argv[2]) < 0) // compare - prefix is bigger
    {
      lower = offset;
      mid = (upper - lower) / 2;
      mid = mid - (mid % 32);
      mid = mid + lower;
      offset = lseek(fd, mid, SEEK_SET);
    }
  }

  // prefix found in file
  if(target == 1)
  {
    char result[26];
    int letters = 26;
    offset = lseek(fd, 6, SEEK_CUR);

    // strip trailing spaces and leave only the 6-digit prefix
    for(int i = 24; i > 0; i--)
    {
      if(result[i] == ' ')
        letters--;
    }
    result[letters] = '\n';
    munmap(region, offset);
    close(fd);
    exit(0); // sucessful
  }

  // prefix not in file
  else
  {
    munmap(region, offset);
    close(fd);
    exit(1); // unsucessful
  }

  return 0;
}
