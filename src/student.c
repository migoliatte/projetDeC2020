#include "../headers/header.h"

/*Initialise la structure studentList_t*/
studentList_t *studentInitialisation(void)
{
  studentList_t *liste = malloc(sizeof(*liste));
  if (liste == NULL)
  {
    exit(EXIT_FAILURE);
  }
  liste->student = NULL;
  liste->next = NULL;
  liste->nbr = 0;
  return liste;
}

/*Récuperation des informations de l'étudiant ajouté par l'utilisateur*/
studentList_t *createNewStudent(studentList_t *studentList)
{
  char *studentFirstname = malloc(SIZE_MAX * sizeof(char));
  char *studentLastname = malloc(SIZE_MAX * sizeof(char));
  char *studentPromotion = malloc(SIZE_MAX * sizeof(char));
  if (studentFirstname == NULL || studentLastname == NULL || studentPromotion == NULL)
  {
    exit(EXIT_FAILURE);
  }

  fgetsCheck(studentFirstname, "Veuillez rentrer votre prénom", SIZE_MAX);

  fgetsCheck(studentLastname, "\nVeuillez rentrer votre nom de famille", SIZE_MAX);

  fgetsCheck(studentPromotion, "\nVeuillez rentrer votre promotion", SIZE_MAX);

  /* Création du nouvel élément */
  student_t *student = malloc(sizeof(*student));
  if (student == NULL)
  {
    exit(EXIT_FAILURE);
  }
  student->firstname = malloc(SIZE_MAX * sizeof(char));
  student->lastname = malloc(SIZE_MAX * sizeof(char));
  student->promotion = malloc(SIZE_MAX * sizeof(char));

  strcpy(student->firstname, studentFirstname);
  strcpy(student->lastname, studentLastname);
  strcpy(student->promotion, studentPromotion);
  student->subjectList = subjectInitialisation();

  student->id = studentList->nbr + 1;

  studentList = addNewStudent(studentList, student);
  if (studentList->student)
  {
    studentList->nbr = studentList->student->id;
  }
  free(studentFirstname);
  free(studentLastname);
  free(studentPromotion);
  return studentList;
}

/*Ajoute l'étudiant dans la structure studentList_t en fonction des données de l'utilisateur*/
studentList_t *addNewStudent(studentList_t *studentList, student_t *student)
{
  if (studentList == NULL)
  {
    // exit(EXIT_FAILURE);
    studentList->student = student;
    studentList->next = NULL;
    return studentList;
  }
  else
  {
    studentList_t *new = malloc(sizeof(studentList_t));

    new->student = student;
    new->next = studentList;
    return new;
  }
}

/*Supprime et libère dans la mémoire le premier étudiant de studentList*/
void deleteFirstStudent(studentList_t *studentList)
{
  if (studentList == NULL)
  {
    exit(EXIT_FAILURE);
  }

  if (studentList->student != NULL)
  {
    student_t *toDelete = studentList->student;
    free(toDelete->firstname);
    free(toDelete->lastname);
    free(toDelete->promotion);
    deleteAllSubject(&toDelete->subjectList);
    studentList = studentList->next;

    free(toDelete);
  }
}

/* Affichage de prenom/nom/id de tous les étudiants */
void printListStudent(studentList_t *studentList)
{
  if (studentList == NULL)
  {
    exit(EXIT_FAILURE);
  }
  if (studentList->student == NULL)
  {
    fprintf(stdout, "-+-+-+-+-+-+-+ Veuillez créer des étudiants -+-+-+-+-+-+-+-+-\n");
  }
  else
  {
    studentList_t *currentList = studentList;
    fprintf(stdout, "-+-+-+-+-+-+-+ Début printListStudent-+-+-+-+-+-+-+-+-\n");

    while (currentList->next)
    {
      fprintf(stdout, "prenom -> %s / nom -> %s / promotion -> %s / id : %d \n", currentList->student->firstname, currentList->student->lastname, currentList->student->promotion, currentList->student->id);
      if (currentList->next->next)
      {
        fprintf(stdout, "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
      }
      currentList = currentList->next;
    }
    fprintf(stdout, "-+-+-+-+-+-+-+ Fin printListStudent-+-+-+-+-+-+-+-+-\n");
  }
}

/*Permet de récuperer un étudiant dans la liste (s'il éxiste)*/
studentList_t *search(studentList_t *studentList, int etudiantId)
{
  studentList_t *currentList = studentList;
  while (currentList->next)
  {

    if (currentList->student->id == etudiantId)
    {
      return currentList;
    }
    else
    {
      currentList = currentList->next;
    }
  }
  printf("Eleve non trouvé\n");
  return NULL;
}

/*Permet d'afficher les informations de l'étudiant recherché*/
void displaySearchedStudent(studentList_t *studentList, int etudiantId)
{
  studentList_t *studentToDisplay = search(studentList, etudiantId);
  if (studentToDisplay)
  {
    printf("L'élève recherché est : %s %s %s\n", studentToDisplay->student->firstname, studentToDisplay->student->lastname, studentToDisplay->student->promotion);
    printListSubject(studentToDisplay->student->subjectList);
  }
}

/*Supprime un étudiant recherché*/
char *deleteSpecificItem(studentList_t **studentList, int etudiantId)
{
  if (!(*studentList)->student)
  {
    return "-+-+-+-+-+-+-+ Veuillez créer des étudiants -+-+-+-+-+-+-+-+-\n";
  }
  if ((*studentList)->student == NULL)
  {
    printf("veuillez créer des étudiants !\n");
  }
  studentList_t *studentToDelete = *studentList;
  studentList_t *tmp = NULL;

  while (studentToDelete->student->id != etudiantId && studentToDelete->next != NULL)
  {
    tmp = studentToDelete;
    studentToDelete = studentToDelete->next;
  }

  if (studentToDelete->student->id == etudiantId)
  {
    if (tmp)
    {
      tmp->next = studentToDelete->next;
    }
    else
    {
      *studentList = studentToDelete->next;
    }
    free(studentToDelete->student->firstname);
    free(studentToDelete->student->lastname);
    free(studentToDelete->student->promotion);
    deleteAllSubject(&studentToDelete->student->subjectList);
    free(studentToDelete->student);
    free(studentToDelete);
    return "Eleve supprimé";
  }
  return "NOPE";
}

/*Permet d'afficher toutes les informations nécessaires pour un étudiant*/
void allTypeOfDisplayStudent(studentList_t *studentList, char *namePromotion, int type)
{
  char **nameOfPromotion = initDoubleChar(studentList->nbr, SIZE_MAX);
  int exist = 0;
  int i = 0;
  int j = 0;
  double note = 0;
  int nbr = 0;

  if (studentList == NULL)
  {
    exit(EXIT_FAILURE);
  }

  if (studentList->student == NULL)
  {
    fprintf(stdout, "-+-+-+-+-+-+-+ Veuillez créer des étudiants -+-+-+-+-+-+-+-+-\n");
  }
  else
  {
    studentList_t *currentList = studentList;
    fprintf(stdout, "-+-+-+-+-+-+-+ Début printListStudent-+-+-+-+-+-+-+-+-\n");

    while (currentList->next)
    {
      if (type == 1)
      {
        fprintf(stdout, "prenom -> %s / nom -> %s / promotion -> %s / id : %d \n", currentList->student->firstname, currentList->student->lastname, currentList->student->promotion, currentList->student->id);
      }
      else if (type == 2)
      {
        for (i = 0; i < studentList->nbr; i++)
        {
          if (strcmp(nameOfPromotion[i], currentList->student->promotion) == 0)
          {
            exist = 1;
          }
        }
        if (exist == 0)
        {
          fprintf(stdout, " promotion -> %s\n", currentList->student->promotion);
          strcpy(nameOfPromotion[j], currentList->student->promotion);
          j++;
        }
        exist = 0;
      }
      else if (type == 3)
      {
        if (strcmp(currentList->student->promotion, namePromotion) == 0)
        {
          fprintf(stdout, "prenom -> %s / nom -> %s / promotion -> %s / id : %d \n", currentList->student->firstname, currentList->student->lastname, currentList->student->promotion, currentList->student->id);
          if (currentList->student->subjectList)
          {
            subjectList_t *actualSubject = currentList->student->subjectList;
            while (actualSubject->next)
            {
              note += actualSubject->subject->note * actualSubject->subject->scale;
              nbr += 1 * actualSubject->subject->scale;
              actualSubject = actualSubject->next;
            }
          }
        }
      }
      if (currentList->next->next)
      {
        fprintf(stdout, "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
      }
      currentList = currentList->next;
    }
    if (type == 3)
    {
      if (note)
      {
        fprintf(stdout, "moyenne de la promotion -> %.2lf \n", note / nbr);
      }
    }

    fprintf(stdout, "-+-+-+-+-+-+-+ Fin printListStudent-+-+-+-+-+-+-+-+-\n");
  }
  freeDoubleChar(&nameOfPromotion, studentList->nbr);
}

/*Permet l'affichage d'un étudiant pour le modifier par la suite*/
void displaySearchedStudentForModification(studentList_t *studentList, int etudiantId)
{
  studentList_t *studentToDisplay = search(studentList, etudiantId);
  int choice;
  if (studentToDisplay)
  {
    printf("1) Le Prénom de l'Etudiant est : %s \n", studentToDisplay->student->firstname);
    printf("2) Le Nom de l'Etudiant est : %s \n", studentToDisplay->student->lastname);
    printf("3) Sa promotion : %s  \n", studentToDisplay->student->promotion);
    printf("4) Pour modifier ses notes \n");
    choice = verifInt("Que voulais vous modifier entrez votre choix qui doit être une des nombres de la list : \n");
    enterToContinue();
    modificationList(&studentToDisplay, choice);
  }
}

/*Affiche la liste des modifications possibles*/
char *modificationList(studentList_t **studentList, int choice)
{

  char *replace = malloc(SIZE_MAX * sizeof(char));

  studentList_t *studentModif = *studentList;
  if (studentModif->student == NULL)
  {
    return "Il n'y a pas d'étudiants";
  }
  if (choice == 1)
  {

    fgetsCheck(replace, "Le nouveau Prénom de l'Eleve : \n", SIZE_MAX);
    strcpy(studentModif->student->firstname, replace);
    printf("Le prenom a été modifié \n");
  }
  else if (choice == 2)
  {

    fgetsCheck(replace, "Le nouveau Nom de l'Eleve : \n", SIZE_MAX);
    strcpy(studentModif->student->lastname, replace);
    printf("Le nom a été modifié \n");
  }
  else if (choice == 3)
  {

    fgetsCheck(replace, "Le nouveau Promo de l'Eleve : \n", SIZE_MAX);
    strcpy(studentModif->student->promotion, replace);
    printf("La pormo a été modifié \n");
  }
  else if (choice == 4)
  {
    printListSubjectForModification(studentModif->student->subjectList);
  }

  return "fait";
}

/*Permet d'ajouter une note à tous les élèves d'une promotion, élève par élève*/
void insertNoteForOnePromotion(studentList_t *studentList, char *namePromotion)
{
  char **nameOfPromotion = initDoubleChar(studentList->nbr, SIZE_MAX);
  if (studentList == NULL)
  {
    exit(EXIT_FAILURE);
  }

  if (studentList->student == NULL)
  {
    fprintf(stdout, "-+-+-+-+-+-+-+ Veuillez créer des étudiants -+-+-+-+-+-+-+-+-\n");
  }
  else
  {
    studentList_t *currentList = studentList;
    fprintf(stdout, "-+-+-+-+-+-+-+ Début d'ajout de note pour une promotion-+-+-+-+-+-+-+-+-\n");

    while (currentList->next)
    {

      if (strcmp(currentList->student->promotion, namePromotion) == 0)
      {
        fprintf(stdout, "prenom -> %s / nom -> %s / promotion -> %s / id : %d \n", currentList->student->firstname, currentList->student->lastname, currentList->student->promotion, currentList->student->id);
        createNewSubject(currentList->student);
      }

      if (currentList->next->next)
      {
        fprintf(stdout, "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
      }
      currentList = currentList->next;
    }
    fprintf(stdout, "-+-+-+-+-+-+-+ Fin printListStudent-+-+-+-+-+-+-+-+-\n");
  }
  freeDoubleChar(&nameOfPromotion, studentList->nbr);
}