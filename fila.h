#ifndef FILA_H
#define FILA_H

typedef struct QueueExams QueueExams;
typedef struct ExamRecord ExamRecord;

typedef struct Patient patient;
typedef struct ListOfPatients ListPatient;

typedef struct ListOfMachines ListMachines;
typedef struct Machines Machines;

typedef struct QueueReport QueueReport;
typedef struct Pathologies Pathologie;

typedef struct ListOfRadiologist ListRadiologist;
typedef struct Radiologist Radiologist;

typedef struct list_node ListNode;
typedef struct Queue_Node QueueNode;

typedef struct LogEvent LogEvent;
typedef struct log Log;
typedef struct PatologyWaitTime PatologyWaitTime;

patient *newPatient(char *name, char *cpf, int age, int id);

ListPatient *ListPatient_create();

int ListPatient_size(ListPatient *list);

int ListEmpty(ListPatient *list);

void ListPatient_insert(ListPatient *list, patient *patients);

int ListPatient_remove(ListPatient *list, patient *patients);

void ListPatient_free(ListPatient *list);

QueueExams *QueueExams_create();

int QueueEmpty(QueueExams *q);

void QueueEnqueue(QueueExams *q, int newID);

void QueueDequeue(QueueExams *q);

void QueueFree(QueueExams *q);

int ListEmpty_Machines(ListMachines *m);

ListMachines *ListMachines_create();

void initializeMachines(int qtd, ListMachines *m);

void insert_machines(ListMachines *m, QueueExams *patient, int time);

int machine_disponible(ListMachines *machine);

QueueReport *QueueReport_create();

int QueueReportEmpty(QueueReport *q);

void Exam_Record(QueueReport *report, ListMachines *m, int time, Log *log);

Pathologie *Assessing_Pathologies();

int ListEmpty_Radiologist(ListRadiologist *r);

ListRadiologist *Radiologist_create();

void initializeRadiologist(int qtd, ListRadiologist *r);

void insert_radio(ListRadiologist *r, QueueReport *patient, int time);

void remove_radio(ListRadiologist *r, int time, Log *l);

void patient_print(ListPatient *l);

void QueueExams_print(QueueExams *exams);

void machine_print(ListMachines *machine);

void QueueReport_print(QueueReport *r);

void radio_print(ListRadiologist *radio);

int total_path(QueueReport *r, const char *p);

int tempWait_path(QueueReport *r, const char *p);

float averageReportTime(QueueReport *report);

void calculateAverageReportTimePerPatology(QueueReport *report, PatologyWaitTime *patologyWaitTimes, int numPatologies);

int examsBeyondTimeLimit(QueueReport *report, int timeLimit);

void printMetrics(QueueReport *report);

Log *create_log();

void log_event(Log *log, const char *message);

void save_log_to_file(const Log *log, const char *filename);

void listpatient_free(ListPatient *p);

void listmach_free(ListMachines *mach);

void listradiologist_free(ListRadiologist *radio);

void qexam_free(QueueExams *exam);

void qreport_free(QueueReport *report);

void sleepMicroseconds(unsigned long microseconds);

void save_log_to_file(const Log *log, const char *filename);

void log_event(Log *log, const char *message);

void msg_newPatient(Log *log, int time, patient *p);

void msg_record(ExamRecord *r, Log *log, int num);

void msg_radio(Log *log, Radiologist *radio);

void msg_Metrics(QueueReport *report, Log *log, int time);

#endif
