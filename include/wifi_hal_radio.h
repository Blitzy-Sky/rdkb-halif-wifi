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
#ifndef __WIFI_HAL_RADIO_H__
#define __WIFI_HAL_RADIO_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "wifi_hal_ap.h"

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

/**
 * @brief Selects the OFDM guard interval a radio transmits with.
 *
 * A shorter interval raises throughput and a longer one tolerates more delay
 * spread, so the choice trades capacity against multipath robustness. The values
 * are distinct bits, which lets a capability field advertise several at once
 * while `wifi_setGuardInterval()` applies exactly one.
 */
typedef enum
{
    wifi_guard_interval_400 = 0x01,   /**< 400ns guard interval. */
    wifi_guard_interval_800 = 0x02,   /**< 800ns guard interval. */
    wifi_guard_interval_1600 = 0x04,  /**< 1600ns guard interval. */
    wifi_guard_interval_3200 = 0x08,  /**< 3200ns guard interval. */
    wifi_guard_interval_auto = 0x10,  /**< Automatic guard interval selection. */
} wifi_guard_interval_t;

/**
 * @brief Element count of `wifi_radio_operationParam_t::channelSecondary`, bounding
 *        how many secondary channels may accompany one primary channel.
 *
 * @note The name is spelt without word separators, unlike the `MAX_NUM_*` form used
 *       elsewhere in this header. It is retained exactly as spelt for source
 *       compatibility.
 */
#define MAXNUMSECONDARYCHANNELS 7
/**
 * @brief Element count of `wifi_radio_operationParam_t::channels_per_bandwidth`, one
 *        entry per `wifi_channelBandwidth_t` value.
 *
 * @note This is a bound on the array, not a claim that a radio supports every
 *       bandwidth; `wifi_channels_list_per_bandwidth_t::chanwidth` identifies which
 *       bandwidth a given entry describes.
 */
#define MAX_NUM_CHANNELBANDWIDTH_SUPPORTED  6

/**
 * @brief Identifies which stage of DFS radar handling a radio has reached.
 *
 * A radio operating on a DFS channel must monitor for radar and vacate the channel
 * when it detects one, so these values let a caller distinguish a detection from the
 * channel-availability check and non-occupancy period that follow it.
 */
typedef enum
{
    WIFI_EVENT_RADAR_DETECTED,       /**< Radar detected. */
    WIFI_EVENT_RADAR_CAC_FINISHED,   /**< Radar Channel Availability Check (CAC) finished. */
    WIFI_EVENT_RADAR_CAC_ABORTED,    /**< Radar CAC aborted. */
    WIFI_EVENT_RADAR_NOP_FINISHED,   /**< Radar Non-Occupancy Period (NOP) finished. */
    WIFI_EVENT_RADAR_PRE_CAC_EXPIRED, /**< Radar pre-CAC expired. */
    WIFI_EVENT_RADAR_CAC_STARTED     /**< Radar CAC started. */
} wifi_radar_eventType_t;

/**
 * @brief Reports whether a channel may be used now, and if not, what is holding it.
 *
 * `CHAN_STATE_AVAILABLE` means the channel is usable; every other value names a DFS
 * stage that must complete first. The enumeration carries no ordering rule, so a
 * caller must not infer a transition sequence from the declaration order.
 */
typedef enum
{
    CHAN_STATE_AVAILABLE = 1,        /**< Channel is available. */
    CHAN_STATE_DFS_NOP_FINISHED,     /**< DFS NOP finished. */
    CHAN_STATE_DFS_NOP_START,       /**< DFS NOP started. */
    CHAN_STATE_DFS_CAC_START,       /**< DFS CAC started. */
    CHAN_STATE_DFS_CAC_COMPLETED    /**< DFS CAC completed. */
} wifi_channelState_t;

/**
 * @brief Pairs one channel number with its current usability.
 *
 * `wifi_radio_operationParam_t::channel_map` carries an array of these, which is how
 * a caller learns which channels a radio may currently be moved to.
 */
typedef struct _wifi_channelMap_t
{
    INT ch_number;              /**< IEEE 802.11 channel number this entry describes. */
    wifi_channelState_t ch_state; /**< Usability, or the DFS stage blocking it. */
} wifi_channelMap_t;

/**
 * @brief Describes which 20MHz sub-channels are punctured out of a wide 802.11be channel.
 *
 * Preamble puncturing lets a radio keep a wide channel while excluding individual
 * 20MHz sub-channels that are unusable, rather than falling back to a narrower
 * bandwidth. The structure is packed to match the vendor layout, so it must not be
 * copied field-by-field into a padded equivalent.
 */
typedef struct
{
    USHORT punct_bitmap; /**< A bitmap of disabled 20MHz channels. */
    UCHAR punct_acs_threshold; /**< Puncturing ACS threshold. */
} __attribute__((packed)) wifi_radio_11be_puncturing_info_t;

/**
 * @brief Element count of `wifi_operating_classes_t::nonOperable`, bounding how many
 *        non-operable channels may be reported for one operating class.
 *
 * @note The name is spelt without word separators, and reads as a per-channel bound
 *       although the array it dimensions is per operating class. It is retained
 *       exactly as spelt for source compatibility; a future revision of this
 *       interface is expected to rename it for readability.
 */
#define MAXNUMNONOPERABLECHANNELS 10

/**
 * @brief Element count of `wifi_radio_operationParam_t::operatingClasses`, bounding
 *        how many operating classes may be reported for one radio band.
 *
 * @note As with `MAXNUMNONOPERABLECHANNELS`, the name is spelt without word
 *       separators and is retained exactly as spelt for source compatibility; a
 *       future revision of this interface is expected to rename it for readability.
 */
#define MAXNUMOPERCLASSESPERBAND 20

/**
 * @brief Element count of `wifi_radio_operationParam_t::amsduTid`, one entry per WMM
 *        traffic identifier.
 *
 * Each traffic identifier represents a user priority and traffic class as defined by
 * WMM, so the array enables or disables A-MSDU aggregation per priority rather than
 * for the radio as a whole.
 */
#define MAX_AMSDU_TID 8

/**
 * @brief Operating Classes information.
 *
 * Structure that holds information of the operating class,
 * such as maximum transmit power, array of non-operable channel
 * associated with that operating class.
 */
typedef struct
{
    UINT opClass;                                /**< Global operating Class value */
    INT maxTxPower;                              /**< Max Tx Power */
    UINT numberOfNonOperChan;                    /**< Number of Nonoperable channels */
    UINT nonOperable[MAXNUMNONOPERABLECHANNELS]; /**< Array of Non Operable channel value */
} __attribute__((packed)) wifi_operating_classes_t;

/**
 * @brief Radio temperature information.
 *
 * Structure that holds the radio temperature information.
 */
typedef struct _wifi_radioTemperature_t
{
    UINT radio_Temperature; /**< Wi-Fi radio chipset temperature. */
} wifi_radioTemperature_t;

/**
 * @brief Wi-Fi radio operation parameters.
 *
 * This structure contains the operation parameters for a Wi-Fi radio.
 */
typedef struct 
{
    BOOL enable;                /**< Whether the radio is enabled. */
    wifi_freq_bands_t band;    /**< The radio frequency band. */
    BOOL autoChannelEnabled;     /**< Whether auto channel selection is enabled. */
    UINT channel;               /**< The radio primary channel. */
    UINT numSecondaryChannels;  /**< The number of secondary channels in the list. */
    UINT channelSecondary[MAXNUMSECONDARYCHANNELS]; /**< The list of secondary radio channels. */
    wifi_channelBandwidth_t channelWidth; /**< The channel bandwidth. */
    wifi_ieee80211Variant_t variant; /**< The radio operating mode. */
    UINT csa_beacon_count; /**< Specifies how long Channel Switch Announcement (CSA) needs to be announced. */
    wifi_countrycode_type_t countryCode; /**< The country code. */
    UINT regDomain; /**< The regulatory domain. */
    wifi_operating_env_t operatingEnvironment; /**< The Wi-Fi operating environment. */
    wifi_channelMap_t channel_map[64]; /**< Channel map. */
    BOOL DCSEnabled; /**< Whether Dynamic Channel Selection (DCS) is enabled. */
    UINT dtimPeriod; /**< The DTIM period. */
    UINT beaconInterval; /**< The beacon interval. */
    UINT operatingClass; /**< The operating class. */
    UINT basicDataTransmitRates; /**< The basic data transmit rates in Mbps. It uses bitmask to return multiple bitrates and wifi_bitrate_t has the definition of valid values. */
    UINT operationalDataTransmitRates; /**< The operational data transmit rates in Mbps. It uses bitmask to return multiple bitrates and wifi_bitrate_t has the definition of valid values. */
    UINT fragmentationThreshold; /**< The fragmentation threshold in bytes. */
    wifi_guard_interval_t guardInterval; /**< The guard interval. */
    UINT transmitPower; /**< The transmit power in percentage, e.g., "75", "100". */
    UINT rtsThreshold; /**< The packet size threshold in bytes to apply RTS/CTS backoff rules. */
    BOOL factoryResetSsid; /**< Whether to factory reset the SSID. */
    UINT radioStatsMeasuringRate; /**< The rate at which radio statistics are measured. */
    UINT radioStatsMeasuringInterval; /**< The interval at which radio statistics are measured. */
    BOOL ctsProtection; /**< Whether CTS protection is enabled. */
    BOOL obssCoex; /**< Whether OBSS coex is enabled. */
    BOOL stbcEnable; /**< Whether STBC is enabled. */
    BOOL greenFieldEnable; /**< Whether greenfield is enabled. */
    UINT userControl; /**< User control. */
    UINT adminControl; /**< Admin control. */
    UINT chanUtilThreshold; /**< Channel utilization threshold. */
    BOOL chanUtilSelfHealEnable; /**< Whether channel utilization self-healing is enabled. */
    BOOL DfsEnabled; /**< Whether DFS is enabled. */
    BOOL DfsEnabledBootup; /**< Whether DFS is enabled on bootup. */
    BOOL EcoPowerDown; /**< Whether eco power down is enabled. */
    wifi_radio_11be_puncturing_info_t puncturingInfo; /**< Puncturing information. */
    UINT autoChanRefreshPeriod; /**< Auto channel refresh period. */
    INT mcs; /**< MCS index. */
    BOOL amsduEnable; /**< Whether AMSDU is enabled. */
    BOOL amsduTid[MAX_AMSDU_TID]; /**< Whether AMSDU is enabled for particular traffic id. */
    UINT DFSTimer; /**< DFS timer. */
    char radarDetected[256]; /**< Radar detected information. */
    BOOL acs_keep_out_reset; /**< ACS Keep Out Channels list to be reset */
    wifi_channels_list_per_bandwidth_t  channels_per_bandwidth[MAX_NUM_CHANNELBANDWIDTH_SUPPORTED]; /**< All the channel list for a particular channel bandwidth */
    UINT numOperatingClasses; /**< Number of valid operating classes in the array operatingClasses */
    wifi_operating_classes_t operatingClasses[MAXNUMOPERCLASSESPERBAND]; /**< Array of supported Operating classes as per Data elements Schema */
} __attribute__((packed)) wifi_radio_operationParam_t;

/**
 * @brief Enhanced Distributed Channel Access (EDCA) parameters.
 */
typedef struct
{
    CHAR aifsn; /**< Arbitration Inter-Frame Space (AIFS) number. */
    CHAR cw_min; /**< Minimum contention window size. */
    CHAR cw_max; /**< Maximum contention window size. */
    CHAR timer; /**< Timer value. */
} wifi_edca_t;

/**
 * @brief Downlink data acknowledgement types.
 */
typedef enum
{
    wifi_dl_data_ack_immediate, /**< Immediate acknowledgement. */
    wifi_dl_data_block_ack_immediate, /**< Immediate block acknowledgement. */
    wifi_dl_data_block_ack_deferred, /**< Deferred block acknowledgement. */
} wifi_dl_data_ack_type_t;

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Reads how many times a radio has been reset.
 *
 * The count lets a caller detect a radio that is recovering repeatedly, which is
 * usually a symptom rather than a configuration state. This interface does not state
 * the epoch the count is measured from or whether it survives a reboot, so a caller
 * should compare successive readings rather than treat one reading as absolute.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_int  Caller-allocated `ULONG` that receives the reset count. The
 *                         caller allocates and releases it; the `HAL` writes into it
 *                         and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the count; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The count was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_int` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments; a failure that persists across
 *                          retries should be logged and the count treated as
 *                          unavailable.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_getRadioResetCount(INT radioIndex, ULONG *output_int);

/**
 * @brief Restores every radio to its factory default configuration.
 *
 * Access Point parameters are deliberately left untouched, so this is a radio-level
 * reset rather than a whole-interface one; a caller that needs both must reset the
 * Access Points separately. The call takes no arguments, so it cannot be scoped to a
 * subset of radios - use `wifi_factoryResetRadio()` for that.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success every radio holds its factory default parameters. On failure this
 *       interface does not state whether the reset was applied to some radios and not
 *       others, so a caller should read the parameters back rather than assume either.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS Every radio was reset.
 * @retval WIFI_HAL_ERROR   The vendor layer could not complete the reset. The caller
 *                          should read the radio parameters back with
 *                          `wifi_getRadioOperatingParameters()` to establish the
 *                          resulting state before retrying.
 *
 * @warning This discards every radio setting a caller has applied, including channel,
 *          bandwidth and transmit power. It is not a diagnostic call.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_factoryResetRadio
 */
INT wifi_factoryResetRadios();

/**
 * @brief Restores one radio to its factory default configuration.
 *
 * Access Point parameters are deliberately left untouched, so this resets the radio's
 * own parameters only. This is the scoped counterpart of `wifi_factoryResetRadios()`.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`. Declared as `int` rather than the `INT`
 *                        used elsewhere in this header; the two are the same type.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio holds its factory default parameters. On failure the
 *       configuration is left unspecified, so a caller should read it back.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The radio was reset.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, or the vendor layer could not
 *                          complete the reset. The caller should validate the index and
 *                          read the parameters back before retrying.
 *
 * @warning This discards every setting a caller has applied to this radio.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_factoryResetRadios
 */
INT wifi_factoryResetRadio(int radioIndex);

/**
 * @brief Reads whether a radio is administratively enabled.
 *
 * This reports the configured intent, not what the hardware is currently doing: a
 * radio can be enabled here and still not be operational. Use
 * `wifi_getRadioStatus()` for the operational answer.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_bool  Caller-allocated `BOOL` that receives `TRUE` when the radio
 *                          is enabled and `FALSE` when it is not. The caller allocates
 *                          and releases it; the `HAL` writes into it and retains no
 *                          reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the administrative state; on failure it is left
 *       unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_bool` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the state as unknown rather
 *                          than assuming a default.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioEnable
 * @see wifi_getRadioStatus
 */
INT wifi_getRadioEnable(INT radioIndex, BOOL *output_bool);

/**
 * @brief Brings a radio up or takes it down.
 *
 * Disabling a radio takes every Access Point hosted on it off the air, so this is the
 * widest-reaching radio control in this header. This interface does not state whether
 * the change survives a reboot; `Persistence Model` in `docs/pages/halSpec.md` places
 * configuration persistence with the upper layer.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[in] enable      `TRUE` to enable the radio, `FALSE` to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio holds the requested administrative state. On failure the
 *       state is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, or the vendor layer could not
 *                          apply the change. The caller should read the state back with
 *                          `wifi_getRadioEnable()` rather than retrying blindly.
 *
 * @note Enabling a radio does not by itself make it operational; poll
 *       `wifi_getRadioStatus()` for that.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioEnable
 */
INT wifi_setRadioEnable(INT radioIndex, BOOL enable);

/**
 * @brief Reads whether a radio is operationally up.
 *
 * This is the observed state of the hardware, which is what a caller needs after
 * enabling a radio or changing its channel. It can differ from the administrative
 * state returned by `wifi_getRadioEnable()` - for example while a DFS channel
 * availability check is still in progress.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_bool  Caller-allocated `BOOL` that receives `TRUE` when the radio
 *                          is operational and `FALSE` when it is not. The caller
 *                          allocates and releases it; the `HAL` writes into it and
 *                          retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the operational state; on failure it is left
 *       unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_bool` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and retry the poll rather than
 *                          concluding the radio is down.
 *
 * @note This interface specifies no transition model for the operational state, so a
 *       caller waiting for a radio to come up must poll rather than expect an event.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioEnable
 */
INT wifi_getRadioStatus(INT radioIndex, BOOL *output_bool);

/**
 * @brief Reads the operating-system interface name a radio is exposed under.
 *
 * The name is what a caller needs to correlate this radio with a network interface it
 * can see outside the `HAL`, for example when reading kernel statistics.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_string  Caller-allocated, caller-owned character buffer that
 *                            receives the NUL-terminated interface name. The `HAL`
 *                            writes into it and retains no reference to it after
 *                            returning, so a caller may pass a stack buffer safely.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the buffer holds a NUL-terminated name; on failure its contents are
 *       left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The name was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_string` is NULL, or the
 *                          vendor layer could not supply the name. The caller should
 *                          validate its arguments and treat the name as unavailable
 *                          rather than reading the buffer.
 *
 * @warning The signature carries no buffer length, so this interface does not specify
 *          how many bytes the `HAL` may write. A caller must size the buffer from its
 *          own platform's interface-name limit; there is no way for the `HAL` to
 *          detect a buffer that is too small.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_getRadioIfName(INT radioIndex, CHAR *output_string);

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Reads the channels a radio is currently occupying.
 *
 * A radio using a wide bandwidth occupies several 20MHz channels at once, so the
 * answer is a list rather than a single number. This is the observed occupancy, not
 * the configured primary channel, which `wifi_getRadioOperatingParameters()` reports.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] channel_list  Caller-allocated `wifi_channels_list_t` that receives the
 *                           occupied channels in `channels_list` and their count in
 *                           `num_channels`. The caller allocates and releases it; the
 *                           `HAL` writes into it and retains no reference to it after
 *                           returning. The array holds `MAX_CHANNELS` entries and only
 *                           the first `num_channels` are written.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success `num_channels` gives the number of valid entries; on failure the
 *       structure is left unspecified, so a caller must not read `num_channels`.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The list was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `channel_list` is NULL, or the
 *                          vendor layer could not supply the list. The caller should
 *                          validate its arguments and treat the occupancy as unknown.
 *
 * @note This declaration is compiled only when `WIFI_HAL_VERSION_3_PHASE2` is defined,
 *       so a caller must guard its use with the same macro.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioOperatingParameters
 */
INT wifi_getRadioChannelsInUse(wifi_radio_index_t radioIndex, wifi_channels_list_t* channel_list);
#endif

/**
 * @brief Reads whether a radio may select DFS channels.
 *
 * Dynamic Frequency Selection governs whether the radio is allowed onto channels that
 * are shared with radar. With DFS disabled the radio is restricted to non-DFS
 * channels, which is why a channel set may be refused even though the hardware
 * supports it.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_bool  Caller-allocated `BOOL` that receives `TRUE` when DFS is
 *                          enabled and `FALSE` when it is not. The caller allocates and
 *                          releases it; the `HAL` writes into it and retains no
 *                          reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the DFS setting; on failure it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_bool` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the setting as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioDfsEnable
 * @see wifi_getRadioDfsAtBootUpEnable
 */
INT wifi_getRadioDfsEnable(INT radioIndex, BOOL *output_bool);

/**
 * @brief Allows or forbids a radio's use of DFS channels.
 *
 * Enabling DFS widens the channel set the radio may be moved to, at the cost of
 * accepting radar-driven channel changes; disabling it confines the radio to non-DFS
 * channels. This setting applies from now on - use
 * `wifi_setRadioDfsAtBootUpEnable()` to change what the radio does at the next boot.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`. Declared here as `INT`, unlike the `UINT`
 *                        that `wifi_setZeroDFSState()` takes.
 * @param[in] enabled     `TRUE` to permit DFS channels, `FALSE` to forbid them.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio holds the requested DFS setting. On failure the setting is
 *       unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, the regulatory domain does not
 *                          permit the request, or the vendor layer could not apply it.
 *                          The caller should read the setting back with
 *                          `wifi_getRadioDfsEnable()` rather than retrying blindly.
 *
 * @warning Disabling DFS while the radio is operating on a DFS channel may force a
 *          channel change. This interface does not state which channel is selected, so
 *          a caller should read the operating parameters back afterwards.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioDfsEnable
 */
INT wifi_setRadioDfsEnable(INT radioIndex, BOOL enabled);

/**
 * @brief Reads whether a radio will have DFS enabled after the next boot.
 *
 * This is the boot-time intent rather than the running setting, so it can differ from
 * `wifi_getRadioDfsEnable()` after a caller has changed one and not the other.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] enable     Caller-allocated `BOOL` that receives `TRUE` when DFS is to be
 *                        enabled at boot and `FALSE` when it is not. The caller
 *                        allocates and releases it; the `HAL` writes into it and retains
 *                        no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the boot-time setting; on failure it is left
 *       unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `enable` is NULL, or the vendor
 *                          layer could not supply the value. The caller should validate
 *                          its arguments and treat the setting as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioDfsAtBootUpEnable
 * @see wifi_getRadioDfsEnable
 */
INT wifi_getRadioDfsAtBootUpEnable(INT radioIndex, BOOL *enable);

/**
 * @brief Sets whether a radio will have DFS enabled after the next boot.
 *
 * The change takes effect at the next boot and does not alter the running setting, so
 * a caller that wants both must also call `wifi_setRadioDfsEnable()`.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[in] enable      `TRUE` to enable DFS at the next boot, `FALSE` to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the boot-time setting holds the requested value. On failure it is
 *       unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, or the vendor layer could not
 *                          store the setting. The caller should read it back with
 *                          `wifi_getRadioDfsAtBootUpEnable()` rather than retrying
 *                          blindly.
 *
 * @note Storage of this setting across a reboot is the implementation's
 *       responsibility; `Persistence Model` in `docs/pages/halSpec.md` places general
 *       configuration persistence with the upper layer.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioDfsAtBootUpEnable
 */
INT wifi_setRadioDfsAtBootUpEnable(INT radioIndex, BOOL enable);

/**
 * @brief Reads the Modulation Coding Scheme index a radio is configured with.
 *
 * The MCS index fixes the modulation and coding rate, and therefore the data rate the
 * radio transmits at. This interface does not state the index range, which depends on
 * the 802.11 variant in use, so a caller should treat the value as opaque unless it
 * knows the variant from `wifi_getRadioOperatingParameters()`.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_INT  Caller-allocated `INT` that receives the MCS index. The caller
 *                         allocates and releases it; the `HAL` writes into it and
 *                         retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the configured index; on failure it is left
 *       unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The index was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_INT` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the index as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioMCS
 */
INT wifi_getRadioMCS(INT radioIndex, INT *output_INT);

/**
 * @brief Fixes the Modulation Coding Scheme index a radio transmits at.
 *
 * Setting an index pins the modulation and coding rate instead of leaving rate
 * selection to the driver, which is useful for testing and harmful in production if
 * the chosen rate is not sustainable on the link.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[in] MCS         MCS index to apply. This interface does not state the accepted
 *                        range, which depends on the 802.11 variant the radio is
 *                        operating in, so a caller must not infer a bound from a
 *                        failure.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio transmits using the requested index. On failure the
 *       configuration is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The index was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, the vendor layer rejected
 *                          `MCS`, or it could not apply the change. The caller should
 *                          read the value back with `wifi_getRadioMCS()` rather than
 *                          retrying with the same argument.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioMCS
 */
INT wifi_setRadioMCS(INT radioIndex, INT MCS);

/**
 * @brief Reads a radio's current transmit power in dBm.
 *
 * This is the absolute power, which is what a caller needs to reason about regulatory
 * limits and link budget. `wifi_getRadioPercentageTransmitPower()` reports the same
 * setting relative to the radio's maximum, and `wifi_setRadioTransmitPower()` accepts
 * only the percentage form - so this value is read-only through this interface.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_ulong  Caller-allocated `ULONG` that receives the transmit power in
 *                           dBm. The caller allocates and releases it; the `HAL` writes
 *                           into it and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the power in dBm; on failure it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The power was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_ulong` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the power as unknown rather
 *                          than assuming a maximum.
 *
 * @note The parameter is unsigned, so this interface cannot express a negative dBm
 *       value through it.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioPercentageTransmitPower
 */
INT wifi_getRadioTransmitPower(INT radioIndex, ULONG *output_ulong);

/**
 * @brief Reads a radio's current transmit power as a percentage of its maximum.
 *
 * This is the same setting `wifi_setRadioTransmitPower()` writes, so it is the form to
 * read back after a set; `wifi_getRadioTransmitPower()` gives the absolute dBm figure.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_ulong  Caller-allocated `ULONG` that receives the power as a
 *                           percentage of full power, in the range 0 to 100. The caller
 *                           allocates and releases it; the `HAL` writes into it and
 *                           retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the percentage; on failure it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The percentage was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_ulong` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the setting as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioTransmitPower
 * @see wifi_getRadioTransmitPower
 */
INT wifi_getRadioPercentageTransmitPower(INT radioIndex, ULONG *output_ulong);

/**
 * @brief Sets a radio's transmit power as a percentage of its maximum.
 *
 * Expressing the setting as a percentage lets one value be applied across radios with
 * different absolute maxima. The corresponding dBm figure can then be read with
 * `wifi_getRadioTransmitPower()`.
 *
 * @param[in] radioIndex     Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                           `MAX_NUM_RADIOS`.
 * @param[in] TransmitPower  Power to apply, as a percentage of full power in the range
 *                           0 to 100. `wifi_radio_operationParam_t::transmitPower`
 *                           documents the same quantity, giving 75 and 100 as examples.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio transmits at the requested percentage. On failure the
 *       setting is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The power was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `TransmitPower` is outside the
 *                          accepted range, or the vendor layer could not apply the
 *                          change. The caller should clamp the value to 0 to 100 and
 *                          read the setting back rather than retrying unchanged.
 *
 * @warning Reducing transmit power shrinks coverage and can disconnect associated
 *          clients at the edge of the cell.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioPercentageTransmitPower
 */
INT wifi_setRadioTransmitPower(INT radioIndex, ULONG TransmitPower);

/**
 * @brief Reads the span of carrier sense thresholds a radio will accept.
 *
 * A caller needs this before calling `wifi_setRadioCarrierSenseThresholdInUse()`,
 * because the accepted span is a hardware property and differs between radios.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output     Caller-allocated `INT` that receives the supported threshold
 *                        range in dBm. The caller allocates and releases it; the `HAL`
 *                        writes into it and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the supported range; on failure it is left
 *       unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The range was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output` is NULL, or the vendor
 *                          layer could not supply the value. The caller should validate
 *                          its arguments and treat the range as unknown rather than
 *                          assuming one.
 *
 * @note A single `INT` cannot carry two endpoints, so this interface does not state
 *       whether the value is a span, a minimum or a maximum. A caller should not derive
 *       a bound for `wifi_setRadioCarrierSenseThresholdInUse()` from it without
 *       vendor-specific knowledge.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioCarrierSenseThresholdInUse
 */
INT wifi_getRadioCarrierSenseThresholdRange(INT radioIndex, INT *output);

/**
 * @brief Reads the RSSI level at which a radio declares the channel busy.
 *
 * This is the radio's CS/CCA (Carrier Sense/Clear Channel Assessment) threshold.
 * Raising it makes the radio ignore weak neighbouring transmissions, which is how an
 * Access Point avoids being held off the air by many faint Wi-Fi sources in a dense
 * deployment.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output     Caller-allocated `INT` that receives the threshold in dBm. The
 *                        caller allocates and releases it; the `HAL` writes into it and
 *                        retains no reference to it after returning. The type is signed
 *                        because an RSSI threshold is normally negative.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the threshold in use; on failure it is left
 *       unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The threshold was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output` is NULL, or the vendor
 *                          layer could not supply the value. The caller should validate
 *                          its arguments and treat the threshold as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioCarrierSenseThresholdInUse
 */
INT wifi_getRadioCarrierSenseThresholdInUse(INT radioIndex, INT *output);

/**
 * @brief Sets the RSSI level at which a radio declares the channel busy.
 *
 * Raising the CS/CCA threshold reduces how often the radio defers to weak neighbouring
 * transmissions, trading interference tolerance for airtime. Read the supported span
 * with `wifi_getRadioCarrierSenseThresholdRange()` first.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[in] threshold   Threshold to apply, in dBm. Signed, because an RSSI threshold
 *                        is normally negative. The accepted span is a hardware property
 *                        for which this interface states no fixed bound.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio uses the requested threshold. On failure the setting is
 *       unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The threshold was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, the vendor layer rejected
 *                          `threshold`, or it could not apply the change. The caller
 *                          should read the supported range and the value in use back,
 *                          rather than retrying with the same argument.
 *
 * @warning Setting the threshold too high makes the radio transmit over ongoing
 *          neighbouring transmissions, which degrades both networks.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioCarrierSenseThresholdInUse
 * @see wifi_getRadioCarrierSenseThresholdRange
 */
INT wifi_setRadioCarrierSenseThresholdInUse(INT radioIndex, INT threshold);

/**
 * @brief Commits previously staged radio-level settings to the hardware.
 *
 * Several setters in this header stage a value rather than acting on it immediately;
 * this call makes the staged set active in one step, which avoids reconfiguring the
 * radio once per individual change.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the staged settings are active in the hardware. On failure this
 *       interface does not state whether any part of the set was applied, so a caller
 *       should read the operating parameters back rather than assume either.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The staged settings were applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, or the vendor layer could not
 *                          apply the staged set. The caller should read the parameters
 *                          back with `wifi_getRadioOperatingParameters()` to establish
 *                          the resulting state.
 *
 * @note This interface does not enumerate which setters stage rather than apply, so a
 *       caller that wants a deterministic result should configure the radio through
 *       `wifi_setRadioOperatingParameters()`, which takes the whole parameter set at
 *       once.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioOperatingParameters
 */
INT wifi_applyRadioSettings(INT radioIndex);

/**
 * @brief Enables or disables CTS protection on a radio.
 *
 * CTS protection makes the radio reserve the medium before transmitting, so that
 * stations which cannot decode its higher-rate preamble still defer. It costs airtime
 * and is worth enabling only where legacy stations share the channel.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[in] enable      `TRUE` to enable CTS protection, `FALSE` to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `RETURN_ERR` and
 *      changes nothing.
 * @post On success the radio holds the requested setting. On failure it is unchanged.
 *
 * @return The status of the operation.
 * @retval RETURN_OK  CTS protection was set as requested.
 * @retval RETURN_ERR `radioIndex` is out of range, or the vendor layer could not apply
 *                    the change. The caller should validate the index and read the
 *                    radio's operating parameters back rather than retrying blindly.
 *
 * @execution Synchronous
 * @sideeffect None
 *
 * @note This block states its outcome as `RETURN_OK`/`RETURN_ERR` where its neighbours
 *       use `WIFI_HAL_SUCCESS`/`WIFI_HAL_ERROR`. The two pairs are numerically identical
 *       aliases defined in `wifi_hal_generic.h`, and the spelling is retained here
 *       because it is this API's own documented contract.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioObssCoexistenceEnable
 */
INT wifi_setRadioCtsProtectionEnable(INT radioIndex, BOOL enable);

/**
 * @brief Enables or disables OBSS coexistence for the radio behind an Access Point.
 *
 * With OBSS (Overlapping Basic Service Set) coexistence enabled the radio falls back
 * to a 20MHz channel width when a neighbouring BSS would otherwise be interfered
 * with, trading throughput for coexistence.
 *
 * @param[in] apIndex  Index of the Access Point whose radio is affected, in the range
 *                     `AP_INDEX_1` to `AP_INDEX_24`. Note that this is an Access Point
 *                     index, unlike the radio index every other setter in this header
 *                     takes, even though the setting applies to the whole radio.
 * @param[in] enable   `TRUE` to enable OBSS coexistence, `FALSE` to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio holds the requested setting. On failure it is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not
 *                          apply the change. The caller should validate the index and
 *                          read the radio's operating parameters back rather than
 *                          retrying blindly.
 *
 * @warning Because the setting is radio-wide, applying it through one Access Point
 *          index affects every other Access Point hosted on the same radio.
 *
 * @note Enabling this can reduce the channel width below the configured value, so a
 *       caller comparing `wifi_getRadioOperatingParameters()` against what it set
 *       should expect a difference.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioCtsProtectionEnable
 */
INT wifi_setRadioObssCoexistenceEnable(INT apIndex, BOOL enable);

/**
 * @brief Sets the frame size above which a radio fragments transmissions.
 *
 * Fragmenting large frames raises the chance each piece survives a noisy channel, at
 * the cost of per-fragment overhead. A threshold at or above the maximum frame size
 * effectively disables fragmentation.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[in] threshold   Threshold in bytes. Unsigned, and documented for the same
 *                        quantity by `wifi_radio_operationParam_t::fragmentationThreshold`.
 *                        This interface states no accepted range, so a caller must not
 *                        infer a bound from a failure.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio fragments at the requested size. On failure the setting is
 *       unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The threshold was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, the vendor layer rejected
 *                          `threshold`, or it could not apply the change. The caller
 *                          should read the value back through
 *                          `wifi_getRadioOperatingParameters()` rather than retrying
 *                          with the same argument.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_setRadioFragmentationThreshold(INT radioIndex, UINT threshold);

/**
 * @brief Enables or disables Space-Time Block Coding on a radio.
 *
 * STBC transmits the same data across several antennas so a receiver can combine them,
 * which improves range and reliability on a marginal link without raising the data
 * rate.
 *
 * @param[in] radioIndex   Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[in] STBC_Enable  `TRUE` to enable STBC, `FALSE` to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio holds the requested setting. On failure it is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, the radio does not support
 *                          STBC, or the vendor layer could not apply the change. The
 *                          caller should read
 *                          `wifi_radio_operationParam_t::stbcEnable` back rather than
 *                          retrying blindly.
 *
 * @note This interface provides no capability query for STBC, so a caller cannot
 *       distinguish an unsupported radio from a transient failure by the return value
 *       alone.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_setRadioSTBCEnable(INT radioIndex, BOOL STBC_Enable);

/**
 * @brief Reads whether a radio aggregates frames into A-MSDUs.
 *
 * This reports the radio-wide setting only. Per-traffic-identifier control is carried
 * by `wifi_radio_operationParam_t::amsduTid`, which this call does not expose.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_bool  Caller-allocated `BOOL` that receives `TRUE` when A-MSDU
 *                          aggregation is enabled and `FALSE` when it is not. The caller
 *                          allocates and releases it; the `HAL` writes into it and
 *                          retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the setting; on failure it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_bool` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the setting as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioAMSDUEnable
 */
INT wifi_getRadioAMSDUEnable(INT radioIndex, BOOL *output_bool);

/**
 * @brief Enables or disables A-MSDU aggregation on a radio.
 *
 * Aggregating several MSDUs into one frame cuts per-frame overhead and raises
 * throughput, at the cost of losing the whole aggregate when the frame is lost.
 *
 * @param[in] radioIndex   Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[in] amsduEnable  `TRUE` to enable aggregation, `FALSE` to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio holds the requested setting. On failure it is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, or the vendor layer could not
 *                          apply the change. The caller should read the setting back
 *                          with `wifi_getRadioAMSDUEnable()` rather than retrying
 *                          blindly.
 *
 * @note This is the radio-wide switch. To vary aggregation per WMM traffic identifier,
 *       set `wifi_radio_operationParam_t::amsduTid` through
 *       `wifi_setRadioOperatingParameters()`; that array holds `MAX_AMSDU_TID` entries.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioAMSDUEnable
 */
INT wifi_setRadioAMSDUEnable(INT radioIndex, BOOL amsduEnable);

/**
 * @brief Reads how long a radio has been running since it last started.
 *
 * Read together with `wifi_getRadioResetCount()` this distinguishes a radio that has
 * been stable for a long period from one that has just restarted.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] uptime     Caller-allocated `ULONG` that receives the elapsed time in
 *                        seconds since the radio started. The caller allocates and
 *                        releases it; the `HAL` writes into it and retains no reference
 *                        to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the uptime in seconds; on failure it is left
 *       unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The uptime was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `uptime` is NULL, or the vendor
 *                          layer could not supply the value. The caller should validate
 *                          its arguments and treat the uptime as unavailable.
 *
 * @note This interface does not state what the value reads while the radio is down, so
 *       a caller should confirm the radio is operational with `wifi_getRadioStatus()`
 *       before interpreting it.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioResetCount
 */
INT wifi_getRadioUpTime(INT radioIndex, ULONG *uptime);

/**
 * @brief Reports whether a radio supports Reverse Direction Grant.
 *
 * RDG lets a station transmit back inside a TXOP the Access Point already holds,
 * saving a separate contention round. This is a capability query, not a setting, so a
 * caller uses it to decide whether an RDG-dependent feature can be offered at all.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_bool  Caller-allocated `BOOL` that receives `TRUE` when the radio
 *                          supports RDG and `FALSE` when it does not. The caller
 *                          allocates and releases it; the `HAL` writes into it and
 *                          retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the capability; on failure it is left unspecified,
 *       so a caller must not read it as "unsupported".
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The capability was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_bool` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments; a failure is not evidence that RDG
 *                          is unsupported.
 *
 * @note This interface exposes no setter for RDG, so the capability cannot be turned on
 *       or off through this header.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_getRadioReverseDirectionGrantSupported(INT radioIndex, BOOL *output_bool);

/**
 * @brief Reads whether a radio negotiates block acknowledgement automatically.
 *
 * Automatic block ACK lets the radio set up block-ACK sessions itself instead of
 * requiring them to be arranged externally, which is what makes frame aggregation
 * effective in practice.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_bool  Caller-allocated `BOOL` that receives `TRUE` when automatic
 *                          block ACK is enabled and `FALSE` when it is not. The caller
 *                          allocates and releases it; the `HAL` writes into it and
 *                          retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the setting; on failure it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_bool` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the setting as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioAutoBlockAckEnable
 */
INT wifi_getRadioAutoBlockAckEnable(INT radioIndex, BOOL *output_bool);

/**
 * @brief Enables or disables automatic block-acknowledgement negotiation on a radio.
 *
 * Disabling it suppresses the radio's own block-ACK setup, which removes most of the
 * benefit of frame aggregation and is normally only useful for testing.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[in] enable      `TRUE` to enable automatic block ACK, `FALSE` to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio holds the requested setting. On failure it is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, or the vendor layer could not
 *                          apply the change. The caller should read the setting back
 *                          with `wifi_getRadioAutoBlockAckEnable()` rather than retrying
 *                          blindly.
 *
 * @note This interface does not state whether the change affects block-ACK sessions
 *       already established, so a caller must not assume existing sessions are torn
 *       down.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioAutoBlockAckEnable
 */
INT wifi_setRadioAutoBlockAckEnable(INT radioIndex, BOOL enable);

/**
 * @brief Reads whether a radio snoops IGMP to steer multicast traffic.
 *
 * With snooping enabled the radio forwards a multicast group's frames only towards
 * stations that joined it, instead of flooding every associated station - which
 * matters most on a busy radio carrying video.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] output_bool  Caller-allocated `BOOL` that receives `TRUE` when IGMP
 *                          snooping is enabled and `FALSE` when it is not. The caller
 *                          allocates and releases it; the `HAL` writes into it and
 *                          retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the setting; on failure it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_bool` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the setting as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioIGMPSnoopingEnable
 */
INT wifi_getRadioIGMPSnoopingEnable(INT radioIndex, BOOL *output_bool);

/**
 * @brief Enables or disables IGMP snooping on a radio.
 *
 * Enabling snooping confines each multicast group to the stations that joined it.
 * Disabling it makes the radio flood multicast to every associated station, which
 * wastes airtime but is sometimes needed by clients that do not join correctly.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[in] enable      `TRUE` to enable IGMP snooping, `FALSE` to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio holds the requested setting. On failure it is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, or the vendor layer could not
 *                          apply the change. The caller should read the setting back
 *                          with `wifi_getRadioIGMPSnoopingEnable()` rather than retrying
 *                          blindly.
 *
 * @note This interface does not state whether group memberships already learnt are
 *       discarded when snooping is disabled and relearnt when it is re-enabled.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioIGMPSnoopingEnable
 */
INT wifi_setRadioIGMPSnoopingEnable(INT radioIndex, BOOL enable);

/**
 * @brief Enables or disables Zero-wait DFS on a radio, and its pre-CAC option.
 *
 * Zero-wait DFS lets a radio move onto a DFS channel without the usual off-air
 * channel-availability check, because the check has already been performed on a
 * secondary chain. That is what makes a DFS channel change appear instantaneous to
 * associated clients instead of costing a minute or more of downtime.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`. Declared here as `UINT`, unlike the `INT`
 *                        that `wifi_setRadioDfsEnable()` takes.
 * @param[in] enable      `TRUE` to enable Zero-wait DFS, `FALSE` to disable it.
 * @param[in] precac      `TRUE` to enable pre-CAC, `FALSE` to disable it. Pre-CAC
 *                        checks DFS channels ahead of needing them.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio holds both requested settings. On failure this interface
 *       does not state whether one of the two was applied, so a caller should read them
 *       back with `wifi_getZeroDFSState()`.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS Both settings were applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, the radio or regulatory domain
 *                          does not permit the request, or the vendor layer could not
 *                          apply it. The caller should read the state back rather than
 *                          retrying with the same arguments.
 *
 * @warning `precac` is meaningful only in the EU regulatory domain. This interface does
 *          not state whether requesting it elsewhere is rejected or silently ignored, so
 *          a caller must read the state back rather than assume it took effect.
 *
 * @note Zero-wait DFS depends on the radio having a spare chain to run the check on, so
 *       it is not available on every platform. This interface exposes no capability
 *       query for it.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`. In particular, enabling
 *       Zero-wait DFS starts a channel availability check - it does not wait for one.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getZeroDFSState
 * @see wifi_setRadioDfsEnable
 */
INT wifi_setZeroDFSState(UINT radioIndex, BOOL enable, BOOL precac);

/**
 * @brief Reads a radio's Zero-wait DFS and pre-CAC settings.
 *
 * This is the read-back for `wifi_setZeroDFSState()`, and the only way to confirm that
 * a platform accepted either option.
 *
 * @param[in] radioIndex  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                        `MAX_NUM_RADIOS`.
 * @param[out] enable     Caller-allocated `BOOL` that receives `TRUE` when Zero-wait DFS
 *                        is enabled and `FALSE` when it is not. The caller allocates and
 *                        releases it; the `HAL` writes into it and retains no reference
 *                        to it after returning.
 * @param[out] precac     Caller-allocated `BOOL` that receives the pre-CAC setting, on
 *                        the same ownership terms as `enable`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to either output.
 * @post On success both outputs hold their current setting; on failure both are left
 *       unspecified, so a caller must not read either unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS Both settings were retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, either output pointer is NULL,
 *                          or the vendor layer could not supply the values. The caller
 *                          should validate its arguments and treat both settings as
 *                          unknown.
 *
 * @note `precac` is meaningful only in the EU regulatory domain; elsewhere its value
 *       carries no operational meaning.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setZeroDFSState
 */
INT wifi_getZeroDFSState(UINT radioIndex, BOOL *enable, BOOL *precac);

/* 802.11ax HAL API prototypes */

/**
 * @brief Selects how a radio multiplexes downlink transmissions to several clients.
 *
 * 802.11ax can serve several clients in one transmission, either by splitting the
 * channel in frequency (OFDMA) or by spatial streams (MU-MIMO). This chooses which,
 * and therefore which kind of client mix the radio is optimised for.
 *
 * @param[in] radio_index  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[in] mu_type      Downlink multiplexing type to apply. Must be one of the
 *                         `wifi_dl_mu_type_t` enumerators declared in
 *                         `wifi_hal_generic.h`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio uses the requested type. On failure the setting is
 *       unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The type was applied.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `mu_type` is not a declared
 *                          enumerator, the radio does not support 802.11ax, or the
 *                          vendor layer could not apply the change. The caller should
 *                          confirm the radio's variant through
 *                          `wifi_getRadioOperatingParameters()` before retrying.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getDownlinkMuType
 * @see wifi_setUplinkMuType
 */
INT wifi_setDownlinkMuType(INT radio_index, wifi_dl_mu_type_t mu_type);

/**
 * @brief Reads how a radio multiplexes downlink transmissions to several clients.
 *
 * @param[in] radio_index  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[out] mu_type     Caller-allocated `wifi_dl_mu_type_t` that receives the
 *                         configured downlink multiplexing type. The caller allocates
 *                         and releases it; the `HAL` writes into it and retains no
 *                         reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds a declared `wifi_dl_mu_type_t` enumerator; on
 *       failure it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The type was retrieved.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `mu_type` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the type as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setDownlinkMuType
 */
INT wifi_getDownlinkMuType(INT radio_index, wifi_dl_mu_type_t *mu_type);

/**
 * @brief Selects how a radio multiplexes uplink transmissions from several clients.
 *
 * Uplink multiplexing is scheduled by the Access Point, so this setting decides how the
 * radio solicits simultaneous transmissions from associated clients.
 *
 * @param[in] radio_index  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[in] mu_type      Uplink multiplexing type to apply. Must be one of the
 *                         `wifi_ul_mu_type_t` enumerators declared in
 *                         `wifi_hal_generic.h`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio uses the requested type. On failure the setting is
 *       unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The type was applied.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `mu_type` is not a declared
 *                          enumerator, the radio does not support 802.11ax, or the
 *                          vendor layer could not apply the change. The caller should
 *                          read the value back with `wifi_getUplinkMuType()` rather than
 *                          retrying with the same argument.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getUplinkMuType
 * @see wifi_setDownlinkMuType
 */
INT wifi_setUplinkMuType(INT radio_index, wifi_ul_mu_type_t mu_type);

/**
 * @brief Reads how a radio multiplexes uplink transmissions from several clients.
 *
 * @param[in] radio_index  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[out] mu_type     Caller-allocated `wifi_ul_mu_type_t` that receives the
 *                         configured uplink multiplexing type. The caller allocates and
 *                         releases it; the `HAL` writes into it and retains no reference
 *                         to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds a declared `wifi_ul_mu_type_t` enumerator; on
 *       failure it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The type was retrieved.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `mu_type` is NULL, or the
 *                          vendor layer could not supply the value. The caller should
 *                          validate its arguments and treat the type as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setUplinkMuType
 */
INT wifi_getUplinkMuType(INT radio_index, wifi_ul_mu_type_t *mu_type);

/**
 * @brief Sets the OFDM guard interval a radio transmits with.
 *
 * A shorter interval raises throughput; a longer one tolerates more delay spread.
 * `wifi_guard_interval_auto` hands the choice to the driver.
 *
 * @param[in] radio_index     Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                            `MAX_NUM_RADIOS`.
 * @param[in] guard_interval  Exactly one `wifi_guard_interval_t` value. The enumerators
 *                            are distinct bits so that a capability field can advertise
 *                            several, but this call applies a single value; the effect
 *                            of passing a bitwise combination is not specified by this
 *                            interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio transmits with the requested interval. On failure the
 *       setting is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The interval was applied.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, the radio does not support the
 *                          requested interval, or the vendor layer could not apply the
 *                          change. The caller should read the value back with
 *                          `wifi_getGuardInterval()` rather than retrying with the same
 *                          argument.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getGuardInterval
 */
INT wifi_setGuardInterval(INT radio_index, wifi_guard_interval_t guard_interval);

/**
 * @brief Reads the OFDM guard interval a radio is configured with.
 *
 * Where `wifi_guard_interval_auto` was set, this interface does not state whether the
 * read-back reports `wifi_guard_interval_auto` or the interval the driver selected, so a
 * caller should not treat a concrete value as evidence that automatic selection is off.
 *
 * @param[in] radio_index     Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                            `MAX_NUM_RADIOS`.
 * @param[out] guard_interval  Caller-allocated `wifi_guard_interval_t` that receives the
 *                             configured interval. The caller allocates and releases it;
 *                             the `HAL` writes into it and retains no reference to it
 *                             after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds a declared `wifi_guard_interval_t` value; on failure
 *       it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The interval was retrieved.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `guard_interval` is NULL, or
 *                          the vendor layer could not supply the value. The caller
 *                          should validate its arguments and treat the interval as
 *                          unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setGuardInterval
 */
INT wifi_getGuardInterval(INT radio_index, wifi_guard_interval_t *guard_interval);

/**
 * @brief Sets the BSS colour a radio advertises.
 *
 * The BSS colour is the short 802.11ax identifier that lets a receiver decide quickly
 * whether a frame belongs to its own network, which is what makes spatial reuse
 * possible. Two overlapping BSSs sharing a colour defeat that, so the value must be
 * chosen against what is already in use.
 *
 * @param[in] radio_index  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[in] color        Colour to advertise. Read the currently unused values with
 *                         `wifi_getAvailableBSSColor()` first; this interface does not
 *                         state the accepted range beyond the `UCHAR` type.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio advertises the requested colour. On failure the advertised
 *       colour is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The colour was applied.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, the vendor layer rejected
 *                          `color`, or it could not apply the change. The caller should
 *                          pick a value from `wifi_getAvailableBSSColor()` rather than
 *                          retrying with the same argument.
 *
 * @note Changing the colour is visible to associated clients, which must relearn it from
 *       the beacon.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getBSSColor
 * @see wifi_getAvailableBSSColor
 */
INT wifi_setBSSColor(INT radio_index, UCHAR color);

/**
 * @brief Reads the BSS colour a radio currently advertises.
 *
 * @param[in] radio_index  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[out] color       Caller-allocated `UCHAR` that receives the advertised colour.
 *                         The caller allocates and releases it; the `HAL` writes into it
 *                         and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the output holds the advertised colour; on failure it is left
 *       unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The colour was retrieved.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `color` is NULL, or the vendor
 *                          layer could not supply the value. The caller should validate
 *                          its arguments and treat the colour as unknown.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setBSSColor
 */
INT wifi_getBSSColor(INT radio_index, UCHAR *color);

/**
 * @brief Reads which BSS colours a radio sees as free to use.
 *
 * This is the query to make before `wifi_setBSSColor()`: it reports the colours not
 * already claimed by an overlapping BSS, so a caller can pick one that preserves
 * spatial reuse.
 *
 * @param[in] radio_index       Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                              `MAX_NUM_RADIOS`.
 * @param[in] maxNumberColors   Number of elements `colorList` can hold. The `HAL` writes
 *                              no more than this many entries.
 * @param[out] colorList        Caller-allocated array of at least `maxNumberColors`
 *                              `UCHAR` elements that receives the available colours. The
 *                              caller allocates and releases it; the `HAL` writes into it
 *                              and retains no reference to it after returning. Only the
 *                              first `*numColorReturned` elements are written.
 * @param[out] numColorReturned  Caller-allocated `INT` that receives how many entries of
 *                               `colorList` were written, which never exceeds
 *                               `maxNumberColors`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to either output.
 * @post On success `*numColorReturned` gives the number of valid entries and the rest of
 *       the array is untouched. On failure both outputs are left unspecified, so a
 *       caller must not read `*numColorReturned`.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The list was retrieved.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `maxNumberColors` is not
 *                          positive, either output pointer is NULL, or the vendor layer
 *                          could not supply the list. The caller should validate its
 *                          arguments and treat the available set as unknown rather than
 *                          empty.
 *
 * @warning `maxNumberColors` is the caller's own promise about the array it passed. If it
 *          overstates the allocation the `HAL` will write past the end of the buffer, and
 *          this interface gives the `HAL` no way to detect that.
 *
 * @note This interface does not state what happens when more colours are available than
 *       `maxNumberColors` - whether the call truncates and succeeds or fails - so a
 *       caller that needs the complete set should size the array for the full 802.11ax
 *       colour space rather than relying on either behaviour.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setBSSColor
 */
INT wifi_getAvailableBSSColor(INT radio_index, INT maxNumberColors, UCHAR* colorList, INT *numColorReturned);

/**
 * @brief Reads the MU EDCA channel-access parameters for one access category.
 *
 * MU EDCA applies a separate, usually more restrictive, set of contention parameters to
 * a client that the Access Point is scheduling, which is what stops scheduled clients
 * from also contending on their own and undoing the schedule.
 *
 * @param[in] radio_index  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[in] ac           Access category to read, as a `wifi_access_category_t`
 *                         enumerator declared in `wifi_hal_generic.h`. Parameters are
 *                         per category, so a caller wanting all of them must call once
 *                         per category.
 * @param[out] edca        Caller-allocated `wifi_edca_t` that receives the AIFS number,
 *                         the minimum and maximum contention window and the timer. The
 *                         caller allocates and releases it; the `HAL` writes into it and
 *                         retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success every member of `edca` holds the value in force for `ac`; on failure
 *       the structure is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The parameters were retrieved.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `ac` is not a declared
 *                          enumerator, `edca` is NULL, or the vendor layer could not
 *                          supply the values. The caller should validate its arguments
 *                          and treat the parameters as unknown.
 *
 * @note This interface exposes no setter for MU EDCA, so these parameters are read-only
 *       through this header.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_getMuEdca(INT radio_index, wifi_access_category_t ac, wifi_edca_t *edca);

/**
 * @brief Selects how a radio requires downlink data to be acknowledged.
 *
 * Immediate acknowledgement keeps latency low; deferred block acknowledgement lets a
 * receiver acknowledge a whole burst at once and costs less airtime.
 *
 * @param[in] radio_index  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[in] ack_type     One of the `wifi_dl_data_ack_type_t` enumerators declared in
 *                         this header.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio requires the requested acknowledgement type. On failure the
 *       setting is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The type was applied.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `ack_type` is not a declared
 *                          enumerator, or the vendor layer could not apply the change.
 *                          The caller should pass a declared enumerator and treat a
 *                          repeated failure as unsupported.
 *
 * @note This interface exposes no getter for this setting, so a caller cannot read back
 *       what it applied.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_setDownlinkDataAckType(INT radio_index, wifi_dl_data_ack_type_t ack_type);

/**
 * @brief Reads a radio's default 802.11ax parameter set.
 *
 * These are the values the radio starts from, which gives a caller a known baseline to
 * restore after experimenting with the individual 802.11ax setters above.
 *
 * @param[in] radio_index  Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                         `MAX_NUM_RADIOS`.
 * @param[out] params      Caller-allocated `wifi_80211ax_params_t` that receives the
 *                         default parameter set. The caller allocates and releases it;
 *                         the `HAL` writes into it and retains no reference to it after
 *                         returning. The structure is declared in `wifi_hal_generic.h`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the structure holds the defaults; on failure it is left unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The defaults were retrieved.
 * @retval WIFI_HAL_ERROR   `radio_index` is out of range, `params` is NULL, the radio
 *                          does not support 802.11ax, or the vendor layer could not
 *                          supply the values. The caller should validate its arguments
 *                          and treat the defaults as unavailable.
 *
 * @note These are defaults, not the values in force. Read the current settings with the
 *       corresponding getters above.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_get80211axDefaultParameters(INT radio_index, wifi_80211ax_params_t *params);

/////////////////////////// tri radio definitions /////////////////////////////////

/**
 * @brief Applies a radio's complete operating configuration in one call.
 *
 * This is the primary way to configure a radio: channel, bandwidth, secondary channels,
 * 802.11 variant, country code, regulatory domain, DTIM and beacon timing, rates,
 * thresholds and the operating-class list all travel together. Applying them as one set
 * avoids the intermediate states a sequence of individual setters would pass through,
 * which is why the sequence diagram in `docs/pages/halSpec.md` uses this call to bring a
 * radio up.
 *
 * @param[in] index            Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                             `MAX_NUM_RADIOS`.
 * @param[in] operationParam   Caller-allocated, caller-owned
 *                             `wifi_radio_operationParam_t` holding the configuration to
 *                             apply. The `HAL` reads it and retains no reference to it
 *                             after returning, so a caller may pass a stack object
 *                             safely. Every member is significant, so a caller changing
 *                             one field should read the current set with
 *                             `wifi_getRadioOperatingParameters()` and modify that rather
 *                             than passing a partly populated structure. `channelSecondary`
 *                             holds `MAXNUMSECONDARYCHANNELS` entries and
 *                             `numSecondaryChannels` how many are valid;
 *                             `operatingClasses` holds `MAXNUMOPERCLASSESPERBAND` entries
 *                             and `numOperatingClasses` how many are valid.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and changes nothing.
 * @post On success the radio operates with the supplied parameters. On failure this
 *       interface does not state whether part of the set was applied, so a caller should
 *       read the parameters back rather than assume the radio is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was applied.
 * @retval WIFI_HAL_ERROR   `index` is out of range, `operationParam` is NULL, the
 *                          requested combination is not permitted by the radio or the
 *                          regulatory domain, or the vendor layer could not apply it. The
 *                          caller should read the parameters back to establish the
 *                          resulting state before retrying, rather than resending the
 *                          same structure.
 *
 * @warning Changing the channel takes the radio off the air briefly and disassociates
 *          clients that do not follow the channel switch, so this is not a call to make
 *          on a live radio without cause.
 *
 * @note Moving to a DFS channel is subject to `wifi_setRadioDfsEnable()` and, where
 *       supported, `wifi_setZeroDFSState()`. This call does not wait for a channel
 *       availability check to finish; poll `wifi_getRadioStatus()` for that.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioOperatingParameters
 * @see wifi_applyRadioSettings
 */
INT wifi_setRadioOperatingParameters(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam);

/**
 * @brief Reads a radio's complete operating configuration.
 *
 * This is the read-back for `wifi_setRadioOperatingParameters()` and the recommended
 * starting point for any change: read the set, modify the members that matter, apply the
 * whole set.
 *
 * @param[in] index            Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                             `MAX_NUM_RADIOS`.
 * @param[out] operationParam  Caller-allocated `wifi_radio_operationParam_t` that
 *                             receives the configuration. The caller allocates and
 *                             releases it; the `HAL` writes into it and retains no
 *                             reference to it after returning. Read `numSecondaryChannels`
 *                             and `numOperatingClasses` to learn how many entries of
 *                             `channelSecondary` and `operatingClasses` are valid.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the structure holds the configuration in force; on failure it is left
 *       unspecified, so a caller must not use it as the basis for a subsequent set.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was retrieved.
 * @retval WIFI_HAL_ERROR   `index` is out of range, `operationParam` is NULL, or the
 *                          vendor layer could not supply the values. The caller should
 *                          validate its arguments and must not apply a structure it
 *                          failed to read.
 *
 * @note The structure reports what the radio is doing, which can differ from what was
 *       requested - for example where `wifi_setRadioObssCoexistenceEnable()` has narrowed
 *       the channel width.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioOperatingParameters
 */
INT wifi_getRadioOperatingParameters(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam);

/**
 * @brief Reads the BSSs a radio found on a channel.
 *
 * This is how a caller learns what else is on the air - the input to channel selection
 * and to neighbour reporting. The results are returned through a `HAL`-allocated array,
 * so the ownership rule below is an exception to the general one in `Memory Model` in
 * `docs/pages/halSpec.md`, which that topic expressly permits an API to state.
 *
 * @param[in] index     Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                      `MAX_NUM_RADIOS`.
 * @param[in] channel   Caller-allocated, caller-owned `wifi_channel_t` naming the channel
 *                      number and band to report on. The `HAL` reads it and retains no
 *                      reference to it after returning.
 * @param[out] bss      Pointer to a pointer to an array of `wifi_bss_info_t` structures.
 *                      The array is allocated by the HAL layer and should be freed by the
 *                      caller. The caller passes the address of its own pointer variable,
 *                      which the `HAL` sets to the allocated array.
 * @param[out] num_bss  Caller-allocated `UINT` that receives the number of elements in
 *                      the array `*bss` points at.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and allocates nothing.
 * @post On success `*bss` points at an array of `*num_bss` elements that the caller must
 *       free. On failure this interface does not state whether `*bss` was written, so a
 *       caller must initialise its pointer to NULL beforehand and free it only when it is
 *       non-NULL.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The results were retrieved and the array was allocated.
 * @retval WIFI_HAL_ERROR   `index` is out of range, any pointer argument is NULL, the
 *                          allocation failed, or the vendor layer could not supply the
 *                          results. The caller should validate its arguments and treat
 *                          the scan as having produced nothing; it must not read
 *                          `*num_bss`.
 *
 * @warning The caller owns the array after a successful call and leaks it if it does not
 *          free it. A repeated caller must free the previous array before the next call
 *          overwrites its pointer.
 *
 * @note A result of zero BSSs is a successful outcome, not an error. This interface does
 *       not state whether `*bss` is allocated at all in that case, so a caller should
 *       free it only when it is non-NULL.
 * @note This call reports results that are already available; it does not itself wait for
 *       a scan to complete, and must not suspend or invoke any blocking system calls -
 *       see `Blocking calls` in `docs/pages/halSpec.md`. For notification when fresh
 *       results arrive, register a handler with `wifi_scanResults_callback_register()`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_scanResults_callback_register
 * @see wifi_scanResults_callback
 */
INT wifi_getScanResults(wifi_radio_index_t index, wifi_channel_t *channel, wifi_bss_info_t **bss, UINT *num_bss);

/**
 * @brief Handler type the `HAL` invokes when fresh scan results become available.
 *
 * A caller implements a function of this type and installs it with
 * `wifi_scanResults_callback_register()`. Being a callback, the direction of the return
 * value is inverted relative to the rest of this header: the value below is what the
 * IMPLEMENTER returns TO the `HAL`, not something a caller receives.
 *
 * @param[in] index     Index of the Wi-Fi radio the results belong to, in the range
 *                      `RADIO_INDEX_1` to `MAX_NUM_RADIOS`.
 * @param[out] bss      Pointer to a pointer to an array of `wifi_bss_info_t` structures.
 *                      The array is allocated by the HAL layer, in the same shape
 *                      `wifi_getScanResults()` uses. Per `Asynchronous Notification Model`
 *                      in `docs/pages/halSpec.md` the handler must copy anything it needs
 *                      to keep rather than retain the pointer beyond the call.
 * @param[out] num_bss  Pointer to the number of elements in the array `*bss` points at.
 *
 * @returns The handler's own outcome, reported back to the `HAL`.
 * @retval WIFI_HAL_SUCCESS  The handler accepted and processed the results.
 *
 * @note This interface does not specify how the `HAL` interprets any other returned
 *       value, so an implementer should return `WIFI_HAL_SUCCESS` on successful handling
 *       and must not rely on a non-success return causing the `HAL` to retry, re-deliver
 *       or log. Report handling failures through the implementer's own logging instead.
 * @note The handler must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. It should hand the results to its
 *       own worker and return promptly.
 * @note `Threading Model` in `docs/pages/halSpec.md` states that the `HAL` is expected to
 *       be thread safe, but it does not state whether the `HAL` may invoke this handler
 *       from more than one thread at a time. An implementer should therefore make the
 *       handler thread safe rather than assume serialised delivery.
 *
 * @see wifi_scanResults_callback_register
 * @see wifi_getScanResults
 */
typedef INT ( * wifi_scanResults_callback)(wifi_radio_index_t index, wifi_bss_info_t **bss, UINT *num_bss);

/**
 * @brief Installs the caller's handler for scan-result notifications.
 *
 * After registration the `HAL` reports each set of fresh scan results through the
 * supplied handler, which spares a caller from polling `wifi_getScanResults()`. The most
 * recently registered handler replaces any previous one. This is one of the asynchronous
 * registration functions listed under `Asynchronous Notification Model` in
 * `docs/pages/halSpec.md`.
 *
 * @param[in] callback_proc  Handler to install, of type `wifi_scanResults_callback`. The
 *                           `HAL` retains this function pointer and invokes it until it
 *                           is replaced, so the function must remain callable for that
 *                           whole period. The effect of passing NULL is not specified by
 *                           this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. The effect of registering beforehand is not specified
 *      by this interface.
 * @post The handler is installed and is invoked on each subsequent set of scan results.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_scanResults_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The scan-result array reaching the handler is allocated by the `HAL`. The
 *          client must copy what it needs during the call rather than retain the pointer,
 *          per `Asynchronous Notification Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_scanResults_callback
 * @see wifi_getScanResults
 */
void wifi_scanResults_callback_register(wifi_scanResults_callback callback_proc);

/**
 * @brief Reads a radio chipset's current temperature.
 *
 * The reading lets a caller detect thermal stress before it degrades throughput or
 * triggers a vendor-side protective action.
 *
 * @param[in] radioIndex      Index of the Wi-Fi radio, in the range `RADIO_INDEX_1` to
 *                            `MAX_NUM_RADIOS`.
 * @param[out] output_struct   Caller-allocated `wifi_radioTemperature_t` whose
 *                             `radio_Temperature` member receives the chipset
 *                             temperature. The caller allocates and releases it; the
 *                             `HAL` writes into it and retains no reference to it after
 *                             returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      and writes nothing to the output.
 * @post On success the structure holds a temperature reading; on failure it is left
 *       unspecified, so a caller must not read it as zero degrees.
 *
 * @returns The status of the operation. The temperature itself is returned through
 *          `output_struct`, not through the return value.
 * @retval WIFI_HAL_SUCCESS The temperature was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` is out of range, `output_struct` is NULL, the
 *                          platform exposes no temperature sensor for this radio, or the
 *                          vendor layer could not read it. The caller should validate its
 *                          arguments and treat the temperature as unavailable rather than
 *                          as a safe value.
 *
 * @note The member is unsigned and this interface states no unit or scale for it, so a
 *       caller should not assume degrees Celsius, and should compare successive readings
 *       rather than test one against an absolute threshold.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_hal_getRadioTemperature(wifi_radio_index_t radioIndex, wifi_radioTemperature_t *output_struct);


/** @} */  //END OF GROUP WIFI_HAL_APIS

#ifdef __cplusplus
}
#endif

#endif
