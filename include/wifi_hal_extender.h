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

#ifndef __WIFI_HAL_EXTENDER_H__
#define __WIFI_HAL_EXTENDER_H__

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

/**
 * @brief Wi-Fi channel statistics.
 *
 * One instance describes the survey result for a single 20MHz channel. The structure is
 * both an input and an output of `wifi_getRadioChannelStats()`: the caller sets
 * `ch_number` to name the channel it wants, and the `HAL` writes every remaining member.
 * The caller allocates and owns the array, per `Memory Model` in `docs/pages/halSpec.md`.
 *
 * The six `ch_utilization_*` members are busy-time accumulators rather than percentages.
 * A caller derives a proportion by dividing one of them by `ch_utilization_total` over the
 * same sampling interval, or by differencing two successive reads of the same member.
 *
 * @see wifi_getRadioChannelStats
 */
typedef struct _wifi_channelStats
{
    INT ch_number; /*!< IEEE 802.11 channel number, always a 20MHz channel. Set by the
                        caller to select the channel and echoed by the `HAL`. This
                        interface does not carry the band in this structure, so the
                        channel number alone must identify the channel for the radio
                        being queried. */
    BOOL ch_in_pool; /*!< Whether the channel is in the radio's scan pool, as a `BOOL`
                        (`unsigned char`, `TRUE` or `FALSE`). When `FALSE` the driver
                        does not need to scan this channel, so a caller should not read
                        the remaining members as a fresh survey of it. */
    INT ch_noise; /*!< Average noise floor on the channel, in dBm. Normally negative. */
    BOOL ch_radar_noise; /*!< Whether radar was detected on the channel. Meaningful on
                        5GHz only; this interface does not state what the member carries
                        for a 2.4GHz or 6GHz channel. */
    INT ch_max_80211_rssi; /*!< Strongest RSSI observed from a neighbouring AP on this
                        channel, in dBm. Normally negative. */
    INT ch_non_80211_noise; /*!< Average noise attributed to non-802.11 sources. This
                        interface does not state the unit of this member; the two noise
                        and RSSI members above it are expressed in dBm. */
    INT ch_utilization; /*!< 802.11 utilization of the channel, as a percentage in the
                        range 0 to 100. */
    ULLONG ch_utilization_total; /*!< Accumulated time the radio was able to observe the
                        channel, which is the denominator for the five busy accumulators
                        below. This interface does not state the time unit or the epoch
                        the accumulator counts from, so a caller must use it only as a
                        ratio or a difference, never as an absolute duration. */
    ULLONG ch_utilization_busy; /*!< Accumulated time the channel was busy, where busy is
                        Rx + Tx + interference. Same unit and epoch as
                        `ch_utilization_total`. */
    ULLONG ch_utilization_busy_tx; /*!< Accumulated time the radio spent transmitting on
                        the channel. A component of `ch_utilization_busy`. */
    ULLONG ch_utilization_busy_rx; /*!< Accumulated time the radio spent receiving on the
                        channel, where Rx is Rx_obss + Rx_self + Rx_err, the last covering
                        both self and OBSS errors. A component of
                        `ch_utilization_busy`. */
    ULLONG ch_utilization_busy_self; /*!< Accumulated time the radio spent receiving from
                        its own connected clients. A component of
                        `ch_utilization_busy_rx`. */
    ULLONG ch_utilization_busy_ext; /*!< Accumulated time the 40MHz extension channel was
                        busy. Reported for the extension channel, not for the 20MHz
                        channel named by `ch_number`, so it is meaningful only where the
                        radio is operating at 40MHz or wider. */
} wifi_channelStats_t;

/* MCS/NSS/BW rate table and indexes that should be used for supported rates
   ----------------------------------------------
   | type | bw         | nss        |  mcs
   ----------------------------------------------
   | OFDM | 0 (20Mhz)  | 0 (legacy) |  0 - 6M
   |      |            |            |  1 - 9M
   |      |            |            |  2 - 12M
   |      |            |            |  3 - 18M
   |      |            |            |  4 - 24M
   |      |            |            |  5 - 36M
   |      |            |            |  6 - 48M
   |      |            |            |  7 - 54M
   ----------------------------------------------
   | CCK  | 0 (20Mhz)  | 0 (legacy) |  8 - L1M
   |      |            |            |  9 - L2M
   |      |            |            | 10 - L5.5M
   |      |            |            | 11 - L11M
   |      |            |            | 12 - S2M
   |      |            |            | 13 - S5.5M
   |      |            |            | 14 - S11M"
   ----------------------------------------------
   | VHT  | 0 (20Mhz)  | 1 (chain1) |  1 - HT/VHT
   |      | 1 (40Mhz)  | ...        |  2 - HT/VHT
   |      | 2 (80MHz)  | 8 (chain8) |  3 - HT/VHT
   |      | 2 (160MHz) |            |  4 - HT/VHT
   |      |            |            |  5 - HT/VHT
   |      |            |            |  6 - HT/VHT
   |      |            |            |  7 - HT/VHT
   |      |            |            |  8 - VHT
   |      |            |            |  9 - VHT
   ----------------------------------------------
   NOTE: The size of this table on 4x4 can be big - we could send only non-zero elements!
*/

/**
 * @brief RX statistics for a specific rate.
 *
 * One instance carries the receive counters for a single rate, identified by the
 * `nss`, `mcs` and `bw` triple that the rate table above indexes.
 * `wifi_getApAssociatedDeviceRxStatsResult()` returns an array of these, one element per
 * rate the client has used.
 *
 * @see wifi_getApAssociatedDeviceRxStatsResult
 */
typedef struct _wifi_associated_dev_rate_info_rx_stats
{
    UCHAR nss; /*!< Number of spatial streams: 0 for the legacy OFDM and CCK rates, 1 or
                        more for HT and VHT. Together with `mcs` and `bw` this identifies
                        the rate the counters below belong to, as laid out in the rate
                        table above this structure. */
    UCHAR mcs; /*!< MCS index for the rate: 0 to 7 for HT, 0 to 9 for VHT, and for the
                        legacy rates the table index above rather than an MCS - 0 to 7 for
                        OFDM and 8 to 14 for CCK. */
    USHORT bw; /*!< Bandwidth of the rate in MHz - 20, 40, 80 or 160. Note that this
                        member is a count of MHz, unlike the `bw` column of the rate table
                        above, which is an index. */
    ULLONG flags; /*!< Bitmask stating which of the members below the `HAL` actually
                        populated, so a caller can tell an unpopulated counter from a
                        genuine zero. This interface names the flags HAS_BYTES, HAS_MSDUS,
                        HAS_MPDUS, HAS_PPDUS, HAS_BW_80P80, HAS_RSSI_COMB and
                        HAS_RSSI_ARRAY but does not define their values anywhere, so a
                        caller cannot test them portably from this header alone and the
                        bit positions are a platform convention. */
    ULLONG bytes; /*!< Bytes received at this rate. A monotonically increasing counter;
                        valid only if the HAS_BYTES flag is set. */
    ULLONG msdus; /*!< MSDUs received at this rate. Valid only if the HAS_MSDUS flag is
                        set. */
    ULLONG mpdus; /*!< MPDUs received at this rate. Valid only if the HAS_MPDUS flag is
                        set. */
    ULLONG ppdus; /*!< PPDUs received at this rate. Valid only if the HAS_PPDUS flag is
                        set. */
    ULLONG retries; /*!< Frames received at this rate that were marked as retries. `flags` above is
                         what says which counters of this structure are valid, and it names no flag
                         for this member, so this interface does not establish when it may be read;
                         the counter is a subset of the frames counted elsewhere for the same rate
                         rather than an addition to them. */
    UCHAR rssi_combined; /*!< Most recent RSSI across all chains for this rate, valid only
                        if the HAS_RSSI_COMB flag is set. Carried as an unsigned octet
                        above the noise floor rather than as a signed dBm value, on the
                        same basis as `rssi_array` below. */
    /* Per antenna RSSI (above noise floor) for all widths (primary,secondary) 
        -----------------------------------------------
        | chain_num |  20MHz [pri20                   ]
        |           |  40MHZ [pri20,sec20             ] 
        |           |  80MHz [pri20,sec20,sec40,      ]
        |           | 160MHz [pri20,sec20,sec40,sec80 ]
        -----------------------------------------------
        |  1        |  rssi  [pri20,sec20,sec40,sec80 ]
        |  ...      |  ...
        |  8        |  rssi  [pri20,sec20,sec40,sec80 ]
        -----------------------------------------------    */    
    UCHAR rssi_array[8][4]; /*!< Per-chain RSSI above the noise floor, valid only if the
                        HAS_RSSI_ARRAY flag is set. The first index is the chain, 0 to 7
                        for up to eight chains, and the second is the 20MHz sub-band
                        within the operating bandwidth in the order primary, secondary,
                        secondary-40 and secondary-80. Only the leading sub-bands are
                        meaningful: one entry at 20MHz, two at 40MHz, three at 80MHz and
                        four at 160MHz, so `bw` bounds how much of each row a caller may
                        read, and the number of chains the radio has bounds how many rows
                        it may read. The comment block above this structure draws the same
                        layout. */
} wifi_associated_dev_rate_info_rx_stats_t;

/**
 * @brief TX statistics for a specific rate.
 *
 * One instance carries the transmit counters for a single rate, identified by the
 * `nss`, `mcs` and `bw` triple that the rate table above indexes.
 * `wifi_getApAssociatedDeviceTxStatsResult()` returns an array of these, one element per
 * rate the client has used. The receive counterpart is
 * `wifi_associated_dev_rate_info_rx_stats_t`, which additionally carries RSSI.
 *
 * @see wifi_getApAssociatedDeviceTxStatsResult
 */
typedef struct _wifi_associated_dev_rate_info_tx_stats
{
    UCHAR nss; /*!< Number of spatial streams: 0 for the legacy OFDM and CCK rates, 1 or
                        more for HT and VHT. Together with `mcs` and `bw` this identifies
                        the rate the counters below belong to, as laid out in the rate
                        table above `wifi_associated_dev_rate_info_rx_stats_t`. */
    UCHAR mcs; /*!< MCS index for the rate: 0 to 7 for HT, 0 to 9 for VHT, and for the
                        legacy rates the table index rather than an MCS. */
    USHORT bw; /*!< Bandwidth of the rate in MHz - 20, 40, 80 or 160. */
    ULLONG flags; /*!< Bitmask stating which of the members below the `HAL` actually
                        populated, so a caller can tell an unpopulated counter from a
                        genuine zero. The flag names are those listed for
                        `wifi_associated_dev_rate_info_rx_stats_t`, and this interface
                        does not define their values, so the bit positions are a platform
                        convention. The RSSI flags do not apply here, because this
                        structure carries no RSSI member. */
    ULLONG bytes; /*!< Bytes transmitted at this rate. A monotonically increasing counter;
                        valid only if the HAS_BYTES flag is set. */
    ULLONG msdus; /*!< MSDUs transmitted at this rate. Valid only if the HAS_MSDUS flag is
                        set. */
    ULLONG mpdus; /*!< MPDUs transmitted at this rate. Valid only if the HAS_MPDUS flag is
                        set. */
    ULLONG ppdus; /*!< PPDUs transmitted at this rate. Valid only if the HAS_PPDUS flag is
                        set. */
    ULLONG retries; /*!< Retransmissions at this rate. A subset of `attempts`. */
    ULLONG attempts; /*!< Transmit attempts at this rate, counting the first attempt and
                        every retry, so `attempts` is at least `retries` and the
                        difference is the number of frames first attempted at this
                        rate. */
} wifi_associated_dev_rate_info_tx_stats_t;

/**
 * @brief TID entry.
 *
 * One instance carries the transmit-time statistics for a single traffic identifier over
 * the reporting interval. Sixteen of these make up `wifi_associated_dev_tid_stats_t`.
 *
 * @see wifi_associated_dev_tid_stats_t
 */
typedef struct wifi_associated_dev_tid_entry
{
    UCHAR ac; /*!< IEEE 802.11e access category the TID maps into - background, best
                        effort, video or voice. This interface does not define the
                        numeric encoding of this member. */
    UCHAR tid; /*!< Traffic identifier this entry describes, in the range 0 to 15. In
                        `wifi_associated_dev_tid_stats_t` the entry's array index carries
                        the same value. */
    ULLONG ewma_time_ms; /*!< Exponentially weighted moving average of MSDU transmit
                        latency for this TID, in milliseconds, taken over the most recent
                        MSDUs. This interface does not state the averaging window or the
                        weighting used. */
    ULLONG sum_time_ms; /*!< Total MSDU transmit time accumulated for this TID over the
                        reporting interval, in milliseconds. Dividing it by `num_msdus`
                        gives the mean latency for the interval. */
    ULLONG num_msdus; /*!< MSDUs transmitted for this TID over the reporting interval, and
                        the divisor for `sum_time_ms`. A value of zero means no traffic,
                        in which case `sum_time_ms` carries no useful mean. */
} wifi_associated_dev_tid_entry_t;

/**
 * @brief TID statistics.
 *
 * The `HAL` fills an instance of this structure in
 * `wifi_getApAssociatedDeviceTidStatsResult()`, where the caller allocates the storage
 * and keeps ownership of it.
 *
 * @see wifi_getApAssociatedDeviceTidStatsResult
 */
typedef struct wifi_associated_dev_tid_stats
{
    wifi_associated_dev_tid_entry_t tid_array[16]; /*!< Statistics for all sixteen traffic
                        identifiers. The array is fixed at sixteen elements and is indexed
                        by TID, so element `n` describes TID `n` and every element is
                        addressable; there is no separate count member and a caller must
                        not expect one. An element for a TID that carried no traffic is
                        reported with a zero `num_msdus`. */
} wifi_associated_dev_tid_stats_t;

/**
 * @brief Associated device statistics.
 *
 * Aggregate traffic counters for one associated client, together with the two most recent
 * RSSI snapshots. Unlike the per-rate statistics structures, this one is not broken down
 * by rate. The `HAL` fills an instance in `wifi_getApAssociatedDeviceStats()`, where the
 * caller allocates the storage and keeps ownership of it.
 *
 * All eight counters accumulate from an epoch this interface does not state, so a caller
 * should difference two successive reads rather than treat a single read as a total for a
 * period of its own choosing.
 *
 * @see wifi_getApAssociatedDeviceStats
 */
typedef struct _wifi_associated_dev_stats
{
    ULLONG cli_rx_bytes; /*!< Bytes received from the client, including framing
                        characters. */
    ULLONG cli_tx_bytes; /*!< Bytes transmitted to the client, including framing
                        characters. */
    ULLONG cli_rx_frames; /*!< Frames received from the client. `cli_rx_retries` below is the subset
                               of them marked as retries; this interface states neither the interval
                               the counter covers nor when it resets, so a caller should difference
                               successive reads. */
    ULLONG cli_tx_frames; /*!< Frames transmitted to the client, counting first attempts.
                               `cli_tx_retries` below counts retransmissions in addition to these,
                               so the two must not be added as though they partitioned one total. */
    ULLONG cli_rx_retries; /*!< Frames received from the client that were marked as
                        retries. A subset of `cli_rx_frames`. */
    ULLONG cli_tx_retries; /*!< Frames retransmitted to the client. Counted in addition to
                        the first attempt, so it is not bounded by `cli_tx_frames`. */
    ULLONG cli_rx_errors; /*!< Receive errors on frames from the client, for example a
                        failed FCS check. */
    ULLONG cli_tx_errors; /*!< Transmit errors on frames to the client, for example a
                        frame discarded after exhausting its retry limit. */
    double cli_rx_rate; /*!< Mean receive data rate for the client. This interface does
                        not state the unit of this member or the interval it is averaged
                        over; comparable rate members elsewhere in the interface are
                        expressed in Mbps. */
    double cli_tx_rate; /*!< Mean transmit data rate for the client, on the same terms as
                        `cli_rx_rate`. */
    wifi_rssi_snapshot_t cli_rssi_bcn; /*!< RSSI of the last four beacons received, which
                        is the snapshot a client `VAP` (STA role) can report. Each of the
                        four entries in `wifi_rssi_snapshot_t` - declared in
                        `wifi_hal_telemetry.h` - carries its own relative age in seconds,
                        and its `count` member is the frame sequence number rather than a
                        count of valid entries. */
    wifi_rssi_snapshot_t cli_rssi_ack; /*!< RSSI of the last four ACKs received, which is
                        the snapshot an AP `VAP` can report, on the same terms as
                        `cli_rssi_bcn`. */
} wifi_associated_dev_stats_t;

/**
 * @brief Steering configuration per AP.
 *
 * This structure defines the configuration for each Access Point (AP) added
 * to a steering group.
 *
 * Channel utilization is sampled every `utilCheckIntervalSec` seconds, and
 * after collecting `utilAvgCount` samples, the
 * `WIFI_STEERING_EVENT_CHAN_UTILIZATION` event is sent with the averaged
 * value.
 *
 * Client activity checking is performed every `inactCheckIntervalSec`
 * seconds. If a client is idle for `inactCheckThresholdSec` seconds, it is
 * marked as inactive. The `WIFI_STEERING_EVENT_CLIENT_ACTIVITY` event is
 * sent whenever a client changes state between active and inactive.
 */
typedef struct
{
    INT apIndex; /*!< Index of the AP this entry configures. The indices this interface
                        defines are `AP_INDEX_1` to `AP_INDEX_24` in
                        `wifi_hal_generic.h`. */
    UINT utilCheckIntervalSec; /*!< Channel utilization sampling period, in seconds. */
    UINT utilAvgCount; /*!< Number of samples averaged before a
                        `WIFI_STEERING_EVENT_CHAN_UTILIZATION` event is sent, so an event
                        is reported roughly every `utilCheckIntervalSec * utilAvgCount`
                        seconds. */
    UINT inactCheckIntervalSec; /*!< Client activity check period, in seconds. */
    UINT inactCheckThresholdSec; /*!< Idle time after which a client is marked inactive,
                        in seconds. Crossing it in either direction sends a
                        `WIFI_STEERING_EVENT_CLIENT_ACTIVITY` event. */
} wifi_steering_apConfig_t;

/**
 * @brief Configuration per Client
 *
 * This defines the per-client, per-apIndex configuration settings. The
 * high water mark + low water mark pairs define RSSI ranges, in which
 * given packet types (probe or auth) are responded to as long as the RSSI
 * of the request packet is within the defined range.
 *
 * The RSSI crossings define thresholds which result in steering events
 * being generated when a connected client's RSSI crosses above or below
 * the given threshold.
 *
 * `authRejectReason`, when non-zero, results in auth requests being
 * rejected with the given reason code. When set to zero, auth requests
 * that do not fall in the RSSI hwm+lwm range will be silently ignored.
 *
 * @see https://supportforums.cisco.com/document/141136/80211-association-status-80211-deauth-reason-codes
 */
typedef struct
{
    UINT rssiProbeHWM;     /*!< Upper bound of the RSSI range in which probe requests are
                                answered. A probe request whose RSSI is outside the
                                `rssiProbeLWM` to `rssiProbeHWM` range is not responded
                                to. */
    UINT rssiProbeLWM;     /*!< Lower bound of the range `rssiProbeHWM` above closes: a probe
                                request whose RSSI falls below it is not responded to. This
                                interface states no unit for the value, and carries it as a `UINT`,
                                so a caller must not pass a negative dBm figure here. */
    UINT rssiAuthHWM;      /*!< Upper bound of the RSSI range in which auth requests are
                                answered. An auth request outside the `rssiAuthLWM` to
                                `rssiAuthHWM` range is rejected with `authRejectReason`,
                                or silently ignored when that member is zero. */
    UINT rssiAuthLWM;      /*!< Lower bound of the range `rssiAuthHWM` above closes: an auth request
                                below it is rejected with `authRejectReason`, or silently ignored
                                where that member is zero. Carried as a `UINT` with no unit stated,
                                so a negative dBm figure must not be passed. */
    UINT rssiInactXing;    /*!< RSSI threshold whose crossing marks the inactivity
                                transition in a `WIFI_STEERING_EVENT_RSSI_XING` event,
                                reported in the event's `inactveXing` member. */
    UINT rssiHighXing;     /*!< RSSI threshold whose crossing is reported in the event's
                                `highXing` member. */
    UINT rssiLowXing;      /*!< RSSI threshold whose crossing is reported in the event's
                                `lowXing` member. */
    UINT authRejectReason; /*!< IEEE 802.11 status code returned when an auth request is
                                rejected for falling outside the `rssiAuthLWM` to
                                `rssiAuthHWM` range. Zero disables rejection, so such a
                                request is silently ignored instead; see the reason-code
                                reference in the block above. */
} wifi_steering_clientConfig_t;

/**
 * @brief Wifi Disconnect Sources
 *
 * These are the possible sources of a wifi disconnect.
 * If the disconnect was initiated by the client, then `DISCONNECT_SOURCE_REMOTE`
 * should be used.
 * If initiated by the local AP, then `DISCONNECT_SOURCE_LOCAL` should be used.
 * If this information is not available, then `DISCONNECT_SOURCE_UNKNOWN` should be used.
 */
typedef enum
{
    DISCONNECT_SOURCE_UNKNOWN = 0, /**< Unknown source. */
    DISCONNECT_SOURCE_LOCAL,       /**< Initiated locally. */
    DISCONNECT_SOURCE_REMOTE       /**< Initiated remotely. */
} wifi_disconnectSource_t;

/**
 * @brief Wifi Disconnect Types
 *
 * These are the types of wifi disconnects.
 */
typedef enum
{
    DISCONNECT_TYPE_UNKNOWN = 0, /**< Unknown type. */
    DISCONNECT_TYPE_DISASSOC,   /**< Disassociation. */
    DISCONNECT_TYPE_DEAUTH,     /**< Deauthentication. */
    DISCONNECT_TYPE_RECONNECT   /**< Reconnection. */
} wifi_disconnectType_t;

/**
 * @brief Wifi Steering Event Types
 *
 * These are the different steering event types that are sent by the wifi_hal
 * steering library.
 */
typedef enum
{
    WIFI_STEERING_EVENT_PROBE_REQ = 1,    /**< Probe Request Event. */
    WIFI_STEERING_EVENT_CLIENT_CONNECT,     /**< Client association completed successfully Event. */
    WIFI_STEERING_EVENT_CLIENT_DISCONNECT,  /**< Client Disconnect Event. */
    WIFI_STEERING_EVENT_CLIENT_ACTIVITY,    /**< Client Active Change Event. */
    WIFI_STEERING_EVENT_CHAN_UTILIZATION,   /**< Channel Utilization Event. */
    WIFI_STEERING_EVENT_RSSI_XING,          /**< Client RSSI Crossing Event. */
    WIFI_STEERING_EVENT_RSSI,               /**< Instant Measurement Event. */
    WIFI_STEERING_EVENT_AUTH_FAIL           /**< Client Auth Failure Event. */
} wifi_steering_eventType_t;

/**
 * @brief RSSI Crossing Values
 *
 * These are the RSSI crossing values provided in RSSI crossing events.
 */
typedef enum
{
    WIFI_STEERING_RSSI_UNCHANGED = 0, /**< RSSI hasn't crossed. */
    WIFI_STEERING_RSSI_HIGHER,       /**< RSSI went higher. */
    WIFI_STEERING_RSSI_LOWER         /**< RSSI went lower. */
} wifi_steering_rssiChange_t;

/**
 * @brief STA datarate information
 *
 * These are STA capabilities values.
 */
typedef struct
{
    UINT maxChwidth;        /*!< Widest channel bandwidth the STA supports. This interface
                                 does not state whether the member is a count of MHz or a
                                 bandwidth code, so a caller must not compare it against
                                 either without knowing the platform convention. */
    UINT maxStreams;        /*!< Highest number of spatial streams the STA supports, as a
                                 count. */
    UINT phyMode;           /*!< PHY mode the STA supports. This interface does not define
                                 the encoding of this member. */
    UINT maxMCS;            /*!< Highest MCS index the STA supports. This interface states no
                                 accepted range and does not say which 802.11 generation numbers the
                                 index, so a caller must not compare it against an index taken from
                                 a different generation; `maxStreams` above is the separate
                                 spatial-stream count. */
    UINT maxTxpower;        /*!< Highest transmit power the STA supports. This interface
                                 does not state the unit of this member. */
    UINT isStaticSmps;      /*!< Non-zero if the STA is operating in static spatial
                                 multiplexing power save mode, which restricts it to a
                                 single receive chain. Carried as a `UINT` rather than a
                                 `BOOL`, so a caller should test for non-zero rather than
                                 compare against `TRUE`. */
    UINT isMUMimoSupported; /*!< Non-zero if the STA supports MU-MIMO, on the same terms as
                                 `isStaticSmps`. */
} wifi_steering_datarateInfo_t;

/**
 * @brief Radio Resource Management (RRM) capabilities.
 */
typedef struct
{
    BOOL linkMeas;      /*!< `TRUE` where the STA reports support for 802.11 link measurement. Every
                             member of this structure is one decoded bit of the five-octet RM
                             Enabled Capabilities field that `wifi_getRMCapabilities()` returns, so
                             an absent capability here means the bit was clear rather than that the
                             STA was not asked. */
    BOOL neighRpt;      /*!< `TRUE` where the STA reports support for neighbour reports, decoded
                             from the same RM Enabled Capabilities field. `bcnRptPassive`,
                             `bcnRptActive` and `bcnRptTable` below are the separate beacon-report
                             capabilities and are not implied by this one. */
    BOOL bcnRptPassive; /**< Supports Passive 11k scans. */
    BOOL bcnRptActive;  /**< Supports Active 11k scans. */
    BOOL bcnRptTable;   /*!< `TRUE` where the STA can answer a beacon request from its stored beacon
                             table rather than by scanning. It is independent of `bcnRptPassive` and
                             `bcnRptActive` above, so a caller must test the mode it intends to
                             request. */
    BOOL lciMeas;       /*!< `TRUE` where the STA reports support for location civic and LCI
                             measurement, decoded from the same RM Enabled Capabilities field. */
    BOOL ftmRangeRpt;   /*!< `TRUE` where the STA reports support for fine timing measurement range
                             reporting, decoded from the same RM Enabled Capabilities field. This
                             interface declares no call that requests such a report, so the member
                             states a capability only. */
} wifi_steering_rrmCaps_t;

/**
 * @brief Probe Request Event Data
 *
 * This data is provided with `WIFI_STEERING_EVENT_PROBE_REQ`.
 */
typedef struct
{
    mac_address_t client_mac; /*!< MAC address of the probing client, as the six octets of
                              `mac_address_t` in `wifi_hal_generic.h`. */
    UINT rssi;           /*!< RSSI of the received probe request. Held in a `UINT`, so it
                              is not a signed dBm value; the interface does not state the
                              encoding, and the RSSI members of the other steering events
                              use the same one, which is the value compared against the
                              `wifi_steering_clientConfig_t` water marks. */
    BOOL broadcast;      /*!< `TRUE` if the probe request was a broadcast probe rather than
                              one directed at a specific SSID. */
    BOOL blocked;        /*!< `TRUE` if the AP suppressed its probe response, which happens
                              when `rssi` falls outside the `rssiProbeLWM` to
                              `rssiProbeHWM` range configured for the client. */
} wifi_steering_evProbeReq_t;

#ifdef WIFI_HAL_VERSION_3_PHASE2

/**
 * @brief Client Connect Event Data - bitmask band form, compiled where
 *        `WIFI_HAL_VERSION_3_PHASE2` is defined.
 *
 * This data is provided with `WIFI_STEERING_EVENT_CLIENT_CONNECT`, as the `connect` member
 * of the `data` union in `wifi_steering_event_t`. It reports what the associating client is
 * capable of, which is the input a steering decision needs before it can move that client.
 *
 * This interface declares `wifi_steering_evConnect_t` twice, once in each arm of
 * `WIFI_HAL_VERSION_3_PHASE2`, and the two declarations are separately documented because
 * their public field layouts differ - see `Platform or Product Customization` in
 * `docs/pages/halSpec.md`. This is the arm that carries the client's band support as the
 * single bitmask `bandsCap`; the other arm carries it as the three `BOOL` members
 * `bandCap2G`, `bandCap5G` and `bandCap6G`. The two are not interchangeable: they differ in
 * member set, in size and in the offsets of the members that follow, so a handler compiled
 * against one form must not be given a structure produced by the other, and source that has
 * to build both ways must guard its access to the band members on the same flag.
 */
typedef struct
{
    mac_address_t client_mac; /*!< MAC address of the client that associated, as the six
                                   octets of `mac_address_t` in `wifi_hal_generic.h`. It is
                                   the key that ties this event to the disconnect, activity
                                   and RSSI events for the same client. */
    UINT isBTMSupported; /*!< Non-zero if the client supports BSS Transition Management,
                              which is what makes an 802.11v steer possible for it through
                              `wifi_setBTMRequest()`. Carried as a `UINT` rather than a
                              `BOOL`, so a caller should test for non-zero rather than
                              compare against `TRUE`. */
    UINT isRRMSupported; /*!< Non-zero if the client supports Radio Resource Management,
                              which is what makes a beacon measurement possible for it
                              through `wifi_setRMBeaconRequest()`. Carried as a `UINT` on
                              the same terms as `isBTMSupported`. The decoded detail of
                              what it supports is in `rrmCaps`. */
    wifi_freq_bands_t bandsCap; /*!< Bands the client is capable of, as a bitwise OR of
                                     `wifi_freq_bands_t` values from `wifi_hal_generic.h`.
                                     The type is an enumeration used as a bitmask, so a
                                     caller must test individual bits rather than compare
                                     the member against a single enumerator. This form can
                                     express the split-5GHz (`WIFI_FREQUENCY_5L_BAND`,
                                     `WIFI_FREQUENCY_5H_BAND`) and `WIFI_FREQUENCY_60_BAND`
                                     cases that the three-`BOOL` form of the other arm
                                     cannot. */
    wifi_steering_datarateInfo_t datarateInfo; /*!< Client's PHY capabilities - widest
                                     bandwidth, spatial streams, PHY mode, highest MCS and
                                     transmit power, and the two power-save and MU-MIMO
                                     flags. The member-by-member caveats are on
                                     `wifi_steering_datarateInfo_t` itself. */
    wifi_steering_rrmCaps_t rrmCaps; /*!< Decoded RM Enabled Capabilities of the client:
                                     which measurements it will accept. Meaningful only
                                     where `isRRMSupported` is non-zero; this interface does
                                     not state what the member carries otherwise. */
} wifi_steering_evConnect_t;

#else
/**
 * @brief Client Connect Event Data - three-`BOOL` band form, compiled where
 *        `WIFI_HAL_VERSION_3_PHASE2` is not defined.
 *
 * This data is provided with `WIFI_STEERING_EVENT_CLIENT_CONNECT`, as the `connect` member
 * of the `data` union in `wifi_steering_event_t`. It reports what the associating client is
 * capable of, which is the input a steering decision needs before it can move that client.
 *
 * This interface declares `wifi_steering_evConnect_t` twice, once in each arm of
 * `WIFI_HAL_VERSION_3_PHASE2`, and the two declarations are separately documented because
 * their public field layouts differ - see `Platform or Product Customization` in
 * `docs/pages/halSpec.md`. This is the arm that carries the client's band support as the
 * three `BOOL` members `bandCap2G`, `bandCap5G` and `bandCap6G`; the other arm replaces all
 * three with the single `wifi_freq_bands_t` bitmask `bandsCap`. The two are not
 * interchangeable: they differ in member set, in size and in the offsets of the members that
 * follow, so a handler compiled against one form must not be given a structure produced by
 * the other, and source that has to build both ways must guard its access to the band
 * members on the same flag.
 */
typedef struct
{
    mac_address_t client_mac; /*!< MAC address of the client that associated, as the six
                                   octets of `mac_address_t` in `wifi_hal_generic.h`. It is
                                   the key that ties this event to the disconnect, activity
                                   and RSSI events for the same client. */
    UINT isBTMSupported; /*!< Non-zero if the client supports BSS Transition Management,
                              which is what makes an 802.11v steer possible for it. Carried
                              as a `UINT` rather than a `BOOL`, so a caller should test for
                              non-zero rather than compare against `TRUE`. */
    UINT isRRMSupported; /*!< Non-zero if the client supports Radio Resource Management,
                              which is what makes a beacon measurement possible for it.
                              Carried as a `UINT` on the same terms as `isBTMSupported`. The
                              decoded detail of what it supports is in `rrmCaps`. */
    BOOL bandCap2G; /*!< `TRUE` if the client is 2.4GHz capable. */
    BOOL bandCap5G; /*!< `TRUE` if the client is 5GHz capable. This form carries one flag
                         for the whole 5GHz band, so it does not distinguish the low and
                         high halves that `WIFI_FREQUENCY_5L_BAND` and
                         `WIFI_FREQUENCY_5H_BAND` name in the other arm. */
    BOOL bandCap6G; /*!< `TRUE` if the client is 6GHz capable. This form has no member for
                         60GHz, so a 60GHz capability is not reportable in this
                         configuration. */
    wifi_steering_datarateInfo_t datarateInfo; /*!< Client's PHY capabilities - widest
                                     bandwidth, spatial streams, PHY mode, highest MCS and
                                     transmit power, and the two power-save and MU-MIMO
                                     flags. The member-by-member caveats are on
                                     `wifi_steering_datarateInfo_t` itself. */
    wifi_steering_rrmCaps_t rrmCaps; /*!< Decoded RM Enabled Capabilities of the client:
                                     which measurements it will accept. Meaningful only
                                     where `isRRMSupported` is non-zero; this interface does
                                     not state what the member carries otherwise. */
} wifi_steering_evConnect_t;
#endif

/**
 * @brief Client Disconnect Event Data
 *
 * This data is provided with `WIFI_STEERING_EVENT_CLIENT_DISCONNECT`.
 */
typedef struct
{
    mac_address_t client_mac; /*!< Address of the client that disconnected, as the six raw octets of
                                   `mac_address_t` rather than text. `reason` and `source` below say
                                   why it went and which end initiated it. */
    UINT reason; /*!< IEEE 802.11 reason code carried in the deauthentication or
                        disassociation frame; see the reason-code reference on
                        `wifi_steering_clientConfig_t`. */
    wifi_disconnectSource_t source; /*!< Which end initiated the disconnect, or
                        `DISCONNECT_SOURCE_UNKNOWN` where the `HAL` cannot tell. */
    wifi_disconnectType_t type; /*!< How the client was disconnected, or
                        `DISCONNECT_TYPE_UNKNOWN` where the `HAL` cannot tell. */
} wifi_steering_evDisconnect_t;

/**
 * @brief Client Activity Change Event Data
 *
 * This data is provided with `WIFI_STEERING_EVENT_CLIENT_ACTIVITY`.
 */
typedef struct
{
    mac_address_t client_mac; /*!< Address of the client whose activity changed. `mac_address_t` is
                                   six raw octets rather than text, so it carries no terminator;
                                   `active` beside it says which way the activity changed. */
    BOOL active; /*!< `TRUE` where the client has become active and `FALSE` where it has become
                      idle; the event reports the change rather than a level, so a caller tracks
                      state from the sequence of events. This interface states no idle threshold or
                      timeout behind the transition. */
} wifi_steering_evActivity_t;

/**
 * @brief Channel Utilization Event Data
 *
 * This data is provided with `WIFI_STEERING_EVENT_CHAN_UTILIZATION`.
 */
typedef struct
{
    UINT utilization; /**< Channel utilization (0-100). */
} wifi_steering_evChanUtil_t;

/**
 * @brief Client RSSI Crossing Event Data
 *
 * This data is provided with `WIFI_STEERING_EVENT_RSSI_XING`.
 */
typedef struct
{
    mac_address_t client_mac; /*!< MAC address of the client whose RSSI crossed a
                        threshold. */
    UINT rssi; /*!< Client's RSSI at the crossing, in the same encoding as the RSSI members
                        of the other steering events. */
    wifi_steering_rssiChange_t inactveXing; /*!< Direction in which `rssi` crossed
                        `rssiInactXing`, or `WIFI_STEERING_RSSI_UNCHANGED` if it did not.
                        The member name omits the "i" of "inactive". */
    wifi_steering_rssiChange_t highXing; /*!< Direction in which `rssi` crossed
                        `rssiHighXing`, or `WIFI_STEERING_RSSI_UNCHANGED` if it did not. */
    wifi_steering_rssiChange_t lowXing; /*!< Direction in which `rssi` crossed
                        `rssiLowXing`, or `WIFI_STEERING_RSSI_UNCHANGED` if it did not. */
} wifi_steering_evRssiXing_t;

/**
 * @brief Client RSSI Measurement Event Data
 *
 * This data is provided with `WIFI_STEERING_EVENT_RSSI`, which is sent in
 * response to a request for the client's current RSSI measurement.
 */
typedef struct
{
    mac_address_t client_mac; /*!< MAC address of the measured client, matching the
                        `client_mac` passed to `wifi_steering_clientMeasure()`. */
    UINT rssi; /*!< Measured RSSI for the client, in the same encoding as the RSSI members
                        of the other steering events. Where the measurement follows the
                        recommended method this is the average over the ACKs to five NULL
                        frames, as `wifi_steering_clientMeasure()` describes. */
} wifi_steering_evRssi_t;

/**
 * @brief Auth Failure Event Data
 *
 * This data is provided with `WIFI_STEERING_EVENT_AUTH_FAIL`.
 */
typedef struct
{
    mac_address_t client_mac; /*!< MAC address of the client whose authentication
                        failed. */
    UINT rssi; /*!< RSSI of the received auth frame, in the same encoding as the RSSI
                        members of the other steering events. This is the value compared
                        against the `rssiAuthLWM` to `rssiAuthHWM` range. */
    UINT reason; /*!< IEEE 802.11 status code sent in the rejection, which is the
                        `authRejectReason` configured for the client. Meaningful only where
                        `bsRejected` is `TRUE`. */
    BOOL bsBlocked; /*!< `TRUE` if band steering suppressed the auth response deliberately,
                        because `rssi` fell outside the configured auth range, rather than
                        the authentication failing for another cause. */
    BOOL bsRejected; /*!< `TRUE` if band steering sent an explicit rejection carrying
                        `reason`. `bsBlocked` `TRUE` with this `FALSE` is the silent-ignore
                        case that an `authRejectReason` of zero selects. */
} wifi_steering_evAuthFail_t;

/**
 * @brief Wifi Steering Event
 *
 * This is the data containing a single steering event. The `HAL` passes one instance to a
 * registered `wifi_steering_eventCB_t` handler.
 *
 * The `data` member is a union, and `type` is its discriminant: exactly one union member
 * is valid for any given event, and reading any other member is undefined. The mapping is
 * one-to-one with the `wifi_steering_eventType_t` values:
 *
 * - `WIFI_STEERING_EVENT_PROBE_REQ` selects `data.probeReq`
 * - `WIFI_STEERING_EVENT_CLIENT_CONNECT` selects `data.connect`
 * - `WIFI_STEERING_EVENT_CLIENT_DISCONNECT` selects `data.disconnect`
 * - `WIFI_STEERING_EVENT_CLIENT_ACTIVITY` selects `data.activity`
 * - `WIFI_STEERING_EVENT_CHAN_UTILIZATION` selects `data.chanUtil`
 * - `WIFI_STEERING_EVENT_RSSI_XING` selects `data.rssiXing`
 * - `WIFI_STEERING_EVENT_RSSI` selects `data.rssi`
 * - `WIFI_STEERING_EVENT_AUTH_FAIL` selects `data.authFail`
 *
 * @warning A handler must branch on `type` before touching `data`, and must treat a `type`
 *          value it does not recognise as carrying no readable union member rather than
 *          falling through to a default one.
 *
 * @see wifi_steering_eventCB_t
 * @see wifi_steering_eventType_t
 */
typedef struct
{
    wifi_steering_eventType_t type; /*!< Event type, and the discriminant that selects
                        which member of `data` is valid. */
    INT apIndex; /*!< Index of the AP the event came from. The indices this interface
                        defines are `AP_INDEX_1` to `AP_INDEX_24` in
                        `wifi_hal_generic.h`. */
    ULLONG timestamp_ms; /*!< Time the event occurred, in milliseconds. Optional: this
                        interface neither states the epoch it is measured from nor defines
                        a value that means "not supplied", so a caller should use it only
                        to order events from the same source. */
    union
    {
        wifi_steering_evProbeReq_t probeReq; /*!< Valid when `type` is
                        `WIFI_STEERING_EVENT_PROBE_REQ`. */
        wifi_steering_evConnect_t connect; /*!< Valid when `type` is
                        `WIFI_STEERING_EVENT_CLIENT_CONNECT`. */
        wifi_steering_evDisconnect_t disconnect; /*!< Valid when `type` is
                        `WIFI_STEERING_EVENT_CLIENT_DISCONNECT`. */
        wifi_steering_evActivity_t activity; /*!< Valid when `type` is
                        `WIFI_STEERING_EVENT_CLIENT_ACTIVITY`. */
        wifi_steering_evChanUtil_t chanUtil; /*!< Valid when `type` is
                        `WIFI_STEERING_EVENT_CHAN_UTILIZATION`. */
        wifi_steering_evRssiXing_t rssiXing; /*!< Valid when `type` is
                        `WIFI_STEERING_EVENT_RSSI_XING`. */
        wifi_steering_evRssi_t rssi; /*!< Valid when `type` is
                        `WIFI_STEERING_EVENT_RSSI`, which is the reply to
                        `wifi_steering_clientMeasure()`. */
        wifi_steering_evAuthFail_t authFail; /*!< Valid when `type` is
                        `WIFI_STEERING_EVENT_AUTH_FAIL`. */
    } data; /*!< The event payload, selected by `type`: exactly one arm is valid, the one
                        whose own comment names the `type` value it belongs to. The arms
                        overlap in storage, so a handler must switch on `type` before
                        reading this member, and must treat no arm as readable where it does
                        not recognise `type`. */
} wifi_steering_event_t;

// 802.11v BSS Transition Management Definitions
/**
 * @brief Maximum number of BTM devices.
 *
 * No structure in this header is bounded by this limit; within this interface its only
 * uses are in `wifi_hal_deprecated.h`, which is not part of the documented contract.
 */
#define MAX_BTM_DEVICES 64

/**
 * @brief Maximum length of a URL.
 *
 * Bounds the `url` member of `wifi_BTMRequest_t`, in octets.
 */
#define MAX_URL_LEN 512

/**
 * @brief Maximum number of BSS transition candidates.
 *
 * Bounds the `candidates` array of `wifi_BTMRequest_t`, `wifi_BTMQuery_t` and
 * `wifi_BTMResponse_t`, each of which carries its own `numCandidates` count of the
 * elements actually populated.
 */
#define MAX_CANDIDATES 64

/**
 * @brief Maximum size of vendor-specific data.
 *
 * Bounds the `buff` member of `wifi_VendorSpecific_t`, in octets.
 */
#define MAX_VENDOR_SPECIFIC 32

/**
 * @brief BSS Termination Duration subelement.
 *
 * This structure represents the BSS Termination Duration subelement (ID = 4),
 * as defined in 802.11 section 9.4.2.2.
 */
typedef struct
{
    ULONG tsf; /*!< Value of the 8-octet TSF timer at which the BSS termination takes
                        effect. Carried in a `ULONG`, which is not 8 octets wide on every
                        platform, so a caller must not assume the full TSF range is
                        representable here. */
    USHORT duration; /*!< Length of time the BSS will be down from `tsf`, in minutes, as
                        802.11 defines the subelement. */
} wifi_BTMTerminationDuration_t;

/**
 * @brief Condensed Country String.
 */
typedef struct
{
    CHAR condensedStr[3]; /*!< First two octets of the `dot11CountryString` MIB value,
                        which are the two-character ISO 3166-1 country code. The array is
                        three octets, one more than the code itself needs; this interface
                        does not state what the third octet holds, nor whether the `HAL`
                        writes a terminator, so a caller should read at most two
                        characters rather than relying on termination. */
} wifi_CondensedCountryString_t;

/**
 * @brief TSF information.
 *
 * This structure represents the TSF Information field of a Neighbor Report element, as
 * defined in 802.11-2016 section 9.4.2.36.
 */
typedef struct
{
    USHORT offset; /*!< Time until the neighbour AP's next beacon, in units of 512
                        microseconds, measured from the last beacon of the serving AP. */
    USHORT interval; /*!< Beacon interval of the neighbour AP, in time units of 1024
                        microseconds. */
} wifi_TSFInfo_t;

/**
 * @brief BSS Transition Candidate Preference.
 */
typedef struct
{
    UCHAR preference; /*!< Preference for transitioning to this candidate, in the range 1
                        to 255, where a higher value is more preferred. 802.11 reserves 0
                        to mean the candidate should not be transitioned to. */
} wifi_BSSTransitionCandidatePreference_t;

/**
 * @brief Bearing information.
 *
 * This structure represents the Bearing subelement of a Neighbor Report element, as
 * defined in 802.11-2016 section 9.4.2.36, which locates the neighbour AP relative to the
 * serving AP.
 */
typedef struct
{
    USHORT bearing; /*!< Direction of the neighbour AP from the serving AP, in units of
                        one degree clockwise from true north. */
    UINT dist; /*!< Straight-line distance to the neighbour AP, in centimetres. */
    USHORT height; /*!< Height of the neighbour AP relative to the serving AP, in
                        centimetres. */
} wifi_Bearing_t;

/**
 * @brief Wide Bandwidth Channel Element.
 *
 * This structure represents the Wide Bandwidth Channel Element (ID = 194),
 * as defined in 802.11-2016 section 9.4.2.161.
 */
typedef struct
{
    UCHAR bandwidth; /*!< Channel width, as the 802.11 Channel Width code rather than a
                        count of MHz. */
    UCHAR centerSeg0; /*!< Channel index of the centre frequency of the first frequency
                        segment. */
    UCHAR centerSeg1; /*!< Channel index of the centre frequency of the second frequency
                        segment, used for 80+80MHz. Zero where there is no second
                        segment. */
} wifi_WideBWChannel_t;

/**
 * @brief Measurement information.
 *
 * This structure represents a Measurement Request element carried in a Neighbor Request
 * frame, as defined in 802.11-2016 section 9.4.2.20.
 *
 * The `u` member is a union and `type` is its discriminant: for a measurement of LCI type
 * only `u.lci` is valid, and for one of LCR type only `u.lcr` is valid. Reading the other
 * member is undefined, and a caller that does not recognise `type` must treat neither as
 * readable.
 */
typedef struct
{
    UCHAR token; /*!< Measurement token, which relates this request to the report that
                        answers it. */
    UCHAR mode; /*!< Measurement request mode bitmask, carrying the enable, request and
                        report control bits 802.11 defines for the element. */
    UCHAR type; /*!< Measurement type, and the discriminant that selects which member of
                        `u` is valid. */
    union
    {
        UCHAR lci; /*!< Location Configuration Information (LCI) subject, valid when `type`
                        names an LCI measurement. */
        UCHAR lcr; /*!< Location Civic Report (LCR) subject, valid when `type` names an LCR
                        measurement. */
    } u; /*!< The measurement subject, selected by `type`: only `lci` is valid for a
                        measurement of LCI type and only `lcr` for one of LCR type. The arms
                        overlap in storage, so a caller must read `type` first, and must
                        treat neither arm as readable where it does not recognise `type`. */
} wifi_Measurement_t;

/**
 * @brief HT Capabilities Element.
 *
 * This structure represents the HT Capabilities Element (ID = 45), as defined
 * in 802.11-2016 section 9.4.2.56.
 */
typedef struct
{
    USHORT info; /*!< HT Capability Information field, as the 16-bit bitmask 802.11 defines
                        for the element. This interface does not name the individual
                        bits. */
    UCHAR ampduParams; /*!< A-MPDU Parameters field, carrying the maximum A-MPDU length
                        exponent and the minimum MPDU start spacing. */
    UCHAR mcs[16]; /*!< Supported MCS Set field, as 16 octets of bitmask. Fixed length,
                        with no count member: all 16 octets are always present, and a
                        supported MCS is indicated by its bit rather than by an index into
                        this array. */
    USHORT extended; /*!< HT Extended Capabilities field, as a 16-bit bitmask. */
    UINT txBeamCaps; /*!< Transmit Beamforming Capabilities field, as a 32-bit
                        bitmask. */
    UCHAR aselCaps; /*!< ASEL Capability field, carrying the antenna selection
                        capabilities as a bitmask. */
} wifi_HTCapabilities_t;

/**
 * @brief VHT Capabilities Element.
 *
 * This structure represents the VHT Capabilities Element (ID = 191), as
 * defined in 802.11-2016 section 9.4.2.158.
 *
 * The element has two parts and so does this structure: `info` is the VHT Capabilities
 * Information field, and the four `USHORT` members after it are the Supported VHT-MCS and
 * NSS Set field, split into its four 16-bit subfields in the order the cited section defines
 * them. Every member is a packed bitmask or a bit-packed rate rather than a plain number,
 * and this interface names none of the individual bits, so a caller decodes each member
 * against the cited section rather than reading it as a value.
 *
 * The structure is of fixed shape, with no length or presence member of its own, so all five
 * members are always present. Where one of these structures is reached through
 * `wifi_NeighborReport_t` it is meaningful only while that report's `vhtCapsPresent` flag is
 * `TRUE`.
 */
typedef struct
{
    UINT   info;                   /*!< VHT Capabilities Information field, as the 32-bit
                                        bitmask the cited section defines: maximum MPDU
                                        length, supported channel width set, the STBC,
                                        beamformer and beamformee capability bits, and the
                                        remaining per-STA VHT capability bits. This interface
                                        names none of them individually. */
    USHORT mcs;                    /*!< Rx VHT-MCS Map, the first 16-bit subfield of the
                                        Supported VHT-MCS and NSS Set: two bits per spatial
                                        stream naming the highest MCS the reporting STA can
                                        receive on that stream, decoded per the cited
                                        section. It is a per-stream map, not an MCS
                                        index. */
    USHORT rxHighestSupportedRate; /*!< Rx Highest Supported Long GI Data Rate subfield,
                                        carried in the low bits of the member with the
                                        adjacent subfield the cited section places beside it
                                        occupying the rest. A caller must therefore mask out
                                        the rate rather than read the whole member as a
                                        rate. */
    USHORT txVHTmcs;               /*!< Tx VHT-MCS Map, on the same two-bits-per-stream terms
                                        as `mcs` but for what the reporting STA can
                                        transmit. The two maps are independent, so a caller
                                        must not read either as covering both
                                        directions. */
    USHORT txHighestSupportedRate; /*!< Tx Highest Supported Long GI Data Rate subfield, on
                                        the same terms as `rxHighestSupportedRate`. */
} wifi_VHTCapabilities_t;

/**
 * @brief HT Operation Element.
 *
 * This structure represents the HT Operation Element (ID = 61), as defined in
 * 802.11-2016 section 9.4.2.57.
 */
typedef struct
{
    UCHAR primary; /*!< Primary channel of the BSS the element describes. The element carries no
                        band, and the same channel number denotes different channels in different
                        bands, so a caller must take the band from the context the element arrived
                        in. */
    UCHAR opInfo[5]; /*!< HT Operation Information field, as the five octets of bitmask
                        802.11 defines for the element. Fixed length, with no count
                        member. */
    UCHAR mcs[16]; /*!< Basic MCS Set field, as 16 octets of bitmask, on the same terms as
                        the `mcs` member of `wifi_HTCapabilities_t`. */
} wifi_HTOperation_t;

/**
 * @brief VHT Operation Element.
 *
 * This structure represents the VHT Operation Element (ID = 192), as defined
 * in 802.11-2016 section 9.4.2.159.
 *
 * Where `wifi_VHTCapabilities_t` reports what a STA is able to do, this element reports how
 * the BSS is actually operating, so the two are read together rather than interchangeably.
 * The structure is of fixed shape with no length or presence member of its own; reached
 * through `wifi_NeighborReport_t` it is meaningful only while that report's `vhtOpPresent`
 * flag is `TRUE`.
 */
typedef struct
{
    wifi_WideBWChannel_t opInfo; /*!< VHT Operation Information field: the operating channel
                        width code and the centre-frequency indices of the first and second
                        frequency segments, held in the same three members as the Wide
                        Bandwidth Channel element. `bandwidth` is a channel width code rather
                        than a count of MHz, and `centerSeg1` is zero where the BSS is not
                        using 80+80MHz - see `wifi_WideBWChannel_t` for the member
                        detail. */
    USHORT mcs_nss; /*!< Basic VHT-MCS and NSS Set field, as the 16-bit bitmask the cited
                        section defines: two bits per spatial stream naming the highest MCS
                        every STA in the BSS is required to support on that stream. It is the
                        BSS-wide requirement, not a per-STA capability, and this interface
                        names none of the individual bits, so a caller decodes it against the
                        cited section. */
} wifi_VHTOperation_t;

/**
 * @brief Secondary Channel Offset Element.
 *
 * This structure represents the Secondary Channel Offset Element (ID = 62),
 * as defined in 802.11-2016 section 9.4.2.20.
 */
typedef struct
{
    UCHAR secondaryChOffset; /*!< Position of the secondary channel relative to the
                        primary, as the 802.11 Secondary Channel Offset code: no secondary
                        channel, secondary above, or secondary below. */
} wifi_SecondaryChannelOffset_t;

/**
 * @brief RM Enabled Capabilities Element.
 *
 * This structure represents the RM Enabled Capabilities Element (ID = 70),
 * as defined in 802.11-2016 section 9.4.2.45.
 */
typedef struct
{
    UCHAR capabilities[5]; /*!< RM Enabled Capabilities field, as the five octets of
                        bitmask 802.11 defines for the element. Fixed length, with no count
                        member: all five octets are always present. This is the same
                        five-octet encoding that `wifi_getRMCapabilities()` returns, and
                        `wifi_steering_rrmCaps_t` carries the decoded form of the subset
                        that steering uses. */
} wifi_RMEnabledCapabilities_t;

/**
 * @brief Vendor Specific Element.
 *
 * This structure represents the Vendor Specific Element (ID = 221), as
 * defined in 802.11-2016 section 9.4.2.26.
 */
typedef struct
{
    UCHAR oui[5]; /*!< Organization Identifier, which is either the first 3 octets or all 5
                        octets of this array depending on the format in use. This interface
                        carries no member stating which, so a caller must determine the
                        length from the OUI itself. */
    UCHAR buff[MAX_VENDOR_SPECIFIC]; /*!< Vendor-specific content, up to
                        `MAX_VENDOR_SPECIFIC` octets. This interface carries no length
                        member for it, so how much of the buffer is meaningful is not
                        established here and a caller must derive it from the vendor's own
                        format. */
} wifi_VendorSpecific_t;

/**
 * @brief Measurement Pilot Transmission Element.
 *
 * This structure represents the Measurement Pilot Transmission Element
 * (ID = 66), as defined in 802.11-2016 section 9.4.2.42.
 */
typedef struct
{
    UCHAR pilot; /*!< Measurement Pilot Interval, in time units of 1024 microseconds. Zero
                        means measurement pilot frames are not transmitted. */
    wifi_VendorSpecific_t vendorSpecific; /*!< Optional vendor-specific subelement carried
                        with the pilot transmission information. This interface carries no
                        flag stating whether it is present, so a caller must treat it as
                        meaningful only where the vendor's own format says it is. */
} wifi_MeasurementPilotTransmission_t;

/**
 * @brief Neighbor Report.
 *
 * This structure represents the Neighbor Report element (ID = 52), as defined in
 * 802.11-2016 section 9.4.2.36. One instance describes a single candidate AP, and arrays
 * of them are carried as the candidate lists of `wifi_BTMRequest_t`, `wifi_BTMQuery_t` and
 * `wifi_BTMResponse_t`.
 *
 * Every optional subelement is expressed as a pair: a `BOOL` named `<field>Present`
 * followed by the field itself. The `BOOL` is the only thing that makes the field
 * meaningful - a caller must test it before reading the field, and must not infer
 * presence from a field being non-zero, because this interface does not state that the
 * `HAL` clears an absent one. A caller populating a report must set both, and a caller
 * reading one must treat a field whose flag is `FALSE` as carrying nothing.
 *
 * @see wifi_BTMRequest_t
 */
typedef struct
{
    bssid_t bssid; /*!< BSSID of the candidate AP, as the six octets of `bssid_t` in
                        `wifi_hal_generic.h`. */
    //  32 bit optional value, bit fileds are
    //  b0, b1 for reachability
    //  b2 security
    //  b3 key scope
    //  b4 to b9 capabilities
    //  b10 mobility domain
    //  b11 high troughput
    //  b12 very high throughput
    //  b13 ftm
    //  b14 to b31 reserved
    UINT info; /*!< BSSID Information field, as a 32-bit bitmask whose bit assignments are
                        listed in the comment immediately above this member. */
    UCHAR opClass; /*!< Operating class of the candidate AP's channel, which is what gives
                        `channel` its band and width. */
    UCHAR channel; /*!< Channel number of the candidate AP, interpreted within
                        `opClass`. */
    UCHAR phyTable; /*!< PHY type of the candidate AP, as the 802.11 PHY type code. */
    BOOL tsfPresent; /*!< `TRUE` where `tsfInfo` below carries TSF information; where it is `FALSE`
                          a caller must not read that member. Each optional field of this structure
                          has its own such flag, and this interface does not state what an absent
                          field's storage holds. */
    wifi_TSFInfo_t tsfInfo; /*!< TSF Information subelement for the candidate AP - when its next
                                 beacon is due and at what interval - valid only where `tsfPresent`
                                 is `TRUE`. See `wifi_TSFInfo_t` for the units of both of its
                                 fields. */
    BOOL condensedCountrySringPresent; /*!< `TRUE` if `condensedCountryStr` carries the
                        candidate AP's country code. The member name misspells "String" as
                        "Sring": the identifier a caller has to write is
                        `condensedCountrySringPresent`, and the correctly spelt form does not
                        exist in this interface. The type name `wifi_CondensedCountryString_t`
                        and the field name `condensedCountryStr` beside it are both spelt
                        normally, so the misspelling is confined to this one flag. */
    wifi_CondensedCountryString_t condensedCountryStr; /*!< Condensed Country String of the
                        candidate AP, valid only where `condensedCountrySringPresent` is
                        `TRUE`. See `wifi_CondensedCountryString_t` for how much of
                        `condensedStr` a caller may read. */
    BOOL bssTransitionCandidatePreferencePresent; /*!< `TRUE` where
                                                       `bssTransitionCandidatePreference` below
                                                       carries a preference; where it is `FALSE` a
                                                       caller must not read that member. */
    wifi_BSSTransitionCandidatePreference_t bssTransitionCandidatePreference; /*!< How strongly the
                        serving AP prefers this candidate, valid only where
                        `bssTransitionCandidatePreferencePresent` is `TRUE`.
                        `wifi_BSSTransitionCandidatePreference_t` gives the 1 to 255 range, the
                        direction in which it increases, and what 802.11 reserves 0 for. */
    BOOL btmTerminationDurationPresent; /*!< `TRUE` where `btmTerminationDuration` below carries a
                                             termination time and duration; where it is `FALSE` a
                                             caller must not read that member. */
    wifi_BTMTerminationDuration_t btmTerminationDuration; /*!< When the candidate's BSS is due to
                        terminate and for how long, valid only where `btmTerminationDurationPresent`
                        is `TRUE`. See `wifi_BTMTerminationDuration_t` for the units of both fields
                        and for the width caveat on its `tsf` member. */
    BOOL bearingPresent; /*!< `TRUE` where `bearing` below carries the candidate's direction,
                              distance and height; where it is `FALSE` a caller must not read that
                              member. */
    wifi_Bearing_t bearing; /*!< Where the candidate AP lies relative to the serving AP, valid only
                                 where `bearingPresent` is `TRUE`. See `wifi_Bearing_t` for the
                                 direction, distance and height units. */
    BOOL wideBandWidthChannelPresent; /*!< `TRUE` where `wideBandwidthChannel` below carries the
                                           candidate's channel width and centre-frequency segments;
                                           where it is `FALSE` a caller must not read that member.
                                           The flag and the member it guards are spelt differently -
                                           `wideBandWidthChannelPresent` against
                                           `wideBandwidthChannel` - so each must be written exactly
                                           as declared. */
    wifi_WideBWChannel_t wideBandwidthChannel; /*!< Channel width and centre-frequency segments the
                        candidate AP uses, valid only where `wideBandWidthChannelPresent` is `TRUE`.
                        The flag is spelt `wideBandWidthChannelPresent` and this member
                        `wideBandwidthChannel`, so a caller must write each exactly as declared. See
                        `wifi_WideBWChannel_t`, which states that `bandwidth` is an 802.11 code
                        rather than a count of MHz. */
    BOOL htCapsPresent; /*!< `TRUE` where `htCaps` below carries the candidate's HT Capabilities
                             element; where it is `FALSE` a caller must not read that member. It
                             says nothing about the VHT fields, which have their own flags. */
    wifi_HTCapabilities_t htCaps; /*!< HT Capabilities element of the candidate AP, valid only where
                                       `htCapsPresent` above is `TRUE`. See `wifi_HTCapabilities_t`,
                                       whose fields are 802.11 bitmasks and whose supported-MCS set
                                       is a fixed 16 octets with no count member. */
    BOOL vhtCapsPresent; /*!< Whether the VHT Capabilities field is present. The flag is
                        spelt `vhtCapsPresent` but the field it guards is spelt
                        `vbhtCaps`. */
    wifi_VHTCapabilities_t vbhtCaps; /*!< VHT Capabilities of the candidate AP, valid only
                        where `vhtCapsPresent` is `TRUE`. The member name carries a stray
                        "b": the identifier a caller has to write is `vbhtCaps`, and there is
                        no `vhtCaps` member in this structure to write instead, even though
                        `htCaps` above and `vhtOp` below are both spelt normally. The
                        misspelling is confined to this one field. See
                        `wifi_VHTCapabilities_t` for the member detail. */
    BOOL htOpPresent; /*!< `TRUE` where `htOp` below carries the candidate's HT Operation element;
                           where it is `FALSE` a caller must not read that member. */
    wifi_HTOperation_t htOp; /*!< HT Operation element of the candidate AP, valid only where
                                  `htOpPresent` above is `TRUE`. See `wifi_HTOperation_t` for its
                                  primary channel, its five octets of operation information and its
                                  basic MCS set. */
    BOOL vhtOpPresent; /*!< `TRUE` where `vhtOp` below carries the candidate's VHT Operation
                            element; where it is `FALSE` a caller must not read that member.
                            `vhtCapsPresent` above guards the separate capabilities field. */
    wifi_VHTOperation_t vhtOp; /*!< VHT Operation element of the candidate AP, valid only where
                                    `vhtOpPresent` above is `TRUE`. See `wifi_VHTOperation_t`, whose
                                    `opInfo.bandwidth` is a channel width code rather than a count
                                    of MHz and whose `mcs_nss` is the BSS-wide requirement rather
                                    than a per-STA capability. */
    BOOL secondaryChannelOffsetPresent; /*!< `TRUE` where `secondaryChannelOffset` below carries the
                                             offset of the candidate's secondary channel; where it
                                             is `FALSE` a caller must not read that member. */
    wifi_SecondaryChannelOffset_t secondaryChannelOffset; /*!< Position of the candidate AP's
                        secondary channel relative to its primary, valid only where
                        `secondaryChannelOffsetPresent` is `TRUE`. `wifi_SecondaryChannelOffset_t`
                        states that the value is the 802.11 offset code rather than a count of MHz. */
    BOOL rmEnabledCapsPresent; /*!< `TRUE` where `rmEnabledCaps` below carries the candidate's radio
                                    measurement capabilities; where it is `FALSE` a caller must not
                                    read that member. */
    wifi_RMEnabledCapabilities_t rmEnabledCaps; /*!< RM Enabled Capabilities element of the
                                                     candidate AP, valid only where
                                                     `rmEnabledCapsPresent` above is `TRUE`.
                                                     `wifi_RMEnabledCapabilities_t` carries the five
                                                     octets of bitmask in the same encoding
                                                     `wifi_getRMCapabilities()` returns. */
    BOOL msmtPilotTransmissionPresent; /*!< `TRUE` where `msmtPilotTransmission` below carries the
                                            candidate's pilot interval; where it is `FALSE` a caller
                                            must not read that member. */
    wifi_MeasurementPilotTransmission_t msmtPilotTransmission; /*!< Measurement Pilot Transmission
                        element of the candidate AP, valid only where `msmtPilotTransmissionPresent`
                        above is `TRUE`. `wifi_MeasurementPilotTransmission_t` states that its
                        interval is in units of 1024 microseconds and that zero means pilot frames
                        are not transmitted. */
    BOOL vendorSpecificPresent; /*!< `TRUE` where `vendorSpecific` below carries a Vendor Specific
                                     element; where it is `FALSE` a caller must not read that
                                     member. The element's own content is in the vendor's format,
                                     which this interface does not define. */
    wifi_VendorSpecific_t vendorSpecific; /*!< Vendor Specific element carried with this candidate,
                        valid only where `vendorSpecificPresent` is `TRUE`. `wifi_VendorSpecific_t`
                        states that neither the length of its OUI nor the meaningful extent of its
                        buffer is established by this interface, so a caller must take both from the
                        vendor's own format. */
    ssid_t target_ssid; /*!< SSID of the candidate AP, in the 32-octet `ssid_t` of
                        `wifi_hal_generic.h`. An SSID may occupy all 32 octets, so it is
                        not necessarily NUL-terminated and a caller must not treat it as a
                        C string. This member is not paired with a presence flag, unlike
                        the optional subelements above it. */
} wifi_NeighborReport_t;

/**
 * @brief BSS Transition Management Request Frame.
 *
 * This structure represents the BSS Transition Management Request Frame, as
 * defined in 802.11-2016 section 9.6.14.9.
 */
typedef struct
{
    UCHAR token; /*!< Dialog token relating this request to the response that answers it.
                        Set by the STA on a STA-initiated transaction; on an AP-initiated
                        request sent with `wifi_setBTMRequest()` the AP chooses it. */
    UCHAR requestMode; /*!< Request Mode bitmask, carrying the candidate-list-included,
                        abridged, disassociation-imminent, BSS-termination-included and
                        ESS-disassociation-imminent bits 802.11 defines. */
    USHORT timer; /*!< Disassociation Timer, in TBTTs, counting down to the point at which
                        the AP will disassociate the STA. Zero means the AP has no
                        disassociation intent. */
    UCHAR validityInterval; /*!< Time for which the candidate list stays valid, in TBTTs,
                        measured from the transmission of this frame. */
    // The optional fields may include:
    // 1. BSS Termination Duration Subelement, ID = 4. 802.11-2016 Figure 9-300.
    // 2. Session Information URL.
    // 3. BSS Transition Candidate List Entries
    wifi_BTMTerminationDuration_t termDuration; /*!< BSS Termination Duration subelement,
                        meaningful only where `requestMode` sets the BSS-termination
                        -included bit. */
    UCHAR disassociationImminent; /*!< Non-zero if the AP intends to disassociate the STA,
                        which duplicates the corresponding bit of `requestMode`. This
                        interface does not state which takes precedence if the two
                        disagree, so a caller should set them consistently. */
    USHORT urlLen; /*!< Number of valid octets in `url`, from 0 to `MAX_URL_LEN`. Zero
                        means no session information URL is carried. */
    CHAR url[MAX_URL_LEN]; /*!< Session Information URL, bounded by `MAX_URL_LEN` octets
                        and valid only for its leading `urlLen` octets. This interface does
                        not state that the `HAL` NUL-terminates it, so a caller must use
                        `urlLen` rather than treating it as a C string. */
    UCHAR numCandidates; /*!< Number of populated elements in `candidates`, from 0 to
                        `MAX_CANDIDATES`. Zero means no candidate list is carried, which
                        should agree with the candidate-list-included bit of
                        `requestMode`. */
    wifi_NeighborReport_t candidates[MAX_CANDIDATES]; /*!< Candidate APs the STA may
                        transition to, in preference order. The array is bounded by
                        `MAX_CANDIDATES` and only its leading `numCandidates` elements are
                        valid; the remainder must not be read. */
} wifi_BTMRequest_t;

/**
 * @brief BSS Transition Management Query Frame.
 *
 * This structure represents the BSS Transition Management Query Frame, as
 * defined in 802.11-2016 section 9.6.14.8.
 */
typedef struct
{
    UCHAR token; /*!< Dialog token set by the STA, which the AP must echo in the
                        `wifi_BTMRequest_t` it returns so the STA can relate the two. */
    UCHAR queryReason; /*!< BTM Query Reason code, stating why the STA is asking to
                        transition. */
    UCHAR numCandidates; /*!< Number of populated elements in `candidates`, from 0 to
                        `MAX_CANDIDATES`. Zero means the STA offered no candidates of its
                        own. */
    wifi_NeighborReport_t candidates[MAX_CANDIDATES]; /*!< Candidate APs the STA proposes,
                        bounded by `MAX_CANDIDATES`. Only the leading `numCandidates`
                        elements are valid; the remainder must not be read. */
} wifi_BTMQuery_t;

/**
 * @brief BSS Transition Management Response Frame.
 *
 * This structure represents the BSS Transition Management Response Frame, as
 * defined in 802.11-2016 section 9.6.14.10.
 */
typedef struct
{
    UCHAR token; /*!< Dialog token echoed from the `wifi_BTMRequest_t` this response
                        answers, which is how a caller matches the two. */
    UCHAR status; /*!< BTM Status code stating whether the STA accepted the transition and,
                        where it did not, why. Zero is acceptance; every other value is a
                        rejection reason 802.11 defines. */
    UCHAR terminationDelay; /*!< Delay the STA requests before the BSS is terminated, in
                        TBTTs. Meaningful only where `status` is the code that reports the
                        STA is not accepting termination yet. */
    bssid_t target; /*!< BSSID the STA has chosen to transition to. Meaningful only where
                        `status` reports acceptance. */
    UCHAR numCandidates; /*!< Number of populated elements in `candidates`, from 0 to
                        `MAX_CANDIDATES`. */
    wifi_NeighborReport_t candidates[MAX_CANDIDATES]; /*!< Candidate APs the STA reports,
                        bounded by `MAX_CANDIDATES`. Only the leading `numCandidates`
                        elements are valid; the remainder must not be read. */
} wifi_BTMResponse_t;

/**
 * @brief Neighbor Request Frame.
 *
 * This structure represents the Neighbor Request Frame, as defined in
 * 802.11-2016 section 9.6.7.6.
 */
typedef struct
{
    UCHAR token; /*!< Dialog token relating this request to the neighbour report that
                        answers it. */
    UCHAR ssidLen; /*!< Number of valid octets in `ssid`, from 0 to 32. Zero means no SSID
                        is carried, so the request is not scoped to one network. */
    ssid_t ssid; /*!< SSID the request is scoped to, in the 32-octet `ssid_t` of
                        `wifi_hal_generic.h`, valid only for its leading `ssidLen` octets
                        and not necessarily NUL-terminated. */
    UCHAR measCount; /*!< Number of populated elements in `measurements`, which this
                        structure bounds at 2. */
    wifi_Measurement_t measurements[2]; /*!< Measurement requests carried with the
                        neighbour request, at most two - one LCI and one LCR. Only the
                        leading `measCount` elements are valid; the remainder must not be
                        read. */
} wifi_NeighborRequestFrame_t;

// 802.11k Beacon request & report structures and function prototypes
/**
 * @brief Maximum number of requested elements in a beacon report.
 *
 * Bounds the `ids` array of `wifi_RequestedElementIDS_t`, and therefore also of
 * `wifi_ExtdRequestedElementIDS_t`.
 */
#define MAX_REQUESTED_ELEMS 8

/**
 * @brief Maximum number of channels in a channel report.
 *
 * Bounds the `channels` array of `wifi_ChannelReport_t`, in entries.
 */
#define MAX_CHANNELS_REPORT 64

/**
 * @brief Beacon reporting configuration.
 *
 * This structure represents the Beacon Reporting subelement of a Beacon Request, which
 * makes the report conditional rather than unconditional.
 */
typedef struct
{
    UCHAR condition; /*!< Reporting Condition code, naming which comparison against
                        `threshold` triggers a report - for example RCPI or RSNI rising
                        above or falling below it. Zero is 802.11's unconditional report,
                        for which `threshold` carries nothing. */
    UCHAR threshold; /*!< Reference value that `condition` compares against, in the units
                        of whichever quantity `condition` selects. */
} wifi_BeaconReporting_t;

/**
 * @brief Requested element IDs.
 */
typedef struct
{
    UCHAR ids[MAX_REQUESTED_ELEMS]; /*!< Element IDs the reporting STA is asked to include
                        in each beacon report, bounded at `MAX_REQUESTED_ELEMS` entries.
                        The array is of fixed length and this interface carries neither a
                        count member nor a terminator for it, so how many entries are
                        populated is not specified by this interface and a caller cannot
                        determine the logical length from the data. In particular a zero
                        entry does not mark the end of the list: 0 is the SSID element ID,
                        so it is a valid element ID rather than a sentinel. Both parties
                        therefore have to agree the populated length outside this
                        interface. */
} wifi_RequestedElementIDS_t;

/**
 * @brief Extended requested element IDs.
 *
 * An alias of `wifi_RequestedElementIDS_t`, used where the IDs requested are element ID
 * extensions rather than element IDs. The two are distinct fields of
 * `wifi_BeaconRequest_t` with the same layout and the same bound.
 */
typedef wifi_RequestedElementIDS_t wifi_ExtdRequestedElementIDS_t;

/**
 * @brief AP Channel Report Element.
 *
 * This structure represents the AP Channel Report Element (ID = 51), as
 * defined in 802.11-2016 section 9.4.2.36.
 */
typedef struct
{
    UCHAR opClass; /*!< Operating class shared by every channel in `channels`, which is
                        what gives those channel numbers their band and width. */
    UCHAR channels[MAX_CHANNELS_REPORT]; /*!< Channels the measurement is to cover, within
                        `opClass`, bounded at `MAX_CHANNELS_REPORT` entries. The array is of
                        fixed length and this interface carries neither a count member nor a
                        terminator for it, so how many entries are populated is not
                        specified by this interface and a caller cannot determine the
                        logical length from the data. A zero entry is not a sentinel either:
                        this interface gives channel 0 a meaning of its own on the `channel`
                        member of `wifi_BeaconRequest_t`. Both parties therefore have to
                        agree the populated length outside this interface. */
} wifi_ChannelReport_t;

/**
 * @brief Beacon Request.
 *
 * This structure represents the Beacon Request frame, as defined in
 * 802.11-2016 section 9.4.2.21.7. A caller passes one to `wifi_setRMBeaconRequest()` to
 * ask a peer STA to measure beacons and report what it heard.
 *
 * The optional subelements follow the same pairing as `wifi_NeighborReport_t`: a `BOOL`
 * named `<field>Present` followed by the field itself, and the field is meaningful only
 * where the flag is `TRUE`.
 *
 * @see wifi_setRMBeaconRequest
 * @see wifi_BeaconReport_t
 */
typedef struct
{
    UCHAR opClass; /*!< Operating class the measurement is to be made in, which is what
                        gives `channel` its band and width. */
    UCHAR channel; /*!< Channel the STA is to measure, within `opClass`. 802.11 reserves 0
                        to mean every channel in the operating class, and 255 to mean the
                        channels named by `channelReport`. */
    USHORT randomizationInterval; /*!< Upper bound of the random delay the STA applies
                        before starting the measurement, in time units of 1024
                        microseconds. Zero means start as soon as possible. */
    USHORT duration; /*!< Length of the measurement, in time units of 1024
                        microseconds. */
    UCHAR mode; /*!< Measurement mode, selecting passive, active or beacon-table
                        measurement. */
    bssid_t bssid; /*!< BSSID the measurement is scoped to. The all-ones broadcast address
                        requests every BSSID. */
    BOOL ssidPresent; /*!< `TRUE` if `ssid` carries an SSID to scope the measurement
                        to. */
    ssid_t ssid; /*!< SSID the measurement is scoped to, valid only where `ssidPresent` is
                        `TRUE`. Held in the 32-octet `ssid_t` of `wifi_hal_generic.h` and
                        not necessarily NUL-terminated. */
    BOOL beaconReportingPresent; /*!< `TRUE` where `beaconReporting` below carries the condition and
                                      threshold that decide when a beacon is reported; where it is
                                      `FALSE` a caller must not read that member.
                                      `reportingRetailPresent` below guards the separate
                                      reporting-detail level. */
    wifi_BeaconReporting_t beaconReporting; /*!< Condition and threshold that decide when the STA
                                                 reports a measured beacon, valid only where
                                                 `beaconReportingPresent` above is `TRUE`.
                                                 `wifi_BeaconReporting_t` states that a zero
                                                 condition is 802.11's unconditional report, for
                                                 which the threshold carries nothing. */
    BOOL reportingRetailPresent; /*!< `TRUE` if `reportingDetail` carries a reporting
                        detail level. The member name misspells "reporting detail"; it is
                        the presence flag for `reportingDetail` and nothing else. */
    UCHAR reportingDetail; /*!< How much of each measured beacon the STA is to report: no
                        fixed fields or elements, the fixed fields plus the requested
                        elements, or all fixed fields and elements. Valid only where
                        `reportingRetailPresent` is `TRUE`. */
    BOOL wideBandWidthChannelPresent; /*!< `TRUE` where `wideBandwidthChannel` below scopes the
                                           measurement to a channel width and centre-frequency
                                           segments; where it is `FALSE` a caller must not read that
                                           member. The flag and the member it guards are spelt
                                           differently - `wideBandWidthChannelPresent` against
                                           `wideBandwidthChannel` - so each must be written exactly
                                           as declared. */
    wifi_WideBWChannel_t wideBandwidthChannel; /*!< Channel width and centre-frequency segments the
                        measurement is scoped to, valid only where `wideBandWidthChannelPresent` is
                        `TRUE`. See `wifi_WideBWChannel_t` for the encoding of `bandwidth` and for
                        what a zero second segment means. */
    BOOL requestedElementIDSPresent; /*!< `TRUE` where `requestedElementIDS` below names the
                                          elements to include in each report; where it is `FALSE` a
                                          caller must not read that member.
                                          `extdRequestedElementIDSPresent` below guards the extended
                                          IDs separately, so the two may differ. */
    wifi_RequestedElementIDS_t requestedElementIDS; /*!< Element IDs the reporting STA is asked to
                        include in each beacon report, valid only where `requestedElementIDSPresent`
                        is `TRUE`. `wifi_RequestedElementIDS_t` states that its array carries
                        neither a count nor a terminator, so the populated length has to be agreed
                        outside this interface; `extdRequestedElementIDS` below carries the extended
                        IDs separately. */
    BOOL extdRequestedElementIDSPresent; /*!< `TRUE` where `extdRequestedElementIDS` below names the
                                              element ID extensions to include; where it is `FALSE`
                                              a caller must not read that member. It is independent
                                              of `requestedElementIDSPresent` above. */
    wifi_ExtdRequestedElementIDS_t extdRequestedElementIDS; /*!< Element ID extensions the reporting
                        STA is asked to include, valid only where `extdRequestedElementIDSPresent`
                        above is `TRUE`. The type is an alias of `wifi_RequestedElementIDS_t`, so
                        its array carries neither a count nor a terminator and the populated length
                        has to be agreed outside this interface; `requestedElementIDS` above carries
                        the plain element IDs. */
    BOOL channelReportPresent; /*!< `TRUE` if `channelReport` names the channels to
                        measure, which is what makes a `channel` of 255 meaningful. */
    wifi_ChannelReport_t channelReport; /*!< Channels to measure, valid only where
                        `channelReportPresent` is `TRUE`. */
    BOOL vendorSpecificPresent; /*!< `TRUE` if `vendorSpecific` carries a vendor-specific
                        subelement. */
    wifi_VendorSpecific_t vendorSpecific; /*!< Vendor-specific subelement, valid only where
                        `vendorSpecificPresent` is `TRUE`. */
    USHORT numRepetitions; /*!< Number of times the measurement is to be repeated after the
                        first. Zero means measure once and do not repeat; 65535 means
                        repeat until cancelled, which is what
                        `wifi_cancelRMBeaconRequest()` stops. */
} wifi_BeaconRequest_t;

/**
 * @brief Beacon Report.
 *
 * This structure represents the Beacon Report frame, as defined in
 * 802.11-2016 section 9.4.2.22.7. One instance describes one measured BSS, and the `HAL`
 * delivers them to a registered `wifi_RMBeaconReport_callback` handler.
 *
 * @see wifi_RMBeaconReport_callback
 * @see wifi_BeaconRequest_t
 */
typedef struct
{
    UCHAR opClass; /*!< Operating class the measurement was made in, which is what gives
                        `channel` its band and width. */
    UCHAR channel; /*!< Channel the reported BSS was measured on, within `opClass`. */
    ULLONG startTime; /*!< Value of the measuring STA's TSF timer when the measurement
                        started. An 8-octet TSF value, so it is comparable only against
                        other timestamps from the same STA. */
    USHORT duration; /*!< Length of the measurement, in time units of 1024 microseconds. */
    UCHAR frameInfo; /*!< Reported Frame Information field, carrying the PHY type of the
                        reported frame and whether it was a beacon, probe response or
                        measurement pilot. */
    UCHAR rcpi; /*!< Received Channel Power Indicator for the reported frame, as the 802.11
                        RCPI encoding: an unsigned value in 0.5dBm steps from -110dBm,
                        rather than a signed dBm figure. */
    UCHAR rsni; /*!< Received Signal to Noise Indicator for the reported frame, as the
                        802.11 RSNI encoding: an unsigned value in 0.5dB steps from
                        -10dB. */
    bssid_t bssid; /*!< Address of the BSS this report describes, as the six raw octets of `bssid_t`
                        rather than text. It is the measured BSS rather than the one that requested
                        the measurement. */
    UCHAR antenna; /*!< Antenna ID the measurement was taken on. Zero means the antenna is
                        unknown or not reported. */
    UINT tsf; /*!< TSF value of the reported BSS at the time of measurement. Held in a
                        `UINT`, so it carries the lower half of an 8-octet TSF rather than
                        the whole of one, unlike `startTime`. */
    BOOL wideBandWidthChannelPresent; /*!< `TRUE` if `wideBandwidthChannel` carries wide
                        bandwidth channel information for the reported BSS. */
    wifi_WideBWChannel_t wideBandwidthChannel; /*!< Wide bandwidth channel information,
                        valid only where `wideBandWidthChannelPresent` is `TRUE`. */
    USHORT numRepetitions; /*!< Number of repetitions remaining for the measurement this
                        report answers, echoing the `numRepetitions` of the
                        `wifi_BeaconRequest_t` that started it. */
} wifi_BeaconReport_t;
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */

/**
 * @brief Gets radio channel statistics.
 * 
 * This function retrieves the utilization status of the specified radio 
 * channels. The `input_output_channelStats_array` is used to specify the 
 * channels of interest and to store the retrieved statistics.
 * 
 * When `array_size` is 0, the function returns ONCHAN statistics in a single 
 * `wifi_channelStats_t` element. Otherwise, the `input_output_channelStats_array` 
 * should be pre-filled with the channel numbers to query, and the function will 
 * fill the corresponding elements with the channel statistics.
 * 
 * This is the survey call a steering or channel-selection caller uses to compare
 * candidate channels: the busy-time accumulators in each element are what a caller
 * differences to obtain channel occupancy.
 *
 * @param[in] radioIndex              Index of the radio to survey. The indices this
 *                                    interface defines are `RADIO_INDEX_1` to
 *                                    `RADIO_INDEX_3` in `wifi_hal_generic.h`, bounded by
 *                                    `MAX_NUM_RADIOS`, which is 2 or 3 depending on
 *                                    whether `WIFI_HAL_VERSION_3` is defined.
 * @param[in,out] input_output_channelStats_array Array of `wifi_channelStats_t` that
 *                                    carries the request in and the result out. The caller
 *                                    allocates and owns it, per `Memory Model` in
 *                                    `docs/pages/halSpec.md`. Whether the implementation
 *                                    retains the pointer beyond the call is not specified
 *                                    by this interface, so the caller should keep the
 *                                    array allocated and unmoved while the `HAL` remains
 *                                    initialised. On the way in the caller sets only
 *                                    `ch_number` in each of the first `array_size`
 *                                    elements; on the way out the `HAL` writes every
 *                                    other member of those elements and leaves
 *                                    `ch_number` as the caller set it. It must hold at
 *                                    least `array_size` elements, and at least one
 *                                    element even when `array_size` is 0.
 * @param[in] array_size              Number of elements the `HAL` may read and write,
 *                                    which bounds `input_output_channelStats_array`. A
 *                                    value of 0 selects the ONCHAN-only form, in which the
 *                                    `HAL` ignores the input channel numbers and writes
 *                                    one element describing the radio's current operating
 *                                    channel.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the first `array_size` elements have been written, or the single ONCHAN
 *       element where `array_size` is 0. On failure this interface does not state whether
 *       any element was written, so a caller must treat the whole array as undefined
 *       rather than reading the elements it believes were reached.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The requested statistics were written.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments, discard the array contents, and treat channel
 *                          occupancy as unknown rather than as zero, since a zeroed
 *                          accumulator is indistinguishable from an idle channel.
 *
 * @warning An `array_size` of 0 still causes one element to be written, so passing 0 with
 *          a NULL or zero-length array overruns the caller's storage.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, so a
 *       caller must not assume it triggers a fresh off-channel survey; the interface does
 *       not state how recent the returned statistics are.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_channelStats_t
 */
INT wifi_getRadioChannelStats(INT radioIndex, wifi_channelStats_t *input_output_channelStats_array, INT array_size);

/**
 * @brief Gets the per-rate receive statistics for an associated device.
 *
 * This function retrieves the receive statistics for an associated client
 * on the specified radio, broken down by rate. The statistics are returned
 * in the `stats_array`, which is allocated by the HAL and should be freed
 * by the caller.
 * 
 * One array element is returned per rate the client has received at, identified by the
 * element's `nss`, `mcs` and `bw` members. `handle` lets a caller tell a counter that grew
 * from one that restarted because the client re-associated.
 *
 * @param[in] radioIndex          Index of the radio the client is associated on. The
 *                                indices this interface defines are `RADIO_INDEX_1` to
 *                                `RADIO_INDEX_3` in `wifi_hal_generic.h`, bounded by
 *                                `MAX_NUM_RADIOS`.
 * @param[in] clientMacAddress    Pointer to the six-octet MAC address of the client to
 *                                report on. The caller allocates and owns it and the `HAL`
 *                                reads it during the call. One address is passed, not an
 *                                array.
 * @param[out] stats_array         Pointer to a pointer to an array of
 *                                 `wifi_associated_dev_rate_info_rx_stats_t` structures to
 *                                 store the receive statistics. The array is allocated by
 *                                 the HAL layer and should be freed by the caller. That is
 *                                 an explicit exception to `Memory Model` in
 *                                 `docs/pages/halSpec.md`, under which memory the `HAL`
 *                                 creates stays `HAL`-owned, and the exception applies
 *                                 only to this array. On success it holds
 *                                 `*output_array_size` elements. This interface does not
 *                                 name the function the `HAL` allocated it with, so the
 *                                 matching release function is a platform convention
 *                                 rather than something established here.
 * @param[out] output_array_size   Pointer to a variable to store the size of the returned
 *                                 array, in elements. The caller allocates and owns the
 *                                 variable. It bounds `*stats_array`, so a caller must
 *                                 read it before indexing the array. A value of zero means
 *                                 the client has received at no rate the `HAL` tracks, and
 *                                 this interface does not state whether `*stats_array` is
 *                                 a usable pointer in that case.
 * @param[out] handle              Pointer to a status validation handle used to determine
 *                                 reconnections. This handle is incremented for every
 *                                 association, so a caller that stores it alongside a
 *                                 previous sample can detect that the client
 *                                 re-associated and that the counters therefore restarted.
 *                                 The caller allocates and owns the variable. This
 *                                 interface states neither the value the handle starts
 *                                 from nor how long it stays valid, so a caller should
 *                                 compare it only against its own previous reading for the
 *                                 same client and must not treat it as an identifier it
 *                                 can pass anywhere else.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*stats_array` points to an array of `*output_array_size` elements that
 *       the caller is responsible for freeing, and `*handle` carries the association
 *       generation the counters belong to. On failure this interface does not state whether
 *       any output was written, so a caller must treat all three as undefined and must not
 *       release `*stats_array`.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The statistics were retrieved and the outputs written.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments, release nothing, and retry rather than treating
 *                          the result as a client with no receive traffic.
 *
 * @warning The `HAL` allocates a fresh array on each successful call and nothing in this
 *          interface releases a previous one, so a caller that samples repeatedly must free
 *          every array it is given.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_associated_dev_rate_info_rx_stats_t
 * @see wifi_getApAssociatedDeviceTxStatsResult
 */
INT wifi_getApAssociatedDeviceRxStatsResult(INT radioIndex, mac_address_t *clientMacAddress, wifi_associated_dev_rate_info_rx_stats_t **stats_array, UINT *output_array_size, ULLONG *handle);

/**
 * @brief Gets the per-rate transmit statistics for an associated device.
 *
 * This function retrieves the transmit statistics for an associated client
 * on the specified radio, broken down by rate. The statistics are returned
 * in the `stats_array`, which is allocated by the HAL and should be freed
 * by the caller.
 * 
 * One array element is returned per rate the client has been transmitted to at, identified
 * by the element's `nss`, `mcs` and `bw` members. This is the transmit counterpart of
 * `wifi_getApAssociatedDeviceRxStatsResult()` and follows the same ownership and handle
 * rules.
 *
 * @param[in] radioIndex          Index of the radio the client is associated on. The
 *                                indices this interface defines are `RADIO_INDEX_1` to
 *                                `RADIO_INDEX_3` in `wifi_hal_generic.h`, bounded by
 *                                `MAX_NUM_RADIOS`.
 * @param[in] clientMacAddress    Pointer to the six-octet MAC address of the client to
 *                                report on. The caller allocates and owns it and the `HAL`
 *                                reads it during the call.
 * @param[out] stats_array         Pointer to a pointer to an array of
 *                                 `wifi_associated_dev_rate_info_tx_stats_t` structures to
 *                                 store the transmit statistics. The array is allocated by
 *                                 the HAL layer and should be freed by the caller, which
 *                                 is an explicit exception to `Memory Model` in
 *                                 `docs/pages/halSpec.md` applying only to this array. On
 *                                 success it holds `*output_array_size` elements, and this
 *                                 interface does not name the matching release function.
 * @param[out] output_array_size   Pointer to a variable to store the size of the returned
 *                                 array, in elements. The caller allocates and owns it. It
 *                                 bounds `*stats_array`, so a caller must read it before
 *                                 indexing the array. Zero means the client has been
 *                                 transmitted to at no rate the `HAL` tracks.
 * @param[out] handle              Pointer to a status validation handle used to determine
 *                                 reconnections. This handle is incremented for every
 *                                 association, so a caller can detect that the counters
 *                                 restarted. The caller allocates and owns the variable.
 *                                 This interface states neither its starting value nor its
 *                                 lifetime, so it should be compared only against the
 *                                 caller's own previous reading for the same client.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*stats_array` points to an array of `*output_array_size` elements that
 *       the caller is responsible for freeing, and `*handle` carries the association
 *       generation the counters belong to. On failure all three outputs must be treated as
 *       undefined and `*stats_array` must not be released.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The statistics were retrieved and the outputs written.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments, release nothing, and retry rather than treating
 *                          the result as a client with no transmit traffic.
 *
 * @warning The `HAL` allocates a fresh array on each successful call and nothing in this
 *          interface releases a previous one, so a caller that samples repeatedly must free
 *          every array it is given.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_associated_dev_rate_info_tx_stats_t
 * @see wifi_getApAssociatedDeviceRxStatsResult
 */
INT wifi_getApAssociatedDeviceTxStatsResult(INT radioIndex, mac_address_t *clientMacAddress, wifi_associated_dev_rate_info_tx_stats_t **stats_array, UINT *output_array_size, ULLONG *handle);

/**
 * @brief Gets the TID statistics for an associated device.
 *
 * This function retrieves the TID (Traffic Identifier) statistics for an
 * associated client on the specified radio.
 * 
 * The result covers all sixteen traffic identifiers in one fixed-size structure, so unlike
 * the per-rate statistics calls it allocates nothing and returns no element count.
 *
 * @param[in] radioIndex        Index of the radio the client is associated on. The indices
 *                              this interface defines are `RADIO_INDEX_1` to
 *                              `RADIO_INDEX_3` in `wifi_hal_generic.h`, bounded by
 *                              `MAX_NUM_RADIOS`.
 * @param[in] clientMacAddress  Pointer to the six-octet MAC address of the client to
 *                              report on. The caller allocates and owns it and the `HAL`
 *                              reads it during the call.
 * @param[out] tid_stats        Pointer to a `wifi_associated_dev_tid_stats_t` structure to
 *                              store the TID statistics. The caller allocates and owns the
 *                              storage, per `Memory Model` in `docs/pages/halSpec.md`.
 *                              The `HAL` writes it during the call, and whether the
 *                              implementation retains the pointer afterwards is not
 *                              specified by this interface, so the caller should keep the
 *                              structure allocated and unmoved while the `HAL` remains
 *                              initialised. One structure is written, not an array, and
 *                              it always carries all sixteen TID entries indexed by TID.
 * @param[in] handle           Status validation handle used to determine reconnections,
 *                             incremented for every association. The caller allocates and
 *                             owns the variable.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success every element of `tid_stats->tid_array` has been written, with a zero
 *       `num_msdus` for a TID that carried no traffic. On failure the contents are
 *       undefined and must not be read.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The statistics were retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments and treat the per-TID latencies as unknown rather
 *                          than as zero.
 *
 * @note This interface documents `handle` as an input here and on
 *       `wifi_getApAssociatedDeviceStats()`, but as an output on
 *       `wifi_getApAssociatedDeviceRxStatsResult()` and
 *       `wifi_getApAssociatedDeviceTxStatsResult()`, while declaring it as `ULLONG *` in
 *       all four. The direction is therefore not consistently established by this
 *       interface, so a caller should initialise the variable before the call and re-read
 *       it afterwards rather than relying on either behaviour.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_associated_dev_tid_stats_t
 * @see wifi_associated_dev_tid_entry_t
 */
INT wifi_getApAssociatedDeviceTidStatsResult(INT radioIndex, mac_address_t *clientMacAddress, wifi_associated_dev_tid_stats_t *tid_stats, ULLONG *handle);

/**
 * @brief Gets the statistics for an associated device.
 *
 * This function retrieves the statistics for an associated client on the
 * specified Access Point.
 * 
 * The counters are aggregates over all rates, which makes this the cheap summary call: a
 * caller that needs the per-rate breakdown uses
 * `wifi_getApAssociatedDeviceRxStatsResult()` or its transmit counterpart instead. This is
 * also the only call in this header that is scoped by AP rather than by radio.
 *
 * @param[in] apIndex              Index of the AP the client is associated with. The
 *                                 indices this interface defines are `AP_INDEX_1` to
 *                                 `AP_INDEX_24` in `wifi_hal_generic.h`.
 * @param[in] clientMacAddress     Pointer to the six-octet MAC address of the client to
 *                                 report on. The caller allocates and owns it and the
 *                                 `HAL` reads it during the call.
 * @param[out] associated_dev_stats Pointer to a `wifi_associated_dev_stats_t` structure to
 *                                   store the device statistics. The caller allocates and
 *                                   owns the storage, per `Memory Model` in
 *                                   `docs/pages/halSpec.md`. The `HAL` writes it during
 *                                   the call, and whether the implementation retains the
 *                                   pointer afterwards is not specified by this
 *                                   interface, so the caller should keep the structure
 *                                   allocated and unmoved while the `HAL` remains
 *                                   initialised. One structure is written, not an array.
 * @param[in] handle               Status validation handle used to determine
 *                                 reconnections. This handle is incremented for
 *                                 every association. The caller allocates and owns the
 *                                 variable.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success every member of `*associated_dev_stats` has been written, including
 *       both RSSI snapshots. On failure the contents are undefined and must not be read.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The statistics were retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments and treat the client's traffic and signal level as
 *                          unknown rather than as zero.
 *
 * @note This interface documents `handle` as an input here and on
 *       `wifi_getApAssociatedDeviceTidStatsResult()`, but as an output on the two per-rate
 *       statistics calls, while declaring it as `ULLONG *` in all four, so the direction is
 *       not consistently established.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_associated_dev_stats_t
 * @see wifi_getApAssociatedDeviceRxStatsResult
 */
INT wifi_getApAssociatedDeviceStats(INT apIndex, mac_address_t *clientMacAddress, wifi_associated_dev_stats_t *associated_dev_stats, ULLONG *handle);

/**
 * @brief Gets the radio index associated with an SSID.
 *
 * This function retrieves the index of the radio associated with the
 * specified SSID entry.
 * 
 * It is the lookup that lets a caller holding an SSID index address the radio-scoped calls
 * in this header, such as `wifi_getRadioChannelStats()`, whose `radioIndex` argument this
 * supplies.
 *
 * @param[in] ssidIndex  Index of the SSID entry to resolve. This interface does not define
 *                       a separate SSID index space, and the indices it defines for the
 *                       equivalent VAP-scoped arguments are `AP_INDEX_1` to `AP_INDEX_24`
 *                       in `wifi_hal_generic.h`.
 * @param[out] radioIndex Pointer to a variable to store the radio index. The caller
 *                       allocates and owns the variable; the `HAL` writes it during the
 *                       call. On success it holds one of the radio indices this interface
 *                       defines, `RADIO_INDEX_1` to `RADIO_INDEX_3`, bounded by
 *                       `MAX_NUM_RADIOS`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*radioIndex` holds the index of the radio the SSID entry belongs to.
 *       On failure it must be treated as undefined and must not be used to address another
 *       call.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The radio index was written.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          `ssidIndex` and must not fall back to a default radio index,
 *                          since addressing the wrong radio would apply a subsequent call
 *                          to the wrong hardware.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_applySSIDSettings
 */
INT wifi_getSSIDRadioIndex(INT ssidIndex, INT *radioIndex);

/**
 * @brief Applies SSID and AP settings to the hardware.
 *
 * This function applies the SSID and AP (in the case of Access Point
 * devices) settings to the hardware.
 * 
 * It is the commit step for configuration a caller has already staged through the setter
 * calls elsewhere in this interface: until it is called, those settings need not be in
 * force on the hardware.
 *
 * @param[in] ssidIndex SSID index whose staged settings are to be applied. This interface
 *                      does not define a separate SSID index space, and the indices it
 *                      defines for the equivalent VAP-scoped arguments are `AP_INDEX_1` to
 *                      `AP_INDEX_24` in `wifi_hal_generic.h`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the `HAL` has accepted the request to apply the settings for
 *       `ssidIndex`. This interface does not state whether the hardware has finished
 *       applying them when the call returns, nor whether applying them interrupts
 *       associated clients, so a caller must not read a successful return as evidence that
 *       the new configuration is already in force.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The `HAL` accepted the request for `ssidIndex`.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it does not state whether
 *                          a failure leaves the previous configuration in force or the
 *                          hardware partly reconfigured, so the caller should read the
 *                          affected settings back rather than assuming either.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, and
 *       this interface establishes no completion notification for it.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_getSSIDRadioIndex
 */
INT wifi_applySSIDSettings(INT ssidIndex);

/**
 * @brief Starts a neighbor scan.
 *
 * The call asks the radio behind an AP to survey the air for neighbouring BSSs, visiting
 * each channel in turn and dwelling on it for `dwell_time`. It is how a caller populates
 * its own picture of the RF neighbourhood before making a channel or steering decision.
 *
 * @param[in] apIndex    Index of the AP whose radio performs the scan. The indices this
 *                       interface defines are `AP_INDEX_1` to `AP_INDEX_24` in
 *                       `wifi_hal_generic.h`.
 * @param[in] scan_mode  Scan mode, as one of the `wifi_neighborScanMode_t` values declared
 *                       in `wifi_hal_ap.h`: none, full, on-channel, off-channel, survey or
 *                       selected-channels. Only the selected-channels mode gives `chan_num`
 *                       and `chan_list` a meaning; this interface does not state what the
 *                       `HAL` does with a channel list supplied alongside any other mode.
 * @param[in] dwell_time Time the radio spends on each channel in the hopping sequence, in
 *                       milliseconds. This interface states neither a legal range nor the
 *                       meaning of zero, so a caller should supply a positive value and
 *                       must not read zero as "use a platform default".
 * @param[in] chan_num   Number of entries in `chan_list`, which bounds that array. Zero
 *                       means no channel list is supplied, in which case the scope of the
 *                       scan is whatever `scan_mode` alone implies.
 * @param[in] chan_list  Array of channel numbers to scan, holding `chan_num` entries. The
 *                       caller allocates and owns it, per `Memory Model` in
 *                       `docs/pages/halSpec.md`; the `HAL` reads it during the call, and
 *                       whether the implementation retains the pointer afterwards is not
 *                       specified by this interface, so the caller should keep the array
 *                       allocated and unmoved while the `HAL` remains initialised. This
 *                       interface does not carry a band alongside the channel numbers, so
 *                       they are interpreted in the band of the radio behind `apIndex`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the `HAL` has accepted the scan request for `apIndex`. The scan is not
 *       complete when the call returns.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The `HAL` accepted the scan request.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments and retry rather than waiting for results that
 *                          will not arrive.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, so it
 *       returns before the scan has run and does not itself wait out `dwell_time`.
 * @note This interface does not establish how the completion of a scan started here is
 *       signalled, nor which call returns its results, so a caller must not assume a
 *       notification and must not infer a completion time from `dwell_time` and
 *       `chan_num`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. This interface does not state what happens when a scan is
 *       started while one is already running on the same radio.
 *
 * @see wifi_setApScanFilter
 */
INT wifi_startNeighborScan(INT apIndex, wifi_neighborScanMode_t scan_mode, INT dwell_time, UINT chan_num, UINT *chan_list);

/**
 * @brief Sets the CSA deauthentication mode for an AP.
 *
 * This function sets the Channel Switch Announcement (CSA) deauthentication
 * mode for the specified Access Point. This mode determines how the AP will
 * deauthenticate clients before switching to a new channel, in the event that
 * some clients do not support or react to CSA.
 *
 * The setting is a policy that persists for subsequent channel switches on that AP; the
 * call itself neither announces a channel switch nor deauthenticates anything.
 *
 * @param[in] apIndex The index of the Access Point array. The indices this interface
 *                    defines are `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`.
 * @param[in] mode    Enum value indicating the deauthentication mode:
 *                    0 = none,
 *                    1 = unicast,
 *                    2 = broadcast.
 *                    Those three are the whole legal range; this interface defines no
 *                    enumeration type for them, so a caller passes the bare value.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the mode is in force for subsequent channel switches on `apIndex`. This
 *       interface provides no getter for the setting, so a caller that needs to know the
 *       current mode must track what it last set.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The mode was set for `apIndex`.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it does not state whether
 *                          a failure leaves the previous mode in force, so the caller
 *                          should re-apply the mode it wants rather than assuming either
 *                          outcome.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_setApCsaDeauth(INT apIndex, INT mode);

/**
 * @brief Enables or disables the scan filter in the driver.
 *
 * When the filter is enabled the driver reports only the BSSs whose ESSID matches, which
 * narrows what a subsequent `wifi_startNeighborScan()` returns. Two values make up the
 * setting: `mode`, which turns the filter on or off, and `essid`, which names what to
 * match.
 *
 * The interface describes the two forms of the call in terms of both arguments together.
 * When `essid` is blank (`apIndex` is -1), the configured SSID on the
 * interface is used. When `essid` is not empty (`apIndex` is 0 to 15), the
 * filter will apply to the provided ESSID.
 *
 * @param[in] apIndex The index of the Access Point array. This call is the one place in
 *                    this header where a negative index is meaningful: -1 selects the
 *                    blank-ESSID form described above, and the positive range the
 *                    interface states for this argument is 0 to 15, which is narrower than
 *                    the `AP_INDEX_1` to `AP_INDEX_24` range `wifi_hal_generic.h` defines.
 * @param[in] mode    Whether the filter is disabled or enabled. This interface does not
 *                    define an enumeration for it, and treats it as the boolean the
 *                    description calls out, so a caller should pass 0 to disable and 1 to
 *                    enable.
 * @param[in] essid   The ESSID to filter on, as a caller-allocated string. The caller
 *                    passes NUL-terminated text, because this interface carries no length
 *                    parameter for the argument. The caller owns the storage, per `Memory
 *                    Model` in `docs/pages/halSpec.md`; the `HAL` reads it during the
 *                    call, and whether the implementation retains the pointer afterwards
 *                    is not specified by this interface, so the caller should keep it
 *                    allocated and unmoved while the `HAL` remains initialised. A blank
 *                    string selects the form that filters on the interface's own
 *                    configured SSID. This interface states no maximum length for the
 *                    argument. The `ssid_t` type it uses for an SSID elsewhere is 32
 *                    octets with no room reserved for a terminator, so 32 characters is
 *                    the longest SSID the interface can represent at all; a caller should
 *                    stay within that and must not assume a longer argument is accepted.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the filter setting is in force for subsequent scans on that interface.
 *       This interface provides no getter for it, so a caller that needs to know the
 *       current setting must track what it last set, and must disable the filter
 *       explicitly rather than expecting a scan to clear it.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The filter setting was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments and must not start a scan expecting the filter to
 *                          be in force, since a failed call leaves the previous setting in
 *                          an unspecified state.
 *
 * @warning A filter left enabled suppresses non-matching BSSs from every later scan on
 *          that interface, so a caller that scans for neighbours after filtering will see
 *          an incomplete neighbourhood rather than an empty one.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_startNeighborScan
 */
INT wifi_setApScanFilter(INT apIndex, INT mode, CHAR *essid);

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Adds a steering group.
 *
 * This function adds a steering group, which defines a group of Access Points
 * (APs) that can have steering done between them.
 *
 * The group is the unit every other steering call is addressed to: a client can only be
 * configured, measured, disconnected or reported on within a group that already exists.
 * Each element of `cfgArray` also sets the sampling intervals that govern how often the
 * channel-utilization and client-activity events fire for its AP.
 *
 * @param[in] steeringGroupIndex The index of the steering group to create or replace. This
 *                               interface states no range for it and defines no maximum
 *                               number of groups, so a caller should use the indices its
 *                               own configuration allocates. Note that the client calls in
 *                               this header spell the same argument `steeringgroupIndex`,
 *                               with a lower-case "g".
 * @param[in] numElements        The number of elements in the `cfgArray`, which bounds that
 *                               array. This interface does not state whether zero is legal
 *                               or what it would mean.
 * @param[in] cfgArray           The array of `wifi_steering_apConfig_t`
 *                               structures, containing the settings for each
 *                               AP in the group. It holds `numElements` entries. The caller
 *                               allocates and owns the array, per `Memory Model` in
 *                               `docs/pages/halSpec.md`, and this interface does not state
 *                               whether the `HAL` copies the contents during the call or
 *                               retains the pointer, so a caller must keep the array
 *                               allocated and unmodified for as long as the group exists
 *                               rather than freeing it when the call returns.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the group exists and the client calls in this header may be addressed
 *       to `steeringGroupIndex`. This interface does not state whether calling it again
 *       with an existing index replaces the group or is rejected, nor does it provide a
 *       call that removes a group.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK If successful.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note `RETURN_OK` and `RETURN_ERR`, defined in `wifi_hal_generic.h`, carry the same
 *       values as the `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR` codes the rest of this
 *       interface uses for the same two outcomes. The two codes above are the whole
 *       return-code contract of this call, per `Internal Error Handling` in
 *       `docs/pages/halSpec.md`. On failure the caller should validate its arguments and
 *       must not address client calls to the group, since this interface does not state
 *       whether a partly configured group is left behind.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @note This declaration is compiled only where `WIFI_HAL_VERSION_3_PHASE2` is defined.
 *
 * @see wifi_steering_apConfig_t
 * @see wifi_steering_clientSet
 * @see wifi_steering_eventRegister
 */
INT wifi_steering_setGroup(UINT steeringGroupIndex, UINT numElements, wifi_steering_apConfig_t *cfgArray);
#endif

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
 
/**
 * @brief Wi-Fi steering types.
 */
typedef enum
{
    pre_assoc_probe_block, /**< Pre-association probe block. */
    pre_assoc_assoc_block, /**< Pre-association association block. */
    post_assoc_idle_80211v, /**< Post-association idle 802.11v steering. */
    post_assoc_idle_kick_mac, /**< Post-association idle kick MAC steering. */
    post_assoc_active_80211v, /**< Post-association active 802.11v steering. */
    post_assoc_active_kickmac, /**< Post-association active kick MAC steering. */
} wifi_steer_type_t;

/**
 * @brief Wi-Fi steering matching condition.
 */
typedef unsigned int wifi_steer_matching_condition_t;

/**
 * @brief Wi-Fi steering trigger data.
 */
typedef struct
{
    CHAR *module; /*!< Name of the module that raised the steering trigger. The member is a bare
                       `CHAR *`: this interface states neither the representation, whether the bytes
                       are `NUL`-terminated, how many of them are readable, which side owns the
                       storage nor how long it stays valid. A handler reached through
                       `wifi_steerTriggered_callback` must therefore not run an unbounded string
                       function over the pointer, must not free it, and cannot bound a copy of the
                       name from this interface - it has to take the length from the platform
                       implementer. */
    mac_address_t sta_mac; /*!< Address of the station being steered, as the six raw octets of
                                `mac_address_t` rather than text. `src_bss` and `dst_bss` below are
                                the BSSs it is steered from and to. */
    mac_address_t src_bss; /*!< Address of the BSS the station is being steered away from, in the
                                same six raw octets. This interface does not state what the member
                                holds for a pre-association steer, where there is no serving BSS. */
    mac_address_t dst_bss; /*!< Address of the BSS the station is being steered towards, in the same
                                six raw octets. This interface does not state what the member holds
                                where the trigger names no target. */
    wifi_steer_type_t type; /*!< Which steering action was triggered, as one of the
                                 `wifi_steer_type_t` enumerators - probe or association blocking
                                 before association, and 802.11v or kick-MAC steering of an idle or
                                 active station afterwards. That enumeration assigns no values, so a
                                 caller must compare against the enumerators. */
    wifi_steer_matching_condition_t cond; /*!< Condition that matched to raise the steering trigger.
                                               `wifi_steer_matching_condition_t` is a typedef of
                                               `unsigned int` and this interface enumerates no
                                               values for it, so a caller cannot decide from this
                                               interface which condition a given value denotes;
                                               `type` above names the steering type as a
                                               `wifi_steer_type_t`. */
} wifi_steer_trigger_data_t;
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
 
/**
 * @brief Wi-Fi steering triggered callback function.
 *
 * A caller installs a handler of this type with
 * `wifi_steerTriggered_callback_register()`. The `HAL` then invokes it whenever a steering
 * decision is triggered for a client, passing the steering type that fired and the source
 * and destination BSSs involved.
 *
 * @param[in] apIndex Index of the AP the trigger relates to. The indices this interface
 *                    defines are `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`.
 * @param[in] data    Pointer to the steering trigger data. This interface does not state
 *                    which component owns the storage, how long it stays valid, or how many
 *                    bytes are readable through it, so the client must not assume either
 *                    that it survives the call or that it may be released. The one
 *                    established obligation is the client copy duty in
 *                    `Asynchronous Notification Model` in `docs/pages/halSpec.md`: copy what
 *                    is needed before returning, and neither retain nor free the pointer.
 *                    That applies to the `module` string the structure carries as much as to
 *                    the structure itself, and the copy of that string must be bounded by a
 *                    length obtained from the implementation rather than by an unbounded
 *                    string function, because no extent is stated for it.
 *
 * @returns The status of the operation, returned by the handler to the `HAL`.
 * @retval WIFI_HAL_SUCCESS The handler accepted the trigger.
 * @retval WIFI_HAL_ERROR   The handler could not process the trigger.
 *
 * @note This interface does not state how the `HAL` acts on a non-success return, so a
 *       handler must not rely on the trigger being retried or re-delivered, and must not
 *       use the return value to veto the steering decision.
 * @note The handler must not suspend and must not invoke any blocking system calls; it
 *       should do no more than pass the event to its own event handler task, per
 *       `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe and may enter the handler on a `HAL`
 *       thread, so the handler must serialise its own access to caller state; see
 *       `Threading Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_steerTriggered_callback_register
 * @see wifi_steer_trigger_data_t
 */
typedef INT (*wifi_steerTriggered_callback)(INT apIndex, wifi_steer_trigger_data_t *data);
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Registers a callback function for steering triggered events.
 *
 * After registration the `HAL` reports each steering trigger through the supplied handler,
 * so a caller can observe steering decisions without polling. The `module` argument names
 * the registering component, which is what the `module` member of
 * `wifi_steer_trigger_data_t` carries back on each event.
 *
 * @param[in] callback_proc Pointer to the callback function to register, of type
 *                          `wifi_steerTriggered_callback`. The `HAL` keeps this
 *                          function pointer after the call returns, since it invokes
 *                          the handler later, so the function must remain callable for
 *                          as long as steering triggers are wanted. This interface
 *                          declares no call that removes this handler and states no
 *                          end to a registration, so it does not establish when the
 *                          `HAL` stops using the pointer. The effect of passing NULL
 *                          is not specified by this interface.
 * @param[in] module        Name identifying the registering component, as a
 *                          caller-allocated string; the caller passes NUL-terminated
 *                          text, because this interface carries no length parameter for
 *                          the argument. This interface defines neither the set of legal
 *                          names, nor a maximum length, nor whether the `HAL` copies the
 *                          string or retains the pointer, so a caller must keep the
 *                          storage allocated and unmodified for as long as the handler
 *                          stays registered. It is the only registrar in this interface
 *                          that takes such an argument.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. The effect of registering beforehand is not specified
 *      by this interface.
 * @post The handler is installed and is invoked on each subsequent steering trigger.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes on each
 *             steering trigger, and keeps that function pointer - and, since the
 *             interface does not state that the `HAL` copies it, possibly the `module`
 *             string - after this call returns. This interface states no other effect.
 *
 * @note This call returns no status, so a caller cannot tell from it whether the
 *       registration succeeded and must not treat its return as a confirmation; observing
 *       a first callback is the only evidence this interface offers.
 * @note This interface defines a single registration point and describes neither a list of
 *       handlers nor a way to unregister. It does not state whether a later registration
 *       replaces an earlier one, adds to it or is rejected, so a caller should register
 *       once and must not assume a handler can be removed.
 * @note The registration call itself is synchronous and does not block, per
 *       `Blocking calls` in `docs/pages/halSpec.md`; delivery of `wifi_steerTriggered_callback` is
 *       asynchronous.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_steerTriggered_callback
 * @see wifi_steer_trigger_data_t
 */
void wifi_steerTriggered_callback_register(wifi_steerTriggered_callback callback_proc, CHAR *module);
/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

/**
 * @brief Wi-Fi steering event callback function.
 *
 * A caller installs a handler of this type with `wifi_steering_eventRegister()` and removes
 * it with `wifi_steering_eventUnregister()`. The `HAL` then delivers every steering event
 * through it: probe requests, client connect and disconnect, activity changes, channel
 * utilization, RSSI crossings, the reply to `wifi_steering_clientMeasure()`, and auth
 * failures.
 *
 * @param[in] steeringgroupIndex Index of the steering group the event belongs to, as passed
 *                               to `wifi_steering_setGroup()` - which spells the same
 *                               argument `steeringGroupIndex`, with a capital "G".
 * @param[in] event              Pointer to the steering event. This interface does not
 *                               state which component owns the storage or how long it stays
 *                               valid, so the client must not assume it survives the call
 *                               or that it may be released. The established obligation is
 *                               the client copy duty in `Asynchronous Notification Model`
 *                               in `docs/pages/halSpec.md`: copy before returning, and
 *                               neither retain nor free the pointer. The handler must read the `type`
 *                               member first and then only the union member `type` selects,
 *                               as `wifi_steering_event_t` sets out.
 *
 * @note This handler returns `void`, so a handler cannot report a processing failure back
 *       to the `HAL` and the `HAL` has no way to learn that an event was not consumed.
 * @note The handler must not suspend and must not invoke any blocking system calls; it
 *       should do no more than pass the event to its own event handler task, per
 *       `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe and may enter the handler on a `HAL`
 *       thread, so the handler must serialise its own access to caller state; see
 *       `Threading Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_steering_eventRegister
 * @see wifi_steering_eventUnregister
 * @see wifi_steering_event_t
 */
typedef void (*wifi_steering_eventCB_t)(UINT steeringgroupIndex, wifi_steering_event_t *event);
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */

/**
 * @brief Registers for steering event callbacks.
 *
 * This is the single delivery point for every steering event across every group: the
 * handler is not registered per group, and the group each event came from is carried in the
 * handler's own `steeringgroupIndex` argument. A caller that wants events must register
 * before the events it cares about occur, since this interface offers no way to retrieve
 * past ones.
 *
 * @param[in] event_cb Pointer to the callback function to register, of type
 *                     `wifi_steering_eventCB_t`. The `HAL` keeps this function pointer
 *                     after the call returns, since it invokes the handler later, so the
 *                     function must remain callable until the registration is removed
 *                     with `wifi_steering_eventUnregister()`, which is the only removal
 *                     this interface declares. The effect of passing NULL is not
 *                     specified by this interface; a caller that wants to stop
 *                     delivery should call `wifi_steering_eventUnregister()` instead.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. The effect of registering beforehand is not specified by
 *      this interface.
 * @post On success the handler is installed and receives subsequent steering events. This
 *       interface does not state whether events are delivered for groups created after the
 *       registration, nor whether a second registration replaces the first or is rejected.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK If successful.
 * @retval RETURN_ERR If any error is detected.
 *
 * @execution callback
 * @sideeffect On success, installs `event_cb` as the single delivery point for
 *             steering events across every group, and keeps that function pointer
 *             after this call returns until `wifi_steering_eventUnregister()` removes
 *             it. This interface states no other effect.
 *
 * @note `RETURN_OK` and `RETURN_ERR`, defined in `wifi_hal_generic.h`, carry the same
 *       values as the `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR` codes the rest of this
 *       interface uses for the same two outcomes, and are the whole return-code contract of
 *       this call. This interface does not state whether the handler is installed after
 *       `RETURN_ERR`, so a caller must not wait for events on the strength of a failed
 *       registration and must not assume the previous registration state is intact
 *       either. `wifi_steering_eventUnregister()` is the only call this interface offers
 *       for returning to a known state.
 * @note The registration call itself is synchronous and does not block, per
 *       `Blocking calls` in `docs/pages/halSpec.md`; delivery of `wifi_steering_eventCB_t` is
 *       asynchronous.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_steering_eventCB_t
 * @see wifi_steering_eventUnregister
 * @see wifi_steering_setGroup
 */
INT wifi_steering_eventRegister(wifi_steering_eventCB_t event_cb);

/**
 * @brief Unregisters for steering event callbacks.
 *
 * This is the teardown counterpart of `wifi_steering_eventRegister()`. It takes no
 * arguments because the interface holds a single registration rather than a list, so there
 * is no handler to name and no group to scope the removal to.
 *
 * @pre A handler must have been installed with `wifi_steering_eventRegister()`. This
 *      interface does not state what unregistering without a registration does, so a caller
 *      should not rely on it being harmless.
 * @post On success no steering events are delivered. This interface does not state whether
 *       an event already being delivered is allowed to complete, so a caller must keep the
 *       state its handler touches valid until it can be sure no handler is running.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK If successful.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note `RETURN_OK` and `RETURN_ERR` carry the same values as `WIFI_HAL_SUCCESS` and
 *       `WIFI_HAL_ERROR`, and are the whole return-code contract of this call. On
 *       `RETURN_ERR` the caller must assume the handler may still be invoked and so must
 *       not free anything the handler uses.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_steering_eventRegister
 * @see wifi_steering_eventCB_t
 */
INT wifi_steering_eventUnregister(void);

/**
 * @brief Adds or modifies client configuration for an AP.
 *
 * The configuration supplied here is what makes the AP treat one client differently from
 * the rest: it sets the RSSI ranges within which probe and auth requests are answered, and
 * the thresholds whose crossing produces a `WIFI_STEERING_EVENT_RSSI_XING` event. Until a
 * client has been set on an AP, none of that applies to it.
 *
 * @param[in] steeringgroupIndex Index of the steering group the AP belongs to, as passed to
 *                               `wifi_steering_setGroup()` - which spells the same argument
 *                               `steeringGroupIndex`, with a capital "G".
 * @param[in] apIndex            Index of the AP within the group that the configuration
 *                               applies to. The indices this interface defines are
 *                               `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`, and
 *                               the AP must be one of those named in the group's
 *                               `cfgArray`.
 * @param[in] client_mac         MAC address of the client to configure, as the six octets of
 *                               `mac_address_t` in `wifi_hal_generic.h`. Because that type
 *                               is an array, the argument is passed as a pointer to the
 *                               caller's storage: the caller owns it and the `HAL` reads it
 *                               during the call.
 * @param[in] config             Pointer to the client configuration. One structure is
 *                               passed, not an array. The caller allocates and owns it, per
 *                               `Memory Model` in `docs/pages/halSpec.md`, and this
 *                               interface does not state whether the `HAL` copies the
 *                               contents or retains the pointer, so a caller must keep the
 *                               structure allocated and unmodified while the client remains
 *                               configured.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @pre The group named by `steeringgroupIndex` must already exist, having been created by
 *      `wifi_steering_setGroup()`. This interface does not state what happens if it does
 *      not, so a caller must not rely on the group being created implicitly.
 * @post On success the configuration is in force for that client on that AP, replacing any
 *       previous configuration for the same client and AP. This interface provides no
 *       getter, so a caller that needs the current configuration must track what it set.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK If successful.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note `RETURN_OK` and `RETURN_ERR`, defined in `wifi_hal_generic.h`, carry the same
 *       values as the `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR` codes the rest of this
 *       interface uses, and are the whole return-code contract of this call. On
 *       `RETURN_ERR` the caller should validate the group, AP and pointer arguments, and
 *       must not assume the client is being steered - nor that a previous configuration for
 *       it survived, which this interface does not state.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_steering_clientConfig_t
 * @see wifi_steering_clientRemove
 * @see wifi_steering_setGroup
 */
INT wifi_steering_clientSet(UINT steeringgroupIndex, INT apIndex, mac_address_t client_mac, wifi_steering_clientConfig_t *config);

/**
 * @brief Removes client configuration from an AP.
 *
 * This is the teardown counterpart of `wifi_steering_clientSet()`. Removing the
 * configuration stops the RSSI ranges and crossing thresholds being applied to that client
 * on that AP; it does not disconnect the client, which is what
 * `wifi_steering_clientDisconnect()` does.
 *
 * @param[in] steeringgroupIndex Index of the steering group the AP belongs to.
 * @param[in] apIndex            Index of the AP the configuration is removed from. The
 *                               indices this interface defines are `AP_INDEX_1` to
 *                               `AP_INDEX_24` in `wifi_hal_generic.h`.
 * @param[in] client_mac         MAC address of the client whose configuration is removed,
 *                               as the six octets of `mac_address_t`. The caller owns the
 *                               storage and the `HAL` reads it during the call.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the client is no longer configured on that AP, and the interface's
 *       default treatment applies to it. After removal the caller may release the
 *       `wifi_steering_clientConfig_t` it passed to `wifi_steering_clientSet()`.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK If successful.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note `RETURN_OK` and `RETURN_ERR` carry the same values as `WIFI_HAL_SUCCESS` and
 *       `WIFI_HAL_ERROR`, and are the whole return-code contract of this call. This
 *       interface does not distinguish a client that was not configured in the first place
 *       from a genuine failure, so on `RETURN_ERR` the caller must not release the
 *       configuration structure and should retry the removal.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_steering_clientSet
 * @see wifi_steering_clientDisconnect
 */
INT wifi_steering_clientRemove(UINT steeringgroupIndex, INT apIndex, mac_address_t client_mac);

/**
 * @brief Initiates an instant client RSSI measurement.
 *
 * This function initiates an instant client RSSI measurement. The recommended
 * method of performing this measurement is to send five NULL Wi-Fi frames to
 * the client and average the RSSI of the ACK frames returned. This averaged
 * RSSI value should be sent back using the `WIFI_STEERING_EVENT_RSSI`
 * steering event type.
 *
 * Instant measurement improves user experience by not reacting to
 * false-positive RSSI crossings.
 *
 * @param[in] steeringgroupIndex Index of the steering group the AP belongs to.
 * @param[in] apIndex            Index of the AP the client is associated with. The indices
 *                               this interface defines are `AP_INDEX_1` to `AP_INDEX_24` in
 *                               `wifi_hal_generic.h`.
 * @param[in] client_mac         MAC address of the client to measure, as the six octets of
 *                               `mac_address_t`. The caller owns the storage and the `HAL`
 *                               reads it during the call. The same address is echoed in the
 *                               `client_mac` member of the event that reports the result.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @pre A handler must be registered with `wifi_steering_eventRegister()` before the call,
 *      because the measured value is reported only through the
 *      `WIFI_STEERING_EVENT_RSSI` event and this call returns no RSSI of its own. This
 *      interface does not state whether the measurement is attempted when no handler is
 *      registered.
 * @post On success the `HAL` has accepted the measurement request. The measurement has not
 *       been taken when the call returns: the result arrives later as a
 *       `WIFI_STEERING_EVENT_RSSI` event carrying `wifi_steering_evRssi_t`. This interface
 *       does not state a time by which that event arrives, nor whether one is delivered at
 *       all if the client does not answer, so a caller must apply its own timeout rather
 *       than waiting indefinitely.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If successful.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note `RETURN_OK` and `RETURN_ERR` carry the same values as `WIFI_HAL_SUCCESS` and
 *       `WIFI_HAL_ERROR`, and are the whole return-code contract of this call. `RETURN_OK`
 *       reports only that the request was accepted, not that a measurement succeeded; on
 *       `RETURN_ERR` the caller should validate the group, AP and client, and must not wait
 *       for an event.
 * @note This interface carries no request identifier on either the call or the event, so a
 *       caller with several measurements outstanding can only match a result to a request
 *       by the client MAC address it echoes.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, so it
 *       returns before the NULL frames have been sent and their ACKs averaged.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_steering_evRssi_t
 * @see wifi_steering_eventRegister
 */
INT wifi_steering_clientMeasure(UINT steeringgroupIndex, INT apIndex, mac_address_t client_mac);

/**
 * @brief Initiates a client disconnect.
 *
 * This is the enforcement step of a steering decision: having decided a client belongs on
 * another AP in the group, the caller drops it here so that it re-associates. The `reason`
 * code the client is given is what tells it whether to come back to this AP.
 *
 * @param[in] steeringgroupIndex Index of the steering group the AP belongs to.
 * @param[in] apIndex            Index of the AP the client is disconnected from. The
 *                               indices this interface defines are `AP_INDEX_1` to
 *                               `AP_INDEX_24` in `wifi_hal_generic.h`.
 * @param[in] client_mac         MAC address of the client to disconnect, as the six octets
 *                               of `mac_address_t`. The caller owns the storage and the
 *                               `HAL` reads it during the call.
 * @param[in] type               How to disconnect the client, as one of the
 *                               `wifi_disconnectType_t` values: disassociation,
 *                               deauthentication or reconnection.
 *                               `DISCONNECT_TYPE_UNKNOWN` is defined for reporting a
 *                               disconnect whose type is not known, so it is not a
 *                               meaningful request here.
 * @param[in] reason             Reason code to provide in the deauthentication
 *                               or disassociation frame, as an IEEE 802.11 reason code; see
 *                               the reference below. This interface does not state which
 *                               code the `HAL` uses if an unrecognised one is passed.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the `HAL` has accepted the disconnect request. This interface does not
 *       state whether the client has been dropped by the time the call returns, so a caller
 *       that must observe the outcome should watch for the
 *       `WIFI_STEERING_EVENT_CLIENT_DISCONNECT` event, whose `source` and `type` members
 *       report how the client actually left. Nothing here prevents the client
 *       re-associating with the same AP.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If successful.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note `RETURN_OK` and `RETURN_ERR` carry the same values as `WIFI_HAL_SUCCESS` and
 *       `WIFI_HAL_ERROR`, and are the whole return-code contract of this call. This
 *       interface does not distinguish a client that was not associated from a genuine
 *       failure, so on `RETURN_ERR` the caller should read the client's association state
 *       rather than inferring it, and must not repeat the disconnect blindly.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see https://supportforums.cisco.com/document/141136/80211-association-status-80211-deauth-reason-codes
 * @see wifi_disconnectType_t
 * @see wifi_steering_evDisconnect_t
 */
INT wifi_steering_clientDisconnect(UINT steeringgroupIndex, INT apIndex, mac_address_t client_mac, wifi_disconnectType_t type, UINT reason);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
*/

#ifdef WIFI_HAL_VERSION_3_PHASE2

/**
 * @brief Callback function invoked when a STA sends a BTM query.
 *
 * This callback function is invoked when a STA sends a BTM query message to a
 * VAP in the gateway. The driver will use the frame returned from this
 * function to process the response to the query. A BTM transaction is started
 * by a STA sending a query or by the AP sending an autonomous request. This
 * callback is used for the former.
 *
 * @param[in] apIndex The Access Point index.
 * @param[in] peerMac The MAC address of the peer STA the query was received
 *                    from.
 * @param[in] query A pointer to a `wifi_BTMQuery_t` structure containing the
 *                  BTM query frame received from the STA.
 * @param[in] inMemSize The size of the memory allocated by the callback for
 *                      the `request` parameter. The caller should set this to
 *                      the maximum size for the request, otherwise the
 *                      callback may drop elements or return an error.
 * @param[out] request A pointer to a `wifi_BTMRequest_t` structure to be
 *                     populated with the BTM request frame to send in
 *                     response to the query. The caller allocates the memory
 *                     for the response. The caller may free the memory when
 *                     the callback returns and the response is sent to the
 *                     STA.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The operation was successful.
 * @retval WIFI_HAL_ERROR An error occurred during the operation.
 *
 * @note On `WIFI_HAL_ERROR` this interface does not state what the driver sends to the
 *       STA, so a handler that cannot build a request should still leave `request` in a
 *       consistent state rather than partly filled.
 * @note This interface does not state which component owns `query` or how long it stays
 *       valid, so a handler must neither assume it survives the call nor release it. A
 *       handler that needs the query afterwards copies it before returning, per
 *       `Asynchronous Notification Model` in `docs/pages/halSpec.md`. `request` is the caller's own
 *       storage and the driver reads it once the handler returns.
 * @note The handler must not suspend and must not invoke any blocking system calls, per
 *       `Blocking calls` in `docs/pages/halSpec.md`. It runs on the path that answers the
 *       STA, so a slow handler delays the BTM response.
 * @note The `HAL` is expected to be thread safe and may enter the handler on a `HAL`
 *       thread, so the handler must serialise its own access to caller state; see
 *       `Threading Model` in `docs/pages/halSpec.md`.
 * @note In this build `peerMac` is a `mac_address_t`, the six raw octets; where
 *       `WIFI_HAL_VERSION_3_PHASE2` is not defined the same argument is a `CHAR *`
 *       instead, so a handler written against one form does not compile against the other.
 *
 * @see wifi_BTMQueryRequest_callback_register
 * @see wifi_BTMQuery_t
 * @see wifi_BTMRequest_t
 */
typedef INT (*wifi_BTMQueryRequest_callback)(UINT apIndex,
                                                    mac_address_t peerMac,
                                                    wifi_BTMQuery_t *query,
                                                    UINT inMemSize,
                                                    wifi_BTMRequest_t *request);

/**
 * @brief Callback function invoked when a STA responds to a BTM request.
 *
 * This callback function is invoked when a STA responds to a BTM request from
 * the gateway.
 *
 * @param[in] apIndex The Access Point index.
 * @param[in] peerMac The MAC address of the peer the response was received
 *                    from.
 * @param[in] response A pointer to a `wifi_BTMResponse_t` structure
 *                     containing the BTM response frame received from the STA.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The operation was successful.
 * @retval WIFI_HAL_ERROR An error occurred during the operation.
 *
 * @note This is a notification rather than a request for a frame, so the return value
 *       reports only whether the handler consumed it. This interface does not state how the
 *       `HAL` acts on `WIFI_HAL_ERROR`, so a handler must not rely on redelivery.
 * @note This interface does not state which component owns `response` or how long it stays
 *       valid, so a handler must neither assume it survives the call nor release it. A
 *       handler that needs it afterwards copies it before returning, per
 *       `Asynchronous Notification Model` in `docs/pages/halSpec.md`. Its `token` member is what matches the response
 *       to the request that prompted it, and its `status` member is what says whether the
 *       STA accepted the transition.
 * @note The handler must not suspend and must not invoke any blocking system calls, per
 *       `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe and may enter the handler on a `HAL`
 *       thread, so the handler must serialise its own access to caller state; see
 *       `Threading Model` in `docs/pages/halSpec.md`.
 * @note In this build `peerMac` is a `mac_address_t`, the six raw octets; where
 *       `WIFI_HAL_VERSION_3_PHASE2` is not defined the same argument is a `CHAR *`
 *       instead.
 *
 * @see wifi_BTMQueryRequest_callback_register
 * @see wifi_BTMResponse_t
 * @see wifi_setBTMRequest
 */
typedef INT (*wifi_BTMResponse_callback)(UINT apIndex,
                                            mac_address_t peerMac,
                                            wifi_BTMResponse_t *response);

#else

/**
 * @brief Callback function invoked when a STA sends a BTM query.
 *
 * This callback function is invoked when a STA sends a BTM query message to a
 * VAP in the gateway. The driver will use the frame returned from this
 * function to process the response to the query. A BTM transaction is started
 * by a STA sending a query or by the AP sending an autonomous request. This
 * callback is used for the former.
 *
 * @param[in] apIndex The Access Point index.
 * @param[in] peerMac The MAC address of the peer STA the query was received
 *                    from.
 * @param[in] query A pointer to a `wifi_BTMQuery_t` structure containing the
 *                  BTM query frame received from the STA.
 * @param[in] inMemSize The size of the memory allocated by the callback for
 *                      the `request` parameter. The caller should set this to
 *                      the maximum size for the request, otherwise the
 *                      callback may drop elements or return an error.
 * @param[out] request A pointer to a `wifi_BTMRequest_t` structure to be
 *                     populated with the BTM request frame to send in
 *                     response to the query. The caller allocates the memory
 *                     for the response. The caller may free the memory when
 *                     the callback returns and the response is sent to the
 *                     STA.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The operation was successful.
 * @retval WIFI_HAL_ERROR An error occurred during the operation.
 *
 * @note On `WIFI_HAL_ERROR` this interface does not state what the driver sends to the
 *       STA, so a handler that cannot build a request should still leave `request` in a
 *       consistent state rather than partly filled.
 * @note `query` and `peerMac` are pointers the `HAL` supplies. `Asynchronous Notification
 *       Model` in `docs/pages/halSpec.md` makes the copy the client's responsibility during
 *       the callback, so a handler that needs either afterwards must copy it before
 *       returning. That is the only statement this interface makes about their
 *       availability: it does not say which component owns the storage, how long it stays
 *       valid, or whether a handler may release it, so the handler should release neither
 *       and retain neither pointer. `query` is a `wifi_BTMQuery_t *`, so its extent follows
 *       from the type; `peerMac` is a bare `CHAR *` with no stated extent, so a copy of it
 *       cannot be bounded from this interface. `request` is the caller's own storage and
 *       the driver reads it once the handler returns.
 * @note The handler must not suspend and must not invoke any blocking system calls, per
 *       `Blocking calls` in `docs/pages/halSpec.md`. It runs on the path that answers the
 *       STA, so a slow handler delays the BTM response.
 * @note The `HAL` is expected to be thread safe and may enter the handler on a `HAL`
 *       thread, so the handler must serialise its own access to caller state; see
 *       `Threading Model` in `docs/pages/halSpec.md`.
 * @note In this build `peerMac` is a `CHAR *`. This interface does not state whether it
 *       points at a NUL-terminated MAC address string or at six raw octets, so a handler
 *       must not assume either; where `WIFI_HAL_VERSION_3_PHASE2` is defined the same
 *       argument is a `mac_address_t` and the question does not arise.
 *
 * @see wifi_BTMQueryRequest_callback_register
 * @see wifi_BTMQuery_t
 * @see wifi_BTMRequest_t
 */
typedef INT (*wifi_BTMQueryRequest_callback)(UINT apIndex,
                                                    CHAR *peerMac,
                                                    wifi_BTMQuery_t *query,
                                                    UINT inMemSize,
                                                    wifi_BTMRequest_t *request);

/**
 * @brief Callback function invoked when a STA responds to a BTM request.
 *
 * This callback function is invoked when a STA responds to a BTM request from
 * the gateway.
 *
 * @param[in] apIndex The Access Point index.
 * @param[in] peerMac The MAC address of the peer the response was received
 * from.
 * @param[in] response A pointer to a `wifi_BTMResponse_t` structure
 *                     containing the BTM response frame received from the STA.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The operation was successful.
 * @retval WIFI_HAL_ERROR An error occurred during the operation.
 *
 * @note This is a notification rather than a request for a frame, so the return value
 *       reports only whether the handler consumed it. This interface does not state how the
 *       `HAL` acts on `WIFI_HAL_ERROR`, so a handler must not rely on redelivery.
 * @note `response` and `peerMac` are pointers the `HAL` supplies. `Asynchronous
 *       Notification Model` in `docs/pages/halSpec.md` makes the copy the client's
 *       responsibility during the callback, so a handler that needs either afterwards must
 *       copy it before returning. That is the only statement this interface makes about
 *       their availability: it does not say which component owns the storage, how long it
 *       stays valid, or whether a handler may release it, so the handler should release
 *       neither and retain neither pointer. `response` is a `wifi_BTMResponse_t *`, so its
 *       extent follows from the type; `peerMac` is a bare `CHAR *` with no stated extent, so
 *       a copy of it cannot be bounded from this interface. The response's `token` member is
 *       what matches it to the request that prompted it, and its `status` member is what
 *       says whether the STA accepted the transition.
 * @note The handler must not suspend and must not invoke any blocking system calls, per
 *       `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe and may enter the handler on a `HAL`
 *       thread, so the handler must serialise its own access to caller state; see
 *       `Threading Model` in `docs/pages/halSpec.md`.
 * @note In this build `peerMac` is a `CHAR *`, and this interface does not state whether it
 *       points at a NUL-terminated MAC address string or at six raw octets; where
 *       `WIFI_HAL_VERSION_3_PHASE2` is defined the same argument is a `mac_address_t`.
 *
 * @see wifi_BTMQueryRequest_callback_register
 * @see wifi_BTMResponse_t
 */
typedef INT (*wifi_BTMResponse_callback)(UINT apIndex,
                                            CHAR *peerMac,
                                            wifi_BTMResponse_t *response);
#endif
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Registers a callback function for BTM queries.
 *
 * The two handlers cover the two halves of an 802.11v BSS Transition Management
 * transaction, and they are installed together per AP. `btmQueryCallback` is invoked when a
 * STA asks to be transitioned and must return the request frame the driver sends back;
 * `btmResponseCallback` is invoked when a STA answers a request, whether that request came
 * from the query handler or from `wifi_setBTMRequest()`.
 *
 * @param[in] apIndex            Index of the AP the handlers are installed for. The indices
 *                               this interface defines are `AP_INDEX_1` to `AP_INDEX_24` in
 *                               `wifi_hal_generic.h`. Registration is per AP, so a caller
 *                               that serves several VAPs must register for each.
 * @param[in] btmQueryCallback  Pointer to the callback function for BTM queries, of type
 *                               `wifi_BTMQueryRequest_callback`. The `HAL` keeps this
 *                               function pointer after the call returns, since it invokes
 *                               the handler later, so it must remain callable for as long
 *                               as BTM queries are to be answered. This interface declares
 *                               no call that removes it and states no end to a
 *                               registration, so it does not establish when the `HAL`
 *                               stops using the pointer.
 * @param[in] btmResponseCallback Pointer to the callback function for BTM
 *                                responses, of type `wifi_BTMResponse_callback`, retained
 *                                on the same terms. This interface does not state whether
 *                                either pointer may be NULL to install only one handler, so
 *                                a caller should supply both.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. The effect of registering beforehand is not specified by
 *      this interface.
 * @post On success both handlers are installed for `apIndex`. This interface defines no
 *       unregister call for them, so a caller must not assume the handlers can be
 *       removed, and it does not state whether a later registration for the same
 *       `apIndex` replaces the earlier handlers, adds to them or is rejected.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handlers were installed for `apIndex`.
 * @retval WIFI_HAL_ERROR   The handlers were not installed as requested. This interface
 *                          does not state whether either handler is installed after a
 *                          failure, so a caller must not call `wifi_setBTMRequest()`
 *                          expecting a response notification and must not assume the
 *                          previous registration state is intact either.
 *
 * @execution callback
 * @sideeffect On success, installs both handlers for `apIndex` and keeps both function
 *             pointers after this call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and does not block, per
 *       `Blocking calls` in `docs/pages/halSpec.md`; delivery of both handlers is asynchronous.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_BTMQueryRequest_callback
 * @see wifi_BTMResponse_callback
 * @see wifi_setBTMRequest
 */
INT wifi_BTMQueryRequest_callback_register(UINT apIndex, wifi_BTMQueryRequest_callback btmQueryCallback, wifi_BTMResponse_callback btmResponseCallback);

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Sends a BTM request to a non-AP STA.
 *
 * This is the AP-initiated half of 802.11v BSS Transition Management: the AP asks a STA to
 * move, without the STA having queried first. It is how a steering decision is delivered to
 * a client that supports BTM, in place of the disconnect that
 * `wifi_steering_clientDisconnect()` performs. The STA's answer arrives through the
 * `wifi_BTMResponse_callback` handler.
 *
 * @param[in] apIndex  Index of the AP to send the request from. The indices this interface
 *                     defines are `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`.
 * @param[in] peerMac  MAC address of the peer STA, as the six octets of `mac_address_t`.
 *                     The caller owns the storage and the `HAL` reads it during the call.
 * @param[in] request  Pointer to the BTM request frame to send. One frame is passed, not
 *                     an array. The caller allocates and owns it, per `Memory Model` in
 *                     `docs/pages/halSpec.md`; the `HAL` reads it during the call, and
 *                     whether the implementation retains the pointer afterwards is not
 *                     specified by this interface, so the caller should keep the
 *                     structure allocated and unmoved while the `HAL` remains
 *                     initialised. The caller sets `token` to the dialog token it will
 *                     match the response against, sets `numCandidates` to bound the
 *                     `candidates` array and `urlLen` to bound `url`, and must set
 *                     `requestMode` consistently with the optional fields it has
 *                     populated.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @pre A `wifi_BTMResponse_callback` handler should be installed for `apIndex` with
 *      `wifi_BTMQueryRequest_callback_register()` before the request is sent, since that
 *      handler is the only way this interface reports the STA's answer.
 * @post On success the `HAL` has accepted the request for transmission. The STA has not
 *       answered when the call returns, and this interface states neither a time by which
 *       the response callback arrives nor what happens if the STA never answers, so a caller
 *       must apply its own timeout. A successful return is not evidence that the STA
 *       transitioned; only a response carrying an accepting `status` is.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The `HAL` accepted the request for transmission.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments and fall back to
 *                          `wifi_steering_clientDisconnect()` if it must move a client that
 *                          cannot be transitioned.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, so it
 *       returns before the frame is acknowledged.
 * @note This declaration is compiled only where `WIFI_HAL_VERSION_3_PHASE2` is defined.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_BTMRequest_t
 * @see wifi_BTMResponse_callback
 * @see wifi_BTMQueryRequest_callback_register
 */
INT wifi_setBTMRequest(UINT apIndex, mac_address_t peerMac, wifi_BTMRequest_t *request);
#endif

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
 
/**
 * @brief Callback function invoked when a STA responds to a Beacon Request.
 * 
 * This callback function is invoked when a station (STA) responds to a Beacon 
 * Request from the gateway, or as a triggered autonomous report. 
 * 
 * An autonomous report can be configured by a Beacon Request by setting the 
 * enable, request, and report bits in the measurement request, as described in 
 * 802.11-2016 Table 9-81 and section 11.11.8. 
 * 
 * When a triggered autonomous report causes the callback to be invoked, the 
 * dialog token and measurement token are both set to 0.
 *
 * @param[in] apIndex      Index of the AP that received the report. The indices this
 *                         interface defines are `AP_INDEX_1` to `AP_INDEX_24` in
 *                         `wifi_hal_generic.h`.
 * @param[in] out_struct   Pointer to the beacon report the `HAL` delivers, as an array of
 *                         `wifi_BeaconReport_t` holding `*out_array_size` elements, one per
 *                         measured BSS. The three pointer arguments are named from the
 *                         `HAL`'s point of view, because they are the `HAL`'s outputs, but
 *                         relative to the handler all three are inputs: the `HAL` allocates
 *                         the storage and fills it before entering the handler, and the
 *                         handler only reads it.
 * @param[in] out_array_size Pointer to the number of `wifi_BeaconReport_t` elements
 *                         `out_struct` points at, in elements. A handler must read
 *                         `*out_array_size` before indexing and must not read past the last
 *                         element it accounts for. This interface states no minimum, so a
 *                         handler must not assume that at least one element is present.
 * @param[in] out_DialogToken Pointer to the dialog token of the measurement this report
 *                         answers, which is the value `wifi_setRMBeaconRequest()` wrote to
 *                         its own `out_DialogToken` for that request. For a triggered
 *                         autonomous report it is 0, as the description above states, which
 *                         is how a handler tells a solicited report from an unsolicited
 *                         one.
 *
 * @returns The status of the operation, returned by the handler to the `HAL`.
 * @retval RETURN_OK If successful.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note `RETURN_OK` and `RETURN_ERR`, defined in `wifi_hal_generic.h`, carry the same
 *       values as the `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR` codes the rest of this
 *       interface uses. This interface does not state how the `HAL` acts on `RETURN_ERR`,
 *       so a handler must not rely on redelivery.
 * @note The three pointers carry event data into the handler rather than results back out
 *       of it. This interface does not state which component owns the three pointers or how
 *       long they stay valid, so a handler that needs the report afterwards copies the
 *       elements it wants before returning, and must not free or retain any of the three,
 *       per `Asynchronous Notification Model` in `docs/pages/halSpec.md`. This interface does
 *       not state that the `HAL` reads anything written back through them, so a handler
 *       must not use them to return data.
 * @note The handler must not suspend and must not invoke any blocking system calls, per
 *       `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe and may enter the handler on a `HAL`
 *       thread, so the handler must serialise its own access to caller state; see
 *       `Threading Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_RMBeaconRequestCallbackRegister
 * @see wifi_RMBeaconRequestCallbackUnregister
 * @see wifi_BeaconReport_t
 * @see wifi_setRMBeaconRequest
 */
typedef INT (*wifi_RMBeaconReport_callback)(UINT apIndex,
                                            wifi_BeaconReport_t *out_struct,
                                            UINT *out_array_size,
                                            UCHAR *out_DialogToken);
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
 
/**
 * @brief Registers a callback function for Beacon Requests.
 *
 * The handler installed here is where every 802.11k beacon report for the AP arrives,
 * whether it answers a `wifi_setRMBeaconRequest()` or is a triggered autonomous report.
 * `wifi_setRMBeaconRequest()` returns an error if no handler has been registered for the
 * AP, so this call comes first in that sequence.
 *
 * @param[in] apIndex              Index of the AP the handler is installed for. The indices
 *                                 this interface defines are `AP_INDEX_1` to `AP_INDEX_24`
 *                                 in `wifi_hal_generic.h`. Registration is per AP, so a
 *                                 caller that serves several VAPs must register for each.
 * @param[in] beaconReportCallback Pointer to the callback function to register, of type
 *                                 `wifi_RMBeaconReport_callback`. The `HAL` retains this
 *                                 function pointer and invokes it until it is unregistered,
 *                                 so the function must remain callable for that whole
 *                                 period. The effect of passing NULL is not specified by
 *                                 this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. The effect of registering beforehand is not specified by
 *      this interface.
 * @post On success beacon reports for `apIndex` are delivered to the handler. This interface
 *       does not state whether registering a second handler for the same AP adds to a list
 *       or replaces the first, so a caller should keep one handler per AP.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was installed for `apIndex`.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. On failure the caller should
 *                          not issue a beacon request, since it would be rejected for want
 *                          of a handler and no report would arrive.
 *
 * @execution callback
 * @sideeffect On success, installs `beaconReportCallback` as the handler that receives
 *             beacon reports for `apIndex`, and keeps that function pointer after this
 *             call returns until `wifi_RMBeaconRequestCallbackUnregister()` removes it.
 *             This interface states no other effect.
 *
 * @note The registration call itself is synchronous and does not block, per
 *       `Blocking calls` in `docs/pages/halSpec.md`; delivery of the report is asynchronous.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_RMBeaconReport_callback
 * @see wifi_RMBeaconRequestCallbackUnregister
 * @see wifi_setRMBeaconRequest
 */
INT wifi_RMBeaconRequestCallbackRegister(UINT apIndex, wifi_RMBeaconReport_callback beaconReportCallback);

/**
 * @brief Unregisters a callback function for Beacon Requests.
 *
 * This is the teardown counterpart of `wifi_RMBeaconRequestCallbackRegister()`. Unlike
 * `wifi_steering_eventUnregister()`, it names both the AP and the handler being removed.
 *
 * @param[in] apIndex              Index of the AP the handler was installed for. The indices
 *                                 this interface defines are `AP_INDEX_1` to `AP_INDEX_24`
 *                                 in `wifi_hal_generic.h`.
 * @param[in] beaconReportCallback Pointer to the callback function to unregister. It must be
 *                                 the same function pointer that was registered for
 *                                 `apIndex`; this interface does not state what happens when
 *                                 a different one is passed.
 *
 * @pre A handler must have been installed for `apIndex` with
 *      `wifi_RMBeaconRequestCallbackRegister()`.
 * @post On success no further beacon reports are delivered for `apIndex`. This interface
 *       does not state whether a report already being delivered is allowed to complete, nor
 *       whether requests still outstanding are cancelled, so a caller should cancel them
 *       with `wifi_cancelRMBeaconRequest()` first and must keep the state its handler
 *       touches valid until it can be sure no handler is running.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was removed for `apIndex`.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. On failure the caller must
 *                          assume the handler may still be invoked and so must not free
 *                          anything the handler uses.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`. It is a
 *       teardown call rather than a registration, so it carries no callback-execution
 *       marker.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_RMBeaconRequestCallbackRegister
 * @see wifi_cancelRMBeaconRequest
 */
INT wifi_RMBeaconRequestCallbackUnregister(UINT apIndex, wifi_RMBeaconReport_callback beaconReportCallback);

/**
 * @brief Cancels a pending radio measurement beacon request.
 *
 * A beacon request configured to repeat continues producing reports until it is cancelled,
 * so this is how a caller ends a repeating measurement it started with
 * `wifi_setRMBeaconRequest()`.
 *
 * @param[in] apIndex     Index of the AP the request was sent from. The indices this
 *                        interface defines are `AP_INDEX_1` to `AP_INDEX_24` in
 *                        `wifi_hal_generic.h`.
 * @param[in] dialogToken Dialog token of the request to cancel, which is the value
 *                        `wifi_setRMBeaconRequest()` wrote to its `out_DialogToken` output
 *                        for that request. This interface pairs the two through this
 *                        argument and states no other correlation mechanism, so a caller
 *                        must keep the token it was given in order to be able to cancel.
 *                        The token is a single octet, and this interface does not state
 *                        whether 0 is legal here, so a caller should not pass it: 0 is the
 *                        value a triggered autonomous report carries instead of a real
 *                        token.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the `HAL` has accepted the cancellation. This interface does not state
 *       whether a report already in flight is still delivered, so a caller must be ready to
 *       receive one after cancelling and must keep its handler usable until it is.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The `HAL` accepted the cancellation.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it does not separate an
 *                          unknown token from a genuine failure, so the caller should treat
 *                          the measurement as possibly still running and must not release
 *                          its handler on the strength of this code alone.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_setRMBeaconRequest
 * @see wifi_RMBeaconRequestCallbackUnregister
 */
INT wifi_cancelRMBeaconRequest(UINT apIndex, UCHAR dialogToken);

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Sends a radio measurement beacon request.
 *
 * This function sends a radio measurement beacon request to a peer STA. The
 * request is sent based on the information in the `in_request` parameter.
 * Returns an error if a callback has not been registered for the AP.
 *
 * This is how a caller learns what a client can hear: the STA measures beacons on the
 * requested channels and reports them back through the registered
 * `wifi_RMBeaconReport_callback`, which is the input a steering decision needs and which no
 * AP-side survey can supply.
 *
 * @param[in] apIndex        Index of the VAP to send the request from. The indices this
 *                           interface defines are `AP_INDEX_1` to `AP_INDEX_24` in
 *                           `wifi_hal_generic.h`.
 * @param[in] peer           MAC address of the peer device to send the request to, as the
 *                           six octets of `mac_address_t`. The caller owns the storage and
 *                           the `HAL` reads it during the call.
 * @param[in] in_request     Pointer to a `wifi_BeaconRequest_t` structure containing the
 *                           beacon request information. One request is passed, not an
 *                           array. The caller allocates and owns it, per `Memory Model`
 *                           in `docs/pages/halSpec.md`; the `HAL` reads it during the
 *                           call, and whether the implementation retains the pointer
 *                           afterwards is not specified by this interface, so the caller
 *                           should keep the structure allocated and unmoved while the
 *                           `HAL` remains initialised. Each optional subelement is read
 *                           only where its `<field>Present` flag is `TRUE`, and
 *                           `numRepetitions` decides whether the measurement runs once or
 *                           repeats until cancelled.
 * @param[out] out_DialogToken Pointer to a variable that receives the dialog token of this
 *                           request. The caller allocates and owns the single-octet
 *                           variable. The parameter is an output only - a caller supplies no
 *                           token and this interface names no component as assigning the
 *                           value, so which one chooses it is not established here and a
 *                           caller must treat the octet as opaque rather than construct or
 *                           predict one. What the value is for is correlation: on success it
 *                           identifies this request, it is echoed in the `out_DialogToken`
 *                           of each report the handler receives, and it is the value
 *                           `wifi_cancelRMBeaconRequest()` takes. A caller that intends to
 *                           cancel or to correlate reports must therefore keep it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @pre A handler must be registered for `apIndex` with
 *      `wifi_RMBeaconRequestCallbackRegister()`. The description above states that this
 *      call returns an error otherwise, so the registration is a hard pre-condition rather
 *      than a recommendation.
 * @post On success the request has been accepted for transmission and `*out_DialogToken`
 *       holds its token. No measurement has been made when the call returns: reports arrive
 *       later through the registered handler. This interface states neither a time by which
 *       a report arrives nor what happens if the STA never answers, so a caller must apply
 *       its own timeout and cancel a repeating request it no longer wants. On failure
 *       `*out_DialogToken` must be treated as undefined.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK  If successful.
 * @retval RETURN_ERR If any error is detected.
 *
 * @note `RETURN_OK` and `RETURN_ERR`, defined in `wifi_hal_generic.h`, carry the same
 *       values as the `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR` codes the rest of this
 *       interface uses, and are the whole return-code contract of this call. The one cause
 *       this interface binds to `RETURN_ERR` is the missing handler registration, which the
 *       description above states outright. Beyond that the code is opaque: it establishes
 *       that the request was not accepted and not why, so a caller cannot distinguish an
 *       invalid argument from an unsupported request from an absent peer by reading it, and
 *       must not branch as though it could. Confirming the registration, and the peer's
 *       capabilities with `wifi_getRMCapabilities()`, is what a caller can do before
 *       retrying; anything further has to come from the implementation's own diagnostics.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, so it
 *       returns before the measurement has been made and does not wait out the request's
 *       `duration`.
 * @note This declaration is compiled only where `WIFI_HAL_VERSION_3_PHASE2` is defined.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_BeaconRequest_t
 * @see wifi_RMBeaconReport_callback
 * @see wifi_cancelRMBeaconRequest
 * @see wifi_getRMCapabilities
 */
INT wifi_setRMBeaconRequest(UINT apIndex,
                            mac_address_t peer,
                            wifi_BeaconRequest_t *in_request,
                            UCHAR *out_DialogToken);

/**
 * @brief Gets the Radio Measurement capabilities of a peer device.
 *
 * A caller uses this to find out whether a client can service a beacon measurement before
 * asking it to, since `wifi_setRMBeaconRequest()` only reports that a request was accepted
 * for transmission and not that the STA is able to answer it. Unlike every other call in
 * this header, it is scoped by peer alone and takes no AP or radio index.
 *
 * @param[in] peer            MAC address of the peer device, as the six octets of
 *                            `mac_address_t`. The caller owns the storage and the `HAL`
 *                            reads it during the call.
 * @param[out] out_Capabilities Pointer to an array to store the capabilities, as defined in
 *                            802.11-2016. The array is a fixed five octets, which is the
 *                            length of the RM Enabled Capabilities element and the same
 *                            encoding the `capabilities` member of
 *                            `wifi_RMEnabledCapabilities_t` carries. The caller allocates
 *                            and owns the storage and must provide all five octets. The
 *                            `HAL` writes them during the call, and whether the
 *                            implementation retains the pointer afterwards is not
 *                            specified by this interface, so the caller should keep the
 *                            five octets allocated and unmoved while the `HAL` remains
 *                            initialised. The array is exactly as long as it is declared,
 *                            so there is no count argument and none is needed. This
 *                            interface does not name the individual bits, so which octet
 *                            and bit carries a given capability follows 802.11-2016
 *                            rather than anything established here;
 *                            `wifi_steering_rrmCaps_t` names the subset of those
 *                            capabilities that steering uses, in decoded form.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success all five octets have been written. On failure the contents are undefined
 *       and must not be decoded; in particular a caller must not read an all-zero array as
 *       a peer that supports nothing.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The capabilities were written.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should treat the
 *                          peer's RM support as unknown rather than absent, and may still
 *                          issue a beacon request and rely on its return code.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, so it
 *       reports capabilities the `HAL` already holds from association rather than querying
 *       the peer over the air.
 * @note This declaration is compiled only where `WIFI_HAL_VERSION_3_PHASE2` is defined.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_RMEnabledCapabilities_t
 * @see wifi_steering_rrmCaps_t
 * @see wifi_setRMBeaconRequest
 */
INT wifi_getRMCapabilities(mac_address_t peer, UCHAR out_Capabilities[5]);
#endif

/** @} */  //END OF GROUP WIFI_HAL_APIS

#ifdef __cplusplus
}
#endif

#endif //__WIFI_HAL_EXTENDER_H__