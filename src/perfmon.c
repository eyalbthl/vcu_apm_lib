/*********************************************************************
 * Copyright (C) 2017-2021 Xilinx, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 ********************************************************************/

#include <perfapm.h>
#include "uio_perfmon.h"

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_t perf_count_thread;
static volatile unsigned long vcu_apm_counter[4][2];

static void *thread_perf_counter(void* temp) {

	uPerfMon_Enable();
	while (1) {

		/* Create pthread cancellation point */
		pthread_testcancel();
		if (uPerfMon_Check_SIC_Overflow_Mask()) {
			vcu_apm_counter[E_APM0][E_READ_BYTE_CNT] = uPerfMon_getCounterValue(E_APM0, E_READ_BYTE_CNT);
			vcu_apm_counter[E_APM0][E_WRITE_BYTE_CNT] = uPerfMon_getCounterValue( E_APM0, E_WRITE_BYTE_CNT);
			vcu_apm_counter[E_APM1][E_READ_BYTE_CNT] = uPerfMon_getCounterValue(E_APM1, E_READ_BYTE_CNT);
			vcu_apm_counter[E_APM1][E_WRITE_BYTE_CNT] = uPerfMon_getCounterValue(E_APM1, E_WRITE_BYTE_CNT);
			vcu_apm_counter[E_APM2][E_READ_BYTE_CNT] = uPerfMon_getCounterValue(E_APM2, E_READ_BYTE_CNT);
			vcu_apm_counter[E_APM2][E_WRITE_BYTE_CNT] = uPerfMon_getCounterValue(E_APM2, E_WRITE_BYTE_CNT);

			vcu_apm_counter[E_APM3][E_READ_BYTE_CNT] = uPerfMon_getCounterValue(E_APM3, E_READ_BYTE_CNT);
			vcu_apm_counter[E_APM3][E_WRITE_BYTE_CNT] = uPerfMon_getCounterValue(E_APM3, E_WRITE_BYTE_CNT);
		}
		usleep(POLL_INTERVAL);
	}

	uPerfMon_Disable();
	pthread_exit(NULL);

	return NULL;
}

int perf_monitor_init() {
	int ret = 0;

	ret = uPerfMon_Init();
	if (ret) {
		printf("perf_monitor [error] Initialization failed \n");
		return EXIT_FAIL;
	}

	// spawn a thread to throw out performance numbers
	ret = pthread_create(&perf_count_thread, NULL, &thread_perf_counter, NULL);
	if (ret) {
		printf("perf_monitor [error] pthread_create failed %d\n", ret);
		return EXIT_FAIL;
	}

	return EXIT_SUCCESS;
}

int perf_monitor_deinit(void) {
	int ret = 0;

	ret = pthread_cancel(perf_count_thread);
	ret = pthread_join(perf_count_thread, NULL);

#ifdef DEBUG_MODE
	printf("perf_monitor [info] pthread_join %d \n",ret);
#endif

	return ret;
}

unsigned long perf_monitor_get_rd_wr_cnt(enum Perf_APM port, enum Perf_Operation op) {
	unsigned long rd_wr_count;

	switch (port) {
	case E_APM0:
	case E_APM1:
	case E_APM2:
	case E_APM3:
		switch (op) {
		case E_READ_BYTE_CNT:
		case E_WRITE_BYTE_CNT:
			rd_wr_count = vcu_apm_counter[port][op];
			break;

		default:
			return EXIT_FAIL;
		}
		break;

	default:
		return EXIT_FAIL;
	}

	/* Convert beats to bytes transferred */
	return rd_wr_count = rd_wr_count * 16;
}
