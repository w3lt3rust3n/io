/*
   IO - Note Encryptor | io_encryptutils.c
   ------------------------------------------------------------------------------
   p34c3w41k3r <p34c3w41k3r@protonmail.ch>

   Copyright 2016 - Philentropy

   IO's is free software: you can redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the License,
   or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ------------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>
#include <gcrypt.h>
#include <sys/wait.h>
#include <sys/types.h>

extern char **environ;

#define CLI_ARG 2

char fname_enc[25] = {"\0"};

int encrypt_note(char *fname)
{
  pid_t encrypt_proc;
  //  char *encryptNote = NULL;
  char *encrypt_arg[] = {"/usr/bin/gpg", "--batch", "--no-tty", "-c", fname, NULL};

  do
    {
      encrypt_proc = fork();
    }
        while(encrypt_proc == -1 && (errno == EAGAIN));

        if(encrypt_proc == 0)
        {
          execve("/usr/bin/gpg", encrypt_arg, environ);
        }
        else
        {
          if(wait(NULL) == -1)
            {
              perror("wait:");
              return 1;
            }
        }

        printf("\033[32m[INFO]\033[0m: The note is encrypted.\n");

        if(remove(fname) == -1)
        {
                perror("remove:");
                return 1;
        }
        else
        {
                printf("\033[32m[INFO]\033[0m: The uncrypted note file is correctly deleted.\n");
        }
        return 0;
}

int decrypt_note(char *fname)
{
        FILE *fp_encryptNote;
        pid_t decrypt_proc;
        char ext_io[4] = ".io";

        //printf("fname_enc -> %s\n", fname_enc);
        strcpy(fname_enc, fname);
        strcat(fname_enc, ext_io);
        //printf("fname_enc must be-> %s\n", fname_enc);

        char *decrypt_arg[] = {"gpg", "-q", "-o", fname_enc, "-d", fname, NULL};

        if((fp_encryptNote = fopen(fname, "r")) == NULL)
          {
          /*fprintf(stderr, "\033[31m[ERROR]:\033[0m IO failed to open the encrypted note.\n"
             "%s.\n", strerror(errno));*/
            g_error("IO failed to open the encrypted note.");
            return 1;
          }
          else
          {
            do
              {
                decrypt_proc = fork();
              }
                while(decrypt_proc == -1 && (errno == EAGAIN));

                if(decrypt_proc == 0)
                {
                        execv("/usr/bin/gpg", decrypt_arg);
                }
                else
                {
                        if(wait(NULL) == -1)
                        {
                                perror("wait:");
                                return 1;
                        }
                }
        }
        fclose(fp_encryptNote);
        return 0;
}
