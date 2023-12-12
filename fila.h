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

patient *novopaciente(char *name, char *cpf, int age, int id);

ListPatient *listapacientecriada();

int tamanholistapaciente(ListPatient *list);

int listavazia(ListPatient *list);

void listainserepaciente(ListPatient *list, patient *patients);

int listaremovepaciente(ListPatient *list, patient *patients);

void listapacientefree(ListPatient *list);

QueueExams *queuecriarexames();

int queuevazia(QueueExams *q);

void queueenqueue(QueueExams *q, int newID);

void queuedequeue(QueueExams *q);

void queuefree(QueueExams *q);

int listamaquinasvazias(ListMachines *m);

ListMachines *listamaquinascriadas();

void inicializamaquinas(int qtd, ListMachines *m);

void inseremaquinas(ListMachines *m, QueueExams *patient, int time);

int maquinadisponivel(ListMachines *machine);

QueueReport *criarqueuereport();

int queuereportvazio(QueueReport *q);

void Exam_Record(QueueReport *report, ListMachines *m, int time, Log *log);

Pathologie *AssessingPathologies();

int listavaziaradiologia(ListRadiologist *r);

ListRadiologist *criaradiologia();

void inciaradiologia(int qtd, ListRadiologist *r);

void insereradio(ListRadiologist *r, QueueReport *patient, int time);

void removeradio(ListRadiologist *r, int time, Log *l);

void printpaciente(ListPatient *l);

void printqueueexam(QueueExams *exams);

void printmaquina(ListMachines *machine);

void queuereportprint(QueueReport *r);

void printradio(ListRadiologist *radio);

int total_path(QueueReport *r, const char *p);

int tempWait_path(QueueReport *r, const char *p);

float averageReportTime(QueueReport *report);

void calculateAverageReportTimePerPatology(QueueReport *report, PatologyWaitTime *patologyWaitTimes, int numPatologies);

int examsBeyondTimeLimit(QueueReport *report, int timeLimit);

void printmetrics(QueueReport *report);

Log *createlog();

void log_event(Log *log, const char *message);

void savelog_to_file(const Log *log, const char *filename);

void listapaciente(ListPatient *p);

void listmach_free(ListMachines *mach);

void listradiologist_free(ListRadiologist *radio);

void qexamfree(QueueExams *exam);

void qreportfree(QueueReport *report);

void sleepmicroseconds(unsigned long microseconds);

void savelog_to_file(const Log *log, const char *filename);

void log_event(Log *log, const char *message);

void msgnovopaciente(Log *log, int time, patient *p);

void msgrecord(ExamRecord *r, Log *log, int num);

void msgradio(Log *log, Radiologist *radio);

void msgmetrics(QueueReport *report, Log *log, int time);

#endif
