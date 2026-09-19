#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS_PER_WARD 20
#define MAX_PATIENTS 200

int specialtyID[NUM_SPECIALTIES] = {1, 2, 3, 4};

char specialtyName[NUM_SPECIALTIES][30] = {
    "General Practice (OPD)",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};

double baseConsultFee[NUM_SPECIALTIES] = {
    1500.00, 2500.00, 4500.00, 5000.00
};

int consultMinutes[NUM_SPECIALTIES] = {
    15, 20, 30, 30
};

int dailyPatientCap[NUM_SPECIALTIES] = {
    30, 20, 12, 10
};

int specialtyQueueCount[NUM_SPECIALTIES] = {0, 0, 0, 0};

int wardID[NUM_WARDS] = {1, 2, 3, 4};

char wardName[NUM_WARDS][40] = {
    "General Ward",
    "Paediatric Ward",
    "Surgical Ward",
    "ICU (Intensive Care Unit)"
};

double wardDailyRate[NUM_WARDS] = {
    3000.00, 6000.00, 12000.00, 25000.00
};

int wardBedCapacity[NUM_WARDS] = {20, 10, 10, 5};

int bedOccupancy[NUM_WARDS][MAX_BEDS_PER_WARD];

int patientCount = 0;

int patientNumericID[MAX_PATIENTS];
char patientName[MAX_PATIENTS][60];
int patientAge[MAX_PATIENTS];
int urgencyLevel[MAX_PATIENTS];
int specialtyIndex[MAX_PATIENTS];
int isAdmitted[MAX_PATIENTS];
int wardIndexArr[MAX_PATIENTS];
int bedNumberArr[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];

double baseFeeArr[MAX_PATIENTS];
double surchargeArr[MAX_PATIENTS];
double surchargePctArr[MAX_PATIENTS];
double wardCostArr[MAX_PATIENTS];
double grossTotalArr[MAX_PATIENTS];
double discountArr[MAX_PATIENTS];
double discountPctArr[MAX_PATIENTS];
double finalPayableArr[MAX_PATIENTS];
double waitTimeArr[MAX_PATIENTS];

int registrationOrder[MAX_PATIENTS];

void initializeBeds(void);
void loadBedStatus(void);
void saveBedStatus(void);
void showSpecialtyMenu(void);
void showWardMenu(void);
int findFreeBed(int wIdx);
double calculateWaitTime(int specIdx);



int main(void)
{
    int choice;

    initializeBeds();
    loadBedStatus();

    do
    {
        printf("\n");
        printf("====================================================\n");
        printf("       SMART HOSPITAL ADMISSION & BILLING SYSTEM\n");
        printf("====================================================\n");
        printf(" 1. Register New Patient\n");
        printf(" 2. Show Patients (Priority Order)\n");
        printf(" 3. Generate Summary Reports\n");
        printf(" 4. Save & Exit\n");
        printf("----------------------------------------------------\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            choice = -1;
        }

        switch (choice)
        {
            case 1:
                showSpecialtyMenu();
                showWardMenu();
                printf("\n[Register Patient - coming soon]\n");
                break;
            case 2:
                printf("\n[Priority List - coming soon]\n");
                break;
            case 3:
                printf("\n[Reports - coming soon]\n");
                break;
            case 4:
                saveBedStatus();
                printf("\nBed status saved.\n");
                printf("Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != 4);

    return 0;
}

void initializeBeds(void)
{
    int w, b;

    for (w = 0; w < NUM_WARDS; w++)
    {
        for (b = 0; b < MAX_BEDS_PER_WARD; b++)
        {
            bedOccupancy[w][b] = 0;
        }
    }
}

void loadBedStatus(void)
{
    FILE *fp;
    int w, b, value;

    fp = fopen("beds_status.txt", "r");

    if (fp == NULL)
    {
        printf("\n(No previous beds_status.txt found - starting fresh)\n");
        return;
    }

    for (w = 0; w < NUM_WARDS; w++)
    {
        for (b = 0; b < wardBedCapacity[w]; b++)
        {
            if (fscanf(fp, "%d", &value) == 1)
            {
                bedOccupancy[w][b] = value;
            }
        }
    }

    fclose(fp);
    printf("\nBed status loaded from beds_status.txt\n");
}

void saveBedStatus(void)
{
    FILE *fp;
    int w, b;

    fp = fopen("beds_status.txt", "w");

    if (fp == NULL)
    {
        printf("\nError: Could not save bed status.\n");
        return;
    }

    for (w = 0; w < NUM_WARDS; w++)
    {
        for (b = 0; b < wardBedCapacity[w]; b++)
        {
            fprintf(fp, "%d ", bedOccupancy[w][b]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void showSpecialtyMenu(void)
{
    int i;
    printf("\n--- SPECIALTIES ---\n");
    for (i = 0; i < NUM_SPECIALTIES; i++)
    {
        printf(" %d. %-24s Fee: LKR %.2f  (%d mins)\n",
            specialtyID[i], specialtyName[i], baseConsultFee[i], consultMinutes[i]);
    }
}

void showWardMenu(void)
{
    int i;
    printf("\n--- WARDS ---\n");
    for (i = 0; i < NUM_WARDS; i++)
    {
        printf(" %d. %-30s Rate: LKR %.2f/day  Capacity: %d\n",
            wardID[i], wardName[i], wardDailyRate[i], wardBedCapacity[i]);
    }
}

int findFreeBed(int wIdx)
{
    int b;
    for (b = 0; b < wardBedCapacity[wIdx]; b++)
    {
        if (bedOccupancy[wIdx][b] == 0)
        {
            bedOccupancy[wIdx][b] = 1;
            return b + 1;
        }
    }
    return -1;
}

double calculateWaitTime(int specIdx)
{
    double wait;
    wait = specialtyQueueCount[specIdx] * (double)consultMinutes[specIdx];
    specialtyQueueCount[specIdx]++;
    return wait;
}


