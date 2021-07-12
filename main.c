#include <stdio.h>
#include <stdlib.h>

int maxline(void){
    FILE *fpointer = fopen("contact_info.csv","r");
    char buffer[150];   // Buffer for fgets
    char *record;
    fseek(fpointer, 45, SEEK_SET);      // Start from the 2nd row
    int maxlinenum = 1;          // Counter for the current line number

    while ((fgets(buffer, sizeof(buffer), fpointer)) != NULL)   // Read lines until the end of the file.
    {
        record = strtok(buffer, ",\n");
        if (atoi(record)> maxlinenum){
            maxlinenum = atoi(record);
        }
    }
    printf("This entry will be ID number %d.\n", maxlinenum+1);
    fclose(fpointer);
    return maxlinenum+1;
}

void addcontact(void){
    int id_num = maxline();
    FILE *fpointer = fopen("contact_info.csv","a");
    char firstname[20],lastname[20],address[40],phone[13],email[50];
    int confirm = 2;

    while(confirm != 1){
        printf("Please enter your first name: ");
        scanf(" %s", firstname);
        printf("Please enter your last name: ");
        scanf(" %s", lastname);
        getchar();                                                                  // Removes the "\n" that is included at the end of the previous scanf call
        printf("Please enter your address: ");
        fgets(address, 40, stdin);
        address[strcspn(address, "\n")] = 0;                                        // strcspn will remove the "\n" that is included at the end of the fgets call previous
        printf("Please enter your phone number in the format 'XXX-XXX-XXXX': ");
        scanf("%s", &phone);
        printf("Please enter your email address: ");
        scanf(" %s", email);
        printf("\n");

        printf("These are the details you have provided:\n");
        printf("First Name: %s\n", firstname);
        printf("Last Name: %s\n", lastname);
        printf("Address: %s\n", address);
        printf("Phone: %s\n", phone);
        printf("Email: %s\n\n", email);
        printf("Is this correct?\n1. Yes\n2. No\n");
        printf("Please enter the number for the action: ");
        scanf("%d", &confirm);
        printf("\n");
    }

    fprintf(fpointer, "%d,%s,%s,%s,%s,%s\n", id_num, firstname, lastname, address, phone, email);
    printf("Contact Added Successfully.\n\n");
    fclose(fpointer);
}

void listcontacts(void){
    FILE *fpointer = fopen("contact_info.csv","r");
    char buffer[150];   // Buffer for fgets
    char *record;       // Pointer to address that stores tokens from strtok
    fseek(fpointer, 45, SEEK_SET);      // Start from the 2nd row
    int j = 0;          // Counter for the columns

    while ((fgets(buffer, sizeof(buffer), fpointer)) != NULL)   // Read lines until the end of the file.
    {
        record = strtok(buffer, ",\n");                         // First strtok call points to the buffer.
        while (record != NULL){
            printf("%s | ", record);
            if (j == 5)                                         // When you reach the end of the columns, break from the inner while loop.
                break;
            record = strtok(NULL,",\n");                        // Subsequent strtok calls point to NULL
            j++;
        }
        printf("\n");
        j = 0;
    }
    printf("\n");
    fclose(fpointer);
}

void contactsearch(void){
    FILE *fpointer = fopen("contact_info.csv","r");
    char search[50];
    char buffer[150];   // Buffer for fgets
    char *record;       // Pointer to address that stores tokens from strtok
    fseek(fpointer, 45, SEEK_SET);      // Start from the 2nd row
    int res = 0;          // Counter for the results
    printf("Please enter a your search term: ");
    scanf(" %s",search);

    while ((fgets(buffer, sizeof(buffer), fpointer)) != NULL)   // Read lines until the end of the file.
    {
        if (strstr(buffer, search) != NULL){
            record = strtok(buffer, ",\n");
            for(int j = 0; j<6; j++){
                printf("%s | ", record);
                record = strtok(NULL,",\n");
            }
            printf("\n\n");
            res++;
        }
    }
    if (res>0){
        printf("Results found: %d\n\n", res);
    } else {
        printf("No results found.\n\n");
    }
    fclose(fpointer);
}

void editcontact(void){
    FILE *ftemp = fopen("temp.csv","w");
    FILE *fpointer = fopen("contact_info.csv","r");
    char search[50];
    char buffer[150];   // Buffer for fgets
    char *record;       // Pointer to address that stores tokens from strtok
    int id_num;
    char firstname[20],lastname[20],address[40],phone[13],email[50];
    int confirm = 2;
    printf("Please enter the phone number of the contact you would like to edit: ");
    scanf(" %s",search);

    // Copying from contact_info.csv into temp.csv excluding the row matching the phone number.
    while ((fgets(buffer, sizeof(buffer), fpointer)) != NULL)   // Read lines until the end of the file.
    {
        if (strstr(buffer, search) != NULL){
            printf("The contact below will be edited:\n");
            record = strtok(buffer, ",\n");
            id_num = atoi(record);
            printf("%d\n",id_num);
            for(int j = 0; j<6; j++){
                printf("%s | ", record);
                record = strtok(NULL,",\n");
            }
            printf("\n");
            continue;
        } else {
            fprintf(ftemp, "%s", buffer);
        }
    }

    // Asking user to edit their details.
    while(confirm != 1){
        printf("Please enter your first name: ");
        scanf(" %s", firstname);
        printf("Please enter your last name: ");
        scanf(" %s", lastname);
        getchar();                                                                  // Removes the "\n" that is included at the end of the previous scanf call
        printf("Please enter your address: ");
        fgets(address, 40, stdin);
        address[strcspn(address, "\n")] = 0;                                        // strcspn will remove the "\n" that is included at the end of the fgets call previous
        printf("Please enter your phone number in the format 'XXX-XXX-XXXX': ");
        scanf("%s", &phone);
        printf("Please enter your email address: ");
        scanf(" %s", email);
        printf("\n");

        printf("These are the details you have provided:\n");
        printf("First Name: %s\n", firstname);
        printf("Last Name: %s\n", lastname);
        printf("Address: %s\n", address);
        printf("Phone: %s\n", phone);
        printf("Email: %s\n\n", email);
        printf("Is this correct?\n1. Yes\n2. No\n");
        printf("Please enter the number for the action: ");
        scanf("%d", &confirm);
        printf("\n");
    }

    fprintf(ftemp, "%d,%s,%s,%s,%s,%s", id_num, firstname, lastname, address, phone, email);
    fclose(ftemp);
    fclose(fpointer);
    remove("contact_info.csv");
    rename("temp.csv","contact_info.csv");

}

void deletecontact(void){
    FILE *ftemp = fopen("temp.csv","w");
    FILE *fpointer = fopen("contact_info.csv","r");
    char search[50];
    char buffer[150];
    printf("Please enter a phone number in the format XXX-XXX-XXXX of the contact you would like to delete: ");
    scanf(" %s",search);

    while ((fgets(buffer, sizeof(buffer), fpointer)) != NULL)   // Read lines until the end of the file.
    {
        if (strstr(buffer, search) != NULL){
            continue;
        } else {
            fprintf(ftemp, "%s", buffer);
        }
    }

    fclose(ftemp);
    fclose(fpointer);
    remove("contact_info.csv");
    rename("temp.csv","contact_info.csv");
    printf("Contact with phone number %s has been deleted.\n\n", search);
}

int main()
{

    int action;

    printf("****Welcome to the Contact Management System!****\n\n");

    while(1){
        printf("    MAIN MENU    \n");
        printf("========================\n");
        printf("[1] List all contacts\n[2] Add new contact\n[3] Search contact\n[4] Edit contact\n[5] Delete contact\n[6] Exit Program\n");
        printf("========================\n");
        printf("Please enter the number for the action: ");
        scanf("%d", &action);

        if (action == 6){
            break;
        } else {
            switch (action)
            {
                case 1:
                    listcontacts();
                    break;
                case 2:
                    addcontact();
                    break;
                case 3:
                    contactsearch();
                    break;
                case 4:
                    editcontact();
                    break;
                case 5:
                    deletecontact();
                    break;
                default:
                    printf("You have entered an invalid value. Please Try Again.\n");
                    break;
            }
        }
    }

    return 0;
}
