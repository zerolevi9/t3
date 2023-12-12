#include "fila.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura dos pacientes 
struct Patient {

  char name[50]; // Nome 
  char cpf[20];  // CPF
  int age;       // Idade
  int id;        // Código dos pacientes
};

// Lista de pacientes
struct ListOfPatients {
  int count;       // Contador
  ListNode *first; // Primeiro paciente
  ListNode *last;  // Ultimo paciente
};


struct list_node {
  patient *patients; 
  ListNode *next;  
  ListNode *prev;   
};

//Retém exclusivamente os IDs dos novos pacientes.
struct QueueExams {

  QueueNode *front; 
  QueueNode *rear;  
};


struct Queue_Node {
  int id;          
  QueueNode *next; //Próximo paciente da fila
};

/* Lista de maquinas */
struct ListOfMachines {
  int count;       //Contador
  Machines *first; //Primeira máquina
  Machines *last;  //Última máquina  
};

/* Estrutura das maquinas */
struct Machines {
  int examDuration; // Duração 
  int patientID;    // Paciente ocupando a máquina
  int time;
  Machines *next;
  Machines *prev;
};

struct QueueReport {
  ExamRecord *front;       //Inicial registro de exame na fila de laudos
  ExamRecord *rear;        //Mais recente de exame na fila de laudos
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

struct ListOfRadiologist{
  int count;      
  Radiologist *first; 
  Radiologist *last; 
};

struct Radiologist
{
  int occupation;
  int patientID;
  int durationRad;  // Duração do laudo
  int time;
  Radiologist *next;
  Radiologist *prev;
};


// Cria novos pacientes
patient *newPatient(char name[], char cpf[], int age, int id) {
    patient *newPatient = (patient *)malloc(sizeof(patient));  // Aloca memória para o registro do paciente 

    if (newPatient != NULL) {
        strcpy(newPatient->name, name);   //Define o nome
        strcpy(newPatient->cpf, cpf);     // Define o CPF
        newPatient->age = age;            // Define a idade 
        newPatient->id = id;              // Define o Cod
    }

    return newPatient;
}

// Cria Lista de pacientes
ListPatient *ListPatient_create() {
    ListPatient *list = (ListPatient *)malloc(sizeof(ListPatient)); // Aloca memória para a lista de pacientes

    if (list != NULL) {
        list->count = 0;      
        list->first = NULL;   
        list->last = NULL;    
    }

    return list;            // Lista de pacientes 
}


int ListPatient_size(ListPatient *list) {
  return list->count; 
};


int ListEmpty(ListPatient *list) { 
  return ListPatient_size(list) == 0; 
}

// Insere um novo paciente a lista 
void ListPatient_insert(ListPatient *list, patient *patients) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));  // Memória para cada nó de pacientes 
    if (node != NULL) {
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
}

// REmove paciente da lista 
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
            {
                prev->next = node->next;
            }
            free(node);
            list->count--; //Atualiza a quantidade de pacientes na lista 
            return 1;      // true
        } 
        else 
        {
            prev = node;
            node = node->next;
        }
    }

    return 0; // false
}

//Remove todos os pacientes da memória 
void ListPatient_free(ListPatient *list) {
  ListNode *node = list->first;

  while (node != NULL) {
    ListNode *temp = node->next;
    free(node);
    node = temp;
  }
  free(list);
};

// Cria a fila de exames 
QueueExams *QueueExams_create() {
  QueueExams *q = (QueueExams *)malloc(sizeof(QueueExams));   
  q->front = q->rear = NULL;                                  
  return q;                                                   
}

int QueueEmpty(QueueExams *q) { 
  return q->front == NULL; 
}


// Adiciona um novo paciente na fila
void QueueEnqueue(QueueExams *q, int newID) {
    QueueNode *node = (QueueNode *)calloc(1, sizeof(QueueNode)); 
    if (node != NULL) {
        node->id = newID;     
        node->next = NULL;     

        if (QueueEmpty(q))
            q->front = node;    
        else
            q->rear->next = node; 
        q->rear = node;       
    }
}

// Retira o primeiro paciente da fila de exames 
void QueueDequeue(QueueExams *q) {
  QueueNode *temp = q->front;  

  if (q->front == q->rear) {
    q->front = q->rear = NULL;  
  } else {
    q->front = q->front->next;   
  }
  free(temp); 
}

// Limpa a a fil
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

// Cria a lista de máquinas 
ListMachines *ListMachines_create() {
ListMachines *list_M = (ListMachines *)malloc(sizeof(ListMachines));
  list_M->count = 0;       
  list_M->first = NULL;    
  list_M->last = NULL; 
  return list_M;          
};

// Inicializa as máquinas de acordo com a quantidade de máquinas no hospital 
void initializeMachines(int qtd, ListMachines *m) {
    for (int i = 0; i < qtd; i++) {
        Machines *mach = (Machines *)malloc(sizeof(Machines));
        // Inicializa os campos da máquina
        mach->examDuration = 0;
        mach->patientID = 0;
        mach->time = 0;
        mach->next = m->first;
        mach->prev = NULL;
        // Verifica se a lista não está vazia
        if (!ListEmpty_Machines(m))
            m->first->prev = mach;
        else
            m->last = mach;
        // Atualiza o ponteiro para o primeiro elemento da lista
        m->first = mach;
        // Atualiza o contador de elementos na lista
        m->count++;
    }
}

//Verifica a disponibilidade dos aparelhos 
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

int QueueReportEmpty(QueueReport *report) { 
  return report->front == NULL; 
}

QueueReport *QueueReport_create() {
  QueueReport *report = (QueueReport *)malloc(sizeof(QueueReport));
  report->front = report->rear = NULL; /* Fila começa vazia */
  return report;
}

//Verifica se terminou o exame e transfere para a fila de laudo 
void Exam_Record(QueueReport *report, ListMachines *m, int time) {
    int check = machine_check(m, time);

    while (check != -1) {
        ExamRecord *r = (ExamRecord *)malloc(sizeof(ExamRecord));
        r->finishTime = time;
        r->id = check;
        r->path = Assessing_Pathologies();
        r->next = NULL;
        if (QueueReportEmpty(report)) {
            report->front = report->rear = r;
        } else {
            report->rear->next = r;
            report->rear = r;
        }
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

Pathologie *Assessing_Pathologies() {
  int numb_random = rand() % 100 + 1;
  char condition[20];
  switch (numb_random) {
  case 1 ... 30:
    return CreatePathologie(strcpy(condition,"Saúde Normal"), 1);
  case 31 ... 50:
    return CreatePathologie(strcpy(condition,"Bronquite"), 2);
  case 51 ... 70:
    return CreatePathologie(strcpy(condition,"Pneumonia"), 3);
  case 71 ... 85:
    return CreatePathologie(strcpy(condition,"Fratura de Fêmur"), 4);
  case 86 ... 100:
    return CreatePathologie(strcpy(condition, "Apendicite"), 4);
  default:
    return NULL;
  }
}

int ListEmpty_Radiologist(ListRadiologist *r){
  return (r->count == 0);
}

//Lista de radiologista 
ListRadiologist *Radiologist_create() {
  ListRadiologist *list_R = (ListRadiologist *)malloc(sizeof(ListRadiologist)); 
  list_R->count = 0;       
  list_R->first = NULL;     
  list_R->last = NULL;      
  return list_R;          
};

//Inicializa o radiologista
void initializeRadiologist(int qtd, ListRadiologist *r) {
    for (int i = 0; i < qtd; i++) {
        Radiologist *rad = (Radiologist *)malloc(sizeof(Radiologist));
        rad->occupation = 0;
        rad->patientID = 0;
        rad->time = 0;
        rad->durationRad = 0;
        rad->next = r->first;
        rad->prev = NULL;
        if (!ListEmpty_Radiologist(r)) {
            r->first->prev = rad;
        } else {
            r->last = rad;
        }
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

void insert_radio(ListRadiologist *r, QueueReport *patient, int time) {
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

void remove_radio(ListRadiologist *r,int time){
  for(Radiologist *radio = r->first; radio != NULL; radio = radio->next){

    if(time == (radio->durationRad + radio->time)){
      radio->durationRad = 0;
      radio->occupation = 0;
      radio->patientID = 0;
      radio->time = 0;
    }
  }
}

//Printa a lista de pacientes 
void patient_print(ListPatient *l){
   for (ListNode *p = l->first; p != NULL; p = p->next)
   {
    printf("Name: %s\n", p->patients->name);
    printf("CPF: %s\n", p->patients->cpf);
    printf("Age: %d\n", p->patients->age);
    printf("ID: %d\n", p->patients->id);
    printf("\n");
   }
   printf("\n");
}

// Printa a fila de exames 
void QueueExams_print(QueueExams *exams){
  for(QueueNode *patient = exams->front; patient != NULL; patient = patient->next  ){
    printf("%d ", patient->id);
  }
}

// Printa as maquinas 
void machine_print(ListMachines *machine){
  for(Machines *m = machine->first; m != NULL; m = m->next ){
    int ID = m->patientID;
    printf("Duração do exame: %d ID: %d Horario: %d Quantidade de maquinas: %d \n", m->examDuration, ID, m->time, machine->count);

  }
}

void QueueReport_print(QueueReport *r) {
  for (ExamRecord *rec = r->front; rec != NULL; rec = rec->next){

    printf("FILA DO LAUDO - ID do paciente: %d Horário: %d Condição: %s\n", rec->id, rec->finishTime, rec->path->condition);
  }
}
void radio_print(ListRadiologist *radio){
  for(Radiologist *r = radio->first; r != NULL; r = r->next ){
    int ID = r->patientID;
    printf("RADIOLOGISTA - Duração do exame: %d ID: %d Horario: %d Quantidade de maquinas: %d \n", r->durationRad, ID, r->time, radio->count);

  }
}

int total_path(QueueReport *r, const char *p){
  int cont = 0;
  for (ExamRecord *record = r->front; record != NULL; record = record->next){
    
    int result = strcmp(record->path->condition, p);
    if(result == 0){
      cont++;
    }
  }
  return cont;
}

int tempWait_path(QueueReport *r, const char *p){
  int cont = 0;
  for (ExamRecord *record = r->front; record != NULL; record = record->next){
    
    int result = strcmp(record->path->condition, p);
    if(result == 0){
      cont = cont + record->finishTime;
    }
  }
  return cont;
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

void listpatient_free(ListPatient *p){
   ListNode *node_pat = p->first;
   ListNode *temp;
   while (node_pat != NULL){
      temp = node_pat->next;
      free(node_pat);
      node_pat = temp;
   }
   free(p); 
}

void listmach_free(ListMachines *mach){
   Machines *node_mach = mach->first;
   Machines *temp;
   while (node_mach != NULL){
      temp = node_mach->next;
      free(node_mach);
      node_mach = temp;
   }
   free(mach); 
}

void listradiologist_free(ListRadiologist *radio){
   Radiologist *node_radio = radio->first;
   Radiologist *temp;
   while (node_radio != NULL){
      temp = node_radio->next;
      free(node_radio);
      node_radio = temp;
   }
   free(radio); 
}

void qexam_free(QueueExams *exam){
   QueueNode *node_exam = exam->front;
   while (node_exam != NULL){
      QueueNode *temp = node_exam->next;
      free(node_exam);           
      node_exam = temp;              
   }
   free(exam); 
}

void qreport_free(QueueReport *report){
   ExamRecord *record = report->front;
   while (record != NULL){
      ExamRecord *temp = record->next;
      free(record);
      record = temp;
   }
   free(report);
}
