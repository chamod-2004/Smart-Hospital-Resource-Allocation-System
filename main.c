#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_SPECIALTIES 4
#define NUM_WARDS 4

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

int main(void)
{
    int choice;

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
                printf("\n[Register Patient - coming soon]\n");
                break;
            case 2:
                printf("\n[Priority List - coming soon]\n");
                break;
            case 3:
                printf("\n[Reports - coming soon]\n");
                break;
            case 4:
                printf("\nGoodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != 4);

    return 0;
}
