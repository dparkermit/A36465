#include "P1395_CAN_SLAVE.h"
#include "A36465.h"

void ETMCanSlaveExecuteCMDBoardSpecific(ETMCanMessage* message_ptr) {
  unsigned int index_word;

  index_word = message_ptr->word3;
  switch (index_word) 
    {
      /*
	Place all board specific commands here
      */
    case ETM_CAN_REGISTER_AFC_SET_1_HOME_POSITION_AND_OFFSET:
      afc_motor.home_position = message_ptr->word0;
      // unused offset
      ETMAnalogSetOutput(&global_data_A36465.aft_control_voltage, message_ptr->word2);
      _CONTROL_NOT_CONFIGURED = 0;
      break;
      
    case ETM_CAN_REGISTER_AFC_CMD_SELECT_AFC_MODE:
      _STATUS_AFC_MODE_MANUAL_MODE = 0;
      break;
      
    case ETM_CAN_REGISTER_AFC_CMD_SELECT_MANUAL_MODE:
      _STATUS_AFC_MODE_MANUAL_MODE = 1;
      break;
      
    case ETM_CAN_REGISTER_AFC_CMD_SET_MANUAL_TARGET_POSITION:
      global_data_A36465.manual_target_position = message_ptr->word0;
      break;

    case ETM_CAN_REGISTER_AFC_CMD_RELATIVE_MOVE_MANUAL_TARGET:
      if (message_ptr->word1) {
	// decrease the target position;
	if (global_data_A36465.manual_target_position > message_ptr->word0) {
	  global_data_A36465.manual_target_position -= message_ptr->word0;
	} else {
	  global_data_A36465.manual_target_position = 0;
	}
      } else {
	// increase the target position;
	if ((0xFFFF - message_ptr->word0) > global_data_A36465.manual_target_position) {
	  global_data_A36465.manual_target_position += message_ptr->word0;
	} else {
	  global_data_A36465.manual_target_position = 0xFFFF;
	}
      }
      break;
      
    default:
      local_can_errors.invalid_index++;
      break;
    }
}


void ETMCanSlaveLogCustomPacketC(void) {
  /* 
     Use this to log Board specific data packet
     This will get executed once per update cycle (1.6 seconds) and will be spaced out in time from the other log data
  */

  ETMCanSlaveLogData(
		     ETM_CAN_DATA_LOG_REGISTER_AFC_FAST_POSITION,
		     etm_can_next_pulse_count,
		     afc_motor.current_position,
		     afc_motor.target_position,
		     0
		     );
}


void ETMCanSlaveLogCustomPacketD(void) {
  /* 
     Use this to log Board specific data packet
     This will get executed once per update cycle (1.6 seconds) and will be spaced out in time from the other log data
  */
  ETMCanSlaveLogData(
		     ETM_CAN_DATA_LOG_REGISTER_AFC_FAST_READINGS,
		     etm_can_next_pulse_count,
		     1,//global_data_A36465.aft_A_sample.reading_scaled_and_calibrated,
		     35,//global_data_A36465.aft_B_sample.reading_scaled_and_calibrated,
		     17//global_data_A36465.aft_filtered_error_for_client
		     );
}


void ETMCanSlaveLogCustomPacketE(void) {
  /* 
     Use this to log Board specific data packet
     This will get executed once per update cycle (1.6 seconds) and will be spaced out in time from the other log data
  */
  ETMCanSlaveLogData(
		     ETM_CAN_DATA_LOG_REGISTER_AFC_SLOW_SETTINGS,
		     afc_motor.home_position,
		     0, // Dparker return the programed offset here
		     afc_motor.current_position, 
		     global_data_A36465.aft_control_voltage.set_point
		     );
}

void ETMCanSlaveLogCustomPacketF(void) {
  /* 
     Use this to log Board specific data packet
     This will get executed once per update cycle (1.6 seconds) and will be spaced out in time from the other log data
  */

}


  



