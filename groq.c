#include "groq.h"

#include <string.h>
#include <stdio.h>

#include "cJSON.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_crt_bundle.h"

/* ================= CONFIG ================= */

#define GROQ_API_KEY "your_groq_api_key"
#define GROQ_URL     "https://api.groq.com/openai/v1/chat/completions"

/* ========================================== */

static const char *TAG = "GROQ";

/* HTTP response buffer */
static char response_buffer[4096];
static int response_len = 0;

/* ================= HTTP EVENT HANDLER ================= */

static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    if (evt->event_id == HTTP_EVENT_ON_DATA && evt->data_len > 0) {
        if (response_len + evt->data_len < sizeof(response_buffer)) {
            memcpy(response_buffer + response_len,
                   evt->data,
                   evt->data_len);
            response_len += evt->data_len;
        }
    }
    return ESP_OK;
}

/* ================= GROQ REQUEST ================= */

void groq_send_prompt(const char *prompt)
{
    char post_data[512];

    /* Reset response buffer */
    response_len = 0;
    memset(response_buffer, 0, sizeof(response_buffer));

    /* Build JSON payload */
    snprintf(post_data, sizeof(post_data),
        "{"
        "\"model\":\"llama-3.1-8b-instant\","
        "\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}],"
        "\"max_tokens\":1500"
        "}",
        prompt);

    esp_http_client_config_t config = {
        .url = GROQ_URL,
        .method = HTTP_METHOD_POST,
        .event_handler = http_event_handler,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        .timeout_ms = 15000,
        .crt_bundle_attach = esp_crt_bundle_attach,  // TLS verification
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Authorization",
                              "Bearer " GROQ_API_KEY);
    esp_http_client_set_post_field(client,
                                   post_data,
                                   strlen(post_data));

    ESP_LOGI(TAG, "Sending prompt to Groq...");

    esp_err_t err = esp_http_client_perform(client);
    esp_http_client_cleanup(client);

    if (err != ESP_OK) {
        printf("\nAI RESPONSE\n------------------------------\n");
        printf("Request failed: %s\n", esp_err_to_name(err));
        printf("------------------------------\n");
        return;
    }

    /* ================= PARSE JSON ================= */

    response_buffer[response_len] = '\0';

    cJSON *root = cJSON_Parse(response_buffer);
    if (!root) {
        printf("\nAI RESPONSE\n------------------------------\n");
        printf("Invalid JSON response\n");
        printf("------------------------------\n");
        return;
    }

    cJSON *choices = cJSON_GetObjectItem(root, "choices");
    if (!choices || !cJSON_IsArray(choices)) {
        cJSON_Delete(root);
        printf("\nAI RESPONSE\n------------------------------\n");
        printf("No choices found\n");
        printf("------------------------------\n");
        return;
    }

    cJSON *choice = cJSON_GetArrayItem(choices, 0);
    cJSON *message = cJSON_GetObjectItem(choice, "message");
    cJSON *content = cJSON_GetObjectItem(message, "content");

    printf("\nAI RESPONSE\n------------------------------\n");
    if (content && cJSON_IsString(content)) {
        printf("%s\n", content->valuestring);
    } else {
        printf("No content received\n");
    }
    printf("------------------------------\n");

    cJSON_Delete(root);
}
