/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef __WIFI_HAL_CLIENTMGT_H__
#define __WIFI_HAL_CLIENTMGT_H__

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Reads whether band steering is currently enabled.
 *
 * Band steering moves a dual-band capable client between the 2.4GHz and 5GHz
 * radios according to the channel-utilization, RSSI, PHY-rate and inactivity
 * thresholds configured through the other `wifi_*BandSteering*` calls. This
 * call reports the master on/off state only; it reports none of those
 * thresholds and none of the AP pairs the feature operates over.
 *
 * @param[out] enable Caller-supplied `BOOL` that receives `TRUE` when band
 *                    steering is enabled and `FALSE` when it is disabled. The
 *                    caller owns the storage (`docs/pages/halSpec.md`, Memory
 *                    Model); the result of passing `NULL` is not specified by
 *                    this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*enable` holds the current state. On failure `*enable`
 *       must be treated as undefined; the single failure code this API defines
 *       does not distinguish causes, so a caller should report band steering
 *       as unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the state was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setBandSteeringEnable()
 */
INT wifi_getBandSteeringEnable(BOOL *enable);

/**
 * @brief Switches band steering on or off.
 *
 * While enabled, the implementation may move dual-band capable clients between
 * the 2.4GHz and 5GHz radios using the thresholds set through the other
 * `wifi_*BandSteering*` calls, across the AP pairs given to
 * `wifi_setBandSteeringApGroup()`. While disabled, a client stays on the radio
 * it associated with and those thresholds have no effect.
 *
 * @param[in] enable `TRUE` to enable band steering, `FALSE` to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getBandSteeringEnable()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the state was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @note This interface does not state whether the setting survives a restart.
 *       `docs/pages/halSpec.md` records that Wi-Fi `HAL` configuration is
 *       maintained by the upper layer, so a caller should not treat the `HAL`
 *       as the store of record for it.
 *
 * @see wifi_getBandSteeringEnable(), wifi_setBandSteeringApGroup()
 */
INT wifi_setBandSteeringEnable(BOOL enable);

/**
 * @brief Reads the AP pairs that band steering operates over.
 *
 * The value is written in the same textual form
 * `wifi_setBandSteeringApGroup()` accepts: one-based AP index pairs,
 * `"$index_2.4G,$index_5G;..."`, for example `"1,2;3,4;7,8"`.
 *
 * @param[out] output_ApGroup Caller-allocated buffer that receives the group
 *                            string, on the representation terms the
 *                            description above states: this interface does not
 *                            establish termination or a length, so every read
 *                            must be bounded by the size the caller allocated.
 *                            The caller allocates and owns it
 *                            (`docs/pages/halSpec.md`, Memory Model).
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `output_ApGroup` holds the value in that form. This
 *       interface does not state whether it is NUL-terminated or how its
 *       length is conveyed, so a caller must not assume either and must bound
 *       every read by the size it allocated. On failure its contents must be
 *       treated as undefined; the single failure code this API defines does
 *       not separate an unconfigured group from a read failure, so a caller
 *       should not parse the buffer.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the group was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @warning The declaration carries no length parameter, and this interface
 *          states neither the size `output_ApGroup` must provide nor an upper
 *          bound on the string written into it. A caller cannot size the
 *          buffer from this interface alone and must take the bound from the
 *          platform it targets.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setBandSteeringApGroup()
 */
INT wifi_getBandSteeringApGroup(char *output_ApGroup);

/**
 * @brief Configures the AP pairs that band steering operates over.
 *
 * Each pair names the 2.4GHz and 5GHz Access Point a client may be moved
 * between, so a client on one member of a pair can only be steered to the
 * other member of that pair. `ApGroup` is a string of one-based AP index pairs
 * in the form `"$index_2.4G,$index_5G;$index_2.4G,$index_5G"`.
 *
 * For example `"1,2;3,4;7,8"` gives three pairs: `1,2` the private network
 * APs, `3,4` the XH network APs and `7,8` the LnF network APs. At least one
 * pair must be present.
 *
 * @param[in] ApGroup Caller-allocated group string in the form above, holding
 *                    at least one pair. The caller passes NUL-terminated text,
 *                    because this interface carries no length parameter for
 *                    the argument and states no maximum length for it. The
 *                    caller owns the buffer (`docs/pages/halSpec.md`, Memory
 *                    Model) and must keep it valid for the duration of the
 *                    call.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getBandSteeringApGroup()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If the group was applied.
 * @retval RETURN_ERR If any error is detected.
 *
 * @warning The declaration carries no length parameter and this interface
 *          states no maximum length for `ApGroup`. It is also declared as a
 *          pointer to non-`const`, so the declaration does not by itself
 *          establish that the implementation leaves the buffer unmodified, and
 *          this interface does not state whether it does; a caller must not
 *          depend on the contents being unchanged on return.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @note This declaration states its result with the `RETURN_OK`/`RETURN_ERR`
 *       spelling; they are aliases of `WIFI_HAL_SUCCESS`/`WIFI_HAL_ERROR` and
 *       carry the same values (`wifi_hal_generic.h`).
 *
 * @see wifi_getBandSteeringApGroup(), wifi_setBandSteeringEnable()
 */
INT wifi_setBandSteeringApGroup(char *ApGroup);

/**
 * @brief Reads the channel-utilization level at which a radio counts as busy
 *        for steering decisions.
 *
 * Band steering weighs this threshold together with the RSSI and PHY-rate
 * thresholds when deciding whether a client should be moved off the radio
 * named by `radioIndex`.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[out] pBuThreshold Caller-supplied `INT` that receives the configured
 *                          threshold. This interface states neither the unit
 *                          nor the range of the value, so a caller must not
 *                          read it as a percentage or as any other particular
 *                          scale.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*pBuThreshold` holds the configured threshold. On failure
 *       `*pBuThreshold` must be treated as undefined; the single failure code
 *       this API defines does not distinguish causes, so a caller should
 *       report the band-utilization threshold as unavailable rather than act
 *       on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the threshold was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setBandSteeringBandUtilizationThreshold()
 */
INT wifi_getBandSteeringBandUtilizationThreshold(INT radioIndex, INT *pBuThreshold);

/**
 * @brief Sets the channel-utilization level at which a radio counts as busy
 *        for steering decisions.
 *
 * Raising the threshold makes the implementation tolerate a busier radio
 * before it considers moving a client off it; lowering it makes steering react
 * to a lighter load. The value takes effect only while band steering is
 * enabled.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[in] buThreshold Threshold to apply. This interface states neither the
 *                        unit nor the accepted range of the value, so a caller
 *                        cannot tell a rejected value from any other failure
 *                        and must take the accepted range from the platform it
 *                        targets.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getBandSteeringBandUtilizationThreshold()` before
 *       relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the threshold was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getBandSteeringBandUtilizationThreshold(),
 *      wifi_setBandSteeringEnable()
 */
INT wifi_setBandSteeringBandUtilizationThreshold(INT radioIndex, INT buThreshold);

/**
 * @brief Reads the signal-strength threshold that steering compares a client
 *        against.
 *
 * The threshold is held per radio and is read back in the same terms
 * `wifi_setBandSteeringRSSIThreshold()` applies it in, which differ between
 * the 2.4GHz and 5GHz bands.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[out] pRssiThreshold Caller-supplied `INT` that receives the
 *                            configured threshold. This interface does not
 *                            state the unit of the value, so a caller must not
 *                            read it as dBm without confirming that against
 *                            the platform it targets.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*pRssiThreshold` holds the configured threshold. On
 *       failure `*pRssiThreshold` must be treated as undefined; the single
 *       failure code this API defines does not distinguish causes, so a caller
 *       should report the RSSI threshold as unavailable rather than act on the
 *       value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the threshold was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setBandSteeringRSSIThreshold()
 */
INT wifi_getBandSteeringRSSIThreshold(INT radioIndex, INT *pRssiThreshold);

/**
 * @brief Sets the signal-strength threshold that steering compares a client
 *        against.
 *
 * The threshold is read in opposite directions on the two bands. For 2.4GHz,
 * the expectation is that a client whose 2.4GHz RSSI is below the value is
 * steered to 2.4GHz. For 5GHz, a client whose 5GHz RSSI is above the value is
 * steered to 5GHz. A caller therefore sets the two radios independently rather
 * than sharing one value between them.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[in] rssiThreshold Threshold to apply, interpreted as above for the
 *                          band `radioIndex` names. This interface states
 *                          neither the unit nor the accepted range, so a
 *                          caller must take both from the platform it targets.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getBandSteeringRSSIThreshold()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the threshold was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getBandSteeringRSSIThreshold()
 */
INT wifi_setBandSteeringRSSIThreshold(INT radioIndex, INT rssiThreshold);

/**
 * @brief Reads the PHY-rate threshold that steering compares a client against.
 *
 * The threshold lets steering act on the rate a client is actually achieving
 * rather than on signal strength alone, which matters for a client that
 * reports a strong signal but negotiates a low rate.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[out] pPrThreshold Caller-supplied `INT` that receives the configured
 *                          threshold. This interface does not state the unit
 *                          of the value, so a caller must not read it as
 *                          Mbit/s without confirming that against the platform
 *                          it targets.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*pPrThreshold` holds the configured threshold. On failure
 *       `*pPrThreshold` must be treated as undefined; the single failure code
 *       this API defines does not distinguish causes, so a caller should
 *       report the PHY-rate threshold as unavailable rather than act on the
 *       value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the threshold was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setBandSteeringPhyRateThreshold()
 */
INT wifi_getBandSteeringPhyRateThreshold(INT radioIndex, INT *pPrThreshold);

/**
 * @brief Sets the PHY-rate threshold that steering compares a client against.
 *
 * Raising the threshold makes steering act on a client that would otherwise be
 * left where it is; lowering it makes steering leave slower clients alone. The
 * value takes effect only while band steering is enabled.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[in] prThreshold Threshold to apply. This interface states neither the
 *                        unit nor the accepted range of the value, so a caller
 *                        must take both from the platform it targets.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getBandSteeringPhyRateThreshold()` before relying on
 *       it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the threshold was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getBandSteeringPhyRateThreshold()
 */
INT wifi_setBandSteeringPhyRateThreshold(INT radioIndex, INT prThreshold);

/**
 * @brief Reads how long a client must be inactive before it is steered while
 *        the radio is overloaded.
 *
 * This is the overload counterpart of the idle inactivity time: it applies
 * when the radio is above the band-utilization threshold, where steering is
 * willing to act sooner.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[out] overloadInactiveTime Caller-supplied `INT` that receives the
 *                                  configured inactivity time, in seconds.
 *                                  This interface does not state the accepted
 *                                  range of the value.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*overloadInactiveTime` holds the configured time in
 *       seconds. On failure `*overloadInactiveTime` must be treated as
 *       undefined; the single failure code this API defines does not
 *       distinguish causes, so a caller should report the overload inactivity
 *       time as unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the time was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setBandSteeringOverloadInactiveTime(),
 *      wifi_getBandSteeringIdleInactiveTime()
 */
INT wifi_getBandSteeringOverloadInactiveTime(INT radioIndex, INT *overloadInactiveTime);

/**
 * @brief Sets how long a client must be inactive before it is steered while
 *        the radio is overloaded.
 *
 * A shorter time makes steering act more readily on a loaded radio; a longer
 * one holds a client in place until it has been quiet for longer. The value
 * takes effect only while band steering is enabled.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[in] overloadInactiveTime Inactivity time to apply, in seconds. This
 *                                 interface does not state the accepted range
 *                                 of the value.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getBandSteeringOverloadInactiveTime()` before relying
 *       on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the time was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getBandSteeringOverloadInactiveTime(),
 *      wifi_setBandSteeringIdleInactiveTime()
 */
INT wifi_setBandSteeringOverloadInactiveTime(INT radioIndex, INT overloadInactiveTime);

/**
 * @brief Reads how long a client must be inactive before it is steered while
 *        the radio is idle.
 *
 * This is the counterpart of the overload inactivity time and applies when the
 * radio is not above the band-utilization threshold.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[out] idleInactiveTime Caller-supplied `INT` that receives the
 *                              configured inactivity time, in seconds. This
 *                              interface does not state the accepted range of
 *                              the value.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*idleInactiveTime` holds the configured time in seconds.
 *       On failure `*idleInactiveTime` must be treated as undefined; the
 *       single failure code this API defines does not distinguish causes, so a
 *       caller should report the idle inactivity time as unavailable rather
 *       than act on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the time was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setBandSteeringIdleInactiveTime(),
 *      wifi_getBandSteeringOverloadInactiveTime()
 */
INT wifi_getBandSteeringIdleInactiveTime(INT radioIndex, INT *idleInactiveTime);

/**
 * @brief Sets how long a client must be inactive before it is steered while
 *        the radio is idle.
 *
 * A longer time keeps a quiet client on its current radio; a shorter one lets
 * steering move it sooner. The value takes effect only while band steering is
 * enabled.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio. `wifi_hal_generic.h` defines
 *                       `RADIO_INDEX_1` to `RADIO_INDEX_3` and bounds the
 *                       count with `MAX_NUM_RADIOS`; an index outside the
 *                       configured range has no specified behaviour.
 *
 * @param[in] idleInactiveTime Inactivity time to apply, in seconds. This
 *                             interface does not state the accepted range of
 *                             the value.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getBandSteeringIdleInactiveTime()` before relying on
 *       it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the time was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getBandSteeringIdleInactiveTime(),
 *      wifi_setBandSteeringOverloadInactiveTime()
 */
INT wifi_setBandSteeringIdleInactiveTime(INT radioIndex, INT idleInactiveTime);

/**
 * @brief Reads one stored band-steering event.
 *
 * Each record describes a single steering decision: when it happened, which
 * client was moved, which SSID it was moved from and to, and why. A caller
 * walks the store by calling this function with successive `record_index`
 * values.
 *
 * @param[in] record_index Index of the record to read. This interface states
 *                         neither the number of records the implementation
 *                         keeps nor the base of the index, so a caller
 *                         discovers the end of the store only by reaching a
 *                         failing call.
 *
 * @param[out] pSteeringTime Caller-supplied `ULONG` that receives the time of
 *                           the steering event, in UTC seconds.
 *
 * @param[out] pClientMAC Caller-allocated buffer that receives the MAC address
 *                        of the steered client. The caller allocates and owns
 *                        it (`docs/pages/halSpec.md`, Memory Model).
 *
 * @param[out] pSourceSSIDIndex Caller-supplied `INT` that receives the index
 *                              of the SSID the client was steered from.
 *
 * @param[out] pDestSSIDIndex Caller-supplied `INT` that receives the index of
 *                            the SSID the client was steered to.
 *
 * @param[out] pSteeringReason Caller-supplied `INT` that receives the reason
 *                             the client was steered. This interface does not
 *                             enumerate the reason values.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success every output parameter has been written for the record
 *       named by `record_index`. On failure all five must be treated as
 *       undefined: the single failure code covers a read error, an index past
 *       the end of the store and the case where no steering has occurred, so a
 *       caller cannot tell them apart and must stop walking rather than skip
 *       the index and continue.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the record was read.
 * @retval WIFI_HAL_ERROR   If any error is detected, if no steering occurred,
 *                          or if the record index is out of bounds.
 *
 * @warning `pClientMAC` carries no length parameter, and this interface states
 *          neither the size the buffer must provide nor the textual form
 *          written into it. `wifi_hal_generic.h` defines `MAC_STR_LEN` as 18
 *          for a MAC address string, but this declaration does not reference
 *          it, so a caller must take the size from the platform it targets.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setBandSteeringEnable()
 */
INT wifi_getBandSteeringLog(INT record_index, ULONG *pSteeringTime, CHAR *pClientMAC, INT *pSourceSSIDIndex, INT *pDestSSIDIndex, INT *pSteeringReason);

/**
 * @brief Reads the associated-device count at which an Access Point is
 *        reported as approaching capacity.
 *
 * `HighWatermarkThreshold` is a monitoring level, not a limit: reaching it
 * does not stop further clients associating. It should be less than or equal
 * to `MaxAssociatedDevices`, which is the absolute maximum number of clients
 * allowed to connect to the Access Point.
 *
 * The default should equal `MaxAssociatedDevices`, except that where
 * `MaxAssociatedDevices` is 0 the default should be 50. A threshold of 0 means
 * no specific limit and disables the watermark calculation.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] output Caller-supplied `UINT` that receives the configured
 *                    `HighWatermarkThreshold`, where 0 means the watermark
 *                    calculation is disabled.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*output` holds the configured threshold. On failure
 *       `*output` must be treated as undefined; the single failure code this
 *       API defines does not distinguish causes, so a caller should report the
 *       threshold as unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If the threshold was read.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @note This declaration states its result with the `RETURN_OK`/`RETURN_ERR`
 *       spelling; they are aliases of `WIFI_HAL_SUCCESS`/`WIFI_HAL_ERROR` and
 *       carry the same values (`wifi_hal_generic.h`).
 *
 * @see wifi_setApAssociatedDevicesHighWatermarkThreshold(),
 *      wifi_getApAssociatedDevicesHighWatermarkThresholdReached()
 */
INT wifi_getApAssociatedDevicesHighWatermarkThreshold(INT apIndex, UINT *output);

/**
 * @brief Sets the associated-device count at which an Access Point is reported
 *        as approaching capacity.
 *
 * Setting the threshold changes only what is reported: it does not cap the
 * number of associated clients. It should be less than or equal to
 * `MaxAssociatedDevices`, which is the absolute maximum number of clients
 * allowed to connect to the Access Point.
 *
 * The default should equal `MaxAssociatedDevices`, except that where
 * `MaxAssociatedDevices` is 0 the default should be 50. Setting 0 means no
 * specific limit and disables the watermark calculation.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] Threshold `HighWatermarkThreshold` to apply, where 0 disables the
 *                      watermark calculation. This interface does not state
 *                      whether a value above `MaxAssociatedDevices` is
 *                      rejected or accepted.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getApAssociatedDevicesHighWatermarkThreshold()` before
 *       relying on it.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If the threshold was applied.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @note This declaration states its result with the `RETURN_OK`/`RETURN_ERR`
 *       spelling; they are aliases of `WIFI_HAL_SUCCESS`/`WIFI_HAL_ERROR` and
 *       carry the same values (`wifi_hal_generic.h`).
 *
 * @see wifi_getApAssociatedDevicesHighWatermarkThreshold()
 */
INT wifi_setApAssociatedDevicesHighWatermarkThreshold(INT apIndex, UINT Threshold);

/**
 * @brief Reads how many times an Access Point has reached its
 *        associated-device threshold.
 *
 * The counter records how often the current total number of associated devices
 * has reached `HighWatermarkThreshold`, which lets a caller see how often the
 * Access Point approaches its client capacity rather than only whether it once
 * did.
 *
 * The count may be derived from `AssociatedDeviceNumberOfEntries` or by
 * another implementation-specific means, and is typically updated whenever the
 * Access Point receives a new association request.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] output Caller-supplied `UINT` that receives the number of times
 *                    the threshold has been reached. This interface does not
 *                    state what resets the counter or whether it saturates.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*output` holds the count. On failure `*output` must be
 *       treated as undefined; the single failure code this API defines does
 *       not distinguish causes, so a caller should report the counter as
 *       unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If the count was read.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @note This declaration states its result with the `RETURN_OK`/`RETURN_ERR`
 *       spelling; they are aliases of `WIFI_HAL_SUCCESS`/`WIFI_HAL_ERROR` and
 *       carry the same values (`wifi_hal_generic.h`).
 *
 * @see wifi_getApAssociatedDevicesHighWatermarkThreshold(),
 *      wifi_getApAssociatedDevicesHighWatermark()
 */
INT wifi_getApAssociatedDevicesHighWatermarkThresholdReached(INT apIndex, UINT *output);

/**
 * @brief Reads the greatest number of devices ever associated with an Access
 *        Point at one time.
 *
 * The value is a peak, not a current count: it is the maximum number of
 * devices concurrently associated with the Access Point since the device or
 * the Wi-Fi module was last reset. It is independent of
 * `HighWatermarkThreshold`, which is a configured reporting level rather than
 * an observed figure.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] output Caller-supplied `UINT` that receives the peak
 *                    concurrently associated device count since the last
 *                    reset.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*output` holds the peak count. On failure `*output` must
 *       be treated as undefined; the single failure code this API defines does
 *       not distinguish causes, so a caller should report the high watermark
 *       as unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the high watermark was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getApAssociatedDevicesHighWatermarkDate(),
 *      wifi_getApAssociatedDevicesHighWatermarkThreshold()
 */
INT wifi_getApAssociatedDevicesHighWatermark(INT apIndex, UINT *output);

/**
 * @brief Reads when an Access Point last reached its peak associated-device
 *        count.
 *
 * The value timestamps the figure `wifi_getApAssociatedDevicesHighWatermark()`
 * returns, so the two are read together: the count on its own does not say
 * whether the peak is recent or dates from shortly after the last reset.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] output_in_seconds Caller-supplied `ULONG` that receives the time
 *                               the peak was reached, in UTC seconds. This
 *                               interface does not state what the value holds
 *                               where the peak has never been reached.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*output_in_seconds` holds the timestamp in UTC seconds. On
 *       failure `*output_in_seconds` must be treated as undefined; the single
 *       failure code this API defines does not distinguish causes, so a caller
 *       should report the high-watermark date as unavailable rather than act
 *       on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the date was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getApAssociatedDevicesHighWatermark()
 */
INT wifi_getApAssociatedDevicesHighWatermarkDate(INT apIndex, ULONG *output_in_seconds);

/**
 * @brief Sets how much 802.11r Fast BSS Transition support an Access Point
 *        offers.
 *
 * Three levels are available. Disabled offers no fast transition. Full support
 * offers it and advertises the mobility domain in beacon frames. Adaptive
 * support is the same as full support except that the Mobility Domain element
 * is not sent in beacon frames, so a client learns of the capability during
 * association rather than from the beacon.
 *
 * The three values are the ordinals of `wifi_fastTrasitionSupport_t`, declared
 * later in this header, although the parameter is declared `UCHAR` rather than
 * that enumeration.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] activate Support level to apply: 0 disabled
 *                     (`FT_SUPPORT_DISABLED`), 1 full FT support
 *                     (`FT_SUPPORT_FULL`), 2 adaptive support
 *                     (`FT_SUPPORT_ADAPTIVE`). The result of passing any other
 *                     value is not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getBSSTransitionActivated()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the support level was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note The matching getter is `wifi_getBSSTransitionActivated()`, which drops
 *       `Fast` from the name and reports the level through a `BOOL *`. A
 *       `BOOL` cannot separate adaptive support from full support, so a caller
 *       that needs to know which of the two is configured cannot recover it
 *       through this interface.
 *
 * @note This API is not `wifi_setBSSTransitionActivation()`. That one is the
 *       802.11v BSS Transition Management switch and takes a `UINT` index; the
 *       two names differ only in their final syllable.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getBSSTransitionActivated(), wifi_pushApFastTransitionConfig(),
 *      wifi_setBSSTransitionActivation()
 */
INT wifi_setFastBSSTransitionActivated(INT apIndex, UCHAR activate);

/**
 * @brief Reads how much 802.11r Fast BSS Transition support an Access Point
 *        offers.
 *
 * The value is the support level last applied by
 * `wifi_setFastBSSTransitionActivated()`, which documents it as 0 disabled, 1
 * full FT support and 2 adaptive support.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] activate Caller-supplied `BOOL` that receives the support level.
 *                      The caller owns the storage (`docs/pages/halSpec.md`,
 *                      Memory Model).
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*activate` holds the support level. On failure `*activate`
 *       must be treated as undefined; the single failure code this API defines
 *       does not distinguish causes, so a caller should report Fast BSS
 *       Transition support as unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the support level was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @warning The setter accepts three levels but this getter returns them
 *          through a `BOOL *`, which cannot represent the third. A caller can
 *          learn whether fast transition is off, but not whether full or
 *          adaptive support is configured.
 *
 * @note This API is not `wifi_getBSSTransitionActivation()`. That one reports
 *       the 802.11v BSS Transition Management switch and takes a `UINT` index;
 *       the two names differ only in their final syllable.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setFastBSSTransitionActivated(), wifi_getBSSTransitionActivation()
 */
INT wifi_getBSSTransitionActivated(INT apIndex, BOOL *activate);
/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief The EAP and EAPOL timing parameters an Access Point uses during
 *        802.1X authentication.
 *
 * The six members form three pairs, each a timeout and a maximum retry count,
 * covering the EAPOL key exchange, the EAP Identity Request and the EAP
 * Request. `wifi_getEAP_Param()` fills the whole structure in one call, while
 * `wifi_setEAP_Param()` writes one setting at a time and selects it by name.
 *
 * This interface does not state the unit of the three timeout members.
 *
 * @see wifi_getEAP_Param(), wifi_setEAP_Param()
 */
typedef struct _wifi_eap_config_t
{
    unsigned int uiEAPOLKeyTimeout; /*!< Time the authenticator waits for a
                                     * reply to an EAPOL key message, M1 or
                                     * M3, before resending it. */
    unsigned int uiEAPOLKeyRetries; /*!< Maximum number of times an
                                     * unanswered EAPOL key message is
                                     * resent before authentication is
                                     * abandoned. */
    unsigned int uiEAPIdentityRequestTimeout; /*!< Time the authenticator
                                               * waits for a reply to an EAP
                                               * Identity Request before
                                               * resending it. */
    unsigned int uiEAPIdentityRequestRetries; /*!< Maximum number of times
                                               * an unanswered EAP Identity
                                               * Request is resent. */
    unsigned int uiEAPRequestTimeout; /*!< Time the authenticator waits for
                                       * a reply to an EAP Request before
                                       * resending it. */
    unsigned int uiEAPRequestRetries; /*!< Maximum number of times an
                                       * unanswered EAP Request is resent. */
} wifi_eap_config_t;
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Sets one of the EAP or EAPOL timing parameters an Access Point uses
 *        during 802.1X authentication.
 *
 * The parameters govern how patient the authenticator is: the EAPOL key
 * exchange, covering the M1 and M3 messages, the EAP Identity Request, and the
 * EAP Request, each with a timeout and a maximum retry count. Lengthening a
 * timeout or raising a retry count makes association more tolerant of a slow
 * or lossy client at the cost of a longer path to reporting failure.
 *
 * @param[in] apIndex Index of the `VAP` the call applies to, declared `UINT`
 *                    here rather than the `INT` used elsewhere in this header.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24` and `MAX_NUM_VAP_PER_RADIO` as 8; an index
 *                    outside the configured range has no specified behaviour.
 *
 * @param[in] value Timeout or retry count to apply. This interface states
 *                  neither the unit of a timeout nor the accepted range of
 *                  either kind of value.
 *
 * @param[in] param Caller-allocated selector naming the setting group:
 *                  `"eapolkey"`, `"eapidentityrequest"` or `"eaprequest"`. The
 *                  caller passes NUL-terminated text, because this interface
 *                  carries no length parameter for the argument and states no
 *                  maximum length for it. The caller owns the buffer
 *                  (`docs/pages/halSpec.md`, Memory Model) and must keep it
 *                  valid for the duration of the call. This interface does not
 *                  state whether the comparison is case sensitive, nor the
 *                  result of passing a name outside that set.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getEAP_Param()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If the setting was applied.
 * @retval RETURN_ERR If any error is detected.
 *
 * @warning Each accepted `param` value names a pair of settings, a timeout and
 *          a retry count, but this interface does not state how a caller
 *          selects one of the pair. `wifi_eap_config_t` exposes the six
 *          settings separately on the read path, so a caller can observe both
 *          halves of a pair but cannot tell from this interface which half a
 *          given call writes.
 *
 * @note `param` is declared as a pointer to non-`const`, so the declaration
 *       does not by itself establish that the implementation leaves the string
 *       unmodified, and this interface does not state whether it does.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @note This declaration states its result with the `RETURN_OK`/`RETURN_ERR`
 *       spelling; they are aliases of `WIFI_HAL_SUCCESS`/`WIFI_HAL_ERROR` and
 *       carry the same values (`wifi_hal_generic.h`).
 *
 * @see wifi_getEAP_Param(), wifi_eap_config_t
 */
INT wifi_setEAP_Param(UINT apIndex, UINT value, char *param);

/**
 * @brief Reads all six EAP and EAPOL timing parameters for an Access Point in
 *        one call.
 *
 * Where `wifi_setEAP_Param()` writes one setting at a time and selects it by
 * name, this call returns the whole set at once as a `wifi_eap_config_t`, with
 * the timeout and the retry count of each of the three groups held in separate
 * members.
 *
 * @param[in] apIndex Index of the `VAP` the call applies to, declared `UINT`
 *                    here rather than the `INT` used elsewhere in this header.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24` and `MAX_NUM_VAP_PER_RADIO` as 8; an index
 *                    outside the configured range has no specified behaviour.
 *
 * @param[out] output Caller-allocated `wifi_eap_config_t` that receives the
 *                    six settings. The caller allocates and owns it
 *                    (`docs/pages/halSpec.md`, Memory Model). This interface
 *                    does not state the unit of the three timeout members.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success every member of `*output` has been written. On failure the
 *       whole structure must be treated as undefined; the single failure code
 *       this API defines does not distinguish causes, so a caller should not
 *       read any member of it.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If the settings were read.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @note This declaration states its result with the `RETURN_OK`/`RETURN_ERR`
 *       spelling; they are aliases of `WIFI_HAL_SUCCESS`/`WIFI_HAL_ERROR` and
 *       carry the same values (`wifi_hal_generic.h`).
 *
 * @see wifi_setEAP_Param(), wifi_eap_config_t
 */
INT wifi_getEAP_Param(UINT apIndex, wifi_eap_config_t *output);

/**
 * @brief Reads whether an Access Point accepts Fast BSS Transition over the
 *        distribution system.
 *
 * FT over DS lets a client run the transition exchange through the Access
 * Point it is still associated with instead of directly with the target, which
 * is what allows a transition where the client cannot yet reach the target
 * over the air. See IEEE 802.11-2016 section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] activate Caller-supplied `BOOL` that receives `TRUE` when FT
 *                      over DS is activated. The caller owns the storage
 *                      (`docs/pages/halSpec.md`, Memory Model).
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*activate` holds the current state. On failure `*activate`
 *       must be treated as undefined; the single failure code this API defines
 *       does not distinguish causes, so a caller should report FT over DS as
 *       unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the state was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note `wifi_setFTOverDSActivated()` has an identical signature to this
 *       function, `(INT, BOOL *)`, so the two differ only by name and a
 *       mis-selected call still compiles.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setFTOverDSActivated()
 */
INT wifi_getFTOverDSActivated(INT apIndex, BOOL *activate);

/**
 * @brief Sets whether an Access Point accepts Fast BSS Transition over the
 *        distribution system.
 *
 * Activating FT over DS lets a client run the transition exchange through its
 * current Access Point rather than directly with the target; deactivating it
 * requires the client to reach the target over the air. See IEEE 802.11-2016
 * section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] activate Caller-supplied `BOOL` holding the state to apply, read
 *                     from the pointer: `TRUE` to activate FT over DS, `FALSE`
 *                     to deactivate it. The caller owns the storage
 *                     (`docs/pages/halSpec.md`, Memory Model) and must keep it
 *                     valid for the duration of the call.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getFTOverDSActivated()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the state was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @warning This setter takes a pointer to non-`const`, so the declaration does
 *          not establish whether the implementation also writes through it,
 *          and this interface does not state whether it does; a caller must
 *          not depend on the pointed-to value being unchanged on return. The
 *          signature is also identical to `wifi_getFTOverDSActivated()`, so
 *          the compiler cannot catch a mis-selected call.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getFTOverDSActivated()
 */
INT wifi_setFTOverDSActivated(INT apIndex, BOOL *activate);

/**
 * @brief Reads the Mobility Domain Identifier an Access Point advertises for
 *        Fast BSS Transition.
 *
 * A client may only make a fast transition between Access Points that
 * advertise the same Mobility Domain Identifier, so the value defines the set
 * of Access Points it can roam across without a full reassociation. See IEEE
 * 802.11-2016 section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] mobilityDomain Caller-allocated array of exactly two `UCHAR`, as
 *                            the declaration states, that receives the
 *                            Mobility Domain Identifier. The caller allocates
 *                            and owns it (`docs/pages/halSpec.md`, Memory
 *                            Model).
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success both elements of `mobilityDomain` have been written. On
 *       failure both must be treated as undefined; the single failure code
 *       this API defines does not distinguish causes, so a caller should not
 *       act on the array.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the identifier was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note The identifier is a 16-bit value: `wifi_FastTransitionConfig_t` in
 *       this header carries the same setting as a `USHORT` (`mobilityDomain`).
 *       This interface does not state the octet order in which the two array
 *       elements hold it, so a caller must not assume either ordering when
 *       converting between the array form and the `USHORT` form.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setFTMobilityDomainID(), wifi_FastTransitionConfig_t
 */
INT wifi_getFTMobilityDomainID(INT apIndex, UCHAR mobilityDomain[2]);

/**
 * @brief Sets the Mobility Domain Identifier an Access Point advertises for
 *        Fast BSS Transition.
 *
 * Two Access Points must carry the same identifier before a client can make a
 * fast transition between them, so the value is applied consistently across
 * every Access Point intended to form one mobility domain. Under adaptive
 * support the identifier is still set but is not sent in beacon frames. See
 * IEEE 802.11-2016 section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] mobilityDomain Caller-allocated array of exactly two `UCHAR`, as
 *                           the declaration states, holding the Mobility
 *                           Domain Identifier to apply. The caller owns it
 *                           (`docs/pages/halSpec.md`, Memory Model) and must
 *                           keep it valid for the duration of the call.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getFTMobilityDomainID()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the identifier was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note The identifier is a 16-bit value: `wifi_FastTransitionConfig_t` in
 *       this header carries the same setting as a `USHORT` (`mobilityDomain`).
 *       This interface does not state the octet order in which the two array
 *       elements hold it, so a caller must not assume either ordering when
 *       converting between the array form and the `USHORT` form.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getFTMobilityDomainID(), wifi_setFastBSSTransitionActivated()
 */
INT wifi_setFTMobilityDomainID(INT apIndex, UCHAR mobilityDomain[2]);

/**
 * @brief Reads whether an Access Point accepts the Fast BSS Transition
 *        resource request protocol.
 *
 * The resource request protocol lets a client reserve resources at the target
 * Access Point before it transitions, so it can find out whether the target
 * can carry it while it is still associated with the current one. See IEEE
 * 802.11-2016 section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] supported Caller-supplied `BOOL` that receives `TRUE` when the
 *                       resource request protocol is supported. The caller
 *                       owns the storage (`docs/pages/halSpec.md`, Memory
 *                       Model).
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*supported` holds the current state. On failure
 *       `*supported` must be treated as undefined; the single failure code
 *       this API defines does not distinguish causes, so a caller should
 *       report resource request support as unavailable rather than act on the
 *       value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the state was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note `wifi_setFTResourceRequestSupported()` has an identical signature to
 *       this function, `(INT, BOOL *)`, so the two differ only by name and a
 *       mis-selected call still compiles.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setFTResourceRequestSupported()
 */
INT wifi_getFTResourceRequestSupported(INT apIndex, BOOL *supported);

/**
 * @brief Sets whether an Access Point accepts the Fast BSS Transition resource
 *        request protocol.
 *
 * Declaring support lets a client reserve resources at this Access Point
 * before transitioning to it; withdrawing it requires the client to transition
 * without that assurance. See IEEE 802.11-2016 section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] supported Caller-supplied `BOOL` holding the state to apply, read
 *                      from the pointer: `TRUE` to declare support, `FALSE` to
 *                      withdraw it. The caller owns the storage
 *                      (`docs/pages/halSpec.md`, Memory Model) and must keep
 *                      it valid for the duration of the call.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getFTResourceRequestSupported()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the state was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @warning This setter takes a pointer to non-`const`, so the declaration does
 *          not establish whether the implementation also writes through it,
 *          and this interface does not state whether it does; a caller must
 *          not depend on the pointed-to value being unchanged on return. The
 *          signature is also identical to
 *          `wifi_getFTResourceRequestSupported()`, so the compiler cannot
 *          catch a mis-selected call.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getFTResourceRequestSupported()
 */
INT wifi_setFTResourceRequestSupported(INT apIndex, BOOL *supported);

/**
 * @brief Reads how long an Access Point keeps a Fast BSS Transition R0 key
 *        valid.
 *
 * The R0 key sits at the top of the fast transition key hierarchy. Once its
 * lifetime expires a client must authenticate in full again rather than
 * transition, so the value bounds how long roaming stays cheap for a given
 * client. See IEEE 802.11-2016 section 13.4.2.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] lifetime Caller-supplied `UINT` that receives the configured
 *                      lifetime. The caller owns the storage
 *                      (`docs/pages/halSpec.md`, Memory Model). This interface
 *                      does not state the unit of the value.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*lifetime` holds the configured lifetime. On failure
 *       `*lifetime` must be treated as undefined; the single failure code this
 *       API defines does not distinguish causes, so a caller should report the
 *       R0 key lifetime as unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the lifetime was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note `wifi_FastTransitionConfig_t` in this header carries the same setting
 *       as a `USHORT` (`r0KeyLifeTime`) while this API uses `UINT`, so a value
 *       that does not fit in 16 bits cannot be carried through
 *       `wifi_pushApFastTransitionConfig()`.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setFTR0KeyLifetime(), wifi_pushApFastTransitionConfig()
 */
INT wifi_getFTR0KeyLifetime(INT apIndex, UINT *lifetime);

/**
 * @brief Sets how long an Access Point keeps a Fast BSS Transition R0 key
 *        valid.
 *
 * A longer lifetime keeps fast transition available to a client for longer; a
 * shorter one forces a full authentication sooner. See IEEE 802.11-2016
 * section 13.4.2.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] lifetime Caller-supplied `UINT` holding the lifetime to apply,
 *                     read from the pointer. The caller owns the storage
 *                     (`docs/pages/halSpec.md`, Memory Model) and must keep it
 *                     valid for the duration of the call. This interface
 *                     states neither the unit nor the accepted range of the
 *                     value.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getFTR0KeyLifetime()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the lifetime was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @warning This setter takes a pointer to non-`const`, so the declaration does
 *          not establish whether the implementation also writes through it,
 *          and this interface does not state whether it does; a caller must
 *          not depend on the pointed-to value being unchanged on return.
 *
 * @note `wifi_FastTransitionConfig_t` in this header carries the same setting
 *       as a `USHORT` (`r0KeyLifeTime`) while this API uses `UINT`, so a value
 *       that does not fit in 16 bits cannot be carried through
 *       `wifi_pushApFastTransitionConfig()`.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getFTR0KeyLifetime(), wifi_pushApFastTransitionConfig()
 */
INT wifi_setFTR0KeyLifetime(INT apIndex, UINT *lifetime);

/**
 * @brief Reads the R0 Key Holder identifier an Access Point uses for Fast BSS
 *        Transition.
 *
 * The R0 Key Holder is the entity holding the top-level key of the fast
 * transition hierarchy. A client uses the identifier to work out which
 * hierarchy a target Access Point belongs to, and so whether a key it already
 * holds can be reused there. See IEEE 802.11-2016 section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] keyHolderID Caller-allocated buffer that receives the R0 Key
 *                         Holder identifier. The caller allocates and owns it
 *                         (`docs/pages/halSpec.md`, Memory Model).
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `keyHolderID` holds the identifier. On failure its contents
 *       must be treated as undefined; the single failure code this API defines
 *       does not distinguish causes, so a caller should not parse the buffer.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the identifier was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @warning The declaration carries no length parameter, and this interface
 *          states neither the size the buffer must provide nor whether the
 *          value is NUL-terminated. The nearest bound this header gives is
 *          `wifi_r0KH_t::nasId`, declared `nas_id_t`, which
 *          `wifi_hal_generic.h` defines as `char[49]`; this declaration does
 *          not reference it, so a caller must take the size from the platform
 *          it targets.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setFTR0KeyHolderID(), wifi_r0KH_t
 */
INT wifi_getFTR0KeyHolderID(INT apIndex, UCHAR *keyHolderID);

/**
 * @brief Sets the R0 Key Holder identifier an Access Point uses for Fast BSS
 *        Transition.
 *
 * Access Points that are meant to share one fast transition key hierarchy are
 * given the same R0 Key Holder identifier, so the value is applied
 * consistently across that set. See IEEE 802.11-2016 section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] keyHolderID Caller-allocated buffer holding the R0 Key Holder
 *                        identifier to apply. The caller owns it
 *                        (`docs/pages/halSpec.md`, Memory Model) and must keep
 *                        it valid for the duration of the call.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getFTR0KeyHolderID()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the identifier was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @warning The declaration carries no length parameter, and this interface
 *          states neither the size the buffer must provide nor whether the
 *          value is NUL-terminated. The nearest bound this header gives is
 *          `wifi_r0KH_t::nasId`, declared `nas_id_t`, which
 *          `wifi_hal_generic.h` defines as `char[49]`; this declaration does
 *          not reference it, so a caller must take the size from the platform
 *          it targets.
 *
 * @note The parameter is declared as a pointer to non-`const`, so the
 *       declaration does not by itself establish that the implementation
 *       leaves the buffer unmodified, and this interface does not state
 *       whether it does.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getFTR0KeyHolderID(), wifi_r0KH_t
 */
INT wifi_setFTR0KeyHolderID(INT apIndex, UCHAR *keyHolderID);

/**
 * @brief Reads the R1 Key Holder identifier an Access Point uses for Fast BSS
 *        Transition.
 *
 * The R1 Key Holder sits below the R0 Key Holder in the fast transition key
 * hierarchy and is per Access Point, so the identifier names this Access Point
 * within the hierarchy the R0 Key Holder identifier names. See IEEE
 * 802.11-2016 section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] keyHolderID Caller-allocated buffer that receives the R1 Key
 *                         Holder identifier. The caller allocates and owns it
 *                         (`docs/pages/halSpec.md`, Memory Model).
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `keyHolderID` holds the identifier. On failure its contents
 *       must be treated as undefined; the single failure code this API defines
 *       does not distinguish causes, so a caller should not parse the buffer.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the identifier was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @warning The declaration carries no length parameter, and this interface
 *          states neither the size the buffer must provide nor whether the
 *          value is NUL-terminated. The nearest bound this header gives is
 *          `wifi_r1KH_t::r1khId`, declared `mac_address_t`, which
 *          `wifi_hal_generic.h` defines as six octets; this declaration does
 *          not reference it, so a caller must take the size from the platform
 *          it targets.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setFTR1KeyHolderID(), wifi_r1KH_t
 */
INT wifi_getFTR1KeyHolderID(INT apIndex, UCHAR *keyHolderID);

/**
 * @brief Sets the R1 Key Holder identifier an Access Point uses for Fast BSS
 *        Transition.
 *
 * The identifier distinguishes this Access Point from its peers inside one
 * fast transition key hierarchy, so each Access Point in a mobility domain is
 * given its own value. See IEEE 802.11-2016 section 13.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] keyHolderID Caller-allocated buffer holding the R1 Key Holder
 *                        identifier to apply. The caller owns it
 *                        (`docs/pages/halSpec.md`, Memory Model) and must keep
 *                        it valid for the duration of the call.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getFTR1KeyHolderID()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the identifier was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @warning The declaration carries no length parameter, and this interface
 *          states neither the size the buffer must provide nor whether the
 *          value is NUL-terminated. The nearest bound this header gives is
 *          `wifi_r1KH_t::r1khId`, declared `mac_address_t`, which
 *          `wifi_hal_generic.h` defines as six octets; this declaration does
 *          not reference it, so a caller must take the size from the platform
 *          it targets.
 *
 * @note The parameter is declared as a pointer to non-`const`, so the
 *       declaration does not by itself establish that the implementation
 *       leaves the buffer unmodified, and this interface does not state
 *       whether it does.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_getFTR1KeyHolderID(), wifi_r1KH_t
 */
INT wifi_setFTR1KeyHolderID(INT apIndex, UCHAR *keyHolderID);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief The levels of 802.11r Fast BSS Transition support an Access Point can
 *        offer.
 *
 * The ordinals are the values `wifi_setFastBSSTransitionActivated()` accepts
 * for its `activate` argument, although that parameter is declared `UCHAR`
 * rather than this enumeration.
 *
 * @note The type name is spelt `wifi_fastTrasitionSupport_t` in the
 *       declaration. The spelling is retained unchanged for source
 *       compatibility with existing consumers.
 *
 * @see wifi_setFastBSSTransitionActivated(), wifi_FastTransitionConfig_t
 */
typedef enum
{
    FT_SUPPORT_DISABLED, /*!< No fast transition; a client authenticates in
                          * full at every Access Point. */
    FT_SUPPORT_FULL, /*!< Fast transition offered, with the Mobility Domain
                      * element carried in beacon frames. */
    FT_SUPPORT_ADAPTIVE /*!< Fast transition offered as for
                         * `FT_SUPPORT_FULL`, except that the Mobility
                         * Domain element is not sent in beacon frames, so a
                         * client learns of the capability during
                         * association. */
} wifi_fastTrasitionSupport_t;

/**
 * @brief The number of entries in the R0 and R1 key holder tables of
 *        `wifi_FastTransitionConfig_t`.
 *
 * It bounds both `r0KH` and `r1KH`, so neither `numR0KHs` nor `numR1KHs` may
 * exceed it.
 *
 * @see wifi_FastTransitionConfig_t
 */
#define MAX_KEY_HOLDERS 8

/**
 * @brief One R0 key holder in a Fast BSS Transition key hierarchy.
 *
 * An R0 key holder holds the top-level key of the hierarchy. An Access Point
 * carries a table of them in `wifi_FastTransitionConfig_t::r0KH` so that it
 * can exchange keys with the peers a client may transition between.
 *
 * @see wifi_FastTransitionConfig_t, wifi_getFTR0KeyHolderID()
 */
typedef struct
{
    mac_address_t mac; /*!< MAC address at which this R0 key holder is
                        * reached. */
    nas_id_t nasId; /*!< NAS identifier of this R0 key holder, the value
                     * `wifi_setFTR0KeyHolderID()` applies. Declared
                     * `nas_id_t`, `char[49]` in `wifi_hal_generic.h`. */
    r0r1_key_t key; /*!< Shared key protecting key transfers with this
                     * holder. Declared `r0r1_key_t`, 16 octets. */
} wifi_r0KH_t;

/**
 * @brief One R1 key holder in a Fast BSS Transition key hierarchy.
 *
 * An R1 key holder sits one level below the R0 key holder and is per Access
 * Point. An Access Point carries a table of them in
 * `wifi_FastTransitionConfig_t::r1KH` so that it can exchange derived keys
 * with its peers.
 *
 * @see wifi_FastTransitionConfig_t, wifi_getFTR1KeyHolderID()
 */
typedef struct
{
    mac_address_t mac; /*!< MAC address at which this R1 key holder is
                        * reached. */
    mac_address_t r1khId; /*!< Identifier of this R1 key holder, the value
                           * `wifi_setFTR1KeyHolderID()` applies. Declared
                           * `mac_address_t`, six octets. */
    r0r1_key_t key; /*!< Shared key protecting key transfers with this
                     * holder. Declared `r0r1_key_t`, 16 octets. */
} wifi_r1KH_t;

/**
 * @brief A complete 802.11r Fast BSS Transition configuration for one Access
 *        Point.
 *
 * `wifi_pushApFastTransitionConfig()` applies the whole structure in one call.
 * Most members have a matching `wifi_getFT*` and `wifi_setFT*` pair, but the
 * R0 and R1 key holder tables do not, so this structure is the only way to
 * install them.
 *
 * Two members are narrower here than in the per-setting calls:
 * `mobilityDomain` is a `USHORT` where `wifi_getFTMobilityDomainID()` uses two
 * octets, and `r0KeyLifeTime` is a `USHORT` where `wifi_setFTR0KeyLifetime()`
 * takes a `UINT`.
 *
 * @see wifi_pushApFastTransitionConfig(), MAX_KEY_HOLDERS
 */
typedef struct
{
    wifi_fastTrasitionSupport_t support; /*!< Level of fast transition
                                          * support to offer. */
    USHORT mobilityDomain; /*!< Mobility Domain Identifier, held here as a
                            * 16-bit value; `wifi_setFTMobilityDomainID()`
                            * carries the same setting as two octets. */
    BOOL overDS; /*!< `TRUE` to accept fast transition over the distribution
                  * system. */
    nas_id_t r0KeyHolder; /*!< R0 key holder identifier for this Access
                           * Point. Declared `nas_id_t`, `char[49]`. */
    USHORT r0KeyLifeTime; /*!< Lifetime of the R0 key. Narrower than the
                           * `UINT` `wifi_setFTR0KeyLifetime()` takes, so a
                           * larger value cannot be carried here. Unit not
                           * specified by this interface. */
    mac_address_t r1KeyHolder; /*!< R1 key holder identifier for this Access
                                * Point. Declared `mac_address_t`, six
                                * octets. */
    USHORT reassocDeadLine; /*!< Deadline for a client to complete
                             * reassociation after a fast transition. Unit
                             * not specified by this interface. */
    BOOL pmkR1Push; /*!< `TRUE` to distribute derived R1 keys to the holders
                     * listed in `r1KH` rather than have a holder request
                     * one when it is needed. */
    UCHAR numR0KHs; /*!< How many leading entries of `r0KH` hold a valid R0
                     * key holder. */
    wifi_r0KH_t r0KH[MAX_KEY_HOLDERS]; /*!< R0 key holders this Access Point
                                        * may exchange keys with. `numR0KHs`
                                        * gives how many leading entries are
                                        * valid and must not exceed
                                        * `MAX_KEY_HOLDERS`. */
    UCHAR numR1KHs; /*!< How many leading entries of `r1KH` hold a valid R1
                     * key holder. */
    wifi_r1KH_t r1KH[MAX_KEY_HOLDERS]; /*!< R1 key holders this Access Point
                                        * may exchange keys with. `numR1KHs`
                                        * gives how many leading entries are
                                        * valid and must not exceed
                                        * `MAX_KEY_HOLDERS`. */
} wifi_FastTransitionConfig_t;

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Applies a complete Fast BSS Transition configuration to an Access
 *        Point in one call.
 *
 * The structure carries the support level, mobility domain, over-DS switch,
 * key lifetime and the R0 and R1 key holder tables together, so a caller can
 * install a consistent configuration instead of building it up through the
 * individual `wifi_setFT*` calls. The key holder tables are the part that has
 * no per-setting equivalent, so this is the only way to install them.
 *
 * @param[in] apIndex Index of the Access Point the call applies to.
 *                    `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] ftData Caller-allocated `wifi_FastTransitionConfig_t` holding the
 *                   configuration to apply. Every member is read, so a caller
 *                   must populate all of them; in particular `numR0KHs` and
 *                   `numR1KHs` bound the valid entries of `r0KH` and `r1KH`
 *                   and must not exceed `MAX_KEY_HOLDERS`. The caller owns the
 *                   structure (`docs/pages/halSpec.md`, Memory Model) and must
 *                   keep it valid for the duration of the call; this interface
 *                   does not state whether the implementation retains the
 *                   pointer beyond it.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the configuration is in effect. On failure this interface
 *       does not state whether any part of it was applied, so a caller should
 *       read the individual settings back through the `wifi_getFT*` calls
 *       before relying on them.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the configuration was applied.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This interface does not state how this call interacts with the
 *       individual `wifi_setFT*` calls, or which takes precedence where both
 *       are used on one Access Point, so a caller that mixes the two should
 *       read the settings back.
 *
 * @note `ftData` is declared as a pointer to non-`const`, so the declaration
 *       does not by itself establish that the implementation leaves the
 *       structure unmodified, and this interface does not state whether it
 *       does.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_FastTransitionConfig_t, wifi_setFastBSSTransitionActivated()
 */
INT wifi_pushApFastTransitionConfig(INT apIndex, wifi_FastTransitionConfig_t *ftData);

/**
 * @brief Activates or deactivates 802.11v BSS Transition Management on an
 *        Access Point.
 *
 * Activated and deactivated mean enabled and disabled; the words are used here
 * because that is the terminology of the 802.11 specification.
 *
 * The switch changes two things at once. While deactivated the gateway ignores
 * BTM report requests, as defined in IEEE 802.11-2016 section 11.11.10.3, and
 * the BSS Transition bit in any Extended Capabilities element the Access Point
 * named by `apIndex` sends out is set according to `activate`. A client
 * therefore both stops being asked to move and stops being told the capability
 * exists.
 *
 * @param[in] apIndex Index of the Access Point the call applies to, declared
 *                    `UINT` here rather than the `INT` used elsewhere in this
 *                    header. `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] activate `TRUE` to activate BSS Transition Management, `FALSE` to
 *                     deactivate it.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getBSSTransitionActivation()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If the state was applied.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note This API and `wifi_setFastBSSTransitionActivated()` control different
 *       features whose names differ only in their final syllable: `Activation`
 *       here is the 802.11v BSS Transition Management switch, while
 *       `Activated` is the 802.11r Fast BSS Transition support level. They
 *       also take different index types, `UINT` here and `INT` there.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @note This declaration states its result with the `RETURN_OK`/`RETURN_ERR`
 *       spelling; they are aliases of `WIFI_HAL_SUCCESS`/`WIFI_HAL_ERROR` and
 *       carry the same values (`wifi_hal_generic.h`).
 *
 * @see wifi_getBSSTransitionActivation(), wifi_setFastBSSTransitionActivated()
 */
INT wifi_setBSSTransitionActivation(UINT apIndex, BOOL activate);

/**
 * @brief Reads whether 802.11v BSS Transition Management is active on an
 *        Access Point.
 *
 * The value is the state last applied by `wifi_setBSSTransitionActivation()`,
 * which governs both whether the gateway acts on BTM report requests and
 * whether the Access Point advertises the capability.
 *
 * @param[in] apIndex Index of the Access Point the call applies to, declared
 *                    `UINT` here rather than the `INT` used elsewhere in this
 *                    header. `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] activate Caller-supplied `BOOL` that receives `TRUE` when BSS
 *                      Transition Management is activated. The caller owns the
 *                      storage (`docs/pages/halSpec.md`, Memory Model).
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*activate` holds the current state. On failure `*activate`
 *       must be treated as undefined; the single failure code this API defines
 *       does not distinguish causes, so a caller should report BSS Transition
 *       Management as unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the state was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This API is not `wifi_getBSSTransitionActivated()`. That one reports
 *       the 802.11r Fast BSS Transition support level and takes an `INT`
 *       index; the two names differ only in their final syllable.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setBSSTransitionActivation(), wifi_getBSSTransitionActivated()
 */
INT wifi_getBSSTransitionActivation(UINT apIndex, BOOL *activate);

/**
 * @brief Activates or deactivates neighbor reporting on an Access Point.
 *
 * A neighbor report tells a client which nearby Access Points it might move
 * to, so the client can scan a short candidate list rather than every channel.
 * Activated and deactivated mean enabled and disabled; the words are used here
 * because that is the terminology of the 802.11 specification.
 *
 * While deactivated the gateway ignores neighbor report requests, as defined
 * in IEEE 802.11-2016 section 11.11.10.3.
 *
 * @param[in] apIndex Index of the Access Point the call applies to, declared
 *                    `UINT` here rather than the `INT` used elsewhere in this
 *                    header. `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[in] activate `TRUE` to activate neighbor reporting, `FALSE` to
 *                     deactivate it.
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success the new value is in effect. On failure this interface does
 *       not state whether the value was applied, so a caller should read it
 *       back with `wifi_getNeighborReportActivation()` before relying on it.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If the state was applied.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note Neighbor reporting and BSS Transition Management are usually
 *       configured together, the report naming candidate Access Points and the
 *       transition request asking a client to move to one. This interface does
 *       not couple the two switches, so each must be set separately.
 *
 * @note The `wifi_hal.h` umbrella records these definitions as the 802.11k
 *       Neighbor Request and Response support added in Wi-Fi `HAL` 2.11.0.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @note This declaration states its result with the `RETURN_OK`/`RETURN_ERR`
 *       spelling; they are aliases of `WIFI_HAL_SUCCESS`/`WIFI_HAL_ERROR` and
 *       carry the same values (`wifi_hal_generic.h`).
 *
 * @see wifi_getNeighborReportActivation(), wifi_setBSSTransitionActivation()
 */
INT wifi_setNeighborReportActivation(UINT apIndex, BOOL activate);

/**
 * @brief Reads whether neighbor reporting is active on an Access Point.
 *
 * The value is the state last applied by `wifi_setNeighborReportActivation()`,
 * which governs whether the gateway answers neighbor report requests.
 *
 * @param[in] apIndex Index of the Access Point the call applies to, declared
 *                    `UINT` here rather than the `INT` used elsewhere in this
 *                    header. `wifi_hal_generic.h` defines `AP_INDEX_1` to
 *                    `AP_INDEX_24`; an index outside the configured range has
 *                    no specified behaviour.
 *
 * @param[out] activate Caller-supplied `BOOL` that receives `TRUE` when
 *                      neighbor reporting is activated. The caller owns the
 *                      storage (`docs/pages/halSpec.md`, Memory Model).
 *
 * @pre `wifi_init()` must have completed successfully; see
 *      `Initialization and Startup` in `docs/pages/halSpec.md`. This interface
 *      does not specify the outcome of a call made beforehand: neither the
 *      status code nor the effect of the call is established, so a caller must
 *      not rely on either.
 * @post On success `*activate` holds the current state. On failure `*activate`
 *       must be treated as undefined; the single failure code this API defines
 *       does not distinguish causes, so a caller should report neighbor
 *       reporting as unavailable rather than act on the value.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If the state was read.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note The `wifi_hal.h` umbrella records these definitions as the 802.11k
 *       Neighbor Request and Response support added in Wi-Fi `HAL` 2.11.0.
 *
 * @note This function must not suspend and must not invoke any blocking system
 *       calls, and the `HAL` is expected to be thread safe, so it may be
 *       called concurrently from separate caller threads
 *       (`docs/pages/halSpec.md`, Blocking calls and Threading Model).
 *
 * @see wifi_setNeighborReportActivation()
 */
INT wifi_getNeighborReportActivation(UINT apIndex, BOOL *activate);

/** @} */  //END OF GROUP WIFI_HAL_APIS

#ifdef __cplusplus
}
#endif

#endif