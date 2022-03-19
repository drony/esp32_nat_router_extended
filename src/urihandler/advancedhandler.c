#include "handler.h"
#include <sys/param.h>
#include "router_globals.h"
static const char *TAG = "Advancedhandler";

esp_err_t advanced_download_get_handler(httpd_req_t *req)
{
    if (isLocked())
    {
        return unlock_handler(req);
    }

    httpd_req_to_sockfd(req);
    extern const char advanced_start[] asm("_binary_advanced_html_start");
    extern const char advanced_end[] asm("_binary_advanced_html_end");
    const size_t advanced_html_size = (advanced_end - advanced_start);

    // char *display = NULL;

    int param_count = 1;

    int keepAlive = 0;
    char *aliveCB = "";
    get_config_param_int("keep_alive", &keepAlive);
    size_t size = param_count * 2; //%s for parameter substitution

    if (keepAlive == 1)
    {
        aliveCB = "checked";
    }

    size = size + strlen(aliveCB); //+ strlen(textColor) + strlen(clients) + strlen(db);
    ESP_LOGI(TAG, "Allocating additional %d bytes for config page.", advanced_html_size + size);
    char *advanced_page = malloc(advanced_html_size + size);
    sprintf(advanced_page, advanced_start, aliveCB);
    closeHeader(req);
    esp_err_t ret = httpd_resp_send(req, advanced_page, strlen(advanced_page) - (param_count * 2)); // -2 for every parameter substitution (%s)
    free(advanced_page);
    free(aliveCB);
    // free(clients);
    // free(db);
    return ret;
}
