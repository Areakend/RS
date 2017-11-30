#include "all.h"

void cut_buffer(char* buffer)
{
    // Servira à stocker chaque mot de la saisie utilisateur
    char oneword[20];
    // On stocke la saisie utilisateur
    char *token = buffer;
    // Index des différents tableaux
    int i = 0, i2 = 0, i3 = 0;

    for(i=0;i<strlen(token);i++)
    {
        switch(token[i])
        {
            case ' ':
                {
                    // On supprimme les espaces
                    i=i+1;
                    every_word[i2] = oneword;
                    i2=i2+1;
                };
                break;

            case '\0':
                {
                    // On supprimme les \0
                    i=i+1;
                    every_word[i2] = oneword;
                    i2=i2+1;
                };
                break;

            case '\n':
                {
                    // On supprimme les \n
                    i=i+1;
                    every_word[i2] = oneword;
                    i2=i2+1;
                };
                break;

            default :
                {
                    // On constitue le mot au complet
                    oneword[i3] = token[i];
                    i3=i3+1;
                    oneword[i3] = '\0';
                };
                break;
        }
    }
    every_word[i2] = NULL;
}


/*void inputParser(char *buffer) {
  char *buff = &buffer;
  char token[MAX_ARGS_SIZE];

  //Avancée dans le buffer
  int i;
  //Avancée dans le mot en cours
  int i2 = 0;
  //Avancée dans l'index de tableaux de mots
  int i3 = 0;

  for(i = 0; i < strlen(*buff); i++) {
    switch (*buff[i]) {
    case ' ':
        token[i2] = '\0'
        every_word[i3][] = *token;
        token = NULL;
        i2 = 0;
        i3++;
      break;
    case '\n':
      break;
    case '\0':
      break;
    default:
      {
        token[i2] = *(buff);
        i2++;
      }
      break;
    }
    i++;
  }
}
*/
