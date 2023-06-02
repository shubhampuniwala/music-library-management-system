// double-ended linked list
// The program reads song details from a file named "song.txt" and updates the file whenever a song is added or deleted.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*a structure called node is defined. This structure represents a single song in the playlist and contains the following fields:

song: a character array to store the name of the song.
movie: a character array to store the name of the movie the song belongs to.
artist: a character array to store the name of the artist who performed the song.
filepath: a character array to store the file path of the song.
like: a boolean value (true or false) indicating whether the song is liked or not.
prev and next: pointers to the previous and next nodes in the linked list.*/

int count = 0;
struct node
{
    char song[50];
    char movie[50];
    char artist[50];
    char filepath[100];
    bool like;
    struct node *prev;
    struct node *next;
} *start, *new, *tail, *del;

/*Function enterdata that takes a pointer to a node structure and a string details as input.
This function is responsible for parsing the details of a song and assigning them to the corresponding fields of the node structure.*/

void enterdata(struct node *ptr, char details[])
{
    char *cp, word[] = "\t";
    cp = strtok(details, word);
    for (int i = 0; i <= 4; i++)
    {
        if (i == 0)
        {
            strcpy(ptr->song, cp);
        }
        else if (i == 1)
        {
            strcpy(ptr->movie, cp);
        }
        else if (i == 2)
        {
            strcpy(ptr->artist, cp);
        }
        else if (i == 3)
        {
            strcpy(ptr->filepath, cp);
        }
        else if (i == 4)
        {
            int value = atoi(cp);
            ptr->like = value;
        }
        cp = strtok(NULL, word);
    }
}

/*Function createlist that reads song details from a file named "song.txt" and creates a double-ended linked list of songs.
It opens the file in read mode, reads each line, and uses the enterdata function to parse and assign the details to a new node structure.
The newly created node is then added to the linked list. The file is closed after reading all the songs.*/

void createlist()
{
    // create double ended linked list
    FILE *fp;
    fp = fopen("song.txt", "r");
    char detail[250], *cp, word[] = " ";
    // Move file pointer to the end of the file
    fseek(fp, 0L, SEEK_END);
    // Get current position of file pointer
    long size = ftell(fp);
    if (!feof(fp) && size != 0)
    {
        fseek(fp, 0L, SEEK_SET);
        // add starting node
        new = (struct node *)malloc(sizeof(struct node));
        fgets(detail, 150, fp);
        // printf("%s",detail);
        enterdata(new, detail);
        new->prev = NULL;
        new->next = NULL;
        start = new;
        tail = new;
    }
    while (!feof(fp) && start != NULL)
    {
        // add next node
        new = (struct node *)malloc(sizeof(struct node));
        fgets(detail, 150, fp);
        // printf("%s",detail);
        enterdata(new, detail);
        new->prev = tail;
        tail->next = new;
        tail = new;
    }
    fclose(fp);
}

/* Function updatefile that updates the contents of the "song.txt" file based on the current state of the linked list.
It opens the file in write mode and iterates through each node in the list, writing its details to the file. Finally, it closes the file.*/

void updatefile()
{
    // update content of file after adding or deleting song.
    FILE *fp;
    fp = fopen("song.txt", "w");
    char detail[250];
    struct node *temp = start;
    while (temp != NULL)
    {
        fprintf(fp, "%s\t%s\t%s\t%s\t%d", temp->song, temp->movie, temp->artist, temp->filepath, temp->like);
        if (temp->next != NULL)
        {
            fprintf(fp, "\n");
        }
        temp = temp->next;
    }
    fclose(fp);
}

/*The program defines a function displaylist that displays the songs in the linked list.
It iterates through each node and prints its song details, including the song name, movie name, artist name, and whether it is liked or not.*/

void displaylist()
{
    struct node *temp = start;
    count = 1;
    printf("Sr NO.  %-30s %-30s %-20s %10s\n", "Song", "Movie", "Artist", "LIKED SONGS");
    while (temp != NULL)
    {
        printf("%4d    %-30s %-30s %-20s ", count, temp->song, temp->movie, temp->artist);
        if (temp->like == 1)
        {
            printf("%10s\n", "LIKED");
        }
        else
        {
            printf("%10s\n", "UNLIKED");
        }
        temp = temp->next;
        count++;
    }
}

/* Function getdata that takes a pointer to a node structure and prompts the user to enter the details of a new song.
The user is asked to provide the song name, movie name, artist name, file path, and whether they want to add the song to the liked songs list.*/

void getdata(struct node *ptr)
{
    char s[10];
    gets(s);
    printf("Enter Song Name:");
    gets(ptr->song);
    printf("Enter movie Name:");
    gets(ptr->movie);
    printf("Enter artist Name:");
    gets(ptr->artist);
    printf("Enter filepath :");
    gets(ptr->filepath);
    printf("Do you want to add this song in liked song list (0/1):");
    scanf("%d", &ptr->like);
}

/*Function addsong that adds a new song to the linked list.
It creates a new node structure, calls the getdata function to get the song details from the user, and adds the new node at the beginning of the list.*/

void addsong()
{
    new = (struct node *)malloc(sizeof(struct node));
    getdata(new);
    new->prev = NULL;
    if (start != NULL)
    {
        new->next = start;
        start->prev = new;
        start = new;
    }
    else
    {
        new->next = NULL;
        start = new;
    }
    printf("Song added");
}

/*function deletesong that deletes a song from the linked list.
It prompts the user to enter the serial number of the song they want to delete, then it finds the corresponding node in the list and removes it from the list.
If the deleted node was the start node, the start pointer is updated accordingly.*/

void deletesong()
{
    displaylist();
    int n;
    printf("enter Sr no. of song that you want to delete:");
    scanf("%d", &n);
    struct node *temp = start;
    // delete start node
    if (n >= count || n < 1)
    {
        printf("\n\nInvalid Choice\n");
    }
    else if (n == 1)
    {
        temp->next->prev = NULL;
        start = temp->next;
        free(temp);
        printf("Song Deleted");
    }
    else
    {
        while (n - 2 > 0)
        {
            temp = temp->next;
            n--;
        }
        del = temp->next;
        // delete last node
        if (del->next == NULL)
        {
            temp->next = NULL;
            free(del);
        }
        // delete node in between of list
        else
        {
            temp->next = temp->next->next;
            temp->next->prev = temp->next;
            free(del);
        }
        printf("Song Deleted");
    }
}

/*Function print_distinct that prints distinct movie or artist names from the linked list.
It takes a string cho as input, which specifies whether to print distinct movies or artists.
It iterates through the list and checks if the movie or artist of the current node has already been printed. If not, it prints the name.*/

void print_distinct(char cho[])
{
    struct node *temp = start;
    struct node *curr = start;
    while (curr != NULL)
    {
        if (cho == "movie")
        {
            int found = 0;
            struct node *temp = start;
            while (temp != curr)
            {
                if (strcmp(temp->movie, curr->movie) == 0)
                {
                    found = 1;
                    break;
                }
                temp = temp->next;
            }
            if (!found)
            {
                printf("%s\n", curr->movie);
            }
            curr = curr->next;
        }
        else
        {
            int found = 0;
            struct node *temp = start;
            while (temp != curr)
            {
                if (strcmp(temp->artist, curr->artist) == 0)
                {
                    found = 1;
                    break;
                }
                temp = temp->next;
            }
            if (!found)
            {
                printf("%s\n", curr->artist);
            }
            curr = curr->next;
        }
    }
}

/*Function filter that filters songs based on the user's choice of filtering by movie or artist.
It prompts the user to enter their choice and then calls the print_distinct function to print the distinct movies or artists from the list.
The user is then asked to enter their specific choice, and the program displays the songs that match the chosen movie or artist.*/

void filter()
{
    int n, c = 1;
    char choice[50];
    printf("\n1. Filter By Movie\n");
    printf("2. Filter By Artist\n");
    printf("Enter your choice: ");
    scanf("%d", &n);
    if (n == 1)
    {
        print_distinct("movie");
        printf("Enter your choice:");
        gets(choice);
        gets(choice);
        struct node *temp = start;
        printf("Sr NO.  %-30s %-30s %-20s\n", "Song", "Movie", "Artist\n");
        while (temp != NULL)
        {
            if (strcmp(temp->movie, choice) == 0)
            {
                printf("%4d    %-30s %-30s %-20s \n", c, temp->song, temp->movie, temp->artist);
                c += 1;
                temp = temp->next;
            }
            temp = temp->next;
        }
    }
    else
    {
        print_distinct("artist");
        printf("Enter your choice: ");
        gets(choice);
        gets(choice);
        struct node *temp = start;
        printf("Sr NO.  %-30s %-30s %-20s\n", "Song", "Movie", "Artist\n");
        while (temp != NULL)
        {
            if (strcmp(temp->artist, choice) == 0)
            {
                printf("%4d    %-30s %-30s %-20s \n", c, temp->song, temp->movie, temp->artist);
                c += 1;
            }
            temp = temp->next;
        }
    }
}

/*the main function is defined, which serves as the entry point of the program.
It starts by calling the createlist function to initialize the linked list from the "song.txt" file.
It then presents a menu to the user, allowing them to add a song, delete a song, display the list, filter songs, or exit the program.
The user's choice is read from the input, and the corresponding function is called.
The menu is displayed repeatedly until the user chooses to exit.*/

void main()
{
    createlist();
    int choice;
    do
    {
        printf("\n1. Add a song\n");
        printf("2. Delete a song\n");
        printf("3. Display the list\n");
        printf("4. Filter songs on basis of artist,movie\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addsong();
            updatefile();
            break;
        case 2:
            deletesong();
            updatefile();
            break;
        case 3:
            displaylist();
            printf("\n");
            break;
        case 4:
            filter();
            printf("\n");
            break;
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n\n");
            break;
        }
    } while (choice != 5);
}