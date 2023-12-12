#include "fila.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Patient {

  char name[50]; 
  char cpf[20]; 
  int age;     
  int id;      
};


struct ListOfPatients {
  int count;     
  ListNode *first; 
  ListNode *last; 
};


struct list_node {
  patient *patients; 
  ListNode *next;    
  ListNode *prev;    
};

struct QueueExams {
  QueueNode *front; 
  QueueNode *rear;  
};


struct Queue_Node {
  int id;     
  QueueNode *next; 
};

struct ListOfMachines {
  int count;       
  Machines *first; 
  Machines *last;  
};

struct Machines {
  int examDuration; 
  int patientID;    
  int time;
  Machines *next;
  Machines *prev;
};

struct QueueReport {
  ExamRecord *front;      
  ExamRecord *rear;       
};

struct ExamRecord {
  int id;
  int finishTime;  
  Pathologie *path;
  ExamRecord *next;
};

struct Pathologies {

  char condition[20]; 
  int urgency;        
};

struct ListOfRadiologist {
  int count;      
  Radiologist *first; 
  Radiologist *last; 
};
struct Radiologist {
  int occupation;
  int patientID;
  int durationRad;  
  int time;
  Radiologist *next;
  Radiologist *prev;
};

struct PatologyWaitTime {
    char patology[20];
    int totalWaitTime;
    int numberOfExams;
};

struct LogEvent {
  char message[256];
  time_t timestamp;
};

struct log {
  LogEvent events[350000];
  int count;
};

patient *newPatient(char name[], char cpf[], int age, int id) {

  patient *newPatient = (patient *)malloc(sizeof(patient));  

  strcpy(newPatient->name, name); 
  strcpy(newPatient->cpf, cpf);   
  newPatient->age = age;            
  newPatient->id = id;              

  return newPatient;
};

/* Função que cria a lista de paciente */
ListPatient *ListPatient_create() {
ListPatient *list = (ListPatient *)malloc(sizeof(ListPatient));

  list->count = 0;        
  list->first = NULL;     
  list->last = NULL;      

  return list;            
};

int ListPatient_size(ListPatient *list) {
  return list->count;
};

int ListEmpty(ListPatient *list) { 
  return ListPatient_size(list) == 0; 
}

void ListPatient_insert(ListPatient *list, patient *patients) {

  ListNode *node = (ListNode *)malloc(sizeof(ListNode));   
  node->patients = patients;                              
  node->next = list->first;                         
  node->prev = NULL;                                     
  if (!ListEmpty(list))
    list->first->prev = node; 
  else
    list->last = node;       

  list->first = node;   
  list->count++;       
}

int ListPatient_remove(ListPatient *list, patient *patients) {

  ListNode *node = list->first; 
  ListNode *prev = NULL; 

  while (node != NULL) {
    if (node->patients == patients)  
    {
      if (prev == NULL) 
      {
        list->first = node->next;
      } 
      else
        prev->next = node->next;

      free(node);
      return 1; 
    } 
    else 
    {
      prev = node;
      node = node->next;
    }
  }

  return 0; 
}


void ListPatient_free(ListPatient *list) {
  ListNode *node = list->first;

  while (node != NULL) {
    ListNode *temp = node->next;
    free(node);
    node = temp;
  }
  free(list);
};

QueueExams *QueueExams_create() {
  QueueExams *q = (QueueExams *)malloc(sizeof(QueueExams));  
  q->front = q->rear = NULL;                                 
  return q;                         
}

int QueueEmpty(QueueExams *q) { 
  return q->front == NULL; 
}

void QueueEnqueue(QueueExams *q, int newID) {
  QueueNode *node = (QueueNode *)calloc(1,sizeof(QueueNode)); 
  node->id = newID;   
  node->next = NULL;    

  if (QueueEmpty(q))
    q->front = node;     
  else
    q->rear->next = node; 

  q->rear = node;         
}

void QueueDequeue(QueueExams *q) {
  QueueNode *temp = q->front;    

  if (q->front == q->rear) {
    q->front = q->rear = NULL;   
  } else {
    q->front = q->front->next;   
  }
  free(temp);  
}

void QueueFree(QueueExams *q) {
  QueueNode *p = q->front;

  while (p != NULL) {        
    QueueNode *temp = p->next; 
    free(p);
    p = temp;
  }
  free(q);
}

int ListEmpty_Machines(ListMachines *m){
  return (m->count == 0);
}


ListMachines *ListMachines_create() {
ListMachines *list_M = (ListMachines *)malloc(sizeof(ListMachines)); 

  list_M->count = 0;        
  list_M->first = NULL;    
  list_M->last = NULL;     

  return list_M;       
};

void initializeMachines(int qtd, ListMachines *m){
  for(int i = 0; i < qtd; i++ ){

    Machines *mach = (Machines *)malloc(sizeof(Machines));
    mach->examDuration = 0;
    mach->patientID = 0;
    mach->time = 0;
    mach->next = m->first;
    mach->prev = NULL;

    if (!ListEmpty_Machines(m))
      m->first->prev = mach;
    else
      m->last = mach;

    m->first = mach;
    m->count++;
  }
  
}


static Machines* checkMachinesAvailability(ListMachines *machine){ 

  if(!ListEmpty_Machines(machine)){
    for(Machines *m = machine->first; m != NULL; m = m->next ){
      if(m->examDuration == 0 || m->patientID == 0 || m->time == 0){
        return m;
      }
    }
  }
  return NULL;
}


void insert_machines(ListMachines *m, QueueExams *patient, int time) {
  Machines *mach = checkMachinesAvailability(m);

  if (mach != NULL) {
    QueueNode *pat = patient->front;

    if (pat != NULL && pat->id != 0) {

      int numb_rand = rand() % 5 + 5;
      mach->examDuration = numb_rand;
      mach->patientID = pat->id;
      mach->time = time;

      QueueDequeue(patient);
    }
  }
}


int machine_check(ListMachines *machine, int time) {
    for (Machines *m = machine->first; m != NULL; m = m->next) {
        if (m->examDuration + m->time == time) {
            int ID = m->patientID;
            m->patientID = 0;
            m->examDuration = 0;
            m->time = 0;
            return ID;
        }
    }
    return -1;
}

int machine_disponible(ListMachines *machine) {
    int count = 0;
    if (!ListEmpty_Machines(machine)) {
        for (Machines *m = machine->first; m != NULL; m = m->next) {
            if (m->examDuration == 0 || m->patientID == 0 || m->time == 0) {
                count++;
            }
        }
    }
    return count;
}


int QueueReportEmpty(QueueReport *report) { 
  return report->front == NULL; 
}


QueueReport *QueueReport_create() {
  QueueReport *report = (QueueReport *)malloc(sizeof(QueueReport));
  report->front = report->rear = NULL; /* Fila começa vazia */
  return report;
}


void recordExam(QueueReport *report, ListMachines *machineList, int currentTime, Log *log) {
    int availableMachineID;

    while ((availableMachineID = machine_check(machineList, currentTime)) != -1) {
        ExamRecord *examRecord = (ExamRecord *)malloc(sizeof(ExamRecord));
        examRecord->finishTime = currentTime;
        examRecord->id = availableMachineID;
        examRecord->path = Assessing_Pathologies(); 
        examRecord->next = NULL;

        if (QueueReportEmpty(report)) {
            report->front = report->rear = examRecord;
        } else {
            report->rear->next = examRecord;
            report->rear = examRecord;
        }

        msg_record(examRecord, log, machine_disponible(machineList));
    }
}

void QueueDequeue_report(QueueReport *q) {
  ExamRecord *temp = q->front;  

  if (q->front == q->rear) {
    q->front = q->rear = NULL;  
  } else {
    q->front = q->front->next;  
  }
  free(temp);  
}

static Pathologie *createPathologie(const char condition[20], int severity) {
    Pathologie *pathology = (Pathologie *)malloc(sizeof(Pathologie));
    strcpy(pathology->condition, condition);
    pathology->urgency = severity;
    return pathology;
}

Pathologie *Assessing_Pathologies() {
    int randomNum = rand() % 100 + 1;
    char condition[20];

    switch (randomNum) {
    case 1 ... 30:
        return createPathologie(strcpy(condition, "Saúde Normal"), 1);
    case 31 ... 50:
        return createPathologie(strcpy(condition, "Bronquite"), 2);
    case 51 ... 70:
        return createPathologie(strcpy(condition, "Pneumonia"), 3);
    case 71 ... 85:
        return createPathologie(strcpy(condition, "Fratura de Fêmur"), 4);
    case 86 ... 100:
        return createPathologie(strcpy(condition, "Apendicite"), 4);
    default:
        return NULL;
    }
}


int ListEmpty_Radiologist(ListRadiologist *r){
  return (r->count == 0);
}


ListRadiologist *Radiologist_create() {
  ListRadiologist *list_R = (ListRadiologist *)malloc(sizeof(ListRadiologist)); 

  list_R->count = 0;  
  list_R->first = NULL;  
  list_R->last = NULL;   
  return list_R;        
};

void initializeRadiologist(int qtd, ListRadiologist *r){
  for(int i = 0; i < qtd; i++ ){

    Radiologist *rad = (Radiologist *)malloc(sizeof(Radiologist));
    rad->occupation = 0;
    rad->patientID = 0;
    rad->time = 0;
    rad->durationRad = 0;
    rad->next = r->first;
    rad->prev = NULL;

    if (!ListEmpty_Radiologist(r))
      r->first->prev = rad;
    else
      r->last = rad;

    r->first = rad;
    r->count++;
  }
  
}

static Radiologist* checkRadiologistAvailability(ListRadiologist *radio){ 

  if(!ListEmpty_Radiologist(radio)){
    for(Radiologist *r = radio->first; r != NULL; r = r->next ){
      if(r->occupation == 0){
        return r;
      }
    }
  }
  return NULL;
}

void insert_radio(ListRadiologist *radiologists, QueueReport *patientQueue, int currentTime) {
    Radiologist *radiologist = checkRadiologistAvailability(radiologists);

    if (radiologist != NULL) {
        ExamRecord *patient = patientQueue->front;

        if (patient != NULL && patient->id != 0) {
            int randomDuration = rand() % 21 + 10;
            radiologist->durationRad = randomDuration;
            radiologist->patientID = patient->id;
            radiologist->time = currentTime;
            radiologist->occupation = 1;
            QueueDequeue_report(patientQueue);
        }
    }
}

void remove_radio(ListRadiologist *radiologists, int currentTime, Log *log) {
    for (Radiologist *radiologist = radiologists->first; radiologist != NULL; radiologist = radiologist->next) {
        if (currentTime == (radiologist->durationRad + radiologist->time)) {
            msg_radio(log, radiologist);
            radiologist->durationRad = 0;
            radiologist->occupation = 0;
            radiologist->patientID = 0;
            radiologist->time = 0;
        }
    }
}

void printPatients(ListPatient *patients) {
    for (ListNode *node = patients->first; node != NULL; node = node->next) {
        printf("Name: %s\n", node->patients->name);
        printf("CPF: %s\n", node->patients->cpf);
        printf("Age: %d\n", node->patients->age);
        printf("ID: %d\n", node->patients->id);
        printf("\n");
    }
    printf("\n");
}

void printExamQueue(QueueExams *exams) {
    for (QueueNode *node = exams->front; node != NULL; node = node->next) {
        printf("%d ", node->id);
    }
}

void printMachines(ListMachines *machines) {
    for (Machines *machine = machines->first; machine != NULL; machine = machine->next) {
        int patientID = machine->patientID;
        printf("Duração do exame: %d ID: %d Horario: %d Quantidade de maquinas: %d \n", machine->examDuration, patientID, machine->time, machines->count);
    }
}

void printQueueReport(QueueReport *report) {
    for (ExamRecord *record = report->front; record != NULL; record = record->next) {
        printf("FILA DO LAUDO - ID do paciente: %d Horário: %d Condição: %s\n", record->id, record->finishTime, record->path->condition);
    }
}

void printRadiologists(ListRadiologist *radiologists) {
    for (Radiologist *radiologist = radiologists->first; radiologist != NULL; radiologist = radiologist->next) {
        int patientID = radiologist->patientID;
        printf("RADIOLOGISTA - Duração do exame: %d ID: %d Horario: %d Quantidade de maquinas: %d \n", radiologist->durationRad, patientID, radiologist->time, radiologists->count);
    }
}

float averageReportTime(QueueReport *report) {
    if (QueueReportEmpty(report)) {
        return 0.0;
    }

    int totalReports = 0;
    int totalTime = 0;

    for (ExamRecord *record = report->front; record != NULL; record = record->next) {
        totalTime += record->finishTime;
        totalReports++;
    }

    return (float)totalTime / totalReports;
}

void averageReportTimePerPatology(QueueReport *report, PatologyWaitTime *patologyWaitTimes, int numPatologies) {
    if (QueueReportEmpty(report)) {
        return;
    }

    for (ExamRecord *record = report->front; record != NULL; record = record->next) {
        for (int i = 0; i < numPatologies; i++) {
            if (strcmp(record->path->condition, patologyWaitTimes[i].patology) == 0) {
                patologyWaitTimes[i].totalWaitTime += record->finishTime;
                patologyWaitTimes[i].numberOfExams++;
            }
        }
    }
}

int countExamsBeyondTimeLimit(QueueReport *report, int timeLimit) {
    int count = 0;

    for (ExamRecord *record = report->front; record != NULL; record = record->next) {
        if (record->finishTime > timeLimit) {
            count++;
        }
    }

    return count;
}

int examsBeyondTimeLimit(QueueReport *report, int timeLimit) {
    int count = 0;

    for (ExamRecord *record = report->front; record != NULL; record = record->next) {
        if (record->finishTime > timeLimit) {
            count++;
        }
    }

    return count;
}
s
void printMetrics(QueueReport *report) {
    printf("\nTempo médio de laudo: %.2ft\n", averageReportTime(report));


    const char *patologies[] = {"Saúde Normal", "Bronquite", "Pneumonia", "Fratura de Fêmur", "Apendicite"};
    int numPatologies = sizeof(patologies) / sizeof(patologies[0]);
    PatologyWaitTime patologyWaitTimes[numPatologies];
    for (int i = 0; i < numPatologies; i++) {
        strcpy(patologyWaitTimes[i].patology, patologies[i]);
        patologyWaitTimes[i].totalWaitTime = 0;
        patologyWaitTimes[i].numberOfExams = 0;
    }

    averageReportTimePerPatology(report, patologyWaitTimes, numPatologies);

    for (int i = 0; i < numPatologies; i++) {
        if (patologyWaitTimes[i].numberOfExams > 0) {
            float averageTime = (float)patologyWaitTimes[i].totalWaitTime / patologyWaitTimes[i].numberOfExams;
            printf("[TMP] - %s: %.2ft\n", patologyWaitTimes[i].patology, averageTime);
        }
    }

    int timeLimit = 7200;
    int examsBeyondLimit = examsBeyondTimeLimit(report, timeLimit);
    printf("Exames realizados após o limite : %d\n", timeLimit, examsBeyondLimit);
}
void listpatient_free(ListPatient *p) {
    ListNode *node = p->first;
    while (node != NULL) {
        ListNode *temp = node->next;
        free(node->patients);
        free(node);
        node = temp;
    }
    free(p);
}

void listmach_free(ListMachines *m) {
    Machines *node = m->first;
    while (node != NULL) {
        Machines *temp = node->next;
        free(node);
        node = temp;
    }
    free(m);
}

void listradiologist_free(ListRadiologist *r) {
    Radiologist *node = r->first;
    while (node != NULL) {
        Radiologist *temp = node->next;
        free(node);
        node = temp;
    }
    free(r);
}

void qexam_free(QueueExams *q) {
    QueueNode *node = q->front;
    while (node != NULL) {
        QueueNode *temp = node->next;
        free(node);
        node = temp;
    }
    free(q);
}

void examRecord_free(ExamRecord *record) {
    free(record->path);
    free(record);
}

void qreport_free(QueueReport *report) {
    ExamRecord *record = report->front;
    while (record != NULL) {
        ExamRecord *temp = record->next;
        examRecord_free(record);
        record = temp;
    }
    free(report);
}

void sleepMicroseconds(unsigned long microseconds) {
    struct timespec req;
    req.tv_sec = microseconds / 1000000;
    req.tv_nsec = (microseconds % 1000000) * 1000;
    nanosleep(&req, NULL);
}

Log* create_log() {
    Log *l = (Log *)malloc(sizeof(Log));
    l->count = 0;
    l->events[0].message[0] = '\0';
    return l;
}

void log_event(Log *log, const char *message) {
    if (log->count < 350000) {
        strncat(log->events[log->count].message, message, sizeof(log->events[log->count].message) - 1);
        log->events[log->count].timestamp = time(NULL);
        log->count++;
    } else {
        printf("Erro: Log está cheio. Não é possível adicionar mais eventos.\n");
    }
}

void save_log_to_file(const Log *log, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < log->count; i++) {
            fprintf(file, "%s", log->events[i].message);
        }
        fclose(file);
    } else {
        printf("Erro: Não foi possível abrir o arquivo %s para escrita.\n", filename);
    }
}

void msg_newPatient(Log *log, int time, patient *p) {
    char entry[255];
    snprintf(entry, sizeof(entry), "[TEMPO %dt : event] Entrada de novo paciente no hospital. [ID: %d   Nome: %s    CPF: %s    Idade: %d]\n", 
            time, p->id, p->name, p->cpf, p->age);
    log_event(log, entry);
}

void msg_record(ExamRecord *record, Log *log, int numMachines) {
    char entry[255];
    snprintf(entry, sizeof(entry), "[TEMPO %dt : event] Exame do paciente de ID %d realizado. [Condição: %s    Qtd. de Máquinas Disponíveis: %d]\n",
            record->finishTime, record->id, record->path->condition, numMachines);
    log_event(log, entry);
}

void msg_radio(Log *log, Radiologist *radiologist) {
    char entry[255];
    int totalTime = radiologist->durationRad + radiologist->time;
    snprintf(entry, sizeof(entry), "[TEMPO %dt : event] Laudo de Exame do paciente de ID %d finalizado pelo radiologista. [Duração do laudo: %dt]\n", totalTime, radiologist->patientID, radiologist->durationRad);
    log_event(log, entry);
}

void initializePatologyWaitTimes(PatologyWaitTime *patologyWaitTimes, const char *patologies[], int numPatologies) {
    for (int i = 0; i < numPatologies; i++) {
        PatologyWaitTime *currentPatology = &patologyWaitTimes[i];
        strcpy(currentPatology->patology, patologies[i]);
        currentPatology->totalWaitTime = 0;
        currentPatology->numberOfExams = 0;
    }
}


void calculateAverageReportTimePerPatology(QueueReport *report, PatologyWaitTime *patologyWaitTimes, int numPatologies) {
    averageReportTimePerPatology(report, patologyWaitTimes, numPatologies);
}

void printMetricsByPatology(Log *log, PatologyWaitTime *patologyWaitTimes, int numPatologies, int time) {
    char entry[500];

    for (int i = 0; i < numPatologies; i++) {
        PatologyWaitTime *currentPatology = &patologyWaitTimes[i];
        if (currentPatology->numberOfExams > 0) {
            float averageTime = (float)currentPatology->totalWaitTime / currentPatology->numberOfExams;
            snprintf(entry, sizeof(entry), "[TEMPO %dt : metric] (TMP) Tempo médio de laudo - %s: %.2ft\n", time, currentPatology->patology, averageTime);
            log_event(log, entry);
        }
    }
}

void printAdditionalMetrics(Log *log, QueueReport *report, int time) {
    char entry[500];
    float storage;

    int timeLimit = 7200;
    int examsBeyondLimit = examsBeyondTimeLimit(report, timeLimit);

    entry[0] = '\0';

    if ((storage = averageReportTime(report)) != 0) {
        snprintf(entry + strlen(entry), sizeof(entry) - strlen(entry), "[TEMPO %dt : metric] (TML) Tempo médio de laudo: %.2ft\n", time, storage);
    }

    if (examsBeyondLimit != 0) {
        snprintf(entry + strlen(entry), sizeof(entry) - strlen(entry), "[TEMPO %dt : metric] (QEL) Qtd. de exames realizados após o limite de tempo estabelecido (Tempo limite = %dt): %d\n",
                time, timeLimit, examsBeyondLimit);
    }

    log_event(log, entry);
}

void msg_Metrics(QueueReport *report, Log *log, int time) {
    const char *patologies[] = {"Saúde Normal", "Bronquite", "Pneumonia", "Fratura de Fêmur", "Apendicite"};
    int numPatologies = sizeof(patologies) / sizeof(patologies[0]);

    PatologyWaitTime patologyWaitTimes[numPatologies];
    initializePatologyWaitTimes(patologyWaitTimes, patologies, numPatologies);

    calculateAverageReportTimePerPatology(report, patologyWaitTimes, numPatologies);
    printMetricsByPatology(log, patologyWaitTimes, numPatologies, time);

    printAdditionalMetrics(log, report, time);
}
