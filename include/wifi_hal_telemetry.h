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
#ifndef __WIFI_HAL_TELEMETRY_H__
#define __WIFI_HAL_TELEMETRY_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "wifi_hal_generic.h"
/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief Wi-Fi radio traffic statistics.
 *
 * One instance carries the traffic counters and channel-condition metrics of a single
 * radio. The caller allocates the instance and passes its address to
 * `wifi_getRadioTrafficStats2()`, which fills every member; the caller keeps ownership
 * of the storage, per `Memory Model` in `docs/pages/halSpec.md`.
 *
 * Two kinds of member sit side by side here and must be read differently. The `ULONG`
 * members are cumulative counts of bytes or packets. The `INT` metrics - activity
 * factor, carrier-sense-threshold exceeded, retransmission and the three noise-floor
 * figures - together with `radio_ChannelUtilization`, are instead recalculated over the
 * measuring interval named in their member comments, and each reports `-1` until its
 * first interval has completed. A caller must therefore test those members for `-1`
 * before using them rather than treating the value as a measurement.
 *
 * @note This interface does not state what resets the counters, or how often. The one
 *       statement it makes is that `radio_StatisticsStartTime` is updated whenever the
 *       radio statistics are reset, so a caller computing a delta between two reads
 *       should read that member alongside the counters and discard any delta that spans
 *       a change in it.
 * @see wifi_getRadioTrafficStats2
 */
typedef struct _wifi_radioTrafficStats2
{
    ULONG radio_BytesSent;             /**< The total number of bytes transmitted out of the interface, including framing characters. */
    ULONG radio_BytesReceived;         /**< The total number of bytes received on the interface, including framing characters. */
    ULONG radio_PacketsSent;           /**< The total number of packets transmitted out of the interface. */
    ULONG radio_PacketsReceived;       /**< The total number of packets received on the interface. */
    ULONG radio_ErrorsSent;            /**< The total number of outbound packets that could not be transmitted because of errors. */
    ULONG radio_ErrorsReceived;        /**< The total number of inbound packets that contained errors preventing them from being delivered to a higher-layer protocol. */
    ULONG radio_DiscardPacketsSent;     /**< The total number of outbound packets which were chosen to be discarded even though no errors had been detected to prevent their being transmitted. One possible reason for discarding such a packet could be to free up buffer space. */
    ULONG radio_DiscardPacketsReceived; /**< The total number of inbound packets which were chosen to be discarded even though no errors had been detected to prevent their being delivered. One possible reason for discarding such a packet could be to free up buffer space. */
    ULONG radio_PLCPErrorCount;        /**< The number of packets that were received with a detected Physical Layer Convergence Protocol (PLCP) header error. */
    ULONG radio_FCSErrorCount;         /**< The number of packets that were received with a detected FCS error. This parameter is based on dot11FCSErrorCount from [Annex C/802.11-2012]. */
    ULONG radio_InvalidMACCount;       /**< The number of packets that were received with a detected invalid MAC header error. */
    ULONG radio_PacketsOtherReceived;  /**< The number of packets that were received, but which were destined for a MAC address that is not associated with this interface. */
    INT   radio_NoiseFloor;            /**< The noise floor for this radio channel where a recoverable signal can be obtained. Expressed as a signed integer in the range (-110:0). Measurement should capture all energy (in dBm) from sources other than Wi-Fi devices as well as interference from Wi-Fi devices too weak to be decoded. Measured in dBm */
    ULONG radio_ChannelUtilization;     /**< Percentage of time the channel was occupied by the radio's own activity (Activity Factor) or the activity of other radios. Channel utilization MUST cover all user traffic, management traffic, and time the radio was unavailable for CSMA activities, including DIFS intervals, etc. The metric is calculated and updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval". The calculation of this metric MUST only use the data collected from the just completed interval. If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in Percentage */
    INT   radio_ActivityFactor;       /**< Percentage of time that the radio was transmitting or receiving Wi-Fi packets to/from associated clients. Activity factor MUST include all traffic that deals with communication between the radio and clients associated to the radio as well as management overhead for the radio, including NAV timers, beacons, probe responses,time for receiving devices to send an ACK, SIFC intervals, etc. The metric is calculated and updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval". The calculation of this metric MUST only use the data collected from the just completed interval. If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in Percentage */
    INT   radio_CarrierSenseThreshold_Exceeded; /**< Percentage of time that the radio was unable to transmit or receive Wi-Fi packets to/from associated clients due to energy detection (ED) on the channel or clear channel assessment (CCA). The metric is calculated and updated in this Parameter at the end of the interval defined by "Radio Statistics Measuring Interval". The calculation of this metric MUST only use the data collected from the just completed interval. If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in Percentage */
    INT   radio_RetransmissionMetirc;   /**< Percentage of packets that had to be re-transmitted. Multiple re-transmissions of the same packet count as one. The metric is calculated and updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval". The calculation of this metric MUST only use the data collected from the just completed interval. If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in percentage */
    INT   radio_MaximumNoiseFloorOnChannel; /**< Maximum Noise on the channel during the measuring interval. The metric is updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval". The calculation of this metric MUST only use the data collected in the just completed interval. If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in dBm */
    INT   radio_MinimumNoiseFloorOnChannel; /**< Minimum Noise on the channel. The metric is updated in this Parameter at the end of the interval defined by "Radio Statistics Measuring Interval". The calculation of this metric MUST only use the data collected in the just completed interval. If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in dBm */
    INT   radio_MedianNoiseFloorOnChannel;  /**< Median Noise on the channel during the measuring interval. The metric is updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval". The calculation of this metric MUST only use the data collected in the just completed interval. If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in dBm */
    ULONG radio_StatisticsStartTime;     /**< The date and time at which the collection of the current set of statistics started. This time must be updated whenever the radio statistics are reset, which makes it the only signal in this structure that a reset has occurred. This interface does not state the epoch or the unit the value is expressed in, so a caller should compare successive readings rather than interpret one in absolute terms. */
} wifi_radioTrafficStats2_t;

/**
 * @brief Wi-Fi SSID traffic statistics.
 *
 * One instance carries the traffic counters of a single SSID. The caller allocates the
 * instance and passes its address to `wifi_getSSIDTrafficStats2()`, which fills every
 * member; the caller keeps ownership of the storage, per `Memory Model` in
 * `docs/pages/halSpec.md`. Every member is a cumulative count of bytes or packets -
 * unlike `wifi_radioTrafficStats2_t`, this structure carries no interval metric and no
 * `-1` sentinel.
 *
 * The four retransmission counters answer different questions and are easily conflated:
 * `ssid_RetransCount` counts every retransmitted packet, `ssid_RetryCount` counts
 * packets that succeeded after at least one retransmission, `ssid_MultipleRetryCount`
 * counts those that needed more than one, and `ssid_FailedRetransCount` counts those
 * that exhausted the 802.11 retry limit without succeeding.
 *
 * @note This interface does not state what resets these counters, or how often, and this
 *       structure has no start-time member of its own, so a caller cannot tell from it
 *       whether a reset occurred between two reads.
 * @see wifi_getSSIDTrafficStats2
 */
typedef struct _wifi_ssidTrafficStats2
{
    ULONG ssid_BytesSent;              /**< The total number of bytes transmitted out of the interface, including framing characters. */
    ULONG ssid_BytesReceived;          /**< The total number of bytes received on the interface, including framing characters. */
    ULONG ssid_PacketsSent;            /**< The total number of packets transmitted out of the interface. */
    ULONG ssid_PacketsReceived;        /**< The total number of packets received on the interface. */
    ULONG ssid_RetransCount;           /**< The total number of transmitted packets which were retransmissions. Two retransmissions of the same packet results in this counter incrementing by two. */
    ULONG ssid_FailedRetransCount;      /**< The number of packets that were not transmitted successfully due to the number of retransmission attempts exceeding an 802.11 retry limit. This parameter is based on dot11FailedCount from [802.11-2012]. */
    ULONG ssid_RetryCount;             /**< The number of packets that were successfully transmitted after one or more retransmissions. This parameter is based on dot11RetryCount from [802.11-2012]. */
    ULONG ssid_MultipleRetryCount;    /**< The number of packets that were successfully transmitted after more than one retransmission. This parameter is based on dot11MultipleRetryCount from [802.11-2012]. */
    ULONG ssid_ACKFailureCount;         /**< The number of expected ACKs that were never received. This parameter is based on dot11ACKFailureCount from [802.11-2012]. */
    ULONG ssid_AggregatedPacketCount;  /**< The number of aggregated packets that were transmitted. This applies only to 802.11n and 802.11ac. */
    ULONG ssid_ErrorsSent;            /**< The total number of outbound packets that could not be transmitted because of errors. */
    ULONG ssid_ErrorsReceived;        /**< The total number of inbound packets that contained errors preventing them from being delivered to a higher-layer protocol. */
    ULONG ssid_UnicastPacketsSent;    /**< The total number of packets that higher-level protocols requested for transmission and which were not addressed to a multicast or broadcast address at this layer, including those that were discarded or not sent. */
    ULONG ssid_UnicastPacketsReceived; /**< The total number of received packets, delivered by this layer to a higher layer, which were not addressed to a multicast or broadcast address at this layer. */
    ULONG ssid_DiscardedPacketsSent;   /**< The total number of outbound packets which were chosen to be discarded even though no errors had been detected to prevent their being transmitted. One possible reason for discarding such a packet could be to free up buffer space. */
    ULONG ssid_DiscardedPacketsReceived; /**< The total number of inbound packets which were chosen to be discarded even though no errors had been detected to prevent their being delivered. One possible reason for discarding such a packet could be to free up buffer space. */
    ULONG ssid_MulticastPacketsSent;   /**< The total number of packets that higher-level protocols requested for transmission and which were addressed to a multicast address at this layer, including those that were discarded or not sent. */
    ULONG ssid_MulticastPacketsReceived; /**< The total number of received packets, delivered by this layer to a higher layer, which were addressed to a multicast address at this layer. */
    ULONG ssid_BroadcastPacketsSent;   /**< The total number of packets that higher-level protocols requested for transmission and which were addressed to a broadcast address at this layer, including those that were discarded or not sent. */
    ULONG ssid_BroadcastPacketsRecevied; /**< The total number of received packets, delivered by this layer to a higher layer, which were addressed to a broadcast address at this layer. The member name carries an upstream misspelling of "Received"; the spelling is part of the declared interface, so a caller must use it as written. */
    ULONG ssid_UnknownPacketsReceived;  /**< The total number of packets received via the interface which were discarded because of an unknown or unsupported protocol. */
} wifi_ssidTrafficStats2_t;

/**
 * @brief Neighboring Access Point information.
 * Do not edit the elements for this data structure 
 *
 * One instance describes one Access Point that a radio of this device observed on the
 * air. Instances are the elements of the array `wifi_getNeighboringWiFiStatus()`
 * allocates, so their lifetime is the lifetime of that array and the caller releases the
 * array as a whole rather than element by element.
 *
 * Every `CHAR` member is a fixed-capacity text field whose declared array size is its
 * capacity: 64 bytes for the identity, mode, security and supported-standards fields, 16
 * for the band, operating-standard and bandwidth fields, and 256 for the two
 * comma-separated rate lists. This interface does not state whether the text is
 * NUL-terminated, so a caller must bound every read by the declared size.
 *
 * `ap_ChannelUtilization` and `ap_StaCount` are carried in the BSS Load information
 * element, which an Access Point need not advertise, so both are meaningful only when
 * `bss_load_element_present` is `TRUE`.
 *
 * @see wifi_getNeighboringWiFiStatus
 */
typedef struct _wifi_neighbor_ap2
{
    CHAR ap_SSID[64];               /**< The current Service Set Identifier (SSID) in use by the neighboring Wi-Fi AP. The value may be empty for hidden SSIDs. */
    CHAR ap_BSSID[64];              /**< The BSSID used for the neighboring Wi-Fi SSID. */
    CHAR ap_Mode[64];               /**< The mode the neighboring Wi-Fi radio is operating in. Enumeration of: AdHoc, Infrastructure. */
    UINT ap_Channel;                /**< The current radio channel used by the neighboring Wi-Fi radio. */
    INT ap_SignalStrength;           /**< An indicator of radio signal strength (RSSI) of the neighboring Wi-Fi radio measured in dBm, as an average of the last 100 packets received. */
    CHAR ap_SecurityModeEnabled[64];  /**< The type of encryption the neighboring Wi-Fi SSID advertises. Enumeration of: None, WPA-WPA2 etc. */
    CHAR ap_EncryptionMode[64];       /**< Comma-separated list of strings. The type of encryption the neighboring Wi-Fi SSID advertises. Each list item is an enumeration of: TKIP, AES. */
    CHAR ap_OperatingFrequencyBand[16]; /**< Indicates the frequency band at which the radio this SSID instance is operating. Enumeration of: 2.4GHz, 5GHz. */
    CHAR ap_SupportedStandards[64];   /**< Comma-separated list of strings. List items indicate which IEEE 802.11 standards this instance can support simultaneously, in the frequency band specified by ap_OperatingFrequencyBand. */
    CHAR ap_OperatingStandards[16];   /**< Comma-separated list of strings. Each list item MUST be a member of the list reported by the ap_SupportedStandards parameter. List items indicate which IEEE 802.11 standard that is detected for this instance. */
    CHAR ap_OperatingChannelBandwidth[16]; /**< Indicates the bandwidth at which the channel is operating. */
    UINT ap_BeaconPeriod;             /**< Time interval (in ms) between transmitting beacons. */
    INT ap_Noise;                    /**< Indicator of average noise strength (in dBm) received from the neighboring Wi-Fi radio. */
    CHAR ap_BasicDataTransferRates[256]; /**< Comma-separated list (maximum list length 256) of strings. Basic data transmit rates (in Mbps) for the SSID. For example, if ap_BasicDataTransferRates is "1,2", this indicates that the SSID is operating with basic rates of 1 Mbps and 2 Mbps. */
    CHAR ap_SupportedDataTransferRates[256]; /**< Comma-separated list (maximum list length 256) of strings. Data transmit rates (in Mbps) for unicast frames at which the SSID will permit a station to connect. For example, if ap_SupportedDataTransferRates is "1,2,5.5", this indicates that the SSID will only permit connections at 1 Mbps, 2 Mbps and 5.5 Mbps. */
    UINT ap_DTIMPeriod;               /**< The number of beacon intervals that elapse between transmission of Beacon frames containing a TIM element whose DTIM count field is 0. This value is transmitted in the DTIM Period field of beacon frames. [802.11-2012] */
    UINT ap_ChannelUtilization;       /**< Indicates the fraction of the time the AP senses that the channel is in use by the neighboring AP for transmissions. Valid only when bss_load_element_present is true. */
    UINT ap_freq;                        /**< Frequency the neighboring AP was observed on. This interface does not state the unit of this member; the comparable `freq` member of `wifi_bss_info_t` in `wifi_hal_generic.h` is likewise unqualified, so a caller must not assume MHz or kHz and should prefer `ap_Channel` with `ap_OperatingFrequencyBand` where an unambiguous value is needed. */
    BOOL bss_load_element_present;    /**< Flag indicating presence of BSS Load IE; controls validity of related fields. */
    UINT ap_StaCount;                /**< Number of stations currently associated with the BSS. Valid only when bss_load_element_present is true. */
} wifi_neighbor_ap2_t;

/*    Explanation:
                             these are actually 3 host-endian integers
                            in this example they are big-endian because
                             the piranha's host cpu is big-endian MIPS
                                    _____________|____________
                                   /             |            \
                                  |              |            |
                             _____|______    ____|____    ____|_____
                            |            |  |         |  |          |
     ap1       glastackrssi:75  74  73  77  2  3  68  1  0  0  0  136
                            ^^^^^^^^^^^^^^  ^^^^^^^^^^^  ^^^^^^^^^^^^
                                  |              |            |
                         last 4 rssi values      |      sample counter
                                                 |
                                         last 4 rssi's age
    
                                the "77" rssi is 1 second old
                                         ______|______
                                        /             \
                                        |             |
     ap1       glastackrssi:75  74  73  77  2  3  68  1  0  0  0  136
                                     |             |
                                     \____________/
                                           |
                                 the 2nd most recent rssi of "73"
                                 is 68 seconds old *in relation*
                                 to the 1st ("77") therefore it is
                                 68 + 1 seconds old *now*   */
/**
 * @brief RSSI snapshot.
 *
 * A four-deep history of received signal strength, laid out as three host-endian byte
 * groups: the last four RSSI readings, their ages, and a frame counter. The worked
 * example in the comment above this structure shows one such snapshot as the vendor
 * stacks it.
 *
 * The ages are *relative*, which is the one thing a caller must get right: `time_s[i]`
 * is the age of `rssi[i]` measured against the reading before it, so an absolute age is
 * the running sum of the entries up to and including that index - in the worked example
 * the second-most-recent reading is 68 seconds older than the most recent, which is
 * itself 1 second old, making it 69 seconds old now.
 *
 * No API in this header takes this type. It reaches a caller as the `cli_rssi_bcn` and
 * `cli_rssi_ack` members of `wifi_associated_dev_stats_t` in `wifi_hal_extender.h`,
 * which is where beacon-derived and ACK-derived snapshots are distinguished.
 *
 * @note The readings are `UCHAR` while every other RSSI value in this interface is a
 *       signed `INT` in dBm. This interface does not state the encoding that maps these
 *       bytes onto a signed dBm value, so a caller must not cast an entry to `INT` and
 *       read it as dBm.
 * @see wifi_associated_dev_stats_t
 */
typedef struct _wifi_rssi_snapshot
{
    UCHAR  rssi[4];    /**< Last 4 RSSI values received. These are host-endian integers.
                         *   In this example, they are big-endian because the CPU is big-endian MIPS. */
    UCHAR  time_s[4];  /**< Relative age of the last 4 RSSI values, in seconds, indexed in
                         *   step with `rssi`. Each entry is the age of its reading measured
                         *   against the reading before it, not against now, so an absolute
                         *   age is the running sum up to that index. These are host-endian
                         *   integers, in the same layout as `rssi` above. */
    USHORT count;      /**< Sequence number of received management (beacon, ACK) frames. */
} wifi_rssi_snapshot_t;

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Access Point RSSI information - the form built when
 *        `WIFI_HAL_VERSION_3_PHASE2` is defined.
 *
 * This structure is declared twice, once in each arm of the surrounding conditional, and
 * the two forms are not interchangeable: the `#else` arm below types `ap_channelWidth` as
 * a plain `UINT`. Which form a caller compiles against is therefore fixed at build time
 * by `WIFI_HAL_VERSION_3_PHASE2`, and code that assigns to that member does not port
 * between the two configurations without a cast.
 *
 * No API in this header takes this type. Its only consumer in the interface is the
 * per-channel RSSI list of the channel-scan structure in `wifi_hal_deprecated.h`, a
 * header this repository declares out of scope, so a caller of the current surface reads
 * neighbour signal strength through `ap_SignalStrength` in `wifi_neighbor_ap2_t`
 * instead.
 *
 * @see wifi_neighbor_ap2_t
 */
typedef struct _wifi_apRssi
{
    CHAR ap_BSSID[6]; /**< BSSID of the Access Point, held as six raw octets - the width
                           `ETHER_ADDR_LEN` and `mac_address_t` use in
                           `wifi_hal_generic.h`. The field cannot hold a printable MAC
                           string and leaves no room for a terminator, so a caller must
                           read it as six bytes and not as text. */
    wifi_channelBandwidth_t ap_channelWidth; /**< The channel width; 1 for 20MHz, 2 for 40MHz, 4 for 80MHz, 8 for 160MHz, 10 for 80+80MHz. Under this build configuration the member is the enumeration `wifi_channelBandwidth_t` in `wifi_hal_generic.h`, so the compiler checks what is assigned to it. Those values are the enumerators' bit values written in hexadecimal - `WIFI_CHANNELBANDWIDTH_80_80MHZ` is `0x10` - and that enumeration additionally defines `WIFI_CHANNELBANDWIDTH_320MHZ` as `0x20`. */
    INT ap_rssi; /**< RSSI of the neighboring AP, in dBm, as a signed value. */
} wifi_apRssi_t;
#else
/**
 * @brief Access Point RSSI information - the form built when
 *        `WIFI_HAL_VERSION_3_PHASE2` is not defined.
 *
 * This is the default form of the structure. It differs from the `#ifdef` arm above in
 * one caller-visible respect: `ap_channelWidth` is a plain `UINT` here, so the compiler
 * accepts any unsigned value and offers no checking against the bandwidth enumeration,
 * even though the values the member carries are the same codes. The encoding and the
 * consumer of this type are described in that arm and are not repeated here.
 *
 * @see wifi_neighbor_ap2_t
 */
typedef struct _wifi_apRssi
{
    CHAR ap_BSSID[6]; /**< BSSID of the Access Point, held as six raw octets - the width
                           `ETHER_ADDR_LEN` and `mac_address_t` use in
                           `wifi_hal_generic.h`. The field cannot hold a printable MAC
                           string and leaves no room for a terminator, so a caller must
                           read it as six bytes and not as text. */
    UINT ap_channelWidth; /**< The channel width; 1 for 20MHz, 2 for 40MHz, 4 for 80MHz, 8 for 160MHz, 10 for 80+80MHz. Under this build configuration the member is an unchecked `UINT`, so a caller is responsible for assigning only the `wifi_channelBandwidth_t` codes from `wifi_hal_generic.h`, whose values these are in hexadecimal. */
    INT ap_rssi; /**< RSSI of the neighboring AP, in dBm, as a signed value. */
} wifi_apRssi_t;
#endif

// VAP telemetry report structures and function prototypes
/**
 * @brief Maximum number of VAPs this interface can present.
 *
 * The product of `MAX_NUM_RADIOS` and `MAX_NUM_VAP_PER_RADIO` in `wifi_hal_generic.h`,
 * so the value follows the build rather than being fixed: `MAX_NUM_RADIOS` is 3 when
 * `WIFI_HAL_VERSION_3` is defined and 2 otherwise, and `MAX_NUM_VAP_PER_RADIO` is 8,
 * giving 24 or 16. A caller sizing an array of per-VAP records should use this macro so
 * that the size follows the same flag.
 *
 * @note This is the interface-wide ceiling, not the number of VAPs a platform actually
 *       provisions; how many exist on a given device is not established here.
 */
#define MAX_VAP (MAX_NUM_RADIOS * MAX_NUM_VAP_PER_RADIO)

/**
 * @brief VAP telemetry information.
 *
 * Structure that holds the VAP telemetry information for one virtual Access Point. The
 * caller allocates an instance and passes its address to `wifi_getVAPTelemetry()`, which
 * fills it; the caller keeps ownership of the storage, per `Memory Model` in
 * `docs/pages/halSpec.md`.
 *
 * @see wifi_getVAPTelemetry
 */
typedef struct
{
    UINT txOverflow; /**< Count of Wi-Fi transmit overflow events recorded for the VAP.
                          This interface does not state which event increments the
                          counter, what happens when a `UINT` wraps, or what resets it,
                          so a caller should compare successive readings rather than read
                          a single value as an absolute total. */
} wifi_VAPTelemetry_t;

/**
* @typedef wifi_na_sta_req_params_t
* @brief Parameters to request unassociated station information.
*
* This structure specifies the station (STA) and radio context used to query
* unassociated station link metrics. The HAL implementation converts the
* operating class and channel to vendor-specific frequency/bandwidth parameters.
*
* The caller allocates an instance, fills all three members and passes a `const` pointer
* to `wifi_getNASta()`, which reads it during the call and, being `const`, does not
* modify it. The caller keeps ownership of the storage.
*
* The operating class is what makes the channel unambiguous: a channel number alone does
* not name a band, so both members must describe the same channel or the request does not
* identify a measurement context.
*
* @see wifi_getNASta
* @see wifi_na_sta_info_t
*/
typedef struct {
    mac_address_t sta_mac; /**< Station MAC address to query, as the six raw octets of
                                `mac_address_t` in `wifi_hal_generic.h`. This is the
                                station to measure, and it must not be associated to the
                                queried AP - see `wifi_getNASta()`. */
    UINT channel; /**< Channel number within the operating class named by `op_class`. The
                       channels a class admits are those the matching entry of
                       `op_class_ch_list_t` in `wifi_hal_generic.h` lists; this interface
                       does not state what happens when the pair is inconsistent. */
    UINT op_class; /**< Operating class (IEEE 802.11 global operating class), numbered as
                        in IEEE 802.11-2020 Table E-4 and used the same way by
                        `op_class_ch_list_t` in `wifi_hal_generic.h`. */
} wifi_na_sta_req_params_t;

/**
* @typedef wifi_na_sta_info_t
* @brief Returned unassociated station link metrics.
*
* Contains the measurement result for one unassociated station, expressed
* as RCPI (Received Channel Power Indicator) per IEEE 802.11-2020 §9.4.2.37.
*
* The caller allocates an instance and passes its address to `wifi_getNASta()`, which
* fills every member; the caller keeps ownership of the storage. The first three members
* restate the context the request supplied, so a caller correlating several measurements
* can identify a result without tracking the request alongside it.
*
* @see wifi_getNASta
* @see wifi_na_sta_req_params_t
*/
typedef struct {
    mac_address_t sta_mac; /**< Station MAC address (echoed/filled by implementation), as
                                the six raw octets of `mac_address_t` in
                                `wifi_hal_generic.h`. */
    UINT channel; /**< Channel on which the measurement was performed. This interface does
                       not state that it must equal the requested channel, so a caller
                       correlating results should read the value rather than assume it. */
    UINT op_class; /**< Operating class on which the measurement was performed, numbered
                        as in IEEE 802.11-2020 Table E-4. */
    UINT rcpi; /**< RCPI value: (RSSI_dBm + 110) * 2, clamped to [0, 220]. The encoding
                    gives half-dBm resolution, so 0 is -110 dBm and 220 is 0 dBm, and a
                    caller recovers dBm as `rcpi / 2 - 110`. This interface defines no
                    sentinel for an unavailable measurement, so a caller must not read any
                    particular value as "no result" and should rely on the return status
                    instead. */
} wifi_na_sta_info_t;

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */

/**
 * @brief Reads one radio's traffic counters and channel-condition metrics.
 *
 * This function retrieves detailed traffic statistics for the specified radio: byte and
 * packet totals, error and discard counts, and the interval metrics for channel
 * utilization, activity factor, carrier sense, retransmission and noise floor. The
 * figures cover the radio as a whole; per-SSID accounting is a separate call,
 * `wifi_getSSIDTrafficStats2()`.
 *
 * Two properties of `wifi_radioTrafficStats2_t` govern how the result is read. The
 * interval metrics report `-1` until their first measuring interval has completed, so a
 * caller must test for that before using them. And `radio_StatisticsStartTime` is the
 * only indication the structure gives that the counters were reset between two reads,
 * which matters to any caller computing deltas.
 *
 * @param[in] radioIndex    Index of the Wi-Fi radio channel. The indices this interface
 *                          defines are `RADIO_INDEX_1` to `RADIO_INDEX_3` in
 *                          `wifi_hal_generic.h`, bounded by `MAX_NUM_RADIOS`, whose value
 *                          depends on `WIFI_HAL_VERSION_3`. Which of them a platform
 *                          provisions is not established by this interface.
 * @param[out] output_struct Pointer to a `wifi_radioTrafficStats2_t` structure
 *                           to store the traffic statistics. The caller allocates and
 *                           owns the structure; the `HAL` writes its members and keeps no
 *                           reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the members of `*output_struct` hold the radio's statistics. On
 *       failure this interface does not state whether any member was written, so a caller
 *       must treat the whole structure as unspecified rather than partially valid.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The statistics were retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` names no radio, `output_struct` is NULL, or the
 *                          vendor layer could not supply the statistics. The caller should
 *                          validate its arguments and discard the structure rather than
 *                          publishing its contents as a measurement of zero, then retry on
 *                          its next polling cycle.
 *
 * @note This interface does not state what resets these counters or how often, so a
 *       caller must not assume they are cumulative since boot. `radio_StatisticsStartTime`
 *       is the only reset signal available.
 * @note The `2` suffix marks the second generation of this call. The first-generation
 *       structure `wifi_radioTrafficStats_t` is still declared in
 *       `wifi_hal_deprecated.h`, which this repository declares out of scope, so this is
 *       the form a caller should use.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_radioTrafficStats2_t
 * @see wifi_getSSIDTrafficStats2
 * @see wifi_setRadioStatsEnable
 */
INT wifi_getRadioTrafficStats2(INT radioIndex, wifi_radioTrafficStats2_t *output_struct);

/**
 * @brief Reads one SSID's packet and byte counters.
 *
 * This function retrieves basic traffic statistics for the specified SSID: the byte and
 * packet totals, the unicast, multicast and broadcast breakdowns, the error and discard
 * counts, and the four retransmission counters that distinguish a retry from a failure.
 * Where `wifi_getRadioTrafficStats2()` reports the radio as a whole, this call attributes
 * traffic to a single SSID, which is what makes per-service accounting possible on a
 * radio hosting several.
 *
 * @param[in] ssidIndex     SSID index. This interface defines no named constants for SSID
 *                          indices - `wifi_hal_generic.h` defines `RADIO_INDEX_*` and
 *                          `AP_INDEX_*` but nothing for SSIDs - and it does not state that
 *                          SSID indices share the AP numbering, so a caller must obtain
 *                          the index from the interface that provisioned the SSID rather
 *                          than assuming a range.
 * @param[out] output_struct Pointer to a `wifi_ssidTrafficStats2_t` structure to
 *                           store the traffic statistics. The caller allocates and owns
 *                           the structure; the `HAL` writes its members and keeps no
 *                           reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the members of `*output_struct` hold the SSID's counters. On failure
 *       this interface does not state whether any member was written, so a caller must
 *       treat the whole structure as unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The statistics were retrieved.
 * @retval WIFI_HAL_ERROR   `ssidIndex` names no SSID, `output_struct` is NULL, or the
 *                          vendor layer could not supply the statistics. The caller should
 *                          validate its arguments, discard the structure rather than
 *                          treating it as a zeroed measurement, and retry on its next
 *                          polling cycle.
 *
 * @note This interface does not state what resets these counters or how often, and this
 *       structure carries no start-time member, so a caller cannot detect a reset between
 *       two reads from the result alone.
 * @note The `2` suffix marks the second generation of this call. The first-generation
 *       `wifi_getSSIDTrafficStats()` and its `wifi_ssidTrafficStats_t` structure remain in
 *       `wifi_hal_deprecated.h`, marked deprecated there and out of scope for this
 *       repository, so this is the form a caller should use.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_ssidTrafficStats2_t
 * @see wifi_getRadioTrafficStats2
 */
INT wifi_getSSIDTrafficStats2(INT ssidIndex, wifi_ssidTrafficStats2_t *output_struct);


#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Reports the Access Points a radio can see, optionally scanning first.
 *
 * This function retrieves the status of neighboring Wi-Fi networks as an array of
 * `wifi_neighbor_ap2_t` records, one per Access Point observed - SSID, BSSID, channel,
 * signal strength, security and rate advertisements. It is the input a caller uses for
 * channel selection and neighbour reporting.
 *
 * The results come back through a `HAL`-allocated array, which is an exception to the
 * general rule in `Memory Model` in `docs/pages/halSpec.md` that memory the `HAL` creates
 * stays `HAL`-owned; that topic expressly permits an API to state such an exception, and
 * the ownership statement on `neighbor_ap_array` below is this call's.
 *
 * @param[in] radioIndex        Radio index. The indices this interface defines are
 *                              `RADIO_INDEX_1` to `RADIO_INDEX_3` in
 *                              `wifi_hal_generic.h`, bounded by `MAX_NUM_RADIOS`.
 * @param[in] scan              Whether to start a neighbor scan. `TRUE` asks the `HAL` to
 *                              start one, `FALSE` asks only for what it already holds.
 * @param[out] neighbor_ap_array Pointer to a pointer to an array of
 *                               `wifi_neighbor_ap2_t` structures to store the
 *                               neighboring AP information. The array is allocated by the
 *                               HAL layer and should be freed by the caller. The caller
 *                               passes the address of its own pointer variable, which the
 *                               `HAL` sets to the allocated array, and frees the array as
 *                               a whole rather than element by element. This interface does
 *                               not name the allocator the `HAL` used, so the matching
 *                               release function is a platform convention rather than
 *                               something established here.
 * @param[out] output_array_size Pointer to a variable to store the size of the
 *                               returned array, in elements. The caller allocates and owns
 *                               the variable. On success it holds the number of Access
 *                               Points reported, which may be zero. On failure this
 *                               interface does not state whether either output was
 *                               written, so a caller must treat both as unspecified and
 *                               must not free `*neighbor_ap_array`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*neighbor_ap_array` points at an array of `*output_array_size`
 *       records that the caller is responsible for freeing, and each record describes one
 *       Access Point. On failure neither output may be relied on and nothing may be
 *       freed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The results were retrieved; `*output_array_size` may be zero if
 *                          no Access Point was observed.
 * @retval WIFI_HAL_ERROR   `radioIndex` names no radio, either pointer argument is NULL,
 *                          the allocation failed, or the vendor layer could not supply the
 *                          results. The caller should validate its arguments, free
 *                          nothing, and retry rather than treating the failure as an empty
 *                          neighbourhood.
 *
 * @warning The `HAL` allocates a fresh array on each successful call and nothing in this
 *          interface releases a previous one, so a repeated caller must free the array it
 *          holds before the next call overwrites its pointer. A caller should also
 *          initialise its pointer to NULL beforehand, because a failed call leaves it
 *          unspecified.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`. A scan takes far longer than a
 *       non-blocking call may take, and this interface does not state how a scan started
 *       with `scan` set to `TRUE` completes, nor whether the array returned by that same
 *       call reflects the new scan or results already held. A caller must therefore not
 *       assume the results are those of the scan it just requested;
 *       `wifi_scanResults_callback_register()` in `wifi_hal_radio.h` is the notification
 *       mechanism this interface provides for fresh scan results.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @note This declaration is only compiled when `WIFI_HAL_VERSION_3_PHASE2` is defined.
 *       Without that flag, a same-named call with no `scan` argument is declared in
 *       `wifi_hal_deprecated.h`, which this repository declares out of scope - so the
 *       flag decides both whether this form exists and how many arguments a caller
 *       passes.
 * @see wifi_neighbor_ap2_t
 */
INT wifi_getNeighboringWiFiStatus(INT radioIndex, BOOL scan, wifi_neighbor_ap2_t **neighbor_ap_array, UINT *output_array_size);
#endif

/**
 * @brief Reads how busy a radio's band currently is.
 *
 * This function retrieves the utilization of the radio band as a percentage - a single
 * figure for how much of the air time is occupied, which a caller uses to decide whether
 * a channel is worth staying on. Where `wifi_getRadioTrafficStats2()` returns utilization
 * as one member of a large structure recalculated on a fixed interval, this call answers
 * the same question on its own.
 *
 * @param[in] radioIndex         Radio index. The indices this interface defines are
 *                               `RADIO_INDEX_1` to `RADIO_INDEX_3` in
 *                               `wifi_hal_generic.h`, bounded by `MAX_NUM_RADIOS`.
 * @param[out] output_percentage Pointer to a variable to store the utilization
 *                               percentage. The caller allocates and owns the `INT`; the
 *                               `HAL` writes into it and keeps no reference to it. The
 *                               unit is a percentage of air time. The type is signed and
 *                               this interface does not state the range of the value or
 *                               whether a negative result signals "not yet measured" as
 *                               the interval metrics of `wifi_radioTrafficStats2_t`
 *                               explicitly do, so a caller should range-check the value
 *                               before using it rather than assume 0 to 100.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*output_percentage` holds the utilization figure. On failure this
 *       interface does not state whether it was written, so a caller must treat it as
 *       unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The utilization was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` names no radio, `output_percentage` is NULL, or
 *                          the vendor layer could not supply the figure. The caller should
 *                          validate its arguments and treat utilization as unknown - not
 *                          as zero, which would read as an idle channel.
 *
 * @note This interface does not state the interval the figure is measured over, so two
 *       readings taken in quick succession are not guaranteed to describe disjoint
 *       periods.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioTrafficStats2
 */
INT wifi_getRadioBandUtilization (INT radioIndex, INT *output_percentage);

/**
 * @brief Reads per-client diagnostics for every device associated with an Access Point.
 *
 * This is the client-level telemetry call: one `wifi_associated_dev3_t` record per
 * associated device, carrying signal strength, data rates, per-client byte and packet
 * counters, retransmission counts, spatial-stream and multi-link state, and TWT session
 * information. It is how a caller enumerates who is connected and how well each one is
 * doing.
 *
 * The results come back through a `HAL`-allocated array, an exception to the general rule
 * in `Memory Model` in `docs/pages/halSpec.md` which that topic expressly permits an API
 * to state.
 *
 * This call has a second documented use, described with `wifi_associated_dev3_t` in
 * `wifi_hal_generic.h`: to collect Channel State Information the caller instead allocates
 * `associated_dev_array` itself for `*output_array_size` devices and names each device in
 * that array's `cli_MACAddress`, whereupon the `HAL` must allocate the `cli_CsiData`
 * member of each record and the caller is responsible for releasing it. The two uses
 * invert who allocates the array, so a caller must be deliberate about which one it is
 * making. Note that the description in `wifi_hal_generic.h` writes that member as
 * `cli_CSIData`; the declared name is `cli_CsiData`.
 *
 * @param[in] apIndex              Index of the Access Point. The indices this interface
 *                                 defines are `AP_INDEX_1` to `AP_INDEX_24` in
 *                                 `wifi_hal_generic.h`; which of them a platform
 *                                 provisions is not established by this interface.
 * @param[out] associated_dev_array Pointer to a pointer to an array of
 *                                   `wifi_associated_dev3_t` structures to store
 *                                   the diagnostic results. In the ordinary use the array
 *                                   is allocated by the HAL layer and should be freed by
 *                                   the caller, which passes the address of its own
 *                                   pointer variable for the `HAL` to set. In the CSI use
 *                                   described above the caller allocates the array and
 *                                   the `HAL` allocates only `cli_CsiData` within it.
 * @param[out] output_array_size    Pointer to a variable to store the size of the
 *                                   returned array, in elements. The caller allocates and
 *                                   owns the variable. On success it holds the number of
 *                                   associated devices, which may be zero when none is
 *                                   connected; in the CSI use the caller sets it to the
 *                                   number of devices it has provided. On failure this
 *                                   interface does not state whether either output was
 *                                   written, so a caller must treat both as unspecified
 *                                   and must not free a `HAL`-allocated array.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*associated_dev_array` holds `*output_array_size` records, one per
 *       associated device. On failure neither output may be relied on and nothing may be
 *       freed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The diagnostics were retrieved; `*output_array_size` may be
 *                          zero if no device is associated.
 * @retval WIFI_HAL_ERROR   `apIndex` names no Access Point, either pointer argument is
 *                          NULL, the allocation failed, or the vendor layer could not
 *                          supply the diagnostics. The caller should validate its
 *                          arguments, free nothing, and retry rather than concluding that
 *                          no client is connected.
 *
 * @warning A successful call in the ordinary use hands the caller an array it must free,
 *          and nothing in this interface releases a previous one, so a repeated caller
 *          leaks unless it frees the array it holds before the next call overwrites its
 *          pointer.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @note The `3` suffix marks the third generation of this call, matching the
 *       `wifi_associated_dev3_t` record it fills. The first and second generations remain
 *       in `wifi_hal_deprecated.h`, which this repository declares out of scope, so this
 *       is the form a caller should use.
 * @see wifi_getApAssociatedClientDiagnosticResult
 */
INT wifi_getApAssociatedDeviceDiagnosticResult3(INT apIndex, wifi_associated_dev3_t **associated_dev_array, UINT *output_array_size);

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Reads the diagnostics of one named associated client.
 *
 * This is the single-client counterpart of
 * `wifi_getApAssociatedDeviceDiagnosticResult3()`: it fills one `wifi_associated_dev3_t`
 * record for the client the caller names, instead of returning a record for every
 * associated device. A caller polling one known client should prefer it, because it needs
 * no allocation and no array walk.
 *
 * @param[in] apIndex    Index of the Access Point. The indices this interface defines are
 *                       `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`.
 * @param[in] mac_addr   MAC address of the associated client, as the six raw octets of
 *                       `mac_address_t` in `wifi_hal_generic.h` - not a printable string.
 *                       The array is passed by reference as C arrays always are, and the
 *                       `HAL` reads it during the call; this interface does not state that
 *                       the `HAL` modifies it, and a caller has no reason to expect it to.
 * @param[out] dev_conn  Pointer to a `wifi_associated_dev3_t` structure to store
 *                       the diagnostic results. The caller allocates and owns the
 *                       structure; the `HAL` writes into it and keeps no reference to it
 *                       after returning. Note that a `wifi_associated_dev3_t` contains the
 *                       `cli_CsiData` pointer member, and this interface does not state
 *                       whether this call populates it, so a caller must not assume it is
 *                       either valid or NULL.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*dev_conn` describes the named client. On failure this interface does
 *       not state whether any member was written, so a caller must treat the whole
 *       structure as unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The diagnostics were retrieved for the named client.
 * @retval WIFI_HAL_ERROR   `apIndex` names no Access Point, `dev_conn` is NULL, the named
 *                          client is not associated with this Access Point, or the vendor
 *                          layer could not supply the diagnostics. This interface does not
 *                          distinguish those causes, so a caller cannot read the result as
 *                          proof that the client has disconnected; it should validate its
 *                          arguments and confirm the client's presence with
 *                          `wifi_getApAssociatedDeviceDiagnosticResult3()` before drawing
 *                          that conclusion.
 *
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @note This declaration is only compiled when `WIFI_HAL_VERSION_3_PHASE2` is defined.
 *       Without that flag a same-named call taking `char *mac_addr` instead of
 *       `mac_address_t` is declared in `wifi_hal_deprecated.h`, which this repository
 *       declares out of scope - so the flag decides not only whether this form exists but
 *       how the client address is passed.
 * @see wifi_getApAssociatedDeviceDiagnosticResult3
 */
INT wifi_getApAssociatedClientDiagnosticResult(INT apIndex, mac_address_t mac_addr, wifi_associated_dev3_t *dev_conn);
#endif

/**
 * @brief Turns detailed per-client statistics collection on or off for a radio.
 *
 * This function enables or disables the collection of detailed statistics
 * for clients associated with the specified radio. The control is per radio, not per
 * Access Point, so it covers every client on every VAP that radio hosts. Detailed
 * collection costs the vendor layer work it need not do when nobody is reading the
 * figures, which is why it is switchable at all.
 *
 * @param[in] radioIndex  Index of the radio. The indices this interface defines are
 *                        `RADIO_INDEX_1` to `RADIO_INDEX_3` in `wifi_hal_generic.h`,
 *                        bounded by `MAX_NUM_RADIOS`.
 * @param[in] enable      Whether to enable (true) or disable (false) the collection
 *                        of detailed statistics. `TRUE` and `FALSE` are defined in
 *                        `wifi_hal_generic.h`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success detailed per-client statistics collection holds the requested state for
 *       the named radio. On failure the state is unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The requested state was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` names no radio, or the vendor layer could not
 *                          apply the change. The caller should validate its argument and
 *                          treat the collection state as unknown; this interface provides
 *                          no getter for it, so the state cannot be read back.
 *
 * @note This interface exposes no counterpart getter for this setting, unlike
 *       `wifi_setRadioStatsEnable()`, which pairs with `wifi_getRadioStatsEnable()`. A
 *       caller that needs to know the current state must track what it last set.
 * @note This interface does not state which statistics this setting governs, nor what a
 *       per-client getter such as `wifi_getApAssociatedDeviceDiagnosticResult3()` reports
 *       while collection is disabled, so a caller must not assume that enabling this is a
 *       pre-condition of those calls or that their results are meaningless without it.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`. Since the change is applied
 *       without blocking, this interface does not state when collection actually starts.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApAssociatedDeviceDiagnosticResult3
 * @see wifi_setRadioStatsEnable
 */
INT wifi_setClientDetailedStatisticsEnable(INT radioIndex, BOOL enable);

/**
 * @brief Reads whether radio statistics collection is currently enabled.
 *
 * This function retrieves the enable status of radio statistics collection for one radio -
 * the state that `wifi_setRadioStatsEnable()` writes. It reports the configured setting,
 * which is what lets a caller restore it rather than overwrite a setting another component
 * relies on.
 *
 * @param[in] radioIndex    Index of the radio. The indices this interface defines are
 *                          `RADIO_INDEX_1` to `RADIO_INDEX_3` in `wifi_hal_generic.h`,
 *                          bounded by `MAX_NUM_RADIOS`.
 * @param[out] output_enable Pointer to a variable to store the enable status
 *                           (true if enabled, false if disabled). The caller allocates and
 *                           owns the `BOOL`; the `HAL` writes into it and keeps no
 *                           reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*output_enable` holds the current setting. On failure this interface
 *       does not state whether it was written, so a caller must treat it as unspecified
 *       rather than as `FALSE`.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was retrieved.
 * @retval WIFI_HAL_ERROR   `radioIndex` names no radio, `output_enable` is NULL, or the
 *                          vendor layer could not supply the setting. The caller should
 *                          validate its arguments and treat the setting as unknown; it must
 *                          not fall back to setting a value it has not read, which would
 *                          silently change another component's configuration.
 *
 * @note This interface does not state whether the setting persists across a restart;
 *       `Persistence Model` in `docs/pages/halSpec.md` places configuration persistence
 *       with the upper layer.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setRadioStatsEnable
 */
INT wifi_getRadioStatsEnable(INT radioIndex, BOOL *output_enable);

/**
 * @brief Turns radio statistics collection on or off.
 *
 * This function enables or disables the collection of radio statistics for one radio - the
 * radio-level counterpart of `wifi_setClientDetailedStatisticsEnable()`, and the setting
 * `wifi_getRadioStatsEnable()` reads back. Collection is switchable because it costs the
 * vendor layer work that is wasted when nothing is polling.
 *
 * @param[in] radioIndex  Index of the radio. The indices this interface defines are
 *                        `RADIO_INDEX_1` to `RADIO_INDEX_3` in `wifi_hal_generic.h`,
 *                        bounded by `MAX_NUM_RADIOS`.
 * @param[in] enable      Whether to enable (true) or disable (false) radio statistics
 *                        collection. `TRUE` and `FALSE` are defined in
 *                        `wifi_hal_generic.h`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success radio statistics collection holds the requested state for the named
 *       radio, and `wifi_getRadioStatsEnable()` reports it. On failure the state is
 *       unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The requested state was applied.
 * @retval WIFI_HAL_ERROR   `radioIndex` names no radio, or the vendor layer could not
 *                          apply the change. The caller should read the state back with
 *                          `wifi_getRadioStatsEnable()` rather than retrying blindly or
 *                          assuming the state it asked for.
 *
 * @note This interface does not state what `wifi_getRadioTrafficStats2()` reports while
 *       collection is disabled, nor whether disabling and re-enabling collection resets
 *       the counters. A caller must therefore neither treat this call as a documented
 *       pre-condition of reading statistics, nor use it as a way to zero them.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`. Because the call returns without
 *       blocking, this interface does not state when collection actually starts or stops.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioStatsEnable
 * @see wifi_getRadioTrafficStats2
 * @see wifi_setClientDetailedStatisticsEnable
 */
INT wifi_setRadioStatsEnable(INT radioIndex, BOOL enable);

/**
 * @brief Reads the transmit-overflow telemetry of one VAP.
 *
 * This call reports the telemetry a virtual Access Point keeps in its own right, as
 * distinct from the traffic counters of the SSID it carries or of the radio beneath it. In
 * the shipped `wifi_VAPTelemetry_t` that is the transmit overflow count, which indicates
 * the VAP could not absorb offered traffic - a different failure from the error and
 * discard counters of `wifi_getSSIDTrafficStats2()`.
 *
 * @param[in] apIndex   Access Point index. Note that this call declares the index as
 *                      `UINT` where every other index in this header is `INT`; the value
 *                      space is the same. The indices this interface defines are
 *                      `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`, and
 *                      `MAX_VAP` above gives the interface-wide ceiling on the number of
 *                      VAPs.
 * @param[out] telemetry Pointer to a `wifi_VAPTelemetry_t` structure to store the
 *                       VAP telemetry data. The caller allocates and owns the structure;
 *                       the `HAL` writes into it and keeps no reference to it after
 *                       returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*telemetry` holds the VAP's telemetry. On failure this interface does
 *       not state whether it was written, so a caller must treat it as unspecified.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The telemetry was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` names no VAP, `telemetry` is NULL, or the vendor
 *                          layer could not supply the telemetry. The caller should validate
 *                          its arguments and discard the structure rather than reporting a
 *                          zero overflow count, which would read as a healthy VAP.
 *
 * @note This interface does not state what resets `txOverflow` or what happens when the
 *       `UINT` wraps, so a caller should compare successive readings rather than treat one
 *       value as an absolute total.
 * @note This function must not suspend and must not invoke any blocking system calls;
 *       see `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_VAPTelemetry_t
 * @see wifi_getSSIDTrafficStats2
 */
INT wifi_getVAPTelemetry(UINT apIndex, wifi_VAPTelemetry_t *telemetry);

/**
* @brief Measures the link quality of a station that is not associated to this AP.
*
* Retrieves link metrics (RCPI) for a given unassociated station.
* The caller provides the STA MAC, operating class, and channel.
* The HAL implementation converts these to vendor-specific radio parameters,
* performs the measurement, and returns the result as RCPI.
*
* This is the one call in this header that measures a device which is *not* a client of
* this Access Point, which is what makes it useful for steering and for assessing a
* neighbouring network's clients. Every other per-client call here reports devices that are
* associated.
*
* @param[in] apIndex AP/VAP index identifying the AP instance to query. The indices this
*                    interface defines are `AP_INDEX_1` to `AP_INDEX_24` in
*                    `wifi_hal_generic.h`.
* @param[in] params Request parameters (STA MAC, operating class, channel). The caller
*                   allocates and owns the structure. The pointer is `const`, so the `HAL`
*                   reads the request and does not modify it, and a caller may reuse the
*                   same instance across calls. The `HAL` keeps no reference to it after
*                   returning.
* @param[out] sta_info Output structure filled with measurement result. The caller
*                      allocates and owns the storage and the `HAL` writes all four
*                      members; the first three restate the request context and `rcpi`
*                      carries the measurement.
*
* @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
*      in `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
*      requirements and, per `Component Runtime Execution Requirements`, likely results in
*      undefined behaviour.
* @pre `params` must point to valid memory and `sta_info` must point to valid writable
*      memory; neither may be NULL.
* @pre The STA must not be currently associated to the queried AP. This interface does not
*      state what a measurement of an associated station returns, so a caller holding an
*      association should use `wifi_getApAssociatedClientDiagnosticResult()` instead.
* @post On success `sta_info->rcpi` holds the measurement and the other three members
*       identify what was measured. On failure this interface does not state whether any
*       member was written, so a caller must treat the whole structure as unspecified.
*
* @returns Status code.
* @retval WIFI_HAL_SUCCESS Operation completed successfully.
* @retval WIFI_HAL_ERROR Operation failed (invalid args, unsupported, internal error). The
*                        causes are not distinguished, so a caller cannot tell an
*                        unsupported platform from a station that could not be heard; it
*                        should validate its arguments, treat the metric as unavailable, and
*                        not retry a request the platform may not implement at all.
*
* @note This function must not suspend and must not invoke any blocking system calls;
*       see `Blocking calls` in `docs/pages/halSpec.md`. Measuring an unassociated station
*       requires off-channel radio time, and this interface does not state how that is
*       reconciled with returning immediately, nor whether a successful return means the
*       measurement was just taken or was already held.
* @note The `HAL` is expected to be thread safe, per `Threading Model` in
*       `docs/pages/halSpec.md`.
* @see wifi_na_sta_req_params_t
* @see wifi_na_sta_info_t
* @see wifi_getApAssociatedClientDiagnosticResult
*/
INT wifi_getNASta(INT apIndex, const wifi_na_sta_req_params_t *params, wifi_na_sta_info_t *sta_info);

/** @} */  //END OF GROUP WIFI_HAL_APIS

#ifdef __cplusplus
}
#endif

#endif
