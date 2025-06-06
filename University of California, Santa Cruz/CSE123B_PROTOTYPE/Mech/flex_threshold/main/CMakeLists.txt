idf_component_register(
  SRCS "main.c"
  INCLUDE_DIRS "."
  REQUIRES cjson wifi_provisioning esp_http_client esp_wifi esp_event esp_http_server esp_netif nvs_flash driver
)
