#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
