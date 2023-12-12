#include "fila.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

  srand(time(NULL));

  int relatorio = 11; 
  int nextID = 1000;
  int max_time = 43200;
  float sec;
  char filename[10];
  patient *patient;

  printf("\nAtualizações de Métricas: ");
  scanf("%f", &sec);
  int sec2 = sec * 1000000;

  printf("Digite o nome do arquivo: ");
  scanf("%s", filename);
  ListPatient *list_patient = listapacientecriada();
  QueueExams *exams = queuecriarexames();
  QueueReport *report = criarqueuereport();
  ListMachines *Machine = listamaquinascriadas();
  ListRadiologist *Radio = criaradiologia();
  Log *log = createlog();

  FILE *arquivo = fopen(filename, "r");

  if (arquivo == NULL) {
    printf("Erro ao abrir arquivo.\n\n");
    return 1;
  }

  inicializamaquinas(5, Machine);
 iniciaradiologia(3, Radio);

  for(int time=1; time <= max_time; time++) {
    if (rand() % 5 + 1 == 1) {

      char name[50];
      int age = 0;
      char cpf[20];

      fscanf(arquivo, "%49[^\n]\n%15[^\n]\n%d\n", name, cpf, &age);

        patient = novopaciente(name, cpf, age, nextID);
        msgnovopaciente(log, time, patient);
        listainserepaciente(list_patient, patient);
        queueenqueue(exams, nextID);
        nextID++;
    }
    inseremaquinas(Machine, exams, time);
    Exam_Record(report, Machine, time, log);
    insereradio(Radio, report, time); 
    removeradio(Radio, time, log);
    if((relatorio % 10) == 0){
      printmetrics(report);           
      sleepmicroseconds(sec2);
    }
    msgmetrics(report, log, time);  
    relatorio = relatorio + 1;
  }

  // Salva o log em um arquivo
  save_log_to_file(log, "log.txt");
  fclose(arquivo);
  qexamfree(exams);
  qreportfree(report);
  listradiologist_free(Radio);
  listmach_free(Machine);
  listapaciente(list_patient);
  free(log);

  return 0;
}
