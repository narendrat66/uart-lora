#include "mgos.h"
#include "mgos_gpio.h"
#include "mgos_adc.h"
#include "mgos_uart.h"
#include "mgos_arduino_lora.h"

LoRaClass* lora;
long int frequency = 868000000;

const uint8_t uart_no = 2;
struct mgos_uart_config ucfg;

static void uart_dispatcher(int uart_nmbr, void *arg) 
{
  char rx_buf[15] = {0};
  //size_t rx_av = mgos_uart_read_avail(uart_nmbr);
  
  if (mgos_uart_read_avail(uart_nmbr))
  {
     mgos_uart_read(uart_nmbr, &rx_buf, sizeof(rx_buf));
     LOG(LL_INFO, ("Serial Data Received: %s", rx_buf));
   
     mgos_LoRa_beginpacket(lora,0);
     mgos_LoRa_write(lora,(uint8_t*)rx_buf,strlen(rx_buf));
     mgos_LoRa_endpacket(lora);
     memset(rx_buf,'\0',sizeof(rx_buf));
  }

  (void)arg;
}

enum mgos_app_init_result mgos_app_init(void) {
   	
   	
lora = mgos_LoRa_create();

mgos_LoRa_setpins(lora,5,27,2);

if(!mgos_LoRa_begin(lora,frequency))
{
   LOG(LL_INFO,("LORA NOT INTIALIZED"));
}
else
{   LOG(LL_INFO,("LORA INTIALIZED"));}

  //Fill provided `cfg` structure with the default values.
  mgos_uart_config_set_defaults(uart_no, &ucfg);

  /*
   * At this point it is possible to adjust baud rate, pins and other settings.
   * 115200 8-N-1 is the default mode, but we set it anyway
   */
  ucfg.baud_rate = 115200;
  ucfg.num_data_bits = 8;
  ucfg.parity = MGOS_UART_PARITY_NONE;
  ucfg.stop_bits = MGOS_UART_STOP_BITS_1;
  ucfg.rx_buf_size = 256;
  ucfg.tx_buf_size = 256;

  if (!mgos_uart_configure(uart_no, &ucfg)) {
    LOG(LL_ERROR, ("Failed to configure UART%d", uart_no));
  }

  /*
 * Set UART dispatcher: a callback which gets called when there is data in the
 * input buffer or space available in the output buffer.
 */
  mgos_uart_set_dispatcher(uart_no, uart_dispatcher, NULL /* arg */);

  /* Controls whether UART receiver is enabled. */
  mgos_uart_set_rx_enabled(uart_no, true);

  if(!(mgos_uart_is_rx_enabled( uart_no)))
  {
	LOG(LL_ERROR, ("UART%d Receiver not Enabled", uart_no));
  }

  return MGOS_APP_INIT_SUCCESS;
}

