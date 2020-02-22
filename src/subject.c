#include "../headers/header.h"

/*Initialise la structure subjectList_t*/
subjectList_t *subjectInitialisation(void)
{
    subjectList_t *subjectList = malloc(sizeof(*subjectList));
    if (subjectList == NULL)
    {
        exit(EXIT_FAILURE);
    }
    subjectList->next = NULL;
    subjectList->subject = NULL;
    subjectList->nbr = 0;
    return subjectList;
}

/*permet de rentrer une note differente ( matiere / note / coeff ) à tous les eleves */
studentList_t *createNewSubjectForAllStudent(studentList_t *studentList)
{
    studentList_t *actualStudent = studentList;
    fprintf(stdout, "-+-+-+-+-+-+-+ Début createNewSubjectForAllStudent-+-+-+-+-+-+-+-+-\n");
    while (actualStudent->next)
    {
        fprintf(stdout, "Ajout d'une note à %s %s de la classe %s\n", actualStudent->student->firstname, actualStudent->student->lastname, actualStudent->student->promotion);

        actualStudent->student->subjectList = createNewSubject(actualStudent);
        fprintf(stdout, "Nom de matière -> %s / note -> %.2lf / coeff -> %d\n", actualStudent->student->subjectList->subject->name, actualStudent->student->subjectList->subject->note, actualStudent->student->subjectList->subject->scale);
        enterToContinue();
        if (actualStudent->next->next)
        {
            fprintf(stdout, "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
        }
        actualStudent = actualStudent->next;
    }
    fprintf(stdout, "-+-+-+-+-+-+-+ Fin createNewSubjectForAllStudent-+-+-+-+-+-+-+-+-\n");
    return studentList;
}

/*Récuperation des informations de la matière ajouté par l'utilisateur*/
subjectList_t *createNewSubject(studentList_t *studentList)
{
    if (studentList->student)
    {
        char *subjectName = malloc(SIZE_MAX * sizeof(char));
        if (subjectName == NULL)
        {
            exit(EXIT_FAILURE);
        }
        double subjectNote = verifDouble();
        double subjectScale = verifInt("Coefficiant de la matière : ");
        fgetsCheck(subjectName, "\nVeuillez rentrer le nom de la matière", SIZE_MAX);

        /* Création du nouvel élément */
        subject_t *subject = malloc(sizeof(*subject));
        if (subject == NULL)
        {
            exit(EXIT_FAILURE);
        }

        subject->name = malloc(SIZE_MAX * sizeof(char));
        strcpy(subject->name, subjectName);
        subject->note = subjectNote;
        subject->scale = subjectScale;

        subject->id = studentList->student->subjectList->nbr + 1;
        studentList->student->subjectList = addNewSubject(studentList->student->subjectList, subject);
        if (studentList->student->subjectList->subject)
        {
            studentList->student->subjectList->nbr = studentList->student->subjectList->subject->id;
        }
        free(subjectName);
        return studentList->student->subjectList;
    }
    else
    {
        printf("veuillez creer des étudiants !\n");
        return NULL;
    }
}

/*Ajoute la matière dans la structure subjectList_t en fonction des données de l'utilisateur*/
subjectList_t *addNewSubject(subjectList_t *subjectList, subject_t *subject)
{
    if (subjectList == NULL)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        subjectList_t *new = malloc(sizeof(subjectList_t));
        new->subject = subject;
        new->next = subjectList;
        return new;
    }
}

/*Supprime et libère dans la mémoire la première matière de subjectList_t d'un étudiant */
void deleteFirstSubject(subjectList_t *subjectList)
{
    if (subjectList == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (subjectList->subject != NULL)
    {
        subject_t *toDelete = subjectList->subject;
        free(toDelete->name);
        subjectList = subjectList->next;
        free(toDelete);
    }
}

/*Supprime et libère dans la mémoire toutes les matière de subjectList_t d'un étudiant*/
void deleteAllSubject(subjectList_t **subjectList)
{
    subjectList_t *actualSubject = *subjectList;
    while (actualSubject != NULL)
    {
        subjectList_t *temp = actualSubject;
        actualSubject = actualSubject->next;
        deleteFirstSubject(temp);
    }
    *subjectList = NULL;
}

/*Permet d'afficher toutes les notes et les informations d'un étudiant*/
void printListSubject(subjectList_t *subjectList)
{
    if (subjectList == NULL)
    {
        fprintf(stdout, "-+-+-+-+-+-+-+ Cet étudiant n'a pas de notes -+-+-+-+-+-+-+-+-\n");
    }
    else
    {
        if (subjectList->subject == NULL)
        {
            fprintf(stdout, "-+-+-+-+-+-+-+ Veuillez créer des notes -+-+-+-+-+-+-+-+-\n");
        }
        else
        {
            int *notes = malloc(subjectList->nbr * sizeof(int));
            int *scale = malloc(subjectList->nbr * sizeof(int));
            char *name = initDoubleChar(subjectList->nbr, SIZE_MAX);
            int i = 0;
            int z = 0;
            int exist = 0;
            int moyenneAll = 0;
            char **nameList = initDoubleChar(subjectList->nbr, SIZE_MAX);
            int **moyenneByName = initDoubleInt(subjectList->nbr, SIZE_MAX);
            int x=0;
            subjectList_t *actualSubject = subjectList;
            fprintf(stdout, "-+-+-+-+-+-+-+ Début printListSubject-+-+-+-+-+-+-+-+-\n");
            while (actualSubject->next)
            {
                notes[i] = actualSubject->subject->note;
                scale[i] = actualSubject->subject->scale;
                strcpy(name[i], actualSubject->subject->name);
                i++;
                fprintf(stdout, "id : %d , Nom de matière -> %s / note -> %.2lf / coeff -> %d\n", actualSubject->subject->id, actualSubject->subject->name, actualSubject->subject->note, actualSubject->subject->scale);
                if (actualSubject->next->next)
                {
                    fprintf(stdout, "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
                }
                actualSubject = actualSubject->next;
            }
            for (int j = 0; j < i; j++)
            {
                moyenneAll += notes[j] * scale[j];
                for (z = 0; z < j; z++)
                {
                    if (strcmp(name[z],nameList[z]) == 0)
                    {
                        exist = 1;
                        moyenneByName[z]+=notes[j] * scale[j];
                    }
                }
                if (exist == 0)
                {
                    fprintf(stdout, " name -> %s\n", subjectList->subject->name);
                    strcpy(nameList[z], name[j]);
                    x++;
                }
                exist = 0;
            }
            fprintf(stdout, "-+-+-+-+-+-+-+ Fin printListSubject-+-+-+-+-+-+-+-+-\n");
            freeDoubleChar(name, subjectList->nbr);
            freeDoubleInt(&moyenneByName, subjectList->nbr);
        }
    }
}
