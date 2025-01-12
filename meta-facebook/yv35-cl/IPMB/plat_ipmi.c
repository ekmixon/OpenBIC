/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include "cmsis_os2.h"
#include "board_device.h"
#include "objects.h"
#include "ipmi.h"
#include "plat_ipmi.h"
#include <string.h>
#include "hal_i2c.h"
#include "plat_i2c.h"
#include "sensor.h"
#include "fru.h"
#include "plat_fru.h"
#include "ipmi_def.h"
#include "bic-util.h"
#include "fw_update.h"
#include "hal_gpio.h"

bool pal_is_not_return_cmd(uint8_t netfn, uint8_t cmd) {
  if ( (netfn == NETFN_OEM_1S_REQ) && (cmd == CMD_OEM_MSG_OUT) ) {
    return 1;
  } else if ( (netfn == NETFN_OEM_1S_REQ) && (cmd == CMD_OEM_MSG_IN) ) {
    return 1;
  }

  // Reserve for future commands

  return 0;
}

void pal_SENSOR_GET_SENSOR_READING(ipmi_msg *msg) {
  uint8_t status, snr_num, reading;
  
  if (msg->data_len != 1) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }
 
  if (!enable_sensor_poll) {
    printf("Reading sensor cache while sensor polling disable\n");
    msg->completion_code = CC_CAN_NOT_RESPOND;
    return;
  }
 
  snr_num = msg->data[0];
  status = get_sensor_reading(snr_num, &reading, get_from_cache); // Fix to get_from_cache. As need real time reading, use OEM command to get_from_sensor.

  switch (status) {
    case SNR_READ_SUCCESS:
      msg->data[0] = reading;
      // SDR sensor initialization bit6 enable scan, bit5 enable event
      // retunr data 1 bit 7 set to 0 to disable all event msg. bit 6 set to 0 disable sensor scan
      msg->data[1] = ( (full_sensor_table[SnrNum_SDR_map[snr_num]].sensor_init & 0x40) | ( (full_sensor_table[SnrNum_SDR_map[snr_num]].sensor_init & 0x20) << 2) );
      msg->data[2] = 0xc0; // fix to threshold deassert status, BMC will compare with UCR/UNR itself
      msg->data_len = 3;
      msg->completion_code = CC_SUCCESS;
      break;
    case SNR_FAIL_TO_ACCESS:
      msg->completion_code = CC_NODE_BUSY; // transection error
      break;
    case SNR_NOT_ACCESSIBLE:
      msg->completion_code = CC_NOT_SUPP_IN_CURR_STATE; // DC off
      break;
    case SNR_NOT_FOUND:
      msg->completion_code = CC_INVALID_DATA_FIELD; // request sensor number not found
      break;
    case SNR_UNSPECIFIED_ERROR:
    default :
      msg->completion_code = CC_UNSPECIFIED_ERROR; // unknown error
      break;
  }
  return;
}

void pal_APP_GET_DEVICE_ID(ipmi_msg *msg) {
  if (msg->data_len != 0) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  msg->data[0] = DEVICE_ID;
  msg->data[1] = DEVICE_REVISION;
  msg->data[2] = FIRMWARE_REVISION_1;
  msg->data[3] = FIRMWARE_REVISION_2;
  msg->data[4] = IPMI_VERSION;
  msg->data[5] = ADDITIONAL_DEVICE_SUPPORT;
  msg->data[6] = (WW_IANA_ID & 0xFF);
  msg->data[7] = (WW_IANA_ID >> 8) & 0xFF;
  msg->data[8] = (WW_IANA_ID >> 16) & 0xFF;
  msg->data[9] = (PRODUCT_ID & 0xFF);
  msg->data[10] = (PRODUCT_ID >> 8) & 0xFF;
  msg->data[11] = (AUXILIARY_FW_REVISION >> 24) & 0xFF;
  msg->data[12] = (AUXILIARY_FW_REVISION >> 16) & 0xFF;
  msg->data[13] = (AUXILIARY_FW_REVISION >> 8) & 0xFF;
  msg->data[14] = (AUXILIARY_FW_REVISION & 0xFF);
  msg->data_len = 15;
  msg->completion_code = CC_SUCCESS;

  return;
}

void pal_APP_WARM_RESET(ipmi_msg *msg) {
  if (msg->data_len != 0) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  bic_warm_reset();

  msg->data_len = 0;
  msg->completion_code = CC_SUCCESS;

  return;
}


void pal_APP_GET_SELFTEST_RESULTS(ipmi_msg *msg) {
  if (msg->data_len != 0) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  uint8_t test_result = 0;
  // CannotAccessSel
  test_result = (test_result | GET_TEST_RESULT) << 1;
  // CannotAccessSdrr
  test_result = (test_result | is_SDR_not_init) << 1;
  // CannotAccessFru
  // Get common header
  uint8_t checksum = 0;
  EEPROM_ENTRY fru_entry;

  fru_entry.config.dev_id = 0;
  fru_entry.offset = 0;           
  fru_entry.data_len = 8;
  FRU_read(&fru_entry);
  for(uint8_t i = 0; i < fru_entry.data_len; i++) {
    checksum += fru_entry.data[i];
  }

  if (checksum == 0) {
    test_result = (test_result | 0) << 1;
  } else {
    test_result = (test_result | 1) << 1;
  }
  // IpmbLinesDead
  test_result = (test_result | GET_TEST_RESULT) << 1;
  // SdrrEmpty
  test_result = (test_result | is_SDR_not_init) << 1;
  // InternalCorrupt
  if (checksum == 0) {
    test_result = (test_result | 0) << 1;
  } else {
    test_result = (test_result | 1) << 1;
  }

  // UpdateFWCorrupt
  test_result = (test_result | GET_TEST_RESULT) << 1;
  // OpFWCorrupt
  test_result = test_result | GET_TEST_RESULT;

  msg->data[0] = test_result == 0x00 ? 0x55 : 0x57;
  msg->data[1] = test_result == 0x00 ? 0x00 : test_result;
  msg->data_len = 2;
  msg->completion_code = CC_SUCCESS;

  return;
}

void pal_APP_MASTER_WRITE_READ(ipmi_msg *msg) {
  uint8_t retry = 3;
  uint8_t bus_7bit;
  I2C_MSG i2c_msg;

  if (msg->data_len < 4) { // at least include bus, addr, rx_len, offset
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  bus_7bit = ( (msg->data[0]-1) >> 1); // should ignore bit0, all bus public
  i2c_msg.bus = i2c_bus_to_index[bus_7bit];
  i2c_msg.slave_addr = (msg->data[1] >> 1); // 8 bit address to 7 bit 
  i2c_msg.rx_len = msg->data[2];
  i2c_msg.tx_len = msg->data_len - 3;

  if (i2c_msg.tx_len == 0) {
    msg->completion_code = CC_INVALID_DATA_FIELD;
    return;
  }

  memcpy(&i2c_msg.data[0], &msg->data[3], i2c_msg.tx_len);
  msg->data_len = i2c_msg.rx_len;

  if (i2c_msg.rx_len == 0) {
    if ( i2c_master_write(&i2c_msg, retry) ) {
      msg->completion_code = CC_SUCCESS;
    } else {
      msg->completion_code = CC_I2C_BUS_ERROR;
    }
  } else {
    if ( i2c_master_read(&i2c_msg, retry) ) {
      memcpy(&msg->data[0], i2c_msg.data, i2c_msg.rx_len);
      msg->completion_code = CC_SUCCESS;
    } else {
      msg->completion_code = CC_I2C_BUS_ERROR;
    }
  }

  return;
}

void pal_STORAGE_GET_FRUID_INFO(ipmi_msg *msg) {
  uint8_t fruid;
  uint16_t fru_size;

  if (msg->data_len != 1) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  fruid = msg->data[0];

  if (fruid >= MAX_FRU_ID) { // check if FRU is defined
    msg->completion_code = CC_INVALID_DATA_FIELD;
    return;
  }

  fru_size = find_FRU_size(fruid);
  if(fru_size == 0xFFFF) { // indicate ID not found
    msg->completion_code = CC_UNSPECIFIED_ERROR;
    return;
  }

  msg->data[0] = fru_size & 0xFF;          // lsb
  msg->data[1] = (fru_size >> 8) & 0xFF;   // msb
  msg->data[2] = get_FRU_access(fruid);    // access type
  msg->data_len = 3;

  return;
}

void pal_STORAGE_READ_FRUID_DATA(ipmi_msg *msg) {
  uint8_t status;  
  EEPROM_ENTRY fru_entry;

  if (msg->data_len != 4) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  fru_entry.config.dev_id = msg->data[0];
  fru_entry.offset = (msg->data[2] << 8) | msg->data[1];
  fru_entry.data_len = msg->data[3];

  status = FRU_read(&fru_entry);

  msg->data_len = fru_entry.data_len + 1;
  msg->data[0] = fru_entry.data_len;

  for(uint8_t i = 0; i < fru_entry.data_len; i++) {
    msg->data[i + 1] = fru_entry.data[i];
  }

  switch (status) {
    case FRU_READ_SUCCESS:
      msg->completion_code = CC_SUCCESS;
      break;
    case FRU_INVALID_ID:
      msg->completion_code = CC_INVALID_PARAM;
      break;
    case FRU_OUT_OF_RANGE:
      msg->completion_code = CC_PARAM_OUT_OF_RANGE;
      break;
    case FRU_FAIL_TO_ACCESS:
      msg->completion_code = CC_FRU_DEV_BUSY;
      break;
    default:
      msg->completion_code = CC_UNSPECIFIED_ERROR;
      break;
  }
  return;
}

void pal_STORAGE_WRITE_FRUID_DATA(ipmi_msg *msg) {
  uint8_t status;
  EEPROM_ENTRY fru_entry;

  if (msg->data_len < 4) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  fru_entry.config.dev_id = msg->data[0];
  fru_entry.offset = (msg->data[2] << 8) | msg->data[1];
  fru_entry.data_len = msg->data_len - 3; // skip id and offset
  memcpy(&fru_entry.data[0], &msg->data[3], fru_entry.data_len);

  msg->data[0] = msg->data_len - 3;
  msg->data_len = 1;
  status = FRU_write(&fru_entry);

  switch (status) {
    case FRU_WRITE_SUCCESS:
      msg->completion_code = CC_SUCCESS;
      break;
    case FRU_INVALID_ID:
      msg->completion_code = CC_INVALID_PARAM;
      break;
    case FRU_OUT_OF_RANGE:
      msg->completion_code = CC_PARAM_OUT_OF_RANGE;
      break;
    case FRU_FAIL_TO_ACCESS:
      msg->completion_code = CC_FRU_DEV_BUSY;
      break;
    default:
      msg->completion_code = CC_UNSPECIFIED_ERROR;
      break;
  }
  return;
}

void pal_STORAGE_RSV_SDR(ipmi_msg *msg) {
  uint16_t RSV_ID;

  if (msg->data_len != 0) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  RSV_ID = SDR_get_RSV_ID();
  msg->data[0] = RSV_ID & 0xFF;
  msg->data[1] = (RSV_ID >> 8) & 0xFF;
  msg->data_len = 2;
  msg->completion_code = CC_SUCCESS;  

  return;
}

void pal_STORAGE_GET_SDR(ipmi_msg *msg) {
  uint16_t next_record_ID;
  uint16_t rsv_ID,record_ID;
  uint8_t offset,req_len;
  uint8_t *table_ptr;

  rsv_ID = (msg->data[1] << 8) | msg->data[0];
  record_ID = (msg->data[3] << 8) | msg->data[2];
  offset = msg->data[4];
  req_len = msg->data[5];
   
 
  if (msg->data_len != 6) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  if ( !SDR_RSV_ID_check(rsv_ID) ) {
    msg->completion_code = CC_INVALID_RESERVATION;
    return;
  }

  if ( !SDR_check_record_ID(record_ID) ) {
    msg->completion_code = CC_INVALID_DATA_FIELD;
    return;
  }

  if ( (req_len+2) > IPMI_DATA_MAX_LENGTH ) { // request length + next record ID should not over IPMB data limit
    msg->completion_code = CC_LENGTH_EXCEEDED;
    return;
  }

  if ( (offset+req_len) > full_sensor_table[record_ID].record_len ) {
    msg->completion_code = CC_PARAM_OUT_OF_RANGE;
    return;
  }
  
  next_record_ID = SDR_get_record_ID(record_ID);
  msg->data[0] = next_record_ID & 0xFF;
  msg->data[1] = (next_record_ID >> 8) & 0xFF;

  table_ptr = (uint8_t*)&full_sensor_table[record_ID];
  memcpy(&msg->data[2], (table_ptr+offset), req_len);

  msg->data_len = req_len + 2; // return next record ID + sdr data
  msg->completion_code = CC_SUCCESS;

  return;  
}

void pal_OEM_MSG_OUT(ipmi_msg *msg) {
  uint8_t  target_IF;
  ipmb_error status;
  ipmi_msg *bridge_msg = (ipmi_msg*)pvPortMalloc(sizeof(ipmi_msg));

  memset(bridge_msg, 0, sizeof(ipmi_msg));
  
  if (msg->completion_code != CC_INVALID_IANA) {
    msg->completion_code = CC_SUCCESS;
  }

  if (msg->data_len <= 2) { // Should input target, netfn, cmd
    msg->completion_code = CC_INVALID_LENGTH;
  }

  target_IF = msg->data[0];

  if ( (pal_IPMB_config_table[IPMB_inf_index_map[target_IF]].Inf == Reserve_IFs) || (pal_IPMB_config_table[IPMB_inf_index_map[target_IF]].EnStatus == Disable) ) { // Bridge to invalid or disabled interface
    printf("OEM_MSG_OUT: Invalid bridge interface: %x\n",target_IF);
    msg->completion_code = CC_NOT_SUPP_IN_CURR_STATE;
  }
  
  if (msg->completion_code == CC_SUCCESS) { // only send to target while msg is valid
    if (DEBUG_IPMI) {
      printf("bridge len %d, netfn %x, cmd %x\n", msg->data_len, msg->data[1] >> 2, msg->data[2]);
    }

    bridge_msg->data_len = msg->data_len -3;
    bridge_msg->seq_source = msg->seq_source;
    bridge_msg->InF_target = msg->data[0];
    bridge_msg->InF_source = msg->InF_source;
    bridge_msg->netfn = msg->data[1] >> 2;
    bridge_msg->cmd = msg->data[2];

    if (bridge_msg->data_len != 0) {
      memcpy( &bridge_msg->data[0], &msg->data[3], bridge_msg->data_len * sizeof(msg->data[0]) );
    }

    status = ipmb_send_request(bridge_msg, IPMB_inf_index_map[target_IF]);

    if (status != ipmb_error_success) {
      printf("OEM_MSG_OUT send IPMB req fail status: %x",status);
      msg->completion_code = CC_BRIDGE_MSG_ERR;
    }

    vPortFree(bridge_msg);
  }
  
  if (msg->completion_code != CC_SUCCESS) { // Return to source while data is invalid or sending req to Tx task fail

    msg->data_len=0;
    status = ipmb_send_response(msg, IPMB_inf_index_map[msg->InF_source]);

    if (status != ipmb_error_success) {
      printf("OEM_MSG_OUT send IPMB resp fail status: %x",status);
    }
  }

  return;
}

void pal_OEM_GET_GPIO(ipmi_msg *msg) {
  if (msg->data_len != 0) { // only input enable status
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }
  
  uint8_t eight_bit_value = 0;
  uint8_t gpio_value, gpio_cnt, data_len;
  gpio_cnt = gpio_ind_to_num_table_cnt + (8 - (gpio_ind_to_num_table_cnt % 8)); // Bump up the gpio_ind_to_num_table_cnt to multiple of 8.
  data_len = gpio_cnt / 8;
  msg->data_len = data_len;
  for(uint8_t i = 0; i < gpio_cnt; i++) {
    gpio_value = (i >= gpio_ind_to_num_table_cnt) ? 0 : gpio[0].get(&gpio[0], gpio_ind_to_num_table[i]);
    eight_bit_value = (eight_bit_value << 1) | gpio_value;
    msg->data[i / 8] = eight_bit_value;
  }
  msg->completion_code = CC_SUCCESS;
  return;
}
    
void pal_OEM_SET_GPIO(ipmi_msg *msg) {
  uint8_t write_byte, value_byte, write_bit, value_bit, gpio_num, data_len, index = 0;
  data_len = (gpio_ind_to_num_table_cnt % 8 == 0) ? gpio_ind_to_num_table_cnt / 8 : (gpio_ind_to_num_table_cnt / 8) + 1;
  for(int i = 0; i < data_len; i++) {
    write_byte = msg->data[i];
    value_byte = msg->data[i + data_len];
    for(int j = 0; j < 8; j++) {
      write_bit = (write_byte >> (7 - j)) & 0x1;
      value_bit = (value_byte >> (7 - j)) & 0x1;
      index++;
      if (write_bit == 1 && index <= gpio_ind_to_num_table_cnt) {
        gpio_num = gpio_ind_to_num_table[index];
        gpio[0].set_direction(&gpio[0], gpio_num, 1);
        gpio[0].set(&gpio[0], gpio_num, value_bit);
      }
    }
  }
  msg->data_len = 0;
  msg->completion_code = CC_SUCCESS;
  return;
}

void pal_OEM_SENSOR_POLL_EN(ipmi_msg *msg) {
  if (msg->data_len != 1) { // only input enable status
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  enable_sensor_poll = msg->data[0]; 
  msg->completion_code = CC_SUCCESS;
  return;
}

void pal_OEM_FW_UPDATE(ipmi_msg *msg) {
  /*********************************
 * buf 0:   target, 0x80 indicate last byte
 * buf 1~4: offset, 1 lsb
 * buf 5~6: length, 5 lsb
 * buf 7~N: data
 ***********************************/
  if (msg->data_len < 8) { 
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  uint8_t target = msg->data[0], status;
  uint32_t offset = ((msg->data[4] << 24) | (msg->data[3] << 16) | (msg->data[2] << 8) | msg->data[1]);
  uint16_t length = ((msg->data[6] << 8) | msg->data[5]);

  if( (length == 0) || (length != msg->data_len-7) ) {
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  if(target == BIOS_UPDATE) {
    if(offset > 0x4000000) { // BIOS size maximum 64M bytes
      msg->completion_code = CC_PARAM_OUT_OF_RANGE;
      return;
    }
    status = fw_update(offset, length, &msg->data[7], (target & UPDATE_EN), spi1_bus, spi0_cs);

  } else if( (target == BIC_UPDATE) || (target == (BIC_UPDATE | UPDATE_EN)) ) {
    if(offset > 0x50000) { // Expect BIC firmware size not bigger than 320k
      msg->completion_code = CC_PARAM_OUT_OF_RANGE;
      return;
    }
    status = fw_update(offset, length, &msg->data[7], (target & UPDATE_EN), fmc_bus, fmc_cs);
  }

  msg->data_len = 0;

  switch (status) {
    case fwupdate_success:
      msg->completion_code = CC_SUCCESS;
      break;
    case fwupdate_out_of_heap:
      msg->completion_code = CC_LENGTH_EXCEEDED;
      break;
    case fwupdate_over_length:
      msg->completion_code = CC_OUT_OF_SPACE;
      break;
    case fwupdate_repeated_updated:
      msg->completion_code = CC_INVALID_DATA_FIELD;
      break;
    case fwupdate_update_fail:
      msg->completion_code = CC_TIMEOUT;
      break;
    default:
      msg->completion_code = CC_UNSPECIFIED_ERROR;
      break;
  }
  if (status != fwupdate_success) {
    printf("spi fw cc: %x\n", msg->completion_code);
  }

  return;

}

void pal_OEM_GET_SET_GPIO(ipmi_msg *msg) {
  uint8_t value;
  uint8_t completion_code = CC_INVALID_LENGTH;
  uint8_t gpio_num = gpio_ind_to_num_table[msg->data[1]];

  do {
    if(msg->data[0] == 0) {    // Get GPIO output status
      if(msg->data_len != 2) {
        break;
      }
      msg->data[0] = gpio_num;
      msg->data[1] = gpio[0].get(&gpio[0], gpio_num);
      completion_code = CC_SUCCESS;

    } else if(msg->data[0] == 1) {    // Set GPIO output status
      if(msg->data_len != 3) {
        break;
      }
      msg->data[0] = gpio_num;
      gpio_set(gpio_num, msg->data[2]);
      msg->data[1] = gpio[0].get(&gpio[0], gpio_num);
      completion_code = CC_SUCCESS;

    } else if (msg->data[0] == 2) {    // Get GPIO direction status
      if(msg->data_len != 2) {
        break;
      }
      msg->data[0] = gpio_num;
      msg->data[1] = gpio[0].get_direction(&gpio[0], gpio_num);
      completion_code = CC_SUCCESS;

    } else if (msg->data[0] == 3) {    // Set GPIO direction status
      if(msg->data_len != 3) {
        break;
      }
      msg->data[0] = gpio_num;
      gpio[0].set_direction(&gpio[0], gpio_num, msg->data[2]);
      msg->data[1] = gpio[0].get_direction(&gpio[0], gpio_num);
      completion_code = CC_SUCCESS;
    }
  } while(0);

  msg->data_len = 2;  // Return GPIO number, status
  msg->completion_code = completion_code;
  return;
}

void pal_OEM_I2C_DEV_SCAN(ipmi_msg *msg) {
  if (msg->data[0] == 0x9C && msg->data[1] == 0x9C && msg->data[2] == 0x00) {
    while(1); // hold firmware for debug only
  }

  if (msg->data_len != 1) { // only input enable status
    msg->completion_code = CC_INVALID_LENGTH;
    return;
  }

  uint8_t bus, addr, slave_count = 0;
  bus = i2c_bus_to_index[msg->data[0]];
  msg->data_len = 0;

  for(addr = 0x3; addr < 0x7F; addr++) {
    if ( !i2c_write(&i2c[bus], addr, NULL, 0, I2C_M_STOP) ) {
      msg->data[slave_count] = addr << 1; // transfer to 8bit addr
      msg->data_len++;
      slave_count++;
    }
  }

  msg->completion_code = CC_SUCCESS;
  return;
}

