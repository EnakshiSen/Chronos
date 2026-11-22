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
    char subChoice[3][2][200];        // [original choice][which sub-option]
    char subConsequence[3][2][300];
};

struct Event events[10];

int count = 0;
int stability = 100;
int decisionsMade = 0;

int firstChoice[10] = {0};   // 0 = not chosen yet, otherwise 1/2/3
// Function prototypes
int readEvents();
void displayTimeline();
void showEvent(int index);
void travelToEvent();
void makeDecision(int index);
void showConsequence(int index, int decision);
void showRules();

void showRules()
{
    printf("\n====================================\n");
    printf("          RULES OF CHRONOS\n");
    printf("====================================\n");

    printf("\n1. You are a time traveler. Visit historical events using their Event ID.");
    printf("\n2. At each event you choose 1 of 3 interventions.");
    printf("\n3. Timeline Stability starts at 100%% and can never go above 100%%.");
    printf("\n4. Your choices change stability:");
    printf("\n5. Revisit an event to unlock follow-up options based on your first choice.");
    printf("\n6. History is connected. If you disrupt an event that a later event");
    printf("\n   depends on, visiting that later event causes a PARADOX and costs");
    printf("\n   an extra 5%% stability for each broken dependency.");
    printf("\n7. If stability drops below 50%%, the timeline collapses. You die");
    printf("\n   and respawn with stability restored to 100%%.");
    printf("\n8. Use the menu to view the timeline, check stability, or exit.");

    printf("\n\nPress ENTER to begin...");
    getchar();
}

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
        return 0;
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

                default:
                    if (field >= 7 && field <= 18)
                    {
                        int k = field - 7;
                        int c = k / 4;          // which original choice (0-2)
                        int s = (k % 4) / 2;    // which sub-option (0-1)

                        if (k % 2 == 0)
                            strcpy(events[count].subChoice[c][s], token);
                        else
                            strcpy(events[count].subConsequence[c][s], token);
                    }
                    break;
            }

            field++;

            token = strtok(NULL, "|");
        }

        if (field == 19)
        {
            count++;
            field = 0;
        }
    }

    fclose(fp);

    
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
// Array index: 0=101, 1=102, 2=103, 3=104, 4=105, 5=106, 6=107, 7=108, 8=109, 9=110
// Each event depends on up to 2 earlier events (-1 = none)
const int deps[10][2] =
{
    { -1, -1 },   // 101 WWI: none
    {  0, -1 },   // 102 WWII <- 101 WWI
    {  3, -1 },   // 103 Hiroshima <- 104 Newton
    {  7,  8 },   // 104 Newton <- 108 Descartes, 109 Galileo
    { -1, -1 },   // 105 Rebellion of 1857: none
    {  3, -1 },   // 106 Wright flight <- 104 Newton
    {  3,  5 },   // 107 Mangalyaan <- 104 Newton, 106 Wright
    {  8, -1 },   // 108 Descartes <- 109 Galileo
    { -1, -1 },   // 109 Galileo: none
    {  7, -1 }    // 110 Dartmouth <- 108 Descartes
};

// The first choice (1/2/3) that disrupts history in each event (0 = none)
const int disruptChoice[10] = { 1, 0, 0, 2, 0, 2, 0, 2, 2, 0 };

const char *brokenText[10][2] =
{
    { "", "" },
    { "WWI did not unfold as in history, so the instability behind this invasion is missing and the plans rest on shaky ground.", "" },
    { "Without Newton's Principia, classical mechanics never matured. Nuclear physics is decades away, so the bomb does not exist.", "" },
    { "Without Descartes' analytic geometry, Newton lacks the mathematics to express his laws.",
      "Without Galileo's observations, Newton lacks the groundwork on motion and the heavens." },
    { "", "" },
    { "Without Newton's mechanics, the Wrights have no theory of forces to design their wings.", "" },
    { "Without Newton, orbital mechanics is unknown and a Mars orbit insertion is impossible.",
      "Without the Wrights, aviation and aerospace engineering never took off." },
    { "Without Galileo's discoveries, the scientific revolution stalls and Descartes' work lacks context.", "" },
    { "", "" },
    { "Without analytic geometry, the mathematical foundation for computing machines is missing.", "" }
};

int isBroken(int src)
{
    return firstChoice[src] != 0 && firstChoice[src] == disruptChoice[src];
}

int brokenCount(int index)
{
    int s, n = 0;

    for (s = 0; s < 2; s++)
    {
        int src = deps[index][s];
        if (src >= 0 && isBroken(src))
            n++;
    }
    return n;
}

void showRipple(int index)
{
    int s, shown = 0;

    for (s = 0; s < 2; s++)
    {
        int src = deps[index][s];

        if (src < 0 || firstChoice[src] == 0)
            continue;               // no dependency, or not visited yet

        if (!shown)
        {
            printf("\n---------- RIPPLES FROM EARLIER HISTORY ----------\n");
            shown = 1;
        }

        if (isBroken(src))
            printf("! PARADOX (%s): %s\n", events[src].title, brokenText[index][s]);
        else
            printf("- %s (%d) unfolded as needed, so this era stands on solid ground.\n",
                   events[src].title, events[src].year);
    }
}

void showEvent(int index)
{
    printf("\n====================================\n");
    printf("TIME TRAVEL SUCCESSFUL!\n");
    printf("====================================\n");

    printf("\nEvent: %s", events[index].title);
    printf("\nYear: %d", events[index].year);
    printf("\n\n%s\n", events[index].scenario);

    showRipple(index);
    
    printf("\nWhat would you like to do?\n");

    makeDecision(index);
}

void makeDecision(int index)
{
    const char *text[4];
    int kind[4];   // 0 = original choice, 1 = unlocked sub-option
    int id[4];     // original choice number (1-3) or sub-option number (0-1)
    int n = 0, c, s, decision;
    int fc = firstChoice[index];

    // Original choices, except the one already chosen
    for (c = 1; c <= 3; c++)
    {
        if (c == fc)
            continue;

        text[n] = (c == 1) ? events[index].choiceA :
                  (c == 2) ? events[index].choiceB :
                             events[index].choiceC;
        kind[n] = 0;
        id[n] = c;
        n++;
    }

    // Unlocked sub-options
    if (fc != 0)
    {
        for (s = 0; s < 2; s++)
        {
            text[n] = events[index].subChoice[fc - 1][s];
            kind[n] = 1;
            id[n] = s;
            n++;
        }
    }

    // Show the menu
    for (c = 0; c < n; c++)
    {
        printf("\n%d. %s", c + 1, text[c]);
        if (kind[c] == 1)
            printf("  [UNLOCKED]");
    }

    printf("\n\nEnter your decision (1-%d): ", n);
    scanf("%d", &decision);

    printf("\n====================================\n");

    if (decision < 1 || decision > n)
    {
        printf("\nInvalid decision! No changes made.");
        return;
    }

    int pick = decision - 1;
    printf("Decision selected: %s\n", text[pick]);

    if (kind[pick] == 0)
    {
        // Original choice (same effects as before)
        int choice = id[pick];

        if (choice == 1)
        {
            stability += 5;
            printf("\nYou have successfully altered this historical event.");
        }
        else if (choice == 2)
        {
            stability -= 5;
            printf("\nThe timeline has experienced an unexpected disturbance.");
        }
        else
        {
            stability -= 10;
            printf("\nYour intervention has created an unpredictable outcome.");
        }

        if (firstChoice[index] == 0)
            firstChoice[index] = choice;   // unlocks sub-options for next visit

        decisionsMade++;
        showConsequence(index, choice);
    }
    else
    {
        // Unlocked sub-option
        int sub = id[pick];

        if (sub == 0)
        {
            stability += 5;
            printf("\nYour follow-up action strengthens the timeline.");
        }
        else
        {
            stability -= 5;
            printf("\nYour follow-up action ripples through time.");
        }

        decisionsMade++;

        printf("\n\n========== A New Path Unfolds ==========\n");
        printf("%s", events[index].subConsequence[fc - 1][sub]);
        printf("\n========================================\n");
    }

        // Paradox penalty: broken dependencies hurt stability
    int penalty = brokenCount(index) * 5;
    if (penalty > 0)
    {
        stability -= penalty;
        printf("\n\nParadox penalty: -%d stability (history this event depends on was changed).", penalty);
    }
    // Stability cannot exceed 100%
    if (stability > 100)
        stability = 100;

    printf("\n\nTimeline Stability: %d%%", stability);
    printf("\nDecisions Made: %d", decisionsMade);
    printf("\n====================================\n");

    // Death / respawn (unchanged)
    if (stability < 50)
    {
        printf("\n\n");
        printf("====================================\n");
        printf("          YOU ARE DEAD!\n");
        printf("====================================\n");

        printf("\nThe timeline has collapsed.");
        printf("\nYour existence has been erased.");
        printf("\n\nPress ENTER to respawn...");

        getchar();
        getchar();

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
    showRules(); 
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
