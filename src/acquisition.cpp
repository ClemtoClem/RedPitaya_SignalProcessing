#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <vector>
#include "acquisition.hpp"
#include "globals.hpp"

uint32_t getTimeDelay(int decimation) {	
	/* Find optimal decimation setting */
	if (decimation < 8) {
		return 131;
	} else if(decimation < 64) {
		return 1048;
	} else if(decimation < 1024) {
		return 8388;
	} else if(decimation < 8192) {
		return 134200;
	} else if(decimation < 65536) {
		return 1000000;
	} else if(decimation == 65536) {
		return 8589000;
	} else {
		return 1000000;
	}
}

static buffers_t *b = NULL;

void initAcquisition(float triggerLevel, int32_t triggerDelay) {
	rp_AcqReset();
	rp_AcqSetTriggerLevel(RP_T_CH_1, triggerLevel);
	rp_AcqSetTriggerDelay(triggerDelay);
	
	b = (buffers_t *) rp_createBuffer(2,BUFFER_SIZE,false,true,false);
}

void releaseAcquisition() {
	if (b != NULL) rp_deleteBuffer(b);
}

void acquisitionChannel1(Signal &signal) {
	signal.resize(BUFFER_SIZE);

	rp_AcqSetDecimationFactor(DECIMATION);
	int timeDelay = getTimeDelay(DECIMATION);
	rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;
	bool fillState = false;

	/* Echantillonnage du signal */
	fillState = false;

	rp_AcqStart();
	usleep(timeDelay);
	rp_AcqSetTriggerSrc(RP_TRIG_SRC_CHA_PE);

	while(1) {
		rp_AcqGetTriggerState(&state);
		if(state == RP_TRIG_STATE_TRIGGERED) {
			break;
		}
	}
	while(!fillState){
		rp_AcqGetBufferFillState(&fillState);
	}

	rp_AcqStop();

	uint32_t pos = 0;
	rp_AcqGetWritePointerAtTrig(&pos);

	rp_AcqGetData(pos, b);
	
	/* Enrgistrement du buffer */
	for (size_t i = 0; i < BUFFER_SIZE; i++) signal[i] = b->ch_d[0][i];

	/* Releasing resources */
	//rp_deleteBuffer(b);
}

void acquisitionChannel2(Signal &signal) {
	signal.resize(BUFFER_SIZE);
	
	rp_AcqSetDecimationFactor(DECIMATION);
	int timeDelay = getTimeDelay(DECIMATION);
	rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;
	bool fillState = false;

	/* Echantillonnage du signal */
	fillState = false;

	rp_AcqStart();
	usleep(timeDelay);
	rp_AcqSetTriggerSrc(RP_TRIG_SRC_CHA_PE);

	while(1) {
		rp_AcqGetTriggerState(&state);
		if(state == RP_TRIG_STATE_TRIGGERED) {
			break;
		}
	}
	while(!fillState){
		rp_AcqGetBufferFillState(&fillState);
	}

	rp_AcqStop();

	uint32_t pos = 0;
	//rp_AcqGetWritePointerAtTrig(&pos);
	rp_AcqGetWritePointer(&pos);

	rp_AcqGetData(pos, b);
	
	/* Enrgistrement du buffer */
	for (size_t i = 0; i < BUFFER_SIZE; i++) signal[i] = b->ch_d[1][i];

	/* Releasing resources */
	//rp_deleteBuffer(b);
}

void acquisitionChannels1_2(Signal &signal1, Signal &signal2, rp_channel_trigger_t trigger) {
	signal1.resize(BUFFER_SIZE);
	signal2.resize(BUFFER_SIZE);
	
	rp_AcqSetDecimationFactor(DECIMATION);
	int timeDelay = getTimeDelay(DECIMATION);
	rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;
	bool fillState = false;

	/* Echantillonnage du signal */
	fillState = false;

	rp_AcqStart();
	usleep(timeDelay);
	rp_AcqSetTriggerSrc(RP_TRIG_SRC_CHA_PE);

	while(1) {
		rp_AcqGetTriggerState(&state);
		if(state == RP_TRIG_STATE_TRIGGERED) {
			break;
		}
	}
	while(!fillState){
		rp_AcqGetBufferFillState(&fillState);
	}

	rp_AcqStop();

	uint32_t pos = 0;
	rp_AcqGetWritePointerAtTrig(&pos);

	rp_AcqGetData(pos, b);
	
	/* Enrgistrement du buffer */
	for (size_t i = 0; i < BUFFER_SIZE; i++) {
		signal1[i] = b->ch_d[0][i];
		signal2[i] = b->ch_d[1][i];
	}
}