#ifndef __RESPONSIVEANALOGREAD_H
#define __RESPONSIVEANALOGREAD_H


class ResponsiveAnalogRead {
	public:

    __attribute__((noinline)) ResponsiveAnalogRead(uint32_t adcIndex, uint32_t threshold ) {
        index = adcIndex;
        thres = threshold;
    }

    __attribute__((noinline)) ~ResponsiveAnalogRead() {

    }

	uint32_t getValue() {
        /* Get the responsive value from last update */
        return responsiveValue;
	}


	__attribute__((noinline)) void update() {
		prevResponsiveValue = responsiveValue;
		responsiveValue = getResponsiveValue(adcvalues[index]);
	}


	__attribute__((noinline)) uint32_t getResponsiveValue(uint32_t newValue) {

		/*
		 * Sleep and edge snap are hard-enabled.
		 * If the new value is very close to an edge, drag it a little closer to the edges.
		 * This'll make it easier to pull the output values right to the extremes without sleeping,
		 * and it'll make movements right near the edge appear larger, making it easier to wake up
           */
		if (newValue < thres) {
			newValue = (newValue << 1) - thres;
		}
		else if (newValue > analogResolution - thres) {
			newValue = (newValue << 1) - analogResolution + thres;
		}

		/* 
		 * Measure the difference between the new value and current value
		 * and use another exponential moving average to work out what
		 * the current margin of error is.
		 */
		errorEMA += ___SMMUL(((newValue - smoothValue) - errorEMA), 0x40000000);

		/* Recalculate sleeping status */
		isSleeping = abs(errorEMA) < thres;

		/*
		 * If sleeping,then don't update responsiveValue in this k-cycle,
		 * just output the existing responsiveValue.
		 */
		if (isSleeping) {
			return smoothValue;
		}

		/* Calculate the exponential moving average. */
		smoothValue += (newValue - smoothValue);

		/* Ensure output is within bounds */
		if (smoothValue < 0) {
			smoothValue = 0;
		}
        	else if (smoothValue > analogResolution - 1) {
			smoothValue = analogResolution - 1;
		}

		/* Expected output is unsigned integer. */
		return (uint32_t) smoothValue;
	}


	private:

	static const uint16_t analogResolution = 4096; /* ADC has 12 bit resolution */

    uint32_t index;
    uint32_t thres;
	uint32_t responsiveValue;
	uint32_t prevResponsiveValue;

	int32_t smoothValue = 0;
	int32_t errorEMA = 0;

	bool_t isSleeping = false;
};

#endif