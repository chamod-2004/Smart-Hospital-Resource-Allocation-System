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
double calculateSurcharge(int urgency, double baseFee, double *pct);
double calculateWardCost(int days, int wIdx);
double calculateGrossTotal(double baseFee, double surcharge, double wardCost);
double calculateDiscount(int age, double grossTotal, double *pct);
double calculateFinalPayable(double gross, double discount);
void registerPatient(void);
void printBill(int i);
void appendPatientRecord(int i);



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
                registerPatient();
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
        for (b = 0; b < MAX_BEDS_PER_WARD; b++)
            bedOccupancy[w][b] = 0;
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
        for (b = 0; b < wardBedCapacity[w]; b++)
            if (fscanf(fp, "%d", &value) == 1)
                bedOccupancy[w][b] = value;

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
            fprintf(fp, "%d ", bedOccupancy[w][b]);
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void showSpecialtyMenu(void)
{
    int i;
    printf("\n--- SPECIALTIES ---\n");
    for (i = 0; i < NUM_SPECIALTIES; i++)
        printf(" %d. %-24s Fee: LKR %.2f  (%d mins)\n",
            specialtyID[i], specialtyName[i], baseConsultFee[i], consultMinutes[i]);
}

void showWardMenu(void)
{
    int i;
    printf("\n--- WARDS ---\n");
    for (i = 0; i < NUM_WARDS; i++)
        printf(" %d. %-30s Rate: LKR %.2f/day  Capacity: %d\n",
            wardID[i], wardName[i], wardDailyRate[i], wardBedCapacity[i]);
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

double calculateSurcharge(int urgency, double baseFee, double *pct)
{
    if (urgency == 1) { *pct = 0.0; return 0.0; }
    if (urgency == 2) { *pct = 0.20; return baseFee * 0.20; }
    *pct = 0.50;
    return baseFee * 0.50;
}

double calculateWardCost(int days, int wIdx)
{
    if (days <= 0 || wIdx < 0) return 0.0;
    return days * wardDailyRate[wIdx];
}

double calculateGrossTotal(double baseFee, double surcharge, double wardCost)
{
    return baseFee + surcharge + wardCost;
}

double calculateDiscount(int age, double grossTotal, double *pct)
{
    if (age < 5 || age > 65) { *pct = 0.15; return grossTotal * 0.15; }
    *pct = 0.0;
    return 0.0;
}

double calculateFinalPayable(double gross, double discount)
{
    return gross - discount;
}

void registerPatient(void)
{
    int i, specID, wID, bed;

    if (patientCount >= MAX_PATIENTS)
    {
        printf("\nPatient records are full.\n");
        return;
    }

    i = patientCount;

    printf("\n--- NEW PATIENT REGISTRATION ---\n");

    printf("Patient Name: ");
    scanf(" %59[^\n]", patientName[i]);

    do
    {
        printf("Patient Age: ");
        scanf("%d", &patientAge[i]);
        if (patientAge[i] < 0) printf("Age cannot be negative.\n");
    } while (patientAge[i] < 0);

    do
    {
        printf("Triage / Urgency Level (1=Normal, 2=Urgent, 3=Critical): ");
        scanf("%d", &urgencyLevel[i]);
    } while (urgencyLevel[i] < 1 || urgencyLevel[i] > 3);

    showSpecialtyMenu();

    do
    {
        printf("Select Specialty ID (1-4): ");
        scanf("%d", &specID);
    } while (specID < 1 || specID > 4);

    specialtyIndex[i] = specID - 1;

    if (specialtyQueueCount[specialtyIndex[i]] >= dailyPatientCap[specialtyIndex[i]])
    {
        printf("\nDaily patient cap for %s has been reached.\n", specialtyName[specialtyIndex[i]]);
        printf("Patient cannot be registered.\n");
        return;
    }

    do
    {
        printf("Is patient admitted to a ward? (1=Yes, 0=No): ");
        scanf("%d", &isAdmitted[i]);
    } while (isAdmitted[i] != 0 && isAdmitted[i] != 1);

    if (isAdmitted[i] == 1)
    {
        showWardMenu();

        do
        {
            printf("Ward ID (1-4): ");
            scanf("%d", &wID);
        } while (wID < 1 || wID > 4);

        wardIndexArr[i] = wID - 1;

        do
        {
            printf("Days Admitted: ");
            scanf("%d", &daysAdmitted[i]);
            if (daysAdmitted[i] <= 0) printf("Days admitted must be greater than 0.\n");
        } while (daysAdmitted[i] <= 0);

        bed = findFreeBed(wardIndexArr[i]);

        if (bed == -1)
        {
            printf("\n*** Sorry, %s is FULL. ***\n", wardName[wardIndexArr[i]]);
            printf("Patient will be registered as OPD instead.\n");
            isAdmitted[i] = 0;
            wardIndexArr[i] = -1;
            bedNumberArr[i] = -1;
            daysAdmitted[i] = 0;
        }
        else
        {
            bedNumberArr[i] = bed;
            printf("\nBed allocated successfully.\n");
            printf("Ward : %s\n", wardName[wardIndexArr[i]]);
            printf("Bed  : %d\n", bedNumberArr[i]);
        }
    }
    else
    {
        wardIndexArr[i] = -1;
        bedNumberArr[i] = -1;
        daysAdmitted[i] = 0;
    }

    baseFeeArr[i] = baseConsultFee[specialtyIndex[i]];
    surchargeArr[i] = calculateSurcharge(urgencyLevel[i], baseFeeArr[i], &surchargePctArr[i]);
    wardCostArr[i] = calculateWardCost(daysAdmitted[i], wardIndexArr[i]);
    grossTotalArr[i] = calculateGrossTotal(baseFeeArr[i], surchargeArr[i], wardCostArr[i]);
    discountArr[i] = calculateDiscount(patientAge[i], grossTotalArr[i], &discountPctArr[i]);
    finalPayableArr[i] = calculateFinalPayable(grossTotalArr[i], discountArr[i]);
    waitTimeArr[i] = calculateWaitTime(specialtyIndex[i]);

    patientNumericID[i] = 1001 + i;
    registrationOrder[i] = i;
    patientCount++;

    printBill(i);
    appendPatientRecord(i);


}

void printBill(int i)
{
    char subsidyNote[40] = "";
    char urgencyText[15];

    if (patientAge[i] < 5 || patientAge[i] > 65)
        strcpy(subsidyNote, "(15% Subsidy Eligible)");

    if (urgencyLevel[i] == 1) strcpy(urgencyText, "Normal");
    else if (urgencyLevel[i] == 2) strcpy(urgencyText, "Urgent");
    else strcpy(urgencyText, "Critical");

    printf("\n");
    printf("====================================================\n");
    printf("          SMART HOSPITAL ADMISSION & BILL\n");
    printf("====================================================\n");
    printf("Patient ID     : PAT-%d\n", patientNumericID[i]);
    printf("Patient Name   : %s\n", patientName[i]);
    printf("Age            : %d Years %s\n", patientAge[i], subsidyNote);
    printf("Specialty      : %s\n", specialtyName[specialtyIndex[i]]);

    if (isAdmitted[i] == 1 && bedNumberArr[i] != -1)
        printf("Assigned Ward  : %s (Bed #%02d)\n", wardName[wardIndexArr[i]], bedNumberArr[i]);
    else
        printf("Assigned Ward  : Outpatient (OPD)\n");

    printf("Urgency Level  : Level %d (%s)\n", urgencyLevel[i], urgencyText);
    printf("----------------------------------------------------\n");
    printf("Base Consultation Fee   : LKR %.2f\n", baseFeeArr[i]);
    printf("Emergency Surcharge     : LKR %.2f (%.0f%%)\n", surchargeArr[i], surchargePctArr[i] * 100);
    printf("Ward Stay Cost (%d Days) : LKR %.2f\n", daysAdmitted[i], wardCostArr[i]);
    printf("----------------------------------------------------\n");
    printf("Gross Total Bill        : LKR %.2f\n", grossTotalArr[i]);
    printf("Age Subsidy Discount    : LKR -%.2f (%.0f%%)\n", discountArr[i], discountPctArr[i] * 100);
    printf("----------------------------------------------------\n");
    printf("Final Payable Amount    : LKR %.2f\n", finalPayableArr[i]);

    if (waitTimeArr[i] <= 0.0)
        printf("Estimated Waiting Time  : 0.00 mins (Immediate Attention)\n");
    else
        printf("Estimated Waiting Time  : %.2f mins\n", waitTimeArr[i]);

    printf("====================================================\n");
}

void appendPatientRecord(int i)
{
    FILE *fp;

    fp = fopen("patient_records.txt", "a");
    if (fp == NULL)
    {
        printf("\nWarning: could not write to patient_records.txt\n");
        return;
    }

    fprintf(fp,
        "PAT-%d | %s | Age:%d | Urgency:%d | Specialty:%s | Ward:%s | Bed:%d | Days:%d | Final:LKR %.2f\n",
        patientNumericID[i],
        patientName[i],
        patientAge[i],
        urgencyLevel[i],
        specialtyName[specialtyIndex[i]],
        (isAdmitted[i] == 1 ? wardName[wardIndexArr[i]] : "OPD (Not Admitted)"),
        bedNumberArr[i],
        daysAdmitted[i],
        finalPayableArr[i]
    );

    fclose(fp);
}
