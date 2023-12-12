#include "fila.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {

  srand(time(NULL));

  int relatorio = 11; 
  int nextID = 1000;
  int max_time = 43200;
  float sec;
  char filename[10];
  patient *patient;

  printf("\nDigite a velocidade de impressão das Atualizações de Métricas [segundos]: ");
  scanf("%f", &sec);
  int sec2 = sec * 1000000;

  printf("Digite o nome do arquivo: ");
  scanf("%s", filename);
  ListPatient *list_patient = ListPatient_create();
  QueueExams *exams = QueueExams_create();
  QueueReport *report = QueueReport_create();
  ListMachines *Machine = ListMachines_create();
  ListRadiologist *Radio = Radiologist_create();
  Log *log = create_log();

  FILE *arquivo = fopen(filename, "r");

  if (arquivo == NULL) {
    printf("Erro ao abrir arquivo.\n\n");
    return 1;
  }

  initializeMachines(5, Machine);
  initializeRadiologist(3, Radio);

  for(int time=1; time <= max_time; time++) {
    if (rand() % 5 + 1 == 1) {

      char name[50];
      int age = 0;
      char cpf[20];

      fscanf(arquivo, "%49[^\n]\n%15[^\n]\n%d\n", name, cpf, &age);

        patient = newPatient(name, cpf, age, nextID);
        msg_newPatient(log, time, patient);
        ListPatient_insert(list_patient, patient);
        QueueEnqueue(exams, nextID);
        nextID++;
    }
    insert_machines(Machine, exams, time);
    recordExam(report, Machine, time, log);
    insert_radio(Radio, report, time); 
    remove_radio(Radio, time, log);
    if((relatorio % 10) == 0){
      printMetrics(report);           
      sleepMicroseconds(sec2);
    }
    msg_Metrics(report, log, time);  
    relatorio = relatorio + 1;
  }

  // Salva o log em um arquivo
  save_log_to_file(log, "log.txt");
  fclose(arquivo);
  qexam_free(exams);
  qreport_free(report);
  listradiologist_free(Radio);
  listmach_free(Machine);
  listpatient_free(list_patient);
  free(log);

  return 0;
}
