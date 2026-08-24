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
#ifndef __WIFI_HAL_AP_H__
#define __WIFI_HAL_AP_H__

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

/**
 * @brief Wi-Fi traffic statistics.
 */
typedef struct _wifi_trafficStats
{
    ULONG wifi_ErrorsSent;            /**< Number of errors sent. */
    ULONG wifi_ErrorsReceived;         /**< Number of errors received. */
    ULONG wifi_UnicastPacketsSent;    /**< Number of unicast packets sent. */
    ULONG wifi_UnicastPacketsReceived; /**< Number of unicast packets received. */
    ULONG wifi_DiscardedPacketsSent;   /**< Number of discarded packets sent. */
    ULONG wifi_DiscardedPacketsReceived; /**< Number of discarded packets received. */
    ULONG wifi_MulticastPacketsSent;   /**< Number of multicast packets sent. */
    ULONG wifi_MulticastPacketsReceived; /**< Number of multicast packets received. */
    ULONG wifi_BroadcastPacketsSent;   /**< Number of broadcast packets sent. */
    ULONG wifi_BroadcastPacketsRecevied; /**< Number of broadcast packets received. */
    ULONG wifi_UnknownPacketsReceived; /**< Number of unknown packets received. */
} wifi_trafficStats_t;

/**
 * @brief IPv4 field values.
 */
typedef enum
{
    wifi_ipv4_field_values_not_available, /**< IPv4 field values are not available. */
    wifi_ipv4_field_values_available, /**< IPv4 field values are available. */
    wifi_ipv4_field_values_post_restricted, /**< IPv4 field values are restricted after NAT. */
    wifi_ipv4_field_values_single_nated_private, /**< IPv4 field values are private and have undergone single NAT. */
    wifi_ipv4_field_values_double_nated_private, /**< IPv4 field values are private and have undergone double NAT. */
    wifi_ipv4_field_values_port_restricted_single_nated, /**< IPv4 field values are port-restricted and have undergone single NAT. */
    wifi_ipv4_field_values_port_restricted_double_nated, /**< IPv4 field values are port-restricted and have undergone double NAT. */
    wifi_ipv4_field_values_not_known /**< IPv4 field values are not known. */
} wifi_ipv4_field_values_t;

/**
 * @brief IPv6 field values.
 */
typedef enum
{
    wifi_ipv6_field_values_not_available, /**< IPv6 field values are not available. */
    wifi_ipv6_field_values_available, /**< IPv6 field values are available. */
    wifi_ipv6_field_values_not_known /**< IPv6 field values are not known. */
} wifi_ipv6_field_values_t;

/**
 * @brief IP address availability.
 */
typedef struct
{
    UCHAR field_format; /**< Field format. */
} __attribute__((packed)) wifi_ipAddressAvailabality_t;

/**
 * @brief Authentication ID values.
 */
typedef enum
{
    wifi_auth_id_reserved, /**< Reserved. */
    wifi_auth_id_expanded_eap, /**< Expanded EAP. */
    wifi_auth_id_inner_auth_eap, /**< Inner authentication EAP. */
    wifi_auth_id_expanded_inner_auth_eap, /**< Expanded inner authentication EAP. */
    wifi_auth_id_credential_type, /**< Credential type. */
    wifi_auth_id_tunneled_eap, /**< Tunneled EAP. */
} wifi_auth_id_t;

/**
 * @brief Authentication method.
 */
typedef struct
{
    UCHAR id; /**< ID. */
    UCHAR length; /**< Length. */
    UCHAR val[16]; /**< Value. */
} __attribute__((packed)) wifi_authMethod_t;

/**
 * @brief EAP method.
 */
typedef struct
{
    UCHAR length; /**< Length. */
    UCHAR method; /**< Method. */
    UCHAR auth_param_count; /**< Number of authentication parameters. */
    wifi_authMethod_t auth_method[16]; /**< Authentication methods. */
} __attribute__((packed)) wifi_eapMethod_t;

/**
 * @brief NAI realm information.
 */
typedef struct
{
    USHORT data_field_length; /**< Data field length. */
    UCHAR encoding; /**< Encoding. */
    UCHAR realm_length; /**< Realm length. */
    UCHAR realm[256]; /**< Realm. */
    UCHAR eap_method_count; /**< Number of EAP methods. */
    wifi_eapMethod_t eap_method[16]; /**< EAP methods. */
} __attribute__((packed)) wifi_naiRealm_t;

/**
 * @brief NAI realm element.
 */
typedef struct
{
    USHORT nai_realm_count; /**< Number of NAI realms. */
    wifi_naiRealm_t nai_realm_tuples[20]; /**< NAI realm tuples. */
} __attribute__((packed)) wifi_naiRealmElement_t;

/**
 * @brief Venue name.
 */
typedef struct
{
    UCHAR length; /**< Length. */
    UCHAR language[3]; /**< Language. */
    UCHAR name[256]; /**< Name. */
} __attribute__((packed)) wifi_venueName_t;

/**
 * @brief Venue name element.
 */
typedef struct
{
    UCHAR venueGroup; /**< Venue group. */
    UCHAR venueType; /**< Venue type. */
    wifi_venueName_t venueNameTuples[16]; /**< Venue name tuples. */
} __attribute__((packed)) wifi_venueNameElement_t;

/**
 * @brief PLMN (Public Land Mobile Network) information.
 */
typedef struct
{
    UCHAR PLMN[3]; /**< PLMN identifier. */
} __attribute__((packed)) wifi_plmn_t;

/**
 * @brief 3GPP PLMN list information element.
 */
typedef struct
{
    UCHAR iei; /**< Information Element Identifier (copy zero for now). */
    UCHAR plmn_length; /**< Length of the PLMN list. */
    UCHAR number_of_plmns; /**< Number of PLMNs in the list. */
    wifi_plmn_t plmn[16]; /**< PLMNs. */
} __attribute__((packed)) wifi_3gpp_plmn_list_information_element_t;

/**
 * @brief 3GPP cellular network information.
 */
typedef struct
{
    UCHAR gud; /**< Globally Unique Identifier. */
    UCHAR uhdLength; /**< Length of the remaining fields. */
    wifi_3gpp_plmn_list_information_element_t plmn_information; /**< PLMN list information element. */
} __attribute__((packed)) wifi_3gppCellularNetwork_t;

/**
 * @brief Domain name tuple.
 */
typedef struct
{
    UCHAR length; /**< Length. */
    UCHAR domainName[255]; /**< Domain name (maximum allowed based on the specification). */
} __attribute__((packed)) wifi_domainNameTuple_t;

/**
 * @brief Domain name.
 */
typedef struct
{
    wifi_domainNameTuple_t domainNameTuple[4]; /**< Domain name tuples. */
} __attribute__((packed)) wifi_domainName_t;

/**
 * @brief OUI (Organizationally Unique Identifier) duple.
 */
typedef struct
{
    UCHAR length; /**< Length. */
    UCHAR oui[15]; /**< OUI. */
} __attribute__((packed)) wifi_ouiDuple_t;

/**
 * @brief Roaming consortium.
 */
typedef struct
{
    wifi_ouiDuple_t ouiDuple[32]; /**< OUI duples. */
} __attribute__((packed)) wifi_roamingConsortium_t;

/**
 * @brief Capability list ANQP.
 */
typedef struct
{
    USHORT capabilityList[64]; /**< Capability list. */
} __attribute__((packed)) wifi_capabilityListANQP_t;

/**
 * @brief Roaming consortium element.
 */
typedef struct
{
    UCHAR wifiRoamingConsortiumCount; /**< Number of roaming consortia. */
    UCHAR wifiRoamingConsortiumOui[3][15 + 1]; /**< Only 3 OIs are allowed in beacon and probe responses. OI length is variable between 3 and 15. */
    UCHAR wifiRoamingConsortiumLen[3]; /**< Lengths of the roaming consortia OIs. */
} __attribute__((packed)) wifi_roamingConsortiumElement_t;


// HS2 Related ANQP Elements start


// =========================================Start-HS2-Operator Friendly Name=========================================================================
// HS2.0 Operator Name Duple #1     HS2.0 Operator Name Duple #2        ......    HS2.0 Operator Name Duple #n
//           variable                            variable                                      variable
// HS2.0 Operator name Duple
// Length                                  Language Code                                     Operator Name
//    1   (3+ operator name)                   3                                               variable

/**
 * @brief HS2.0 Operator Name Duple (figure 9-595).
 */
typedef struct _wifi_HS2_OperatorNameDuple_t
{
    UCHAR length; /**< Length is 3 (language code) + number of octets in operator name field. For example, if operatorName = "aaaa", length is 4 + 3 = 7. */
    UCHAR languageCode[3]; /**< Language code. */
    UCHAR operatorName[252]; /**< Operator name (based on the specification, the maximum length is 252). */
} __attribute__((packed)) wifi_HS2_OperatorNameDuple_t;

/**
 * @brief HS2.0 Operator Friendly Name.
 */
typedef struct
{
    wifi_HS2_OperatorNameDuple_t operatorNameDuple[16]; /**< Operator name duples (16 duples are supported for now). */
} __attribute__((packed)) wifi_HS2_OperatorFriendlyName_t;

// =========================================End-HS2-Operator Friendly Name=========================================================================

// =========================================Start-HS2-WAN Metrics Element=========================================================================
// WAN Info                 Downlink Speed           Uplink Speed     Downlink Load       Uplink Load        LMD
//    1                            4                       4                  1                1               2

/**
 * @brief HS2.0 WAN Metrics Element (figure 9-595).
 */
typedef struct
{
    UCHAR wanInfo; /**< WAN information. */
    UINT downLinkSpeed; /**< Downlink speed in kbps. */
    UINT upLinkSpeed; /**< Uplink speed in kbps. */
    UCHAR downLinkLoad; /**< Downlink load as a percentage. */
    UCHAR upLinkLoad; /**< Uplink load as a percentage. */
    USHORT lmd; /**< Load Measurement Duration in seconds. */
} __attribute__((packed)) wifi_HS2_WANMetrics_t;

/**
 * @brief WAN Info bit field values.
 *
 * Bits:
 *  - B0-B1: Link Status
 *  - B2: Symmetric Link
 *  - B3: At Capacity
 *  - B4-B7: Reserved
 */
typedef enum
{
    wifi_hs2_wan_info_reserved, /**< Reserved. */
    wifi_hs2_wan_info_linkup, /**< Link is up. */
    wifi_hs2_wan_info_linkdown, /**< Link is down. */
    wifi_hs2_wan_info_link_in_test_state /**< Link is in test state. */
} wifi_HS2_Wan_Info_Link_Status_t;

// =========================================End-HS2-WAN Metrics Element=========================================================================

// =========================================Start-HS2-Connection Capability Element=========================================================================
// Proto Port Tuple #1        Proto Port Tuple #2  ............. Proto Port Tuple #n
//          4                        4(optional)                   4(optional)
// Proto Port Tuple Format
// IP Protocol                             Port Number                  Status
//      1                                       2                         1

/**
 * @brief HS2.0 Protocol and Port Tuple (figure 9-595).
 */
typedef struct
{
    UCHAR ipProtocol; /**< IP protocol. */
    USHORT portNumber; /**< Port number. */
    UCHAR status; /**< Status. */
} __attribute__((packed)) wifi_HS2_Proto_Port_Tuple_t;

/**
 * @brief HS2.0 Connection Capability Element (figure 9-595).
 */
typedef struct
{
    wifi_HS2_Proto_Port_Tuple_t protoPortTuple[16]; /**< Protocol and port tuples (16 tuples are supported for now). */
} __attribute__((packed)) wifi_HS2_ConnectionCapability_t;

/**
 * @brief HS2.0 Connection Capability Status values.
 */
typedef enum
{
    wifi_hs2_connection_capability_closed, /**< Closed. */
    wifi_hs2_connection_capability_open, /**< Open. */
    wifi_hs2_connection_capability_unknown, /**< Unknown. */
    wifi_hs2_connection_capability_reserved /**< Reserved. */
} wifi_HS2_ConnectionCapability_Status_t;

// =========================================End-HS2-Connection Capability Element=========================================================================

// =========================================Start-HS2-NAI Realm Query Element=========================================================================

// NAI Realm Count                          NAI Home Realm                    NAI Home Realm   ....  NAI Home Realm
//                                         Name Data #1                        Name Data #2            Name Data #n
//       1                                   variable                         (variable optional)       (variable optional)

// NAI Realm Encoding                                 NAI Home Realm Name Length              NAI Home Realm
//          1                                                      1                               variable

/**
 * @brief HS2.0 NAI Home Realm Data (figure 9-595).
 */
typedef struct
{
    UCHAR encoding; /**< Encoding. */
    UCHAR length; /**< Length. */
    UCHAR name[255]; /**< Name (maximum length is 255 according to the specification). */
} __attribute__((packed)) wifi_HS2_NAI_Home_Realm_Data_t;

/**
 * @brief HS2.0 NAI Home Realm Query (figure 9-595).
 */
typedef struct
{
    UCHAR realmCount; /**< Realm count. */
    wifi_HS2_NAI_Home_Realm_Data_t homeRealmData[20]; /**< Home realm data (up to 255 realms are supported). */
} __attribute__((packed)) wifi_HS2_NAI_Home_Realm_Query_t;

// =========================================End-HS2-NAI Realm Query Element=========================================================================

// =========================================Start-HS2-Capability List=========================================================================
// HS2.0 Capability #1     HS2.0 Capability #2        ......    HS2.0 Capability #n
//             1             0 or 1 (optional)                      0 or 1 (optional)
// =========================================End-HS2-Capability List=========================================================================

/**
 * @brief HS2.0 Capability List.
 */
typedef struct
{
    UCHAR capabilityList[64]; /**< Capability list. */
} __attribute__((packed)) wifi_HS2_CapabilityList_t;

/**
 * @brief EAPOL key frame.
 */
typedef struct
{
    unsigned char descriptor; /**< Descriptor type. */
    unsigned char key_info[2]; /**< Key information. */
    unsigned short key_len; /**< Key length. */
    unsigned char replay[8]; /**< Replay counter. */
    unsigned char nonce[32]; /**< Nonce. */
    unsigned char init_vector[16]; /**< Initialization vector. */
    unsigned char rsc[8]; /**< Receive Sequence Counter. */
    unsigned char key_id[8]; /**< Key identifier. */
    unsigned char mic[16]; /**< Message Integrity Check. */
    unsigned short len; /**< Length of the data. */
    unsigned char data[0]; /**< Data. */
} wifi_eapol_key_frame_t;

/**
 * @brief EAP codes.
 */
typedef enum
{
    wifi_eap_code_request = 1, /**< Request. */
    wifi_eap_code_response, /**< Response. */
    wifi_eap_code_success, /**< Success. */
    wifi_eap_code_failure, /**< Failure. */
} wifi_eap_code_t;

/**
 * @brief EAP frame.
 */
typedef struct
{
    unsigned char code; /**< Code. */
    unsigned char id; /**< ID. */
    unsigned short len; /**< Length of the data. */
    unsigned char data[0]; /**< Data. */
} __attribute__((__packed__)) wifi_eap_frame_t;

/**
 * @brief EAPOL types.
 */
typedef enum
{
    wifi_eapol_type_eap_packet, /**< EAP packet. */
    wifi_eapol_type_eapol_start, /**< EAPOL start. */
    wifi_eapol_type_eapol_logoff, /**< EAPOL logoff. */
    wifi_eapol_type_eapol_key, /**< EAPOL key. */
} wifi_eapol_type_t;

/**
 * @brief Identifies which message of the WPA four-way handshake is being reported.
 *
 * The four EAPOL-Key messages M1 to M4 are exchanged in order, so this value tells a
 * caller how far a client progressed before a handshake stalled.
 */
typedef enum {
    EAPOL_MSG_NONE = 0, /**< No handshake message; the exchange has not started. */
    EAPOL_MSG_M1,       /**< M1: ANonce sent by the authenticator. */
    EAPOL_MSG_M2,       /**< M2: SNonce and MIC returned by the supplicant. */
    EAPOL_MSG_M3,       /**< M3: GTK and MIC sent by the authenticator. */
    EAPOL_MSG_M4        /**< M4: final acknowledgement from the supplicant. */
} eapol_msg_type_t;

/**
 * @brief Identifies which management exchange an EAPOL handshake belongs to.
 *
 * A four-way handshake follows either an initial association or a reassociation, and
 * the two are counted separately.
 */
typedef enum {
    EAPOL_FRAME_UNKNOWN = 0, /**< The originating exchange could not be determined. */
    EAPOL_FRAME_ASSOC,       /**< The handshake followed an association request. */
    EAPOL_FRAME_REASSOC      /**< The handshake followed a reassociation request. */
} eapol_frame_type_t;

/**
 * @brief Index into per-message EAPOL handshake statistics.
 *
 * Combines the handshake message with the exchange it belongs to, giving one counter
 * slot per message-and-exchange pair. `EAPOL_STATUS_TYPE_MAX` is the element count
 * and is not itself a valid index.
 */
typedef enum {
    M1_ASSOC = 0,          /**< M1 seen during an association exchange. */
    M1_REASSOC,            /**< M1 seen during a reassociation exchange. */
    M2_ASSOC,              /**< M2 seen during an association exchange. */
    M2_REASSOC,            /**< M2 seen during a reassociation exchange. */
    M3_ASSOC,              /**< M3 seen during an association exchange. */
    M3_REASSOC,            /**< M3 seen during a reassociation exchange. */
    EAPOL_STATUS_TYPE_MAX  /**< Number of index values; not a valid index itself. */
} eapol_status_type_idx_t;

/**
 * @brief IEEE 802.1X frame header followed by its payload.
 *
 * The `data` member is a zero-length array, so an instance is only meaningful when it
 * overlays a buffer of at least `sizeof(wifi_8021x_frame_t) + len` bytes; `len` gives
 * the payload length that follows the header. The structure is packed to match the
 * on-the-wire layout, so it must not be copied field-by-field into a padded
 * equivalent.
 */
typedef struct
{
    unsigned char version; /**< IEEE 802.1X protocol version from the frame header. */
    unsigned char type; /**< EAPOL packet type; see `wifi_eapol_type_t`. */
    unsigned short len; /**< Length in bytes of the payload that follows this header. */
    unsigned char data[0]; /**< Start of the payload; `len` bytes, not counted in this structure's size. */
} __attribute__((__packed__)) wifi_8021x_frame_t;

/**
 * @brief Wi-Fi direction values.
 */
typedef enum
{
    wifi_direction_unknown, /**< Unknown direction. */
    wifi_direction_uplink, /**< Uplink. */
    wifi_direction_downlink /**< Downlink. */
} wifi_direction_t;

/**
 * @brief RADIUS Server information.
 *
 * Structure that holds the RADIUS server settings.
 */
typedef struct _wifi_radius_setting_t
{
    INT RadiusServerRetries;           /**< Number of retries for RADIUS requests. */
    INT RadiusServerRequestTimeout;    /**< RADIUS request timeout in seconds after which the request must be retransmitted for the number of retries available. */
    INT PMKLifetime;                   /**< Default time in seconds after which a Wi-Fi client is forced to re-authenticate (default: 8 hours). */
    BOOL PMKCaching;                  /**< Enable or disable caching of PMK. */
    INT PMKCacheInterval;              /**< Time interval in seconds after which the PMKSA (Pairwise Master Key Security Association) cache is purged (default: 5 minutes). */
    INT MaxAuthenticationAttempts;     /**< Indicates the number of times a client can attempt to log in with incorrect credentials. When this limit is reached, the client is blacklisted and not allowed to attempt logging into the network. Setting this parameter to 0 (zero) disables the blacklisting feature. */
    INT BlacklistTableTimeout;         /**< Time interval in seconds for which a client will continue to be blacklisted once it is marked so. */
    INT IdentityRequestRetryInterval;  /**< Time interval in seconds between identity requests retries. A value of 0 (zero) disables it. */
    INT QuietPeriodAfterFailedAuthentication; /**< The enforced quiet period (time interval) in seconds following failed authentication. A value of 0 (zero) disables it. */
    //UCHAR RadiusSecret[64];         //<! The secret used for handshaking with the RADIUS server [RFC2865]. When read, this parameter returns an empty string, regardless of the actual value.

} wifi_radius_setting_t;

/**
 * @brief Represents the Wi-Fi scan modes.
 */
typedef enum
{
    WIFI_RADIO_SCAN_MODE_NONE = 0,    /**< No scan. */
    WIFI_RADIO_SCAN_MODE_FULL,      /**< Full scan. */
    WIFI_RADIO_SCAN_MODE_ONCHAN,    /**< On-channel scan. */
    WIFI_RADIO_SCAN_MODE_OFFCHAN,   /**< Off-channel scan. */
    WIFI_RADIO_SCAN_MODE_SURVEY,     /**< Survey scan. */
    WIFI_RADIO_SCAN_MODE_SELECT_CHANNELS    /**< Selected channels scan. */
} wifi_neighborScanMode_t;

/**
 * @brief EAP types.
 */
typedef enum
{
    WIFI_EAP_TYPE_NONE = 0,        /**< No EAP type. */
    WIFI_EAP_TYPE_IDENTITY = 1,    /**< EAP-Identity (RFC 3748). */
    WIFI_EAP_TYPE_NOTIFICATION = 2, /**< EAP-Notification (RFC 3748). */
    WIFI_EAP_TYPE_NAK = 3,        /**< EAP-NAK (Response only, RFC 3748). */
    WIFI_EAP_TYPE_MD5 = 4,         /**< EAP-MD5 (RFC 3748). */
    WIFI_EAP_TYPE_OTP = 5,         /**< EAP-OTP (RFC 3748). */
    WIFI_EAP_TYPE_GTC = 6,         /**< EAP-GTC (RFC 3748). */
    WIFI_EAP_TYPE_TLS = 13,        /**< EAP-TLS (RFC 2716). */
    WIFI_EAP_TYPE_LEAP = 17,       /**< EAP-LEAP (Cisco proprietary). */
    WIFI_EAP_TYPE_SIM = 18,        /**< EAP-SIM (RFC 4186). */
    WIFI_EAP_TYPE_TTLS = 21,       /**< EAP-TTLS (RFC 5281). */
    WIFI_EAP_TYPE_AKA = 23,        /**< EAP-AKA (RFC 4187). */
    WIFI_EAP_TYPE_PEAP = 25,       /**< EAP-PEAP (draft-josefsson-pppext-eap-tls-eap-06.txt). */
    WIFI_EAP_TYPE_MSCHAPV2 = 26,    /**< EAP-MSCHAPv2 (draft-kamath-pppext-eap-mschapv2-00.txt). */
    WIFI_EAP_TYPE_TLV = 33,        /**< EAP-TLV (draft-josefsson-pppext-eap-tls-eap-07.txt). */
    WIFI_EAP_TYPE_TNC = 38,        /**< EAP-TNC (TNC IF-T v1.0-r3). Note: Tentative assignment; type 38 has previously been allocated for EAP-HTTP Digest (funk.com). */
    WIFI_EAP_TYPE_FAST = 43,       /**< EAP-FAST (RFC 4851). */
    WIFI_EAP_TYPE_PAX = 46,        /**< EAP-PAX (RFC 4746). */
    WIFI_EAP_TYPE_PSK = 47,        /**< EAP-PSK (RFC 4764). */
    WIFI_EAP_TYPE_SAKE = 48,       /**< EAP-SAKE (RFC 4763). */
    WIFI_EAP_TYPE_IKEV2 = 49,      /**< EAP-IKEv2 (RFC 5106). */
    WIFI_EAP_TYPE_AKA_PRIME = 50,   /**< EAP-AKA' (RFC 5448). */
    WIFI_EAP_TYPE_GPSK = 51,       /**< EAP-GPSK (RFC 5433). */
    WIFI_EAP_TYPE_PWD = 52,        /**< EAP-PWD (RFC 5931). */
    WIFI_EAP_TYPE_EKE = 53,        /**< EAP-EKE (RFC 6124). */
    WIFI_EAP_TYPE_TEAP = 55,       /**< EAP-TEAP (RFC 7170). */
    WIFI_EAP_TYPE_EXPANDED = 254   /**< EAP-Expanded (RFC 3748). */
} wifi_eap_t;

/**
 * @brief Inner (phase 2) authentication method used inside a tunnelled EAP method.
 *
 * Applies where the outer method is tunnelled, such as `WIFI_EAP_TYPE_PEAP` or
 * `WIFI_EAP_TYPE_TTLS`, and selects how the client's credentials are carried inside
 * that tunnel.
 */
typedef enum {
    WIFI_EAP_PHASE2_EAP,        /**< Enterprise EAP. */
    WIFI_EAP_PHASE2_MSCHAPV2,   /**< Enterprise MSCHAPV2. */
    WIFI_EAP_PHASE2_MSCHAP,     /**< Enterprise MSCHAP. */
    WIFI_EAP_PHASE2_PAP,        /**< Enterprise PAP. */
    WIFI_EAP_PHASE2_CHAP,       /**< Enterprise CHAP. */
    WIFI_EAP_PHASE2_GTC         /**< Enterprise GTC. */
} phase2_type;

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Gets detailed traffic statistics for a specific Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_struct  Caller-allocated `wifi_trafficStats_t` that receives the
 *                            packet and error counters for this Access Point. The caller
 *                            allocates and releases it; the `HAL` writes into it and
 *                            retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds a full set of counters; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The counters were retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The counters are cumulative since the last vendor reset, so a caller measuring a
 *       rate must difference two samples itself; this interface defines no reset call.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_getWifiTrafficStats(INT apIndex, wifi_trafficStats_t *output_struct);

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Gets the list of associated devices for a specific Access Point (AP).
 *
 * Retrieves a list of MAC addresses for devices associated with the specified AP.
 *
 * @param[in] ap_index  Index of the Access Point, in the range `AP_INDEX_1` to
 *                      `AP_INDEX_24`.
 * @param[out] output_deviceMacAddressArray  Caller-allocated array of at least
 *                                           `maxNumDevices` `mac_address_t` elements that
 *                                           receives the MAC address of each associated
 *                                           device. The caller allocates and releases it;
 *                                           the `HAL` writes into it and retains no
 *                                           reference to it after returning. Only the
 *                                           first `*output_numDevices` elements are
 *                                           written.
 * @param[in] maxNumDevices  Number of elements the array can hold. `ASSOC_MAC_ARRAY_MAX`
 *                           is the largest associated-device population this interface
 *                           names, so an array sized to that constant cannot overflow. A
 *                           value of 0 is not useful and the interface does not define
 *                           its effect.
 * @param[out] output_numDevices  Caller-allocated variable that receives the number of
 *                                elements actually written, never more than
 *                                `maxNumDevices`. The caller allocates and releases it;
 *                                the `HAL` writes into it and retains no reference to it
 *                                after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds `*output_numDevices` MAC addresses in the array, and
 *       the remaining elements are left untouched; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The list was retrieved. A value of 0 in `*output_numDevices`
 *                          is a success with no associated devices, not an error.
 * @retval WIFI_HAL_ERROR   `ap_index` is out of range, a required output pointer is NULL,
 *                          or the array is too small for the current population. The
 *                          caller should validate its arguments; a failure that persists
 *                          across retries should be logged and the value treated as
 *                          unavailable.
 *
 * @note This interface does not state whether the call truncates or fails when more than
 *       `maxNumDevices` devices are associated, so a caller should size the array from
 *       `wifi_getApNumDevicesAssociated()` or from `ASSOC_MAC_ARRAY_MAX` rather than
 *       relying on either behaviour.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApNumDevicesAssociated
 */
INT wifi_getApAssociatedDevice(INT ap_index, mac_address_t *output_deviceMacAddressArray, UINT maxNumDevices, UINT *output_numDevices);
#endif
/**
 * @brief Outcome of a RADIUS or EAP authentication, as reported to a callback.
 *
 * The values are the RADIUS and EAP packet codes themselves rather than a dense
 * sequence, which is why they are not consecutive.
 */
typedef enum {
    WIFI_ACCESS_ACCEPT_STATUS = 0, /**< RADIUS Access-Accept: the server authorised the client. */
    WIFI_EAP_SUCCESS_STATUS = 3,   /**< EAP-Success: the EAP exchange completed successfully. */
    WIFI_EAP_FAILURE_STATUS = 23   /**< EAP-Failure: the EAP exchange was rejected. */
} wifi_eap_status_code_t;

/**
 * @brief IEEE 802.11 reason codes carried by deauthentication and disassociation
 *        frames.
 *
 * The subset this interface names is the one a caller can act on: the codes that
 * distinguish a wrong credential from a cipher mismatch from a deliberate
 * disconnect. Each value is the reason code as it appears on the wire, so the set is
 * deliberately sparse.
 */
typedef enum {
    WIFI_REASON_UNSPECIFIED = 1,                 /**< No specific reason was given. */
    WIFI_REASON_PREV_AUTH_NOT_VALID = 2,         /**< Previous authentication is no longer valid; the usual code for an incorrect passphrase. */
    WIFI_REASON_DEAUTH_LEAVING = 3,              /**< The station is leaving or has left the BSS. */
    WIFI_REASON_STA_REQ_ASSOC_WITHOUT_AUTH = 9,  /**< The station associated without first authenticating. */
    WIFI_REASON_INVALID_IE = 13,                 /**< An information element was malformed. */
    WIFI_REASON_MICHAEL_MIC_FAILURE = 14,        /**< A TKIP Michael MIC check failed. */
    WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT = 15,     /**< The four-way handshake did not complete in time. */
    WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,   /**< The group key handshake did not complete in time. */
    WIFI_REASON_IE_IN_4WAY_DIFFERS = 17,         /**< An information element in the four-way handshake differed from the one advertised. */
    WIFI_REASON_GROUP_CIPHER_NOT_VALID = 18,     /**< The requested group cipher is not supported. */
    WIFI_REASON_PAIRWISE_CIPHER_NOT_VALID = 19,  /**< The requested pairwise cipher is not supported. */
    WIFI_REASON_AKMP_NOT_VALID = 20,             /**< The requested authentication and key management suite is not supported. */
    WIFI_REASON_UNSUPPORTED_RSN_IE_VERSION = 21, /**< The RSN information element version is not supported. */
    WIFI_REASON_INVALID_RSN_IE_CAPAB = 22,       /**< The RSN information element capabilities field is invalid. */
    WIFI_REASON_IEEE_802_1X_AUTH_FAILED = 23,    /**< IEEE 802.1X authentication failed. */
    WIFI_REASON_CIPHER_SUITE_REJECTED = 24,      /**< The cipher suite was rejected by local policy. */
    WIFI_REASON_INVALID_PMKID = 49               /**< The supplied PMKID does not match a cached PMKSA. */
} wifi_reason_code_t;

/**
 * @brief IEEE 802.11 status codes carried by authentication, association and
 *        reassociation response frames.
 *
 * A status code says why a request was refused, where a reason code says why an
 * established link was torn down. Each value is the status code as it appears on the
 * wire, so the set is deliberately sparse.
 */
typedef enum {
    WIFI_STATUS_UNSPECIFIED_FAILURE = 1,                 /**< The request failed for an unstated reason. */
    WIFI_STATUS_AUTH_TIMEOUT = 16,                       /**< Authentication did not complete within the allowed time. */
    WIFI_STATUS_ASSOC_REJECTED_TEMPORARILY = 30,         /**< Association was refused for now; the client may retry after the indicated interval. */
    WIFI_STATUS_ROBUST_MGMT_FRAME_POLICY_VIOLATION = 31, /**< The request violated the robust management frame protection policy. */
    WIFI_STATUS_AKMP_NOT_VALID = 43,                     /**< The requested authentication and key management suite is not supported. */
    WIFI_STATUS_INVALID_PMKID = 53                       /**< The supplied PMKID does not match a cached PMKSA. */
} wifi_status_code_t;

/**
 * @brief Restores Access Point parameters to factory defaults.
 *
 * This function resets the parameters of the specified Access Point (AP)
 * to their factory default values, without affecting other APs or radio
 * parameters. It does not require a Wi-Fi reboot.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success every parameter of this Access Point holds its factory default, while
 *       other Access Points and all radio parameters are untouched and no Wi-Fi reboot is
 *       required. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The Access Point was reset.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range or the vendor layer could not
 *                          complete the reset. The caller should read the Access Point
 *                          back with `wifi_getRadioVapInfoMap()` to establish the actual
 *                          state before retrying, because a partial reset is not excluded
 *                          by this interface.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning This discards the Access Point's configuration, including its security settings.
 *          Any configuration the caller needs to keep must be read out first.
 * @see wifi_setApSecurityReset
 */
INT wifi_factoryResetAP(int apIndex);

/**
 * @brief Removes an Access Point and releases the state held for it.
 *
 * This function deletes the specified Access Point (AP) entry from the hardware and
 * clears every internal variable the `HAL` holds for it, so the index carries no
 * configuration until a Virtual Access Point is created on it again.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the Access Point entry is removed from the hardware and the internal
 *       variables associated with it are cleared. On failure no part of the configuration
 *       is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The Access Point was deleted.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range or the vendor layer could not delete
 *                          the entry. The caller should treat the Access Point's state as
 *                          unknown and re-read it rather than assuming it still exists.
 *
 * @note A deleted Access Point must be recreated with `wifi_createVAP()` before any other
 *       call referring to its index is meaningful.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_createVAP
 */
INT wifi_deleteAp(INT apIndex);

/**
 * @brief Gets the name of an Access Point.
 *
 * This function retrieves the name associated with the specified Access Point (AP).
 * The output string buffer must be pre-allocated by the caller with a size of
 * at least 16 bytes.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_string  Caller-allocated buffer of at least 16 bytes that receives
 *                            the NUL-terminated Access Point name. The caller allocates
 *                            and releases it; the `HAL` writes into it and retains no
 *                            reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success `output_string` holds the NUL-terminated name; on failure its contents
 *       are undefined.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The name was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `output_string` is NULL, or the
 *                          vendor layer could not supply the name. The caller should
 *                          validate its arguments; a failure that persists across retries
 *                          should be logged and the name treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApStatus
 */
INT wifi_getApName(INT apIndex, CHAR *output_string);

/**
 * @brief Sets the RTS/CTS threshold for an Access Point.
 *
 * This function sets the packet size threshold, in bytes, for applying
 * RTS/CTS (Request to Send/Clear to Send) backoff rules on the specified
 * Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] threshold  Packet size threshold in bytes above which RTS/CTS backoff is
 *                       applied. This interface does not state the accepted range, so a
 *                       caller should confirm the value was taken by reading the Access
 *                       Point back rather than assuming any bound.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success frames larger than `threshold` use RTS/CTS backoff. On failure no part
 *       of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The threshold was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, the vendor layer rejected
 *                          `threshold`, or it could not apply the change. This interface
 *                          does not state the accepted values for `threshold`, so a caller
 *                          must not infer a bound from the failure; it should report the
 *                          failure rather than retrying with the same argument.
 *
 * @note `wifi_getAPCapabilities()` reports whether this Access Point supports an RTS/CTS
 *       threshold at all, through `rtsThresholdSupported`; a caller should check that
 *       before treating a failure here as unexpected.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getAPCapabilities
 */
INT wifi_setApRtsThreshold(INT apIndex, UINT threshold);

/**
 * @brief Removes internal security settings for an Access Point.
 *
 * This function deletes the internal security variable settings for the
 * specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the internal security variable settings for this Access Point are
 *       deleted. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were removed.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range or the vendor layer could not remove
 *                          the settings. The caller should read the security
 *                          configuration back with `wifi_getApSecurity()` before relying
 *                          on it.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning Removing the security variables leaves the Access Point without the credentials
 *          clients authenticate against, so it must be reconfigured with
 *          `wifi_setApSecurity()` before it is usable again.
 * @see wifi_setApSecurity
 */
INT wifi_removeApSecVaribles(INT apIndex);

/**
 * @brief Disables encryption for an Access Point.
 *
 * This function changes the hardware settings to disable encryption on the
 * specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the hardware no longer encrypts traffic on this Access Point. On
 *       failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS Encryption was disabled.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range or the vendor layer could not change
 *                          the hardware setting. The caller should read the security
 *                          configuration back with `wifi_getApSecurity()` rather than
 *                          assuming either state.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning An Access Point with encryption disabled carries traffic in the clear. This is
 *          intended for controlled test and bring-up use, not for a deployed network.
 * @see wifi_setApSecurity
 */
INT wifi_disableApEncryption(INT apIndex);

/**
 * @brief Gets the number of associated devices for an Access Point.
 *
 * This function retrieves the number of stations currently associated with
 * the specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_ulong  Caller-allocated variable that receives the number of
 *                           stations currently associated. The caller allocates and
 *                           releases it; the `HAL` writes into it and retains no
 *                           reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the current associated-station count; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The count was retrieved. Zero is a valid count, not an error.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The count can change between this call and any later call that enumerates the
 *       devices, so a caller sizing an array from it should allow headroom or size from
 *       `ASSOC_MAC_ARRAY_MAX`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApAssociatedDevice
 */
INT wifi_getApNumDevicesAssociated(INT apIndex, ULONG *output_ulong);

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Disassociates a device from an Access Point.
 *
 * This function manually removes any active Wi-Fi association between the
 * specified client device and the Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] client_mac  MAC address of the client to disassociate, passed by value as a
 *                        `mac_address_t`, so no caller storage is retained.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success any active association between that client and this Access Point is
 *       removed. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The disassociation was issued.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, the client is not associated, or
 *                          the vendor layer could not issue the disassociation. The
 *                          caller should confirm the client is present with
 *                          `wifi_getApAssociatedDevice()` before treating this as a
 *                          fault.
 *
 * @note Nothing prevents the client from associating again immediately. To keep it off the
 *       Access Point, add it to the Access Control List with `wifi_addApAclDevice()` and
 *       set a blacklist filter mode first.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_addApAclDevice
 * @see wifi_kickApAclAssociatedDevices
 */
INT wifi_kickApAssociatedDevice(INT apIndex, mac_address_t client_mac);
#endif

/**
 * @brief Gets the radio index for an Access Point.
 *
 * This function retrieves the index of the radio associated with the
 * specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_int  Caller-allocated variable that receives the index of the radio
 *                         hosting this Access Point. The caller allocates and releases
 *                         it; the `HAL` writes into it and retains no reference to it
 *                         after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the hosting radio's index; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The radio index was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The value is a radio index in the range `RADIO_INDEX_1` to `RADIO_INDEX_3`, bounded
 *       by `MAX_NUM_RADIOS`, which is 2 or 3 depending on whether `WIFI_HAL_VERSION_3` is
 *       defined.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_getApRadioIndex(INT apIndex, INT *output_int);

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Gets the ACL MAC list for an Access Point.
 *
 * This function retrieves the list of MAC addresses in the Access Control List (ACL)
 * for the specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] macArray  Caller-allocated array of at least `maxArraySize` `mac_address_t`
 *                       elements that receives the Access Control List entries. The
 *                       caller allocates and releases it; the `HAL` writes into it and
 *                       retains no reference to it after returning. Only the first
 *                       `*output_numEntries` elements are written.
 * @param[in] maxArraySize  Number of elements the array can hold. `ACL_MAC_ARRAY_MAX` is
 *                          the largest Access Control List this interface names, so an
 *                          array sized to that constant cannot overflow.
 * @param[out] output_numEntries  Caller-allocated variable that receives the number of
 *                                elements actually written, never more than
 *                                `maxArraySize`. The caller allocates and releases it;
 *                                the `HAL` writes into it and retains no reference to it
 *                                after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds `*output_numEntries` MAC addresses in the array, with
 *       the remaining elements untouched; on failure the output is left unspecified, so a
 *       caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The list was retrieved. Zero entries is a success, meaning the
 *                          list is empty.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the array is too small for the current list. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note Size the array from `wifi_getApAclDeviceNum()` or from `ACL_MAC_ARRAY_MAX`: this
 *       interface does not state whether an undersized array truncates or fails.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApAclDeviceNum
 */
INT wifi_getApAclDevices(INT apIndex, mac_address_t *macArray, UINT maxArraySize, UINT* output_numEntries);
#endif

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Adds a MAC address to the Access Control List (ACL) for an Access Point.
 *
 * This function adds the specified MAC address to the filter list for the given Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] DeviceMacAddress  MAC address of the device, passed by value as a
 *                              `mac_address_t`, so no caller storage is retained by the
 *                              callee.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the address is present in this Access Point's Access Control List. On
 *       failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The address was added.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, the list is full, or the vendor
 *                          layer rejected the address. The caller should read the entry
 *                          count with `wifi_getApAclDeviceNum()` before retrying, since a
 *                          full list is not a transient condition.
 *
 * @note Adding an address changes who may associate only while a filter mode is active; the
 *       mode is set by `wifi_setApMacAddressControlMode()` and determines whether the list
 *       acts as a whitelist or a blacklist.
 * @note This interface does not state whether adding an address already present succeeds or
 *       fails, so a caller should not depend on either.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApMacAddressControlMode
 * @see wifi_delApAclDevice
 */
INT wifi_addApAclDevice(INT apIndex, mac_address_t DeviceMacAddress);
#endif

#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Deletes a MAC address from the Access Control List (ACL) for an Access Point.
 *
 * This function removes the specified device's MAC address from the ACL for the given Access Point (AP).
 * This function must not block or invoke any long-running tasks.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] deviceMacAddress  MAC address of the device, passed by value as a
 *                              `mac_address_t`, so no caller storage is retained by the
 *                              callee.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the address is absent from this Access Point's Access Control List. On
 *       failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The address was removed.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, the address is not in the list, or
 *                          the vendor layer could not remove it. The caller should read
 *                          the list back with `wifi_getApAclDevices()` rather than
 *                          assuming the entry is gone.
 *
 * @note Removing an address from a blacklist does not disconnect a client that is already
 *       associated, nor does removing it from a whitelist; use
 *       `wifi_kickApAssociatedDevice()` for that.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_addApAclDevice
 * @see wifi_delApAclDevices
 */
INT wifi_delApAclDevice(INT apIndex, mac_address_t deviceMacAddress);
#endif

/**
 * @brief Deletes all MAC addresses from the Access Control List (ACL) for an Access Point.
 *
 * This function removes all device MAC addresses from the ACL for the given Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success this Access Point's Access Control List is empty. On failure no part
 *       of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The list was cleared.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range or the vendor layer could not clear
 *                          the list. The caller should read the entry count back with
 *                          `wifi_getApAclDeviceNum()`, because a partial clear is not
 *                          excluded by this interface.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning With an empty list and a whitelist filter mode active, no client can associate.
 *          Set the filter mode before or with clearing the list if that is not intended.
 * @see wifi_setApMacAddressControlMode
 */
INT wifi_delApAclDevices(INT apIndex);

/**
 * @brief Gets the number of devices in the Access Control List (ACL) for an Access Point.
 *
 * This function retrieves the number of devices in the filter list for the specified Access Point (AP).
 * This function must not block or invoke any long-running tasks.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_uint  Caller-allocated variable that receives the number of entries
 *                          in the Access Control List. The caller allocates and releases
 *                          it; the `HAL` writes into it and retains no reference to it
 *                          after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the current entry count; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The count was retrieved. Zero is a valid count.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note Use this to size the array passed to `wifi_getApAclDevices()`, bounded by
 *       `ACL_MAC_ARRAY_MAX`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApAclDevices
 */
INT wifi_getApAclDeviceNum(INT apIndex, UINT *output_uint);

/**
 * @brief Enables or disables the "kick" feature for devices on the ACL blacklist for an Access Point.
 *
 * This function controls whether devices on the Access Control List (ACL) blacklist
 * for the specified Access Point (AP) should be actively kicked (disassociated).
 * This function must not block or invoke any long-running tasks.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enable  true to disassociate clients that appear on the Access Control List
 *                    blacklist, false to leave them associated.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success blacklisted clients are actively disassociated when `enable` is true,
 *       and left alone when it is false. On failure no part of the configuration is
 *       changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enable` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should confirm the Access Point is present
 *                          with `wifi_getApStatus()` rather than retrying with the same
 *                          argument.
 *
 * @note This governs enforcement against already-associated clients only. Whether a client
 *       may associate in the first place is governed by the filter mode set with
 *       `wifi_setApMacAddressControlMode()`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApMacAddressControlMode
 */
INT wifi_kickApAclAssociatedDevices(INT apIndex, BOOL enable);

/**
 * @brief Sets the MAC address filter control mode for an Access Point.
 *
 * This function sets the MAC address filtering mode for the specified Access Point (AP).
 * The valid filter modes are:
 *  - 0: Filter disabled.
 *  - 1: Filter as whitelist.
 *  - 2: Filter as blacklist.
 *
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] filterMode  MAC address filter control mode: 0 disables filtering, 1 treats
 *                        the Access Control List as a whitelist, and 2 treats it as a
 *                        blacklist. No other value is defined by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point applies the requested filter mode. On failure no part
 *       of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The mode was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `filterMode` is not 0, 1 or 2, or
 *                          the vendor layer rejected the change. The caller should
 *                          correct the mode value rather than retrying with the same
 *                          argument.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning Selecting whitelist mode while the Access Control List is empty prevents every
 *          client from associating.
 * @see wifi_getApMacAddressControlMode
 * @see wifi_getApAclDevices
 */
INT wifi_setApMacAddressControlMode(INT apIndex, INT filterMode);

/**
 * @brief Gets the MAC address filter control mode for an Access Point.
 *
 * This function retrieves the MAC address filtering mode for the specified
 * Access Point (AP). The returned value indicates the current filter mode:
 *  - 0: Filter disabled.
 *  - 1: Filter as whitelist.
 *  - 2: Filter as blacklist.
 *
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_filterMode  Caller-allocated variable that receives the current
 *                                filter mode: 0 disabled, 1 whitelist, 2 blacklist. The
 *                                caller allocates and releases it; the `HAL` writes into
 *                                it and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the current filter mode; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The mode was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApMacAddressControlMode
 */
INT wifi_getApMacAddressControlMode(INT apIndex, INT *output_filterMode);

/**
 * @brief Sets the VLAN ID for an Access Point.
 *
 * This function sets the VLAN ID for the specified Access Point (AP).
 * The VLAN ID is stored in an internal environment variable.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] vlanId  VLAN identifier to associate with this Access Point. This interface
 *                    does not state the accepted range, so a caller should not assume the
 *                    full IEEE 802.1Q range is available.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the VLAN identifier is recorded in the internal environment variable
 *       that carries it. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The VLAN identifier was set.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, the vendor layer rejected `vlanId`,
 *                          or it could not apply the change. This interface does not state
 *                          the accepted values for `vlanId`, so a caller must not infer a
 *                          bound from the failure; it should report the failure rather than
 *                          retrying with the same argument.
 *
 * @note The value is stored in an internal environment variable, so this interface does not
 *       state whether it takes effect immediately or at the next Access Point restart.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_resetApVlanCfg
 */
INT wifi_setApVlanID(INT apIndex, INT vlanId);

/**
 * @brief Resets the VLAN configuration for an Access Point.
 *
 * This function resets the VLAN configuration for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the Access Point's VLAN configuration is back at its default. On
 *       failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The VLAN configuration was reset.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range or the vendor layer could not reset
 *                          the configuration. The caller should not assume the previous
 *                          VLAN identifier survived, and should set it again explicitly
 *                          if it is needed.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApVlanID
 */
INT wifi_resetApVlanCfg(INT apIndex);

/**
 * @brief Sets the enable status for an Access Point.
 *
 * This function sets the enable status variable for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enable  true to bring the Access Point into service, false to take it out of
 *                    service.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the internal enable variable holds the requested value. On failure no
 *       part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The enable state was set.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enable` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getApEnable()` before relying on it rather than retrying
 *                          with the same argument.
 *
 * @note This sets the requested state. Whether the Access Point has actually reached it is
 *       reported by `wifi_getApStatus()`, and a transition is notified through
 *       `wifi_vapstatus_callback_register()`; `wifi_getApEnable()` reads back only what was
 *       requested here.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApEnable
 * @see wifi_getApStatus
 * @see wifi_vapstatus_callback_register
 */
INT wifi_setApEnable(INT apIndex, BOOL enable);

/**
 * @brief Gets the enable status for an Access Point.
 *
 * This function retrieves the setting of the internal enable status variable
 * for the specified Access Point (AP), which is set by the `wifi_setApEnable()` function.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_bool  Caller-allocated variable that receives the requested enable
 *                          state previously set by `wifi_setApEnable()`. The caller
 *                          allocates and releases it; the `HAL` writes into it and
 *                          retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the requested enable state; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This reports the requested state, not the operational one. Use `wifi_getApStatus()`
 *       to find out whether the Access Point is actually up.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApEnable
 * @see wifi_getApStatus
 */
INT wifi_getApEnable(INT apIndex, BOOL *output_bool);

/**
 * @brief Gets the operational status of an Access Point.
 *
 * This function retrieves the operational status of the specified Access Point (AP)
 * from the driver. The output string will be set to either "Enabled" or "Disabled".
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_string  Caller-allocated buffer of at least 32 bytes that receives
 *                            the operational status as reported by the driver, either
 *                            `"Enabled"` or `"Disabled"`, NUL-terminated. The caller
 *                            allocates and releases it; the `HAL` writes into it and
 *                            retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds one of the two status strings; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The status was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This interface names only `"Enabled"` and `"Disabled"`, so a caller should compare
 *       against both rather than testing for one and inferring the other.
 * @note This reads the driver's operational state, which can differ from the requested
 *       state returned by `wifi_getApEnable()` while a transition is in progress.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApEnable
 */
INT wifi_getApStatus(INT apIndex, CHAR *output_string);

/**
 * @brief Checks whether SSID advertisement is enabled for an Access Point.
 *
 * This function indicates whether or not the beacons transmitted by the
 * specified Access Point (AP) include the SSID name.
 * It returns true if SSID advertisement is enabled, and false otherwise.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_bool  Caller-allocated variable that receives true when beacons
 *                          carry the SSID and false when the SSID is hidden. The caller
 *                          allocates and releases it; the `HAL` writes into it and
 *                          retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the SSID advertisement state; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApSsidAdvertisementEnable
 */
INT wifi_getApSsidAdvertisementEnable(INT apIndex, BOOL *output_bool);

/**
 * @brief Enables or disables SSID advertisement for an Access Point.
 *
 * This function sets an internal variable to control whether the specified
 * Access Point (AP) includes the SSID name in its beacon frames.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enable  true to include the SSID in beacon frames, false to omit it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the internal variable controlling SSID advertisement holds the
 *       requested value. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was set.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enable` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getApSsidAdvertisementEnable()` before relying on it
 *                          rather than retrying with the same argument.
 *
 * @note Hiding the SSID keeps it out of beacons but does not conceal it from a client that
 *       already knows it, so it is not a security control.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApSsidAdvertisementEnable
 */
INT wifi_setApSsidAdvertisementEnable(INT apIndex, BOOL enable);

/**
 * @brief Gets the retry limit for an Access Point.
 *
 * This function retrieves the maximum number of retransmissions allowed for a packet
 * on the specified Access Point (AP). This corresponds to the IEEE 802.11 parameter
 * `dot11ShortRetryLimit`.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output  Caller-allocated variable that receives the maximum number of
 *                     retransmissions allowed for one packet, the IEEE 802.11
 *                     `dot11ShortRetryLimit`. The caller allocates and releases it; the
 *                     `HAL` writes into it and retains no reference to it after
 *                     returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the current retry limit; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The limit was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApRetryLimit
 */
INT wifi_getApRetryLimit(INT apIndex, UINT *output);

/**
 * @brief Sets the retry limit for an Access Point.
 *
 * This function sets the maximum number of retransmissions allowed for a packet
 * on the specified Access Point (AP). This corresponds to the IEEE 802.11 parameter
 * `dot11ShortRetryLimit`.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] number  Maximum number of retransmissions for one packet, the IEEE 802.11
 *                    `dot11ShortRetryLimit`. This interface does not state the accepted
 *                    range, so a caller should read the value back rather than assuming
 *                    any bound was accepted.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point retransmits a packet at most `number` times. On
 *       failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The limit was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, the vendor layer rejected `number`,
 *                          or it could not apply the change. This interface does not state
 *                          the accepted values for `number`, so a caller must not infer a
 *                          bound from the failure; it should report the failure and read
 *                          the setting back with `wifi_getApRetryLimit()` rather than
 *                          retrying with the same argument.
 *
 * @note A high limit trades airtime for reliability, and a limit of 0 disables
 *       retransmission entirely; neither bound is validated by this interface.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApRetryLimit
 */
INT wifi_setApRetryLimit(INT apIndex, UINT number);

/**
 * @brief Checks whether WMM is enabled for an Access Point.
 *
 * This function indicates whether Wi-Fi Multimedia (WMM) support is currently
 * enabled on the specified Access Point (AP). When enabled, WMM support is
 * indicated in the AP's beacon frames.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output  Caller-allocated variable that receives true when Wi-Fi Multimedia
 *                     is enabled and advertised in beacons, false when it is not. The
 *                     caller allocates and releases it; the `HAL` writes into it and
 *                     retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the WMM enable state; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note `wifi_getAPCapabilities()` reports through `WMMSupported` whether this Access Point
 *       can support WMM at all.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApWmmEnable
 * @see wifi_getAPCapabilities
 */
INT wifi_getApWmmEnable(INT apIndex, BOOL *output);   

/**
 * @brief Enables or disables WMM for an Access Point.
 *
 * This function enables or disables Wi-Fi Multimedia (WMM) support on the
 * hardware for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enable  true to enable Wi-Fi Multimedia in hardware, false to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the hardware has WMM in the requested state and beacons reflect it. On
 *       failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enable` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getApWmmEnable()` before relying on it rather than
 *                          retrying with the same argument.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning Disabling WMM also disables U-APSD, which cannot be enabled without it. A caller
 *          that needs U-APSD must enable WMM first.
 * @see wifi_setApWmmUapsdEnable
 * @see wifi_getApWmmEnable
 */
INT wifi_setApWmmEnable(INT apIndex, BOOL enable);

/**
 * @brief Checks whether U-APSD is enabled for an Access Point.
 *
 * This function indicates whether Unscheduled Automatic Power Save Delivery (U-APSD)
 * support is currently enabled on the specified Access Point (AP). When enabled,
 * U-APSD support is indicated in the AP's beacon frames.
 * Note that U-APSD can only be enabled if Wi-Fi Multimedia (WMM) is also enabled.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output  Caller-allocated variable that receives true when Unscheduled
 *                     Automatic Power Save Delivery is enabled and advertised in beacons,
 *                     false when it is not. The caller allocates and releases it; the
 *                     `HAL` writes into it and retains no reference to it after
 *                     returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the U-APSD enable state; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note `wifi_getAPCapabilities()` reports through `UAPSDSupported` whether this Access
 *       Point can support U-APSD at all.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApWmmUapsdEnable
 * @see wifi_getAPCapabilities
 */
INT wifi_getApWmmUapsdEnable(INT apIndex, BOOL *output);

/**
 * @brief Enables or disables U-APSD for an Access Point.
 *
 * This function enables or disables Unscheduled Automatic Power Save Delivery
 * (U-APSD) on the hardware for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enable  true to enable U-APSD in hardware, false to disable it. Enabling
 *                    requires WMM to be enabled already.
 *
 * @pre `wifi_init()` must have completed successfully, and WMM must already be enabled on
 *      this Access Point before U-APSD can be enabled; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made before initialisation, or an attempt to
 *      enable U-APSD while WMM is disabled, fails with `WIFI_HAL_ERROR` and leaves the
 *      configuration unchanged.
 * @post On success the hardware has U-APSD in the requested state. On failure no part of
 *       the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, WMM is disabled while `enable` is
 *                          true, or the vendor layer rejected the change. The caller
 *                          should enable WMM with `wifi_setApWmmEnable()` and retry
 *                          rather than repeating the same call.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApWmmEnable
 */
INT wifi_setApWmmUapsdEnable(INT apIndex, BOOL enable);

/**
 * @brief Sets the WMM ACK policy for an Access Point.
 *
 * This function sets the Wi-Fi Multimedia (WMM) acknowledgment (ACK) policy
 * on the hardware for the specified Access Point (AP).
 * An `ackPolicy` of false means do not acknowledge, and true means acknowledge.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] cla  WMM class of service the policy applies to, identifying the access
 *                 category. This interface does not enumerate the accepted values, so a
 *                 caller should not assume they match `wifi_data_priority_t`.
 * @param[in] ackPolicy  false to leave frames in that class unacknowledged, true to
 *                       acknowledge them.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the hardware applies the requested acknowledgement policy to that
 *       class of service. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The policy was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, the vendor layer rejected `cla`, or
 *                          it could not apply the change. This interface does not state the
 *                          accepted values for `cla`, so a caller must not infer a bound
 *                          from the failure; it should report the failure rather than
 *                          retrying with the same argument.
 *
 * @note The second parameter is named `cla` rather than `class` because this header is
 *       compiled under `extern "C"` by C++ consumers, where `class` is a reserved word. The
 *       short name is deliberate and must not be "corrected".
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApWmmEnable
 */
INT wifi_setApWmmOgAckPolicy(INT apIndex, INT cla, BOOL ackPolicy);

/**
 * @brief Gets the device isolation status for an Access Point.
 *
 * This function retrieves the device isolation status for the specified Access Point (AP).
 * A value of true means that devices connected to the AP are isolated from other devices
 * within the home network (as is typical for a wireless hotspot).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output  Caller-allocated variable that receives true when clients on this
 *                     Access Point are isolated from other devices on the home network,
 *                     false when they are not. The caller allocates and releases it; the
 *                     `HAL` writes into it and retains no reference to it after
 *                     returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the isolation state; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApIsolationEnable
 */
INT wifi_getApIsolationEnable(INT apIndex, BOOL *output);

/**
 * @brief Enables or disables device isolation for an Access Point.
 *
 * This function controls whether devices connected to the specified Access Point (AP)
 * are isolated from other devices within the home network.
 * A value of true means that devices connected to the AP are isolated, as is typically
 * the case for a wireless hotspot.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enable  true to isolate clients on this Access Point from other devices on
 *                    the home network, as a wireless hotspot does; false to let them
 *                    reach each other.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success clients on this Access Point are isolated as requested. On failure no
 *       part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enable` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getApIsolationEnable()` before relying on it rather than
 *                          retrying with the same argument.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApIsolationEnable
 */
INT wifi_setApIsolationEnable(INT apIndex, BOOL enable);

/**
 * @brief Sets the beacon transmission rate for an Access Point.
 *
 * This function sets the beacon transmission rate for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] sBeaconRate  NUL-terminated string giving the beacon transmission rate. The
 *                         values this interface names are `"1Mbps"`, `"2Mbps"`,
 *                         `"5.5Mbps"`, `"6Mbps"`, `"11Mbps"`, `"12Mbps"` and `"24Mbps"`,
 *                         and no other string is defined. The caller owns the storage;
 *                         the `HAL` reads it during the call and must not retain the
 *                         pointer afterwards, so the caller may pass a stack buffer.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point transmits beacons at the requested rate. On failure
 *       no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The rate was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `sBeaconRate` is NULL or not one of
 *                          the named strings, or the rate is not legal on the hosting
 *                          radio's band. The caller should correct the string rather than
 *                          retrying, and should check the radio's band with
 *                          `wifi_getApRadioIndex()` because the legal set differs between
 *                          2.4 GHz and 5 GHz.
 *
 * @note Only the rates this interface names may be passed. A lower beacon rate reaches
 *       further but consumes more airtime.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApBeaconRate
 */
INT wifi_setApBeaconRate(INT apIndex, char *sBeaconRate);

/**
 * @brief Gets the beacon transmission rate for an Access Point.
 *
 * This function retrieves the beacon transmission rate for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_BeaconRate  Caller-allocated buffer of at least 32 bytes that
 *                                receives the current beacon rate as one of the strings
 *                                named by `wifi_setApBeaconRate()`, NUL-terminated. The
 *                                caller allocates and releases it; the `HAL` writes into
 *                                it and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the beacon rate string; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The rate was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApBeaconRate
 */
INT wifi_getApBeaconRate(INT apIndex, char *output_BeaconRate);

/**
 * @brief Gets the maximum number of associated devices allowed for an Access Point.
 *
 * This function retrieves the maximum number of devices that can simultaneously
 * be connected to the specified Access Point (AP). A value of 0 indicates that
 * there is no specific limit on the number of associated devices.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output  Caller-allocated variable that receives the maximum number of
 *                     simultaneously associated devices, where 0 means no limit is
 *                     imposed. The caller allocates and releases it; the `HAL` writes
 *                     into it and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the configured maximum, or 0 for no limit; on failure
 *       the output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The maximum was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note A returned 0 means unlimited, not none. Do not treat it as a closed Access Point.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApMaxAssociatedDevices
 */
INT wifi_getApMaxAssociatedDevices(INT apIndex, UINT *output);

/**
 * @brief Sets the maximum number of associated devices allowed for an Access Point.
 *
 * This function sets the maximum number of devices that can simultaneously
 * be connected to the specified Access Point (AP). A value of 0 indicates that
 * there is no specific limit on the number of associated devices.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] number  Maximum number of simultaneously associated devices, or 0 to impose
 *                    no limit. `ASSOC_MAC_ARRAY_MAX` is the largest population this
 *                    interface names.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point admits at most `number` simultaneous clients, or an
 *       unlimited number when `number` is 0. On failure no part of the configuration is
 *       changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The maximum was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, the supplied value is outside the
 *                          accepted set, or the vendor layer rejected the value. The
 *                          caller should validate the value against the range given above
 *                          and report the failure rather than retrying with the same
 *                          argument.
 *
 * @note This interface does not state what happens to clients already associated when the
 *       new maximum is below the current count, so a caller must not rely on either
 *       outcome.
 * @note A client refused because the Access Point is at capacity is reported through
 *       `wifi_apMaxClientRejection_callback`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApMaxAssociatedDevices
 */
INT wifi_setApMaxAssociatedDevices(INT apIndex, UINT number);

/**
 * @brief Resets the security settings for an Access Point to factory defaults.
 *
 * This function resets the Wi-Fi security settings for the specified Access Point (AP)
 * to their factory default values. The affected settings include `ModeEnabled`, `WEPKey`,
 * `PreSharedKey`, and `KeyPassphrase`.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success `ModeEnabled`, `WEPKey`, `PreSharedKey` and `KeyPassphrase` hold their
 *       factory defaults for this Access Point. On failure no part of the configuration
 *       is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The security settings were reset.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range or the vendor layer could not
 *                          complete the reset. The caller should read the settings back
 *                          with `wifi_getApSecurity()`, because a partial reset is not
 *                          excluded by this interface.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning This discards the Access Point's credentials, so associated clients will fail to
 *          reauthenticate until it is reconfigured.
 * @see wifi_setApSecurity
 * @see wifi_factoryResetAP
 */
INT wifi_setApSecurityReset(INT apIndex);

/**
 * @brief Gets the MFP configuration for an Access Point.
 *
 * This function retrieves the Management Frame Protection (MFP) configuration
 * for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_string  Caller-allocated buffer of at least 64 bytes that receives
 *                            the Management Frame Protection setting as one of
 *                            `"Disabled"`, `"Optional"` or `"Required"`, NUL-terminated.
 *                            The caller allocates and releases it; the `HAL` writes into
 *                            it and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds one of the three named strings; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note Only the three named strings are defined, so a caller should compare against all
 *       three rather than testing one and inferring the rest.
 * @note Where `WIFI_HAL_VERSION_3` is defined the same three states are also available as
 *       the `wifi_mfp_cfg_t` enumeration inside `wifi_vap_security_t`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApSecurityMFPConfig
 * @see wifi_mfp_cfg_t
 */
INT wifi_getApSecurityMFPConfig(INT apIndex, CHAR *output_string);

/**
 * @brief Sets the MFP configuration for an Access Point.
 *
 * This function sets the Management Frame Protection (MFP) configuration for
 * the specified Access Point (AP). The MFP configuration must be saved in a
 * persistent manner so that it can be automatically applied after a Wi-Fi
 * or VAP restart. The configuration must also be applied immediately.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] MfpConfig  NUL-terminated string giving the Management Frame Protection
 *                       setting, which must be exactly `"Disabled"`, `"Optional"` or
 *                       `"Required"`. The caller owns the storage; the `HAL` reads it
 *                       during the call and must not retain the pointer afterwards, so
 *                       the caller may pass a stack buffer.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the setting is applied immediately and is stored persistently, so it
 *       is reapplied after a Wi-Fi or VAP restart. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied and persisted.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `MfpConfig` is NULL or not one of
 *                          the three named strings, or the vendor layer could not persist
 *                          the value. The caller should correct the string rather than
 *                          retrying, and should read the value back to confirm it
 *                          persisted.
 *
 * @note `"Required"` will prevent clients that do not support protected management frames
 *       from associating at all.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApSecurityMFPConfig
 */
INT wifi_setApSecurityMFPConfig(INT apIndex, CHAR *MfpConfig);

/**
 * @brief Gets the RADIUS server settings for an Access Point.
 *
 * This function retrieves the IP address, port number, and secret of the RADIUS
 * server used for WLAN security on the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] IP_output  Caller-allocated buffer of at least 64 bytes that receives the
 *                        server's IP address as a NUL-terminated string, NUL-terminated.
 *                        The caller allocates and releases it; the `HAL` writes into it
 *                        and retains no reference to it after returning.
 * @param[out] Port_output  Caller-allocated variable that receives the server's UDP port
 *                          number. The caller allocates and releases it; the `HAL` writes
 *                          into it and retains no reference to it after returning.
 * @param[out] RadiusSecret_output  Caller-allocated buffer of at least 64 bytes that
 *                                  receives the shared secret as a NUL-terminated string,
 *                                  NUL-terminated. The caller allocates and releases it;
 *                                  the `HAL` writes into it and retains no reference to
 *                                  it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the primary RADIUS server's address, port and secret;
 *       on failure the output is left unspecified, so a caller must not read it unless
 *       the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or no primary RADIUS server is configured. The caller should
 *                          validate its arguments; a failure that persists across retries
 *                          should be logged and the value treated as unavailable.
 *
 * @note This interface does not state what the output holds when no server is configured,
 *       so a caller must not treat an empty address string as a defined result unless the
 *       call succeeded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it, and should clear its own copy once the call returns.
 */
INT wifi_getApSecurityRadiusServer(INT apIndex, CHAR *IP_output, UINT *Port_output, CHAR *RadiusSecret_output); 

/**
 * @brief Sets the RADIUS server settings for an Access Point.
 *
 * This function sets the IP address, port number, and secret of the RADIUS
 * server used for WLAN security on the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] IPAddress  NUL-terminated string giving the primary RADIUS server's IP
 *                       address, at most 63 characters plus the terminator. The caller
 *                       owns the storage; the `HAL` reads it during the call and must not
 *                       retain the pointer afterwards, so the caller may pass a stack
 *                       buffer.
 * @param[in] port  UDP port the server listens on. This interface does not state a
 *                  default, so the caller must supply the deployment's port.
 * @param[in] RadiusSecret  NUL-terminated string giving the shared secret used to
 *                          authenticate with the server, at most 63 characters plus the
 *                          terminator. The caller owns the storage; the `HAL` reads it
 *                          during the call and must not retain the pointer afterwards, so
 *                          the caller may pass a stack buffer.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point authenticates against the primary RADIUS server at
 *       the given address and port using the given secret. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a string argument is NULL or
 *                          malformed, or the vendor layer rejected the settings. The
 *                          caller should validate the address and secret before retrying,
 *                          and must not assume a partial update was avoided without
 *                          reading the settings back.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it, and should clear its own copy once the call returns.
 */
INT wifi_setApSecurityRadiusServer(INT apIndex, CHAR *IPAddress, UINT port, CHAR *RadiusSecret);

/**
 * @brief Gets the secondary RADIUS server settings for an Access Point.
 *
 * This function retrieves the IP address, port number, and secret of the
 * secondary RADIUS server used for WLAN security on the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] IP_output  Caller-allocated buffer of at least 64 bytes that receives the
 *                        server's IP address as a NUL-terminated string, NUL-terminated.
 *                        The caller allocates and releases it; the `HAL` writes into it
 *                        and retains no reference to it after returning.
 * @param[out] Port_output  Caller-allocated variable that receives the server's UDP port
 *                          number. The caller allocates and releases it; the `HAL` writes
 *                          into it and retains no reference to it after returning.
 * @param[out] RadiusSecret_output  Caller-allocated buffer of at least 64 bytes that
 *                                  receives the shared secret as a NUL-terminated string,
 *                                  NUL-terminated. The caller allocates and releases it;
 *                                  the `HAL` writes into it and retains no reference to
 *                                  it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the secondary RADIUS server's address, port and
 *       secret; on failure the output is left unspecified, so a caller must not read it
 *       unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or no secondary RADIUS server is configured. The caller should
 *                          validate its arguments; a failure that persists across retries
 *                          should be logged and the value treated as unavailable.
 *
 * @note This interface does not state what the output holds when no server is configured,
 *       so a caller must not treat an empty address string as a defined result unless the
 *       call succeeded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it, and should clear its own copy once the call returns.
 */
INT wifi_getApSecuritySecondaryRadiusServer(INT apIndex, CHAR *IP_output, UINT *Port_output, CHAR *RadiusSecret_output);

/**
 * @brief Sets the secondary RADIUS server settings for an Access Point.
 *
 * This function sets the IP address, port number, and secret of the secondary RADIUS server
 * used for WLAN security on the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] IPAddress  NUL-terminated string giving the secondary RADIUS server's IP
 *                       address, at most 63 characters plus the terminator. The caller
 *                       owns the storage; the `HAL` reads it during the call and must not
 *                       retain the pointer afterwards, so the caller may pass a stack
 *                       buffer.
 * @param[in] port  UDP port the server listens on. This interface does not state a
 *                  default, so the caller must supply the deployment's port.
 * @param[in] RadiusSecret  NUL-terminated string giving the shared secret used to
 *                          authenticate with the server, at most 63 characters plus the
 *                          terminator. The caller owns the storage; the `HAL` reads it
 *                          during the call and must not retain the pointer afterwards, so
 *                          the caller may pass a stack buffer.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point authenticates against the secondary RADIUS server at
 *       the given address and port using the given secret. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a string argument is NULL or
 *                          malformed, or the vendor layer rejected the settings. The
 *                          caller should validate the address and secret before retrying,
 *                          and must not assume a partial update was avoided without
 *                          reading the settings back.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it, and should clear its own copy once the call returns.
 */
INT wifi_setApSecuritySecondaryRadiusServer(INT apIndex, CHAR *IPAddress, UINT port, CHAR *RadiusSecret);

/**
 * @brief Gets the RADIUS DAS server settings for an Access Point.
 *
 * This function retrieves the IP address, port number, and secret of the
 * RADIUS DAS server used for WLAN security on the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] IP_output  Caller-allocated buffer of at least 64 bytes that receives the
 *                        server's IP address as a NUL-terminated string, NUL-terminated.
 *                        The caller allocates and releases it; the `HAL` writes into it
 *                        and retains no reference to it after returning.
 * @param[out] Port_output  Caller-allocated variable that receives the server's UDP port
 *                          number. The caller allocates and releases it; the `HAL` writes
 *                          into it and retains no reference to it after returning.
 * @param[out] RadiusdasSecret_output  Caller-allocated buffer of at least 64 bytes that
 *                                     receives the shared secret as a NUL-terminated
 *                                     string, NUL-terminated. The caller allocates and
 *                                     releases it; the `HAL` writes into it and retains
 *                                     no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the RADIUS Dynamic Authorization Server server's
 *       address, port and secret; on failure the output is left unspecified, so a caller
 *       must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or no RADIUS Dynamic Authorization Server server is
 *                          configured. The caller should validate its arguments; a
 *                          failure that persists across retries should be logged and the
 *                          value treated as unavailable.
 *
 * @note This interface does not state what the output holds when no server is configured,
 *       so a caller must not treat an empty address string as a defined result unless the
 *       call succeeded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it, and should clear its own copy once the call returns.
 */
INT wifi_getApDASRadiusServer(INT apIndex, CHAR *IP_output, UINT *Port_output, CHAR *RadiusdasSecret_output);

/**
 * @brief Sets the RADIUS DAS server settings for an Access Point.
 *
 * This function sets the IP address, port number, and secret of the RADIUS DAS
 * server used for WLAN security on the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] IPAddress  NUL-terminated string giving the RADIUS Dynamic Authorization
 *                       Server server's IP address, at most 63 characters plus the
 *                       terminator. The caller owns the storage; the `HAL` reads it
 *                       during the call and must not retain the pointer afterwards, so
 *                       the caller may pass a stack buffer.
 * @param[in] port  UDP port the server listens on. This interface does not state a
 *                  default, so the caller must supply the deployment's port.
 * @param[in] RadiusdasSecret  NUL-terminated string giving the shared secret used to
 *                             authenticate with the server, at most 63 characters plus
 *                             the terminator. The caller owns the storage; the `HAL`
 *                             reads it during the call and must not retain the pointer
 *                             afterwards, so the caller may pass a stack buffer.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point authenticates against the RADIUS Dynamic
 *       Authorization Server server at the given address and port using the given secret.
 *       On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a string argument is NULL or
 *                          malformed, or the vendor layer rejected the settings. The
 *                          caller should validate the address and secret before retrying,
 *                          and must not assume a partial update was avoided without
 *                          reading the settings back.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it, and should clear its own copy once the call returns.
 */
INT wifi_setApDASRadiusServer(INT apIndex, CHAR *IPAddress, UINT port, CHAR *RadiusdasSecret);

/**
 * @brief Enables or disables greylist access control on all applicable VAPs.
 *
 * This function enables or disables greylist access control on all
 * applicable Virtual Access Points (VAPs).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] enable  true to enable greylist access control on every applicable Virtual
 *                    Access Point, false to disable it on all of them.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success greylist access control is in the requested state on every applicable
 *       Virtual Access Point. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied to all applicable Virtual Access
 *                          Points.
 * @retval WIFI_HAL_ERROR   The vendor layer could not apply the setting. Because this
 *                          call is device-wide, a failure may leave some Virtual Access
 *                          Points changed and others not: the caller should read each one
 *                          back rather than assuming the call was atomic.
 *
 * @note This call takes no Access Point index: it applies device-wide, unlike every other
 *       access-control call in this header.
 * @note The per-Virtual-Access-Point counterpart is the `network_initiated_greylist` member
 *       of `wifi_front_haul_bss_t`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 */
INT wifi_enableGreylistAccessControl(BOOL enable);

/**
 * @brief Gets the RADIUS settings for an Access Point.
 *
 * This function retrieves the RADIUS settings for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output  Caller-allocated `wifi_radius_setting_t` that receives the RADIUS
 *                     timing, retry and caching parameters. The caller allocates and
 *                     releases it; the `HAL` writes into it and retains no reference to
 *                     it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds a full set of RADIUS parameters; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This reports the RADIUS behaviour parameters -- retries, timeouts, PMK lifetime and
 *       caching, blacklisting -- and not the server address or secret, which
 *       `wifi_getApSecurityRadiusServer()` reports.
 * @note The structure's commented-out `RadiusSecret` member is deliberately absent: the
 *       secret is never returned through this call.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApSecurityRadiusSettings
 * @see wifi_getApSecurityRadiusServer
 */
INT wifi_getApSecurityRadiusSettings(INT apIndex, wifi_radius_setting_t *output);

/**
 * @brief Sets the RADIUS settings for an Access Point.
 *
 * This function sets the RADIUS settings for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] input  Fully populated `wifi_radius_setting_t` to apply. The caller owns the
 *                   structure; the `HAL` reads it during the call and must not retain the
 *                   pointer afterwards. Every member is applied, so a caller should read
 *                   the current settings first and modify them rather than passing a
 *                   partially filled structure.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success all the RADIUS parameters in the structure are in force. On failure no
 *       part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `input` is NULL, or a parameter is
 *                          outside the range the vendor layer accepts. The caller should
 *                          read the settings back to establish which values took effect,
 *                          since this interface does not state that the update is atomic.
 *
 * @note Setting `MaxAuthenticationAttempts` to 0 disables client blacklisting, and 0 in
 *       `IdentityRequestRetryInterval` or `QuietPeriodAfterFailedAuthentication` disables
 *       that behaviour, as the structure's own member documentation states.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApSecurityRadiusSettings
 */
INT wifi_setApSecurityRadiusSettings(INT apIndex, wifi_radius_setting_t *input);

//-----------------------------------------------------------------------------------------------

/**
 * @brief Gets the WPS configuration state for an Access Point.
 *
 * This function retrieves the Wi-Fi Protected Setup (WPS) configuration state
 * for the specified Access Point (AP).
 * The output string will be set to either "Not configured" or "Configured".
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_string  Caller-allocated buffer of at least 32 bytes that receives
 *                            the WPS configuration state, either `"Not configured"` or
 *                            `"Configured"`, NUL-terminated. The caller allocates and
 *                            releases it; the `HAL` writes into it and retains no
 *                            reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds one of the two named strings; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note Only the two named strings are defined by this interface.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApWpsConfiguration
 */
INT wifi_getApWpsConfigurationState(INT apIndex, CHAR *output_string);

/**
 * @brief Sets the WPS enrollee PIN for an Access Point.
 *
 * This function sets the Wi-Fi Protected Setup (WPS) enrollee PIN for the
 * specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] pin  NUL-terminated string giving the WPS enrollee PIN.
 *                 `WIFI_AP_MAX_WPSPIN_LEN` is 9, which accommodates an 8-digit PIN and
 *                 its terminator. The caller owns the storage; the `HAL` reads it during
 *                 the call and must not retain the pointer afterwards, so the caller may
 *                 pass a stack buffer.
 *
 * @pre `wifi_init()` must have completed successfully, and a handler should already be
 *      installed with `wifi_wpsEvent_callback_register()` or the session's outcome will
 *      not be reported; see `Initialization and Startup` in `docs/pages/halSpec.md`. A
 *      call made before initialisation fails with `WIFI_HAL_ERROR` and starts no session.
 * @post On success a PIN-method WPS session is armed with the given PIN, and its outcome
 *       is delivered to the registered WPS event handler. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The PIN was accepted and the session armed.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `pin` is NULL, longer than
 *                          `WIFI_AP_MAX_WPSPIN_LEN` permits or fails its checksum, or WPS
 *                          is not enabled on this Access Point. The caller should
 *                          validate the PIN and confirm WPS is enabled with
 *                          `wifi_getApWpsConfiguration()` before retrying.
 *
 * @note A PIN session expires on its own, reported as `wifi_wps_ev_pin_timeout`; use
 *       `wifi_cancelApWPS()` to end it early.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_wpsEvent_callback_register
 * @see wifi_cancelApWPS
 */
INT wifi_setApWpsEnrolleePin(INT apIndex, CHAR *pin);

/**
 * @brief Simulates a WPS push button press for an Access Point.
 *
 * This function simulates a Wi-Fi Protected Setup (WPS) push button press
 * for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully, and a handler should already be
 *      installed with `wifi_wpsEvent_callback_register()` or the session's outcome will
 *      not be reported; see `Initialization and Startup` in `docs/pages/halSpec.md`. A
 *      call made before initialisation fails with `WIFI_HAL_ERROR` and starts no session.
 * @post On success a push-button WPS session is started, exactly as a physical button
 *       press would start one, and its outcome is delivered to the registered WPS event
 *       handler. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The push-button session was started.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, WPS is not enabled on this Access
 *                          Point, or a session could not be started. The caller should
 *                          confirm WPS is enabled with `wifi_getApWpsConfiguration()`
 *                          rather than retrying immediately.
 *
 * @note The session ends by itself: success is `wifi_wps_ev_success`, expiry of the walk
 *       time is `wifi_wps_ev_pbc_timeout`, and a competing session is
 *       `wifi_wps_ev_pbc_overlap`. Only the event handler distinguishes these -- this
 *       call's success means the session started, not that a client was onboarded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_wpsEvent_callback_register
 * @see wifi_cancelApWPS
 * @see wifi_wps_ev_t
 */
INT wifi_setApWpsButtonPush(INT apIndex);

/**
 * @brief Cancels WPS mode for an Access Point.
 *
 * This function cancels Wi-Fi Protected Setup (WPS) mode for the specified
 * Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success no WPS session is active on this Access Point. On failure no part of
 *       the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS WPS mode was cancelled.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range or the vendor layer could not cancel
 *                          the session. The caller should not assume a session is still
 *                          running, and should wait for the WPS event handler rather than
 *                          polling.
 *
 * @note Cancelling a session that is not running is not defined by this interface as either
 *       success or failure, so a caller should not use the return value to test whether a
 *       session was active.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApWpsButtonPush
 * @see wifi_setApWpsEnrolleePin
 */
INT wifi_cancelApWPS(INT apIndex);

/**
 * @brief Gets the management frame power control value for an Access Point.
 *
 * This function retrieves the ApManagementFramePowerControl value for the
 * specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_dBm  Caller-allocated variable that receives the management frame
 *                         transmit power in dBm. The caller allocates and releases it;
 *                         the `HAL` writes into it and retains no reference to it after
 *                         returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the current management frame power; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The value was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApManagementFramePowerControl
 */
INT wifi_getApManagementFramePowerControl(INT apIndex, INT *output_dBm);

/**
 * @brief Sets the management frame power control value for an Access Point.
 *
 * This function sets the ApManagementFramePowerControl value for the specified
 * Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] dBm  Management frame transmit power in dBm. This interface does not state
 *                 the accepted range, which is bounded by regulatory limits and by the
 *                 hosting radio, so a caller should read the value back rather than
 *                 assuming it was taken.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success management frames are transmitted at the requested power. On failure
 *       no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The value was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, the vendor layer rejected `dBm`, or
 *                          it could not apply the change. This interface does not state the
 *                          accepted values for `dBm`, so a caller must not infer a bound
 *                          from the failure; it should report the failure and read the
 *                          setting back with `wifi_getApManagementFramePowerControl()`
 *                          rather than retrying with the same argument.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApManagementFramePowerControl
 */
INT wifi_setApManagementFramePowerControl(INT apIndex, INT dBm);
/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Callback function invoked when a new Wi-Fi client associates with an Access Point.
 *
 * This callback function is invoked when a new Wi-Fi client associates with
 * the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex         Index of the Access Point.
 * @param[in] associated_dev  Pointer to a structure containing information about the associated device.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 */
typedef INT(* wifi_newApAssociatedDevice_callback)(INT apIndex, wifi_associated_dev3_t *associated_dev);
#else

/**
 * @brief State of one radio link of a Wi-Fi 7 multi-link client.
 *
 * A multi-link device presents one of these per link it has set up. Only entries
 * whose `cli_Valid` is true carry meaningful data, so a caller must test that member
 * before reading any other.
 */
typedef struct {
    BOOL cli_Valid;             /**< True when the rest of this entry is populated; false means the link slot is unused. */
    BOOL cli_IsAssocLink;       /**< True on the single link that carried the association exchange. */
    UCHAR cli_LinkID;           /**< IEEE 802.11be link identifier advertised for this link. */
    UINT cli_VapIndex;          /**< Index of the VAP this link is established on. */
    INT cli_RSSI;               /**< Received signal strength for this link, in dBm. */
    UCHAR cli_LinkAddress[6];   /**< MAC address the client uses on this link, which differs from its MLD address. */
} wifi_mld_sta_link_info_t;

/**
 * @brief Multi-link capability and per-link state of an associated client.
 *
 * Reported as part of `wifi_associated_dev_t`. When `cli_MLDSta` is false the client
 * is a conventional single-link station and the remaining members carry no meaning.
 */
typedef struct {
    BOOL cli_MLDSta;                        /**< True when the client is a Wi-Fi 7 multi-link device; false for a single-link station. */
    wifi_multi_link_modes_t cli_MLModeCapa; /**< Multi-link operating modes the client advertised support for. */
    BOOL cli_TIDLinkMapNegotiation;         /**< True when the client negotiated a traffic-identifier-to-link mapping rather than using the default. */
    wifi_mld_sta_link_info_t cli_LinkInfo[MAX_NUM_RADIOS]; /**< Per-link state, one entry per radio; read only the entries whose `cli_Valid` is true. */
} wifi_mld_sta_info_t;

/**
 * @brief Structure containing information about an associated device.
 */
typedef struct _wifi_associated_dev
{
    UCHAR cli_MACAddress[6];             /**< The MAC address of an associated device. */
    CHAR cli_IPAddress[64];              /**< IP address of the associated device. */
    BOOL cli_AuthenticationState;        /**< Whether an associated device has authenticated (true) or not (false). */
    UINT cli_LastDataDownlinkRate;      /**< The data transmit rate in kbps that was most recently used for transmission from the access point to the associated device. */
    UINT cli_LastDataUplinkRate;        /**< The data transmit rate in kbps that was most recently used for transmission from the associated device to the access point. */
    INT cli_SignalStrength;              /**< An indicator of radio signal strength of the uplink from the associated device to the access point, measured in dBm, as an average of the last 100 packets received from the device. */
    UINT cli_Retransmissions;            /**< The number of packets that had to be re-transmitted, from the last 100 packets sent to the associated device. Multiple re-transmissions of the same packet count as one. */
    BOOL cli_Active;                    /**< Whether or not this node is currently present in the WiFi AccessPoint network. */
    CHAR cli_OperatingStandard[64];     /**< Radio standard the associated Wi-Fi client device is operating under. */
    CHAR cli_OperatingChannelBandwidth[64]; /**< The operating channel bandwidth of the associated device. The channel bandwidth (applicable to 802.11n and 802.11ac specifications only). */
    INT cli_SNR;                         /**< A signal-to-noise ratio (SNR) compares the level of the Wi-Fi signal to the level of background noise. Sources of noise can include microwave ovens, cordless phone, bluetooth devices, wireless video cameras, wireless game controllers, fluorescent lights and more. It is measured in decibels (dB). */
    CHAR cli_InterferenceSources[64];    /**< Wi-Fi operates in two frequency ranges (2.4 Ghz and 5 Ghz) which may become crowded with other radio products which operate in the same ranges. This parameter reports the probable interference sources that this Wi-Fi access point may be observing. The value of this parameter is a comma separated list of the following possible sources: e.g., "MicrowaveOven,CordlessPhone,BluetoothDevices,FluorescentLights,ContinuousWaves,Others". */
    ULONG cli_DataFramesSentAck;         /**< The total number of MSDU frames marked as duplicates and non-duplicates acknowledged. The value of this counter may be reset to zero when the CPE is rebooted. Refer section A.2.3.14 of CableLabs Wi-Fi MGMT Specification. */
    ULONG cli_DataFramesSentNoAck;       /**< The total number of MSDU frames retransmitted out of the interface (i.e., marked as duplicate and non-duplicate) and not acknowledged, but does not exclude those defined in the DataFramesLost parameter. The value of this counter may be reset to zero when the CPE is rebooted. Refer section A.2.3.14 of CableLabs Wi-Fi MGMT Specification. */
    ULONG cli_BytesSent;                 /**< The total number of bytes transmitted to the client device, including framing characters. */
    ULONG cli_BytesReceived;             /**< The total number of bytes received from the client device, including framing characters. */
    INT cli_RSSI;                        /**< The Received Signal Strength Indicator (RSSI) parameter is the energy observed at the antenna receiver for transmissions from the device averaged over past 100 packets received from the device. */
    INT cli_MinRSSI;                     /**< The minimum Received Signal Strength Indicator (RSSI) parameter is the minimum energy observed at the antenna receiver for past transmissions (100 packets). */
    INT cli_MaxRSSI;                     /**< The maximum Received Signal Strength Indicator (RSSI) parameter is the maximum energy observed at the antenna receiver for past transmissions (100 packets). */
    UINT cli_Disassociations;           /**< The total number of client disassociations. Reset the parameter every 24 hours or on reboot. */
    UINT cli_AuthenticationFailures;     /**< The total number of authentication failures. Reset the parameter every 24 hours or on reboot. */
    UINT cli_CapableNumSpatialStreams;  /**< The number of spatial streams supported by the associated client device. */
    wifi_mld_sta_info_t cli_MLDInfo;          /**< Wi-Fi 7 MLO client information. */
} wifi_associated_dev_t;

/**
 * @brief Callback function invoked when a new Wi-Fi client associates with an Access Point.
 *
 * This callback function is invoked when a new Wi-Fi client associates with
 * the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex        Index of the Access Point.
 * @param[in] associated_dev Pointer to a structure containing information about the associated device.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 */
typedef INT(* wifi_newApAssociatedDevice_callback)(INT apIndex, wifi_associated_dev_t *associated_dev);
#endif
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Installs the caller's handler for client association events.
 *
 * After registration the `HAL` reports each new client association through the
 * supplied handler, passing the association details rather than only the fact of
 * the event. The most recently registered handler replaces any previous one. This
 * is the association half of the notification pair whose other half is
 * `wifi_apDisassociatedDevice_callback_register()`.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_newApAssociatedDevice_callback`. The `HAL` retains
 *                           this function pointer and invokes it until it is
 *                           replaced, so the function must remain callable for that
 *                           whole period. The effect of passing NULL is not
 *                           specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent association.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_newApAssociatedDevice_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The associated-device structure reaching the handler is owned by the `HAL` and
 *          is valid only for the duration of that call. The client must copy anything it
 *          needs afterwards, per `Asynchronous Notification Model` in
 *          `docs/pages/halSpec.md`.
 *
 * @see wifi_newApAssociatedDevice_callback
 * @see wifi_apDisassociatedDevice_callback_register
 */
void wifi_newApAssociatedDevice_callback_register(wifi_newApAssociatedDevice_callback callback_proc);
/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief Callback function invoked when a Wi-Fi client disassociates from an Access Point.
 *
 * This callback function is invoked when a Wi-Fi client disassociates from
 * the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex     Index of the Access Point.
 * @param[in] MAC          MAC address of the disassociated device.
 * @param[in] event_type  Type of disassociation, either explicit or due to client inactivity.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 */
typedef INT ( * wifi_apDisassociatedDevice_callback)(INT apIndex, char *MAC, INT event_type);

/**
 * @brief Callback function invoked when a client disassociates, reported with both
 *        endpoints of the exchange.
 *
 * This callback carries more of the exchange than `wifi_apDisassociatedDevice_callback`
 * does: both MAC addresses and the management frame type, as well as the cause. It is
 * the form `onewifi` consumes.
 *
 * @param[in] apIndex     Index of the Access Point that observed the event, in the
 *                        range `AP_INDEX_1` to `AP_INDEX_24`.
 * @param[in] src_mac     NUL-terminated MAC address string of the disassociated device.
 * @param[in] dest_mac    NUL-terminated MAC address string of the Access Point.
 * @param[in] frame_type  Management frame type that carried the disassociation.
 * @param[in] event_type  Cause of the disassociation: explicit, or the result of
 *                        client inactivity.
 *
 * @return The status of the operation
 * @retval RETURN_OK if successful
 * @retval RETURN_ERR if any error is detected
 *
 * @execution Synchronous
 * @sideeffect None
 *
 * @note This function must not suspend and must not invoke any blocking system
 * calls. It should probably just send a message to a driver event handler task.
 * @note `src_mac` and `dest_mac` are owned by the `HAL` and are valid only for the duration
 *       of the call; the client must copy them if they are needed later, per `Asynchronous
 *       Notification Model` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_apDisassociatedDevice_callback
 */
typedef INT ( * wifi_device_disassociated_callback)(INT apIndex, char *src_mac,char *dest_mac, INT frame_type, INT event_type);
/* wifi_stamode_callback() function */
/**
* @brief This call back will be invoked for all of these assoc request,reassoc request,eapol frames
*
* @param[in] apIndex          Access Point Index
* @param[in] mac              MAC address of associated device
* @param[in] key_mgmt         authentication key management  of associated device
* @param[in] type             frame type of associated device
* @param[in] radio            radio to which associated device connected
* @param[in] mode             security mode of gateway with respect to akm
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
* @execution Synchronous
* @sideeffect None
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
typedef INT ( * wifi_stamode_callback)(int apIndex, char *mac, int key_mgmt, int type, int radio, int mode);

/**
 * @brief Callback function invoked when a four-way handshake completes or fails.
 *
 * This callback reports the outcome of the WPA/WPA2/WPA3 four-way handshake for one
 * client, which is how a caller distinguishes a client that failed key negotiation
 * from one that never associated. Install it with
 * `wifi_handshake_callback_register()`.
 *
 * @param[in] apIndex  Index of the Access Point the client was authenticating to, in
 *                     the range `AP_INDEX_1` to `AP_INDEX_24`.
 * @param[in] mac      NUL-terminated MAC address string of the client. The `HAL` owns
 *                     this buffer and it is valid only for the duration of the call.
 * @param[in] status   Outcome of the handshake. This interface does not enumerate the
 *                     values, so a caller must not assume they match
 *                     `wifi_status_code_t` or `eapol_msg_type_t`.
 *
 * @returns The status of the operation, which the implementer of the handler returns
 *          to the `HAL`.
 * @retval RETURN_OK  The handler accepted the notification.
 * @retval RETURN_ERR The handler could not process the notification.
 *
 * @note This function must not suspend and must not invoke any blocking system calls. It
 *       should just send a message to a driver event handler task.
 * @note The client must copy `mac` before returning if the value is needed later, per
 *       `Asynchronous Notification Model` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_handshake_callback_register
 */
typedef INT ( * wifi_handshake_callback)(int apIndex, char *mac, int status);

/**
 * @brief Callback function invoked when an EAPOL exchange times out.
 *
 * This callback reports that an EAPOL message was not answered within the configured
 * timeout, distinguishing a client that went away mid-handshake from one that was
 * refused. The relevant timeouts and retry counts are configured through
 * `wifi_vap_security_t`'s `eapol_key_timeout`, `eapol_key_retries`,
 * `eap_identity_req_timeout` and `eap_req_timeout` members.
 *
 * @param[in] apIndex  Index of the Access Point on which the timeout occurred, in the
 *                     range `AP_INDEX_1` to `AP_INDEX_24`.
 * @param[in] mac      NUL-terminated MAC address string of the client. The `HAL` owns
 *                     this buffer and it is valid only for the duration of the call.
 * @param[in] type     Which EAPOL exchange timed out; see `eapol_msg_type_t` for the
 *                     handshake messages this interface names.
 *
 * @returns The status of the operation, which the implementer of the handler returns
 *          to the `HAL`.
 * @retval RETURN_OK  The handler accepted the notification.
 * @retval RETURN_ERR The handler could not process the notification.
 *
 * @note This interface declares this callback type but no registration function for it, so
 *       there is currently no way for a caller to install one. It is documented here
 *       because it is part of the published type surface.
 * @note This function must not suspend and must not invoke any blocking system calls.
 * @note The client must copy `mac` before returning if the value is needed later, per
 *       `Asynchronous Notification Model` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in `docs/pages/halSpec.md`.
 *
 * @see eapol_msg_type_t
 */
typedef INT ( * wifi_eapol_timeouts_callback)(int apIndex, char *mac, int type);
/* wifi_hal_ap_max_client_rejection_callback_register() function */
/**
 * @brief This call back will be called whenever an authentication response with reject reason 17
 * is received.
 *
 * @param[in] apIndex  Index of the Access Point that rejected the client, in the
 *                     range `AP_INDEX_1` to `AP_INDEX_24`.
 * @param[in] MAC      NUL-terminated MAC address string of the rejected client.
 *                     The `HAL` owns this buffer and it is valid only for the
 *                     duration of the call.
 * @param[in] reason   IEEE 802.11 association status code carried by the rejecting
 *                     response. Reason 17 is the association-denied-because-the-AP-is-
 *                     at-capacity case this callback exists to report.
 *
 * @return The status of the operation
 * @retval RETURN_OK if successful
 * @retval RETURN_ERR if any error is detected
 *
 * @execution Synchronous
 * @sideeffect None
 *
 * @note This function must not suspend and must not invoke any blocking system
 * calls. It should probably just send a message to a driver event handler task.
 * @note The client must copy `MAC` before returning if the value is needed later, per
 *       `Asynchronous Notification Model` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in `docs/pages/halSpec.md`.
 *
 */
typedef INT (*wifi_apMaxClientRejection_callback)(INT apIndex, char *MAC, INT reason);

/**
 * @brief Callback function invoked when a management frame carries a status code.
 *
 * This callback reports the IEEE 802.11 status code the Access Point placed in an
 * authentication, association or reassociation response, which lets a caller
 * distinguish a client that was refused from one that never attempted to join.
 *
 * @param[in] apIndex     Index of the Access Point that sent the frame, in the
 *                        range `AP_INDEX_1` to `AP_INDEX_24`.
 * @param[in] src_mac     NUL-terminated MAC address string of the client device.
 * @param[in] dest_mac    NUL-terminated MAC address string of the Access Point.
 * @param[in] frame_type  Management frame type the status code was carried in.
 * @param[in] status      IEEE 802.11 status code from the response frame; the
 *                        values this interface names are in `wifi_status_code_t`.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system calls.
 * @note `src_mac` and `dest_mac` are owned by the `HAL` and are valid only for the duration
 *       of the call; the client must copy them if they are needed later, per `Asynchronous
 *       Notification Model` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_apStatusCode_callback_register
 * @see wifi_status_code_t
 */
typedef INT ( * wifi_apStatusCode_callback)(int apIndex, char *src_mac,char *dest_mac, int frame_type ,int status);

/**
 * @brief Callback function invoked when a RADIUS or EAP authentication fails.
 *
 * This callback reports a RADIUS or EAP failure observed on the specified Access
 * Point, so a caller can act on an authentication that was attempted and refused
 * rather than inferring it from the absence of an association.
 *
 * @param[in] apIndex         Index of the Access Point on which the failure was
 *                            observed, in the range `AP_INDEX_1` to `AP_INDEX_24`.
 * @param[in] sta_mac         MAC address of the station whose authentication failed,
 *                            passed by value as a `mac_address_t`.
 * @param[in] failure_reason  Reason for the failure. The values this interface names
 *                            are in `wifi_eap_status_code_t`.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * @note This function must not suspend and must not invoke any blocking system calls.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_radiusEapFailure_callback_register
 * @see wifi_eap_status_code_t
 */
typedef INT ( * wifi_radiusEapFailure_callback)(INT apIndex, mac_address_t sta_mac, INT failure_reason);
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Installs the caller's handler for RADIUS and EAP authentication failures.
 *
 * After registration the `HAL` reports each RADIUS or EAP failure through the
 * supplied handler, which lets a caller distinguish a client refused by the
 * authentication server from one that never attempted to join. The most recently
 * registered handler replaces any previous one.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_radiusEapFailure_callback`. The `HAL` retains this
 *                           function pointer and invokes it until it is replaced, so
 *                           the function must remain callable for that whole period.
 *                           The effect of passing NULL is not specified by this
 *                           interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent failure.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_radiusEapFailure_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_radiusEapFailure_callback
 * @see wifi_radiusFallback_failover_callback_register
 */
void wifi_radiusEapFailure_callback_register(wifi_radiusEapFailure_callback callback_proc);

/**
 * @brief Installs the caller's handler for management-frame status codes.
 *
 * After registration the `HAL` reports the IEEE 802.11 status code of each
 * authentication, association and reassociation response through the supplied
 * handler. The most recently registered handler replaces any previous one.
 *
 * @param[in] callback_proc  Handler to install, of type `wifi_apStatusCode_callback`.
 *                           The `HAL` retains this function pointer and invokes it
 *                           until it is replaced, so the function must remain
 *                           callable for that whole period. The effect of passing
 *                           NULL is not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent status-code event.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_apStatusCode_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning Frame data reaching the handler is owned by the `HAL` and is valid only for the
 *          duration of that call; the client must copy anything it needs afterwards.
 *
 * @see wifi_apStatusCode_callback
 */
void wifi_apStatusCode_callback_register(wifi_apStatusCode_callback callback_proc);
/**
 * @brief Installs the caller's handler for station-mode association events.
 *
 * After registration the `HAL` reports each association request, reassociation
 * request and EAPOL frame through the supplied handler, together with the key
 * management, radio and security mode the station used. The most recently
 * registered handler replaces any previous one.
 *
 * @param[in] callback_proc  Handler to install, of type `wifi_stamode_callback`.
 *                           The `HAL` retains this function pointer and invokes it
 *                           until it is replaced, so the function must remain
 *                           callable for that whole period. The effect of passing
 *                           NULL is not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent station-mode event.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_stamode_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The MAC address reaching the handler is owned by the `HAL` and is valid only for
 *          the duration of that call; the client must copy it if needed afterwards.
 *
 * @see wifi_stamode_callback
 */
void wifi_ap_stamode_callback_register(wifi_stamode_callback callback_proc);

/**
 * @brief Installs the caller's handler for four-way handshake outcomes.
 *
 * After registration the `HAL` reports the outcome of each WPA/WPA2/WPA3 four-way
 * handshake through the supplied handler, which lets a caller distinguish a client
 * that failed key negotiation from one that never associated. The most recently
 * registered handler replaces any previous one.
 *
 * @param[in] callback_proc  Handler to install, of type `wifi_handshake_callback`.
 *                           The `HAL` retains this function pointer and invokes it
 *                           until it is replaced, so the function must remain
 *                           callable for that whole period. The effect of passing
 *                           NULL is not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent handshake outcome.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_handshake_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The MAC address reaching the handler is owned by the `HAL` and is valid only for
 *          the duration of that call; the client must copy it if needed afterwards, per
 *          `Asynchronous Notification Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_handshake_callback
 */
void wifi_handshake_callback_register(wifi_handshake_callback callback_proc);
/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief Callback function invoked when a RADIUS server fallback failure occurs.
 *
 * This callback function is invoked when a RADIUS server fallback failure occurs on
 * the specified Access Point (AP).
 *
 * @param[in] apIndex         Index of the Access Point.
 * @param[in] failure_reason  Reason for the failure.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 *
 * In current implementation return value is WIFI_HAL_SUCCESS and any failure of the operation
 * is updated in the failure_reason code.
 */
typedef INT ( * wifi_radiusFallback_failover_callback)(INT apIndex, INT failure_reason);
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Installs the caller's handler for RADIUS fallback and failover events.
 *
 * After registration the `HAL` reports each failure to fall back to, or fail over
 * between, the primary and secondary RADIUS servers through the supplied handler.
 * The most recently registered handler replaces any previous one.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_radiusFallback_failover_callback`. The `HAL`
 *                           retains this function pointer and invokes it until it is
 *                           replaced, so the function must remain callable for that
 *                           whole period. The effect of passing NULL is not
 *                           specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent fallback or
 *       failover event.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_radiusFallback_failover_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @note The registered handler is documented as returning `WIFI_HAL_SUCCESS` even where the
 *       operation failed, reporting the failure in its `failure_reason` argument instead; a
 *       caller must read that argument rather than the handler's return value.
 *
 * @see wifi_radiusFallback_failover_callback
 * @see wifi_setApSecuritySecondaryRadiusServer
 */
void wifi_radiusFallback_failover_callback_register(wifi_radiusFallback_failover_callback callback_proc);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Installs the caller's handler for client disassociation events.
 *
 * After registration the `HAL` reports each client disassociation through the
 * supplied handler, together with whether it was explicit or the result of client
 * inactivity. The most recently registered handler replaces any previous one. This
 * is the disassociation half of the notification pair whose other half is
 * `wifi_newApAssociatedDevice_callback_register()`.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_apDisassociatedDevice_callback`. The `HAL` retains
 *                           this function pointer and invokes it until it is
 *                           replaced, so the function must remain callable for that
 *                           whole period. The effect of passing NULL is not
 *                           specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent disassociation.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_apDisassociatedDevice_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The MAC address reaching the handler is owned by the `HAL` and is valid only for
 *          the duration of that call. The client must copy it if it is needed afterwards,
 *          per `Asynchronous Notification Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_apDisassociatedDevice_callback
 * @see wifi_newApAssociatedDevice_callback_register
 */
void wifi_apDisassociatedDevice_callback_register(wifi_apDisassociatedDevice_callback callback_proc);
/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief Callback function invoked when a deauthentication event occurs.
 *
 * This callback function is invoked when a deauthentication event with reason
 * code 2 (wrong password) is received from a client device.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] ap_index  Index of the Access Point.
 * @param[in] mac       MAC address of the client device.
 * @param[in] reason    Reason code for the deauthentication event.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 */
typedef INT ( * wifi_apDeAuthEvent_callback)(int ap_index, char *mac, int reason);
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief Callback function invoked when a client deauthenticates.
 *
 * This callback reports a deauthentication observed by the Access Point, carrying
 * both endpoints of the exchange and the IEEE 802.11 reason code, so a caller can
 * tell a deliberate disconnect from one forced by the network.
 *
 * @param[in] ap_index    Index of the Access Point that observed the event, in the
 *                        range `AP_INDEX_1` to `AP_INDEX_24`.
 * @param[in] src_mac     NUL-terminated MAC address string of the client device.
 * @param[in] dest_mac    NUL-terminated MAC address string of the Access Point.
 * @param[in] frame_type  Management frame type that carried the deauthentication.
 * @param[in] reason      IEEE 802.11 reason code. The values this interface names
 *                        are in `wifi_reason_code_t`.
 *
 * @return The status of the operation
 * @retval RETURN_OK if successful
 * @retval RETURN_ERR if any error is detected
 *
 * @execution Synchronous
 * @sideeffect None
 *
 * @note This function must not suspend and must not invoke any blocking system
 * calls. It should probably just send a message to a driver event handler task.
 * @note `src_mac` and `dest_mac` are owned by the `HAL` and are valid only for the duration
 *       of the call; the client must copy them if they are needed later, per `Asynchronous
 *       Notification Model` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_reason_code_t
 */
typedef INT ( * wifi_device_deauthenticated_callback)(int ap_index, char *src_mac,char *dest_mac, int frame_type, int reason);

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Installs the caller's handler for client deauthentication events.
 *
 * After registration the `HAL` reports each deauthentication through the supplied
 * handler with its IEEE 802.11 reason code, so a caller can act on the wrong-password
 * case (reason code 2) rather than inferring it. The most recently registered handler
 * replaces any previous one.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_apDeAuthEvent_callback`. The `HAL` retains this
 *                           function pointer and invokes it until it is replaced, so
 *                           the function must remain callable for that whole period.
 *                           The effect of passing NULL is not specified by this
 *                           interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent deauthentication.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_apDeAuthEvent_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The MAC address reaching the handler is owned by the `HAL` and is valid only for
 *          the duration of that call. The client must copy it if it is needed afterwards,
 *          per `Asynchronous Notification Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_apDeAuthEvent_callback
 * @see wifi_reason_code_t
 */
void wifi_apDeAuthEvent_callback_register(wifi_apDeAuthEvent_callback callback_proc);

/**
 * @brief Sets the access network type advertised in an Access Point's Interworking
 *        element.
 *
 * This function sets the access network type that will be included in the
 * Interworking Information Element (IE) in the beacons transmitted by the
 * specified Access Point (AP).
 * Refer to section 8.4.2.94 of IEEE Std 802.11-2012 for the possible values
 * of the access network type:
 *  - 0: Private network
 *  - 1: Private network with guest access
 *  - 2: Chargeable public network
 *  - 3: Free public network
 *  - 4: Personal device network
 *  - 5: Emergency services only network
 *  - 6-13: Reserved
 *  - 14: Test or experimental
 *  - 15: Wildcard
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] accessNetworkType  Access network type advertised in the Interworking
 *                               Information Element, per section 8.4.2.94 of IEEE Std
 *                               802.11-2012: 0 private network, 1 private network with
 *                               guest access, 2 chargeable public network, 3 free public
 *                               network, 4 personal device network, 5 emergency services
 *                               only, 6 to 13 reserved, 14 test or experimental, 15
 *                               wildcard.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success beacons from this Access Point carry the requested access network type
 *       in their Interworking Information Element. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The type was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `accessNetworkType` is outside 0 to
 *                          15, or the vendor layer rejected the change. The caller should
 *                          correct the value rather than retrying with the same argument.
 *
 * @note The value only reaches the air while the Interworking Service is enabled; see
 *       `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getInterworkingAccessNetworkType
 * @see wifi_setApInterworkingServiceEnable
 */
INT wifi_setInterworkingAccessNetworkType(INT apIndex, INT accessNetworkType);


/**
 * @brief Reads the access network type currently advertised in an Access Point's
 *        Interworking element.
 *
 * This function retrieves the access network type that is included in the
 * Interworking Information Element (IE) in the beacons transmitted by the
 * specified Access Point (AP).
 * Refer to section 8.4.2.94 of IEEE Std 802.11-2012 for the possible values
 * of the access network type:
 *  - 0: Private network
 *  - 1: Private network with guest access
 *  - 2: Chargeable public network
 *  - 3: Free public network
 *  - 4: Personal device network
 *  - 5: Emergency services only network
 *  - 6-13: Reserved
 *  - 14: Test or experimental
 *  - 15: Wildcard
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_uint  Caller-allocated variable that receives the advertised access
 *                          network type, one of the values listed for
 *                          `wifi_setInterworkingAccessNetworkType()`. The caller
 *                          allocates and releases it; the `HAL` writes into it and
 *                          retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the advertised access network type; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The type was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setInterworkingAccessNetworkType
 */
INT wifi_getInterworkingAccessNetworkType(INT apIndex, UINT *output_uint);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

/**
 * @brief Interworking Element structure.
 *
 * See 802.11-2016 section 9.4.2.92 for field definitions.
 */
typedef struct
{
    BOOL interworkingEnabled;     /**< Indicates whether interworking is enabled. */
    UINT accessNetworkType;      /**< Access network type. */
    BOOL internetAvailable;      /**< Indicates whether internet access is available. */
    BOOL asra;                   /**< Indicates whether Access Network Query Protocol (ANQP) services are supported. */
    BOOL esr;                   /**< Indicates whether Emergency Services Reaching (ESR) is supported. */
    BOOL uesa;                  /**< Indicates whether Unauthenticated Emergency Service Access (UESA) is supported. */
    BOOL venueOptionPresent;     /**< Indicates whether venue information is present. */
    UCHAR venueGroup;            /**< Venue group. */
    UCHAR venueType;             /**< Venue type. */
    BOOL hessOptionPresent;      /**< Indicates whether Hotspot 2.0 Enhanced Security (HESS) information is present. */
    mac_addr_str_t hessid;       /**< HESSID. Optional; use empty string to indicate no value provided. */
} wifi_InterworkingElement_t;

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Gets the Interworking Service enable status for an Access Point.
 *
 * This function retrieves the enable/disable status of the Interworking Service
 * for the specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_bool  Caller-allocated variable that receives true when the
 *                          Interworking Service is enabled, false when it is disabled.
 *                          The caller allocates and releases it; the `HAL` writes into it
 *                          and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the Interworking Service state; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note `wifi_getAPCapabilities()` reports through `interworkingServiceSupported` whether
 *       this Access Point can support the service at all.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setApInterworkingServiceEnable
 * @see wifi_getAPCapabilities
 */
INT wifi_getApInterworkingServiceEnable(INT apIndex, BOOL *output_bool);

/**
 * @brief Sets the Interworking Service enable status for an Access Point.
 *
 * This function sets the enable/disable status of the Interworking Service
 * for the specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] input_bool  true to enable the Interworking Service, false to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Interworking Service is in the requested state. On failure no part
 *       of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `input_bool` is accepted, so
 *                          a failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getApInterworkingServiceEnable()` before relying on it
 *                          rather than retrying with the same argument.
 *
 * @note Disabling the service stops the Interworking Information Element being advertised,
 *       so the access network type and the element's other fields have no effect on the air
 *       until it is enabled again.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApInterworkingServiceEnable
 * @see wifi_pushApInterworkingElement
 */
INT wifi_setApInterworkingServiceEnable(INT apIndex, BOOL input_bool);

/**
 * @brief Gets the Interworking Element for an Access Point.
 *
 * This function retrieves the Interworking Element that will be sent by the
 * specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_struct  Caller-allocated `wifi_InterworkingElement_t` that receives
 *                            the Interworking Information Element this Access Point
 *                            advertises. The caller allocates and releases it; the `HAL`
 *                            writes into it and retains no reference to it after
 *                            returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the advertised element; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The element was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note The structure's `hessid` member is optional and carries an empty string when no
 *       value is provided, as its own member documentation states.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_pushApInterworkingElement
 */
INT wifi_getApInterworkingElement(INT apIndex, wifi_InterworkingElement_t *output_struct);

/**
 * @brief Sets the Interworking Element for an Access Point.
 *
 * This function sets the Interworking Element that will be sent by the
 * specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] infoEelement  Fully populated `wifi_InterworkingElement_t` to advertise. The
 *                          caller owns the structure; the `HAL` reads it during the call
 *                          and must not retain the pointer afterwards. Every member is
 *                          applied, so read the current element first and modify it
 *                          rather than passing a partially filled structure.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success beacons and probe responses carry the supplied Interworking
 *       Information Element. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The element was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `infoEelement` is NULL, or a field
 *                          is not valid for this Access Point. The caller should read the
 *                          element back to establish what took effect, since this
 *                          interface does not state that the update is atomic.
 *
 * @note The parameter name is spelt `infoEelement` in the declaration. That is how it must
 *       be referred to in documentation and is not a typing error in this comment.
 * @note The element reaches the air only while the Interworking Service is enabled.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApInterworkingElement
 * @see wifi_setApInterworkingServiceEnable
 */
INT wifi_pushApInterworkingElement(INT apIndex, wifi_InterworkingElement_t *infoEelement);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

/**
 * @brief Wi-Fi management frame types.
 */
typedef enum
{
    WIFI_MGMT_FRAME_TYPE_INVALID = -1, /**< Invalid management frame type. */
    WIFI_MGMT_FRAME_TYPE_PROBE_REQ = 0, /**< Probe request frame. */
    WIFI_MGMT_FRAME_TYPE_PROBE_RSP = 1, /**< Probe response frame. */
    WIFI_MGMT_FRAME_TYPE_ASSOC_REQ = 2, /**< Association request frame. */
    WIFI_MGMT_FRAME_TYPE_ASSOC_RSP = 3, /**< Association response frame. */
    WIFI_MGMT_FRAME_TYPE_AUTH = 4,     /**< Authentication frame. */
    WIFI_MGMT_FRAME_TYPE_DEAUTH = 5,   /**< Deauthentication frame. */
    WIFI_MGMT_FRAME_TYPE_REASSOC_REQ = 6, /**< Reassociation request frame. */
    WIFI_MGMT_FRAME_TYPE_REASSOC_RSP = 7, /**< Reassociation response frame. */
    WIFI_MGMT_FRAME_TYPE_DISASSOC = 8, /**< Disassociation frame. */
    WIFI_MGMT_FRAME_TYPE_ACTION = 9,   /**< Action frame. */
    WIFI_MGMT_FRAME_TYPE_AUTH_RSP = 10, /**< Authentication response frame. */
    WIFI_MGMT_FRAME_TYPE_BEACON = 11,   /**< Beacon frame. */
} wifi_mgmtFrameType_t;

/**
 * @brief Wi-Fi data frame types.
 */
typedef enum
{
    WIFI_DATA_FRAME_TYPE_INVALID = -1, /**< Invalid data frame type. */
    WIFI_DATA_FRAME_TYPE_8021x,      /**< 802.1x data frame. */
} wifi_dataFrameType_t;

/**
 * @brief Wi-Fi frame types.
 */
typedef enum
{
    WIFI_FRAME_TYPE_INVALID = -1, /**< Invalid frame type. */
    WIFI_FRAME_TYPE_MGMT,      /**< Management frame. */
    WIFI_FRAME_TYPE_CTRL,      /**< Control frame. */
    WIFI_FRAME_TYPE_DATA,      /**< Data frame. */
} wifi_frameType_t;

/**
 * @brief Structure representing a Wi-Fi frame.
 */
typedef struct
{
    INT ap_index;               /**< Index of the Access Point. */
    mac_address_t sta_mac;      /**< MAC address of the station. */
    wifi_mgmtFrameType_t type;  /**< Type of the frame. */
    wifi_direction_t dir;       /**< Direction of the frame. */
    INT sig_dbm;                /**< Signal strength in dBm. */
    INT phy_rate;               /**< Physical rate. */
    UCHAR token;                /**< Token. */
    UINT recv_freq;             /**< Frequency at which the frame was received. */
    UINT len;                   /**< Length of the data. */
    UCHAR *data;                /**< Pointer to the data. */
} __attribute__((packed)) wifi_frame_t;

/**
 * @brief Callback function invoked when an 802.1x frame is received.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] type     EAPOL type.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 */
typedef void (* wifi_received8021xFrame_callback)(unsigned int ap_index, mac_address_t sta, wifi_eapol_type_t type, void *data, unsigned int len);


/**
 * @brief Callback function invoked when an 802.1x frame is sent.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] type     EAPOL type.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 */
typedef void (* wifi_sent8021xFrame_callback)(unsigned int ap_index, mac_address_t sta, wifi_eapol_type_t type, void *data, unsigned int len);


/**
 * @brief Callback function invoked when an authentication frame is received.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 */
typedef void (* wifi_receivedAuthFrame_callback)(unsigned int ap_index, mac_address_t sta, void *data, unsigned int len);


/**
 * @brief Callback function invoked when an authentication frame is sent.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 */
typedef void (* wifi_sentAuthFrame_callback)(unsigned int ap_index, mac_address_t sta, void *data, unsigned int len);


/**
 * @brief Callback function invoked when an association request frame is received.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 */
typedef void (* wifi_receivedAssocReqFrame_callback)(unsigned int ap_index, mac_address_t sta, void *data, unsigned int len);


/**
 * @brief Callback function invoked when an association response frame is sent.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 */
typedef void (* wifi_sentAssocRspFrame_callback)(unsigned int ap_index, mac_address_t sta, void *data, unsigned int len);


/**
 * @brief Frame hook function type.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] type     Type of the management frame.
 *
 * @returns The status of the operation.
 */
typedef INT (* wifi_hal_frame_hook_fn_t)(INT ap_index, wifi_mgmtFrameType_t type);


#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Callback function invoked when a management frame is received.
 *
 * @param[in] apIndex Index of the Access Point.
 * @param[in] frame   Pointer to the received frame.
 *
 * @returns The status of the operation.
 */
typedef INT (* wifi_receivedMgmtFrame_callback)(INT apIndex, wifi_frame_t *frame);


/**
 * @brief Callback function invoked when a data frame is received.
 *
 * @param[in] apIndex  Index of the Access Point.
 * @param[in] sta_mac  MAC address of the station.
 * @param[in] frame    Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 * @param[in] type     Type of the data frame.
 * @param[in] dir      Direction of the data frame.
 *
 * @returns The status of the operation.
 */
typedef INT (* wifi_receivedDataFrame_callback)(INT apIndex, mac_address_t sta_mac, UCHAR *frame, UINT len, wifi_dataFrameType_t type, wifi_direction_t dir);
#else
/**
 * @brief Callback function invoked when a management frame is received.
 *
 * @param[in] apIndex  Index of the Access Point.
 * @param[in] sta_mac  Pointer to the MAC address of the station.
 * @param[in] frame    Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 * @param[in] type     Type of the management frame.
 * @param[in] dir      Direction of the management frame.
 * @param[in] recv_freq Frequency at which the frame was received.
 *
 * @returns The status of the operation.
 */
typedef INT (* wifi_receivedMgmtFrame_callback)(INT apIndex, UCHAR *sta_mac, UCHAR *frame, UINT len, wifi_mgmtFrameType_t type, wifi_direction_t dir, unsigned int recv_freq);


/**
 * @brief Callback function invoked when a data frame is received.
 *
 * @param[in] apIndex  Index of the Access Point.
 * @param[in] sta_mac  Pointer to the MAC address of the station.
 * @param[in] frame    Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 * @param[in] type     Type of the data frame.
 * @param[in] dir      Direction of the data frame.
 *
 * @returns The status of the operation.
 */
typedef INT (* wifi_receivedDataFrame_callback)(INT apIndex, UCHAR *sta_mac, UCHAR *frame, UINT len, wifi_dataFrameType_t type, wifi_direction_t dir);
#endif


/**
 * @brief Callback function invoked when CSI data is available.
 *
 * @param[in] mac_addr  MAC address of the station.
 * @param[in] csi_data  Pointer to the CSI data.
 *
 * @returns The status of the operation.
 */
typedef INT(* wifi_csi_callback)(mac_address_t mac_addr, wifi_csi_data_t *csi_data);
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */

/**
 * @brief Installs the caller's handler for received management frames.
 *
 * After registration the `HAL` delivers each received management frame to the
 * supplied handler, which is how a caller observes probe, authentication,
 * association and action frames without owning the radio. The most recently
 * registered handler replaces any previous one.
 *
 * @param[in] mgmtRxCallback  Handler to install, of type
 *                            `wifi_receivedMgmtFrame_callback`. The `HAL` retains
 *                            this function pointer and invokes it until it is
 *                            replaced, so the function must remain callable for that
 *                            whole period. The effect of passing NULL is not
 *                            specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. A call made beforehand fails with
 *      `WIFI_HAL_ERROR` and installs nothing.
 * @post On success the handler is installed and is invoked for each subsequent
 *       management frame. On failure no handler is installed and any previously
 *       registered handler remains in place.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was installed.
 * @retval WIFI_HAL_ERROR   The handler could not be installed, for example because
 *                          the `HAL` is not initialised or the vendor layer cannot
 *                          deliver management frames on this platform. The caller
 *                          should log the failure and treat management-frame
 *                          notification as unavailable rather than retrying in a
 *                          loop, because the condition is not transient.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous; delivery of
 *       `wifi_receivedMgmtFrame_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The frame buffer reaching the handler is owned by the `HAL` and is valid only
 *          for the duration of that call. The client must copy any part of it that is
 *          needed afterwards, per `Asynchronous Notification Model` in
 *          `docs/pages/halSpec.md`.
 *
 * @see wifi_receivedMgmtFrame_callback
 * @see wifi_hal_register_frame_hook
 */
INT wifi_mgmt_frame_callbacks_register(wifi_receivedMgmtFrame_callback mgmtRxCallback);

/**
 * @brief Installs the caller's handler for Channel State Information data.
 *
 * After registration the `HAL` delivers Channel State Information (`CSI`) to the
 * supplied handler as it becomes available. Registration alone does not start the
 * flow: the handler is invoked only while the `CSI` engine is enabled for at least
 * one station, which is what `wifi_enableCSIEngine()` controls. The most recently
 * registered handler replaces any previous one.
 *
 * @param[in] callback_proc  Handler to install, of type `wifi_csi_callback`. The
 *                           `HAL` retains this function pointer and invokes it until
 *                           it is replaced, so the function must remain callable for
 *                           that whole period. The effect of passing NULL is not
 *                           specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed. It is invoked only for stations whose `CSI` engine
 *       is enabled, so a correct registration produces no callbacks at all until
 *       `wifi_enableCSIEngine()` enables one.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_csi_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       should just send a message to a driver event handler task. `CSI` can arrive at a
 *       high rate, so any per-sample work belongs on the caller's own thread.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The `wifi_csi_data_t` reaching the handler is owned by the `HAL` and is valid
 *          only for the duration of that call. The client must copy it if it is needed
 *          afterwards, per `Asynchronous Notification Model` in `docs/pages/halSpec.md`.
 *
 * @see wifi_csi_callback
 * @see wifi_enableCSIEngine
 */
void wifi_csi_callback_register(wifi_csi_callback callback_proc);

/**
 * @brief Installs an application frame hook invoked on management frame reception.
 *
 * The hook is an optional application-level extension point: it is consulted when a
 * management frame arrives, receives only the Access Point index and the frame type
 * rather than the frame body, and is simply not called if no application installs
 * one. An application that needs the frame contents should use
 * `wifi_mgmt_frame_callbacks_register()` instead. The most recently registered hook
 * replaces any previous one.
 *
 * @param[in] hook_fn  Hook to install, of type `wifi_hal_frame_hook_fn_t`. The `HAL`
 *                     retains this function pointer and invokes it until it is
 *                     replaced, so the function must remain callable for that whole
 *                     period. The effect of passing NULL is not specified by this
 *                     interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The hook is installed and is consulted on each subsequent management frame
 *       reception.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous and returns nothing; invocation of
 *       `wifi_hal_frame_hook_fn_t` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the hook, which
 *       runs on the frame reception path.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @note This interface does not specify what the `HAL` does with the hook's return value,
 *       so a hook must not assume it can suppress or alter frame processing by returning a
 *       particular status.
 *
 * @see wifi_hal_frame_hook_fn_t
 * @see wifi_mgmt_frame_callbacks_register
 */
void wifi_hal_register_frame_hook(wifi_hal_frame_hook_fn_t hook_fn);

/**
 * @brief Enables or disables the CSI engine for a specific station on a VAP.
 *
 * This function enables or disables the CSI engine for a specific station
 * associated with a Virtual Access Point (VAP).
 * If the MAC address is null, the enable argument must be false, otherwise
 * the function must return failure.
 * If the MAC address is null, the data engine for all STAs needs to be disabled
 * on this VAP.
 *
 * @param[in] apIndex  Index of the Virtual Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] sta  MAC address of the station, passed by value as a `mac_address_t`. An
 *                 all-zero address means every station on this Virtual Access Point, and
 *                 is only legal with `enable` false.
 * @param[in] enable  true to enable the Channel State Information engine for that
 *                    station, false to disable it. Must be false when `sta` is the
 *                    all-zero address.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Channel State Information engine is in the requested state for
 *       that station, or for every station on the Virtual Access Point when `sta` was the
 *       all-zero address and `enable` was false. On failure no part of the configuration
 *       is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The engine state was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `enable` is true while `sta` is the
 *                          all-zero address, the station is not associated, or the vendor
 *                          layer rejected the change. The caller must not retry the
 *                          null-address-with-enable combination, which this interface
 *                          requires to fail.
 *
 * @note Data only reaches the caller once a handler is installed with
 *       `wifi_csi_callback_register()`; enabling the engine without one produces no
 *       callbacks.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_csi_callback_register
 */
INT wifi_enableCSIEngine(INT apIndex, mac_address_t sta, BOOL enable);
/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief Ethernet type values.
 */
#define WIFI_ETH_TYPE_IP       0x0800  /**< IPv4 */
#define WIFI_ETH_TYPE_ARP      0x0806  /**< Address Resolution Protocol */
#define WIFI_ETH_TYPE_REVARP   0x8035  /**< Reverse Address Resolution Protocol */
#define WIFI_ETH_TYPE_VLAN     0x8100  /**< VLAN tag */
#define WIFI_ETH_TYPE_LOOPBACK 0x9000  /**< Loopback */
#define WIFI_ETH_TYPE_IP6      0x86DD  /**< IPv6 */
#define WIFI_ETH_TYPE_EAPOL    0x888e  /**< EAP over LAN */

/**
 * @brief Wi-Fi data priority values.
 */
typedef enum
{
    wifi_data_priority_be,  /**< Best effort */
    wifi_data_priority_bk,  /**< Background */
    wifi_data_priority_ee,  /**< Excellent effort */
    wifi_data_priority_ca,  /**< Critical applications */
    wifi_data_priority_vi,  /**< Video */
    wifi_data_priority_vo,  /**< Voice */
    wifi_data_prioirty_ic,  /**< Internetwork control */
    wifi_data_priority_nc   /**< Network control */
} wifi_data_priority_t;
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Sends a data frame to a client associated with a VAP.
 *
 * This function sends a data frame to a client associated with the specified
 * Virtual Access Point (VAP) index. If the MAC address is null, the data
 * should be broadcast on the VAP.
 *
 * @param[in] apIndex  Index of the Virtual Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] sta  MAC address of the destination station, passed by value as a
 *                 `mac_address_t`. An all-zero address broadcasts the frame on the
 *                 Virtual Access Point.
 * @param[in] data  Caller-owned buffer holding `len` bytes of payload beginning at Layer
 *                  3; it must not include any Layer 2 header. The `HAL` reads it during
 *                  the call and must not retain the pointer afterwards.
 * @param[in] len  Length of `data` in bytes. This interface states no maximum, so a
 *                 caller should keep the frame within the medium's MTU rather than
 *                 relying on the `HAL` to fragment it.
 * @param[in] insert_llc  true to have the implementation insert an LLC header before the
 *                        type field -- DSAP 0xaa, SSAP 0xaa, Control 0x03, then three
 *                        0x00 bytes -- and false to transmit the payload without one.
 * @param[in] eth_proto  Ethernet protocol type for the frame. The values this interface
 *                       names are `WIFI_ETH_TYPE_IP`, `WIFI_ETH_TYPE_ARP`,
 *                       `WIFI_ETH_TYPE_REVARP`, `WIFI_ETH_TYPE_VLAN`,
 *                       `WIFI_ETH_TYPE_LOOPBACK`, `WIFI_ETH_TYPE_IP6` and
 *                       `WIFI_ETH_TYPE_EAPOL`.
 * @param[in] prio  Transmit priority, one of the `wifi_data_priority_t` values, which
 *                  selects the WMM access category the frame is queued on.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the frame has been handed to the driver for transmission. On failure
 *       no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The frame was accepted for transmission.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `data` is NULL, `len` is 0, or the
 *                          driver refused the frame. The caller should validate the
 *                          buffer and, for a unicast address, confirm the station is
 *                          associated before retrying.
 *
 * @note Success means the driver accepted the frame, not that it reached the air or the
 *       peer; this interface provides no transmit confirmation.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_data_priority_t
 * @see wifi_sendActionFrame
 */
INT wifi_sendDataFrame(INT apIndex,
                       mac_address_t sta,
                       UCHAR *data,
                       UINT len,
                       BOOL insert_llc,
                       UINT eth_proto,
                       wifi_data_priority_t prio);

/**
 * @brief Transmits an action frame to a station from a specific VAP.
 *
 * @param[in] apIndex  Index of the Virtual Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] sta  MAC address of the peer to send the frame to, passed by value as a
 *                 `mac_address_t`.
 * @param[in] frequency  Channel frequency to transmit on, which allows a public action
 *                       frame to be sent to a device on an off-channel. Pass 0 to use the
 *                       Virtual Access Point's operating channel.
 * @param[in] frame  Caller-owned buffer holding the frame to transmit. The `HAL` reads
 *                   `len` bytes during the call and must not retain the pointer
 *                   afterwards, so the caller may pass a stack buffer and may reuse or
 *                   free it as soon as the call returns.
 * @param[in] len  Length of `frame` in bytes.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the action frame has been handed to the driver for transmission. On
 *       failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The frame was accepted for transmission.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `frame` is NULL, `len` is 0, or the
 *                          frequency is not usable. The caller should validate the buffer
 *                          and frequency rather than retrying unchanged.
 *
 * @note Use `wifi_sendActionFrameExt()` where the radio must dwell on an off-channel
 *       frequency after transmitting to receive a response; this call states no dwell time.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_sendActionFrameExt
 */
INT wifi_sendActionFrame(INT apIndex,
                         mac_address_t sta,
                         UINT frequency,
                         UCHAR *frame,
                         UINT len);

/**
 * @brief Transmits an action frame to a station from a specific VAP.
 *
 * @param[in] apIndex  Index of the Virtual Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] sta  MAC address of the peer to send the frame to, passed by value as a
 *                 `mac_address_t`.
 * @param[in] frequency  Channel frequency to transmit on, which allows a public action
 *                       frame to be sent to a device on an off-channel. Pass 0 to use the
 *                       Virtual Access Point's operating channel.
 * @param[in] wait  Time in milliseconds to remain on an off-channel frequency after
 *                  transmitting, before returning to the operating channel. Pass 0 not to
 *                  wait.
 * @param[in] frame  Caller-owned buffer holding the frame to transmit. The `HAL` reads
 *                   `len` bytes during the call and must not retain the pointer
 *                   afterwards, so the caller may pass a stack buffer and may reuse or
 *                   free it as soon as the call returns.
 * @param[in] len  Length of `frame` in bytes.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the action frame has been handed to the driver for transmission, and
 *       the radio remains on the given frequency for `wait` milliseconds before returning
 *       to its operating channel. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The frame was accepted for transmission.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `frame` is NULL, `len` is 0, or the
 *                          frequency is not usable. The caller should validate the buffer
 *                          and frequency rather than retrying unchanged.
 *
 * @note A non-zero `wait` takes the radio off its operating channel for that period, during
 *       which associated clients are not served. Keep it as short as the exchange allows.
 * @note This interface does not state whether the call returns before or after the dwell
 *       period elapses, so a caller must not use it as a timing primitive.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_sendActionFrame
 */
INT wifi_sendActionFrameExt(INT apIndex,
                            mac_address_t sta,
                            UINT frequency,
                            UINT wait,
                            UCHAR *frame,
                            UINT len);

/**
 * @brief GAS configuration type.
 */
#define GAS_CFG_TYPE_SUPPORTED 1  /**< Indicates that GAS configuration is supported. */

/**
 * @brief GAS configuration structure.
 *
 * Values correspond to the `dot11GASAdvertisementEntry` field definitions;
 * see 802.11-2016 Annex C.3.
 */
typedef struct _wifi_GASConfiguration_t
{
    UINT AdvertisementID;          /**< Advertisement ID. */
    BOOL PauseForServerResponse;   /**< Whether to pause for server response. */
    UINT ResponseTimeout;          /**< Response timeout in seconds. */
    UINT ComeBackDelay;            /**< Comeback delay in seconds. */
    UINT ResponseBufferingTime;    /**< Response buffering time in seconds. */
    UINT QueryResponseLengthLimit; /**< Query response length limit in bytes. */
} __attribute__((packed)) wifi_GASConfiguration_t;

/**
 * @brief Applies the device-wide Generic Advertisement Service advertisement parameters.
 *
 * This function installs the `dot11GASAdvertisementEntry` parameters that govern how the
 * device answers Generic Advertisement Service (`GAS`) queries: the advertisement
 * identifier it answers for, whether it pauses for a server response, and the response
 * timeout, comeback delay, response buffering time and query response length limit it
 * applies. The structure carries the complete parameter set, so a caller changing one
 * value passes the values it wants to keep for the rest.
 *
 * @param[in] input_struct  Fully populated `wifi_GASConfiguration_t` to apply, whose
 *                          members correspond to the `dot11GASAdvertisementEntry` fields
 *                          of 802.11-2016 Annex C.3. The caller owns the structure; the
 *                          `HAL` reads it during the call and must not retain the pointer
 *                          afterwards. `GAS_CFG_TYPE_SUPPORTED` is the one configuration
 *                          type this interface names.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Generic Advertisement Service uses the supplied advertisement
 *       identifier, response timeout, comeback delay, buffering time and query response
 *       length limit. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was applied.
 * @retval WIFI_HAL_ERROR   `input_struct` is NULL or a member is outside the range the
 *                          vendor layer accepts. The caller should correct the structure
 *                          rather than retrying with the same contents.
 *
 * @note This call takes no Access Point index: the Generic Advertisement Service
 *       configuration applies device-wide, unlike the per-Access-Point interworking calls.
 * @note `ResponseTimeout`, `ComeBackDelay` and `ResponseBufferingTime` are in seconds and
 *       `QueryResponseLengthLimit` is in bytes, as the structure's own member documentation
 *       states.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_GASConfiguration_t
 */
INT wifi_applyGASConfiguration(wifi_GASConfiguration_t *input_struct);

/* Passpoint R1 related APIs */

/**
 * @brief Sets the country code information element in beacon and probe responses.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enabled  true to include the Country information element in beacons and
 *                     probe responses, false to exclude it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success beacons and probe responses carry the Country information element when
 *       `enabled` is true, and omit it when it is false. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enabled` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getCountryIe()` before relying on it rather than retrying
 *                          with the same argument.
 *
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getCountryIe
 */
INT wifi_setCountryIe(INT apIndex, BOOL enabled);

/**
 * @brief Gets the status of the country code information element.
 *
 * This function retrieves the status of the country code information element
 * in beacon and probe responses for the specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] enabled  Caller-allocated variable that receives true when the Country
 *                      information element is included in beacons and probe responses,
 *                      false when it is excluded. The caller allocates and releases it;
 *                      the `HAL` writes into it and retains no reference to it after
 *                      returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the Country information element state; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setCountryIe
 */
INT wifi_getCountryIe(INT apIndex, BOOL *enabled);

/**
 * @brief Enables or disables Layer 2 traffic inspection and filtering.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enabled  true to enable Layer 2 traffic inspection and filtering, false to
 *                     disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point inspects and filters Layer 2 traffic when `enabled`
 *       is true, and does not when it is false. On failure no part of the configuration
 *       is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enabled` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getLayer2TrafficInspectionFiltering()` before relying on
 *                          it rather than retrying with the same argument.
 *
 * @note This is the Hotspot 2.0 L2TIF control, mirrored by the `l2tif` member of
 *       `wifi_passpoint_settings_t`.
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getLayer2TrafficInspectionFiltering
 */
INT wifi_setLayer2TrafficInspectionFiltering(INT apIndex, BOOL enabled);

/**
 * @brief Gets the status of Layer 2 traffic inspection and filtering.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] enabled  Caller-allocated variable that receives true when Layer 2 traffic
 *                      inspection and filtering is enabled, false when it is disabled.
 *                      The caller allocates and releases it; the `HAL` writes into it and
 *                      retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the Layer 2 traffic inspection and filtering state;
 *       on failure the output is left unspecified, so a caller must not read it unless
 *       the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setLayer2TrafficInspectionFiltering
 */
INT wifi_getLayer2TrafficInspectionFiltering(INT apIndex, BOOL *enabled);

/**
 * @brief Sets or clears the DGAF-disabled bit advertised in the Hotspot 2.0 Indication
 *        element.
 *
 * When `disabled` is true the Downstream Group-Addressed Forwarding (`DGAF`) disabled bit
 * is set in the HS2.0 Indication Information Element carried in beacons and probe
 * responses; when it is false the bit is cleared. The name reads as a setter for the
 * downstream group address behaviour, but the value written is the disable bit, so true
 * suppresses forwarding rather than enabling it.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] disabled  true to disable Downstream Group-Addressed Forwarding, which sets
 *                      the DGAF-disabled bit; false to enable forwarding, which clears
 *                      it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the DGAF-disabled bit of the Hotspot 2.0 Indication information
 *       element is set when `disabled` is true, and clear when it is false. On failure no
 *       part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `disabled` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getDownStreamGroupAddress()` before relying on it rather
 *                          than retrying with the same argument.
 *
 * @note The argument's sense is inverted relative to the other toggles here: true disables
 *       the forwarding behaviour rather than enabling a feature.
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getDownStreamGroupAddress
 */
INT wifi_setDownStreamGroupAddress(INT apIndex, BOOL disabled);

/**
 * @brief Gets the status of DGAF disable.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] disabled  Caller-allocated variable that receives true when Downstream
 *                       Group-Addressed Forwarding is disabled, false when it is enabled.
 *                       The caller allocates and releases it; the `HAL` writes into it
 *                       and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the DGAF-disabled state; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setDownStreamGroupAddress
 */
INT wifi_getDownStreamGroupAddress(INT apIndex, BOOL *disabled);

/**
 * @brief Enables or disables the BSS Load Information Element.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enabled  true to include the BSS Load information element in beacons and
 *                     probe responses, false to exclude it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success beacons and probe responses carry the BSS Load information element
 *       when `enabled` is true, and omit it when it is false. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enabled` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getBssLoad()` before relying on it rather than retrying
 *                          with the same argument.
 *
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getBssLoad
 */
INT wifi_setBssLoad(INT apIndex, BOOL enabled);

/**
 * @brief Gets the status of the BSS Load Information Element.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] enabled  Caller-allocated variable that receives true when the BSS Load
 *                      information element is included, false when it is excluded. The
 *                      caller allocates and releases it; the `HAL` writes into it and
 *                      retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the BSS Load information element state; on failure
 *       the output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setBssLoad
 */
INT wifi_getBssLoad(INT apIndex, BOOL *enabled);

/**
 * @brief Enables or disables proxy ARP on the device driver.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enabled  true to enable proxy ARP in the device driver, false to disable it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the device driver answers ARP on behalf of associated clients when
 *       `enabled` is true, and does not when it is false. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enabled` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getProxyArp()` before relying on it rather than retrying
 *                          with the same argument.
 *
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getProxyArp
 */
INT wifi_setProxyArp(INT apIndex, BOOL enabled);

/**
 * @brief Gets the status of proxy ARP from the driver.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] enabled  Caller-allocated variable that receives true when proxy ARP is
 *                      enabled in the driver, false when it is disabled. The caller
 *                      allocates and releases it; the `HAL` writes into it and retains no
 *                      reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the proxy ARP state as reported by the driver; on
 *       failure the output is left unspecified, so a caller must not read it unless the
 *       call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setProxyArp
 */
INT wifi_getProxyArp(INT apIndex, BOOL *enabled);

/**
 * @brief Sets the Hotspot 2.0 status for an Access Point.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enabled  true to enable Hotspot 2.0 on this Access Point, false to disable
 *                     it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point advertises Hotspot 2.0 when `enabled` is true, and
 *       does not when it is false. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `enabled` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getApHotspotElement()` before relying on it rather than
 *                          retrying with the same argument.
 *
 * @note Hotspot 2.0 advertisement builds on the Interworking Service, so enabling it here
 *       without enabling that service has no effect on the air.
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApHotspotElement
 */
INT wifi_pushApHotspotElement(INT apIndex, BOOL enabled);

/**
 * @brief Gets the Hotspot 2.0 status for an Access Point.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] enabled  Caller-allocated variable that receives true when Hotspot 2.0 is
 *                      enabled on this Access Point, false when it is disabled. The
 *                      caller allocates and releases it; the `HAL` writes into it and
 *                      retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the Hotspot 2.0 state; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_pushApHotspotElement
 */
INT wifi_getApHotspotElement(INT apIndex, BOOL *enabled);

/**
 * @brief Sets the Roaming Consortium Information Element contents.
 *
 * This function sets the contents of the Roaming Consortium Information
 * Element that will be included in beacon and probe responses when
 * interworking is enabled and the OI count is greater than 0.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] infoElement  Fully populated `wifi_roamingConsortiumElement_t` carrying the
 *                         Organisation Identifier count, the length of each of the first
 *                         three OIs and the OIs themselves as hex strings. Each OI is 3
 *                         to 15 octets, and only three fit in a beacon or probe response.
 *                         The caller owns the structure; the `HAL` reads it during the
 *                         call and must not retain the pointer afterwards.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success beacons and probe responses carry the supplied Roaming Consortium
 *       information element, provided interworking is enabled and the OI count is greater
 *       than 0. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The element was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `infoElement` is NULL, the OI count
 *                          exceeds the three the structure holds, or an OI length is
 *                          outside 3 to 15 octets. The caller should correct the
 *                          structure rather than retrying with the same contents.
 *
 * @note The element is advertised only while interworking is enabled and the OI count is
 *       greater than 0; either condition unmet leaves it off the air.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getApRoamingConsortiumElement
 * @see wifi_setApInterworkingServiceEnable
 */
INT wifi_pushApRoamingConsortiumElement(INT apIndex, wifi_roamingConsortiumElement_t *infoElement);

/**
 * @brief Gets the Roaming Consortium Information Element contents.
 *
 * This function retrieves the contents of the Roaming Consortium Information
 * Element. If the Roaming Consortium IE is not present, the count will be
 * returned as 0, and the length and OI fields can be ignored.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] infoElement  Caller-allocated `wifi_roamingConsortiumElement_t` that
 *                          receives the advertised Organisation Identifier count, the
 *                          length of each of the first three OIs and the OIs themselves
 *                          as hex strings. The caller allocates and releases it; the
 *                          `HAL` writes into it and retains no reference to it after
 *                          returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the advertised element, where a count of 0 means the
 *       element is not present and the length and OI fields carry nothing meaningful; on
 *       failure the output is left unspecified, so a caller must not read it unless the
 *       call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The element was retrieved. A count of 0 is a success meaning
 *                          no element is advertised.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note Test the count before reading the length or OI fields: at a count of 0 they are
 *       explicitly not meaningful.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_pushApRoamingConsortiumElement
 */
INT wifi_getApRoamingConsortiumElement(INT apIndex, wifi_roamingConsortiumElement_t *infoElement);

/**
 * @brief Disables or enables P2P cross connect.
 *
 * When set to true, this function includes the P2P Information Element in
 * beacon and probe responses, with the P2P Manageability attribute's
 * "Cross Connection Permitted" field set to 0.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] disabled  true to disable P2P cross connect, which advertises the P2P
 *                      information element with "Cross Connection Permitted" set to 0;
 *                      false to permit cross connection.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the P2P information element is advertised with the P2P Manageability
 *       attribute's "Cross Connection Permitted" field set to 0 when `disabled` is true.
 *       On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, or the vendor layer could not apply
 *                          the requested state. Every value of `disabled` is accepted, so a
 *                          failure reflects the index or the vendor layer rather than the
 *                          argument; the caller should read the setting back with
 *                          `wifi_getP2PCrossConnect()` before relying on it rather than
 *                          retrying with the same argument.
 *
 * @note The argument's sense is inverted relative to the other toggles here: true disables
 *       cross connection rather than enabling a feature.
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getP2PCrossConnect
 */
INT wifi_setP2PCrossConnect(INT apIndex, BOOL disabled);

/**
 * @brief Gets the P2P cross connect disabled status.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] disabled  Caller-allocated variable that receives true when P2P cross
 *                       connect is disabled, false when it is permitted. The caller
 *                       allocates and releases it; the `HAL` writes into it and retains
 *                       no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the P2P cross connect state; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setP2PCrossConnect
 */
INT wifi_getP2PCrossConnect(INT apIndex, BOOL *disabled);

/**
 * @brief Gets all TWT sessions connected to an Access Point.
 *
 * This function retrieves all Target Wake Time (TWT) sessions, both individual
 * and broadcast, that are connected to the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] ap_index  Index of the Access Point, in the range `AP_INDEX_1` to
 *                      `AP_INDEX_24`.
 * @param[in] maxNumberSessions  Number of elements the `twtSessions` array can hold.
 *                               `MAX_NUM_TWT_SESSION` is 50, the largest session
 *                               population this interface names, so an array sized to
 *                               that constant cannot overflow.
 * @param[out] twtSessions  Caller-allocated array of at least `maxNumberSessions`
 *                          `wifi_twt_sessions_t` elements that receives the session
 *                          descriptions. The caller allocates and releases it; the `HAL`
 *                          writes into it and retains no reference to it after returning.
 *                          Only the first `*numSessionReturned` elements are written.
 * @param[out] numSessionReturned  Caller-allocated variable that receives the number of
 *                                 elements actually written, never more than
 *                                 `maxNumberSessions`. The caller allocates and releases
 *                                 it; the `HAL` writes into it and retains no reference
 *                                 to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds `*numSessionReturned` session descriptions, both
 *       individual and broadcast, with the remaining array elements untouched; on failure
 *       the output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The sessions were retrieved. Zero sessions is a success, not
 *                          an error.
 * @retval WIFI_HAL_ERROR   `ap_index` is out of range, a required output pointer is NULL,
 *                          or the array is too small for the current session count. The
 *                          caller should validate its arguments; a failure that persists
 *                          across retries should be logged and the value treated as
 *                          unavailable.
 *
 * @note Both individual and broadcast Target Wake Time sessions are returned in the same
 *       array; the caller distinguishes them from the session description itself.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_setBroadcastTWTSchedule
 * @see wifi_setTeardownTWTSession
 */
INT wifi_getTWTsessions(INT ap_index, UINT maxNumberSessions, wifi_twt_sessions_t * twtSessions, UINT* numSessionReturned);


/**
 * @brief Creates or updates a broadcast TWT session.
 *
 * This function creates or updates a broadcast Target Wake Time (TWT) session
 * for the specified Access Point (AP).
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] ap_index  Index of the Access Point, in the range `AP_INDEX_1` to
 *                      `AP_INDEX_24`.
 * @param[in] twtParams  Target Wake Time parameters for the session, passed by value as a
 *                       `wifi_twt_params_t`, so no caller storage is retained.
 * @param[in] create  true to create a new session, false to update the existing session
 *                    identified by `*sessionID`.
 * @param[in,out] sessionID  On entry, when `create` is false, the identifier of the
 *                           session to update. On exit, when `create` is true, the
 *                           identifier of the newly created session. Caller-allocated;
 *                           the `HAL` retains no reference to it after returning. It must
 *                           point to valid storage in both directions, and must hold a
 *                           valid identifier before an update.
 *
 * @pre `wifi_init()` must have completed successfully, and when `create` is false
 *      `*sessionID` must already identify an existing session; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. A call made before initialisation, or an
 *      update naming a session that does not exist, fails with `WIFI_HAL_ERROR` and
 *      changes no schedule.
 * @post On success a broadcast Target Wake Time session exists with the supplied
 *       parameters, and `*sessionID` identifies it. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The session was created or updated, and `*sessionID`
 *                          identifies it.
 * @retval WIFI_HAL_ERROR   `ap_index` is out of range, `sessionID` is NULL, an update
 *                          names a session that does not exist, `MAX_NUM_TWT_SESSION`
 *                          sessions already exist, or a parameter was rejected. The
 *                          caller should enumerate the sessions with
 *                          `wifi_getTWTsessions()` to establish the actual state before
 *                          retrying.
 *
 * @note On failure this interface does not state whether `*sessionID` was modified, so a
 *       caller must not read it unless the call succeeded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getTWTsessions
 * @see wifi_setTeardownTWTSession
 */
INT wifi_setBroadcastTWTSchedule(INT ap_index, wifi_twt_params_t twtParams, BOOL create, INT* sessionID);


/**
 * @brief Tears down a TWT session.
 *
 * This function tears down the individual or broadcast Target Wake Time (TWT)
 * session associated with the specified Access Point (AP) and session ID.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] ap_index  Index of the Access Point, in the range `AP_INDEX_1` to
 *                      `AP_INDEX_24`.
 * @param[in] sessionID  Identifier of the Target Wake Time session to tear down, as
 *                       returned by `wifi_setBroadcastTWTSchedule()` or reported by
 *                       `wifi_getTWTsessions()`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the identified session no longer exists. On failure no part of the
 *       configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The session was torn down.
 * @retval WIFI_HAL_ERROR   `ap_index` is out of range, `sessionID` does not identify an
 *                          existing session, or the vendor layer could not tear it down.
 *                          The caller should enumerate the sessions with
 *                          `wifi_getTWTsessions()` rather than assuming the session is
 *                          gone.
 *
 * @note This tears down individual and broadcast sessions alike; the identifier determines
 *       which.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getTWTsessions
 * @see wifi_setBroadcastTWTSchedule
 */
INT wifi_setTeardownTWTSession(INT ap_index, INT sessionID);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

/**
 * @brief Structure for storing multi-PSK keys.
 *
 * This structure is a container for multi-PSK keys.
 * It is based on the `hostapd wpa_psk_file` implementation:
 * https://w1.fi/cgit/hostap/tree/hostapd/hostapd.wpa_psk
 */
typedef struct _wifi_key_multi_psk
{
    CHAR wifi_keyId[64]; /**< Key identifier (e.g., "key-15" or "key_example"). */
    CHAR wifi_psk[64];   /**< PSK (ASCII passphrase of 8 to 63 characters). */
} wifi_key_multi_psk_t;
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Sets the new set of multi-PSK keys for an Access Point.
 *
 * This function sets a new set of multi-PSK keys for the specified Access Point (AP).
 * Previously set keys are removed.
 * This API is for setting all keys except the primary key, which is set by
 * `wifi_setApSecurityKeyPassphrase`.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] keys  Caller-owned array of `keysNumber` `wifi_key_multi_psk_t` elements.
 *                  Each element's `wifi_keyId` and `wifi_psk` are NUL-terminated and at
 *                  most 63 characters plus the terminator, and a passphrase is 8 to 63
 *                  characters. The `HAL` reads the array during the call and must not
 *                  retain the pointer afterwards, so the caller may free it as soon as
 *                  the call returns.
 * @param[in] keysNumber  Number of elements in `keys`. Pass 0 with any pointer value to
 *                        remove every non-primary key, since the previous set is replaced
 *                        wholesale.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success exactly the supplied keys are active as non-primary keys and every
 *       previously set non-primary key is gone, while the primary key is untouched. On
 *       failure no part of the configuration is changed.
 *
 * @returns The status of the operation
 * @retval WIFI_HAL_SUCCESS The key set was replaced.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, `keys` is NULL while `keysNumber`
 *                          is non-zero, a passphrase is outside 8 to 63 characters, or
 *                          the vendor layer rejected the set. The caller should read the
 *                          keys back with `wifi_getMultiPskKeys()`, because this
 *                          interface does not state that the replacement is atomic.
 *
 * @note The primary key is not reachable through this call. The description above names
 *       `wifi_setApSecurityKeyPassphrase()`, which is declared in
 *       `wifi_hal_deprecated.h` rather than in the current interface, so it is reachable
 *       only through the umbrella `wifi_hal.h` include and is outside the documented
 *       contract. On the current surface the primary key is set with
 *       `wifi_setApSecurity()`, through the `u.key` arm of `wifi_vap_security_t`.
 * @note The structure follows the hostapd `wpa_psk_file` model, as its own documentation
 *       states.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning A `wifi_key_multi_psk_t` holds a passphrase. A caller that logs or traces these
 *          structures must redact `wifi_psk`, and should clear its own copy once it is no
 *          longer needed.
 * @warning This replaces the whole non-primary key set rather than adding to it. A caller
 *          that means to add one key must pass every key it wants to keep.
 * @see wifi_getMultiPskKeys
 * @see wifi_getMultiPskClientKey
 */
INT wifi_pushMultiPskKeys(INT apIndex, wifi_key_multi_psk_t *keys, INT keysNumber);


/**
 * @brief Gets all multi-PSK keys for an Access Point.
 *
 * This function retrieves all multi-PSK keys that are active on the specified
 * Access Point (AP), except for the primary key.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] keys  Caller-allocated array of at least `keysNumber`
 *                   `wifi_key_multi_psk_t` elements that receives the active non-primary
 *                   keys. The caller allocates and releases it; the `HAL` writes into it
 *                   and retains no reference to it after returning.
 * @param[in] keysNumber  Number of elements the array can hold.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the active non-primary keys, up to `keysNumber` of
 *       them; on failure the output is left unspecified, so a caller must not read it
 *       unless the call succeeded.
 *
 * @returns The status of the operation
 * @retval WIFI_HAL_SUCCESS The keys were retrieved.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the array is too small for the active key set. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note This call reports no count, so a caller cannot tell from the interface how many
 *       elements were written or whether the array was too small. It must size the array
 *       from the set it last pushed, or treat an unwritten element as absent.
 * @note The primary key is never returned here.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning A `wifi_key_multi_psk_t` holds a passphrase. A caller that logs or traces these
 *          structures must redact `wifi_psk`, and should clear its own copy once it is no
 *          longer needed.
 * @see wifi_pushMultiPskKeys
 */
INT wifi_getMultiPskKeys(INT apIndex, wifi_key_multi_psk_t *keys, INT keysNumber);


/**
 * @brief Gets the multi-PSK key for a specific client MAC address.
 *
 * This function retrieves the multi-PSK key used by the client with the specified
 * MAC address to authenticate with the specified Access Point (AP).
 * If `wifi_key_multi_psk_t.wifi_keyID` is null, it means that a multi-PSK
 * key was not used for authentication.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] mac  MAC address of the client, passed by value as a `mac_address_t`, so no
 *                 caller storage is retained.
 * @param[out] key  Caller-allocated `wifi_key_multi_psk_t` that receives the multi-PSK
 *                  key that client authenticated with. The caller allocates and releases
 *                  it; the `HAL` writes into it and retains no reference to it after
 *                  returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the key the client used, or a `wifi_keyId` of NULL
 *       where the client did not authenticate with a multi-PSK key; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation
 * @retval WIFI_HAL_SUCCESS The lookup completed. A NULL `wifi_keyId` is a successful
 *                          result meaning the client did not use a multi-PSK key, not a
 *                          failure.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, a required output pointer is NULL,
 *                          or the client is not associated. The caller should validate
 *                          its arguments; a failure that persists across retries should
 *                          be logged and the value treated as unavailable.
 *
 * @note Test `key->wifi_keyId` before reading `key->wifi_psk`: a NULL identifier means no
 *       multi-PSK key was used and the passphrase field carries nothing meaningful.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning A `wifi_key_multi_psk_t` holds a passphrase. A caller that logs or traces these
 *          structures must redact `wifi_psk`, and should clear its own copy once it is no
 *          longer needed.
 * @see wifi_pushMultiPskKeys
 */
INT wifi_getMultiPskClientKey(INT apIndex, mac_address_t mac, wifi_key_multi_psk_t *key);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief Wi-Fi configuration structure.
 */
typedef struct
{
    wifi_GASConfiguration_t gas_config; /**< GAS configuration. */
} __attribute__((packed)) wifi_config_t;

/**
 * @brief Virtual Access Point (VAP) index type.
 */
typedef UINT wifi_vap_index_t;

/**
 * @brief Wi-Fi onboarding methods.
 */
typedef enum
{
    WIFI_ONBOARDINGMETHODS_USBFLASHDRIVE = 0x0001,    /**< USB flash drive. */
    WIFI_ONBOARDINGMETHODS_ETHERNET = 0x0002,        /**< Ethernet. */
    WIFI_ONBOARDINGMETHODS_LABEL = 0x0004,          /**< Label. */
    WIFI_ONBOARDINGMETHODS_DISPLAY = 0x0008,         /**< Display. */
    WIFI_ONBOARDINGMETHODS_EXTERNALNFCTOKEN = 0x0010, /**< External NFC token. */
    WIFI_ONBOARDINGMETHODS_INTEGRATEDNFCTOKEN = 0x0020, /**< Integrated NFC token. */
    WIFI_ONBOARDINGMETHODS_NFCINTERFACE = 0x0040,    /**< NFC interface. */
    WIFI_ONBOARDINGMETHODS_PUSHBUTTON = 0x0080,      /**< Push button. */
    WIFI_ONBOARDINGMETHODS_PIN = 0x0100,            /**< PIN. */
    WIFI_ONBOARDINGMETHODS_PHYSICALPUSHBUTTON = 0x0200, /**< Physical push button. */
    WIFI_ONBOARDINGMETHODS_PHYSICALDISPLAY = 0x0400,  /**< Physical display. */
    WIFI_ONBOARDINGMETHODS_VIRTUALPUSHBUTTON = 0x0800, /**< Virtual push button. */
    WIFI_ONBOARDINGMETHODS_VIRTUALDISPLAY = 0x1000,   /**< Virtual display. */
    WIFI_ONBOARDINGMETHODS_EASYCONNECT = 0x2000,     /**< Easy Connect. */
} wifi_onboarding_methods_t;

/**
 * @brief Maximum length of a WPS PIN.
 */
#define WIFI_AP_MAX_WPSPIN_LEN 9

/**
 * @brief Wi-Fi Protected Setup (WPS) settings.
 */
typedef struct
{
    BOOL enable;                    /**< Whether WPS is enabled. */
    wifi_onboarding_methods_t methods; /**< Onboarding methods supported by WPS. */
    CHAR pin[WIFI_AP_MAX_WPSPIN_LEN]; /**< WPS PIN. */
} __attribute__((packed)) wifi_wps_t;

/**
 * @brief Management Frame Protection (MFP) configuration options.
 */
typedef enum
{
    wifi_mfp_cfg_disabled, /**< MFP disabled. */
    wifi_mfp_cfg_optional, /**< MFP optional. */
    wifi_mfp_cfg_required, /**< MFP required. */
} wifi_mfp_cfg_t;

/**
 * @brief MAC filter modes.
 */
typedef enum
{
    wifi_mac_filter_mode_black_list, /**< Blacklist mode. */
    wifi_mac_filter_mode_white_list, /**< Whitelist mode. */
} wifi_mac_filter_mode_t;

/**
 * @brief RADIUS settings.
 */
typedef struct
{
#ifdef WIFI_HAL_VERSION_3_PHASE2
    ip_addr_t ip;              /**< Primary RADIUS server IP address. */
#else
    unsigned char ip[45];      /**< Primary RADIUS server IP address. */
#endif
    unsigned short port;        /**< Primary RADIUS server port. */
    char key[64];             /**< Primary RADIUS server secret. */
    char identity[64];        /**< Primary RADIUS server identity. */
#ifdef WIFI_HAL_VERSION_3_PHASE2
    ip_addr_t s_ip;              /**< Secondary RADIUS server IP address. */
    ip_addr_t connectedendpoint; /**< The RADIUS server IP address which is currently in use. */
#else
    unsigned char s_ip[45];     /**< Secondary RADIUS server IP address. */
    unsigned char connectedendpoint[45]; /**< The RADIUS server IP address which is currently in use. */
#endif
    unsigned short s_port;      /**< Secondary RADIUS server port. */
    char s_key[64];           /**< Secondary RADIUS server secret. */
    ip_addr_t dasip;           /**< RADIUS DAS server IP address. */
    USHORT dasport;            /**< RADIUS DAS server port. */
    char daskey[64];          /**< RADIUS DAS server secret. */
    UINT max_auth_attempts;    /**< Maximum number of authentication attempts. */
    UINT blacklist_table_timeout; /**< Blacklist table timeout in seconds. */
    UINT identity_req_retry_interval; /**< Identity request retry interval in seconds. */
    UINT server_retries;        /**< Number of RADIUS server retries. */
    wifi_eap_t eap_type;       /**< EAP type. */
    phase2_type phase2;        /**< Enterprise based eap type */
} __attribute__((packed)) wifi_radius_settings_t;

/**
 * @brief Security key types.
 */
typedef enum
{
    wifi_security_key_type_psk,        /**< Pre-shared key. */
    wifi_security_key_type_pass,       /**< Passphrase. */
    wifi_security_key_type_sae,        /**< Simultaneous Authentication of Equals. */
    wifi_security_key_type_psk_sae,     /**< Pre-shared key with SAE. */
    wifi_security_key_type_saeext,     /**< Extended SAE. */
    wifi_security_key_type_sae_saeext,  /**< SAE with extended SAE. */
    wifi_security_key_type_psk_sae_saeext /**< Pre-shared key with SAE and extended SAE. */
} wifi_security_key_type_t;

/**
 * @brief Security key structure.
 */
typedef struct
{
    wifi_security_key_type_t type; /**< Type of the security key. */
    char key[256];              /**< Security key data. */
} __attribute__((packed)) wifi_security_key_t;

/**
 * @brief VAP security settings.
 */
typedef struct
{
    wifi_security_modes_t mode;     /**< Security mode. */
    wifi_security_modes_t repurposed_mode;     /**< Security mode. */
    wifi_encryption_method_t encr;   /**< Encryption method. */
#if defined(WIFI_HAL_VERSION_3)
    wifi_mfp_cfg_t mfp;            /**< MFP configuration. */
#else
    char mfpConfig[32];           /**< MFP configuration. */
#endif
    BOOL wpa3_transition_disable; /**< Whether WPA3 transition is disabled. */
    UINT rekey_interval;         /**< Rekey interval in seconds. */
    BOOL strict_rekey;           /**< Whether strict rekey is enabled. */
    UINT eapol_key_timeout;      /**< EAPOL key timeout in seconds. */
    UINT eapol_key_retries;      /**< Number of EAPOL key retries. */
    UINT eap_identity_req_timeout; /**< EAP identity request timeout in seconds. */
    UINT eap_identity_req_retries; /**< Number of EAP identity request retries. */
    UINT eap_req_timeout;        /**< EAP request timeout in seconds. */
    UINT eap_req_retries;        /**< Number of EAP request retries. */
    BOOL disable_pmksa_caching;   /**< Whether PMKSA caching is disabled. */
    char key_id[32];            /**< OpenFlow tag associated with a PSK. */
    wifi_radius_settings_t repurposed_radius;   /**< To Store RADIUS configs when in Non IEEE802_1x mode */
    union
    {
        wifi_radius_settings_t radius; /**< RADIUS settings. */
        wifi_security_key_t key;      /**< Security key. */
    } u;
} __attribute__((packed)) wifi_vap_security_t;

/**
 * @brief ANQP settings.
 */
typedef struct
{
    int capabilityInfoLength;       /**< Length of the capability information. */
    wifi_capabilityListANQP_t capabilityInfo; /**< Capability information. */
    int venueInfoLength;           /**< Length of the venue information. */
    wifi_venueNameElement_t venueInfo; /**< Venue information. */
    int roamInfoLength;            /**< Length of the roaming consortium information. */
    wifi_roamingConsortium_t roamInfo; /**< Roaming consortium information. */
    wifi_ipAddressAvailabality_t ipAddressInfo; /**< IP address availability information. */
    int realmInfoLength;           /**< Length of the NAI realm information. */
    wifi_naiRealmElement_t realmInfo; /**< NAI realm information. */
    int gppInfoLength;             /**< Length of the 3GPP cellular network information. */
    wifi_3gppCellularNetwork_t gppInfo; /**< 3GPP cellular network information. */
    int domainInfoLength;          /**< Length of the domain name information. */
    wifi_domainName_t domainNameInfo; /**< Domain name information. */
    UCHAR passpointStats[1024];    /**< Passpoint statistics. */
    UINT domainRespCount;         /**< Number of successful domain name responses. */
    UINT realmRespCount;          /**< Number of successful NAI realm responses. */
    UINT gppRespCount;            /**< Number of successful 3GPP cellular network responses. */
    UINT domainFailedCount;        /**< Number of failed domain name responses. */
    UINT realmFailedCount;         /**< Number of failed NAI realm responses. */
    UINT gppFailedCount;           /**< Number of failed 3GPP cellular network responses. */
    UCHAR anqpParameters[4096];     /**< ANQP parameters. */
} __attribute__((packed)) wifi_anqp_settings_t;

/**
 * @brief Passpoint settings.
 */
typedef struct
{
    BOOL enable;                 /**< Whether Passpoint is enabled. */
    BOOL gafDisable;              /**< Whether DGAF is disabled. */
    BOOL p2pDisable;              /**< Whether P2P cross connect is disabled. */
    BOOL l2tif;                  /**< Whether Layer 2 Traffic Inspection and Filtering is enabled. */
    BOOL bssLoad;                 /**< Whether BSS Load IE is enabled. */
    BOOL countryIE;               /**< Whether Country IE is enabled. */
    BOOL proxyArp;                /**< Whether proxy ARP is enabled. */
/* vvvv **The following fields should not to be implemented in the HAL** vvvv */
    int capabilityInfoLength;    /**< Length of the capability information. */
    wifi_HS2_CapabilityList_t capabilityInfo; /**< Capability information. */
    int opFriendlyNameInfoLength; /**< Length of the operator friendly name information. */
    wifi_HS2_OperatorFriendlyName_t opFriendlyNameInfo; /**< Operator friendly name information. */
    int connCapabilityLength;    /**< Length of the connection capability information. */
    wifi_HS2_ConnectionCapability_t connCapabilityInfo; /**< Connection capability information. */
    int realmInfoLength;          /**< Length of the NAI home realm query information. */
    wifi_HS2_NAI_Home_Realm_Query_t realmInfo; /**< NAI home realm query information. */
    wifi_HS2_WANMetrics_t wanMetricsInfo; /**< WAN metrics information. */
    UCHAR hs2Parameters[4096];     /**< HS2 parameters. */
/* ^^^^ Not implemented in the hal ^^^^ */
} __attribute__((packed)) wifi_passpoint_settings_t;

/**
 * @brief Interworking settings.
 */
typedef struct
{
    wifi_InterworkingElement_t interworking; /**< Interworking element. */
    wifi_roamingConsortiumElement_t roamingConsortium; /**< Roaming consortium element. */
    wifi_anqp_settings_t anqp;        /**< ANQP settings. >>*should not to be implemented in the HAL**<< */
    wifi_passpoint_settings_t passpoint; /**< Passpoint settings. */
} __attribute__((packed)) wifi_interworking_t;

/**
 * @brief Pre-association control settings.
 */
typedef struct
{
    char rssi_up_threshold[32];           /**< RSSI up threshold. */
    char snr_threshold[32];              /**< SNR threshold. */
    char cu_threshold[32];               /**< CU threshold. */
    char basic_data_transmit_rates[32];  /**< Basic data transmit rates. */
    char operational_data_transmit_rates[32]; /**< Operational data transmit rates. */
    char supported_data_transmit_rates[32]; /**< Supported data transmit rates. */
    char minimum_advertised_mcs[32];     /**< Minimum advertised MCS. */
    char sixGOpInfoMinRate[32];          /**< 6G operating information minimum rate. */
    char client_deny_assoc_info[45];     /**< Client deny association information. */
    int  time_ms;                        /**< Time to wait for meeting minimum mgmt frames for TCM threshold calcultion */
    int  min_num_mgmt_frames;            /**< Minimum number of mgmt frames required to compute the TCM threshold. */
    char tcm_exp_weightage[32];          /**< Alpha/Exponential weight used in the Exponential Moving Average formula. */
    char tcm_gradient_threshold[32];     /**< Threshold against which TCM Exponential Moving Average is computed. */
    char tcm_client_deny_assoc_info[64]; /**< Transient Client Management deny/assoc information. */
    wifi_vap_name_t vap_name;            /**< VAP name. */
} __attribute__((packed)) wifi_preassoc_control_t;

/**
 * @brief Post-association control settings.
 */
typedef struct
{
    char sampling_interval[32];          /**< Sampling interval. */
    char sampling_count[32];             /**< Sampling count. */
    char rssi_up_threshold[32];           /**< RSSI up threshold. */
    char snr_threshold[32];              /**< SNR threshold. */
    char cu_threshold[32];               /**< CU threshold. */
    char client_force_disassoc_info[45]; /**< Client force disassociation information. */
    wifi_vap_name_t vap_name;            /**< VAP name. */
} __attribute__((packed)) wifi_postassoc_control_t;

/**
 * @brief Per-network parameters conveyed to the RADIUS server as vendor attributes.
 *
 * Used by the amenities configuration of a fronthaul BSS to describe the service
 * level the network offers.
 */
typedef struct { 
    int speed_tier;                      /**< Speed Tier for Radius AVP */
} __attribute__((packed)) network_param_config_t;

/**
 * @brief Amenities network configuration carried on a fronthaul BSS.
 *
 * A container so the amenities configuration can grow without changing the layout of
 * `wifi_front_haul_bss_t`.
 */
typedef struct {
    network_param_config_t npc;          /**< Amenities Network Param Configurations*/
} __attribute__((packed)) amenities_network_config_t;

/**
 * @brief VAP modes.
 */
typedef enum
{
    wifi_vap_mode_ap,      /**< Access Point mode. */
    wifi_vap_mode_sta,     /**< Station mode. */
    wifi_vap_mode_monitor, /**< Monitor mode. */
} wifi_vap_mode_t;

/**
 * @brief Scan parameters.
 */
typedef struct
{
    unsigned int period;   /**< Scan period in seconds. */
    wifi_channel_t channel; /**< Channel to scan (0 means scan all channels in the band). */
} __attribute__((packed)) wifi_scan_params_t;

/**
 * @brief Common MLD information.
 */
typedef struct
{
    BOOL mld_enable;      /**< Whether MLD snooping is enabled. */
    UINT mld_id;          /**< MLD group ID. */
    UINT mld_link_id;     /**< Link ID */
    mac_address_t mld_addr; /**< MLD group MAC address. */
} __attribute__((packed)) wifi_mld_common_info_t;

/**
 * @brief MLD information for a station.
 */
typedef struct
{
    mac_address_t local_addr; /**< Local MAC address. */
    mac_address_t peer_addr;  /**< Peer MAC address. */
    wifi_mld_common_info_t common_info; /**< Common MLD information. */
} __attribute__((packed)) wifi_mld_info_sta_t;

/**
 * @brief MLD information for an Access Point.
 */
typedef struct
{
    wifi_mld_common_info_t common_info; /**< Common MLD information. */
} __attribute__((packed)) wifi_mld_info_ap_t;

/**
 * @brief Backhaul station information.
 */
typedef struct
{
    ssid_t ssid;             /**< SSID. */
    ssid_t repurposed_ssid;   /**< SSID for ignite */
    bssid_t bssid;            /**< BSSID (if all 0, scan the SSID with probes, otherwise connect to the specified BSSID). */
    BOOL enabled;            /**< Whether the backhaul station is enabled. */
    wifi_connection_status_t conn_status; /**< Connection status. */
    wifi_scan_params_t scan_params; /**< Scan parameters. */
    wifi_vap_security_t security; /**< Security settings. */
    mac_address_t mac;        /**< MAC address. */
    wifi_mld_info_sta_t mld_info; /**< MLD information. */
    BOOL ignite_enabled; /**< Whether Ignite is enabled on this backhaul station. */
    BOOL valid_bh_credentials; /**< TRUE if backhaul credentials (SSID and key) are valid. */
} __attribute__((packed)) wifi_back_haul_sta_t;

/**
 * @brief Maximum length of an SSID.
 */
#define WIFI_AP_MAX_SSID_LEN 33

/**
 * @brief Maximum length of the vendor information elements buffer
 *
 * Not a standard value, but a reasonable maximum for vendor elements 
 * Computed by taking Max MPDU size - ~ MAX 802.11 header size - 802.11 FCS size - ~Size of required IEs
 * 2,310 is divisible by the typical Vendor IE size (7 = IE Type[1] + IE Length[1] + OUI[3] + VIE Type[1] + VIE Subtype [1])
 */
#define WIFI_AP_MAX_VENDOR_IE_LEN 2310

/**
 * @brief Fronthaul BSS information.
 *
 * This structure contains information about a fronthaul BSS.
 */
typedef struct {
  CHAR ssid[WIFI_AP_MAX_SSID_LEN]; /**< SSID. */
  BOOL enabled;                  /**< Whether the BSS is enabled. */
  BOOL showSsid;                 /**< Whether to show the SSID. */
  BOOL isolation;                /**< Whether client isolation is enabled. */
  INT mgmtPowerControl;          /**< Management frame power control. */
  UINT bssMaxSta;               /**< Maximum number of stations allowed. */
  BOOL bssTransitionActivated;    /**< Whether BSS transition is activated. */
  BOOL nbrReportActivated;       /**< Whether neighbor report is activated. */

  BOOL rapidReconnectEnable; /**< Whether rapid reconnect is enabled. This field should not be implemented in the HAL. */
  UINT rapidReconnThreshold; /**< Rapid reconnect threshold. This field should not be implemented in the HAL. */
  BOOL vapStatsEnable;      /**< Whether VAP statistics are enabled. This field should not be implemented in the HAL. */

  wifi_vap_security_t security;    /**< Security settings. */
  wifi_interworking_t interworking; /**< Interworking settings. */
  wifi_preassoc_control_t preassoc;  /**< Pre-association control settings. */
  wifi_postassoc_control_t postassoc; /**< Post-association control settings. */
  BOOL mac_filter_enable;           /**< Whether MAC filtering is enabled. */
  amenities_network_config_t am_config;  /**< Connected Building Phase Two */
  wifi_mac_filter_mode_t mac_filter_mode; /**< MAC filter mode. */

  BOOL sec_changed; /**< Whether security settings have changed. This field should not be implemented in the HAL. */

  wifi_wps_t wps;        /**< WPS settings. */
  BOOL wmm_enabled;     /**< Whether WMM is enabled. */
  BOOL UAPSDEnabled;    /**< Whether U-APSD is enabled. */
  wifi_bitrate_t beaconRate; /**< Beacon rate. */
  mac_address_t bssid;    /**< The BSSID. This variable should only be used in the get API. It can't be used to change the interface MAC. */
  UINT wmmNoAck;         /**< WMM no-acknowledgement settings. */
  UINT wepKeyLength;     /**< WEP key length. */
  BOOL bssHotspot;       /**< Whether the BSS is a hotspot. */
  UINT wpsPushButton;    /**< WPS push button configuration. */
  char beaconRateCtl[32]; /**< Beacon rate control string. */
  BOOL network_initiated_greylist; /**< Whether network-initiated greylisting is enabled. */
  BOOL mcast2ucast;              /**< Whether multicast-to-unicast conversion is enabled. */
  BOOL connected_building_enabled; /**< Whether connected building is enabled. */
  BOOL mdu_enabled;   /**< Whether Managed Wifi Phase 2 is enabled. */
  wifi_mld_info_ap_t mld_info;        /**< MLD information. */
  BOOL hostap_mgt_frame_ctrl;        /**< Whether hostapd management frame control is enabled. */
  BOOL mbo_enabled;                  /**< Whether MBO is enabled. */
  BOOL   interop_ctrl;               /**< Whether interop ctrl is enabled. */
  BOOL   interop_tel;                /**< Whether interop telemetry reporting is enabled. */
  UINT    inum_sta;                   /**< configuring interop stations */
  UCHAR vendor_elements[WIFI_AP_MAX_VENDOR_IE_LEN]; /**< The vendor elements to be added to beacon/probe response frames. Includes IE ID (0xDD), Length, and Payload */
  USHORT vendor_elements_len;        /**< Length of vendor_elements currently stored since it is not null terminated */
  char interop_info[64];             /**< NUL-terminated vendor interop descriptor, at most 64 bytes including the terminator. */
  CHAR multi_ap_backhaul_ssid[WIFI_AP_MAX_SSID_LEN]; /**< Multi-AP backhaul SSID. Populated with the mesh backhaul SSID when WPS onboarding is configured. */
  UCHAR multi_ap_backhaul_network_key[256]; /**< Multi-AP backhaul network key, populated with the mesh backhaul key when WPS onboarding is configured. */
} __attribute__((packed)) wifi_front_haul_bss_t;

/**
 * @brief Maximum length of a bridge name.
 */
#define WIFI_BRIDGE_NAME_LEN 32

/**
 * @brief VAP information structure.
 */
typedef struct
{
    wifi_vap_index_t vap_index;     /**< VAP index. */
    wifi_vap_name_t vap_name;      /**< VAP name. */
    wifi_radio_index_t radio_index; /**< Radio index. */
    CHAR bridge_name[WIFI_BRIDGE_NAME_LEN]; /**< Bridge name. */
    wifi_vap_mode_t vap_mode;      /**< VAP mode. */
    wifi_vap_name_t repurposed_vap_name; /**< Repurposed VAP name. */
    CHAR repurposed_bridge_name[WIFI_BRIDGE_NAME_LEN]; /**< Repurposed Bridge Name. */
    union
    {
        wifi_front_haul_bss_t bss_info; /**< Fronthaul BSS information. */
        wifi_back_haul_sta_t sta_info; /**< Backhaul station information. */
    } u;
} __attribute__((packed)) wifi_vap_info_t;

/**
 * @brief VAP information map structure.
 */
typedef struct
{
    unsigned int num_vaps;       /**< Number of VAPs. */
    wifi_vap_info_t vap_array[MAX_NUM_VAP_PER_RADIO]; /**< Array of VAP information structures. */
} __attribute__((packed)) wifi_vap_info_map_t;

/**
 * @brief Wi-Fi AP capabilities.
 */
typedef struct
{
    BOOL rtsThresholdSupported; /**< Whether RTS/CTS threshold is supported. */
    wifi_security_modes_t securityModesSupported; /**< Supported security modes. */
    wifi_onboarding_methods_t methodsSupported; /**< Supported onboarding methods. */
    BOOL WMMSupported; /**< Whether WMM is supported. */
    BOOL UAPSDSupported; /**< Whether U-APSD is supported. */
    BOOL interworkingServiceSupported; /**< Whether interworking service is supported. */
    BOOL BSSTransitionImplemented; /**< Whether BSS transition is implemented. */
} __attribute__((packed)) wifi_ap_capabilities_t;
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Reads the SSID out of a caller-held VAP information structure.
 *
 * This is an accessor over a `wifi_vap_info_t` the caller already holds rather than a
 * query against the driver. Both arms of the structure's union carry an SSID,
 * `u.bss_info.ssid` for a fronthaul BSS and `u.sta_info.ssid` for a backhaul station,
 * and `vap_mode` records which arm is in use. This interface does not state which arm
 * the accessor reads, or whether it consults `vap_mode` to decide, so a caller that
 * needs a specific arm should read that member directly.
 *
 * @param[in] vap  VAP information structure to read. Must be non-NULL and fully
 *                 populated, typically by `wifi_getRadioVapInfoMap()`. The parameter
 *                 is not declared `const`, so this interface does not guarantee the
 *                 structure is left unmodified; a caller that needs that guarantee
 *                 must not rely on it.
 *
 * @pre `vap` points to an initialised `wifi_vap_info_t`. The effect of passing NULL or
 *      a partially populated structure is not specified by this interface.
 * @post The structure's SSID is unchanged by the call.
 *
 * @return A pointer to the NUL-terminated SSID, or NULL if no SSID can be selected.
 *         The two union arms declare different widths: `u.bss_info.ssid` is
 *         `WIFI_AP_MAX_SSID_LEN` bytes, which is 33, while `u.sta_info.ssid` is an
 *         `ssid_t`, which is 32. A caller copying the result should therefore size its
 *         own buffer for the larger of the two. Because the type is `const char *` the
 *         caller must not modify the characters and must not free them.
 *
 * @warning This interface does not specify the lifetime of the returned pointer or whether
 *          it aliases storage inside `vap`. A caller must therefore neither free it nor
 *          assume it stays valid after `vap` is modified or goes out of scope; copy the
 *          string if it is needed beyond the immediate use.
 *
 * @note This accessor reads memory the caller already owns, so no allocation crosses the
 *       interface; see `Memory Model` in `docs/pages/halSpec.md`.
 * @note The call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. That covers the `HAL`'s own state; serialising access to a
 *       `wifi_vap_info_t` the caller shares between threads remains the caller's business.
 * @note Unlike the other declarations in this header this name carries no `wifi_` prefix,
 *       which is a naming inconsistency in the interface rather than a hint that it behaves
 *       differently.
 *
 * @see wifi_getRadioVapInfoMap
 * @see wifi_vap_info_t
 */
const char *get_vap_ssid(wifi_vap_info_t *vap);

/**
 * @brief Reads the Linux bridge name out of a caller-held VAP information structure.
 *
 * This is an accessor over a `wifi_vap_info_t` the caller already holds. The bridge name
 * identifies the bridge the VAP's network interface is attached to, which a caller needs
 * when it configures forwarding or filtering around the VAP. The structure also carries
 * `repurposed_bridge_name`; this interface does not state whether the accessor ever
 * returns that member instead, so a caller that must distinguish the two should read
 * them directly.
 *
 * @param[in] vap  VAP information structure to read. Must be non-NULL and fully
 *                 populated, typically by `wifi_getRadioVapInfoMap()`. The parameter
 *                 is not declared `const`, so this interface does not guarantee the
 *                 structure is left unmodified; a caller that needs that guarantee
 *                 must not rely on it.
 *
 * @pre `vap` points to an initialised `wifi_vap_info_t`. The effect of passing NULL or
 *      a partially populated structure is not specified by this interface.
 * @post The structure's bridge name is unchanged by the call.
 *
 * @return A pointer to the NUL-terminated bridge name, at most
 *         `WIFI_BRIDGE_NAME_LEN` bytes including the terminator, or NULL if the VAP
 *         has no bridge name. Because the type is `const char *` the caller must not
 *         modify the characters and must not free them.
 *
 * @warning This interface does not specify the lifetime of the returned pointer or whether
 *          it aliases `vap->bridge_name`. A caller must therefore neither free it nor
 *          assume it stays valid after `vap` is modified or goes out of scope; copy the
 *          string if it is needed beyond the immediate use.
 *
 * @note This accessor reads memory the caller already owns, so no allocation crosses the
 *       interface; see `Memory Model` in `docs/pages/halSpec.md`.
 * @note The call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. Serialising access to a `wifi_vap_info_t` the caller
 *       shares between threads remains the caller's business.
 * @note Unlike the other declarations in this header this name carries no `wifi_` prefix,
 *       which is a naming inconsistency in the interface rather than a hint that it behaves
 *       differently.
 *
 * @see wifi_getRadioVapInfoMap
 * @see wifi_vap_info_t
 */
const char *get_vap_bridge_name(wifi_vap_info_t *vap);

/**
 * @brief Reads the security configuration out of a caller-held VAP information
 *        structure.
 *
 * This is an accessor over a `wifi_vap_info_t` the caller already holds, reporting the
 * security configuration through `sec`. Both arms of the structure's union carry a
 * `wifi_vap_security_t`, `u.bss_info.security` for a fronthaul BSS and
 * `u.sta_info.security` for a backhaul station, and `vap_mode` records which arm is in
 * use. This interface does not state which arm the accessor reads, so a caller that
 * needs a specific arm should read that member directly.
 *
 * @param[in]  vap  VAP information structure to read. Must be non-NULL and fully
 *                  populated, typically by `wifi_getRadioVapInfoMap()`. The parameter
 *                  is not declared `const`, so this interface does not guarantee the
 *                  structure is left unmodified.
 * @param[out] sec  Caller-allocated `wifi_vap_security_t` that receives the selected
 *                  security configuration. The caller allocates and releases it; the
 *                  callee writes into it and retains no reference to it after
 *                  returning. Its `u` union holds either RADIUS settings or a
 *                  pre-shared key, selected by `mode`.
 *
 * @pre `vap` points to an initialised `wifi_vap_info_t` and `sec` points to storage of
 *      at least `sizeof(wifi_vap_security_t)`. The effect of passing NULL for either
 *      is not specified by this interface.
 * @post On a successful call `*sec` holds the VAP's security configuration. Whether
 *       `*sec` is left untouched on failure is not specified by this interface, so a
 *       caller must not read it unless the call is known to have succeeded.
 *
 * @return An `unsigned int`. This interface does not specify whether that value is a
 *         `HAL` status code or the `wifi_security_modes_t` bit for the selected mode,
 *         and the two are not distinguishable by inspection: `WIFI_HAL_SUCCESS` is 0
 *         while every `wifi_security_modes_t` member is a non-zero power of two, so a
 *         return of 0 is ambiguous between success and no-mode. A caller must not
 *         assume either reading; take the security mode from `sec->mode`, which is
 *         unambiguous, and treat the return value as opaque until the interface
 *         defines it. No per-code list is given here for that reason: this function
 *         returns a value rather than one of the `HAL` status codes.
 *
 * @note This accessor reads memory the caller already owns and writes into a buffer the
 *       caller supplied, so no allocation crosses the interface; see `Memory Model` in
 *       `docs/pages/halSpec.md`.
 * @note The call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. Serialising access to a `wifi_vap_info_t` the caller
 *       shares between threads remains the caller's business.
 * @note Unlike the other declarations in this header this name carries no `wifi_` prefix,
 *       which is a naming inconsistency in the interface rather than a hint that it behaves
 *       differently.
 *
 * @see wifi_getApSecurity
 * @see wifi_vap_security_t
 * @see wifi_security_modes_t
 */
unsigned int get_vap_security_mode(wifi_vap_info_t *vap, wifi_vap_security_t *sec);

/**
 * @brief Creates the Virtual Access Points described by a map on one radio.
 *
 * This function creates every Virtual Access Point (`VAP`) the supplied map describes on
 * the given radio, each one carrying the mode, `SSID`, security and bridge configuration
 * its `wifi_vap_info_t` entry holds. It is the call that brings an Access Point index
 * into existence, and `wifi_deleteAp()` is what removes it again.
 *
 * @param[in] index  Index of the radio to create the Virtual Access Points on, in the
 *                   range `RADIO_INDEX_1` to `RADIO_INDEX_3`, bounded by
 *                   `MAX_NUM_RADIOS`.
 * @param[in] map  Caller-owned `wifi_vap_info_map_t` describing the Virtual Access Points
 *                 to create. `num_vaps` gives how many entries of `vap_array` are
 *                 populated, and must not exceed `MAX_NUM_VAP_PER_RADIO`, which is 8. The
 *                 `HAL` reads the structure during the call and must not retain the
 *                 pointer afterwards.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the described Virtual Access Points exist on that radio; each one's
 *       arrival at an operational state is reported separately through the registered VAP
 *       status handler. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The Virtual Access Points were created.
 * @retval WIFI_HAL_ERROR   `index` is out of range, `map` is NULL, `num_vaps` exceeds
 *                          `MAX_NUM_VAP_PER_RADIO`, or a Virtual Access Point description
 *                          was rejected. The caller should read the radio back with
 *                          `wifi_getRadioVapInfoMap()` to establish which Virtual Access
 *                          Points exist, because this interface does not state that
 *                          creation is atomic across the map.
 *
 * @note Success means the Virtual Access Points were created, not that they are up.
 *       Register with `wifi_vapstatus_callback_register()` beforehand, or poll
 *       `wifi_getApStatus()`, to learn when each becomes operational.
 * @note Each `wifi_vap_info_t` selects its union arm through `vap_mode`: `bss_info` for an
 *       Access Point and `sta_info` for a backhaul station. Populating the wrong arm for
 *       the mode is not detected by this interface.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getRadioVapInfoMap
 * @see wifi_vapstatus_callback_register
 * @see wifi_deleteAp
 */
INT wifi_createVAP(wifi_radio_index_t index, wifi_vap_info_map_t *map);

/**
 * @brief Reads back the Virtual Access Point configuration of one radio.
 *
 * This function reports the Virtual Access Points (`VAP`) that currently exist on the
 * given radio together with the configuration each one carries, which is what a caller
 * reads to establish the outcome of a `wifi_createVAP()` call or to recover the Access
 * Point indices a radio owns.
 *
 * @param[in] index  Index of the radio to read, in the range `RADIO_INDEX_1` to
 *                   `RADIO_INDEX_3`, bounded by `MAX_NUM_RADIOS`.
 * @param[out] map  Caller-allocated `wifi_vap_info_map_t` that receives the radio's
 *                  Virtual Access Points, with `num_vaps` giving how many entries of
 *                  `vap_array` were populated. The caller allocates and releases it; the
 *                  `HAL` writes into it and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds `num_vaps` populated entries, with the remaining
 *       `vap_array` elements left unspecified; on failure the output is left unspecified,
 *       so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The map was retrieved. A `num_vaps` of 0 is a success meaning
 *                          the radio hosts no Virtual Access Points.
 * @retval WIFI_HAL_ERROR   `index` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note Read `num_vaps` first and only then the corresponding entries of `vap_array`; the
 *       elements beyond it are not defined by this interface.
 * @note This is the call that populates the structure the `get_vap_ssid()`,
 *       `get_vap_bridge_name()` and `get_vap_security_mode()` accessors read.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_createVAP
 * @see get_vap_ssid
 */
INT wifi_getRadioVapInfoMap(wifi_radio_index_t index, wifi_vap_info_map_t *map);

/**
 * @brief Sets the security settings for an Access Point.
 *
 * @param[in] ap_index  Index of the Access Point, in the range `AP_INDEX_1` to
 *                      `AP_INDEX_24`.
 * @param[in] security  Caller-owned `wifi_vap_security_t` to apply. Its `u` union arm is
 *                      selected by `mode`: RADIUS settings for an enterprise mode, a key
 *                      for a personal one. The `HAL` reads the structure during the call
 *                      and must not retain the pointer afterwards.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point enforces the supplied security configuration. On
 *       failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was applied.
 * @retval WIFI_HAL_ERROR   `ap_index` is out of range, `security` is NULL, or a member
 *                          was rejected -- for instance a mode the Access Point does not
 *                          support. The caller should check the supported modes with
 *                          `wifi_getAPCapabilities()` and read the configuration back
 *                          rather than assuming the update was atomic.
 *
 * @note Populate the union arm that matches `mode`: filling the RADIUS arm for a personal
 *       mode, or the key arm for an enterprise mode, is not detected by this interface.
 * @note `wifi_getAPCapabilities()` reports the modes this Access Point supports through
 *       `securityModesSupported`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The structure carries credentials in its `u` union. A caller that logs or traces
 *          it must redact them, and should clear its own copy once the call returns.
 * @warning Changing the security configuration disconnects associated clients, which must
 *          reauthenticate with the new credentials.
 * @see wifi_getApSecurity
 * @see wifi_getAPCapabilities
 */
INT wifi_setApSecurity(INT ap_index, wifi_vap_security_t *security);

/**
 * @brief Gets the security settings for an Access Point.
 *
 * @param[in] ap_index  Index of the Access Point, in the range `AP_INDEX_1` to
 *                      `AP_INDEX_24`.
 * @param[out] security  Caller-allocated `wifi_vap_security_t` that receives the Access
 *                       Point's security configuration, whose `u` union arm is selected
 *                       by `mode`. The caller allocates and releases it; the `HAL` writes
 *                       into it and retains no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the security configuration; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was retrieved.
 * @retval WIFI_HAL_ERROR   `ap_index` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note Read `mode` before the `u` union: it is what selects which arm holds meaningful
 *       data.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The returned structure carries credentials in its `u` union. A caller that logs
 *          or traces it must redact them, and should clear its own copy once it is no
 *          longer needed.
 * @see wifi_setApSecurity
 */
INT wifi_getApSecurity(INT ap_index, wifi_vap_security_t *security);

/**
 * @brief Gets the capabilities of an Access Point.
 *
 * @param[in] ap_index  Index of the Access Point, in the range `AP_INDEX_1` to
 *                      `AP_INDEX_24`.
 * @param[out] apCapabilities  Caller-allocated `wifi_ap_capabilities_t` that receives
 *                             what this Access Point supports. The caller allocates and
 *                             releases it; the `HAL` writes into it and retains no
 *                             reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the capability flags; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The capabilities were retrieved.
 * @retval WIFI_HAL_ERROR   `ap_index` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note Use this before configuring an optional feature: `rtsThresholdSupported`,
 *       `WMMSupported`, `UAPSDSupported`, `interworkingServiceSupported` and
 *       `BSSTransitionImplemented` each predict whether the corresponding setter can
 *       succeed, and `securityModesSupported` and `methodsSupported` give the accepted
 *       security modes and onboarding methods.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_ap_capabilities_t
 * @see wifi_setApSecurity
 */
INT wifi_getAPCapabilities(INT ap_index, wifi_ap_capabilities_t *apCapabilities);

/**
 * @brief Gets the WPS configuration for an Access Point.
 *
 * @param[in] ap_index  Index of the Access Point, in the range `AP_INDEX_1` to
 *                      `AP_INDEX_24`.
 * @param[out] wpsConfig  Caller-allocated `wifi_wps_t` that receives the WPS enable
 *                        state, supported onboarding methods and PIN. The caller
 *                        allocates and releases it; the `HAL` writes into it and retains
 *                        no reference to it after returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the WPS configuration; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was retrieved.
 * @retval WIFI_HAL_ERROR   `ap_index` is out of range, a required output pointer is NULL,
 *                          or the vendor layer could not supply the value. The caller
 *                          should validate its arguments; a failure that persists across
 *                          retries should be logged and the value treated as unavailable.
 *
 * @note `methods` is a bit set of `wifi_onboarding_methods_t` values, so test individual
 *       bits rather than comparing the whole field.
 * @note `pin` is `WIFI_AP_MAX_WPSPIN_LEN` bytes, which is 9.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The `pin` member is a credential. A caller that logs or traces this structure
 *          must redact it.
 * @see wifi_setApWpsConfiguration
 * @see wifi_getAPCapabilities
 */
INT wifi_getApWpsConfiguration(INT ap_index, wifi_wps_t* wpsConfig);

/**
 * @brief Sets the WPS configuration for an Access Point.
 *
 * @param[in] ap_index  Index of the Access Point, in the range `AP_INDEX_1` to
 *                      `AP_INDEX_24`.
 * @param[in] wpsConfig  Caller-owned `wifi_wps_t` to apply, whose `methods` is a bit set
 *                       of `wifi_onboarding_methods_t` values and whose `pin` is at most
 *                       `WIFI_AP_MAX_WPSPIN_LEN` bytes including the terminator. The
 *                       `HAL` reads it during the call and must not retain the pointer
 *                       afterwards.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      leaves the configuration unchanged.
 * @post On success the Access Point's WPS configuration matches the structure. On failure
 *       no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was applied.
 * @retval WIFI_HAL_ERROR   `ap_index` is out of range, `wpsConfig` is NULL, a requested
 *                          onboarding method is not supported, or the PIN is malformed.
 *                          The caller should check `methodsSupported` from
 *                          `wifi_getAPCapabilities()` and correct the structure rather
 *                          than retrying unchanged.
 *
 * @note Request only methods that `wifi_getAPCapabilities()` reports in `methodsSupported`.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The `pin` member is a credential. A caller that logs or traces this structure
 *          must redact it.
 * @see wifi_getApWpsConfiguration
 * @see wifi_getAPCapabilities
 */
INT wifi_setApWpsConfiguration(INT ap_index, wifi_wps_t* wpsConfig);

/**
 * @brief Checks whether libhostapd is used.
 *
 * @param[out] output_bool  Caller-allocated variable that receives true when this
 *                          platform's Wi-Fi `HAL` is implemented over libhostapd and
 *                          false when it is not. The caller allocates and releases it;
 *                          the `HAL` writes into it and retains no reference to it after
 *                          returning.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR` and
 *      writes nothing to the output.
 * @post On success the output holds the libhostapd indication; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The indication was retrieved.
 * @retval WIFI_HAL_ERROR   `output_bool` is NULL or the vendor layer could not report the
 *                          value. The caller should treat libhostapd use as unknown and
 *                          avoid `wifi_updateLibHostApdConfig()`.
 *
 * @note This call takes no Access Point index because the answer is a property of the
 *       platform rather than of one Access Point.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_updateLibHostApdConfig
 */
INT wifi_getLibhostapd(BOOL *output_bool);

/**
 * @brief Updates the libhostapd configuration for an Access Point.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in `docs/pages/halSpec.md`. A call made beforehand fails with `WIFI_HAL_ERROR`
 *      without performing the action.
 * @post On success the libhostapd configuration for this Access Point reflects the Access
 *       Point's current settings. On failure no part of the configuration is changed.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was updated.
 * @retval WIFI_HAL_ERROR   `apIndex` is out of range, this platform does not use
 *                          libhostapd, or the update failed. The caller should check
 *                          `wifi_getLibhostapd()` first, since on a platform that does
 *                          not use libhostapd this call has nothing to update.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @see wifi_getLibhostapd
 */
INT wifi_updateLibHostApdConfig(int apIndex);

/**
 * @brief VAP status values.
 */
typedef enum
{
    wifi_vapstatus_down, /**< VAP is down. */
    wifi_vapstatus_up   /**< VAP is up. */
} wifi_vapstatus_t;

/**
 * @brief Callback function invoked when VAP status changes.
 *
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex Index of the VAP.
 * @param[in] status  Status of the VAP.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 */
typedef INT(* wifi_vapstatus_callback)(INT apIndex, wifi_vapstatus_t status);

/**
 * @brief Installs the caller's handler for VAP up and down transitions.
 *
 * After registration the `HAL` reports each transition of a Virtual Access Point
 * between `wifi_vapstatus_down` and `wifi_vapstatus_up` through the supplied handler,
 * which is how a caller learns that a VAP created by `wifi_createVAP()` has actually
 * come up. The most recently registered handler replaces any previous one.
 *
 * @param[in] callback  Handler to install, of type `wifi_vapstatus_callback`. The
 *                      `HAL` retains this function pointer and invokes it until it is
 *                      replaced, so the function must remain callable for that whole
 *                      period. The effect of passing NULL is not specified by this
 *                      interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. A call made beforehand fails with
 *      `WIFI_HAL_ERROR` and installs nothing.
 * @post On success the handler is installed and is invoked on each subsequent VAP
 *       status change. On failure no handler is installed and any previously
 *       registered handler remains in place.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was installed.
 * @retval WIFI_HAL_ERROR   The handler could not be installed, for example because
 *                          the `HAL` is not initialised. The caller should log the
 *                          failure and fall back to polling VAP state through
 *                          `wifi_getRadioVapInfoMap()` rather than waiting for a
 *                          notification that will never arrive.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous; delivery of `wifi_vapstatus_callback`
 *       is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_vapstatus_callback
 * @see wifi_vapstatus_t
 * @see wifi_createVAP
 */
INT wifi_vapstatus_callback_register(wifi_vapstatus_callback callback);

/**
 * @brief Callback function invoked for logging catastrophic failures.
 *
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] fmt Format string for the log message.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 */
typedef INT(* wifi_analytics_callback)(CHAR *fmt, ...);

/**
 * @brief Installs the caller's log sink for catastrophic `HAL` failures.
 *
 * After registration the `HAL` routes its catastrophic-failure messages to the
 * supplied handler, which lets the caller's own logging and telemetry pipeline carry
 * them rather than losing them inside the vendor layer. The most recently registered
 * handler replaces any previous one.
 *
 * @param[in] callback  Handler to install, of type `wifi_analytics_callback`. It takes
 *                      a `printf`-style format string and a variable argument list, so
 *                      the implementation must consume the arguments with the `v`
 *                      family of formatting calls. The `HAL` retains this function
 *                      pointer and invokes it until it is replaced, so the function
 *                      must remain callable for that whole period. The effect of
 *                      passing NULL is not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. A call made beforehand fails with
 *      `WIFI_HAL_ERROR` and installs nothing.
 * @post On success the handler is installed and receives subsequent failure messages.
 *       On failure no handler is installed and any previously registered handler
 *       remains in place.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was installed.
 * @retval WIFI_HAL_ERROR   The handler could not be installed, for example because
 *                          the `HAL` is not initialised. The caller should log the
 *                          failure through its own path and continue; losing this
 *                          sink degrades diagnostics but does not affect Wi-Fi
 *                          operation.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous; invocation of
 *       `wifi_analytics_callback` is asynchronous and may occur on a `HAL` thread.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler, which
 *       is called on failure paths where blocking would compound the problem.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 * @warning The format string and its arguments are owned by the `HAL` and are valid only
 *          for the duration of the call; the handler must format or copy them before
 *          returning.
 *
 * @see wifi_analytics_callback
 */
INT wifi_hal_analytics_callback_register(wifi_analytics_callback callback);

/**
 * @brief Wi-Fi Protected Setup (WPS) event types.
 */
typedef enum
{
    wifi_wps_ev_m2d,                       /**< M2D (Registrar requires more info / deferred). */
    wifi_wps_ev_fail,                      /**< Registration failed. */
    wifi_wps_ev_success,                   /**< Registration succeeded. */
    wifi_wps_ev_pwd_auth_fail,             /**< Password authentication failed. */
    wifi_wps_ev_pbc_overlap,               /**< PBC session overlap detected. */
    wifi_wps_ev_pbc_timeout,               /**< PBC walktime expired before protocol run start. */
    wifi_wps_ev_pbc_active,                /**< PBC mode was activated. */
    wifi_wps_ev_pbc_disable,               /**< PBC mode was disabled. */
    wifi_wps_ev_pin_timeout,               /**< PIN session expired. */
    wifi_wps_ev_pin_disable,               /**< PIN session was disabled. */
    wifi_wps_ev_pin_active,                /**< PIN mode was activated. */
    wifi_wps_ev_er_ap_add,                 /**< External Registrar: AP added. */
    wifi_wps_ev_er_ap_remove,              /**< External Registrar: AP removed. */
    wifi_wps_ev_er_enrollee_add,           /**< External Registrar: Enrollee added. */
    wifi_wps_ev_er_enrollee_remove,        /**< External Registrar: Enrollee removed. */
    wifi_wps_ev_er_ap_settings,            /**< External Registrar: AP settings learned. */
    wifi_wps_ev_er_set_selected_registrar, /**< External Registrar: SetSelectedRegistrar event. */
    wifi_wps_ev_ap_pin_success,            /**< External Registrar used correct AP PIN. */
} wifi_wps_ev_t;

/**
 * @brief Callback function invoked when a WPS (Wi-Fi Protected Setup) event occurs.
 *
 * This callback is triggered by the Wi-Fi HAL to notify upper layers about
 * the result of a WPS operation on the specified access point interface.
 * The callback must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] apIndex Index of the access point interface where the WPS event occurred.
 * @param[in] event   WPS event code indicating the outcome.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS If successful.
 * @retval WIFI_HAL_ERROR   If any error is detected.
 */
typedef INT(* wifi_wpsEvent_callback)(INT apIndex, wifi_wps_ev_t event);

/**
 * @brief Installs the caller's handler for Wi-Fi Protected Setup result events.
 *
 * After registration the `HAL` reports the outcome of every `WPS` operation on any
 * Access Point interface through the supplied handler, which is the only way a caller
 * learns whether a push-button or PIN session succeeded, timed out or overlapped.
 * Only one handler may be registered at a time; registering a new one replaces the
 * previous one.
 *
 * @param[in] callback  Handler to install, of type `wifi_wpsEvent_callback`. The
 *                      `HAL` retains this function pointer and invokes it until it is
 *                      replaced, so the function must remain callable for that whole
 *                      period. The effect of passing NULL is not specified by this
 *                      interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in `docs/pages/halSpec.md`. A call made beforehand fails with
 *      `WIFI_HAL_ERROR` and installs nothing. Register before starting a `WPS`
 *      session with `wifi_setApWpsButtonPush()` or `wifi_setApWpsEnrolleePin()`,
 *      otherwise the session's outcome is not reported.
 * @post On success the handler is installed and is invoked for each subsequent `WPS`
 *       event. On failure no handler is installed and any previously registered
 *       handler remains in place.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was installed.
 * @retval WIFI_HAL_ERROR   The handler could not be installed, for example because
 *                          the `HAL` is not initialised. The caller should log the
 *                          failure and treat `WPS` outcomes as unobservable, since a
 *                          `WPS` session started without a handler completes
 *                          silently.
 *
 * @execution callback
 * @sideeffect None
 *
 * @note The registration call itself is synchronous; delivery of `wifi_wpsEvent_callback`
 *       is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in `docs/pages/halSpec.md`. The same holds for the handler.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_wpsEvent_callback
 * @see wifi_wps_ev_t
 * @see wifi_setApWpsButtonPush
 * @see wifi_cancelApWPS
 */
INT wifi_wpsEvent_callback_register(wifi_wpsEvent_callback callback);

/** @} */  //END OF GROUP WIFI_HAL_APIS

#ifdef __cplusplus
}
#endif

#endif
