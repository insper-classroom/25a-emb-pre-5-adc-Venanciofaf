#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"
QueueHandle_t xQueueData;

// não mexer! Alimenta a fila com os dados do sinal
void data_task(void *p) {
    vTaskDelay(pdMS_TO_TICKS(400));

    int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
    for (int i = 0; i < data_len; i++) {
        xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void process_task(void *p) {
    int data = 0;

    int i = 0;
    int j = 0;
    int vec_aux[50];
    int vec_final[50];
    int y; 
    while (true) {
        if (xQueueReceive(xQueueData, &data, 100)) {
            // implementar filtro aqui!

            printf("O Dado é :  %d \n" , data);
            
            if (i < 5){

                vec_aux[i] = data;
                printf("O Elemento do vetor é %d \n" , vec_aux[i]);
            }

            else{
                
    
                y = (int) ((vec_aux[i-1] + vec_aux[i-2] + vec_aux[i-3] + vec_aux[i-4] + vec_aux[i-5]) / 5);
                vec_aux[i] = data;
                vec_final[j] = y;
                //printf("Y é : %d \n" , y);
                printf("O Elemento do vetor é %d \n" , vec_final[j]);
                j++;
            }

            i = i + 1;
            
            //printf("I é %d \n" , i);
            // deixar esse delay!
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    xQueueData = xQueueCreate(64, sizeof(int));

    xTaskCreate(data_task, "Data task ", 4096, NULL, 1, NULL);
    xTaskCreate(process_task, "Process task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
