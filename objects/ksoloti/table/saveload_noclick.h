#ifndef SAVELOAD_NOCLICK_H
#define SAVELOAD_NOCLICK_H

static bool_t saveload_noclick_busy = false;

void wait_busy(void) {
	volatile uint32_t count = 60000; 
	while (saveload_noclick_busy && count) {
		count--;
		chThdSleepMilliseconds(1);
	}
}

template <typename T> __attribute__((noinline)) static void save_sd(const char* fname, T array, uint32_t LENGTH) {

	wait_busy();
	saveload_noclick_busy = true;

	FIL FileObject;
	FRESULT err;
	UINT bytes_written;

	// codec_clearbuffer(); /* no longer necessary! */
	
	err = f_open(&FileObject, fname, FA_WRITE | FA_CREATE_ALWAYS);
	if (err != FR_OK) { 				/* getting error? */
		report_fatfs_error(err, fname);
		saveload_noclick_busy = false;
		return;
	}
	
	int rem_sz = sizeof(array) * LENGTH; /* length of table in bytes */
	int offset = 0;
	
	while (rem_sz > 0) {
		if (rem_sz > sizeof(fbuff)) { 	/* fbuff is a firmware-internal, general-purpose SD card buffer */
			memcpy((char*) fbuff, (char*) (&array[0]) + offset, sizeof(fbuff));
			err = f_write(&FileObject, fbuff, sizeof(fbuff), &bytes_written);
			rem_sz -= sizeof(fbuff);
			offset += sizeof(fbuff);
		} else {
			memcpy((char*) fbuff, (char*) (&array[0]) + offset, rem_sz);
			err = f_write(&FileObject, fbuff, rem_sz, &bytes_written);
			rem_sz = 0;
		}
	}
	if (err != FR_OK) { 				/* getting error? */
		report_fatfs_error(err, fname);
		saveload_noclick_busy = false;
		return;
	}
	
	err = f_close(&FileObject);
	if (err != FR_OK) { 				/* getting error? */
		report_fatfs_error(err, fname);
		saveload_noclick_busy = false;
		return;
	}
	saveload_noclick_busy = false;
}


template <typename T> __attribute__((noinline)) static void load_sd(const char* fname, T array, uint32_t LENGTH) {

	wait_busy();
	saveload_noclick_busy = true;
	
	FIL FileObject;
	FRESULT err;
	UINT bytes_read;

	// codec_clearbuffer(); /* no longer necessary! */
	
	err = f_open(&FileObject, fname, FA_READ | FA_OPEN_EXISTING); /* open file */
	if (err != FR_OK) { 				/* getting error? */
		report_fatfs_error(err, fname);
		saveload_noclick_busy = false;
		return;
	}
	
	int rem_sz = sizeof(array) * LENGTH;	/* length of table in bytes */
	int offset = 0;
	
	while (rem_sz > 0) {
		if (rem_sz > sizeof(fbuff)) { 	/* fbuff is a firmware-internal, general-purpose SD card buffer */
			err = f_read(&FileObject, fbuff, sizeof(fbuff), &bytes_read);
			if (bytes_read == 0) {
				break;
			}
			memcpy((char*) (&array[0]) + offset, (char*) fbuff, bytes_read);
			rem_sz -= bytes_read;
			offset += bytes_read;
		} else {
			err = f_read(&FileObject, fbuff, rem_sz, &bytes_read);
			memcpy((char*) (&array[0]) + offset, (char*) fbuff, bytes_read);
			rem_sz = 0;
		}
	}
	if (err != FR_OK) { 				/* getting error? */
		report_fatfs_error(err, fname);
		saveload_noclick_busy = false;
		return;
	}
	
	err = f_close(&FileObject); 
	if (err != FR_OK) { 				/* getting error? */
		report_fatfs_error(err, fname);
		saveload_noclick_busy = false;
		return;
	}
	saveload_noclick_busy = false;
}


#endif