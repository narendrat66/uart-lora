#include "mgos.h"
#include "mgos_gpio.h"
#include "mgos_adc.h"
#include "mgos_uart.h"

const uint8_t uart_no = 2;
struct mgos_uart_config ucfg;
char tx_buf[15] = "DELTA_IOT";

static void uart_write_cb(void *arg)
{
  //assert(uart_no == UART_NO);
 // size_t tx_av = mgos_uart_write_avail(uart_no);
 // if (tx_av == 0) 
//  {
 //   return;
//  }
if( mgos_uart_write_avail(uart_no))
{
  LOG(LL_INFO, ("Sending data to UART%d: ", uart_no));
  LOG(LL_INFO, ("TRANSMITTING BUFFER:%s ",tx_buf ));
  mgos_uart_write(uart_no,&tx_buf, strlen(tx_buf));
}
  (void)arg;
}

enum mgos_app_init_result mgos_app_init(void) {
   	
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

  mgos_set_timer(10000 /* ms */, MGOS_TIMER_REPEAT, uart_write_cb, NULL);
  return MGOS_APP_INIT_SUCCESS;
}

