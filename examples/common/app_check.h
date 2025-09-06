
#pragma once

#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"

#define APP_RETURN_ON_ERROR(err, tag, msg)                                                   \
    do                                                                                       \
    {                                                                                        \
        if (err != ESP_OK)                                                                   \
        {                                                                                    \
            ESP_LOGE(tag, "[%s,%d] %s (%s)", __func__, __LINE__, msg, esp_err_to_name(err)); \
            return err;                                                                      \
        }                                                                                    \
    } while (0)
