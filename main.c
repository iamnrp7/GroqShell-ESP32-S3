#include <string.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "driver/uart.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_timer.h"

#include "groq.h"

/* ================= USER CONFIG ================= */

#define WIFI_SSID       "WIFI_SSID" //CHANGE 
#define WIFI_PASSWORD   "PASSWORD" // CHANGE 
#define MAX_RETRY       5

/* =============================================== */

static const char *TAG = "WIFI_GROQ";

static EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static int retry_count = 0;

/* ================= UART INIT ================= */

static void uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM_0, 2048, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0,
                 UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE);
}

/* ================= GROQ TASK ================= */

void groq_task(void *pvParameters)
{
    char line[256];
    int idx = 0;
    uint8_t ch;

    ESP_LOGI("GROQ", "Groq interactive task started");
    printf("\n> ");
    fflush(stdout);

    while (1) {
        int len = uart_read_bytes(UART_NUM_0, &ch, 1, portMAX_DELAY);
        if (len <= 0) continue;

        /* ENTER */
        if (ch == '\r' || ch == '\n') {
            uart_write_bytes(UART_NUM_0, "\r\n", 2);

            if (idx == 0) {
                printf("> ");
                fflush(stdout);
                continue;
            }

            line[idx] = '\0';
            idx = 0;

            printf("You asked:\n%s\n", line);

            int64_t t_start = esp_timer_get_time();
            groq_send_prompt(line);
            int64_t t_end = esp_timer_get_time();

            printf("\n[Execution Time] %.2f ms\n",
                   (t_end - t_start) / 1000.0);

            printf("\n> ");
            fflush(stdout);
        }

        /* BACKSPACE */
        else if (ch == 0x08 || ch == 0x7F) {
            if (idx > 0) {
                idx--;
                uart_write_bytes(UART_NUM_0, "\b \b", 3);
            }
        }

        /* NORMAL CHARACTER */
        else {
            if (idx < sizeof(line) - 1) {
                line[idx++] = ch;
                uart_write_bytes(UART_NUM_0, (char *)&ch, 1);
            }
        }
    }
}

/* ================= WIFI EVENT HANDLER ================= */

static void wifi_event_handler(void *arg,
                               esp_event_base_t event_base,
                               int32_t event_id,
                               void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "WiFi started, connecting...");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (retry_count < MAX_RETRY) {
            esp_wifi_connect();
            retry_count++;
            ESP_LOGI(TAG, "Retrying WiFi...");
        } else {
            xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        retry_count = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

/* ================= WIFI INIT ================= */

static void wifi_init_sta(void)
{
    wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(
        wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY
    );

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to WiFi");

        xTaskCreatePinnedToCore(
            groq_task,
            "groq_task",
            12288,
            NULL,
            5,
            NULL,
            1
        );
    } else {
        ESP_LOGE(TAG, "WiFi connection failed");
    }
}

/* ================= APP MAIN ================= */

void app_main(void)
{
    /* Required for Wi-Fi (do NOT remove) */
    ESP_ERROR_CHECK(nvs_flash_init());

    uart_init();

    ESP_LOGI(TAG, "Starting WiFi + Groq demo");
    wifi_init_sta();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
