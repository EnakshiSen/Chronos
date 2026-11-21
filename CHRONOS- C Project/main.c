#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Event
{
    int id;
    int year;
    char title[100];
    char scenario[300];
    char choiceA[200];
    char choiceB[200];
    char choiceC[200];
};

struct Event events[10];

int count = 0;
int stability = 100;
int decisionsMade = 0;

// Function prototypes
int readEvents();
void displayTimeline();
void showEvent(int index);
void travelToEvent();
void makeDecision(int index);
void showConsequence(int index, int decision);

int readEvents()
{
    FILE *fp;
    char line[1200];
    char *token;


    int field = 0;

    fp = fopen("data/events.txt", "r");

    if (fp == NULL)
    {
        printf("Error opening events.txt!\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // Remove newline character
        line[strcspn(line, "\r\n")] = '\0';

        // Skip empty lines
        if (strlen(line) == 0)
        {
            continue;
        }

        // First field: ID
        if (field == 0)
        {
            token = strtok(line, "|");

            events[count].id = atoi(token);
        }
        else
        {
            token = strtok(line, "|");
        }

        while (token != NULL)
        {
            switch (field)
            {
                case 1:
                    events[count].year = atoi(token);
                    break;

                case 2:
                    strcpy(events[count].title, token);
                    break;

                case 3:
                    strcpy(events[count].scenario, token);
                    break;

                case 4:
                    strcpy(events[count].choiceA, token);
                    break;

                case 5:
                    strcpy(events[count].choiceB, token);
                    break;

                case 6:
                    strcpy(events[count].choiceC, token);
                    break;
            }

            field++;

            token = strtok(NULL, "|");
        }

        if (field == 7)
        {
            count++;
            field = 0;
        }
    }

    fclose(fp);

    // Display all loaded events
    int i;
    for (i = 0; i < count; i++)
    {
        printf("\n====================================\n");
        printf("Event ID: %d\n", events[i].id);
        printf("Year: %d\n", events[i].year);
        printf("Title: %s\n", events[i].title);
        printf("Scenario: %s\n", events[i].scenario);

        printf("\nAvailable Decisions:\n");
        printf("1. %s\n", events[i].choiceA);
        printf("2. %s\n", events[i].choiceB);
        printf("3. %s\n", events[i].choiceC);
    }

    printf("\nTotal events loaded: %d\n", count);
    return count;
}

void displayTimeline()
{
    int i;
    printf("\n========== HISTORICAL TIMELINE ==========\n");

    for (i = 0; i < count; i++)
    {
        printf("\nEvent ID: %d", events[i].id);
        printf("\nYear: %d", events[i].year);
        printf("\nEvent: %s\n", events[i].title);
    }

    printf("\n=========================================\n");
}

void showEvent(int index)
{
    printf("\n====================================\n");
    printf("TIME TRAVEL SUCCESSFUL!\n");
    printf("====================================\n");

    printf("\nEvent: %s", events[index].title);
    printf("\nYear: %d", events[index].year);

    printf("\n\n%s\n", events[index].scenario);

    printf("\nWhat would you like to do?\n");

    printf("\n1. %s", events[index].choiceA);
    printf("\n2. %s", events[index].choiceB);
    printf("\n3. %s", events[index].choiceC);

    makeDecision(index);
}



void makeDecision(int index)
{
    int decision;

    printf("\nEnter your decision (1-3): ");
    scanf("%d", &decision);

    printf("\n====================================\n");

    if (decision == 1)
    {
        printf("Decision selected: %s\n", events[index].choiceA);
        stability += 5;
        printf("\nYou have successfully altered this historical event.");
    }
    else if (decision == 2)
    {
        printf("Decision selected: %s\n", events[index].choiceB);
        stability -= 5;
        printf("\nThe timeline has experienced an unexpected disturbance.");
    }
    else if (decision == 3)
    {
        printf("Decision selected: %s\n", events[index].choiceC);
        stability -= 10;
        printf("\nYour intervention has created an unpredictable outcome.");
    }
    else
    {
        printf("\nInvalid decision! No changes made.");
        return;
    }

    decisionsMade++;
    showConsequence(index, decision);

    // Stability cannot exceed 100%
    if (stability > 100)
    {
        stability = 100;
    }

    printf("\n\nTimeline Stability: %d%%", stability);
    printf("\nDecisions Made: %d", decisionsMade);

    printf("\n====================================\n");

    // Check whether the player has survived
    if (stability < 50)
    {
        printf("\n\n");
        printf("====================================\n");
        printf("          YOU ARE DEAD!\n");
        printf("====================================\n");

        printf("\nThe timeline has collapsed.");
        printf("\nYour existence has been erased.");
        printf("\n\nPress ENTER to respawn...");

        // Clear the leftover newline from scanf
        getchar();

        // Wait for the player to press Enter
        getchar();

        // Respawn
        stability = 100;

        printf("\n====================================\n");
        printf("       TIME MACHINE REACTIVATED\n");
        printf("====================================\n");

        printf("\nYou have been restored to the timeline.");
        printf("\nTimeline Stability: 100%%\n");
    }
}



void travelToEvent()
{
    int id, i;
    int found = 0;

    printf("\nEnter the Event ID you want to visit: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++)
    {
        if (events[i].id == id)
        {
            showEvent(i);
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        printf("\nInvalid Event ID! Please try again.\n");
    }
}



int main()
{
    int choice;

    count = readEvents();

    if (count == 0)
    {
        printf("No events loaded. Exiting game.\n");
        return 0;
    }

    do
    {
        printf("\n====================================\n");
        printf("             CHRONOS\n");
        printf("       TIME TRAVEL SIMULATION\n");
        printf("====================================\n");

        printf("\n1. View Historical Timeline");
        printf("\n2. Travel to a Historical Event");
        printf("\n3. View Timeline Stability");
        printf("\n4. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                displayTimeline();
                break;

            case 2:
                travelToEvent();
                break;

            case 3:
                printf("\n========== TIMELINE STATUS ==========");
                printf("\nTimeline Stability: %d%%", stability);
                printf("\nDecisions Made: %d", decisionsMade);
                printf("\n=====================================\n");
                break;

            case 4:
                printf("\nExiting CHRONOS...");
                printf("\nThank you for playing!\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 4);

    return 0;
}


void showConsequence(int index, int decision)
{
    printf("\n\n========== You've Altered History! ==========\n");

    switch (events[index].id)
    {
        case 101:
            if (decision == 1)
                printf("Franz Ferdinand survives and opens Europe's first royal driving school.");
            else if (decision == 2)
                printf("European leaders spend six months arguing over a conference table.");
            else
                printf("Military pigeons become Europe's most important communication network.");
            break;

        case 102:
            if (decision == 1)
                printf("Poland avoids invasion. Military strategists begin studying how to prevent wars before breakfast.");
            else if (decision == 2)
                printf("Peace negotiations succeed temporarily. Europe runs out of available diplomats.");
            else
                printf("Classified military messages accidentally reach a local bakery.");
            break;

        case 103:
            if (decision == 1)
                printf("International scientists establish an early global nuclear disarmament initiative.");
            else if (decision == 2)
                printf("Nuclear diplomacy becomes the world's most expensive profession.");
            else
                printf("The demonstration becomes the most debated scientific presentation in history.");
            break;

        case 104:
            if (decision == 1)
                printf("Newton's work spreads across Europe. Students begin to say that instead of an apple, a coconut should have fallen on his head.");
            else if (decision == 2)
                printf("Scientists spend decades trying to explain why apples insist on falling down.");
            else
                printf("Newton challenges the scientific community to a public mathematical debate.");
            break;

        case 105:
            if (decision == 1)
                printf("A united resistance becomes a turning point in India's struggle against colonial rule.");
            else if (decision == 2)
                printf("Underground resistance networks begin organising across the country.");
            else
                printf("Rebel groups develop an unexpectedly effective communication network, centuries before group chats.");
            break;

        case 106:
            if (decision == 1)
                printf("The Wright brothers succeed. Their first passenger is a chicken.");
            else if (decision == 2)
                printf("The first flight is delayed, giving another inventor a chance to change aviation history.");
            else
                printf("An unusual aircraft design inspires a new generation of experimental flying machines.");
            break;

        case 107:
            if (decision == 1)
                printf("Mangalyaan inspires space agencies around the world to pursue ambitious low-cost missions.");
            else if (decision == 2)
                printf("Scientists spend months investigating whether the spacecraft discovered something new or took a very expensive detour.");
            else
                printf("India's planetary exploration programme undergoes a major rethink.");
            break;

        case 108:
            if (decision == 1)
                printf("Coordinate geometry spreads rapidly. Students everywhere gain a new way to complain about graphs.");
            else if (decision == 2)
                printf("Mathematicians struggle to find a systematic bridge between algebra and geometry.");
            else
                printf("Scientists spend decades arguing over which coordinate system is superior.");
            break;

        case 109:
            if (decision == 1)
                printf("Galileo's observations gain wider acceptance, accelerating astronomical research.");
            else if (decision == 2)
                printf("Galileo secretly begins teaching astronomy to curious students.");
            else
                printf("The telescope becomes the most controversial scientific instrument in Europe.");
            break;

        case 110:
            if (decision == 1)
                printf("AI research accelerates. DeepBlue becomes famous for beating its creator at chess.");
            else if (decision == 2)
                printf("The formal birth of AI as a research field is delayed.");
            else
                printf("Humanity finally stops blaming AI for unemployment.");
            break;

        default:
            printf("An unknown disturbance has altered the timeline.");
    }

    printf("\n======================================\n");
}
