#include "fila.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

patient *novopaciente(char name[], char cpf[], int age, int id) {

  patient *novopaciente = (patient *)malloc(sizeof(patient)); 
  strcpy(novopaciente->name, name);  
  strcpy(novopaciente->cpf, cpf);     
    novopaciente->age = age;            
    novopaciente->id = id;  
  return novopaciente;
};


ListPatient *listapacientecriada() {
ListPatient *list = (ListPatient *)malloc(sizeof(ListPatient)); 
  list->count = 0;        
  list->first = NULL;    
  list->last = NULL;      
  return list;           
};

int tamanholistapaciente(ListPatient *list) {
  return list->count; 
};

int listavazia(ListPatient *list) { 
  return tamanholistapaciente(list) == 0; 
}

void listainserepaciente(ListPatient *list, patient *patients) {

  ListNode *node = (ListNode *)malloc(sizeof(ListNode));     
  node->patients = patients;                                 
  node->next = list->first;                                  
  node->prev = NULL;                                        

  if (!listavazia(list))
    list->first->prev = node;   
  else
    list->last = node;         

  list->first = node;          
  list->count++;                
}

int listaremovepaciente(ListPatient *list, patient *patients) {

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


void lsitapaciente(ListPatient *list) {
  ListNode *node = list->first;

  while (node != NULL) {
    ListNode *temp = node->next;
    free(node);
    node = temp;
  }
  free(list);
};

QueueExams *queuecriarexames() {
  QueueExams *q = (QueueExams *)malloc(sizeof(QueueExams));   
  q->front = q->rear = NULL;                                  
  return q;                                                  
}

int queuevazia(QueueExams *q) { 
  return q->front == NULL; 
}

void queueenqueue(QueueExams *q, int newID) {
  QueueNode *node = (QueueNode *)calloc(1,sizeof(QueueNode));
  node->id = newID;      
  node->next = NULL;      
  if (queuevazia(q))
    q->front = node;      
  else
    q->rear->next = node; 
  q->rear = node;         
}


void queuedequeue(QueueExams *q) {
  QueueNode *temp = q->front;    

  if (q->front == q->rear) {
    q->front = q->rear = NULL;   
  } else {
    q->front = q->front->next;   
  }
  free(temp);  
}


void queuefree(QueueExams *q) {
  QueueNode *p = q->front;

  while (p != NULL) {          
    QueueNode *temp = p->next;
    free(p);
    p = temp;
  }
  free(q);
}

int listamaquinasvazias(ListMachines *m){
  return (m->count == 0);
}

ListMachines *listamaquinascriadas() {
ListMachines *list_M = (ListMachines *)malloc(sizeof(ListMachines)); 

  list_M->count = 0;        
  list_M->first = NULL;     
  list_M->last = NULL;      
  return list_M;           
};


void inicializamaquinas(int qtd, ListMachines *m){
  for(int i = 0; i < qtd; i++ ){

    Machines *mach = (Machines *)malloc(sizeof(Machines));
    mach->examDuration = 0;
    mach->patientID = 0;
    mach->time = 0;
    mach->next = m->first;
    mach->prev = NULL;
    if (!listamaquinasvazias(m))
      m->first->prev = mach;
    else
      m->last = mach;

    m->first = mach;
    m->count++;
  }

}


static Machines* checkMachinesAvailability(ListMachines *machine){ 

  if(!listamaquinasvazias(machine)){
    for(Machines *m = machine->first; m != NULL; m = m->next ){
      if(m->examDuration == 0 || m->patientID == 0 || m->time == 0){
        return m;
      }
    }
  }
  return NULL;
}


void inseremaquinas(ListMachines *m, QueueExams *patient, int time) {
  Machines *mach = checkMachinesAvailability(m);

  if (mach != NULL) {
    QueueNode *pat = patient->front;

    if (pat != NULL && pat->id != 0) {

      int numb_rand = rand() % 5 + 5;
      mach->examDuration = numb_rand;
      mach->patientID = pat->id;
      mach->time = time;

      queuedequeue(patient);
    }
  }
}


int machine_check(ListMachines *machine, int time){
  for(Machines *m = machine->first; m != NULL; m = m->next ){
    if(m->examDuration + m->time == time){
      int ID = m->patientID;
      m->patientID = 0;
      m->examDuration = 0;
      m->time = 0;
      return ID;
    }
  }
  return -1;
}

int maquinadisponivel(ListMachines * machine){
  int count = 0;
  if(!listamaquinasvazias(machine)){
      for(Machines *m = machine->first; m != NULL; m = m->next ){
        if(m->examDuration == 0 || m->patientID == 0 || m->time == 0){
          count++;
        }
      }
    }
  return count;
}

int queuereportvazio(QueueReport *report) { 
  return report->front == NULL; 
}


QueueReport *criarqueuereport() {
  QueueReport *report = (QueueReport *)malloc(sizeof(QueueReport));
  report->front = report->rear = NULL; 
  return report;
}


void Exam_Record(QueueReport *report, ListMachines *m, int time, Log *log) {
  int check = machine_check(m, time);

  while (check != -1) {
    ExamRecord *record = (ExamRecord *)malloc(sizeof(ExamRecord));
    record->finishTime = time;
    record->id = check;
    record->path = AssessingPathologies();
    record->next = NULL;

    /* Adicionando na fila de registro */
    if (queuereportvazio(report)) {
      report->front = report->rear = record;
    } else {
      report->rear->next = record;
      report->rear = record;
    }

    msgrecord(record, log, maquinadisponivel(m));
    check = machine_check(m, time);
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

static Pathologie *CreatePathologie(char condition[20], int severity) {

  Pathologie *p = (Pathologie *)malloc(sizeof(Pathologie));
  strcpy(p->condition, condition);
  p->urgency = severity;
  return p;
}

Pathologie *AssessingPathologies() {
  int numb_random = rand() % 100 + 1;
  char condition[20];

  switch (numb_random) {
    case 1 ... 30:
      return CreatePathologie(strcpy(condition, "Saúde Normal"), 1);
    case 31 ... 50:
      return CreatePathologie(strcpy(condition, "Bronquite"), 2);
    case 51 ... 70:
      return CreatePathologie(strcpy(condition, "Pneumonia"), 3);
    case 71 ... 85:
      return CreatePathologie(strcpy(condition, "Fratura de Fêmur"), 4);
    case 86 ... 100:
      return CreatePathologie(strcpy(condition, "Apendicite"), 5); 
    default:
      return NULL;
  }
}


int listavaziaradiologia(ListRadiologist *r){
  return (r->count == 0);
}

ListRadiologist *criaradiologia() {
  ListRadiologist *list_R = (ListRadiologist *)malloc(sizeof(ListRadiologist));

  list_R->count = 0;
  list_R->first = NULL;
  list_R->last = NULL;

  return list_R;
}


void iniciaradiologia(int qtd, ListRadiologist *r) {
  for (int i = 0; i < qtd; i++) {
    Radiologist *rad = (Radiologist *)malloc(sizeof(Radiologist));
    rad->occupation = 0;
    rad->patientID = 0;
    rad->time = 0;
    rad->durationRad = 0;
    rad->next = r->first;
    rad->prev = NULL;

    if (!listavaziaradiologia(r)) {
      r->first->prev = rad;
    } else {
      r->last = rad;
    }

    r->first = rad;
    r->count++;
  }
}



static Radiologist* checkRadiologistAvailability(ListRadiologist *radio){ 

  if(!listavaziaradiologia(radio)){
    for(Radiologist *r = radio->first; r != NULL; r = r->next ){
      if(r->occupation == 0){
        return r;
      }
    }
  }
  return NULL;
}


void insereradio(ListRadiologist *r, QueueReport *patient, int time) {
  Radiologist *radio = checkRadiologistAvailability(r);

  if (radio != NULL) {
    ExamRecord *pat = patient->front;

    if (pat != NULL && pat->id != 0) {

      int numb_rand = rand() % 21 + 10;
      radio->durationRad = numb_rand;
      radio->patientID = pat->id;
      radio->time = time;
      radio->occupation = 1;
      QueueDequeue_report(patient);
    }
  }
}


void removeradio(ListRadiologist *r, int time, Log *l) {
  for (Radiologist *radio = r->first; radio != NULL; radio = radio->next) {
    if (time == (radio->durationRad + radio->time)) {
      msgradio(l, radio);
      radio->durationRad = 0;
      radio->occupation = 0;
      radio->patientID = 0;
      radio->time = 0;
    }
  }
}




void printpaciente(ListPatient *l) {
  for (ListNode *p = l->first; p != NULL; p = p->next) {
    printf("Name: %s\n", p->patients->name);
    printf("CPF: %s\n", p->patients->cpf);
    printf("Age: %d\n", p->patients->age);
    printf("ID: %d\n", p->patients->id);
    printf("\n");
  }
  printf("\n");
}



void printqueueexam(QueueExams *exams){

  for(QueueNode *patient = exams->front; patient != NULL; patient = patient->next  ){
    printf("%d ", patient->id);
  }
}

void printmaquina(ListMachines *machine){
  for(Machines *m = machine->first; m != NULL; m = m->next ){
    int ID = m->patientID;
    printf("Duração do exame: %d ID: %d Horario: %d Quantidade de maquinas: %d \n", m->examDuration, ID, m->time, machine->count);

  }
}

void queuereportprint(QueueReport *r) {
  for (ExamRecord *rec = r->front; rec != NULL; rec = rec->next){

    printf("ID do paciente: %d Horário: %d Condição: %s\n", rec->id, rec->finishTime, rec->path->condition);
  }
}

void printradio(ListRadiologist *radio){
  for(Radiologist *r = radio->first; r != NULL; r = r->next ){
    int ID = r->patientID;
    printf("Duração do exame: %d ID: %d Horario: %d Quantidade de maquinas: %d \n", r->durationRad, ID, r->time, radio->count);

  }
}

float averageReportTime(QueueReport *report) {
    if (queuereportvazio(report)) {
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
    if (queuereportvazio(report)) {
        return;
    }

    for (ExamRecord *record = report->front; record != NULL; record = record->next) {
        for (int i = 0; i < numPatologies; i++) {
            if (strcmp(record->path->condition, patologyWaitTimes[i].patology) == 0) {
                patologyWaitTimes[i].totalWaitTime += record->finishTime;
                patologyWaitTimes[i].numberOfExams++;
                break;  // Uma vez que a condição foi encontrada, não é necessário continuar procurando
            }
        }
    }
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


void printmetrics(QueueReport *report) {
    printf("\n*Tempo médio de laudo: %.2ft\n", averageReportTime(report));

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
            printf("%s: %.2ft\n", patologyWaitTimes[i].patology, averageTime);
        }
    }

    int timeLimit = 7200;
    int examsBeyondLimit = examsBeyondTimeLimit(report, timeLimit);

    printf("Exames realizados após o limite (%dt): %d\n", timeLimit, examsBeyondLimit);
}

void listapaciente(ListPatient *p) {
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
    if (m == NULL) {
        return;  // Verificação de ponteiro nulo
    }
    Machines *node = m->first;
    while (node != NULL) {
        Machines *temp = node->next;
        free(node);
        node = temp;
    }
    free(m);
}

void listradiologist_free(ListRadiologist *r) {
    if (r == NULL) {
        return;  // Verificação de ponteiro nulo
    }
    Radiologist *node = r->first;
    while (node != NULL) {
        Radiologist *temp = node->next;
        free(node);
        node = temp;
    }
    free(r);
}
void qexamfree(QueueExams *q) {
    if (q == NULL) {
        return;  
    }
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

void qreportfree(QueueReport *report) {
    ExamRecord *record = report->front;
    while (record != NULL) {
        ExamRecord *temp = record->next;
        examRecord_free(record);
        record = temp;
    }
    free(report);
}

void sleepmicroseconds(unsigned long microseconds) {
    struct timespec req;
    req.tv_sec = microseconds / 1000000;
    req.tv_nsec = (microseconds % 1000000) * 1000;
    nanosleep(&req, NULL);
}


Log* createlog() {
    Log *l = (Log *)malloc(sizeof(Log));
    l->count = 0;
    l->events[0].message[0] = '\0';
    return l;
}

void log_event(Log *log, const char *message) {
    if (log->count < 350000) {
        size_t remainingSpace = sizeof(log->events[log->count].message) - strlen(log->events[log->count].message) - 1;
        strncat(log->events[log->count].message, message, remainingSpace);

        log->events[log->count].timestamp = time(NULL);
        log->count++;
    } else {
        printf("Erro: Log cheio.\n");
    }
}

void save_log_to_file(const Log *log, const char *filename) {
    if (log == NULL) {
        printf("Erro: Ponteiro de log nulo.\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < log->count; i++) {
            fprintf(file, "%s\n", log->events[i].message);
        }
        fclose(file);
    } else {
        printf("Erro: Não foi possível abrir o arquivo %s para escrita.\n", filename);
    }
}

void msgnovopaciente(Log *log, int time, patient *p) {
    char entry[255]; 

    snprintf(entry, sizeof(entry), "%dt: Entrada de novo paciente no hospital. ID:%d   Nome:%s    CPF:%s  Idade:%d]\n", 
            time, p->id, p->name, p->cpf, p->age);
    log_event(log, entry);
}

void msgrecord(ExamRecord *r, Log *log, int num) {
    if (r == NULL || log == NULL) {
        printf("Erro: Ponteiro nulo detectado em msgrecord.\n");
        return;
    }

    char entry[255];
    snprintf(entry, sizeof(entry), "[%dt : evento] Exame do paciente de ID %d realizado. [Condição: %s    Quantidade de Máquinas Disponíveis: %d]\n",
            r->finishTime, r->id, r->path->condition, num);
    log_event(log, entry);
}

void msgradio(Log *log, Radiologist *radio){

  char entry[255];
  snprintf(entry, sizeof(entry), "[%dt : evento] Laudo de Exame do paciente de ID %d finalizado pelo radiologista. [Duração do laudo: %dt]\n", (radio->durationRad + radio->time), radio->patientID, radio->durationRad);
  log_event(log,entry);
}

void msgmetrics(QueueReport *report, Log *log, int time) {
    char entry[500];
    float storage;

    const char *patologies[] = {"Saúde Normal", "Bronquite", "Pneumonia", "Fratura de Fêmur", "Apendicite"};
    int numPatologies = sizeof(patologies) / sizeof(patologies[0]);

    // Inicializar a estrutura para armazenar informações sobre cada patologia
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
            snprintf(entry, sizeof(entry), "%dt :Tempo médio de laudo - %s: %.2ft\n", time, patologyWaitTimes[i].patology, averageTime);
            log_event(log, entry);
        }
    }

    int timeLimit = 7200;
    int examsBeyondLimit = examsBeyondTimeLimit(report, timeLimit);
    entry[0] = '\0';

    if ((storage = averageReportTime(report)) != 0) {
        snprintf(entry + strlen(entry), sizeof(entry) - strlen(entry), "[%dt : metric]Tempo médio de laudo: %.2ft\n", time, storage);
    }

    if (examsBeyondLimit != 0) {
        snprintf(entry + strlen(entry), sizeof(entry) - strlen(entry), "[%dt : metric]Quantidade de exames realizados após o limite de tempo estabelecido (%dt): %d\n", 
                time, timeLimit, examsBeyondLimit);
    }

  log_event(log, entry);
}
