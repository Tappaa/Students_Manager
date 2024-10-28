#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_COURSE_PER_TERM 20

#define ACADEMIC_PROBATION_GRADE 1.5f
#define MAX_OVERALL_ACADEMIC_PROBATION_COUNT 4
#define MAX_CONTINUOUS_ACADEMIC_PROBATION_COUNT 3

#define AP 4.5f
#define A0 4.0f
#define AM 0.0f
#define BP 3.5f
#define B0 3.0f
#define BM 0.0f
#define CP 2.5f
#define C0 2.0f
#define CM 0.0f
#define DP 1.5f
#define D0 1.0f
#define DM 0.0f
#define F 0.0f
#define P 0.0f
#define NP 0.0f

struct this_term {
    int credit[MAX_COURSE_PER_TERM];
    char *grade[MAX_COURSE_PER_TERM];
};

struct grade {
    int credit;
    int registered_credit;
    float grade; // max 4.5, min 0.0
};

struct student {
    int id;
    char name[20];
    int telephone_country_code;
    int telephone_number;
    int birth_date[3]; // 0: year, 1: month, 2: day
    struct this_term this_term_info;
    struct grade overall;
    struct grade latest_term;
    int overall_academic_warning_count;
    int continuous_academic_warning_count;
    int status; // 1: active, 0: inactive, -1: deleted, 2: active (Academic Probation)
};

int get_student_age(struct student s) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900 - s.birth_date[0];
}

void show_student_info(struct student s) {
    printf("Student ID: %d\n", s.id);
    printf("Student Name: %s\n", s.name);
    printf("Student Telephone: +%d%d\n", s.telephone_country_code, s.telephone_number);
    printf("Student Birth Date: %d-%d-%d\n", s.birth_date[0], s.birth_date[1], s.birth_date[2]);
    printf("Student Age: %d\n", get_student_age(s));
    printf("Student Overall Credit: %d\n", s.overall.credit);
    printf("Student Overall Grade: %.2f\n", s.overall.grade);
    printf("Student Latest Term Credit: %d\n", s.latest_term.credit);
    printf("Student Latest Term Grade: %.2f\n", s.latest_term.grade);
    printf("Student Overall Academic Warning Count: %d\n", s.overall_academic_warning_count);
    printf("Student Continuous Academic Warning Count: %d\n", s.continuous_academic_warning_count);
    if (s.status == -1) {
        printf("Student Status: Deleted\n");
    } else if (s.status == 0) {
        printf("Student Status: Inactive\n");
    } else if (s.status == 1) {
        printf("Student Status: Active\n");
    } else if (s.status == 2) {
        printf("Student Status: Active (Academic Probation)\n");
    }
}

//// @deprecated Use update_student_score_auto instead.
float update_student_score_manual(struct student *s, float score) {
    s->overall.grade = score;
    return s->overall.grade;
}

float update_student_score_auto(struct student *s) {
    if (s->status == 1 || s->status == 2) {
        float full_score_f = s->overall.grade * (float)s->overall.credit;
        float this_term_score_f = 0;
        int full_credit = s->overall.credit;
        int this_term_credit = 0;

        int this_term_credit_calc = 0;
        int full_credit_calc = 0;

        // 이번 학기 credit 계산
        for (int i = 0; i < sizeof(s->this_term_info.credit) / sizeof(int); i++) {
            this_term_credit += s->this_term_info.credit[i];
        }
        this_term_credit_calc = this_term_credit;

        // 이번 학기 grade 계산
        for (int i = 0; i < sizeof(s->this_term_info.credit) / sizeof(int); i++) {
            // null check
            if (s->this_term_info.grade[i] == NULL) {
                break;
            }
//        printf("Grade: %s\n", s->this_term_info.grade[i]);
            if (strcmp(s->this_term_info.grade[i], "A+") == 0) {
                this_term_score_f += (AP * (float) s->this_term_info.credit[i]);
//            printf("A+ %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "A0") == 0) {
                this_term_score_f += (A0 * (float) s->this_term_info.credit[i]);
//            printf("A0 %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "A-") == 0) {
                this_term_score_f += (AM * (float) s->this_term_info.credit[i]);
//            printf("A- %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "B+") == 0) {
                this_term_score_f += (BP * (float) s->this_term_info.credit[i]);
//            printf("B+ %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "B0") == 0) {
                this_term_score_f += (B0 * (float) s->this_term_info.credit[i]);
//            printf("B0 %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "B-") == 0) {
                this_term_score_f += (BM * (float) s->this_term_info.credit[i]);
//            printf("B- %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "C+") == 0) {
                this_term_score_f += (CP * (float) s->this_term_info.credit[i]);
//            printf("C+ %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "C0") == 0) {
                this_term_score_f += (C0 * (float) s->this_term_info.credit[i]);
//            printf("C0 %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "C-") == 0) {
                this_term_score_f += (CM * (float) s->this_term_info.credit[i]);
//            printf("C- %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "D+") == 0) {
                this_term_score_f += (DP * (float) s->this_term_info.credit[i]);
//            printf("D+ %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "D0") == 0) {
                this_term_score_f += (D0 * (float) s->this_term_info.credit[i]);
//            printf("D0 %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "D-") == 0) {
                this_term_score_f += (DM * (float) s->this_term_info.credit[i]);
//            printf("D- %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "F") == 0) {
                this_term_score_f += (F * (float) s->this_term_info.credit[i]);
                this_term_credit -= s->this_term_info.credit[i];
//            printf("F %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "P") == 0) {
                this_term_score_f += (P * (float) s->this_term_info.credit[i]);
//            printf("P %lf\n", this_term_score_f);
            } else if (strcmp(s->this_term_info.grade[i], "NP") == 0) {
                this_term_score_f += (NP * (float) s->this_term_info.credit[i]);
                this_term_credit -= s->this_term_info.credit[i];
//            printf("NP %lf\n", this_term_score_f);
            } else {
                this_term_credit -= s->this_term_info.credit[i];
                this_term_credit_calc -= s->this_term_info.credit[i];
//            printf("Invalid grade: %s\n", s->this_term_info.grade[i]);
            }
        }

        full_score_f += this_term_score_f;
        full_credit_calc = full_credit + this_term_credit_calc;
        full_credit += this_term_credit;

        s->overall.grade = full_score_f / (float)full_credit_calc;
        s->overall.credit = full_credit;
        s->overall.registered_credit = full_credit_calc;

        float this_term_grade = this_term_score_f / (float)this_term_credit_calc;

        if (s->latest_term.registered_credit != 0 && (s->latest_term.grade < ACADEMIC_PROBATION_GRADE && this_term_grade < ACADEMIC_PROBATION_GRADE)) {
            s->status = 2;

            s->overall_academic_warning_count++;
            s->continuous_academic_warning_count++;
        } else {
            s->status = 1;
            s->continuous_academic_warning_count = 0;
        }

        if (s->continuous_academic_warning_count >= MAX_CONTINUOUS_ACADEMIC_PROBATION_COUNT) {
            s->status = -1;
            printf("This student is on academic probation for %d continuous terms\n", MAX_CONTINUOUS_ACADEMIC_PROBATION_COUNT);
            printf("Student status changed to be expelled(deleted)\n");
        }
        if (s->overall_academic_warning_count >= MAX_OVERALL_ACADEMIC_PROBATION_COUNT) {
            s->status = -1;
            printf("This student is on academic probation for %d overall terms\n", MAX_OVERALL_ACADEMIC_PROBATION_COUNT);
            printf("Student status changed to be expelled(deleted)\n");
        }

        s->latest_term.grade = this_term_grade;
        s->latest_term.credit = this_term_credit;
        s->latest_term.registered_credit = this_term_credit_calc;

        s->this_term_info = (struct this_term) {0};

        return s->overall.grade;
    } else {
        printf("Student is not active\n");
        return -1.0f;
    }
}

void update_student_status(struct student *s, int status, int reset_academic_probation) {
    s->status = status;

    if (reset_academic_probation == 1) {
        s->overall_academic_warning_count = 0;
        s->continuous_academic_warning_count = 0;
    } else if (reset_academic_probation == 2) {
        s->overall_academic_warning_count = 0;
    } else if (reset_academic_probation == 3) {
        s->continuous_academic_warning_count = 0;
    }
}

int main() {
    // Sample student
    struct student s = {20770922,
                        "John Doe",
                        82,
                        01011111111,
                        {2000, 10, 22},
                        {{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
                         {"A+","A+","A+","A+","A+","A+","A+","A+","A+","A+"}},
                        {0, 0, 0.0f},
                        {0, 0, 0.0f},
                        0,
                        0,
                        1};

    show_student_info(s);
    printf("\n");

    update_student_score_auto(&s);

    s.this_term_info = (struct this_term) {
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {"F","F","F","F","D0","F","C0","NP","NP","NP"}
    };

    show_student_info(s);
    printf("\n");

    update_student_score_auto(&s);

    show_student_info(s);
    printf("\n");

    s.this_term_info = (struct this_term) {
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {"F","F","F","F","D0","F","C0","NP","NP","NP"}
    };

    update_student_score_auto(&s);

    show_student_info(s);
    printf("\n");

    s.this_term_info = (struct this_term) {
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {"A0","A0","A0","A0","A0","A0","A0","A0","A0","A0"}
    };

    update_student_score_auto(&s);

    show_student_info(s);
    printf("\n");

    s.this_term_info = (struct this_term) {
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {"F","F","F","F","D0","F","C0","NP","NP","NP"}
    };

    update_student_score_auto(&s);

    show_student_info(s);
    printf("\n");

    s.this_term_info = (struct this_term) {
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {"F","F","F","F","D0","F","C0","NP","NP","NP"}
    };

    update_student_score_auto(&s);

    show_student_info(s);
    printf("\n");

    s.this_term_info = (struct this_term) {
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {"F","F","F","F","D0","F","C0","NP","NP","NP"}
    };

    update_student_score_auto(&s);

    show_student_info(s);
    printf("\n");

    s.this_term_info = (struct this_term) {
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {"F","F","F","F","D0","F","C0","NP","NP","NP"}
    };

    update_student_score_auto(&s);

    show_student_info(s);
    printf("\n");

    s.this_term_info = (struct this_term) {
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {"F","F","F","F","D0","F","C0","NP","NP","NP"}
    };

    update_student_score_auto(&s);

    show_student_info(s);
    // Sample student

    return 0;
}