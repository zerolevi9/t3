#include "simulation.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {

  srand(time(NULL));

  // Variáveis para controlar o tempo e a entrada de pacientes
  int relatorio = 11; 
  int nextID = 1000;
  int max_time = 43200;
  float sec;
  char filename[10];
  patient *patient;

  // Solicita ao usuário a velocidade de impressão das atualizações de métricas
  printf("\nDigite a velocidade de impressão das Atualizações de Métricas [segundos]: ");
  scanf("%f", &sec);
  int sec2 = sec * 1000000;

  // Solicita ao usuário o nome do arquivo a ser lido
  printf("Digite o nome do arquivo: ");
  scanf("%s", filename);

  // Inicialização das estruturas de dados
  ListPatient *list_patient = ListPatient_create();
  QueueExams *exams = QueueExams_create();
  QueueReport *report = QueueReport_create();
  ListMachines *Machine = ListMachines_create();
  ListRadiologist *Radio = Radiologist_create();
  Log *log = create_log();

  // Abre o arquivo para leitura
  FILE *arquivo = fopen(filename, "r");

  // Verifica se o arquivo foi aberto com sucesso
  if (arquivo == NULL) {
    printf("Erro ao abrir arquivo.\n\n");
    return 1;
  }

  // Inicializa as máquinas e os radiologistas
  initializeMachines(5, Machine);
  initializeRadiologist(3, Radio);

  // Simulação do hospital ao longo do tempo
  for(int time=1; time <= max_time; time++) {

    // Chegada do paciente (20% de chance de chegar um paciente)
    if (rand() % 5 + 1 == 1) {

      char name[50];
      int age = 0;
      char cpf[20];

      fscanf(arquivo, "%49[^\n]\n%15[^\n]\n%d\n", name, cpf, &age);

        // Cria e registra um novo paciente
        patient = newPatient(name, cpf, age, nextID);
        msg_newPatient(log, time, patient);
        ListPatient_insert(list_patient, patient);

        // Adiciona o paciente à fila de exames
        QueueEnqueue(exams, nextID);
        nextID++;
    }

    // Executa as operações relacionadas aos exames e à radiologia
    insert_machines(Machine, exams, time);
    recordExam(report, Machine, time, log);
    insert_radio(Radio, report, time); 
    remove_radio(Radio, time, log);

    // Atualiza as métricas a cada 10 unidades de tempo e imprime na saída
    if((relatorio % 10) == 0){
      printMetrics(report);           
      sleepMicroseconds(sec2);
    }

    // Registra as métricas no log a cada unidade de tempo
    msg_Metrics(report, log, time);  
    relatorio = relatorio + 1;
  }

  // Salva o log em um arquivo
  save_log_to_file(log, "log.txt");

  // Libera a memória alocada e fecha o arquivo
  fclose(arquivo);
  qexam_free(exams);
  qreport_free(report);
  listradiologist_free(Radio);
  listmach_free(Machine);
  listpatient_free(list_patient);
  free(log);

  return 0;
}
