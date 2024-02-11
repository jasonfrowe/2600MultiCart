#define DEBUG_printf printf
#define MQTT_SERVER_HOST "pi5.home.arpa"
#define MQTT_SERVER_PORT 1883

typedef struct MQTT_CLIENT_T_ {
    ip_addr_t remote_addr;
    mqtt_client_t *mqtt_client;
    u32_t received;
    u32_t counter;
    u32_t reconnect;
} MQTT_CLIENT_T;

// Perform initialisation
static MQTT_CLIENT_T* mqtt_client_init(void) {
    MQTT_CLIENT_T *state = calloc(1, sizeof(MQTT_CLIENT_T));
    if (!state) {
        DEBUG_printf("failed to allocate state\n");
        return NULL;
    }
    state->received = 0;
    return state;
}

err_t mqtt_test_connect(MQTT_CLIENT_T *state);

void dns_found(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    MQTT_CLIENT_T *state = (MQTT_CLIENT_T*)callback_arg;
    DEBUG_printf("DNS query finished with resolved addr of %s.\n", ip4addr_ntoa(ipaddr));
    state->remote_addr = *ipaddr;
}

void run_dns_lookup(MQTT_CLIENT_T *state) {
    DEBUG_printf("Running DNS query for %s.\n", MQTT_SERVER_HOST);

    cyw43_arch_lwip_begin();
    err_t err = dns_gethostbyname(MQTT_SERVER_HOST, &(state->remote_addr), dns_found, state);
    cyw43_arch_lwip_end();

    if (err == ERR_ARG) {
        DEBUG_printf("failed to start DNS query\n");
        return;
    }

    if (err == ERR_OK) {
        DEBUG_printf("no lookup needed");
        return;
    }

    while (state->remote_addr.addr == 0) {
        cyw43_arch_poll();
        sleep_ms(1);
    }
}

u32_t data_in = 0;

u8_t buffer[65537];
u32_t data_len = 0;

static void mqtt_pub_start_cb(void *arg, const char *topic, u32_t tot_len) {
    DEBUG_printf("mqtt_pub_start_cb: topic %s\n", topic);

    if (tot_len > 65536) {
        DEBUG_printf("Message length exceeds buffer size, discarding %d.\n", tot_len);
    } else {
        DEBUG_printf("Message received of length %d.\n", tot_len);
        data_in = tot_len;
        data_len = 0;
    }
}


static void mqtt_pub_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    if (data_in > 0) {
        data_in -= len;
        memcpy(&buffer[data_len], data, len);
        // DEBUG_printf("Message part: %d %s\n",len, data);
        data_len += len;

        if (data_in == 0) {
            buffer[data_len] = 0;
            DEBUG_printf("Message received: %s\n", &buffer);
            DEBUG_printf("Message length %d %d.\n", data_len, len);

            uint8_t val = 0;
            uint8_t byte = 0;

            int i = 0;
            int j = 0;
            uint8_t rom_wifi[32768] = { 0 }; // Will be set by setup_cart

            while (i<data_len){
                val = 0;

                byte = buffer[i];
                if (byte >= '0' && byte <= '9') byte = byte - '0';
                else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
                else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
                // shift 4 to make space for new digit, and add the 4 bits of the new digit 
                val = (val << 4) | (byte & 0xF);

                byte = buffer[i+1];
                if (byte >= '0' && byte <= '9') byte = byte - '0';
                else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
                else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
                // shift 4 to make space for new digit, and add the 4 bits of the new digit 
                val = (val << 4) | (byte & 0xF);

                rom_wifi[j] = val;

                // DEBUG_printf("Byte %d.\n", val);

                i+=2;
                j+=1;

            }

            romsize = j;
            for (int ii = 0; ii < romsize; ii++){
                rom_contents[ii] = rom_wifi[ii];
            }

            if (romsize == 4096){
                bankswitch = 0; // No bankswitching
                fpBankSwitching = &BankSwitching_none;
            } else if (romsize == 8192){
                bankswitch = 1; // F8 bankswitching
                fpBankSwitching = &BankSwitching_F8;
            } else if (romsize == 16384){
                bankswitch = 2; // F6 bankswitching
                fpBankSwitching = &BankSwitching_F6;
            } else if (romsize == 32768){
                bankswitch = 3; // F4 bankswitching
                fpBankSwitching = &BankSwitching_F4;
            }

            // DEBUG_printf("New rom %d.\n", romsize);

        }
    }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status != 0) {
        DEBUG_printf("Error during connection: err %d.\n", status);
    } else {
        DEBUG_printf("MQTT connected.\n");
    }
}

void mqtt_pub_request_cb(void *arg, err_t err) {
    MQTT_CLIENT_T *state = (MQTT_CLIENT_T *)arg;
    DEBUG_printf("mqtt_pub_request_cb: err %d\n", err);
    state->received++;
}

void mqtt_sub_request_cb(void *arg, err_t err) {
    DEBUG_printf("mqtt_sub_request_cb: err %d\n", err);
}

err_t mqtt_test_publish(MQTT_CLIENT_T *state)
{
  char buffer[128];

  sprintf(buffer, "{\"message\":\"hello from picow %d / %d\"}", state->received, state->counter);

  err_t err;
  u8_t qos = 0; /* 0 1 or 2, see MQTT specification.  AWS IoT does not support QoS 2 */
  u8_t retain = 0;
  cyw43_arch_lwip_begin();
  err = mqtt_publish(state->mqtt_client, "pico_w/test", buffer, strlen(buffer), qos, retain, mqtt_pub_request_cb, state);
  cyw43_arch_lwip_end();
  if(err != ERR_OK) {
    DEBUG_printf("Publish err: %d\n", err);
  }

  return err; 
}

err_t mqtt_test_connect(MQTT_CLIENT_T *state) {
    struct mqtt_connect_client_info_t ci;
    err_t err;

    memset(&ci, 0, sizeof(ci));

    ci.client_id = "Atari2600";
    ci.client_user = NULL;
    ci.client_pass = NULL;
    ci.keep_alive = 0;
    ci.will_topic = NULL;
    ci.will_msg = NULL;
    ci.will_retain = 0;
    ci.will_qos = 0;

    #if MQTT_TLS

    struct altcp_tls_config *tls_config;
  
    #if defined(CRYPTO_CA) && defined(CRYPTO_KEY) && defined(CRYPTO_CERT)
    DEBUG_printf("Setting up TLS with 2wayauth.\n");
    tls_config = altcp_tls_create_config_client_2wayauth(
        (const u8_t *)ca, 1 + strlen((const char *)ca),
        (const u8_t *)key, 1 + strlen((const char *)key),
        (const u8_t *)"", 0,
        (const u8_t *)cert, 1 + strlen((const char *)cert)
    );
    // set this here as its a niche case at the moment.
    // see mqtt-sni.patch for changes to support this.
    ci.server_name = MQTT_SERVER_HOST;
    #elif defined(CRYPTO_CERT)
    DEBUG_printf("Setting up TLS with cert.\n");
    tls_config = altcp_tls_create_config_client((const u8_t *) cert, 1 + strlen((const char *) cert));
    #endif

    if (tls_config == NULL) {
        DEBUG_printf("Failed to initialize config\n");
        return -1;
    }

    ci.tls_config = tls_config;
    #endif

    const struct mqtt_connect_client_info_t *client_info = &ci;

    err = mqtt_client_connect(state->mqtt_client, &(state->remote_addr), MQTT_SERVER_PORT, mqtt_connection_cb, state, client_info);
    
    if (err != ERR_OK) {
        DEBUG_printf("mqtt_connect return %d\n", err);
    }

    return err;
}

void mqtt_run_test(MQTT_CLIENT_T *state) {
    state->mqtt_client = mqtt_client_new();

    state->counter = 0;  

    if (state->mqtt_client == NULL) {
        DEBUG_printf("Failed to create new mqtt client\n");
        return;
    } 
    // psa_crypto_init();
    if (mqtt_test_connect(state) == ERR_OK) {
        absolute_time_t timeout = nil_time;
        bool subscribed = false;
        mqtt_set_inpub_callback(state->mqtt_client, mqtt_pub_start_cb, mqtt_pub_data_cb, 0);

        while (true) {
            cyw43_arch_poll();
            absolute_time_t now = get_absolute_time();
            if (is_nil_time(timeout) || absolute_time_diff_us(now, timeout) <= 0) {
                if (mqtt_client_is_connected(state->mqtt_client)) {
                    cyw43_arch_lwip_begin();

                    if (!subscribed) {
                        mqtt_sub_unsub(state->mqtt_client, "pico_w/recv", 0, mqtt_sub_request_cb, 0, 1);
                        subscribed = true;
                    }

                    if (mqtt_test_publish(state) == ERR_OK) {
                        if (state->counter != 0) {
                            DEBUG_printf("published %d\n", state->counter);
                        }
                        timeout = make_timeout_time_ms(5000);
                        state->counter++;
                    } // else ringbuffer is full and we need to wait for messages to flush.
                    cyw43_arch_lwip_end();
                } else {
                    // DEBUG_printf(".");
                }
            }
        }
    }
}