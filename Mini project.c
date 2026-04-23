#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <windows.h>
#define AdminID "admin"
#define AdminPASS "admin@123"
// ===== COLORS =====
#define RESET   "\033[0m"

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define PURPLE  "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"

#define PINK    "\033[38;5;205m"
#define SKYBLUE "\033[38;5;117m"
#define CREAM   "\033[38;5;230m"
#define DARKGREEN "\033[38;5;22m"

struct question
{
    char language[20];
    char level[20];
    char question[300];
    char opt1[100];
    char opt2[100];
    char opt3[100];
    char opt4[100];
    int correct;
};

struct AdminData {
    char id[50];
    float cgpa;
    int tech;
    int soft;
};

#define MAX_ATTEMPTS 100

struct report
{
    char id[50];
    int attempts;
    float cgpa[MAX_ATTEMPTS];
    int tech[MAX_ATTEMPTS];
    int soft[MAX_ATTEMPTS];
};

struct student
{
    char id[50];
    char name[50];
    int age,year;
    char course[50];
    float sem[8];
    float overall;
    char password[50];

    int tech_answers[20];
    int tech_correct[20];
    int tech_q_count;

    int soft_answers[10];
    int soft_correct[10];
    int soft_q_count;

    int c_marks;
    int python_marks;
    int cpp_marks;
    int java_marks;
    int total_tech_marks;
    int logical_math_marks;
    int reasoning_marks;
    int total_soft_marks;
};

void signup();
void login();
void admin();
void loading();
void technical_test(struct student all[], int total_students, int index);
void softskill_test(struct student all[], int total_students, int index);
void cgpa_section(struct student all[],int total_students,int index);
int quiz(char language[], struct student all[], int index);
int soft_quiz(char section[], struct student all[], int index);
void save_report_data(struct student s);
void display_student_report(char id[]);
void print_vertical_graph(int c, int py, int cpp, int java, int math, int reason);
void view_certificate(struct student s);
int certificate_exists(char id[]);
void save_certificate(char id[]);
void progress_bar(int score, int total);

int main()
{
    int ch;
    SetConsoleOutputCP(65001); 
    system("color"); 
    srand(time(0));


    printf(CYAN"\n");
    printf("╔══════════════════════════════════════╗\n");
    printf("║     STUDENT ENHANCEMENT SYSTEM       ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. Sign Up                          ║\n");
    printf("║  2. Login                            ║\n");
    printf("║  3. Admin Portal                     ║\n");
    printf("║  4. Exit                             ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf(RESET);

    printf(YELLOW"Enter your choice: "RESET);
    scanf("%d",&ch);

    system("cls");

    if(ch==1)
    {
        signup();
        login();
    }
    else if(ch==2)
    {
        login();
    }
    else if(ch==3)
    {
        admin();
    }
    else
    {
        printf("Invalid Choice\n");
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow)
{
    MessageBox(NULL, "Welcome to Student System", "UI", MB_OK);
    return 0;
}


void signup()
{
    struct student s;
    char newid[50];
    int count=0;

    printf(PURPLE"\n---<>--- SIGN UP ---<>---\n"RESET);

    printf("Name of the student: ");
    scanf("%49s", s.name);

    printf("Age of the student: ");
    scanf("%d", &s.age);

    printf("Year of study (1-4): ");
    scanf("%d", &s.year);

    printf("Course of study: ");
    scanf("%49s", s.course);
    system("cls");

    sprintf(newid, "%s@%d", s.name, s.age);

    FILE *f = fopen("students.csv", "r");

    if(f != NULL) 
    {
        struct student temp;
        char line[600];
        fgets(line, 600, f);

        while(
                fscanf(f,"%[^,],%[^,],%d,%[^,],%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
                temp.id, temp.name, &temp.age, temp.course, &temp.year,
                &temp.sem[0], &temp.sem[1], &temp.sem[2], &temp.sem[3],
                &temp.sem[4], &temp.sem[5], &temp.sem[6], &temp.sem[7],
                &temp.overall,
                &temp.c_marks,
                &temp.python_marks,
                &temp.cpp_marks,
                &temp.java_marks,
                &temp.total_tech_marks,
                &temp.logical_math_marks,
                &temp.reasoning_marks,
                &temp.total_soft_marks,
                temp.password) != EOF)
            {

            if(strncmp(temp.id, newid, strlen(newid)) == 0)
                count++;
            }
        fclose(f);
    }

    if(count == 0)
        strcpy(s.id, newid);
    else
        sprintf(s.id, "%s_%d", newid, count);

    printf(PURPLE"\n---<>--- student ID generated successfully! ---<>---\n"RESET);

    printf("\nGenerated student ID is: %s\n", s.id);

    printf("Set student password: ");
    scanf("%49s", s.password);
    system("cls");

    for(int i = 0; i < 8; i++)
        s.sem[i] = 0;

    s.overall = 0;

    s.c_marks = 0;
    s.python_marks = 0;
    s.cpp_marks = 0;
    s.java_marks = 0;
    s.total_tech_marks = 0;

    s.logical_math_marks = 0;
    s.reasoning_marks = 0;
    s.total_soft_marks = 0;

    f = fopen("students.csv", "a+");
    if (f==NULL)
    {
        printf("Error in opening file \n");
        return;
    }
    fseek(f, 0, SEEK_END);

    if(ftell(f) == 0) 
    {
        fprintf(f,"ID,Name,Age,Course,Year,Sem1,Sem2,Sem3,Sem4,Sem5,Sem6,Sem7,Sem8,Overall,Cmarks,Pythonmarks,Cppmarks,Javamarks,TotalTechMarks,LogicalMath,Reasoning,TotalSoftMarks,Password\n");
    }

    fprintf(f,"%s,%s,%d,%s,%d,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,%s\n",s.id,s.name,s.age,s.course,s.year,s.password);

    fclose(f);

    printf(PURPLE"\n---<>--- Signup successful! Please proceed to login ---<>---\n"RESET);
}

void login() 
{
    char id[50], pass[50];
    struct student all[200];
    struct student temp;
    int found;
    int total_students;
    int index;
    int tp;
    int tech_done = 0;
    int soft_done = 0;

    while(1)   
    {
        found = 0;
        total_students = 0;
        index = -1;

        printf(PURPLE"\n--o-- LOGIN --o--\n"RESET);
        printf("Enter Student ID: ");
        scanf("%49s", id);

        printf("Enter Student Password: ");
        scanf("%49s", pass);

        FILE *f = fopen("students.csv", "r");

        if(f == NULL) 
        {
            printf("No student with the given ID found.\n");
            return;
        }

        char line[600];
        fgets(line, 600, f);

        while(fscanf(f,"%[^,],%[^,],%d,%[^,],%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
                temp.id, temp.name, &temp.age, temp.course, &temp.year,
                &temp.sem[0], &temp.sem[1], &temp.sem[2], &temp.sem[3],
                &temp.sem[4], &temp.sem[5], &temp.sem[6], &temp.sem[7],
                &temp.overall,
                &temp.c_marks,
                &temp.python_marks,
                &temp.cpp_marks,
                &temp.java_marks,
                &temp.total_tech_marks,
                &temp.logical_math_marks,
                &temp.reasoning_marks,
                &temp.total_soft_marks,
                temp.password) != EOF)
        {
            all[total_students] = temp;

            if(strcmp(temp.id, id) == 0 && strcmp(temp.password, pass) == 0) 
            {
                found = 1;
                index = total_students;
            }

            total_students++;
        }

        fclose(f);

        if(found)
        {
            printf(PURPLE"---<>--- Login successful! ---<>---\n"RESET);
            system("cls");
            break;   
        }
        else
        {
            printf("Invalid Student ID or Password. Please try again.\n");
        }
        
    }
    do
    {
    printf(PURPLE"\n╭──────────────────────────────────────────────╮\n"RESET);
    printf(PURPLE"│              🎓 STUDENT MENU 🎓              │\n"RESET);
    printf(PURPLE"├──────────────────────────────────────────────┤\n"RESET);

    printf("│  [1]  Enter Academic CGPA                    │\n");
    printf("│  [2]  Assess Programming Skills              │\n");
    printf("│  [3]  Assess Soft Skills                     │\n");
    printf("│  [4]  View Student Results                   │\n");
    printf("│  [5]  View Certificate                       │\n");
    printf("│  [6]  Logout                                 │\n");

    printf(PURPLE"├──────────────────────────────────────────────┤\n"RESET);
    printf("│    Select an option (1-6):                   │\n");
    printf(PURPLE"╰──────────────────────────────────────────────╯\n"RESET);

    printf(YELLOW"Enter your choice: "RESET);
        scanf("%d",&tp);
        system("cls");

        switch(tp)
        {
            case 1 :
                cgpa_section(all,total_students,index);
                break;

            case 2 :
                technical_test(all,total_students,index);
                tech_done = 1;
                break;

            case 3 :
                softskill_test(all,total_students,index);
                soft_done = 1;
                break;

            case 4 :
                save_report_data(all[index]);   
                display_student_report(all[index].id);
                break;

            case 5 :
                view_certificate(all[index]);   
                break;

            case 6 :
                printf("Logging out\n");
                break;

            default :
                printf("Invalid choice\n");
        }
    } while (tp != 6);
    
}
void admin()
{
    char id[50], pass[50];

    printf(PURPLE "\n==<>==<>==<>== ADMIN LOGIN ==<>==<>==<>==\n\n"RESET);
    printf("Enter Admin ID: ");
    scanf("%s", id);

    printf("Enter Admin Password: ");
    scanf("%s", pass);
    system("cls");

    if(strcmp(id, AdminID) != 0 || strcmp(pass, AdminPASS) != 0)
    {
        printf("\nAccess Denied!\n");
        return;
    }

    printf("\nLogin Successful!\n");

    int choice;

    do {
       
{
    printf(DARKGREEN"\n╔═══════════════════════════════════════════════════════════╗\n"RESET);
        printf(DARKGREEN"║                ADMIN MENU                                 ║\n"RESET);
        printf(DARKGREEN"╠═══════════════════════════════════════════════════════════╣\n"RESET);
        printf("║  %-2d │ %-40s        ║\n",1,"1. Display Overall Results from the Database");
        printf("║  %-2d │ %-40s       ║\n",2,"2. Display Top 3 students from Technical quiz");
        printf("║  %-2d │ %-40s     ║\n",3,"3. Display Top 3 students from Soft Skills quiz");
        printf("║  %-2d │ %-40s   ║\n",4,"4. Display Top 3 students from Academic CGPA list");
        printf("║  %-2d │ %-40s            ║\n",5,"5. Exit Admin Menu");
        printf(DARKGREEN"╚═══════════════════════════════════════════════════════════╝\n"RESET);

        printf(YELLOW"\nEnter your choice: "RESET);
        scanf("%d", &choice);
}
        system("cls");

        if(choice >=1 && choice <=4)
        {
            FILE *fp = fopen("students.csv", "r");
            if(fp == NULL)
            {
                printf("Error opening file\n");
                return;
            }

            struct AdminData students[200];
            int count = 0;
            char line[1000];

            fgets(line, sizeof(line), fp); 

            while(fgets(line, sizeof(line), fp))
            {
                sscanf(line,"%[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%f,%*[^,],%*[^,],%*[^,],%*[^,],%d,%*[^,],%*[^,],%d,%*s",
                    students[count].id,
                    &students[count].cgpa,
                    &students[count].tech,
                    &students[count].soft);
                count++;
            }

            fclose(fp);

            int i, j;
            struct AdminData temp;

           
            if(choice == 2 || choice == 3 || choice == 4)
            {
                for(i=0;i<count-1;i++)
                {
                    for(j=i+1;j<count;j++)
                    {
                        int swap = 0;

                        if(choice==2 && students[i].tech < students[j].tech)
                            swap = 1;

                        if(choice==3 && students[i].soft < students[j].soft)
                            swap = 1;

                        if(choice==4 && students[i].cgpa < students[j].cgpa)
                            swap = 1;

                        if(swap)
                        {
                            temp = students[i];
                            students[i] = students[j];
                            students[j] = temp;
                        }
                    }
                }
            }

            int limit;

            if(choice == 1)
                limit = count;
            else
                limit = (count < 3) ? count : 3;

            printf(CYAN"----------------------------------------------------------\n\n"RESET);
            if(choice == 1)
            {
                printf(PURPLE"Student ID        student CGPA     Tech quiz     Soft quiz\n"RESET);
                printf(CYAN"----------------------------------------------------------\n\n"RESET);

                for(i=0;i<limit;i++)
                {
                    printf("%-15s   %-8.2f            %-8d         %-8d\n",
                           students[i].id,
                           students[i].cgpa,
                           students[i].tech,
                           students[i].soft);
                }
            }

            else if(choice == 2)
            {
                printf(PURPLE"Student ID         Tech Marks\n"RESET);
                printf(CYAN"-------------------------------------------\n\n"RESET);

                for(i=0;i<limit;i++)
                {
                    printf("%-15s %-8d\n",
                           students[i].id,
                           students[i].tech);
                }
            }

            else if(choice == 3)
            {
                printf(PURPLE"Student ID         Soft Marks\n"RESET);
                printf(CYAN"-------------------------------------------\n\n"RESET);

                for(i=0;i<limit;i++)
                {
                    printf("%-15s %-8d\n",
                           students[i].id,
                           students[i].soft);
                }
            }

            else if(choice == 4)
            {
                printf(PURPLE"Student ID         CGPA\n"RESET);
                printf(CYAN"-------------------------------------------\n\n"RESET);

                for(i=0;i<limit;i++)
                {
                    printf("%-15s %-8.2f\n",
                           students[i].id,
                           students[i].cgpa);
                }
            }

           printf(CYAN"----------------------------------------------------------\n\n"RESET);
        }

    } while(choice != 5);
}

void technical_test(struct student all[], int total_students, int index)
{
    all[index].tech_q_count = 0;
    int c_score=0, python_score=0, cpp_score=0, java_score=0;

    printf(PURPLE"\n ===<>=== TECHNICAL ASSESSMENT ===<>===\n"RESET);

    printf(SKYBLUE"\n--- C programming language Test ---\n"RESET);
    c_score = quiz("c", all, index);

    printf(SKYBLUE"\n--- Python programming language Test ---\n"RESET);
    python_score = quiz("python", all, index);

    printf(SKYBLUE"\n--- C++ programming language Test ---\n"RESET);
    cpp_score = quiz("c++", all, index);

    printf(SKYBLUE"\n--- Java programming language Test ---\n"RESET);
    java_score = quiz("java", all, index);

    int total = c_score + python_score + cpp_score + java_score;

    all[index].c_marks = c_score;
    all[index].python_marks = python_score;
    all[index].cpp_marks = cpp_score;
    all[index].java_marks = java_score;
    all[index].total_tech_marks = total;

    printf(PURPLE"\n╔══════════════════════════════════════════════╗\n"RESET);
    printf(PURPLE"║           🏆 TECHNICAL SCOREBOARD 🏆         ║\n"RESET);
    printf(PURPLE"╠══════════════════════════════════════════════╣\n"RESET);

    printf("║ %-20s │ %-10s            ║\n", "Subject", "Score");
    printf("╠══════════════════════╪═══════════════════════╣\n");

    printf("║ %-20s │   %2d / 5              ║\n", "C Programming", c_score);
    printf("║ %-20s │   %2d / 5              ║\n", "Python", python_score);
    printf("║ %-20s │   %2d / 5              ║\n", "C++", cpp_score);
    printf("║ %-20s │   %2d / 5              ║\n", "Java", java_score);

    printf("╠══════════════════════╪═══════════════════════╣\n");
    printf("║ %-20s │  %2d / 20              ║\n", "TOTAL", total);

    printf(PURPLE"╚══════════════════════════════════════════════╝\n\n"RESET);

    progress_bar(total, 20);

    FILE *f = fopen("students.csv","w");

    fprintf(f,"ID,Name,Age,Course,Year,Sem1,Sem2,Sem3,Sem4,Sem5,Sem6,Sem7,Sem8,Overall,Cmarks,Pythonmarks,Cppmarks,Javamarks,TotalTechMarks,LogicalMath,Reasoning,TotalSoftMarks,Password\n");
    for(int i=0;i<total_students;i++)
    {
        fprintf(f,"%s,%s,%d,%s,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
                all[i].id,all[i].name,all[i].age,all[i].course,all[i].year,

                all[i].sem[0],all[i].sem[1],all[i].sem[2],all[i].sem[3],
                all[i].sem[4],all[i].sem[5],all[i].sem[6],all[i].sem[7],
                all[i].overall,

                all[i].c_marks,
                all[i].python_marks,
                all[i].cpp_marks,
                all[i].java_marks,
                all[i].total_tech_marks,

                all[i].logical_math_marks,
                all[i].reasoning_marks,
                all[i].total_soft_marks,
                all[i].password);    }
        fclose(f);
}

void softskill_test(struct student all[], int total_students, int index)
{
    all[index].soft_q_count = 0;
    int math_score, reasoning_score, time_score;

    printf(PURPLE"\n=o=o=o=o= SOFT SKILL ASSESSMENT =o=o=o=o=\n"RESET);

    printf(SKYBLUE"\n--- Logical Math ---\n"RESET);
    math_score = soft_quiz("LogicalMath", all, index);
    

    printf(SKYBLUE"\n--- Logical Reasoning ---\n"RESET);
    reasoning_score = soft_quiz("LogicalReasoning", all, index);

    int total = math_score + reasoning_score;

    all[index].logical_math_marks = math_score;
    all[index].reasoning_marks = reasoning_score;
    all[index].total_soft_marks = total;

    printf(PURPLE"\n╔══════════════════════════════════════════════╗\n"RESET);
    printf(PURPLE"║         🧠 SOFT SKILLS SCOREBOARD 🧠         ║\n"RESET);
    printf(PURPLE"╠══════════════════════════════════════════════╣\n"RESET);

    printf("║ %-25s │ %-10s       ║\n", "Section", "Score");
    printf("╠═══════════════════════════╪══════════════════╣\n");

    printf("║ %-25s │   %2d / 5         ║\n", "Logical Math", math_score);
    printf("║ %-25s │   %2d / 5         ║\n", "Logical Reasoning", reasoning_score);

    printf("╠═══════════════════════════╪══════════════════╣\n");
    printf("║ %-25s │  %2d / 10         ║\n", "TOTAL", total);

    printf(PURPLE"╚══════════════════════════════════════════════╝\n\n"RESET);

    progress_bar(total, 10);
}

void cgpa_section(struct student all[],int total_students,int index)
{
    FILE*f;

    if(all[index].overall>0)
    {
        printf("CGPA of this student is already entered.\n");
        printf(SKYBLUE"student Overall CGPA is : %.2f\n"RESET,all[index].overall);
    }
    else
    {
        int total_sem = all[index].year*2;
        float sum=0;

        for(int i=0; i<total_sem; i++)
        {
            printf("Enter CGPA of Semester %d : ",i+1);
            scanf("%f",&all[index].sem[i]);
            sum=sum+all[index].sem[i];
        }

        all[index].overall = sum / total_sem;

        printf("Student average CGPA : %.2f\n", all[index].overall);

        f = fopen("students.csv", "w");
        if(f==NULL)
        {
            printf("Error in opening file\n");
            return;
        }

        fprintf(f,"ID,Name,Age,Course,Year,Sem1,Sem2,Sem3,Sem4,Sem5,Sem6,Sem7,Sem8,Overall,Cmarks,Pythonmarks,Cppmarks,Javamarks,TotalTechMarks,LogicalMath,Reasoning,TotalSoftMarks,Password\n");
        for(int i = 0; i < total_students; i++) 
        {
            fprintf(f,"%s,%s,%d,%s,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
                    all[i].id,all[i].name,all[i].age,all[i].course,all[i].year,
                    all[i].sem[0],all[i].sem[1],all[i].sem[2],all[i].sem[3],
                    all[i].sem[4],all[i].sem[5],all[i].sem[6],all[i].sem[7],
                    all[i].overall,
                    all[i].c_marks,
                    all[i].python_marks,
                    all[i].cpp_marks,
                    all[i].java_marks,
                    all[i].total_tech_marks,
                    all[i].logical_math_marks,
                    all[i].reasoning_marks,
                    all[i].total_soft_marks,
                    all[i].password);
        fclose(f);

    }
    
    float max = 0;
    int count = 0;
    int rank = 1;
    char topper_name[50] = "None";
    int student_year = all[index].year;
    
    for(int i = 0; i < total_students; i++) 
    {

        if(all[i].year == student_year) 
        {

            count++;

            if(all[i].overall > max) 
            {
                max = all[i].overall;
                strcpy(topper_name, all[i].name);
            }

            if(all[i].overall > all[index].overall)
                rank++;
        }
    }

    printf(PURPLE"\n=<>=<>=<>=<>= YEAR %d RESULTS =<>=<>=<>=<>=\n"RESET, student_year);

    if(max==0)
    {
        printf(RED"CGPA data is not available\n"RESET);
    }
    else
    {
        printf(CREAM"Topper from the data : %s\n"RESET, topper_name);
        printf(CREAM"Topper's average CGPA : %.2f\n"RESET, max);
        printf(CREAM"Student Rank: %d out of %d students in Year %d\n"RESET,rank, count, student_year);
    }
}
}

    int quiz(char language[], struct student all[], int index)
    {
        struct question questions[500];
        int total_questions=0;

        FILE*fp=fopen("technical_quiz.csv","r");
        int q_index = 0;
        if(fp==NULL)
        {
            printf(RED"Error opening technical_quiz.csv\n"RESET);
            return 0;
        }

        char line[800];
        fgets(line,sizeof(line),fp);

        while(fgets(line,sizeof(line),fp))
        {
            struct question temp;

            sscanf(line,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d",
                    temp.language,
                    temp.level,
                    temp.question,
                    temp.opt1,
                    temp.opt2,
                    temp.opt3,
                    temp.opt4,
                    &temp.correct);

            if(strcmp(temp.language,language)==0)
            {
                questions[total_questions] = temp;
                total_questions++;
            }
        }

        fclose(fp);

        if(total_questions == 0)
        {
            printf("No questions found for %s\n", language);
            
            return 0;
        }

        int level_choice;
        char level[20];

        while(1)
        {
            printf(PURPLE"\nSelect the Level for your quiz:\n"RESET);
            printf("1. Easy\n");
            printf("2. Moderate\n");
            printf("3. Difficult\n");
            printf(YELLOW"Enter choice: "RESET);

            if(scanf("%d",&level_choice)!=1)
            {
                while(getchar()!='\n');  
                printf(RED"Invalid input! Enter numbers only.\n\n"RESET);
                continue;
            }

            if(level_choice==1)
            {
                strcpy(level,"easy");
                break;
            }
            else if(level_choice==2)
            {
                strcpy(level,"moderate");
                break;
            }
            else if(level_choice==3)
            {
                strcpy(level,"difficult");
                break;
            }
            else
            {
                printf(RED"Invalid option! Please select 1, 2, or 3.\n\n"RESET);
            }
        }

        int used[500]={0};
        int count=0,score=0;

        while(count<5)
        {
            int r=rand()%total_questions;

            if(used[r]==0 && strcmp(questions[r].level,level)==0)
            {
                used[r]=1;

                printf("\n%s\n",questions[r].question);
                printf("1. %s\n",questions[r].opt1);
                printf("2. %s\n",questions[r].opt2);
                printf("3. %s\n",questions[r].opt3);
                printf("4. %s\n",questions[r].opt4);

                int ans;

                while(1)
                {
                    printf("Enter your answer (1-4): ");
                    if(scanf("%d",&ans)!=1)
                    {
                        while(getchar()!='\n');
                        printf("Invalid input! Enter number.\n");
                        continue;
                    }

                    if(ans>=1 && ans<=4)
                        break;
                    else
                        printf(RED"Invalid choice! Enter 1 to 4 only.\n"RESET);
                }

                
                all[index].tech_answers[q_index] = ans;
                all[index].tech_correct[q_index] = questions[r].correct;
                q_index++;

                
                if(ans == questions[r].correct)
                {
                    printf(GREEN "✔ Correct!\n" RESET);
                    score++;
                }
                else
                {
                    printf(RED "✘ Wrong!\n" RESET);
                    printf(WHITE "Your answer: %d\n" RESET, ans);
                    printf(GREEN "Correct answer: %d\n" RESET, questions[r].correct);
                }
                count++;
                all[index].tech_q_count = q_index;
            }
        }

        printf("\nStudent score in %s: %d/5\n",language,score);
        return score;
    }

    int soft_quiz(char section[], struct student all[], int index)
    {
        FILE *fp = fopen("soft_quiz.csv","r");
        if(fp == NULL)
        {
            printf(RED"Error opening soft_quiz.csv\n"RESET);
            return 0;
        }
        int q_index = 0;
        char line[600];
        fgets(line,sizeof(line),fp);

        struct question questions[200];
        int total_questions = 0;

        while(fgets(line,sizeof(line),fp))
        {
            char sec[50];
            char question[400];
            char correct[100];

            sscanf(line,"%[^,],%[^,],%[^\n]",
                sec, question, correct);

            if(strcmp(sec,section)==0)
            {
                strcpy(questions[total_questions].question, question);
                questions[total_questions].correct = atoi(correct);
                total_questions++;
            }
        }

        fclose(fp);

        if(total_questions < 5)
        {
            printf(RED"Not enough questions in %s\n"RESET,section);
            return 0;
        }

        int used[200] = {0};
        int count = 0;
        int score = 0;

        while(count < 5)
        {
            int r = rand() % total_questions;

            if(used[r] == 0)
            {
                used[r] = 1;

                printf("\n%s\n",questions[r].question);

                int ans;

                while(1)
                {
                    printf("Enter your answer: ");

                    if(scanf("%d",&ans)!=1)
                    {
                        while(getchar()!='\n');
                        printf(RED"Invalid input! Enter number only.\n"RESET);
                        continue;
                    }
                    break;
                }

                
                all[index].soft_answers[q_index] = ans;
                all[index].soft_correct[q_index] = questions[r].correct;
                q_index++;

                
                if(ans == questions[r].correct)
                {
                    printf(GREEN "✔ Correct!\n" RESET);
                    score++;
                }
                else
                {
                    printf(RED "✘ Wrong!\n" RESET);
                    printf(WHITE "Your answer: %d\n" RESET, ans);
                    printf(GREEN "Correct answer: %d\n" RESET, questions[r].correct);
                }
                all[index].soft_q_count = q_index;
                
                count++;
            }
        }

        printf("\nStudent score in %s: %d/5\n",section,score);
        return score;
    }

void save_report_data(struct student s)
{
    FILE *fp = fopen("student_report.csv","a+");

    if(fp==NULL)
    {
        printf(RED"Error opening report file\n"RESET);
        return;
    }

    int attempt = 1;
    char id[50];
    int att;
    float cg;
    int tech, soft;

    rewind(fp);

    char line[200];
    fgets(line,sizeof(line),fp); 

    
    while(fscanf(fp,"%[^,],%d,%f,%d,%d,%*[^\n]\n",
                id,&att,&cg,&tech,&soft) == 5)
    {
        if(strcmp(id,s.id)==0)
        {
            if(att >= attempt)
                attempt = att + 1;
        }
    }

    fseek(fp,0,SEEK_END);

    if(ftell(fp)==0)
    {
        fprintf(fp,"ID,Attempt,CGPA,TechScore,SoftScore,C,Python,CPP,Java,Math,Reasoning,TechAns,SoftAns\n");
    }

    fprintf(fp,"%s,%d,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,",
        s.id,
        attempt,
        s.overall,
        s.total_tech_marks,
        s.total_soft_marks,
        s.c_marks,
        s.python_marks,
        s.cpp_marks,
        s.java_marks,
        s.logical_math_marks,
        s.reasoning_marks);

    
    for(int i=0;i<s.tech_q_count;i++)
    {
        fprintf(fp,"%d|%d", s.tech_answers[i], s.tech_correct[i]);
        if(i != s.tech_q_count-1) fprintf(fp,";");
    }

    fprintf(fp,",");

    
    for(int i=0;i<s.soft_q_count;i++)
    {
        fprintf(fp,"%d|%d", s.soft_answers[i], s.soft_correct[i]);
        if(i != s.soft_q_count-1) fprintf(fp,";");
    }

    fprintf(fp,"\n");

    fclose(fp);
}



    void display_student_report(char id[])
    {
        FILE *fp = fopen("student_report.csv","r");

        if(fp==NULL)
        {
            printf(RED"No Report Found\n"RESET);
            return;
        }

        char sid[50];
        int attempt;
        float cgpa;
        int tech,soft,c,py,cpp,java,math,reason;
        int found=0;

        char line[200];
        fgets(line,sizeof(line),fp); 

        printf(PURPLE"\n=o=o=o=o= PERFORMANCE REPORT =o=o=o=o=\n"RESET);
        printf("\nStudent ID: %s\n\n",id);
        printf(DARKGREEN"------------------------------------------------------------\n"RESET);
        printf(PURPLE"\nNo of attempts\tCGPA score\tTech result\tSoft result\n"RESET);
        printf(DARKGREEN"\n------------------------------------------------------------\n"RESET);

        char tech_ans[500], soft_ans[500];

           while(fscanf(fp,"%[^,],%d,%f,%d,%d,%d,%d,%d,%d,%d,%d,%[^,],%[^\n]\n",
            sid,&attempt,&cgpa,&tech,&soft,&c,&py,&cpp,&java,&math,&reason,
            tech_ans, soft_ans) == 13)
        {
            if(strcmp(sid,id)==0)
            {
                found=1;

                printf("%d        \t%.2f        \t%d          \t%d\n",
                    attempt,cgpa,tech,soft);

                print_vertical_graph(c,py,cpp,java,math,reason);
            }
        }

        if(!found)
            printf(RED"No Record Found\n"RESET);

        fclose(fp);
    }


        void print_vertical_graph(int c, int py, int cpp, int java, int math, int reason)
        {
            int max = 5;

            printf(PURPLE"\n=== Attempt Graph ===\n\n"RESET);

            for(int level = max; level >= 1; level--)
            {
                printf("%d | ", level);

                printf("  %s  ", (c >= level) ? "█" : " ");
                printf("  %s  ", (py >= level) ? "█" : " ");
                printf("  %s  ", (cpp >= level) ? "█" : " ");
                printf("  %s  ", (java >= level) ? "█" : " ");
                printf("  %s  ", (math >= level) ? "█" : " ");
                printf("  %s  ", (reason >= level) ? "█" : " ");

                printf("\n");
            }

            printf("   ------------------------------------------\n");

            printf("     C    Py   Cpp   Jv   Math  Rsn\n\n");
        }

        int certificate_exists(char id[])
        {
            FILE *fp = fopen("certificate.csv","r");

            if(fp == NULL)
                return 0;

            char sid[50];

            while(fscanf(fp,"%s",sid) != EOF)
            {
                if(strcmp(sid,id)==0)
                {
                    fclose(fp);
                    return 1;
                }
            }

            fclose(fp);
            return 0;
        }

        void save_certificate(char id[])
        {
            FILE *fp = fopen("certificate.csv","a");

            if(fp == NULL)
            {
                printf(RED"Error saving certificate\n"RESET);
                return;
            }

            fprintf(fp,"%s\n",id);

            fclose(fp);
        }

        void view_certificate(struct student s)
{

    if(!certificate_exists(s.id))
    {
        save_certificate(s.id);
    }

    printf("\n\n");
    printf(PINK"\n");
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║      ◆ STUDENT SKILL ENHANCEMENT PROGRAM ◆       ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║         ★ PARTICIPATION CERTIFICATE ★            ║\n");
    printf("║                                                  ║\n");
    printf("║            This is to certify that               ║\n");
    printf("║             Name : %-38s║\n"RESET, s.name);                                   
    printf("║                                                  ║\n");
    printf("║       has successfully participated in the       ║\n");
    printf("║        Student Skill Enhancement Program         ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf(RESET);

}


void loading()
{
    printf("Loading");
    for(int i=0;i<5;i++)
    {
        printf(".");
        Sleep(300);
    }
    printf("\n");
}


void progress_bar(int score, int total)
{
    int percent = (score * 100) / total;

    printf(CYAN"\nProgress: ["RESET);

    for(int i=0;i<percent/10;i++)
        printf(GREEN"█"RESET);

    for(int i=percent/10;i<10;i++)
        printf(" ");

    printf(CYAN"] %d%%\n\n"RESET, percent);
}