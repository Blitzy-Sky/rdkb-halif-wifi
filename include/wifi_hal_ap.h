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
 *
 * Every member is a counter, and `Sent` and `Received` are stated from the side of the
 * interface the statistics were read for. This interface states neither the interval a
 * counter covers, nor whether a counter resets when the interface or the `VAP` restarts,
 * nor whether the categories overlap - a frame counted as an error may or may not also
 * appear in a discard counter. A caller therefore cannot read a rate out of a single
 * sample, and should difference successive reads rather than treating any member as an
 * absolute for a known period.
 */
typedef struct _wifi_trafficStats
{
    ULONG wifi_ErrorsSent;            /*!< Frames the interface counted as errors on the transmit
                                           path. This interface does not state what it counts as an
                                           error, nor whether such a frame is also counted in
                                           `wifi_DiscardedPacketsSent`. */
    ULONG wifi_ErrorsReceived;         /*!< Frames the interface counted as errors on the receive
                                            path. What constitutes an error, and whether the same
                                            frame is also counted in
                                            `wifi_DiscardedPacketsReceived`, is not stated here. */
    ULONG wifi_UnicastPacketsSent;    /*!< Unicast packets the interface transmitted. Multicast and
                                           broadcast traffic is counted in separate members below,
                                           so this is not a transmit total. */
    ULONG wifi_UnicastPacketsReceived; /*!< Unicast packets the interface received. Multicast and
                                            broadcast traffic is counted in separate members below,
                                            so this is not a receive total. */
    ULONG wifi_DiscardedPacketsSent;   /*!< Packets dropped on the transmit path instead of being
                                            sent. This interface does not state which conditions
                                            count as a discard. */
    ULONG wifi_DiscardedPacketsReceived; /*!< Packets dropped on the receive path instead of being
                                              delivered. This interface does not state which
                                              conditions count as a discard. */
    ULONG wifi_MulticastPacketsSent;   /*!< Multicast packets the interface transmitted. Broadcast
                                            traffic is counted separately in
                                            `wifi_BroadcastPacketsSent`. */
    ULONG wifi_MulticastPacketsReceived; /*!< Multicast packets the interface received. Broadcast
                                              traffic is counted separately in
                                              `wifi_BroadcastPacketsRecevied`. */
    ULONG wifi_BroadcastPacketsSent;   /*!< Broadcast packets the interface transmitted. Multicast
                                            traffic is counted separately in
                                            `wifi_MulticastPacketsSent`, so this is not a
                                            group-addressed transmit total. */
    ULONG wifi_BroadcastPacketsRecevied; /*!< Broadcast packets the interface received. The member
                                              name misspells `Received` as `Recevied`; the
                                              identifier a caller has to write is
                                              `wifi_BroadcastPacketsRecevied`, and no correctly
                                              spelt member exists in this structure. */
    ULONG wifi_UnknownPacketsReceived; /*!< Packets received that the interface could not place in
                                            any of the categories above. This interface does not
                                            state what makes a packet unknown. */
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
    UCHAR field_format; /*!< IP Address Type Availability field, in the single octet 802.11u encodes
                             it in, carrying the IPv4 and IPv6 availability sub-fields together.
                             This interface states no bit positions for it;
                             `wifi_ipv4_field_values_t` and `wifi_ipv6_field_values_t` above
                             enumerate the values each sub-field takes. */
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
    UCHAR id; /*!< Authentication Information identifier, whose accepted values are the
                   `wifi_auth_id_t` enumerators above; it is what fixes how the octets of `val` are
                   to be read. */
    UCHAR length; /*!< Number of leading octets of `val` that carry the parameter. `val` is
                       dimensioned 16, so a caller must clamp its read at 16 as well as at this
                       length. */
    UCHAR val[16]; /*!< Authentication parameter, of which the leading `length` octets are
                        populated. This interface states no encoding for the octets; that follows
                        from `id`. */
} __attribute__((packed)) wifi_authMethod_t;

/**
 * @brief EAP method.
 */
typedef struct
{
    UCHAR length; /*!< Length field of the EAP method entry as 802.11u encodes it. This interface
                       does not state which of the members below it spans, so a caller should bound
                       `auth_method` by `auth_param_count` rather than by this field. */
    UCHAR method; /*!< EAP method type. This interface enumerates no values for it, so a caller
                       decodes it against the EAP method registry rather than against anything
                       declared here. */
    UCHAR auth_param_count; /*!< Number of leading entries of `auth_method` that carry a parameter.
                                 The array is dimensioned 16, so a caller must clamp its read at 16
                                 as well. */
    wifi_authMethod_t auth_method[16]; /*!< Authentication parameters for this EAP method, of which
                                            the leading `auth_param_count` entries are populated.
                                            The array holds at most 16 whatever `length` expresses. */
} __attribute__((packed)) wifi_eapMethod_t;

/**
 * @brief NAI realm information.
 */
typedef struct
{
    USHORT data_field_length; /*!< Length field of the NAI realm data as 802.11u encodes it. This
                                   interface does not state which members it spans, so a caller
                                   should bound `realm` by `realm_length` and `eap_method` by
                                   `eap_method_count` rather than by this field. */
    UCHAR encoding; /*!< Encoding of `realm`, as the 802.11u NAI Realm Encoding field. This
                         interface enumerates no values for it. */
    UCHAR realm_length; /*!< Number of leading octets of `realm` that carry the name. The field is a
                             `UCHAR`, so it cannot express more than 255, and `realm` is dimensioned
                             256, so a caller must clamp its read at 256 as well. */
    UCHAR realm[256]; /*!< NAI realm name, of which the leading `realm_length` octets are populated
                           in the encoding `encoding` names. It is not stated to be NUL-terminated,
                           so a caller must not run an unbounded string function over it. */
    UCHAR eap_method_count; /*!< Number of leading entries of `eap_method` that carry a method. The
                                 array is dimensioned 16, so a caller must clamp its read at 16 as
                                 well. */
    wifi_eapMethod_t eap_method[16]; /*!< EAP methods this realm accepts, of which the leading
                                          `eap_method_count` entries are populated. The array holds
                                          at most 16. */
} __attribute__((packed)) wifi_naiRealm_t;

/**
 * @brief NAI realm element.
 */
typedef struct
{
    USHORT nai_realm_count; /*!< Number of leading entries of `nai_realm_tuples` that carry a realm.
                                 The field is a `USHORT`, so it can express a count far larger than
                                 the array holds: a value above 20 exceeds this structure's
                                 capacity. */
    wifi_naiRealm_t nai_realm_tuples[20]; /*!< NAI realm tuples, of which the leading
                                               `nai_realm_count` entries are populated. The array
                                               holds at most 20 whatever the count field expresses. */
} __attribute__((packed)) wifi_naiRealmElement_t;

/**
 * @brief Venue name.
 */
typedef struct
{
    UCHAR length; /*!< Length field of the venue name duple as 802.11u encodes it. This interface
                       does not state whether it counts the three `language` octets as well as the
                       name, so a caller cannot derive the name length from it without agreeing that
                       convention with the platform implementer; `name` is bounded at its declared
                       256 octets either way. */
    UCHAR language[3]; /*!< Language of `name`, as the three-octet language code 802.11u carries.
                            Three octets leave no room for a terminator, so a caller must read
                            exactly three. */
    UCHAR name[256]; /*!< Venue name, in the encoding `language` names. It is not stated to be
                          NUL-terminated and this structure carries no separate name length, so a
                          caller must bound any read at the declared 256 octets. */
} __attribute__((packed)) wifi_venueName_t;

/**
 * @brief Venue name element.
 */
typedef struct
{
    UCHAR venueGroup; /*!< Venue Group code of the 802.11u Venue Info field. This interface
                           enumerates no values for it, and `venueType` beside it is only meaningful
                           within this group. */
    UCHAR venueType; /*!< Venue Type code within `venueGroup`: the same numeric type denotes
                          different venues in different groups, so the two members are only
                          meaningful together. */
    wifi_venueName_t venueNameTuples[16]; /*!< Venue name duples, at most 16. This interface
                                               declares no count member for the array, so how many
                                               entries are populated is not established here and a
                                               caller has to take that from the platform implementer
                                               or from each entry's own `length`. */
} __attribute__((packed)) wifi_venueNameElement_t;

/**
 * @brief PLMN (Public Land Mobile Network) information.
 */
typedef struct
{
    UCHAR PLMN[3]; /*!< Public Land Mobile Network identifier, in the three octets the 3GPP encoding
                        packs a mobile country code and mobile network code into. This interface
                        states no field layout within the three octets, so a caller must decode them
                        rather than reading digits out directly. */
} __attribute__((packed)) wifi_plmn_t;

/**
 * @brief 3GPP PLMN list information element.
 */
typedef struct
{
    UCHAR iei; /*!< Element identifier of the PLMN list as the 3GPP encoding defines it. This
                    interface enumerates no values for it and directs that zero be copied here for
                    now, so a caller should write zero rather than deriving a value. */
    UCHAR plmn_length; /*!< Length field of the PLMN list as the 3GPP information element encodes
                            it. This interface does not state whether it counts octets or entries,
                            so a caller should bound `plmn` by `number_of_plmns` beside it rather
                            than by this field. */
    UCHAR number_of_plmns; /*!< Number of leading entries of `plmn` that carry an identifier. The
                                array is dimensioned 16, so a caller must clamp its read at 16 as
                                well. */
    wifi_plmn_t plmn[16]; /*!< PLMN identifiers, of which the leading `number_of_plmns` entries are
                               populated. The array holds at most 16. */
} __attribute__((packed)) wifi_3gpp_plmn_list_information_element_t;

/**
 * @brief 3GPP cellular network information.
 */
typedef struct
{
    UCHAR gud; /*!< GUD field of the 3GPP Cellular Network element, which names the version of the
                    3GPP information the element carries. This interface enumerates no values for
                    it, so a caller decodes it against the 3GPP encoding rather than against
                    anything declared here. */
    UCHAR uhdLength; /*!< Length field the 3GPP encoding places after `gud`. This interface does not
                          state which of the members below it counts, so a caller should bound the
                          PLMN list by `plmn_information.number_of_plmns` rather than by this field. */
    wifi_3gpp_plmn_list_information_element_t plmn_information; /*!< PLMN list the 3GPP Cellular
                        Network element advertises: `wifi_3gpp_plmn_list_information_element_t`
                        carries the identifiers themselves, of which `number_of_plmns` are
                        populated. `uhdLength` above is the element's own length field and this
                        interface does not state how it relates to that count, so a caller should
                        bound its read by the count. */
} __attribute__((packed)) wifi_3gppCellularNetwork_t;

/**
 * @brief Domain name tuple.
 */
typedef struct
{
    UCHAR length; /*!< Number of leading octets of `domainName` that carry the name. That array is
                       dimensioned 255, which its own comment gives as the specification maximum, so
                       a caller must clamp its read at 255 as well. */
    UCHAR domainName[255]; /*!< The name itself, of which the leading `length` octets above are
                                meaningful. The array is dimensioned 255, which is the specification
                                maximum, and this interface states neither the encoding nor whether
                                the value is `NUL`-terminated, so a caller must take its extent from
                                `length` and must not run an unbounded string function over the
                                member. */
} __attribute__((packed)) wifi_domainNameTuple_t;

/**
 * @brief Domain name.
 */
typedef struct
{
    wifi_domainNameTuple_t domainNameTuple[4]; /*!< Domain name tuples, at most 4. This interface
                                                    declares no count member for the array, so how
                                                    many entries are populated is not established
                                                    here and a caller has to take that from the
                                                    platform implementer or from each entry's own
                                                    `length`. */
} __attribute__((packed)) wifi_domainName_t;

/**
 * @brief OUI (Organizationally Unique Identifier) duple.
 */
typedef struct
{
    UCHAR length; /*!< Number of leading octets of `oui` that carry the identifier.
                       `wifi_roamingConsortiumElement_t` below states that an OI is between 3 and 15
                       octets long, and `oui` is dimensioned 15, so a caller must clamp its read at
                       15. */
    UCHAR oui[15]; /*!< Organization Identifier, of which the leading `length` octets are populated.
                        These are raw identifier octets rather than text and carry no terminator. */
} __attribute__((packed)) wifi_ouiDuple_t;

/**
 * @brief Roaming consortium.
 */
typedef struct
{
    wifi_ouiDuple_t ouiDuple[32]; /*!< Roaming consortium OI duples, at most 32. This interface
                                       declares no count member for the array;
                                       `wifi_roamingConsortiumElement_t` below carries the three OIs
                                       that fit in a beacon or probe response together with their
                                       lengths. */
} __attribute__((packed)) wifi_roamingConsortium_t;

/**
 * @brief Capability list ANQP.
 */
typedef struct
{
    USHORT capabilityList[64]; /*!< ANQP capability identifiers the `AP` advertises, at most 64.
                                    This interface declares no count member for the array and
                                    enumerates none of the identifiers, so how many entries are
                                    populated is not established here. */
} __attribute__((packed)) wifi_capabilityListANQP_t;

/**
 * @brief Roaming consortium element.
 */
typedef struct
{
    UCHAR wifiRoamingConsortiumCount; /*!< Number of leading entries of `wifiRoamingConsortiumOui`
                                           and `wifiRoamingConsortiumLen` below that carry an OI.
                                           Both arrays hold three entries, which is the limit the
                                           next member's comment states for a beacon or probe
                                           response, so a caller must clamp its read at 3 as well. */
    UCHAR wifiRoamingConsortiumOui[3][15 + 1]; /**< Only 3 OIs are allowed in beacon and probe responses. OI length is variable between 3 and 15. */
    UCHAR wifiRoamingConsortiumLen[3]; /*!< Number of leading octets of each corresponding
                                            `wifiRoamingConsortiumOui` entry that carry the
                                            identifier - the entries are indexed alike. An OI is
                                            between 3 and 15 octets, and each row is dimensioned 16,
                                            so a caller must clamp its read at the declared row
                                            width. */
} __attribute__((packed)) wifi_roamingConsortiumElement_t;


// HS2 Related ANQP Elements start


// =========================================Start-HS2-Operator Friendly Name=========================================================================
// HS2.0 Operator Name Duple #1     HS2.0 Operator Name Duple #2     through      HS2.0 Operator Name Duple #n
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
    UCHAR languageCode[3]; /*!< Language of `operatorName`, as the three-octet language code the
                                specification carries and which `length` above is documented as
                                including. Three octets leave no room for a terminator, so a caller
                                must read exactly three. */
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
    UCHAR wanInfo; /*!< WAN Info octet, whose bit assignments are listed in the comment on
                        `wifi_HS2_Wan_Info_Link_Status_t` below - link status in bits 0 to 1,
                        symmetric link in bit 2, at capacity in bit 3 - and whose link-status values
                        are that enumeration's members. */
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
// Proto Port Tuple #1        Proto Port Tuple #2    through     Proto Port Tuple #n
//          4                        4(optional)                   4(optional)
// Proto Port Tuple Format
// IP Protocol                             Port Number                  Status
//      1                                       2                         1

/**
 * @brief HS2.0 Protocol and Port Tuple (figure 9-595).
 */
typedef struct
{
    UCHAR ipProtocol; /*!< IP protocol number the tuple describes. This interface enumerates no
                           values for it, so a caller decodes it against the IANA protocol registry. */
    USHORT portNumber; /*!< Transport port the tuple describes, interpreted within `ipProtocol`. */
    UCHAR status; /*!< Whether the protocol and port are reachable through the `AP`, as one of the
                       `wifi_HS2_ConnectionCapability_Status_t` enumerators below. */
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

// NAI Realm Count                          NAI Home Realm                    NAI Home Realm through NAI Home Realm
//                                         Name Data #1                        Name Data #2            Name Data #n
//       1                                   variable                         (variable optional)       (variable optional)

// NAI Realm Encoding                                 NAI Home Realm Name Length              NAI Home Realm
//          1                                                      1                               variable

/**
 * @brief HS2.0 NAI Home Realm Data (figure 9-595).
 */
typedef struct
{
    UCHAR encoding; /*!< Encoding of `name`, as the 802.11u NAI Realm Encoding field. This interface
                         enumerates no values for it. */
    UCHAR length; /*!< Number of leading octets of `name` that carry the realm. That array is
                       dimensioned 255, which its own comment gives as the specification maximum, so
                       a caller must clamp its read at 255 as well. */
    UCHAR name[255]; /**< Name (maximum length is 255 according to the specification). */
} __attribute__((packed)) wifi_HS2_NAI_Home_Realm_Data_t;

/**
 * @brief HS2.0 NAI Home Realm Query (figure 9-595).
 */
typedef struct
{
    UCHAR realmCount; /*!< Number of leading entries of `homeRealmData` that carry a realm.
                           The field is a `UCHAR`, so it can express a count larger than
                           `homeRealmData` can hold; a value above 20 exceeds this
                           structure's capacity. */
    wifi_HS2_NAI_Home_Realm_Data_t homeRealmData[20]; /*!< Home realm data. This array holds
                           at most 20 realms; the 255 the specification allows a realm-count
                           field to express is the protocol limit, not the capacity of this
                           structure. Each entry's `name` holds at most 255 bytes. */
} __attribute__((packed)) wifi_HS2_NAI_Home_Realm_Query_t;

// =========================================End-HS2-NAI Realm Query Element=========================================================================

// =========================================Start-HS2-Capability List=========================================================================
// HS2.0 Capability #1     HS2.0 Capability #2     through      HS2.0 Capability #n
//             1             0 or 1 (optional)                      0 or 1 (optional)
// =========================================End-HS2-Capability List=========================================================================

/**
 * @brief HS2.0 Capability List.
 */
typedef struct
{
    UCHAR capabilityList[64]; /*!< Hotspot 2.0 capability identifiers the `AP` advertises, one per
                                   octet and at most 64. This interface declares no count member for
                                   the array and enumerates none of the identifiers. */
} __attribute__((packed)) wifi_HS2_CapabilityList_t;

/**
 * @brief EAPOL key frame.
 */
typedef struct
{
    unsigned char descriptor; /*!< EAPOL-Key descriptor type, which is what fixes how the rest of
                                   the frame is read. This interface enumerates no values for it. */
    unsigned char key_info[2]; /*!< Key Information field, in two octets. The `KEY_MSG_1_OF_4` to
                                    `KEY_MSG_4_OF_4` macros in `wifi_hal_generic.h` decode it,
                                    testing `key_info[1]` for the key type and version bits under
                                    `KI1_VER_MASK` and `key_info[0]` for the MIC, secure and
                                    encryption bits. */
    unsigned short key_len; /*!< Length of the key the frame conveys. This interface states no unit
                                 for it, and it does not describe `data`: `len` below gives that
                                 length. */
    unsigned char replay[8]; /*!< Key Replay Counter field, in eight raw octets. This interface
                                  states neither the octet order nor a starting value, so a caller
                                  must not compare two counters arithmetically without knowing that
                                  order; the field is not text and carries no terminator. */
    unsigned char nonce[32]; /*!< Nonce of the exchange, in 32 raw octets. It is not text and
                                  carries no terminator. */
    unsigned char init_vector[16]; /*!< Key IV field, in 16 raw octets. This interface does not
                                        state for which descriptor types the field is meaningful. */
    unsigned char rsc[8]; /*!< Key RSC field, in eight raw octets, carrying the starting sequence
                               number for the key being installed. This interface states no octet
                               order for it and does not say for which descriptor types or message
                               numbers it is meaningful. */
    unsigned char key_id[8]; /*!< Key Identifier field, in 8 raw octets. This interface states no
                                  interpretation for them. */
    unsigned char mic[16]; /*!< Key MIC field, in sixteen raw octets. Whether it is present at all
                                follows from the MIC bit of `key_info` above, which the `KEY_MSG_*`
                                macros in `wifi_hal_generic.h` test; this interface states neither
                                the algorithm that produced it nor what the field holds when that
                                bit is clear. */
    unsigned short len; /*!< Number of octets of `data` below that follow the fixed header. It
                             bounds that member and nothing else - `key_len` above is the length of
                             the key the frame conveys - and being a `unsigned short` it carries no
                             sentinel, so a caller must check it against the buffer it actually
                             holds. */
    unsigned char data[0]; /*!< Start of the key data that follows the fixed header, whose length is
                                `len` above. The member is a zero-length array and so occupies
                                nothing in `sizeof(wifi_eapol_key_frame_t)`: a caller must have that
                                many bytes plus `len` behind the pointer, and must read no further
                                than `len`. */
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
 *
 * The three named members are the four-octet fixed header an EAP packet carries - one
 * octet of code, one of identifier and two of length - which is what
 * `sizeof(wifi_eap_frame_t)` yields, the structure being packed. RFC 3748 section 4
 * defines that header and the fields in it.
 */
typedef struct
{
    unsigned char code; /*!< EAP code, whose accepted values are the `wifi_eap_code_t` enumerators
                             above - request, response, success or failure. */
    unsigned char id; /*!< Identifier that pairs an EAP response with the request it answers. */
    unsigned short len; /*!< Length field of the EAP packet. RFC 3748 section 4 defines the
                             on-the-wire Length as covering the whole packet including the
                             four-octet header, so on that reading `data` below holds `len` minus
                             four octets; this interface does not state whether the member carries
                             that on-the-wire value or only the length of `data`, and it states no
                             octet order for it either. A caller must take the convention from the
                             implementation it runs against, must not assume either reading, and
                             must in every case check the value against the buffer it actually
                             holds before reading. */
    unsigned char data[0]; /*!< Start of the EAP payload that follows the fixed header. Its length
                                follows from `len` above under whichever of the two readings
                                recorded there the implementation uses, so this interface does not
                                state it exactly. The member is a zero-length array and so
                                occupies nothing in `sizeof(wifi_eap_frame_t)`: a caller must have
                                the header plus that payload behind the pointer and must read no
                                further than the payload length it established. */
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
    EAPOL_MSG_NONE = 0, /*!< No handshake message; the exchange has not started. */
    EAPOL_MSG_M1,       /*!< M1: ANonce sent by the authenticator. */
    EAPOL_MSG_M2,       /*!< M2: SNonce and MIC returned by the supplicant. */
    EAPOL_MSG_M3,       /*!< M3: GTK and MIC sent by the authenticator. */
    EAPOL_MSG_M4        /*!< M4: final acknowledgement from the supplicant. */
} eapol_msg_type_t;

/**
 * @brief Identifies which management exchange an EAPOL handshake belongs to.
 *
 * A four-way handshake follows either an initial association or a reassociation, and
 * the two are counted separately.
 */
typedef enum {
    EAPOL_FRAME_UNKNOWN = 0, /*!< The originating exchange could not be determined. */
    EAPOL_FRAME_ASSOC,       /*!< The handshake followed an association request. */
    EAPOL_FRAME_REASSOC      /*!< The handshake followed a reassociation request. */
} eapol_frame_type_t;

/**
 * @brief Index into per-message EAPOL handshake statistics.
 *
 * Combines the handshake message with the exchange it belongs to. The enumerators cover
 * M1, M2 and M3 for each of the two exchanges and stop there: there is no `M4_ASSOC` or
 * `M4_REASSOC`, so the `EAPOL_MSG_M4` message that `eapol_msg_type_t` above names has no
 * counter slot reachable through this index and a caller cannot obtain a per-message count
 * for it here. `EAPOL_STATUS_TYPE_MAX` is the element count and is not itself a valid
 * index.
 */
typedef enum {
    M1_ASSOC = 0,          /*!< M1 seen during an association exchange. */
    M1_REASSOC,            /*!< M1 seen during a reassociation exchange. */
    M2_ASSOC,              /*!< M2 seen during an association exchange. */
    M2_REASSOC,            /*!< M2 seen during a reassociation exchange. */
    M3_ASSOC,              /*!< M3 seen during an association exchange. */
    M3_REASSOC,            /*!< M3 seen during a reassociation exchange. */
    EAPOL_STATUS_TYPE_MAX  /*!< Number of index values; not a valid index itself. */
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
    unsigned char version; /*!< IEEE 802.1X protocol version from the frame header. */
    unsigned char type; /*!< EAPOL packet type; see `wifi_eapol_type_t`. */
    unsigned short len; /*!< Length in bytes of the payload that follows this header. */
    unsigned char data[0]; /*!< Start of the payload; `len` bytes, not counted in this structure's size. */
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
    INT RadiusServerRetries;           /*!< How many times a RADIUS request is retransmitted before
                                            the exchange is abandoned; `RadiusServerRequestTimeout`
                                            below gives the wait between attempts in seconds. This
                                            interface states no accepted range and no value that
                                            means unlimited. */
    INT RadiusServerRequestTimeout;    /**< RADIUS request timeout in seconds after which the request must be retransmitted for the number of retries available. */
    INT PMKLifetime;                   /**< Default time in seconds after which a Wi-Fi client is forced to re-authenticate (default: 8 hours). */
    BOOL PMKCaching;                  /*!< `TRUE` where the pairwise master key may be cached so a
                                           returning client can skip a full authentication.
                                           `PMKLifetime` above bounds how long a key stays usable
                                           and `PMKCacheInterval` below how often the cache is
                                           purged, so a caller must set all three coherently. */
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
 *                            allocates and releases it, and the `HAL` writes into it
 *                            during the call. Whether the implementation retains the
 *                            pointer beyond the call is not specified by this interface,
 *                            so the caller should keep the structure allocated and
 *                            unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds a full set of counters; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The counters were retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The counters are cumulative since the last vendor reset, so a caller measuring a
 *       rate must difference two samples itself; this interface defines no reset call.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                                           device. The caller allocates and releases it,
 *                                           and the `HAL` writes into it during the call.
 *                                           Whether the implementation retains the
 *                                           pointer beyond the call is not specified by
 *                                           this interface, so the caller should keep the
 *                                           array allocated and unmoved while the `HAL`
 *                                           remains initialised. This interface does not
 *                                           state whether the implementation writes to
 *                                           elements beyond the reported count, so a
 *                                           caller must neither read them nor rely on
 *                                           their previous contents surviving.
 * @param[in] maxNumDevices  Number of elements the array can hold. `ASSOC_MAC_ARRAY_MAX`
 *                           is the largest associated-device population this interface
 *                           names, so an array sized to that constant cannot overflow. A
 *                           value of 0 is not useful and the interface does not define
 *                           its effect.
 * @param[out] output_numDevices  Caller-allocated variable that receives the number of
 *                                elements actually written, never more than
 *                                `maxNumDevices`. The caller allocates and releases it,
 *                                and the `HAL` writes into it during the call. Whether
 *                                the implementation retains the pointer beyond the call
 *                                is not specified by this interface, so the caller should
 *                                keep the variable allocated and unmoved while the `HAL`
 *                                remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the array's first `*output_numDevices` elements hold MAC addresses;
 *       this interface does not state what, if anything, was written beyond that count.
 *       On failure the output is left unspecified, so a caller must not read it unless
 *       the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The list was retrieved. A value of 0 in `*output_numDevices`
 *                          is a success with no associated devices, not an error.
 * @retval WIFI_HAL_ERROR   The list was not retrieved. This interface does not enumerate
 *                          the conditions that lead to this code. The caller should
 *                          validate its arguments; a failure that persists across retries
 *                          should be logged and the value treated as unavailable.
 *
 * @note This interface does not state whether the call truncates or fails when more than
 *       `maxNumDevices` devices are associated - `WIFI_HAL_ERROR` is not documented as
 *       carrying that meaning, and no separate code is defined for it - so a caller should
 *       size the array from `wifi_getApNumDevicesAssociated()` or from
 *       `ASSOC_MAC_ARRAY_MAX` rather than relying on either behaviour.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @see wifi_getApNumDevicesAssociated
 */
INT wifi_getApAssociatedDevice(INT ap_index, mac_address_t *output_deviceMacAddressArray, UINT maxNumDevices, UINT *output_numDevices);
#endif

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

/**
 * @brief Outcome of a RADIUS or EAP authentication, as reported to a callback.
 *
 * The values are defined by this interface and are not the corresponding protocol packet
 * codes, so a caller must not use an enumerator as a wire value and must not derive one
 * from a captured frame. RFC 2865 section 3 assigns RADIUS code 2 to Access-Accept, and
 * RFC 3748 section 4 assigns EAP code 3 to Success and 4 to Failure; of the three
 * enumerators below only `WIFI_EAP_SUCCESS_STATUS` happens to coincide with its protocol
 * code. This interface does not state why the numbering is sparse.
 */
typedef enum {
    WIFI_ACCESS_ACCEPT_STATUS = 0, /*!< The RADIUS server authorised the client, which RFC 2865
                                        section 3 conveys as Access-Accept. The value 0 is this
                                        interface's own identifier for that outcome, not the RADIUS
                                        code. */
    WIFI_EAP_SUCCESS_STATUS = 3,   /*!< The EAP exchange completed successfully, which RFC 3748
                                        section 4 conveys as an EAP Success packet. */
    WIFI_EAP_FAILURE_STATUS = 23   /*!< The EAP exchange was rejected, which RFC 3748 section 4
                                        conveys as an EAP Failure packet. The value 23 is this
                                        interface's own identifier for that outcome, not the EAP
                                        code. */
} wifi_eap_status_code_t;

/**
 * @brief IEEE 802.11 reason codes carried by deauthentication and disassociation
 *        frames.
 *
 * The subset this interface names is a selection of the codes IEEE Std 802.11-2020
 * Table 9-49 defines, and each value is the reason code as it appears on the wire, so the
 * set is deliberately sparse. That table gives each code one meaning; it does not map a
 * code to a cause such as a mistyped credential, and neither does this interface, so a
 * caller must not read one into a value below.
 */
typedef enum {
    WIFI_REASON_UNSPECIFIED = 1,                 /*!< No specific reason was given. */
    WIFI_REASON_PREV_AUTH_NOT_VALID = 2,         /*!< Previous authentication no longer valid, the
                                                      meaning IEEE Std 802.11-2020 Table 9-49 gives
                                                      reason code 2. That table names no cause for
                                                      it, and this interface narrows it to none. */
    WIFI_REASON_DEAUTH_LEAVING = 3,              /*!< The station is leaving or has left the BSS. */
    WIFI_REASON_STA_REQ_ASSOC_WITHOUT_AUTH = 9,  /*!< The station associated without first authenticating. */
    WIFI_REASON_INVALID_IE = 13,                 /*!< An information element was malformed. */
    WIFI_REASON_MICHAEL_MIC_FAILURE = 14,        /*!< A TKIP Michael MIC check failed. */
    WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT = 15,     /*!< The four-way handshake did not complete in time. */
    WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,   /*!< The group key handshake did not complete in time. */
    WIFI_REASON_IE_IN_4WAY_DIFFERS = 17,         /*!< An information element in the four-way handshake differed from the one advertised. */
    WIFI_REASON_GROUP_CIPHER_NOT_VALID = 18,     /*!< The requested group cipher is not supported. */
    WIFI_REASON_PAIRWISE_CIPHER_NOT_VALID = 19,  /*!< The requested pairwise cipher is not supported. */
    WIFI_REASON_AKMP_NOT_VALID = 20,             /*!< The requested authentication and key management suite is not supported. */
    WIFI_REASON_UNSUPPORTED_RSN_IE_VERSION = 21, /*!< The RSN information element version is not supported. */
    WIFI_REASON_INVALID_RSN_IE_CAPAB = 22,       /*!< The RSN information element capabilities field is invalid. */
    WIFI_REASON_IEEE_802_1X_AUTH_FAILED = 23,    /*!< IEEE 802.1X authentication failed. */
    WIFI_REASON_CIPHER_SUITE_REJECTED = 24,      /*!< The cipher suite was rejected by local policy. */
    WIFI_REASON_INVALID_PMKID = 49               /*!< The supplied PMKID does not match a cached PMKSA. */
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
    WIFI_STATUS_UNSPECIFIED_FAILURE = 1,                 /*!< The request failed for an unstated reason. */
    WIFI_STATUS_AUTH_TIMEOUT = 16,                       /*!< Authentication did not complete within the allowed time. */
    WIFI_STATUS_ASSOC_REJECTED_TEMPORARILY = 30,         /*!< Association was refused for now; the client may retry after the indicated interval. */
    WIFI_STATUS_ROBUST_MGMT_FRAME_POLICY_VIOLATION = 31, /*!< The request violated the robust management frame protection policy. */
    WIFI_STATUS_AKMP_NOT_VALID = 43,                     /*!< The requested authentication and key management suite is not supported. */
    WIFI_STATUS_INVALID_PMKID = 53                       /*!< The supplied PMKID does not match a cached PMKSA. */
} wifi_status_code_t;

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */

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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success every parameter of this Access Point holds its factory default, while
 *       other Access Points and all radio parameters are untouched and no Wi-Fi reboot is
 *       required. On failure this interface does not specify how much of the requested
 *       change was applied, so a caller should read the affected state back rather than
 *       assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The Access Point was reset.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          Access Point back with `wifi_getRadioVapInfoMap()` to establish
 *                          the actual state before retrying, because a partial reset is not
 *                          excluded by this interface.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point entry is removed from the hardware and the internal
 *       variables associated with it are cleared. On failure this interface does not
 *       specify how much of the requested change was applied, so a caller should read the
 *       affected state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The Access Point was deleted.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should treat the
 *                          Access Point's state as unknown and re-read it rather than
 *                          assuming it still exists.
 *
 * @note A deleted Access Point must be recreated with `wifi_createVAP()` before any other
 *       call referring to its index is meaningful.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                            the Access Point name. This interface does not state whether
 *                            the name is `NUL`-terminated or how its length is conveyed,
 *                            so a caller must not assume either: treat the buffer as
 *                            opaque of at most the 16 bytes it is required to provide and
 *                            bound every read by that. The caller allocates and releases
 *                            it, and the `HAL` writes into it during the call. Whether
 *                            the implementation retains the pointer beyond the call is
 *                            not specified by this interface, so the caller should keep
 *                            the buffer allocated and unmoved while the `HAL` remains
 *                            initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success `output_string` holds the Access Point name, on the representation
 *       terms `output_string` above states; on failure its contents are undefined.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The name was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the name treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success frames larger than `threshold` use RTS/CTS backoff. On failure this
 *       interface does not specify how much of the requested change was applied, and it
 *       offers no call that reads this threshold back, so a caller must treat the setting
 *       as being in an unknown state.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The threshold was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it does not state the
 *                          accepted values for `threshold`, so a caller must not infer a
 *                          bound from the failure; it should report the failure rather than
 *                          retrying with the same argument.
 *
 * @note `wifi_getAPCapabilities()` reports whether this Access Point supports an RTS/CTS
 *       threshold at all, through `rtsThresholdSupported`; a caller should check that
 *       before treating a failure here as unexpected.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the internal security variable settings for this Access Point are
 *       deleted. On failure this interface does not specify how much of the requested
 *       change was applied, so a caller should read the affected state back rather than
 *       assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were removed.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          security configuration back with `wifi_getApSecurity()` before
 *                          relying on it.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the hardware no longer encrypts traffic on this Access Point. On failure
 *       this interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS Encryption was disabled.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          security configuration back with `wifi_getApSecurity()` rather
 *                          than assuming either state.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                           releases it, and the `HAL` writes into it during the call.
 *                           Whether the implementation retains the pointer beyond the
 *                           call is not specified by this interface, so the caller should
 *                           keep the variable allocated and unmoved while the `HAL`
 *                           remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the current associated-station count; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The count was retrieved. Zero is a valid count, not an error.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The count can change between this call and any later call that enumerates the
 *       devices, so a caller sizing an array from it should allow headroom or size from
 *       `ASSOC_MAC_ARRAY_MAX`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[in] client_mac  MAC address of the client to disassociate: six octets in
 *                        caller-owned storage. `mac_address_t` is an
 *                        array typedef, so the parameter adjusts to a pointer and the
 *                        callee receives the address of the caller's own storage rather
 *                        than a copy of it. The caller must supply all six octets and
 *                        must keep the storage valid for the duration of the call.
 *                        `Memory Model` in the HAL specification establishes who owns
 *                        that storage but not how long the implementation may hold a
 *                        pointer to it, so the caller should keep the six octets
 *                        allocated and unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success any active association between that client and this Access Point is
 *       removed. On failure this interface does not specify how much of the requested
 *       change was applied, so a caller should read the affected state back rather than
 *       assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The disassociation was issued.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should confirm the
 *                          client is present with `wifi_getApAssociatedDevice()` before
 *                          treating this as a fault.
 *
 * @note Nothing prevents the client from associating again immediately. To keep it off the
 *       Access Point, add it to the Access Control List with `wifi_addApAclDevice()` and
 *       set a blacklist filter mode first.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                         it, and the `HAL` writes into it during the call. Whether the
 *                         implementation retains the pointer beyond the call is not
 *                         specified by this interface, so the caller should keep the
 *                         variable allocated and unmoved while the `HAL` remains
 *                         initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the hosting radio's index; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The radio index was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The value is a radio index in the range `RADIO_INDEX_1` to `RADIO_INDEX_3`, bounded
 *       by `MAX_NUM_RADIOS`, which is 2 or 3 depending on whether `WIFI_HAL_VERSION_3` is
 *       defined.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                       caller allocates and releases it, and the `HAL` writes into it
 *                       during the call. Whether the implementation retains the pointer
 *                       beyond the call is not specified by this interface, so the caller
 *                       should keep the array allocated and unmoved while the `HAL`
 *                       remains initialised. This interface does not state whether the
 *                       implementation writes to elements beyond the reported count, so a
 *                       caller must neither read them nor rely on their previous contents
 *                       surviving.
 * @param[in] maxArraySize  Number of elements the array can hold. `ACL_MAC_ARRAY_MAX` is
 *                          the largest Access Control List this interface names, so an
 *                          array sized to that constant cannot overflow.
 * @param[out] output_numEntries  Caller-allocated variable that receives the number of
 *                                elements actually written, never more than
 *                                `maxArraySize`. The caller allocates and releases it,
 *                                and the `HAL` writes into it during the call. Whether
 *                                the implementation retains the pointer beyond the call
 *                                is not specified by this interface, so the caller should
 *                                keep the variable allocated and unmoved while the `HAL`
 *                                remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the array's first `*output_numEntries` elements hold MAC addresses;
 *       this interface does not state what, if anything, was written beyond that count.
 *       On failure the output is left unspecified, so a caller must not read it unless
 *       the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The list was retrieved. Zero entries is a success, meaning the
 *                          list is empty.
 * @retval WIFI_HAL_ERROR   The list was not retrieved. This interface does not enumerate
 *                          the conditions that lead to this code. The caller should
 *                          validate its arguments; a failure that persists across retries
 *                          should be logged and the value treated as unavailable.
 *
 * @note Size the array from `wifi_getApAclDeviceNum()` or from `ACL_MAC_ARRAY_MAX`: this
 *       interface does not state whether an undersized array truncates or fails, and it
 *       defines no code that carries that meaning, so neither outcome may be assumed.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[in] DeviceMacAddress  MAC address of the device to admit or bar: six octets in
 *                              caller-owned storage. `mac_address_t` is
 *                              an array typedef, so the parameter adjusts to a pointer
 *                              and the callee receives the address of the caller's own
 *                              storage rather than a copy of it. The caller must supply
 *                              all six octets and must keep the storage valid for the
 *                              duration of the call. `Memory Model` in
 *                              the HAL specification establishes who owns that storage
 *                              but not how long the implementation may hold a pointer to
 *                              it, so the caller should keep the six octets allocated and
 *                              unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the address is present in this Access Point's Access Control List. On
 *       failure this interface does not specify how much of the requested change was
 *       applied, so a caller should read the affected state back rather than assume the
 *       previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The address was added.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          entry count with `wifi_getApAclDeviceNum()` before retrying,
 *                          since a full list is not a transient condition.
 *
 * @note Adding an address changes who may associate only while a filter mode is active; the
 *       mode is set by `wifi_setApMacAddressControlMode()` and determines whether the list
 *       acts as a whitelist or a blacklist.
 * @note This interface does not state whether adding an address already present succeeds or
 *       fails, so a caller should not depend on either.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] deviceMacAddress  MAC address of the device to remove: six octets in
 *                              caller-owned storage. `mac_address_t` is
 *                              an array typedef, so the parameter adjusts to a pointer
 *                              and the callee receives the address of the caller's own
 *                              storage rather than a copy of it. The caller must supply
 *                              all six octets and must keep the storage valid for the
 *                              duration of the call. `Memory Model` in
 *                              the HAL specification establishes who owns that storage
 *                              but not how long the implementation may hold a pointer to
 *                              it, so the caller should keep the six octets allocated and
 *                              unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the address is absent from this Access Point's Access Control List. On
 *       failure this interface does not specify how much of the requested change was
 *       applied, so a caller should read the affected state back rather than assume the
 *       previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The address was removed.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          list back with `wifi_getApAclDevices()` rather than assuming the
 *                          entry is gone.
 *
 * @note Removing an address from a blacklist does not disconnect a client that is already
 *       associated, nor does removing it from a whitelist; use
 *       `wifi_kickApAssociatedDevice()` for that.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success this Access Point's Access Control List is empty. On failure this
 *       interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The list was cleared.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          entry count back with `wifi_getApAclDeviceNum()`, because a
 *                          partial clear is not excluded by this interface.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning With an empty list and a whitelist filter mode active, no client can associate.
 *          Set the filter mode before or with clearing the list if that is not intended.
 * @see wifi_setApMacAddressControlMode
 */
INT wifi_delApAclDevices(INT apIndex);

/**
 * @brief Gets the number of devices in the Access Control List (ACL) for an Access Point.
 *
 * This function retrieves the number of devices in the filter list for the specified Access Point (AP).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[out] output_uint  Caller-allocated variable that receives the number of entries
 *                          in the Access Control List. The caller allocates and releases
 *                          it, and the `HAL` writes into it during the call. Whether the
 *                          implementation retains the pointer beyond the call is not
 *                          specified by this interface, so the caller should keep the
 *                          variable allocated and unmoved while the `HAL` remains
 *                          initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the current entry count; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The count was retrieved. Zero is a valid count.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note Use this to size the array passed to `wifi_getApAclDevices()`, bounded by
 *       `ACL_MAC_ARRAY_MAX`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @see wifi_getApAclDevices
 */
INT wifi_getApAclDeviceNum(INT apIndex, UINT *output_uint);

/**
 * @brief Enables or disables the "kick" feature for devices on the ACL blacklist for an Access Point.
 *
 * This function controls whether devices on the Access Control List (ACL) blacklist
 * for the specified Access Point (AP) should be actively kicked (disassociated).
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] enable  true to disassociate clients that appear on the Access Control List
 *                    blacklist, false to leave them associated.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success blacklisted clients are actively disassociated when `enable` is true,
 *       and left alone when it is false. On failure this interface does not specify how
 *       much of the requested change was applied, so a caller should read the affected
 *       state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enable`, so a caller cannot read this code as a
 *                          verdict on the argument; it should confirm the Access Point is
 *                          present with `wifi_getApStatus()`.
 *
 * @note This governs enforcement against already-associated clients only. Whether a client
 *       may associate in the first place is governed by the filter mode set with
 *       `wifi_setApMacAddressControlMode()`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point applies the requested filter mode. On failure this
 *       interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The mode was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should re-check
 *                          `filterMode` against the values its parameter entry above states
 *                          before retrying with the same argument.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                                caller allocates and releases it, and the `HAL` writes
 *                                into it during the call. Whether the implementation
 *                                retains the pointer beyond the call is not specified by
 *                                this interface, so the caller should keep the variable
 *                                allocated and unmoved while the `HAL` remains
 *                                initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the current filter mode; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The mode was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the VLAN identifier is recorded in the internal environment variable
 *       that carries it. On failure this interface does not specify how much of the
 *       requested change was applied, and it offers no call that reads the identifier
 *       back, so a caller must treat the setting as being in an unknown state.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The VLAN identifier was set.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it does not state the
 *                          accepted values for `vlanId`, so a caller must not infer a bound
 *                          from the failure; it should report the failure rather than
 *                          retrying with the same argument.
 *
 * @note The value is stored in an internal environment variable, so this interface does not
 *       state whether it takes effect immediately or at the next Access Point restart.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point's VLAN configuration is back at its default. On failure
 *       this interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The VLAN configuration was reset.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should not assume
 *                          the previous VLAN identifier survived, and should set it again
 *                          explicitly if it is needed.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the internal enable variable holds the requested value. On failure this
 *       interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The enable state was set.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enable`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getApEnable()` before relying on it.
 *
 * @note This sets the requested state. Whether the Access Point has actually reached it is
 *       reported by `wifi_getApStatus()`, and a transition is notified through
 *       `wifi_vapstatus_callback_register()`; `wifi_getApEnable()` reads back only what was
 *       requested here.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                          allocates and releases it, and the `HAL` writes into it during
 *                          the call. Whether the implementation retains the pointer
 *                          beyond the call is not specified by this interface, so the
 *                          caller should keep the variable allocated and unmoved while
 *                          the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the requested enable state; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This reports the requested state, not the operational one. Use `wifi_getApStatus()`
 *       to find out whether the Access Point is actually up.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[out] output_string  Caller-allocated buffer that receives the operational status
 *                            as reported by the driver, either `"Enabled"` or
 *                            `"Disabled"`. This interface does not state whether the
 *                            value is `NUL`-terminated or how its length is conveyed, so
 *                            a caller must not assume either. It states no minimum size
 *                            for the buffer either: the longer of the two values it names
 *                            is eight characters, a ninth byte would be needed to
 *                            terminate it, and nothing here bounds what the driver may
 *                            write instead, so a caller must not treat nine bytes as a
 *                            safe size and must bound every read by the size it
 *                            allocated. The caller allocates and releases it, and the
 *                            `HAL` writes into it during the call. Whether the
 *                            implementation retains the pointer beyond the call is not
 *                            specified by this interface, so the caller should keep the
 *                            buffer allocated and unmoved while the `HAL` remains
 *                            initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds one of the two status strings; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The status was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This interface names only `"Enabled"` and `"Disabled"`, so a caller should compare
 *       against both rather than testing for one and inferring the other.
 * @note This reads the driver's operational state, which can differ from the requested
 *       state returned by `wifi_getApEnable()` while a transition is in progress.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                          allocates and releases it, and the `HAL` writes into it during
 *                          the call. Whether the implementation retains the pointer
 *                          beyond the call is not specified by this interface, so the
 *                          caller should keep the variable allocated and unmoved while
 *                          the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the SSID advertisement state; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the internal variable controlling SSID advertisement holds the
 *       requested value. On failure this interface does not specify how much of the
 *       requested change was applied, so a caller should read the affected state back
 *       rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was set.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enable`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getApSsidAdvertisementEnable()` before relying on it.
 *
 * @note Hiding the SSID keeps it out of beacons but does not conceal it from a client that
 *       already knows it, so it is not a security control.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                     `dot11ShortRetryLimit`. The caller allocates and releases it, and
 *                     the `HAL` writes into it during the call. Whether the
 *                     implementation retains the pointer beyond the call is not specified
 *                     by this interface, so the caller should keep the variable allocated
 *                     and unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the current retry limit; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The limit was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point retransmits a packet at most `number` times. On failure
 *       this interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The limit was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it does not state the
 *                          accepted values for `number`, so a caller must not infer a bound
 *                          from the failure; it should report the failure and read the
 *                          setting back with `wifi_getApRetryLimit()` rather than retrying
 *                          with the same argument.
 *
 * @note A high limit trades airtime for reliability, and a limit of 0 disables
 *       retransmission entirely; neither bound is validated by this interface.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                     caller allocates and releases it, and the `HAL` writes into it
 *                     during the call. Whether the implementation retains the pointer
 *                     beyond the call is not specified by this interface, so the caller
 *                     should keep the variable allocated and unmoved while the `HAL`
 *                     remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the WMM enable state; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note `wifi_getAPCapabilities()` reports through `WMMSupported` whether this Access Point
 *       can support WMM at all.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the hardware has WMM in the requested state and beacons reflect it. On
 *       failure this interface does not specify how much of the requested change was
 *       applied, so a caller should read the affected state back rather than assume the
 *       previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enable`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getApWmmEnable()` before relying on it.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                     false when it is not. The caller allocates and releases it, and the
 *                     `HAL` writes into it during the call. Whether the implementation
 *                     retains the pointer beyond the call is not specified by this
 *                     interface, so the caller should keep the variable allocated and
 *                     unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the U-APSD enable state; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note `wifi_getAPCapabilities()` reports through `UAPSDSupported` whether this Access
 *       Point can support U-APSD at all.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      that breaks either pre-condition - a call made before initialisation, or an
 *      attempt to enable U-APSD while WMM is disabled - so a caller must not rely on a
 *      particular status code or on the configuration being left alone.
 * @post On success the hardware has U-APSD in the requested state. On failure this
 *       interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it states no dependency
 *                          between this setting and `wifi_setApWmmEnable()`, so a caller
 *                          must not read this code as meaning WMM is disabled; it should
 *                          read the setting back with `wifi_getApWmmUapsdEnable()` rather
 *                          than repeating the same call.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the hardware applies the requested acknowledgement policy to that
 *       class of service. On failure this interface does not specify how much of the
 *       requested change was applied, and it offers no call that reads the policy back,
 *       so a caller must treat the setting as being in an unknown state.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The policy was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it does not state the
 *                          accepted values for `cla`, so a caller must not infer a bound
 *                          from the failure; it should report the failure rather than
 *                          retrying with the same argument.
 *
 * @note The second parameter is named `cla` rather than `class` because this header is
 *       compiled under `extern "C"` by C++ consumers, where `class` is a reserved word. The
 *       short name is deliberate and must not be "corrected".
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                     false when they are not. The caller allocates and releases it, and
 *                     the `HAL` writes into it during the call. Whether the
 *                     implementation retains the pointer beyond the call is not specified
 *                     by this interface, so the caller should keep the variable allocated
 *                     and unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the isolation state; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success clients on this Access Point are isolated as requested. On failure this
 *       interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enable`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getApIsolationEnable()` before relying on it.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[in] sBeaconRate  Beacon transmission rate, as a string the caller supplies. The
 *                         caller passes `NUL`-terminated text, because this interface
 *                         carries no length parameter for the argument and states no
 *                         maximum length for it. The values this interface names are
 *                         `"1Mbps"`, `"2Mbps"`, `"5.5Mbps"`, `"6Mbps"`, `"11Mbps"`,
 *                         `"12Mbps"` and `"24Mbps"`, and no other string is defined. The
 *                         caller owns the storage and must keep it valid for the duration
 *                         of the call. Whether the implementation retains the pointer
 *                         afterwards is not specified by this interface, so the caller
 *                         should keep it allocated and unmoved while the `HAL` remains
 *                         initialised rather than assuming a lifetime that ends with the
 *                         call.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point transmits beacons at the requested rate. On failure
 *       this interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The rate was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should re-check
 *                          `sBeaconRate` against the strings its parameter entry above names and
 *                          `apIndex` against the indices its parameter entry above names, and should
 *                          read the value back to confirm what took effect.
 *
 * @note Only the rates this interface names may be passed. A lower beacon rate reaches
 *       further but consumes more airtime.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[out] output_BeaconRate  Caller-allocated buffer that receives the current beacon
 *                                rate as one of the strings named by
 *                                `wifi_setApBeaconRate()`. This interface does not state
 *                                whether the value is `NUL`-terminated or how its length
 *                                is conveyed, so a caller must not assume either. It
 *                                states no minimum size for the buffer either: the
 *                                longest value it names, `"5.5Mbps"`, is seven
 *                                characters, an eighth byte would be needed to terminate
 *                                it, and nothing here bounds what the driver may write
 *                                instead, so a caller must not treat eight bytes as a
 *                                safe size and must bound every read by the size it
 *                                allocated. The caller allocates and releases it, and the
 *                                `HAL` writes into it during the call. Whether the
 *                                implementation retains the pointer beyond the call is
 *                                not specified by this interface, so the caller should
 *                                keep the buffer allocated and unmoved while the `HAL`
 *                                remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the beacon rate string; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The rate was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                     imposed. The caller allocates and releases it, and the `HAL` writes
 *                     into it during the call. Whether the implementation retains the
 *                     pointer beyond the call is not specified by this interface, so the
 *                     caller should keep the variable allocated and unmoved while the
 *                     `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the configured maximum, or 0 for no limit; on failure
 *       the output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The maximum was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note A returned 0 means unlimited, not none. Do not treat it as a closed Access Point.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point admits at most `number` simultaneous clients, or an
 *       unlimited number when `number` is 0. On failure this interface does not specify how
 *       much of the requested change was applied, so a caller should read the affected
 *       state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The maximum was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          the value against the range given above and report the failure
 *                          rather than retrying with the same argument.
 *
 * @note This interface does not state what happens to clients already associated when the
 *       new maximum is below the current count, so a caller must not rely on either
 *       outcome.
 * @note A client refused because the Access Point is at capacity is reported through
 *       `wifi_apMaxClientRejection_callback`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success `ModeEnabled`, `WEPKey`, `PreSharedKey` and `KeyPassphrase` hold their
 *       factory defaults for this Access Point. On failure this interface does not specify
 *       how much of the requested change was applied, so a caller should read the affected
 *       state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The security settings were reset.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          settings back with `wifi_getApSecurity()`, because a partial
 *                          reset is not excluded by this interface.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning This replaces the Access Point's credentials with their factory defaults, so a
 *          client holding the previous credentials no longer has the ones this Access
 *          Point expects. What each client then does is outside this interface, which
 *          neither disconnects associated clients nor states what becomes of them; a
 *          caller that needs the previous credentials honoured must restore them with
 *          `wifi_setApSecurity()`.
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
 *                            `"Disabled"`, `"Optional"` or `"Required"`. This interface
 *                            does not state whether the value is `NUL`-terminated or how
 *                            its length is conveyed, so a caller must not assume either:
 *                            treat the buffer as opaque of at most the 64 bytes it is
 *                            required to provide and bound every read by that. The caller
 *                            allocates and releases it, and the `HAL` writes into it
 *                            during the call. Whether the implementation retains the
 *                            pointer beyond the call is not specified by this interface,
 *                            so the caller should keep the buffer allocated and unmoved
 *                            while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds one of the three named strings; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note Only the three named strings are defined, so a caller should compare against all
 *       three rather than testing one and inferring the rest.
 * @note Where `WIFI_HAL_VERSION_3` is defined the same three states are also available as
 *       the `wifi_mfp_cfg_t` enumeration inside `wifi_vap_security_t`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[in] MfpConfig  Management Frame Protection setting to apply, as a string the
 *                       caller supplies, which must be exactly `"Disabled"`, `"Optional"`
 *                       or `"Required"`. The caller passes `NUL`-terminated text, because
 *                       this interface carries no length parameter for the argument and
 *                       states no maximum length for it. The caller owns the storage and
 *                       must keep it valid for the duration of the call. Whether the
 *                       implementation retains the pointer afterwards is not specified by
 *                       this interface, so the caller should keep it allocated and
 *                       unmoved while the `HAL` remains initialised rather than assuming
 *                       a lifetime that ends with the call.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the setting is applied immediately and is stored persistently, so it
 *       is reapplied after a Wi-Fi or VAP restart. On failure this interface does not
 *       specify how much of the requested change was applied, so a caller should read the
 *       affected state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied and persisted.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should re-check
 *                          the string against the values `@param` names, and should read
 *                          the value back to confirm what took effect.
 *
 * @note `"Required"` will prevent clients that do not support protected management frames
 *       from associating at all.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                        server's IP address as text. This interface does not state
 *                        whether the value is `NUL`-terminated or how its length is
 *                        conveyed, so a caller must not assume either: treat the buffer
 *                        as opaque of at most the 64 bytes it is required to provide and
 *                        bound every read by that. The caller allocates and releases it,
 *                        and the `HAL` writes into it during the call. Whether the
 *                        implementation retains the pointer beyond the call is not
 *                        specified by this interface, so the caller should keep the
 *                        buffer allocated and unmoved while the `HAL` remains
 *                        initialised.
 * @param[out] Port_output  Caller-allocated variable that receives the server's UDP port
 *                          number. The caller allocates and releases it, and the `HAL`
 *                          writes into it during the call. Whether the implementation
 *                          retains the pointer beyond the call is not specified by this
 *                          interface, so the caller should keep the variable allocated
 *                          and unmoved while the `HAL` remains initialised.
 * @param[out] RadiusSecret_output  Caller-allocated buffer of at least 64 bytes that
 *                                  receives the shared secret as text. This interface
 *                                  does not state whether the value is `NUL`-terminated
 *                                  or how its length is conveyed, so a caller must not
 *                                  assume either: treat the buffer as opaque of at most
 *                                  the 64 bytes it is required to provide and bound every
 *                                  read by that. The caller allocates and releases it,
 *                                  and the `HAL` writes into it during the call. Whether
 *                                  the implementation retains the pointer beyond the call
 *                                  is not specified by this interface, so the caller
 *                                  should keep the buffer allocated and unmoved while the
 *                                  `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the primary RADIUS server's address, port and secret;
 *       on failure the output is left unspecified, so a caller must not read it unless
 *       the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This interface does not state what the output holds when no server is configured,
 *       so a caller must not treat an empty address string as a defined result unless the
 *       call succeeded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it. This interface does not establish that the implementation copies
 *          the secret during the call, and does not state whether it retains the pointer
 *          afterwards, so the point at which erasing the caller's storage becomes safe is
 *          not established either: the caller must keep that storage valid and unmoved
 *          while the `HAL` remains initialised, and must take any erasure guarantee it
 *          needs from the implementation it runs against.
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
 * @param[in] IPAddress  Primary RADIUS server's IP address, as a string the caller
 *                       supplies. The caller passes `NUL`-terminated text, because this
 *                       interface carries no length parameter for the argument and states
 *                       no maximum length for it; its readback
 *                       `wifi_getApSecurityRadiusServer()` documents a 64-byte output
 *                       buffer, so a value longer than 63 characters could not be read
 *                       back through this interface. The caller owns the storage and must
 *                       keep it valid for the duration of the call. Whether the
 *                       implementation retains the pointer afterwards is not specified by
 *                       this interface, so the caller should keep it allocated and
 *                       unmoved while the `HAL` remains initialised rather than assuming
 *                       a lifetime that ends with the call.
 * @param[in] port  UDP port the server listens on. This interface does not state a
 *                  default, so the caller must supply the deployment's port.
 * @param[in] RadiusSecret  Shared secret used to authenticate with the server, as a
 *                          string the caller supplies and bounded on the same terms as
 *                          `IPAddress` above: the caller passes `NUL`-terminated text,
 *                          because this interface carries no length parameter for the
 *                          argument and states no maximum length for it. The caller owns
 *                          the storage and must keep it valid for the duration of the
 *                          call. Whether the implementation retains the pointer
 *                          afterwards is not specified by this interface, so the caller
 *                          should keep it allocated and unmoved while the `HAL` remains
 *                          initialised rather than assuming a lifetime that ends with the
 *                          call.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point authenticates against the primary RADIUS server at
 *       the given address and port using the given secret. On failure this interface does
 *       not specify how much of the requested change was applied, so a caller should read
 *       the affected state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          the address and secret before retrying, and must not assume a
 *                          partial update was avoided without reading the settings back.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it. This interface does not establish that the implementation copies
 *          the secret during the call, and does not state whether it retains the pointer
 *          afterwards, so the point at which erasing the caller's storage becomes safe is
 *          not established either: the caller must keep that storage valid and unmoved
 *          while the `HAL` remains initialised, and must take any erasure guarantee it
 *          needs from the implementation it runs against.
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
 *                        server's IP address as text. This interface does not state
 *                        whether the value is `NUL`-terminated or how its length is
 *                        conveyed, so a caller must not assume either: treat the buffer
 *                        as opaque of at most the 64 bytes it is required to provide and
 *                        bound every read by that. The caller allocates and releases it,
 *                        and the `HAL` writes into it during the call. Whether the
 *                        implementation retains the pointer beyond the call is not
 *                        specified by this interface, so the caller should keep the
 *                        buffer allocated and unmoved while the `HAL` remains
 *                        initialised.
 * @param[out] Port_output  Caller-allocated variable that receives the server's UDP port
 *                          number. The caller allocates and releases it, and the `HAL`
 *                          writes into it during the call. Whether the implementation
 *                          retains the pointer beyond the call is not specified by this
 *                          interface, so the caller should keep the variable allocated
 *                          and unmoved while the `HAL` remains initialised.
 * @param[out] RadiusSecret_output  Caller-allocated buffer of at least 64 bytes that
 *                                  receives the shared secret as text. This interface
 *                                  does not state whether the value is `NUL`-terminated
 *                                  or how its length is conveyed, so a caller must not
 *                                  assume either: treat the buffer as opaque of at most
 *                                  the 64 bytes it is required to provide and bound every
 *                                  read by that. The caller allocates and releases it,
 *                                  and the `HAL` writes into it during the call. Whether
 *                                  the implementation retains the pointer beyond the call
 *                                  is not specified by this interface, so the caller
 *                                  should keep the buffer allocated and unmoved while the
 *                                  `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the secondary RADIUS server's address, port and
 *       secret; on failure the output is left unspecified, so a caller must not read it
 *       unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This interface does not state what the output holds when no server is configured,
 *       so a caller must not treat an empty address string as a defined result unless the
 *       call succeeded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it. This interface does not establish that the implementation copies
 *          the secret during the call, and does not state whether it retains the pointer
 *          afterwards, so the point at which erasing the caller's storage becomes safe is
 *          not established either: the caller must keep that storage valid and unmoved
 *          while the `HAL` remains initialised, and must take any erasure guarantee it
 *          needs from the implementation it runs against.
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
 * @param[in] IPAddress  Secondary RADIUS server's IP address, as a string the caller
 *                       supplies. The caller passes `NUL`-terminated text, because this
 *                       interface carries no length parameter for the argument and states
 *                       no maximum length for it; its readback
 *                       `wifi_getApSecuritySecondaryRadiusServer()` documents a 64-byte
 *                       output buffer, so a value longer than 63 characters could not be
 *                       read back through this interface. The caller owns the storage and
 *                       must keep it valid for the duration of the call. Whether the
 *                       implementation retains the pointer afterwards is not specified by
 *                       this interface, so the caller should keep it allocated and
 *                       unmoved while the `HAL` remains initialised rather than assuming
 *                       a lifetime that ends with the call.
 * @param[in] port  UDP port the server listens on. This interface does not state a
 *                  default, so the caller must supply the deployment's port.
 * @param[in] RadiusSecret  Shared secret used to authenticate with the server, as a
 *                          string the caller supplies and bounded on the same terms as
 *                          `IPAddress` above: the caller passes `NUL`-terminated text,
 *                          because this interface carries no length parameter for the
 *                          argument and states no maximum length for it. The caller owns
 *                          the storage and must keep it valid for the duration of the
 *                          call. Whether the implementation retains the pointer
 *                          afterwards is not specified by this interface, so the caller
 *                          should keep it allocated and unmoved while the `HAL` remains
 *                          initialised rather than assuming a lifetime that ends with the
 *                          call.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point authenticates against the secondary RADIUS server at
 *       the given address and port using the given secret. On failure this interface does
 *       not specify how much of the requested change was applied, so a caller should read
 *       the affected state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          the address and secret before retrying, and must not assume a
 *                          partial update was avoided without reading the settings back.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it. This interface does not establish that the implementation copies
 *          the secret during the call, and does not state whether it retains the pointer
 *          afterwards, so the point at which erasing the caller's storage becomes safe is
 *          not established either: the caller must keep that storage valid and unmoved
 *          while the `HAL` remains initialised, and must take any erasure guarantee it
 *          needs from the implementation it runs against.
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
 * @param[out] IP_output  Caller-allocated buffer that receives the server's IP address as
 *                        text. This interface does not state whether the value is
 *                        `NUL`-terminated or how its length is conveyed, so a caller must
 *                        not assume either and must bound every read by the size it
 *                        allocated. It states no minimum size for this buffer, and the
 *                        64-byte minimum its siblings `wifi_getApSecurityRadiusServer()`
 *                        and `wifi_getApSecuritySecondaryRadiusServer()` document is
 *                        theirs rather than a rule for this call, so a caller must not
 *                        carry it over. The caller allocates and releases it, and the
 *                        `HAL` writes into it during the call. Whether the implementation
 *                        retains the pointer beyond the call is not specified by this
 *                        interface, so the caller should keep the buffer allocated and
 *                        unmoved while the `HAL` remains initialised.
 * @param[out] Port_output  Caller-allocated variable that receives the server's UDP port
 *                          number. The caller allocates and releases it, and the `HAL`
 *                          writes into it during the call. Whether the implementation
 *                          retains the pointer beyond the call is not specified by this
 *                          interface, so the caller should keep the variable allocated
 *                          and unmoved while the `HAL` remains initialised.
 * @param[out] RadiusdasSecret_output  Caller-allocated buffer that receives the shared
 *                                     secret as text, on the same terms as `IP_output`
 *                                     above: this interface states neither a minimum size
 *                                     for it, nor whether the value is `NUL`-terminated,
 *                                     nor how its length is conveyed, so a caller must
 *                                     bound every read by the size it allocated. The
 *                                     caller allocates and releases it, and the `HAL`
 *                                     writes into it during the call. Whether the
 *                                     implementation retains the pointer beyond the call
 *                                     is not specified by this interface, so the caller
 *                                     should keep the buffer allocated and unmoved while
 *                                     the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the RADIUS Dynamic Authorization Server server's
 *       address, port and secret; on failure the output is left unspecified, so a caller
 *       must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This interface does not state what the output holds when no server is configured,
 *       so a caller must not treat an empty address string as a defined result unless the
 *       call succeeded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it. This interface does not establish that the implementation copies
 *          the secret during the call, and does not state whether it retains the pointer
 *          afterwards, so the point at which erasing the caller's storage becomes safe is
 *          not established either: the caller must keep that storage valid and unmoved
 *          while the `HAL` remains initialised, and must take any erasure guarantee it
 *          needs from the implementation it runs against.
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
 * @param[in] IPAddress  RADIUS Dynamic Authorization Server server's IP address, as a
 *                       string the caller supplies. The caller passes `NUL`-terminated
 *                       text, because this interface carries no length parameter for the
 *                       argument and states no maximum length for it; the 64-byte output
 *                       buffers its siblings document are theirs, and neither is a bound
 *                       on this argument. The caller owns the storage and must keep it
 *                       valid for the duration of the call. Whether the implementation
 *                       retains the pointer afterwards is not specified by this
 *                       interface, so the caller should keep it allocated and unmoved
 *                       while the `HAL` remains initialised rather than assuming a
 *                       lifetime that ends with the call.
 * @param[in] port  UDP port the server listens on. This interface does not state a
 *                  default, so the caller must supply the deployment's port.
 * @param[in] RadiusdasSecret  Shared secret used to authenticate with the server, as a
 *                             string the caller supplies and bounded on the same terms as
 *                             `IPAddress` above: `NUL`-terminated text, with no length
 *                             parameter and no maximum length stated by this interface.
 *                             The caller owns the storage and must keep it valid for the
 *                             duration of the call. Whether the implementation retains
 *                             the pointer afterwards is not specified by this interface,
 *                             so the caller should keep it allocated and unmoved while
 *                             the `HAL` remains initialised rather than assuming a
 *                             lifetime that ends with the call.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point authenticates against the RADIUS Dynamic
 *       Authorization Server server at the given address and port using the given secret.
 *       On failure this interface does not specify how much of the requested change was
 *       applied, so a caller should read the affected state back rather than assume the
 *       previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          the address and secret before retrying, and must not assume a
 *                          partial update was avoided without reading the settings back.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The RADIUS secret is a credential. A caller that logs or traces these arguments
 *          must redact it. This interface does not establish that the implementation copies
 *          the secret during the call, and does not state whether it retains the pointer
 *          afterwards, so the point at which erasing the caller's storage becomes safe is
 *          not established either: the caller must keep that storage valid and unmoved
 *          while the `HAL` remains initialised, and must take any erasure guarantee it
 *          needs from the implementation it runs against.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success greylist access control is in the requested state on every applicable
 *       Virtual Access Point. On failure this interface does not specify how much of the
 *       requested change was applied, so a caller must treat the setting as unknown on
 *       every applicable Virtual Access Point.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied to all applicable Virtual Access
 *                          Points.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and because the call is
 *                          device-wide and this interface does not state that it is atomic,
 *                          it does not establish how much of the requested change took
 *                          effect: a caller must not assume either that every Virtual
 *                          Access Point was changed or that none was, and must treat the
 *                          setting as unknown on every applicable Virtual Access Point.
 *
 * @note This call takes no Access Point index: it applies device-wide, unlike every other
 *       access-control call in this header.
 * @note This interface declares no call that reads this device-wide setting back. The
 *       nearest observable value is the `network_initiated_greylist` member of
 *       `wifi_front_haul_bss_t`, reported per Virtual Access Point by
 *       `wifi_getRadioVapInfoMap()`, but this interface does not state whether this call
 *       updates that member, so a caller cannot use it to confirm the outcome.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                     releases it, and the `HAL` writes into it during the call. Whether
 *                     the implementation retains the pointer beyond the call is not
 *                     specified by this interface, so the caller should keep the
 *                     structure allocated and unmoved while the `HAL` remains
 *                     initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds a full set of RADIUS parameters; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This reports the RADIUS behaviour parameters -- retries, timeouts, PMK lifetime and
 *       caching, blacklisting -- and neither the server address nor the shared secret:
 *       `wifi_radius_setting_t` declares no member for either, so neither can reach the
 *       caller through this call. `wifi_getApSecurityRadiusServer()` reports the address
 *       in `IP_output`, the port in `Port_output` and the secret in `RadiusSecret_output`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                   structure and must keep it valid for the duration of the call.
 *                   Whether the implementation retains the pointer afterwards is not
 *                   specified by this interface, so the caller should keep it allocated
 *                   and unmoved while the `HAL` remains initialised. Every member is
 *                   applied, so a caller should read the current settings first and
 *                   modify them rather than passing a partially filled structure.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success all the RADIUS parameters in the structure are in force. On failure this
 *       interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The settings were applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          settings back to establish which values took effect, since this
 *                          interface does not state that the update is atomic.
 *
 * @note Setting `MaxAuthenticationAttempts` to 0 disables client blacklisting, and 0 in
 *       `IdentityRequestRetryInterval` or `QuietPeriodAfterFailedAuthentication` disables
 *       that behaviour, as the structure's own member documentation states.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                            `"Configured"`. This interface does not state whether the
 *                            value is `NUL`-terminated or how its length is conveyed, so
 *                            a caller must not assume either: treat the buffer as opaque
 *                            of at most the 32 bytes it is required to provide and bound
 *                            every read by that. The caller allocates and releases it,
 *                            and the `HAL` writes into it during the call. Whether the
 *                            implementation retains the pointer beyond the call is not
 *                            specified by this interface, so the caller should keep the
 *                            buffer allocated and unmoved while the `HAL` remains
 *                            initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds one of the two named strings; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note Only the two named strings are defined by this interface.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[in] pin  WPS enrollee PIN, as a string the caller supplies. The caller passes
 *                 `NUL`-terminated text, because this interface carries no length
 *                 parameter for the argument and states no maximum length for it; the
 *                 `WIFI_AP_MAX_WPSPIN_LEN` bound of 9 is declared for the `pin` member of
 *                 `wifi_wps_t`, not for this argument. The caller owns the storage and
 *                 must keep it valid for the duration of the call. Whether the
 *                 implementation retains the pointer afterwards is not specified by this
 *                 interface, so the caller should keep it allocated and unmoved while the
 *                 `HAL` remains initialised rather than assuming a lifetime that ends
 *                 with the call.
 *
 * @pre `wifi_init()` must have completed successfully, and a handler should already be
 *      installed with `wifi_wpsEvent_callback_register()` or the session's outcome will
 *      not be reported; see `Initialization and Startup` in the HAL specification. This
 *      interface does not specify the outcome of a call made before initialisation:
 *      neither the status code nor whether a session is started is established, so a
 *      caller must not rely on either.
 * @post On success a PIN-method WPS session is armed with the given PIN, and its outcome
 *       is delivered to the registered WPS event handler. On failure this interface does
 *       not specify whether a session was armed, and it offers no call that reports
 *       whether a session is in progress, so a caller must treat the session state as
 *       unknown; any outcome that does follow arrives through the registered WPS event
 *       handler.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The PIN was accepted and the session armed.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          the PIN and confirm WPS is enabled with
 *                          `wifi_getApWpsConfiguration()` before retrying.
 *
 * @note A PIN session expires on its own, reported as `wifi_wps_ev_pin_timeout`; use
 *       `wifi_cancelApWPS()` to end it early.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      not be reported; see `Initialization and Startup` in the HAL specification. This
 *      interface does not specify the outcome of a call made before initialisation:
 *      neither the status code nor whether a session is started is established, so a
 *      caller must not rely on either.
 * @post On success a push-button WPS session is started, exactly as a physical button
 *       press would start one, and its outcome is delivered to the registered WPS event
 *       handler. On failure this interface does not specify whether a session was started,
 *       and it offers no call that reports whether a session is in progress, so a caller
 *       must treat the session state as unknown; any outcome that does follow arrives
 *       through the registered WPS event handler.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The push-button session was started.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should confirm WPS
 *                          is enabled with `wifi_getApWpsConfiguration()` rather than
 *                          retrying immediately.
 *
 * @note The session ends by itself: success is `wifi_wps_ev_success`, expiry of the walk
 *       time is `wifi_wps_ev_pbc_timeout`, and a competing session is
 *       `wifi_wps_ev_pbc_overlap`. Only the event handler distinguishes these -- this
 *       call's success means the session started, not that a client was onboarded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success no WPS session is active on this Access Point. On failure this
 *       interface does not specify whether a session was cancelled, and it offers no call
 *       that reports whether a session is in progress, so a caller must treat the session
 *       state as unknown.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS WPS mode was cancelled.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should not assume
 *                          a session is still running, and should wait for the WPS event
 *                          handler rather than polling.
 *
 * @note Cancelling a session that is not running is not defined by this interface as either
 *       success or failure, so a caller should not use the return value to test whether a
 *       session was active.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                         transmit power in dBm. The caller allocates and releases it,
 *                         and the `HAL` writes into it during the call. Whether the
 *                         implementation retains the pointer beyond the call is not
 *                         specified by this interface, so the caller should keep the
 *                         variable allocated and unmoved while the `HAL` remains
 *                         initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the current management frame power; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The value was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success management frames are transmitted at the requested power. On failure
 *       this interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The value was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it does not state the
 *                          accepted values for `dBm`, so a caller must not infer a bound
 *                          from the failure; it should report the failure and read the
 *                          setting back with `wifi_getApManagementFramePowerControl()`
 *                          rather than retrying with the same argument.
 *
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note `associated_dev` is owned by the `HAL` and this interface states no lifetime for it
 *       beyond this call, so the handler must copy whatever it needs before returning
 *       and must neither retain nor release it, per `Asynchronous Notification
 *       Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
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
    BOOL cli_Valid;             /*!< True when the rest of this entry is populated; false means the link slot is unused. */
    BOOL cli_IsAssocLink;       /*!< True on the single link that carried the association exchange. */
    UCHAR cli_LinkID;           /*!< IEEE 802.11be link identifier advertised for this link. */
    UINT cli_VapIndex;          /*!< Virtual Access Point this link of the multi-link device is
                                     established on. This interface does not state whether the value
                                     is the `apIndex` its own calls take or the
                                     `wifi_vap_info_t::vap_index` of the same `VAP`, so a caller
                                     must not pass it into an `apIndex` argument without checking
                                     the platform convention. */
    INT cli_RSSI;               /*!< Received signal strength for this link, in dBm. */
    UCHAR cli_LinkAddress[6];   /*!< MAC address the client uses on this link, which differs from its MLD address. */
} wifi_mld_sta_link_info_t;

/**
 * @brief Multi-link capability and per-link state of an associated client.
 *
 * Reported as part of `wifi_associated_dev_t`. When `cli_MLDSta` is false the client
 * is a conventional single-link station and the remaining members carry no meaning.
 */
typedef struct {
    BOOL cli_MLDSta;                        /*!< True when the client is a Wi-Fi 7 multi-link device; false for a single-link station. */
    wifi_multi_link_modes_t cli_MLModeCapa; /*!< Modes the client advertised, as a bitmask of
                                                 `wifi_multi_link_modes_t` values, so several may be
                                                 set at once. It is meaningful only where
                                                 `cli_MLDSta` above is `TRUE`, and it records what
                                                 the client claimed rather than what the link is
                                                 using. */
    BOOL cli_TIDLinkMapNegotiation;         /*!< True when the client negotiated a traffic-identifier-to-link mapping rather than using the default. */
    wifi_mld_sta_link_info_t cli_LinkInfo[MAX_NUM_RADIOS]; /*!< Per-link state, one entry per radio; read only the entries whose `cli_Valid` is true. */
} wifi_mld_sta_info_t;

/**
 * @brief Structure containing information about an associated device.
 */
typedef struct _wifi_associated_dev
{
    UCHAR cli_MACAddress[6];             /*!< Address of the associated device, in six raw octets.
                                              It is not text and carries no terminator; where the
                                              device is a multi-link station this is its MLD address
                                              rather than a per-link one, and
                                              `wifi_mld_sta_link_info_t::cli_LinkAddress` carries
                                              the per-link address. */
    CHAR cli_IPAddress[64];              /*!< Address the device holds, as text in 64 bytes. This
                                              interface states neither the family nor the textual
                                              form, nor whether the value is NUL-terminated, and
                                              does not say what the field holds where no address is
                                              known, so a caller must bound any read at 64 bytes and
                                              must not parse the content for structure. */
    BOOL cli_AuthenticationState;        /*!< `TRUE` where the device has completed authentication
                                              with the `AP`. This interface does not state which
                                              exchange that covers, so a caller must not read the
                                              member as meaning a particular key is installed. */
    UINT cli_LastDataDownlinkRate;      /**< The data transmit rate in kbps that was most recently used for transmission from the access point to the associated device. */
    UINT cli_LastDataUplinkRate;        /**< The data transmit rate in kbps that was most recently used for transmission from the associated device to the access point. */
    INT cli_SignalStrength;              /**< An indicator of radio signal strength of the uplink from the associated device to the access point, measured in dBm, as an average of the last 100 packets received from the device. */
    UINT cli_Retransmissions;            /**< The number of packets that had to be re-transmitted, from the last 100 packets sent to the associated device. Multiple re-transmissions of the same packet count as one. */
    BOOL cli_Active;                    /**< Whether or not this node is currently present in the WiFi AccessPoint network. */
    CHAR cli_OperatingStandard[64];     /*!< 802.11 standard the client is operating under, as text
                                             in 64 bytes. This interface enumerates no accepted
                                             strings and does not state whether the value is
                                             `NUL`-terminated, so a caller must bound any read at 64
                                             bytes and should treat the content as opaque. */
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
    UINT cli_CapableNumSpatialStreams;  /*!< Spatial streams the client is capable of. It is the
                                             client's capability rather than what is in use, which
                                             `cli_activeNumSpatialStreams` of
                                             `wifi_associated_dev3_t` reports, and this interface
                                             states no accepted range for either. */
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
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note `associated_dev` is owned by the `HAL` and this interface states no lifetime for it
 *       beyond this call, so the handler must copy whatever it needs before returning
 *       and must neither retain nor release it, per `Asynchronous Notification
 *       Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
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
 * the event. This is the association half of the notification pair whose other
 * half is `wifi_apDisassociatedDevice_callback_register()`. This interface does not
 * state whether registering a second handler replaces the first, adds to it or is
 * rejected, so a caller must not depend on any of those outcomes.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_newApAssociatedDevice_callback`. The `HAL` keeps
 *                           this function pointer after the call returns, since it
 *                           invokes the handler later, so the function must remain
 *                           callable for as long as notifications are wanted. This
 *                           interface declares no call that removes a handler and
 *                           states no end to a registration, so it does not
 *                           establish when the `HAL` stops using the pointer. The
 *                           effect of passing NULL is not specified by this
 *                           interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent association.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes on each
 *             client association, and keeps that function pointer after this call
 *             returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_newApAssociatedDevice_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The associated-device structure reaching the handler is storage the `HAL` owns,
 *          and `wifi_newApAssociatedDevice_callback` states no lifetime for it beyond that
 *          call, so the handler must copy whatever it needs before returning and must
 *          neither retain nor release it, per `Asynchronous Notification Model` in
 *          the HAL specification.
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
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note `MAC` is a bare `char *` the `HAL` supplies. This interface does not state the
 *       representation of the bytes, how many of them are readable, which component owns
 *       the storage, how long it stays valid, or whether a handler may release it.
 *       `Asynchronous Notification Model` in the HAL specification makes the copy the
 *       client's responsibility during the callback, which is the only statement here
 *       about the data's availability, so a handler that needs the address must copy it
 *       before returning. With no extent stated it cannot bound that copy from this
 *       interface: it must not run an unbounded string function over the pointer, and must
 *       take the length from the platform's implementer. It should release nothing and
 *       retain the pointer nowhere.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
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
 * @param[in] src_mac     Address of the disassociated device. The parameter is a bare
 *                        `char *` and this interface states neither the representation of
 *                        the bytes - printable text or raw octets - nor whether they are
 *                        `NUL`-terminated, nor how many are readable.
 * @param[in] dest_mac    Address of the Access Point, under the same open questions as
 *                        `src_mac`: representation, termination and readable extent are
 *                        all unspecified here.
 * @param[in] frame_type  Management frame type that carried the disassociation.
 * @param[in] event_type  Cause of the disassociation: explicit, or the result of
 *                        client inactivity.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 * @execution callback
 * @sideeffect Whatever the handler the caller implements does; this interface
 *             establishes no effect of its own for this type.
 *
 * @note The handler must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. It should hand the event to its own
 *       task and return.
 * @note `src_mac` and `dest_mac` are bare `char *` pointers the `HAL` supplies. This
 *       interface does not state the representation of the bytes, how many of them are
 *       readable, which component owns the storage, how long it stays valid, or whether a
 *       handler may release it. `Asynchronous Notification Model` in
 *       the HAL specification makes the copy the client's responsibility during the
 *       callback, which is the only statement here about the data's availability, so a
 *       handler that needs either address must copy it before returning. With no extent
 *       stated it cannot bound that copy from this interface: it must not run an unbounded
 *       string function over either pointer, and must take the length from the platform's
 *       implementer. It should release nothing and retain neither pointer.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in the HAL specification.
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
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note `mac` is a bare `char *` the `HAL` supplies. This interface does not state the
 *       representation of the bytes, how many of them are readable, which component owns
 *       the storage, how long it stays valid, or whether a handler may release it.
 *       `Asynchronous Notification Model` in the HAL specification makes the copy the
 *       client's responsibility during the callback, which is the only statement here
 *       about the data's availability, so a handler that needs the address must copy it
 *       before returning. With no extent stated it cannot bound that copy from this
 *       interface: it must not run an unbounded string function over the pointer, and must
 *       take the length from the platform's implementer. It should release nothing and
 *       retain the pointer nowhere.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 * @execution callback
 * @sideeffect Whatever the handler the caller implements does; this interface
 *             establishes no effect of its own for this type.
 *
 * @note The handler must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. It should hand the event to its own
 *       task and return.
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
 * @param[in] mac      Address of the client. The parameter is a bare `char *` and this
 *                     interface states neither the representation of the bytes - printable
 *                     text or raw octets - nor whether they are `NUL`-terminated, nor how
 *                     many are readable, nor what storage they sit in.
 * @param[in] status   Outcome of the handshake. This interface does not enumerate the
 *                     values, so a caller must not assume they match
 *                     `wifi_status_code_t` or `eapol_msg_type_t`.
 *
 * @returns The status of the operation, which the implementer of the handler returns
 *          to the `HAL`.
 * @retval RETURN_OK  The handler accepted the notification.
 * @retval RETURN_ERR The handler could not process the notification.
 *
 * @execution callback
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 * @note This function must not suspend and must not invoke any blocking system calls. It
 *       should just send a message to a driver event handler task.
 * @note `Asynchronous Notification Model` in the HAL specification makes the copy the
 *       client's responsibility during the callback, so a handler that needs `mac` must
 *       copy it before returning. That is the only statement this interface makes about the
 *       data's availability: it does not say which component owns the storage, how long it
 *       stays valid, or whether a handler may release it, so the handler should release
 *       nothing and retain the pointer nowhere. With no extent stated it cannot bound the
 *       copy from this interface either - it must not run an unbounded string function over
 *       the pointer, and must take the length from the platform's implementer.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in the HAL specification.
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
 * @param[in] mac      Address of the client. The parameter is a bare `char *` and this
 *                     interface states neither the representation of the bytes - printable
 *                     text or raw octets - nor whether they are `NUL`-terminated, nor how
 *                     many are readable, nor what storage they sit in.
 * @param[in] type     Which EAPOL exchange timed out; see `eapol_msg_type_t` for the
 *                     handshake messages this interface names.
 *
 * @returns The status of the operation, which the implementer of the handler returns
 *          to the `HAL`.
 * @retval RETURN_OK  The handler accepted the notification.
 * @retval RETURN_ERR The handler could not process the notification.
 *
 * @execution callback
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 * @note This interface declares this callback type but no registration function for it, so
 *       there is currently no way for a caller to install one. It is documented here
 *       because it is part of the published type surface.
 * @note This function must not suspend and must not invoke any blocking system calls.
 * @note `Asynchronous Notification Model` in the HAL specification makes the copy the
 *       client's responsibility during the callback, so a handler that needs `mac` must
 *       copy it before returning. That is the only statement this interface makes about the
 *       data's availability: it does not say which component owns the storage, how long it
 *       stays valid, or whether a handler may release it, so the handler should release
 *       nothing and retain the pointer nowhere. With no extent stated it cannot bound the
 *       copy from this interface either - it must not run an unbounded string function over
 *       the pointer, and must take the length from the platform's implementer.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in the HAL specification.
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
 * @param[in] MAC      Address of the rejected client. The parameter is a bare `char *`
 *                     and this interface states neither the representation of the bytes -
 *                     printable text or raw octets - nor whether they are
 *                     `NUL`-terminated, nor how many are readable, nor what storage they
 *                     sit in.
 * @param[in] reason   IEEE 802.11 association status code carried by the rejecting
 *                     response. Reason 17 is the association-denied-because-the-AP-is-
 *                     at-capacity case this callback exists to report.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 * @execution callback
 * @sideeffect Whatever the handler the caller implements does; this interface
 *             establishes no effect of its own for this type.
 *
 * @note The handler must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. It should hand the event to its own
 *       task and return.
 * @note `Asynchronous Notification Model` in the HAL specification makes the copy the
 *       client's responsibility during the callback, so a handler that needs `MAC` must
 *       copy it before returning. That is the only statement this interface makes about the
 *       data's availability: it does not say which component owns the storage, how long it
 *       stays valid, or whether a handler may release it, so the handler should release
 *       nothing and retain the pointer nowhere. With no extent stated it cannot bound the
 *       copy from this interface either - it must not run an unbounded string function over
 *       the pointer, and must take the length from the platform's implementer.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in the HAL specification.
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
 * @param[in] src_mac     Address of the client device. The parameter is a bare `char *`
 *                        and this interface states neither the representation of the bytes
 *                        - printable text or raw octets - nor whether they are
 *                        `NUL`-terminated, nor how many are readable.
 * @param[in] dest_mac    Address of the Access Point, under the same open questions as
 *                        `src_mac`: representation, termination and readable extent are
 *                        all unspecified here.
 * @param[in] frame_type  Management frame type the status code was carried in.
 * @param[in] status      IEEE 802.11 status code from the response frame; the
 *                        values this interface names are in `wifi_status_code_t`.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @execution callback
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 * @note This function must not suspend and must not invoke any blocking system calls.
 * @note `src_mac` and `dest_mac` are bare `char *` pointers the `HAL` supplies. This
 *       interface does not state the representation of the bytes, how many of them are
 *       readable, which component owns the storage, how long it stays valid, or whether a
 *       handler may release it. `Asynchronous Notification Model` in
 *       the HAL specification makes the copy the client's responsibility during the
 *       callback, which is the only statement here about the data's availability, so a
 *       handler that needs either address must copy it before returning. With no extent
 *       stated it cannot bound that copy from this interface: it must not run an unbounded
 *       string function over either pointer, and must take the length from the platform's
 *       implementer. It should release nothing and retain neither pointer.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in the HAL specification.
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
 * @param[in] sta_mac         MAC address of the station whose authentication failed:
 *                            six octets, delivered by the `HAL`.
 *                            `mac_address_t` is an array typedef, so the parameter
 *                            adjusts to a pointer and the handler receives the address
 *                            of storage the `HAL` owns, not a copy of it. The handler
 *                            must treat the six octets as read-only, must copy them if
 *                            it needs them after returning, and must not retain the
 *                            pointer or release it; this interface does not specify how
 *                            long the storage remains valid once the handler returns.
 *                            See `Asynchronous Notification Model` in
 *                            the HAL specification.
 * @param[in] failure_reason  Reason for the failure. The values this interface names
 *                            are in `wifi_eap_status_code_t`.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @execution callback
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 * @note This function must not suspend and must not invoke any blocking system calls.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in the HAL specification.
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
 * authentication server from one that never attempted to join. This interface does
 * not state whether registering a second handler replaces the first, adds to it or
 * is rejected, so a caller must not depend on any of those outcomes.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_radiusEapFailure_callback`. The `HAL` keeps this
 *                           function pointer after the call returns, since it
 *                           invokes the handler later, so the function must remain
 *                           callable for as long as notifications are wanted. This
 *                           interface declares no call that removes a handler and
 *                           states no end to a registration, so it does not
 *                           establish when the `HAL` stops using the pointer. The
 *                           effect of passing NULL is not specified by this
 *                           interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent failure.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes on each
 *             RADIUS or EAP authentication failure, and keeps that function pointer
 *             after this call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_radiusEapFailure_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * handler. This interface does not state whether registering a second handler
 * replaces the first, adds to it or is rejected, so a caller must not depend on any
 * of those outcomes.
 *
 * @param[in] callback_proc  Handler to install, of type `wifi_apStatusCode_callback`.
 *                           The `HAL` keeps this function pointer after the call
 *                           returns, since it invokes the handler later, so the
 *                           function must remain callable for as long as
 *                           notifications are wanted. This interface declares no
 *                           call that removes a handler and states no end to a
 *                           registration, so it does not establish when the `HAL`
 *                           stops using the pointer. The effect of passing NULL is
 *                           not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent status-code event.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes for each
 *             management-frame status code, and keeps that function pointer after
 *             this call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_apStatusCode_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning This interface does not state which component owns the storage the addresses
 *          reaching the handler sit in, how many bytes of either are readable, or how long
 *          they stay valid; `wifi_apStatusCode_callback` records the same position. What
 *          `Asynchronous Notification Model` in the HAL specification establishes is that
 *          the copy is the client's responsibility during the callback, so a handler that
 *          needs an address must copy it before returning, must bound that copy by a length
 *          it obtained from the platform's implementer rather than by an unbounded string
 *          function, and must neither retain nor release the pointer.
 *
 * @see wifi_apStatusCode_callback
 */
void wifi_apStatusCode_callback_register(wifi_apStatusCode_callback callback_proc);
/**
 * @brief Installs the caller's handler for station-mode association events.
 *
 * After registration the `HAL` reports each association request, reassociation
 * request and EAPOL frame through the supplied handler, together with the key
 * management, radio and security mode the station used. This interface does not
 * state whether registering a second handler replaces the first, adds to it or is
 * rejected, so a caller must not depend on any of those outcomes.
 *
 * @param[in] callback_proc  Handler to install, of type `wifi_stamode_callback`.
 *                           The `HAL` keeps this function pointer after the call
 *                           returns, since it invokes the handler later, so the
 *                           function must remain callable for as long as
 *                           notifications are wanted. This interface declares no
 *                           call that removes a handler and states no end to a
 *                           registration, so it does not establish when the `HAL`
 *                           stops using the pointer. The effect of passing NULL is
 *                           not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent station-mode event.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes on each
 *             station-mode event, and keeps that function pointer after this call
 *             returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_stamode_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning This interface does not state which component owns the storage `mac` points at,
 *          how many bytes of it are readable, or how long it stays valid;
 *          `wifi_stamode_callback` records the same position. What `Asynchronous
 *          Notification Model` in the HAL specification establishes is that the copy is
 *          the client's responsibility during the callback, so a handler that needs the
 *          address must copy it before returning, must bound that copy by a length it
 *          obtained from the platform's implementer rather than by an unbounded string
 *          function, and must neither retain nor release the pointer.
 *
 * @see wifi_stamode_callback
 */
void wifi_ap_stamode_callback_register(wifi_stamode_callback callback_proc);

/**
 * @brief Installs the caller's handler for four-way handshake outcomes.
 *
 * After registration the `HAL` reports the outcome of each WPA/WPA2/WPA3 four-way
 * handshake through the supplied handler, which lets a caller distinguish a client
 * that failed key negotiation from one that never associated. This interface does
 * not state whether registering a second handler replaces the first, adds to it or
 * is rejected, so a caller must not depend on any of those outcomes.
 *
 * @param[in] callback_proc  Handler to install, of type `wifi_handshake_callback`.
 *                           The `HAL` keeps this function pointer after the call
 *                           returns, since it invokes the handler later, so the
 *                           function must remain callable for as long as
 *                           notifications are wanted. This interface declares no
 *                           call that removes a handler and states no end to a
 *                           registration, so it does not establish when the `HAL`
 *                           stops using the pointer. The effect of passing NULL is
 *                           not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent handshake outcome.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes on each
 *             four-way handshake outcome, and keeps that function pointer after this
 *             call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_handshake_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning This interface does not state which component owns the storage `mac` points at,
 *          how many bytes of it are readable, or how long it stays valid;
 *          `wifi_handshake_callback` records the same position. What `Asynchronous
 *          Notification Model` in the HAL specification establishes is that the copy is
 *          the client's responsibility during the callback, so a handler that needs the
 *          address must copy it before returning, must bound that copy by a length it
 *          obtained from the platform's implementer rather than by an unbounded string
 *          function, and must neither retain nor release the pointer.
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
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * In current implementation return value is WIFI_HAL_SUCCESS and any failure of the operation
 * is updated in the failure_reason code.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
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
 * This interface does not state whether registering a second handler replaces the
 * first, adds to it or is rejected, so a caller must not depend on any of those
 * outcomes.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_radiusFallback_failover_callback`. The `HAL` keeps
 *                           this function pointer after the call returns, since it
 *                           invokes the handler later, so the function must remain
 *                           callable for as long as notifications are wanted. This
 *                           interface declares no call that removes a handler and
 *                           states no end to a registration, so it does not
 *                           establish when the `HAL` stops using the pointer. The
 *                           effect of passing NULL is not specified by this
 *                           interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent fallback or
 *       failover event.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes on each
 *             RADIUS fallback or failover event, and keeps that function pointer
 *             after this call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_radiusFallback_failover_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * inactivity. This is the disassociation half of the notification pair whose other
 * half is `wifi_newApAssociatedDevice_callback_register()`. This interface does not
 * state whether registering a second handler replaces the first, adds to it or is
 * rejected, so a caller must not depend on any of those outcomes.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_apDisassociatedDevice_callback`. The `HAL` keeps
 *                           this function pointer after the call returns, since it
 *                           invokes the handler later, so the function must remain
 *                           callable for as long as notifications are wanted. This
 *                           interface declares no call that removes a handler and
 *                           states no end to a registration, so it does not
 *                           establish when the `HAL` stops using the pointer. The
 *                           effect of passing NULL is not specified by this
 *                           interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent disassociation.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes on each
 *             client disassociation, and keeps that function pointer after this call
 *             returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_apDisassociatedDevice_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning This interface does not state which component owns the storage `MAC` points at,
 *          how many bytes of it are readable, or how long it stays valid;
 *          `wifi_apDisassociatedDevice_callback` records the same position. What
 *          `Asynchronous Notification Model` in the HAL specification establishes is that
 *          the copy is the client's responsibility during the callback, so a handler that
 *          needs the address must copy it before returning, must bound that copy by a
 *          length it obtained from the platform's implementer rather than by an unbounded
 *          string function, and must neither retain nor release the pointer.
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
 * This callback function is invoked when a deauthentication event with reason code 2 is
 * received from a client device. IEEE Std 802.11-2020 Table 9-49 gives that code the
 * meaning "previous authentication no longer valid"; it does not identify a cause, so a
 * handler must not read one such as a mistyped credential into it. The handler also
 * receives `reason`, and this interface does not state whether any other code is ever
 * reported here, so a handler must read that argument rather than assume the value 2.
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] ap_index  Index of the Access Point.
 * @param[in] mac       MAC address of the client device.
 * @param[in] reason    Reason code for the deauthentication event.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note `mac` is a bare `char *` the `HAL` supplies. This interface does not state the
 *       representation of the bytes, how many of them are readable, which component owns
 *       the storage, how long it stays valid, or whether a handler may release it.
 *       `Asynchronous Notification Model` in the HAL specification makes the copy the
 *       client's responsibility during the callback, which is the only statement here
 *       about the data's availability, so a handler that needs the address must copy it
 *       before returning. With no extent stated it cannot bound that copy from this
 *       interface: it must not run an unbounded string function over the pointer, and must
 *       take the length from the platform's implementer. It should release nothing and
 *       retain the pointer nowhere.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 */
typedef INT ( * wifi_apDeAuthEvent_callback)(int ap_index, char *mac, int reason);

/**
 * @brief Callback function invoked when a client deauthenticates.
 *
 * This callback reports a deauthentication observed by the Access Point, carrying
 * both endpoints of the exchange and the IEEE 802.11 reason code, so a caller can
 * tell a deliberate disconnect from one forced by the network.
 *
 * @param[in] ap_index    Index of the Access Point that observed the event, in the
 *                        range `AP_INDEX_1` to `AP_INDEX_24`.
 * @param[in] src_mac     Address of the client device. The parameter is a bare `char *`
 *                        and this interface states neither the representation of the bytes
 *                        - printable text or raw octets - nor whether they are
 *                        `NUL`-terminated, nor how many are readable.
 * @param[in] dest_mac    Address of the Access Point, under the same open questions as
 *                        `src_mac`: representation, termination and readable extent are
 *                        all unspecified here.
 * @param[in] frame_type  Management frame type that carried the deauthentication.
 * @param[in] reason      IEEE 802.11 reason code. The values this interface names
 *                        are in `wifi_reason_code_t`.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 * @execution callback
 * @sideeffect Whatever the handler the caller implements does; this interface
 *             establishes no effect of its own for this type.
 *
 * @note The handler must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. It should hand the event to its own
 *       task and return.
 * @note `src_mac` and `dest_mac` are bare `char *` pointers the `HAL` supplies. This
 *       interface does not state the representation of the bytes, how many of them are
 *       readable, which component owns the storage, how long it stays valid, or whether a
 *       handler may release it. `Asynchronous Notification Model` in
 *       the HAL specification makes the copy the client's responsibility during the
 *       callback, which is the only statement here about the data's availability, so a
 *       handler that needs either address must copy it before returning. With no extent
 *       stated it cannot bound that copy from this interface: it must not run an unbounded
 *       string function over either pointer, and must take the length from the platform's
 *       implementer. It should release nothing and retain neither pointer.
 * @note The `HAL` is expected to be thread safe, so the handler may be entered on a `HAL`
 *       thread; see `Threading Model` in the HAL specification.
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
 * handler with its IEEE 802.11 reason code, which a caller can act on rather than
 * inferring the event from a link going away. `wifi_apDeAuthEvent_callback` records what
 * the reported code does and does not establish. This interface does not state whether
 * registering a second handler replaces the first, adds to it or is rejected, so a caller
 * must not depend on any of those outcomes.
 *
 * @param[in] callback_proc  Handler to install, of type
 *                           `wifi_apDeAuthEvent_callback`. The `HAL` keeps this
 *                           function pointer after the call returns, since it
 *                           invokes the handler later, so the function must remain
 *                           callable for as long as notifications are wanted. This
 *                           interface declares no call that removes a handler and
 *                           states no end to a registration, so it does not
 *                           establish when the `HAL` stops using the pointer. The
 *                           effect of passing NULL is not specified by this
 *                           interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed and is invoked on each subsequent deauthentication.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes on each
 *             client deauthentication, and keeps that function pointer after this
 *             call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_apDeAuthEvent_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning This interface does not state which component owns the storage `mac` points at,
 *          how many bytes of it are readable, or how long it stays valid;
 *          `wifi_apDeAuthEvent_callback` records the same position. What `Asynchronous
 *          Notification Model` in the HAL specification establishes is that the copy is
 *          the client's responsibility during the callback, so a handler that needs the
 *          address must copy it before returning, must bound that copy by a length it
 *          obtained from the platform's implementer rather than by an unbounded string
 *          function, and must neither retain nor release the pointer.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success beacons from this Access Point carry the requested access network type
 *       in their Interworking Information Element. On failure this interface does not
 *       specify how much of the requested change was applied, so a caller should read the
 *       affected state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The type was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should re-check
 *                          the value against what `@param` states before retrying with the
 *                          same argument.
 *
 * @note The value only reaches the air while the Interworking Service is enabled; see
 *       `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                          allocates and releases it, and the `HAL` writes into it during
 *                          the call. Whether the implementation retains the pointer
 *                          beyond the call is not specified by this interface, so the
 *                          caller should keep the variable allocated and unmoved while
 *                          the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the advertised access network type; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The type was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
    BOOL interworkingEnabled;     /*!< `TRUE` where the `AP` advertises the Interworking element,
                                       which is what makes the rest of this structure meaningful;
                                       where it is `FALSE` this interface does not state what the
                                       other members hold. `wifi_getApInterworkingServiceEnable()`
                                       and `wifi_setApInterworkingServiceEnable()` read and write
                                       the interworking service separately, and this interface does
                                       not state whether they act on this same member. */
    UINT accessNetworkType;      /*!< Access Network Type field of the Interworking element, whose
                                      values 802.11-2016 section 9.4.2.92 defines; this interface
                                      enumerates none of them. */
    BOOL internetAvailable;      /*!< `TRUE` where the Interworking element advertises that the
                                      network offers internet access. It is only meaningful where
                                      `interworkingEnabled` above is `TRUE`, and this interface does
                                      not state that the `AP` verifies the claim before advertising
                                      it. */
    BOOL asra;                   /*!< `TRUE` where the Interworking element advertises `ASRA`,
                                      Additional Step Required for Access: 802.11-2016 section
                                      9.4.2.92 defines that bit of the Access Network Options field
                                      as indicating that the network requires a further step - an
                                      online enrolment, an acceptance of terms or a similar
                                      interaction - before it grants access. It says nothing about
                                      whether the network supports the Access Network Query
                                      Protocol. The member is only meaningful where
                                      `interworkingEnabled` above is `TRUE`, and this interface
                                      neither states which step is required nor exposes it, so a
                                      caller can learn only that one exists. */
    BOOL esr;                   /*!< `TRUE` where the Interworking element advertises that emergency
                                     services are reachable through this network. It is only
                                     meaningful where `interworkingEnabled` above is `TRUE`, and it
                                     is independent of `uesa` below. */
    BOOL uesa;                  /*!< `TRUE` where the Interworking element advertises that emergency
                                     services may be reached without authenticating. It is only
                                     meaningful where `interworkingEnabled` above is `TRUE`, and it
                                     is a separate advertisement from `esr` above. */
    BOOL venueOptionPresent;     /*!< `TRUE` where the element carries a Venue Info field, and so
                                      the flag that makes `venueGroup` and `venueType` below
                                      meaningful. Where it is `FALSE` a caller must not read either
                                      of them. */
    UCHAR venueGroup;            /*!< Venue Group code of the element's Venue Info field, meaningful
                                      only where `venueOptionPresent` above is `TRUE`. This
                                      interface enumerates no values for it. */
    UCHAR venueType;             /*!< Venue Type code within `venueGroup`, meaningful only where
                                      `venueOptionPresent` above is `TRUE`. The same numeric type
                                      denotes different venues in different groups, so the two
                                      members are only meaningful together. */
    BOOL hessOptionPresent;      /*!< `TRUE` where the element carries its optional HESSID field,
                                      and so the flag that makes `hessid` below meaningful. `HESSID`
                                      is the Homogeneous ESS Identifier that 802.11-2016 section
                                      9.4.2.92 defines - a MAC-address-formatted value identifying
                                      the homogeneous ESS an `AP` belongs to - and not a Hotspot 2.0
                                      security feature. Where this member is `FALSE` a caller must
                                      not read `hessid`. */
    mac_addr_str_t hessid;       /*!< Homogeneous ESS Identifier of the element's optional HESSID
                                      field, as a `mac_addr_str_t` string rather than six octets.
                                      Meaningful only where `hessOptionPresent` above is `TRUE`; an
                                      empty string indicates that no value was provided. This
                                      interface states no letter case or separator convention for
                                      the string, so a caller must compare two of them with that in
                                      mind. */
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
 *                          The caller allocates and releases it, and the `HAL` writes
 *                          into it during the call. Whether the implementation retains
 *                          the pointer beyond the call is not specified by this
 *                          interface, so the caller should keep the variable allocated
 *                          and unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the Interworking Service state; on failure the output
 *       is left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note `wifi_getAPCapabilities()` reports through `interworkingServiceSupported` whether
 *       this Access Point can support the service at all.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Interworking Service is in the requested state. On failure this
 *       interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `input_bool`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getApInterworkingServiceEnable()` before relying on it.
 *
 * @note Disabling the service stops the Interworking Information Element being advertised,
 *       so the access network type and the element's other fields have no effect on the air
 *       until it is enabled again.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                            advertises. The caller allocates and releases it, and the
 *                            `HAL` writes into it during the call. Whether the
 *                            implementation retains the pointer beyond the call is not
 *                            specified by this interface, so the caller should keep the
 *                            structure allocated and unmoved while the `HAL` remains
 *                            initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the advertised element; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The element was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note The structure's `hessid` member is optional and carries an empty string when no
 *       value is provided, as its own member documentation states.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                          caller owns the structure and must keep it valid for the
 *                          duration of the call. Whether the implementation retains the
 *                          pointer afterwards is not specified by this interface, so the
 *                          caller should keep it allocated and unmoved while the `HAL`
 *                          remains initialised. Every member is applied, so read the
 *                          current element first and modify it rather than passing a
 *                          partially filled structure.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success beacons and probe responses carry the supplied Interworking
 *       Information Element. On failure this interface does not specify how much of the
 *       requested change was applied, so a caller should read the affected state back
 *       rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The element was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          element back to establish what took effect, since this interface
 *                          does not state that the update is atomic.
 *
 * @note The parameter name is spelt `infoEelement` in the declaration. That is how it must
 *       be referred to in documentation and is not a typing error in this comment.
 * @note The element reaches the air only while the Interworking Service is enabled.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
    INT ap_index;               /*!< Access Point the frame was seen on, in the same `apIndex`
                                     numbering this interface's calls take - `AP_INDEX_1` to
                                     `AP_INDEX_24`. `INT`, and this interface states no sentinel for
                                     `not known`. */
    mac_address_t sta_mac;      /*!< Address of the station the frame came from or was sent to, as
                                     the six raw octets of `mac_address_t` rather than text. `dir`
                                     below is what says which of the two it is. */
    wifi_mgmtFrameType_t type;  /*!< Management frame subtype, as one of the `wifi_mgmtFrameType_t`
                                     enumerators - probe, association, authentication, action and
                                     beacon frames among them. It is not a `wifi_frameType_t`: that
                                     separate enumeration names the management, control and data
                                     classes, and the two must not be mixed. */
    wifi_direction_t dir;       /*!< Whether the frame travelled uplink or downlink, as one of the
                                     `wifi_direction_t` enumerators, with `wifi_direction_unknown`
                                     where the `HAL` cannot tell. It is what fixes the role of
                                     `sta_mac` above. */
    INT sig_dbm;                /**< Signal strength in dBm. */
    INT phy_rate;               /*!< PHY rate the frame was carried at. This interface states no
                                     unit for the value, so a caller must not assume Mbps or kbps,
                                     and no sentinel for a rate the `HAL` could not determine. */
    UCHAR token;                /*!< Token carried with the frame. This interface states neither how
                                     the value is derived nor which frame types populate it, so a
                                     caller should treat it as an opaque correlator supplied
                                     alongside the frame. */
    UINT recv_freq;             /*!< Frequency the frame arrived on. This interface states no unit
                                     for the value, so a caller must not assume MHz; the frame
                                     carries no band member, so the channel cannot be derived from
                                     it here. */
    UINT len;                   /*!< Number of octets behind `data` below. It is the caller's only
                                     bound on that pointer, since the pointer itself carries no
                                     extent, so a handler must read no further than this value. */
    UCHAR *data;                /*!< Start of the frame body. The member is a bare `UCHAR *`: this
                                     interface states neither which side owns the storage, how long
                                     it stays valid, nor whether a handler may free it, so a handler
                                     must bound every read by `len` above, must release nothing, and
                                     must copy anything it needs to keep - `Asynchronous
                                     Notification Model` in the HAL specification makes that copy
                                     the client's responsibility. */
} __attribute__((packed)) wifi_frame_t;

/**
 * @brief Callback function invoked when an 802.1x frame is received.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] type     EAPOL type.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `sta` and `data` are both delivered in storage the `HAL` owns - `sta` is a
 *       `mac_address_t`, an array typedef, so the handler receives the address of the
 *       `HAL`'s six octets rather than a copy of them - and this interface states no
 *       lifetime for either beyond this call. The handler must copy whatever it needs
 *       before returning and must neither retain nor release them, per `Asynchronous
 *       Notification Model` in the HAL specification.
 * @note The handler returns `void`, so it reports nothing back to the `HAL`; a handler
 *       that cannot process the event has no way to say so through this interface.
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
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `sta` and `data` are both delivered in storage the `HAL` owns - `sta` is a
 *       `mac_address_t`, an array typedef, so the handler receives the address of the
 *       `HAL`'s six octets rather than a copy of them - and this interface states no
 *       lifetime for either beyond this call. The handler must copy whatever it needs
 *       before returning and must neither retain nor release them, per `Asynchronous
 *       Notification Model` in the HAL specification.
 * @note The handler returns `void`, so it reports nothing back to the `HAL`; a handler
 *       that cannot process the event has no way to say so through this interface.
 */
typedef void (* wifi_sent8021xFrame_callback)(unsigned int ap_index, mac_address_t sta, wifi_eapol_type_t type, void *data, unsigned int len);


/**
 * @brief Callback function invoked when an authentication frame is received.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `sta` and `data` are both delivered in storage the `HAL` owns - `sta` is a
 *       `mac_address_t`, an array typedef, so the handler receives the address of the
 *       `HAL`'s six octets rather than a copy of them - and this interface states no
 *       lifetime for either beyond this call. The handler must copy whatever it needs
 *       before returning and must neither retain nor release them, per `Asynchronous
 *       Notification Model` in the HAL specification.
 * @note The handler returns `void`, so it reports nothing back to the `HAL`; a handler
 *       that cannot process the event has no way to say so through this interface.
 */
typedef void (* wifi_receivedAuthFrame_callback)(unsigned int ap_index, mac_address_t sta, void *data, unsigned int len);


/**
 * @brief Callback function invoked when an authentication frame is sent.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `sta` and `data` are both delivered in storage the `HAL` owns - `sta` is a
 *       `mac_address_t`, an array typedef, so the handler receives the address of the
 *       `HAL`'s six octets rather than a copy of them - and this interface states no
 *       lifetime for either beyond this call. The handler must copy whatever it needs
 *       before returning and must neither retain nor release them, per `Asynchronous
 *       Notification Model` in the HAL specification.
 * @note The handler returns `void`, so it reports nothing back to the `HAL`; a handler
 *       that cannot process the event has no way to say so through this interface.
 */
typedef void (* wifi_sentAuthFrame_callback)(unsigned int ap_index, mac_address_t sta, void *data, unsigned int len);


/**
 * @brief Callback function invoked when an association request frame is received.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `sta` and `data` are both delivered in storage the `HAL` owns - `sta` is a
 *       `mac_address_t`, an array typedef, so the handler receives the address of the
 *       `HAL`'s six octets rather than a copy of them - and this interface states no
 *       lifetime for either beyond this call. The handler must copy whatever it needs
 *       before returning and must neither retain nor release them, per `Asynchronous
 *       Notification Model` in the HAL specification.
 * @note The handler returns `void`, so it reports nothing back to the `HAL`; a handler
 *       that cannot process the event has no way to say so through this interface.
 */
typedef void (* wifi_receivedAssocReqFrame_callback)(unsigned int ap_index, mac_address_t sta, void *data, unsigned int len);


/**
 * @brief Callback function invoked when an association response frame is sent.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] sta      MAC address of the station.
 * @param[in] data     Pointer to the frame data.
 * @param[in] len      Length of the frame data.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `sta` and `data` are both delivered in storage the `HAL` owns - `sta` is a
 *       `mac_address_t`, an array typedef, so the handler receives the address of the
 *       `HAL`'s six octets rather than a copy of them - and this interface states no
 *       lifetime for either beyond this call. The handler must copy whatever it needs
 *       before returning and must neither retain nor release them, per `Asynchronous
 *       Notification Model` in the HAL specification.
 * @note The handler returns `void`, so it reports nothing back to the `HAL`; a handler
 *       that cannot process the event has no way to say so through this interface.
 */
typedef void (* wifi_sentAssocRspFrame_callback)(unsigned int ap_index, mac_address_t sta, void *data, unsigned int len);


/**
 * @brief Frame hook function type.
 *
 * @param[in] ap_index Index of the Access Point.
 * @param[in] type     Type of the management frame.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller. This interface does not
 *          enumerate the values it accepts from the handler, so no return-code contract is
 *          established for this type; `RETURN_OK` and `RETURN_ERR` in `wifi_hal_generic.h`
 *          are what the neighbouring handlers in this header use, and they are numerically
 *          identical to `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR`.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 */
typedef INT (* wifi_hal_frame_hook_fn_t)(INT ap_index, wifi_mgmtFrameType_t type);


#ifdef WIFI_HAL_VERSION_3_PHASE2
/**
 * @brief Callback function invoked when a management frame is received.
 *
 * @param[in] apIndex Index of the Access Point.
 * @param[in] frame   Pointer to the received frame.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller. This interface does not
 *          enumerate the values it accepts from the handler, so no return-code contract is
 *          established for this type; `RETURN_OK` and `RETURN_ERR` in `wifi_hal_generic.h`
 *          are what the neighbouring handlers in this header use, and they are numerically
 *          identical to `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR`.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `frame` is owned by the `HAL` and this interface states no lifetime for it
 *       beyond this call, so the handler must copy whatever it needs before returning
 *       and must neither retain nor release it, per `Asynchronous Notification
 *       Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
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
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller. This interface does not
 *          enumerate the values it accepts from the handler, so no return-code contract is
 *          established for this type; `RETURN_OK` and `RETURN_ERR` in `wifi_hal_generic.h`
 *          are what the neighbouring handlers in this header use, and they are numerically
 *          identical to `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR`.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `sta_mac` and `frame` are both delivered in storage the `HAL` owns - `sta_mac` is
 *       a `mac_address_t`, an array typedef, so the handler receives the address of the
 *       `HAL`'s six octets rather than a copy of them - and this interface states no
 *       lifetime for either beyond this call. The handler must copy whatever it needs
 *       before returning and must neither retain nor release them, per `Asynchronous
 *       Notification Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
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
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller. This interface does not
 *          enumerate the values it accepts from the handler, so no return-code contract is
 *          established for this type; `RETURN_OK` and `RETURN_ERR` in `wifi_hal_generic.h`
 *          are what the neighbouring handlers in this header use, and they are numerically
 *          identical to `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR`.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `sta_mac` and `frame` are owned by the `HAL` and this interface states no
 *       lifetime for them beyond this call, so the handler must copy whatever it needs
 *       before returning and must neither retain nor release them, per `Asynchronous
 *       Notification Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
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
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller. This interface does not
 *          enumerate the values it accepts from the handler, so no return-code contract is
 *          established for this type; `RETURN_OK` and `RETURN_ERR` in `wifi_hal_generic.h`
 *          are what the neighbouring handlers in this header use, and they are numerically
 *          identical to `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR`.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `sta_mac` and `frame` are owned by the `HAL` and this interface states no
 *       lifetime for them beyond this call, so the handler must copy whatever it needs
 *       before returning and must neither retain nor release them, per `Asynchronous
 *       Notification Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 */
typedef INT (* wifi_receivedDataFrame_callback)(INT apIndex, UCHAR *sta_mac, UCHAR *frame, UINT len, wifi_dataFrameType_t type, wifi_direction_t dir);
#endif


/**
 * @brief Callback function invoked when CSI data is available.
 *
 * @param[in] mac_addr  MAC address of the station.
 * @param[in] csi_data  Pointer to the CSI data.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller. This interface does not
 *          enumerate the values it accepts from the handler, so no return-code contract is
 *          established for this type; `RETURN_OK` and `RETURN_ERR` in `wifi_hal_generic.h`
 *          are what the neighbouring handlers in this header use, and they are numerically
 *          identical to `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR`.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note The handler must not suspend and must not invoke a blocking system call; it
 *       should hand the event to its own task and return, per `Blocking calls` in
 *       the HAL specification.
 * @note `mac_addr` and `csi_data` are both delivered in storage the `HAL` owns -
 *       `mac_addr` is a `mac_address_t`, an array typedef, so the handler receives the
 *       address of the `HAL`'s six octets rather than a copy of them - and this interface
 *       states no lifetime for either beyond this call. The handler must copy whatever it
 *       needs before returning and must neither retain nor release them, per
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
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
 * association and action frames without owning the radio. This interface does not
 * state whether registering a second handler replaces the first, adds to it or is
 * rejected, so a caller must not depend on any of those outcomes.
 *
 * @param[in] mgmtRxCallback  Handler to install, of type
 *                            `wifi_receivedMgmtFrame_callback`. The `HAL` keeps this
 *                            function pointer after the call returns, since it
 *                            invokes the handler later, so the function must remain
 *                            callable for as long as frames are wanted. This
 *                            interface declares no call that removes a handler and
 *                            states no end to a registration, so it does not
 *                            establish when the `HAL` stops using the pointer. The
 *                            effect of passing NULL is not specified by this
 *                            interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the handler is installed and is invoked for each subsequent
 *       management frame. On failure this interface does not specify whether the handler
 *       was installed, nor whether a previously registered handler is still in place, so a
 *       caller should assume neither.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was installed.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. Which conditions produce this
 *                          code rather than another is not specified by this interface;
 *                          `Internal Error Handling` in the HAL specification reserves
 *                          `WIFI_HAL_NOT_READY` for a `HAL` that cannot yet serve a
 *                          request, so a caller should act on the code it actually
 *                          receives. The caller should log the failure and treat
 *                          management-frame notification as unavailable rather than
 *                          retrying in a loop.
 *
 * @execution callback
 * @sideeffect On success, installs `mgmtRxCallback` as the handler the `HAL` invokes
 *             for each received management frame, and keeps that function pointer
 *             after this call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous; delivery of
 *       `wifi_receivedMgmtFrame_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The frame buffer reaching the handler is storage the `HAL` owns, and
 *          `wifi_receivedMgmtFrame_callback` states no lifetime for it beyond that call, so
 *          the handler must copy any part of it that it needs before returning and must
 *          neither retain nor release it, per `Asynchronous Notification Model` in
 *          the HAL specification.
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
 * one station, which is what `wifi_enableCSIEngine()` controls. This interface does
 * not state whether registering a second handler replaces the first, adds to it or
 * is rejected, so a caller must not depend on any of those outcomes.
 *
 * @param[in] callback_proc  Handler to install, of type `wifi_csi_callback`. The
 *                           `HAL` keeps this function pointer after the call
 *                           returns, since it invokes the handler later, so the
 *                           function must remain callable for as long as `CSI` data
 *                           is wanted. This interface declares no call that removes
 *                           a handler and states no end to a registration, so it
 *                           does not establish when the `HAL` stops using the
 *                           pointer. The effect of passing NULL is not specified by
 *                           this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The handler is installed. It is invoked only for stations whose `CSI` engine
 *       is enabled, so a correct registration produces no callbacks at all until
 *       `wifi_enableCSIEngine()` enables one.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes when `CSI`
 *             data becomes available, and keeps that function pointer after this
 *             call returns. This interface states no other effect; in particular it
 *             does not state that registering starts `CSI` collection, which
 *             `wifi_enableCSIEngine()` controls.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_csi_callback` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       should just send a message to a driver event handler task. `CSI` can arrive at a
 *       high rate, so any per-sample work belongs on the caller's own thread.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The `wifi_csi_data_t` reaching the handler is storage the `HAL` owns, and
 *          `wifi_csi_callback` states no lifetime for it beyond that call, so the handler
 *          must copy what it needs before returning and must neither retain nor release it,
 *          per `Asynchronous Notification Model` in the HAL specification.
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
 * `wifi_mgmt_frame_callbacks_register()` instead. This interface does not state
 * whether registering a second hook replaces the first, adds to it or is rejected,
 * so a caller must not depend on any of those outcomes.
 *
 * @param[in] hook_fn  Hook to install, of type `wifi_hal_frame_hook_fn_t`. The `HAL`
 *                     keeps this function pointer after the call returns, since it
 *                     invokes the hook later, so the function must remain callable
 *                     for as long as management frames are to be hooked. This
 *                     interface declares no call that removes a hook and states no
 *                     end to a registration, so it does not establish when the `HAL`
 *                     stops using the pointer. The effect of passing NULL is not
 *                     specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and
 *      Startup` in the HAL specification. The effect of registering beforehand is
 *      not specified by this interface.
 * @post The hook is installed and is consulted on each subsequent management frame
 *       reception.
 *
 * @execution callback
 * @sideeffect Installs `hook_fn` as the hook the `HAL` consults on management-frame
 *             reception, and keeps that function pointer after this call returns.
 *             This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; invocation of
 *       `wifi_hal_frame_hook_fn_t` is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the hook, which
 *       runs on the frame reception path.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[in] sta  MAC address of the station: six octets in caller-owned
 *                 storage, or NULL to address every station on this Virtual Access Point.
 *                 `mac_address_t` is an array typedef, so the parameter adjusts to a
 *                 pointer and the callee receives the address of the caller's own storage
 *                 rather than a copy of it; that is also why NULL is passable and is the
 *                 form this declaration's description gives for the all-stations case.
 *                 The caller must keep the storage valid for the duration of the call.
 *                 `Memory Model` in the HAL specification establishes who owns that
 *                 storage but not how long the implementation may hold a pointer to it,
 *                 so the caller should keep the six octets allocated and unmoved while
 *                 the `HAL` remains initialised. This interface assigns no separate
 *                 meaning to an address whose six octets are all zero, so a caller must
 *                 use NULL, not a zeroed buffer, for the all-stations case.
 * @param[in] enable  true to enable the Channel State Information engine for that
 *                    station, false to disable it. It must be false when `sta` is NULL:
 *                    this declaration's description requires the call to fail for a NULL
 *                    address with `enable` true.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the state of the configuration is
 *      established, so a caller must not rely on either.
 * @post On success the Channel State Information engine is in the requested state for
 *       that station, or is disabled for every station on the Virtual Access Point when
 *       `sta` was NULL and `enable` was false. On failure this interface does not specify
 *       how much of the requested change was applied, so a caller should not assume the
 *       previous state survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The engine state was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This declaration's description names one
 *                          condition explicitly - a NULL `sta` with `enable` true must fail
 *                          - and that combination must not be retried, since the interface
 *                          requires it to fail. Beyond that one case this interface does
 *                          not enumerate the conditions that lead to this code.
 *
 * @note Data only reaches the caller once a handler is installed with
 *       `wifi_csi_callback_register()`; enabling the engine without one produces no
 *       callbacks.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[in] sta  MAC address of the destination station: six octets in
 *                 caller-owned storage. `mac_address_t` is an array typedef, so the
 *                 parameter adjusts to a pointer and the callee receives the address of
 *                 the caller's own storage rather than a copy of it. Passing NULL
 *                 broadcasts the payload on the Virtual Access Point, which is the rule
 *                 this declaration's own description states. The caller must keep the
 *                 storage valid for the duration of the call. `Memory Model` in
 *                 the HAL specification establishes who owns that storage but not how
 *                 long the implementation may hold a pointer to it, so the caller should
 *                 keep the six octets allocated and unmoved while the `HAL` remains
 *                 initialised.
 * @param[in] data  Caller-owned buffer holding `len` bytes of payload beginning at Layer
 *                  3; it must not include any Layer 2 header. The `HAL` reads it during
 *                  the call, and whether the implementation retains the pointer
 *                  afterwards is not specified by this interface, so the caller should
 *                  keep the buffer allocated and unmoved while the `HAL` remains
 *                  initialised.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the frame has been handed to the driver for transmission. On failure
 *       this interface does not specify whether the frame was transmitted, and it offers
 *       no transmit confirmation, so a caller can establish neither outcome; it should
 *       treat the frame as not delivered while allowing that it may have been sent.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The frame was accepted for transmission.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          the buffer and, for a unicast address, confirm the station is
 *                          associated before retrying.
 *
 * @note Success means the driver accepted the frame, not that it reached the air or the
 *       peer; this interface provides no transmit confirmation.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[in] sta  MAC address of the peer to send the frame to: six octets in
 *                 caller-owned storage. `mac_address_t` is an array typedef, so
 *                 the parameter adjusts to a pointer and the callee receives the address
 *                 of the caller's own storage rather than a copy of it. The caller must
 *                 supply all six octets and must keep the storage valid for the duration
 *                 of the call. `Memory Model` in the HAL specification establishes who
 *                 owns that storage but not how long the implementation may hold a
 *                 pointer to it, so the caller should keep the six octets allocated and
 *                 unmoved while the `HAL` remains initialised. This interface assigns no
 *                 meaning to a NULL pointer or to an all-zero address here, so a caller
 *                 must supply a real peer address.
 * @param[in] frequency  Channel frequency to transmit on, which allows a public action
 *                       frame to be sent to a device on an off-channel. Pass 0 to use the
 *                       Virtual Access Point's operating channel.
 * @param[in] frame  Caller-owned buffer holding the frame to transmit. The `HAL` reads
 *                   `len` bytes during the call. Whether the implementation retains the
 *                   pointer afterwards is not specified by this interface, so the caller
 *                   should keep the buffer allocated and unmoved while the `HAL` remains
 *                   initialised rather than freeing or reusing it when the call returns.
 * @param[in] len  Length of `frame` in bytes.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the action frame has been handed to the driver for transmission. On
 *       failure this interface does not specify whether the frame was transmitted, and it
 *       offers no transmit confirmation, so a caller can establish neither outcome; it
 *       should treat the frame as not delivered while allowing that it may have been
 *       sent.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The frame was accepted for transmission.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          the buffer and frequency rather than retrying unchanged.
 *
 * @note Use `wifi_sendActionFrameExt()` where the radio must dwell on an off-channel
 *       frequency after transmitting to receive a response; this call states no dwell time.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 * @param[in] sta  MAC address of the peer to send the frame to: six octets in
 *                 caller-owned storage. `mac_address_t` is an array typedef, so
 *                 the parameter adjusts to a pointer and the callee receives the address
 *                 of the caller's own storage rather than a copy of it. The caller must
 *                 supply all six octets and must keep the storage valid for the duration
 *                 of the call. `Memory Model` in the HAL specification establishes who
 *                 owns that storage but not how long the implementation may hold a
 *                 pointer to it, so the caller should keep the six octets allocated and
 *                 unmoved while the `HAL` remains initialised. This interface assigns no
 *                 meaning to a NULL pointer or to an all-zero address here, so a caller
 *                 must supply a real peer address.
 * @param[in] frequency  Channel frequency to transmit on, which allows a public action
 *                       frame to be sent to a device on an off-channel. Pass 0 to use the
 *                       Virtual Access Point's operating channel.
 * @param[in] wait  Time in milliseconds to remain on an off-channel frequency after
 *                  transmitting, before returning to the operating channel. Pass 0 not to
 *                  wait.
 * @param[in] frame  Caller-owned buffer holding the frame to transmit. The `HAL` reads
 *                   `len` bytes during the call. Whether the implementation retains the
 *                   pointer afterwards is not specified by this interface, so the caller
 *                   should keep the buffer allocated and unmoved while the `HAL` remains
 *                   initialised rather than freeing or reusing it when the call returns.
 * @param[in] len  Length of `frame` in bytes.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the action frame has been handed to the driver for transmission, and
 *       the radio remains on the given frequency for `wait` milliseconds before returning
 *       to its operating channel. On failure this interface specifies neither whether the
 *       frame was transmitted nor whether the radio left its operating channel, and it
 *       offers no transmit confirmation, so a caller can establish neither.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The frame was accepted for transmission.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          the buffer and frequency rather than retrying unchanged.
 *
 * @note A non-zero `wait` takes the radio off its operating channel for that period, during
 *       which associated clients are not served. Keep it as short as the exchange allows.
 * @note This interface does not state whether the call returns before or after the dwell
 *       period elapses, so a caller must not use it as a timing primitive.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @see wifi_sendActionFrame
 */
INT wifi_sendActionFrameExt(INT apIndex,
                            mac_address_t sta,
                            UINT frequency,
                            UINT wait,
                            UCHAR *frame,
                            UINT len);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
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
    UINT AdvertisementID;          /*!< Identifier of the advertisement protocol this GAS
                                        configuration applies to, in the sense of the
                                        `dot11GASAdvertisementEntry` field this structure's comment
                                        cites. This interface enumerates no values for it. */
    BOOL PauseForServerResponse;   /*!< `TRUE` requests that the `AP` wait for the advertisement
                                        server before answering a GAS query, in which case
                                        `ResponseTimeout` and `ComeBackDelay` below bound the wait.
                                        This interface does not state what the `AP` answers where it
                                        is `FALSE`. */
    UINT ResponseTimeout;          /**< Response timeout in seconds. */
    UINT ComeBackDelay;            /**< Comeback delay in seconds. */
    UINT ResponseBufferingTime;    /**< Response buffering time in seconds. */
    UINT QueryResponseLengthLimit; /**< Query response length limit in bytes. */
} __attribute__((packed)) wifi_GASConfiguration_t;
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
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
 *                          of 802.11-2016 Annex C.3. The caller owns the structure and
 *                          must keep it valid for the duration of the call. Whether the
 *                          implementation retains the pointer afterwards is not specified
 *                          by this interface, so the caller should keep it allocated and
 *                          unmoved while the `HAL` remains initialised.
 *                          `GAS_CFG_TYPE_SUPPORTED` is the one configuration type this
 *                          interface names.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Generic Advertisement Service uses the supplied advertisement
 *       identifier, response timeout, comeback delay, buffering time and query response
 *       length limit. On failure this interface does not specify how many of those values
 *       were applied, and it offers no call that reads the active configuration back, so a
 *       caller must treat the configuration as being in an unknown state.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should re-check
 *                          the structure against what the `@param` entries state before
 *                          retrying with the same contents.
 *
 * @note This call takes no Access Point index: the Generic Advertisement Service
 *       configuration applies device-wide, unlike the per-Access-Point interworking calls.
 * @note `ResponseTimeout`, `ComeBackDelay` and `ResponseBufferingTime` are in seconds and
 *       `QueryResponseLengthLimit` is in bytes, as the structure's own member documentation
 *       states.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success beacons and probe responses carry the Country information element when
 *       `enabled` is true, and omit it when it is false. On failure this interface does not
 *       specify how much of the requested change was applied, so a caller should read the
 *       affected state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enabled`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getCountryIe()` before relying on it.
 *
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                      false when it is excluded. The caller allocates and releases it,
 *                      and the `HAL` writes into it during the call. Whether the
 *                      implementation retains the pointer beyond the call is not
 *                      specified by this interface, so the caller should keep the
 *                      variable allocated and unmoved while the `HAL` remains
 *                      initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the Country information element state; on failure the
 *       output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point inspects and filters Layer 2 traffic when `enabled`
 *       is true, and does not when it is false. On failure this interface does not specify
 *       how much of the requested change was applied, so a caller should read the affected
 *       state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enabled`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getLayer2TrafficInspectionFiltering()` before relying on
 *                          it.
 *
 * @note This is the Hotspot 2.0 L2TIF control, mirrored by the `l2tif` member of
 *       `wifi_passpoint_settings_t`.
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                      The caller allocates and releases it, and the `HAL` writes into it
 *                      during the call. Whether the implementation retains the pointer
 *                      beyond the call is not specified by this interface, so the caller
 *                      should keep the variable allocated and unmoved while the `HAL`
 *                      remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the Layer 2 traffic inspection and filtering state;
 *       on failure the output is left unspecified, so a caller must not read it unless
 *       the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the DGAF-disabled bit of the Hotspot 2.0 Indication information
 *       element is set when `disabled` is true, and clear when it is false. On failure this
 *       interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `disabled`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getDownStreamGroupAddress()` before relying on it.
 *
 * @note The argument's sense is inverted relative to the other toggles here: true disables
 *       the forwarding behaviour rather than enabling a feature.
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                       The caller allocates and releases it, and the `HAL` writes into
 *                       it during the call. Whether the implementation retains the
 *                       pointer beyond the call is not specified by this interface, so
 *                       the caller should keep the variable allocated and unmoved while
 *                       the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the DGAF-disabled state; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success beacons and probe responses carry the BSS Load information element
 *       when `enabled` is true, and omit it when it is false. On failure this interface
 *       does not specify how much of the requested change was applied, so a caller should
 *       read the affected state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enabled`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getBssLoad()` before relying on it.
 *
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                      caller allocates and releases it, and the `HAL` writes into it
 *                      during the call. Whether the implementation retains the pointer
 *                      beyond the call is not specified by this interface, so the caller
 *                      should keep the variable allocated and unmoved while the `HAL`
 *                      remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the BSS Load information element state; on failure
 *       the output is left unspecified, so a caller must not read it unless the call
 *       succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the device driver answers ARP on behalf of associated clients when
 *       `enabled` is true, and does not when it is false. On failure this interface does
 *       not specify how much of the requested change was applied, so a caller should read
 *       the affected state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enabled`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getProxyArp()` before relying on it.
 *
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                      allocates and releases it, and the `HAL` writes into it during the
 *                      call. Whether the implementation retains the pointer beyond the
 *                      call is not specified by this interface, so the caller should keep
 *                      the variable allocated and unmoved while the `HAL` remains
 *                      initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the proxy ARP state as reported by the driver; on
 *       failure the output is left unspecified, so a caller must not read it unless the
 *       call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point advertises Hotspot 2.0 when `enabled` is true, and
 *       does not when it is false. On failure this interface does not specify how much of
 *       the requested change was applied, so a caller should read the affected state back
 *       rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `enabled`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getApHotspotElement()` before relying on it.
 *
 * @note Hotspot 2.0 advertisement builds on the Interworking Service, so enabling it here
 *       without enabling that service has no effect on the air.
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                      caller allocates and releases it, and the `HAL` writes into it
 *                      during the call. Whether the implementation retains the pointer
 *                      beyond the call is not specified by this interface, so the caller
 *                      should keep the variable allocated and unmoved while the `HAL`
 *                      remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the Hotspot 2.0 state; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                         The caller owns the structure and must keep it valid for the
 *                         duration of the call. Whether the implementation retains the
 *                         pointer afterwards is not specified by this interface, so the
 *                         caller should keep it allocated and unmoved while the `HAL`
 *                         remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success beacons and probe responses carry the supplied Roaming Consortium
 *       information element, provided interworking is enabled and the OI count is greater
 *       than 0. On failure this interface does not specify how much of the requested change
 *       was applied, so a caller should read the affected state back rather than assume the
 *       previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The element was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should re-check
 *                          the structure against what the `@param` entries state before
 *                          retrying with the same contents.
 *
 * @note The element is advertised only while interworking is enabled and the OI count is
 *       greater than 0; either condition unmet leaves it off the air.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                          as hex strings. The caller allocates and releases it, and the
 *                          `HAL` writes into it during the call. Whether the
 *                          implementation retains the pointer beyond the call is not
 *                          specified by this interface, so the caller should keep the
 *                          structure allocated and unmoved while the `HAL` remains
 *                          initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the advertised element, where a count of 0 means the
 *       element is not present and the length and OI fields carry nothing meaningful; on
 *       failure the output is left unspecified, so a caller must not read it unless the
 *       call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The element was retrieved. A count of 0 is a success meaning
 *                          no element is advertised.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note Test the count before reading the length or OI fields: at a count of 0 they are
 *       explicitly not meaningful.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the P2P information element is advertised with the P2P Manageability
 *       attribute's "Cross Connection Permitted" field set to 0 when `disabled` is true.
 *       On failure this interface does not specify how much of the requested change was
 *       applied, so a caller should read the affected state back rather than assume the
 *       previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The setting was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and it names no rejected
 *                          value for `disabled`, so a caller cannot read this code as a
 *                          verdict on the argument; it should read the setting back with
 *                          `wifi_getP2PCrossConnect()` before relying on it.
 *
 * @note The argument's sense is inverted relative to the other toggles here: true disables
 *       cross connection rather than enabling a feature.
 * @note An information element only reaches the air while the Interworking Service is
 *       enabled; see `wifi_setApInterworkingServiceEnable()`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                       allocates and releases it, and the `HAL` writes into it during
 *                       the call. Whether the implementation retains the pointer beyond
 *                       the call is not specified by this interface, so the caller should
 *                       keep the variable allocated and unmoved while the `HAL` remains
 *                       initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the P2P cross connect state; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The state was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                          descriptions. The caller allocates and releases it, and the
 *                          `HAL` writes into it during the call. Whether the
 *                          implementation retains the pointer beyond the call is not
 *                          specified by this interface, so the caller should keep the
 *                          array allocated and unmoved while the `HAL` remains
 *                          initialised. This interface does not state whether the
 *                          implementation writes to elements beyond the reported count,
 *                          so a caller must neither read them nor rely on their previous
 *                          contents surviving.
 * @param[out] numSessionReturned  Caller-allocated variable that receives the number of
 *                                 elements actually written, never more than
 *                                 `maxNumberSessions`. The caller allocates and releases
 *                                 it, and the `HAL` writes into it during the call.
 *                                 Whether the implementation retains the pointer beyond
 *                                 the call is not specified by this interface, so the
 *                                 caller should keep the variable allocated and unmoved
 *                                 while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the array's first `*numSessionReturned` elements hold session
 *       descriptions, both individual and broadcast; this interface does not state what,
 *       if anything, was written beyond that count. On failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The sessions were retrieved. Zero sessions is a success, not
 *                          an error.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note Both individual and broadcast Target Wake Time sessions are returned in the same
 *       array; the caller distinguishes them from the session description itself.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                           identifier of the newly created session. Caller-allocated.
 *                           Whether the implementation retains the pointer beyond the
 *                           call is not specified by this interface, so the caller should
 *                           keep the storage allocated and unmoved while the `HAL`
 *                           remains initialised. It must point to valid storage in both
 *                           directions, and must hold a valid identifier before an
 *                           update.
 *
 * @pre `wifi_init()` must have completed successfully, and when `create` is false
 *      `*sessionID` must already identify an existing session; see `Initialization and
 *      Startup` in the HAL specification. This interface does not specify the outcome of
 *      a call that breaks either pre-condition - a call made before initialisation, or an
 *      update naming a session that does not exist - so a caller must not rely on a
 *      particular status code or on the existing schedules being left alone.
 * @post On success a broadcast Target Wake Time session exists with the supplied
 *       parameters, and `*sessionID` identifies it. On failure this interface does not
 *       specify how much of the requested change was applied, so a caller should enumerate
 *       the sessions with `wifi_getTWTsessions()` rather than assume the previous
 *       schedules survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The session was created or updated, and `*sessionID`
 *                          identifies it.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should enumerate
 *                          the sessions with `wifi_getTWTsessions()` to establish the
 *                          actual state before retrying.
 *
 * @note On failure this interface does not state whether `*sessionID` was modified, so a
 *       caller must not read it unless the call succeeded.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the identified session no longer exists. On failure this interface does
 *       not specify how much of the requested change was applied, so a caller should read
 *       the affected state back rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The session was torn down.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should enumerate
 *                          the sessions with `wifi_getTWTsessions()` rather than assuming
 *                          the session is gone.
 *
 * @note This tears down individual and broadcast sessions alike; the identifier determines
 *       which.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                  Each element's `wifi_keyId` and `wifi_psk` are the 64-byte character
 *                  arrays those members declare, and the caller writes `NUL`-terminated
 *                  text into each because this interface conveys no separate length for
 *                  either; `wifi_psk` holds a passphrase of 8 to 63 characters, as its
 *                  own declaration states. The `HAL` reads the array during the call.
 *                  Whether the implementation retains the pointer afterwards is not
 *                  specified by this interface, so the caller should keep the array
 *                  allocated and unmoved while the `HAL` remains initialised rather than
 *                  freeing it when the call returns.
 * @param[in] keysNumber  Number of elements in `keys`. Because the previous set is replaced
 *                        wholesale, a shorter set removes the keys it omits. This interface
 *                        does not specify the effect of passing 0, nor of passing NULL in
 *                        `keys`, so a caller must not use either as a documented way to
 *                        remove every non-primary key.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success exactly the supplied keys are active as non-primary keys and every
 *       previously set non-primary key is gone, while the primary key is untouched. On
 *       failure this interface does not specify how much of the requested change was
 *       applied, so a caller should read the affected state back rather than assume the
 *       previous one survived.
 *
 * @returns The status of the operation
 * @retval WIFI_HAL_SUCCESS The key set was replaced.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code, and because it does not state
 *                          that the replacement is atomic it does not establish how much of
 *                          the requested change took effect: a caller must not assume the
 *                          previous set survived, nor that the new one did not.
 *                          `wifi_getMultiPskKeys()` reports the keys that are active,
 *                          subject to the count limitation documented on that call.
 *
 * @note The primary key is not reachable through this call. The description above names
 *       `wifi_setApSecurityKeyPassphrase()`, which is declared in
 *       `wifi_hal_deprecated.h` rather than in the current interface, so it is reachable
 *       only through the umbrella `wifi_hal.h` include and is outside the documented
 *       contract. On the current surface the primary key is set with
 *       `wifi_setApSecurity()`, through the `u.key` arm of `wifi_vap_security_t`.
 * @note The structure follows the hostapd `wpa_psk_file` model, as its own documentation
 *       states.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning A `wifi_key_multi_psk_t` holds a passphrase. A caller that logs or traces these
 *          structures must redact `wifi_psk`. This interface does not establish that the
 *          implementation copies the key material during the call, and does not state
 *          whether it retains the pointer afterwards, so the point at which erasing the
 *          caller's storage becomes safe is not established either: the caller must keep
 *          that storage valid and unmoved while the `HAL` remains initialised, and must
 *          take any erasure guarantee it needs from the implementation it runs against.
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
 *                   keys. The caller allocates and releases it, and the `HAL` writes into
 *                   it during the call. Whether the implementation retains the pointer
 *                   beyond the call is not specified by this interface, so the caller
 *                   should keep the array allocated and unmoved while the `HAL` remains
 *                   initialised.
 * @param[in] keysNumber  Number of elements the array can hold.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the active non-primary keys, up to `keysNumber` of
 *       them; on failure the output is left unspecified, so a caller must not read it
 *       unless the call succeeded.
 *
 * @returns The status of the operation
 * @retval WIFI_HAL_SUCCESS The keys were retrieved.
 * @retval WIFI_HAL_ERROR   The keys were not retrieved. This interface does not enumerate
 *                          the conditions that lead to this code. The caller should
 *                          validate its arguments; a failure that persists across retries
 *                          should be logged and the value treated as unavailable.
 *
 * @note This call reports no count, so a caller cannot tell from the interface how many
 *       elements were written, and it defines no code that means "the array was too
 *       small": whether an undersized array truncates or fails is not specified here. A
 *       caller must size the array from the set it last pushed, and must treat an
 *       unwritten element as absent rather than as an empty key.
 * @note The primary key is never returned here.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning A `wifi_key_multi_psk_t` holds a passphrase. A caller that logs or traces these
 *          structures must redact `wifi_psk`. This interface does not establish that the
 *          implementation copies the key material during the call, and does not state
 *          whether it retains the pointer afterwards, so the point at which erasing the
 *          caller's storage becomes safe is not established either: the caller must keep
 *          that storage valid and unmoved while the `HAL` remains initialised, and must
 *          take any erasure guarantee it needs from the implementation it runs against.
 * @see wifi_pushMultiPskKeys
 */
INT wifi_getMultiPskKeys(INT apIndex, wifi_key_multi_psk_t *keys, INT keysNumber);


/**
 * @brief Gets the multi-PSK key for a specific client MAC address.
 *
 * This function retrieves the multi-PSK key used by the client with the specified
 * MAC address to authenticate with the specified Access Point (AP).
 * An empty `wifi_key_multi_psk_t.wifi_keyId` means that a multi-PSK key was not used for
 * authentication. The member is declared as `CHAR wifi_keyId[64]`, an array, so it is
 * never a NULL pointer; the only absence this structure can represent is an empty string,
 * and this interface states nothing more precise than that.
 *
 * @param[in] apIndex  Index of the Access Point, in the range `AP_INDEX_1` to
 *                     `AP_INDEX_24`.
 * @param[in] mac  MAC address of the client whose key is wanted: six octets in
 *                 caller-owned storage. `mac_address_t` is an array typedef, so
 *                 the parameter adjusts to a pointer and the callee receives the address
 *                 of the caller's own storage rather than a copy of it. The caller must
 *                 supply all six octets and must keep the storage valid for the duration
 *                 of the call. `Memory Model` in the HAL specification establishes who
 *                 owns that storage but not how long the implementation may hold a
 *                 pointer to it, so the caller should keep the six octets allocated and
 *                 unmoved while the `HAL` remains initialised.
 * @param[out] key  Caller-allocated `wifi_key_multi_psk_t` that receives the multi-PSK
 *                  key that client authenticated with. The caller allocates and releases
 *                  it, and the `HAL` writes into it during the call. Whether the
 *                  implementation retains the pointer beyond the call is not specified by
 *                  this interface, so the caller should keep the structure allocated and
 *                  unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the key the client used, or an empty `wifi_keyId` where
 *       the client did not authenticate with a multi-PSK key; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation
 * @retval WIFI_HAL_SUCCESS The lookup completed. An empty `wifi_keyId` is a successful
 *                          result meaning the client did not use a multi-PSK key, not a
 *                          failure.
 * @retval WIFI_HAL_ERROR   The key was not retrieved. This interface does not enumerate the
 *                          conditions that lead to this code, so a caller cannot tell from
 *                          it whether the client is associated. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note Test `key->wifi_keyId[0]` before reading `key->wifi_psk`: an empty identifier means
 *       no multi-PSK key was used and the passphrase field carries nothing meaningful.
 *       `wifi_keyId` is an array, so testing the member itself against NULL is always
 *       false and is not the test this contract calls for.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning A `wifi_key_multi_psk_t` holds a passphrase. A caller that logs or traces these
 *          structures must redact `wifi_psk`. This interface does not establish that the
 *          implementation copies the key material during the call, and does not state
 *          whether it retains the pointer afterwards, so the point at which erasing the
 *          caller's storage becomes safe is not established either: the caller must keep
 *          that storage valid and unmoved while the `HAL` remains initialised, and must
 *          take any erasure guarantee it needs from the implementation it runs against.
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
    wifi_GASConfiguration_t gas_config; /*!< Generic Advertisement Service parameters for the `VAP`
                                             - advertisement identifier, whether to pause for the
                                             server, the response timeout and the query limits. See
                                             `wifi_GASConfiguration_t` for the units of each field. */
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
    BOOL enable;                    /*!< `TRUE` where WPS onboarding is offered on the `VAP`.
                                         `methods` below names which methods are offered and `pin`
                                         the device PIN, and this interface does not state what
                                         those two hold while WPS is disabled. */
    wifi_onboarding_methods_t methods; /*!< Methods offered for WPS onboarding, as a bitmask of
                                            `wifi_onboarding_methods_t` values - those enumerators
                                            are distinct single bits, so several are set at once.
                                            `wifi_ap_capabilities_t::methodsSupported` reports the
                                            methods the `AP` can offer at all. */
    CHAR pin[WIFI_AP_MAX_WPSPIN_LEN]; /*!< Device PIN used for PIN-based onboarding, in the
                                           `WIFI_AP_MAX_WPSPIN_LEN` bytes that macro declares as 9 -
                                           eight digits plus room for a terminator. This interface
                                           states neither the digit count nor that a terminator is
                                           written, so a caller must bound any read at 9 bytes; the
                                           member is meaningful only where `methods` above offers a
                                           PIN method. */
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
    ip_addr_t ip;              /*!< Address of the primary RADIUS server, as the `ip_addr_t` of
                                    `wifi_hal_generic.h` in this preprocessor arm: a family selector
                                    with a union of an IPv4 and an IPv6 address, so a caller must
                                    set `family` before writing it. Where
                                    `WIFI_HAL_VERSION_3_PHASE2` is not defined the same setting is a
                                    45-byte text field instead, so a caller must build against the
                                    arm the platform uses. */
#else
    unsigned char ip[45];      /*!< Address of the primary RADIUS server, as text in 45 bytes in
                                    this preprocessor arm - wide enough for a full IPv6 form. This
                                    interface states neither the textual form nor whether the value
                                    is NUL-terminated, so a caller must bound any read at 45 bytes;
                                    where `WIFI_HAL_VERSION_3_PHASE2` is defined the same setting is
                                    the typed `ip_addr_t` above. */
#endif
    unsigned short port;        /*!< UDP port of the primary RADIUS server. `unsigned short`, so the
                                     whole port range is representable; this interface states no
                                     default and no value meaning `unset`. */
    char key[64];             /*!< Shared secret used with the primary RADIUS server, in 64 bytes.
                                   This interface states neither the encoding nor whether the value
                                   is NUL-terminated, so a caller must bound any read at 64 bytes
                                   and must not run an unbounded string function over it. */
    char identity[64];        /*!< NAS identity presented to the primary RADIUS server, in 64 bytes,
                                   on the same terms as `key` above: no encoding and no termination
                                   are stated, so a caller must bound any read at 64 bytes. */
#ifdef WIFI_HAL_VERSION_3_PHASE2
    ip_addr_t s_ip;              /*!< Address of the fallback RADIUS server, in the same typed
                                      `ip_addr_t` form as `ip` above. `connectedendpoint` below
                                      reports which of the two servers is in use, so a caller must
                                      not infer that from this member. */
    ip_addr_t connectedendpoint; /*!< Address of whichever of `ip` and `s_ip` the `AP` is currently
                                      authenticating against, in the same typed `ip_addr_t` form. It
                                      is the only member of this structure that reports a state
                                      rather than carrying a setting, so a caller must not write it
                                      to choose a server. */
#else
    unsigned char s_ip[45];     /*!< Address of the fallback RADIUS server, as text in 45 bytes on
                                     the same terms as `ip` above: no textual form and no
                                     termination are stated, so a caller must bound any read at 45
                                     bytes. */
    unsigned char connectedendpoint[45]; /*!< Address of whichever of `ip` and `s_ip` the `AP` is
                                              currently authenticating against, as text in 45 bytes
                                              in this preprocessor arm. No textual form and no
                                              termination are stated, so a caller must bound any
                                              read at 45 bytes; the member reports a state rather
                                              than carrying a setting, so writing it does not choose
                                              a server. */
#endif
    unsigned short s_port;      /*!< UDP port of the fallback RADIUS server, on the same terms as
                                     `port` above; this interface does not state that it must differ
                                     from it. */
    char s_key[64];           /*!< Shared secret used with the fallback RADIUS server, in 64 bytes,
                                   with no encoding and no termination stated. This interface does
                                   not state that it must differ from `key` above. */
    ip_addr_t dasip;           /*!< Address of the Dynamic Authorization Server that may send
                                    disconnect or change-of-authorization requests, as an
                                    `ip_addr_t` whose `family` member selects the valid arm of its
                                    union. Unlike the primary and secondary addresses, this member
                                    is typed in both preprocessor arms. */
    USHORT dasport;            /*!< UDP port the Dynamic Authorization Server is contacted on. This
                                    interface states no default, and its width covers the whole port
                                    range. */
    char daskey[64];          /*!< Shared secret used with the Dynamic Authorization Server, in 64
                                   bytes, with no encoding and no termination stated, so a caller
                                   must bound any read at 64 bytes. */
    UINT max_auth_attempts;    /*!< How many times a client may authenticate unsuccessfully before
                                    it is blacklisted; `blacklist_table_timeout` below gives how
                                    long that lasts, in seconds.
                                    `wifi_radius_setting_t::MaxAuthenticationAttempts` documents
                                    zero as disabling blacklisting, and this interface does not
                                    state whether the same holds for this member. */
    UINT blacklist_table_timeout; /**< Blacklist table timeout in seconds. */
    UINT identity_req_retry_interval; /**< Identity request retry interval in seconds. */
    UINT server_retries;        /*!< How many times a RADIUS request is retransmitted before the
                                     exchange is abandoned. This interface states no accepted range,
                                     no value meaning unlimited and no wait between attempts for
                                     this structure - `wifi_radius_setting_t` carries the timeout
                                     that pairs with its own retry count. */
    wifi_eap_t eap_type;       /*!< EAP method the `RADIUS` exchange uses, as one of the
                                    `wifi_eap_t` enumerators. `phase2` below carries the inner
                                    method; this interface does not state which `wifi_eap_t` values
                                    require one. */
    phase2_type phase2;        /*!< Inner EAP method used inside the tunnel the outer method
                                    establishes, as one of the `phase2_type` enumerators - EAP,
                                    MSCHAPv2, MSCHAP, PAP, CHAP or GTC. That enumeration assigns no
                                    values, so a caller must compare against the enumerators; this
                                    interface does not state what the member means where `eap_type`
                                    above needs no inner method. */
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
    wifi_security_key_type_t type; /*!< Which key `key` below carries, as one of the
                                        `wifi_security_key_type_t` enumerators - pre-shared key,
                                        passphrase, SAE and the combinations of them. It is what
                                        fixes how the 256 bytes of `key` are to be read, so it must
                                        be set before that member is written. */
    char key[256];              /*!< The key itself, in 256 bytes, read according to `type` above -
                                     a passphrase and a pre-shared key are not the same encoding.
                                     This interface states neither which of them is NUL-terminated
                                     nor a length member for the buffer, so a caller must bound any
                                     read at 256 bytes. */
} __attribute__((packed)) wifi_security_key_t;

/**
 * @brief VAP security settings.
 */
typedef struct
{
    wifi_security_modes_t mode;     /*!< Security mode the `VAP` operates under, drawn from
                                         `wifi_security_modes_t`. The member is declared as a single
                                         enumerator, so a caller must not build a bitmask of several
                                         here; `wifi_ap_capabilities_t::securityModesSupported`
                                         reports which modes the `AP` supports, in bitmask form. */
    wifi_security_modes_t repurposed_mode;     /*!< Alternative security mode, in the same
                                                    `wifi_security_modes_t` domain as `mode` above,
                                                    kept alongside the repurposed name and SSID
                                                    members elsewhere in this interface. This
                                                    interface states neither when it is used in
                                                    place of `mode` nor what it holds where there is
                                                    no alternative, so a caller must not treat it as
                                                    the mode in force. */
    wifi_encryption_method_t encr;   /*!< Cipher the `VAP` uses, as a `wifi_encryption_method_t`
                                          enumerator. It is only meaningful together with `mode`
                                          above, since a cipher is defined within a security mode. */
#if defined(WIFI_HAL_VERSION_3)
    wifi_mfp_cfg_t mfp;            /*!< Management frame protection setting, as one of the
                                        `wifi_mfp_cfg_t` enumerators - disabled, optional or
                                        required - in this preprocessor arm. Where
                                        `WIFI_HAL_VERSION_3` is not defined the same setting is the
                                        32-byte text field `mfpConfig` below instead, so a caller
                                        must build against the arm the platform uses;
                                        `wifi_getApSecurityMFPConfig()` reports it as a string in
                                        either case. */
#else
    char mfpConfig[32];           /*!< Management frame protection setting, carried as text in 32
                                       bytes in this preprocessor arm. Where `WIFI_HAL_VERSION_3` is
                                       defined the same setting is instead the typed `wifi_mfp_cfg_t
                                       mfp` member above, so a caller must build against the arm the
                                       platform uses. This interface states neither the accepted
                                       strings nor whether the value is NUL-terminated, so a caller
                                       must bound any read at 32 bytes. */
#endif
    BOOL wpa3_transition_disable; /**< Whether WPA3 transition is disabled. */
    UINT rekey_interval;         /**< Rekey interval in seconds. */
    BOOL strict_rekey;           /*!< `TRUE` where the group key is rekeyed strictly on the schedule
                                      `rekey_interval` above sets, rather than only when a station
                                      leaves. This interface states no further effect for the member
                                      and declares no call that reports it on its own. */
    UINT eapol_key_timeout;      /**< EAPOL key timeout in seconds. */
    UINT eapol_key_retries;      /*!< Number of times an EAPOL-Key message is retransmitted before
                                      the exchange is abandoned; `eapol_key_timeout` above gives the
                                      wait between attempts in seconds. This interface states no
                                      accepted range and no value that means unlimited. */
    UINT eap_identity_req_timeout; /**< EAP identity request timeout in seconds. */
    UINT eap_identity_req_retries; /*!< How many times an EAP identity request is retransmitted
                                        before the exchange is abandoned; `eap_identity_req_timeout`
                                        above gives the wait between attempts in seconds. This
                                        interface states no accepted range and no value meaning
                                        unlimited. */
    UINT eap_req_timeout;        /**< EAP request timeout in seconds. */
    UINT eap_req_retries;        /*!< How many times a subsequent EAP request is retransmitted
                                      before the exchange is abandoned; `eap_req_timeout` above
                                      gives the wait between attempts in seconds. It is separate
                                      from the identity-request retry count above, so the two may
                                      differ. */
    BOOL disable_pmksa_caching;   /*!< `TRUE` disables caching of the pairwise master key security
                                       association, so every association runs a full authentication.
                                       The sense is inverted relative to
                                       `wifi_radius_setting_t::PMKCaching`, which enables it, so a
                                       caller must not copy a value from one to the other. */
    char key_id[32];            /*!< Tag carried alongside a pre-shared key so traffic authenticated
                                     with that key can be identified downstream, in 32 bytes. This
                                     interface states neither the encoding nor whether the value is
                                     NUL-terminated, so a caller must bound any read at 32 bytes. */
    wifi_radius_settings_t repurposed_radius;   /**< To Store RADIUS configs when in Non IEEE802_1x mode */
    union
    {
        wifi_radius_settings_t radius; /*!< RADIUS settings, for a mode that authenticates
                        against a server. */
        wifi_security_key_t key;      /*!< Security key, for a mode that authenticates with a
                        pre-shared credential. */
    } u; /*!< The credential the security mode needs, selected by `mode`. `key` carries the
                        pre-shared key or passphrase - `wifi_pushMultiPskKeys()` records
                        that `wifi_setApSecurity()` sets the primary key through this arm -
                        and `radius` carries the authentication-server settings an 802.1X
                        mode needs. This interface does not tabulate which
                        `wifi_security_modes_t` value selects which arm, so a caller must
                        establish that from the mode it is configuring rather than from this
                        declaration. The arms overlap in storage, so writing one destroys
                        the other; `repurposed_radius` is a separate member kept outside the
                        union so that RADIUS settings survive while the union holds a key. */
} __attribute__((packed)) wifi_vap_security_t;

/**
 * @brief ANQP settings.
 */
typedef struct
{
    int capabilityInfoLength;       /*!< Number of octets of `capabilityInfo` below that carry
                                         advertised capabilities. The member is signed and this
                                         interface states no range and no meaning for a negative
                                         value, so a caller should treat anything outside 0 to
                                         `sizeof(capabilityInfo)` as unusable. */
    wifi_capabilityListANQP_t capabilityInfo; /*!< ANQP capability identifiers the `AP` advertises,
                                                   bounded by `capabilityInfoLength` above;
                                                   `wifi_capabilityListANQP_t` holds at most 64 and
                                                   declares no count of its own, so that length is
                                                   the only extent this structure gives. */
    int venueInfoLength;           /*!< Number of octets of `venueInfo` below that are meaningful.
                                        Signed, with no range and no meaning for a negative value
                                        stated here. */
    wifi_venueNameElement_t venueInfo; /*!< Venue group, type and name duples the `AP` advertises,
                                            bounded by `venueInfoLength` above.
                                            `wifi_venueNameElement_t` declares no count for its 16
                                            duples, so that length is the only extent this structure
                                            gives. */
    int roamInfoLength;            /*!< Number of octets of `roamInfo` below that are meaningful.
                                        The member is signed and this interface states no range and
                                        no meaning for a negative value, so a caller should treat
                                        anything outside 0 to `sizeof(roamInfo)` as unusable. */
    wifi_roamingConsortium_t roamInfo; /*!< Roaming consortium organization identifiers the `AP`
                                            advertises through ANQP, bounded by `roamInfoLength`
                                            above. `wifi_roamingConsortium_t` holds at most 32 OI
                                            duples and declares no count of its own, so that length
                                            is the only extent this structure gives. */
    wifi_ipAddressAvailabality_t ipAddressInfo; /*!< IPv4 and IPv6 availability the `AP` advertises
                                                     through ANQP, carried in the single octet
                                                     `wifi_ipAddressAvailabality_t` describes. This
                                                     structure declares no length member for it, and
                                                     that type states no bit positions, so a caller
                                                     decodes the octet against 802.11u. */
    int realmInfoLength;           /*!< Number of octets of `realmInfo` below that are meaningful.
                                        Signed, with no range and no meaning for a negative value
                                        stated here; within that member it is `nai_realm_count` that
                                        bounds the tuples. */
    wifi_naiRealmElement_t realmInfo; /*!< NAI realms the `AP` advertises through ANQP, bounded by
                                           `realmInfoLength` above. Within `wifi_naiRealmElement_t`
                                           it is that structure's own `nai_realm_count` that bounds
                                           its tuples, and the array holds at most 20 whatever the
                                           count field expresses. */
    int gppInfoLength;             /**< Length of the 3GPP cellular network information. */
    wifi_3gppCellularNetwork_t gppInfo; /*!< 3GPP Cellular Network element the `AP` advertises
                                             through ANQP, bounded by `gppInfoLength` above. Within
                                             `wifi_3gppCellularNetwork_t` the PLMN identifiers are
                                             bounded by `plmn_information.number_of_plmns`, at most
                                             16. */
    int domainInfoLength;          /*!< Number of octets of `domainNameInfo` below that are
                                        meaningful. Signed, with no range and no meaning for a
                                        negative value stated here. */
    wifi_domainName_t domainNameInfo; /*!< Domain names the `AP` advertises, bounded by
                                           `domainInfoLength` above. `wifi_domainName_t` declares no
                                           count for its four tuples, so that length is the only
                                           extent this structure gives. */
    UCHAR passpointStats[1024];    /*!< Raw statistics buffer, 1024 octets. This interface states
                                        neither an encoding for it nor a length member, so how much
                                        is populated is not established here; the counters a caller
                                        can actually read are the `domainRespCount` to
                                        `gppFailedCount` members below. */
    UINT domainRespCount;         /*!< Domain-name ANQP queries the `AP` answered successfully. This
                                       interface states neither the interval the counter covers nor
                                       when it resets, so a caller should difference successive
                                       reads; `domainFailedCount` below counts the failures
                                       separately, so the two must be added to get the total asked. */
    UINT realmRespCount;          /*!< NAI realm ANQP queries the `AP` answered successfully, on the
                                       same terms as `domainRespCount` above: no interval and no
                                       reset point are stated, and `realmFailedCount` counts the
                                       failures separately. */
    UINT gppRespCount;            /**< Number of successful 3GPP cellular network responses. */
    UINT domainFailedCount;        /*!< Domain-name ANQP queries the `AP` failed to answer. This
                                        interface does not state what counts as a failure, nor the
                                        interval covered, nor when the counter resets. */
    UINT realmFailedCount;         /*!< NAI realm ANQP queries the `AP` failed to answer, on the
                                        same terms as `domainFailedCount` above: what counts as a
                                        failure is not stated here. */
    UINT gppFailedCount;           /**< Number of failed 3GPP cellular network responses. */
    UCHAR anqpParameters[4096];     /*!< Raw ANQP parameter buffer, 4096 octets. This interface
                                         states neither an encoding for the buffer nor a length
                                         member for it, so how much is populated is not established
                                         here and a caller must treat the content as opaque. */
} __attribute__((packed)) wifi_anqp_settings_t;

/**
 * @brief Passpoint settings.
 */
typedef struct
{
    BOOL enable;                 /*!< `TRUE` where Hotspot 2.0 is offered on the `VAP`. It is the
                                      one member of this structure outside the block marked as not
                                      to be implemented in the `HAL`, together with the flags
                                      immediately below it, so a caller must not rely on the
                                      length-and-element members further down being populated. */
    BOOL gafDisable;              /*!< `TRUE` where downstream group-addressed forwarding is
                                       disabled, so the `AP` does not forward group-addressed frames
                                       to associated stations. `proxyArp` below is what keeps
                                       address resolution working when it is disabled. */
    BOOL p2pDisable;              /**< Whether P2P cross connect is disabled. */
    BOOL l2tif;                  /**< Whether Layer 2 Traffic Inspection and Filtering is enabled. */
    BOOL bssLoad;                 /*!< `TRUE` where the `BSS` advertises the BSS Load element, which
                                       is what makes a scanning station's
                                       `wifi_bss_info_t::bss_load` and its channel utilization and
                                       station count meaningful. */
    BOOL countryIE;               /*!< `TRUE` where the `BSS` advertises the Country element. This
                                       interface does not state which country it carries; the
                                       radio's own `countryCode` and `regDomain` are what set that. */
    BOOL proxyArp;                /*!< `TRUE` where the `AP` answers ARP and neighbour-discovery
                                       requests on behalf of associated stations rather than
                                       forwarding them. It is what Hotspot 2.0 pairs with
                                       `gafDisable` above, and this interface states no dependency
                                       between the two. */
/* vvvv **The following fields should not to be implemented in the HAL** vvvv */
    int capabilityInfoLength;    /*!< Number of octets of `capabilityInfo` below that are
                                      meaningful. It sits inside the block this structure marks as
                                      not to be implemented in the `HAL`, so a caller must not rely
                                      on it being populated. */
    wifi_HS2_CapabilityList_t capabilityInfo; /*!< Hotspot 2.0 capability identifiers, bounded by
                                                   `capabilityInfoLength` above. Inside the block
                                                   this structure marks as not to be implemented in
                                                   the `HAL`. */
    int opFriendlyNameInfoLength; /*!< Number of octets of `opFriendlyNameInfo` below that are
                                       meaningful. The member is signed and this interface states no
                                       range and no meaning for a negative value; it sits inside the
                                       block this structure marks as not to be implemented in the
                                       `HAL`, so a caller must not rely on it being populated. */
    wifi_HS2_OperatorFriendlyName_t opFriendlyNameInfo; /*!< Operator friendly names the `AP`
                                                             advertises, bounded by
                                                             `opFriendlyNameInfoLength` above; each
                                                             duple pairs a three-octet language code
                                                             with a name, as
                                                             `wifi_HS2_OperatorFriendlyName_t`
                                                             describes. It sits inside the block
                                                             this structure marks as not to be
                                                             implemented in the `HAL`, so a caller
                                                             must not rely on it being populated. */
    int connCapabilityLength;    /*!< Number of octets of `connCapabilityInfo` below that are
                                      meaningful. Inside the block this structure marks as not to be
                                      implemented in the `HAL`. */
    wifi_HS2_ConnectionCapability_t connCapabilityInfo; /*!< Protocol and port tuples advertised as
                                                             reachable through the `AP`, bounded by
                                                             `connCapabilityLength` above;
                                                             `wifi_HS2_ConnectionCapability_t` holds
                                                             at most 16 and declares no count of its
                                                             own. Inside the block this structure
                                                             marks as not to be implemented in the
                                                             `HAL`. */
    int realmInfoLength;          /*!< Number of octets of `realmInfo` below that are meaningful.
                                       Signed, with no range and no meaning for a negative value
                                       stated, and inside the block this structure marks as not to
                                       be implemented in the `HAL`. */
    wifi_HS2_NAI_Home_Realm_Query_t realmInfo; /*!< NAI home realm query the `AP` answers, bounded
                                                    by `realmInfoLength` above;
                                                    `wifi_HS2_NAI_Home_Realm_Query_t` carries the
                                                    realm names with their own encoding and length
                                                    fields. Inside the block this structure marks as
                                                    not to be implemented in the `HAL`. */
    wifi_HS2_WANMetrics_t wanMetricsInfo; /*!< WAN link speeds, loads and measurement duration the
                                               `AP` advertises; `wifi_HS2_WANMetrics_t` gives the
                                               unit of each field. Inside the block this structure
                                               marks as not to be implemented in the `HAL`. */
    UCHAR hs2Parameters[4096];     /*!< Raw Hotspot 2.0 parameter buffer, 4096 octets, with neither
                                        an encoding nor a length member stated here. Inside the
                                        block this structure marks as not to be implemented in the
                                        `HAL`. */
/* ^^^^ Not implemented in the hal ^^^^ */
} __attribute__((packed)) wifi_passpoint_settings_t;

/**
 * @brief Interworking settings.
 */
typedef struct
{
    wifi_InterworkingElement_t interworking; /*!< Interworking element the `VAP` advertises. Its own
                                                  `interworkingEnabled` is what says whether the
                                                  element is advertised at all, and its
                                                  `venueOptionPresent` guards its venue fields - see
                                                  `wifi_InterworkingElement_t`. */
    wifi_roamingConsortiumElement_t roamingConsortium; /*!< Roaming consortium element the `VAP`
                                                            advertises in its beacons and probe
                                                            responses:
                                                            `wifi_roamingConsortiumElement_t`
                                                            carries the three OIs that fit there
                                                            together with their lengths, which is a
                                                            smaller set than the ANQP list
                                                            `anqp.roamInfo` can hold. */
    wifi_anqp_settings_t anqp;        /*!< Access Network Query Protocol responses of the `VAP`, as
                                           `wifi_anqp_settings_t` describes - each element paired
                                           with the length that bounds it. The whole member is
                                           marked as not to be implemented in the `HAL`, so a caller
                                           must not rely on it being populated. */
    wifi_passpoint_settings_t passpoint; /*!< Hotspot 2.0 settings of the `VAP`. Every field of
                                              `wifi_passpoint_settings_t` except its `enable` sits
                                              inside the block that structure marks as not to be
                                              implemented in the `HAL`, so a caller must not rely on
                                              the rest being populated. */
} __attribute__((packed)) wifi_interworking_t;

/**
 * @brief Pre-association control settings.
 */
typedef struct
{
    char rssi_up_threshold[32];           /*!< RSSI threshold used in the pre-association decision.
                                               The name says the threshold is an upper one; this
                                               interface does not state which side of it admits a
                                               client. */
    char snr_threshold[32];              /*!< SNR threshold used in the pre-association decision.
                                              This interface states neither the comparison direction
                                              nor the unit. */
    char cu_threshold[32];               /*!< Channel-utilization threshold used in the
                                              pre-association decision. This interface states
                                              neither the comparison direction nor the unit. */
    char basic_data_transmit_rates[32];  /*!< Basic rate set advertised before association.
                                              `wifi_bitrate_t` in `wifi_hal_generic.h` names the
                                              rates this interface recognises elsewhere; this
                                              member's text form is not stated to match those names. */
    char operational_data_transmit_rates[32]; /*!< Operational rate set advertised before
                                                   association, in the same text form as
                                                   `basic_data_transmit_rates` above. */
    char supported_data_transmit_rates[32]; /*!< Supported rate set advertised before association,
                                                 in the same text form; `basic_data_transmit_rates`
                                                 above carries the required subset. */
    char minimum_advertised_mcs[32];     /*!< Lowest MCS index advertised before association. This
                                              interface states no range for the value and no
                                              relation to `wifi_radio_operationParam_t::mcs`. */
    char sixGOpInfoMinRate[32];          /**< 6G operating information minimum rate. */
    char client_deny_assoc_info[45];     /*!< Record of clients denied association, in 45 bytes.
                                              This interface states neither the layout of that text
                                              nor how many clients it can carry. */
    int  time_ms;                        /**< Time to wait for meeting minimum mgmt frames for TCM threshold calcultion */
    int  min_num_mgmt_frames;            /**< Minimum number of mgmt frames required to compute the TCM threshold. */
    char tcm_exp_weightage[32];          /*!< Weight applied to the newest sample in the moving
                                              average, as text in 32 bytes. This interface states
                                              neither the numeric form of that text, nor an accepted
                                              range, nor whether the value is `NUL`-terminated, so a
                                              caller must bound any read at 32 bytes. */
    char tcm_gradient_threshold[32];     /*!< Threshold the moving average is compared against, as
                                              text in 32 bytes. This interface states neither the
                                              numeric form nor the unit of the value, nor which way
                                              the comparison runs, nor whether the text is
                                              `NUL`-terminated, so a caller must bound any read at
                                              32 bytes. */
    char tcm_client_deny_assoc_info[64]; /*!< Transient Client Management deny and association
                                              information, in the 64 bytes this declaration
                                              dimensions. This interface states neither the layout
                                              of that text nor whether it is `NUL`-terminated, so a
                                              caller must bound any read at 64 bytes and should
                                              treat the content as opaque. */
    wifi_vap_name_t vap_name;            /*!< Name of the `VAP` these pre-association settings apply
                                              to, in the 64-byte `wifi_vap_name_t`. Termination is
                                              not stated, so a caller should bound any read at the
                                              declared length. */
} __attribute__((packed)) wifi_preassoc_control_t;

/**
 * @brief Post-association control settings.
 */
typedef struct
{
    char sampling_interval[32];          /*!< Interval between the samples taken for the
                                              post-association decision. This interface states no
                                              unit for the value. */
    char sampling_count[32];             /*!< Number of samples taken before the post-association
                                              decision is made. This interface states no range for
                                              the value. */
    char rssi_up_threshold[32];           /*!< RSSI threshold used in the post-association decision.
                                               The name says the threshold is an upper one; this
                                               interface does not state which side of it keeps a
                                               client associated. */
    char snr_threshold[32];              /*!< SNR threshold used in the post-association decision.
                                              This interface states neither the comparison direction
                                              nor the unit. */
    char cu_threshold[32];               /*!< Channel-utilization threshold used in the
                                              post-association decision. This interface states
                                              neither the comparison direction nor the unit. */
    char client_force_disassoc_info[45]; /*!< Record of clients forcibly disassociated, in 45 bytes.
                                              This interface states neither the layout of that text
                                              nor how many clients it can carry. */
    wifi_vap_name_t vap_name;            /*!< Name of the `VAP` these post-association settings
                                              apply to, in the 64-byte `wifi_vap_name_t`.
                                              Termination is not stated, so a caller should bound
                                              any read at the declared length. */
} __attribute__((packed)) wifi_postassoc_control_t;

/**
 * @brief Per-network parameters conveyed to the RADIUS server as vendor attributes.
 *
 * Used by the amenities configuration of a fronthaul BSS to describe the service
 * level the network offers.
 */
typedef struct { 
    int speed_tier;                      /*!< Service level of the network, conveyed to the RADIUS
                                              server as a vendor attribute. `int`, and this
                                              interface enumerates no tiers, states no range and
                                              gives no value meaning `not set`, so a caller should
                                              carry a value agreed with the operator rather than
                                              composing one. */
} __attribute__((packed)) network_param_config_t;

/**
 * @brief Amenities network configuration carried on a fronthaul BSS.
 *
 * A container so the amenities configuration can grow without changing the layout of
 * `wifi_front_haul_bss_t`.
 */
typedef struct {
    network_param_config_t npc;          /*!< The one per-network parameter block this container
                                              carries, whose `speed_tier` is conveyed to the RADIUS
                                              server as a vendor attribute. The container exists so
                                              the amenities configuration can grow without changing
                                              the layout of `wifi_front_haul_bss_t`. */
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
    BOOL mld_enable;      /*!< `TRUE` where the multi-link device is active, and so the flag that
                               makes the rest of this structure - and the addresses in
                               `wifi_mld_info_sta_t` - meaningful. Where it is `FALSE` this
                               interface does not state what the other members hold. */
    UINT mld_id;          /*!< Identifier of the multi-link device this link belongs to. This
                               interface states no accepted range and no scope the value is unique
                               within, and `mld_link_id` below identifies the link within it. */
    UINT mld_link_id;     /*!< Identifier of the link within the multi-link device that `mld_id`
                               above names. This interface states no range for it and no value
                               meaning `no link`, and it is meaningful only where `mld_enable` is
                               `TRUE`. */
    mac_address_t mld_addr; /*!< Address of the multi-link device as a whole, as the six raw octets
                                 of `mac_address_t` rather than text. It is the MLD address rather
                                 than any per-link address, which `wifi_mld_info_sta_t::local_addr`
                                 and `peer_addr` carry. */
} __attribute__((packed)) wifi_mld_common_info_t;

/**
 * @brief MLD information for a station.
 */
typedef struct
{
    mac_address_t local_addr; /*!< Address of this device's link within the multi-link device, as
                                   the six raw octets of `mac_address_t` rather than text. It is
                                   meaningful only where `common_info.mld_enable` below is `TRUE`. */
    mac_address_t peer_addr;  /*!< Address of the peer's link within the multi-link device, in the
                                   same six raw octets. It is meaningful only where
                                   `common_info.mld_enable` below is `TRUE`. */
    wifi_mld_common_info_t common_info; /*!< Multi-link parameters shared with the `AP` side of this
                                             interface: whether multi-link is enabled at all, the
                                             group identifier, the link identifier and the group
                                             address. `common_info.mld_enable` is what says whether
                                             the rest of this structure, `local_addr` and
                                             `peer_addr` included, describes an active multi-link
                                             device. */
} __attribute__((packed)) wifi_mld_info_sta_t;

/**
 * @brief MLD information for an Access Point.
 */
typedef struct
{
    wifi_mld_common_info_t common_info; /*!< Multi-link parameters of the `AP` - enable flag, group
                                             identifier, link identifier and group address, as
                                             `wifi_mld_common_info_t` describes. This structure
                                             carries nothing else, so `common_info.mld_enable` is
                                             the whole of what tells a caller whether multi-link is
                                             in use. */
} __attribute__((packed)) wifi_mld_info_ap_t;

/**
 * @brief Backhaul station information.
 */
typedef struct
{
    ssid_t ssid;             /*!< SSID the backhaul station connects to, in the 32-octet `ssid_t`;
                                  `repurposed_ssid` below carries the alternative. That type states
                                  that termination is not established, so a caller must bound its
                                  copy at 32 octets. */
    ssid_t repurposed_ssid;   /*!< Alternative SSID, in the 32-octet `ssid_t`, used where
                                   `ignite_enabled` below is set. That type states that termination
                                   is not established and that all 32 octets may be significant, so
                                   a caller must bound its copy at 32; this interface does not state
                                   what the member holds where there is no alternative. */
    bssid_t bssid;            /**< BSSID (if all 0, scan the SSID with probes, otherwise connect to the specified BSSID). */
    BOOL enabled;            /*!< `TRUE` where the backhaul station is to connect. `conn_status`
                                  below reports what came of it, so a caller must not read this
                                  member as a statement that a link is up. */
    wifi_connection_status_t conn_status; /*!< State of the backhaul station's connection, as one of
                                               the `wifi_connection_status_t` enumerators. */
    wifi_scan_params_t scan_params; /*!< How the backhaul station scans for its `AP`:
                                         `wifi_scan_params_t` carries a scan period in seconds and a
                                         channel, where a channel of 0 means every channel in the
                                         band. */
    wifi_vap_security_t security; /*!< Credentials and security mode the backhaul station connects
                                       with. Within `wifi_vap_security_t` it is `mode` that selects
                                       which arm of that structure's union - RADIUS settings or a
                                       pre-shared key - is valid, so a caller must set it first. */
    mac_address_t mac;        /*!< Address of the backhaul station itself, as the six raw octets of
                                   `mac_address_t` rather than text. `bssid` above is the address of
                                   the `AP` it connects to, so the two must not be confused. */
    wifi_mld_info_sta_t mld_info; /*!< Multi-link parameters of the backhaul station, including its
                                       local and peer addresses. See `wifi_mld_info_sta_t`, whose
                                       `common_info.mld_enable` is what says whether the rest
                                       describes an active multi-link device. */
    BOOL ignite_enabled; /*!< `TRUE` where the station uses the Ignite configuration, which is what
                              makes `repurposed_ssid` above meaningful. This interface states no
                              other effect for the member and declares no call that reports it. */
    BOOL valid_bh_credentials; /*!< `TRUE` where the `ssid` and the key inside `security` above are
                                    usable for a connection attempt. This interface does not state
                                    which side sets the member or what it holds before any
                                    credentials have been provisioned, so a caller must not read
                                    `FALSE` as meaning the members are unset rather than merely
                                    unvalidated. */
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
  CHAR ssid[WIFI_AP_MAX_SSID_LEN]; /*!< SSID the `BSS` advertises, in `WIFI_AP_MAX_SSID_LEN` bytes -
                                        33, one more than the 32 octets `ssid_t` uses elsewhere in
                                        this interface, which leaves room for a terminator. This
                                        interface does not state that the `HAL` writes one, so a
                                        caller must bound any read at 33 bytes rather than relying
                                        on termination. */
  BOOL enabled;                  /*!< `TRUE` where the `BSS` is to be brought up. This interface
                                      does not state what the other members of this structure mean
                                      while it is `FALSE`, and it declares no call that reports this
                                      member on its own. */
  BOOL showSsid;                 /*!< `TRUE` where the `BSS` advertises its SSID in beacons and
                                      `FALSE` where the SSID is hidden.
                                      `wifi_getApSsidAdvertisementEnable()` and
                                      `wifi_setApSsidAdvertisementEnable()` read and write that
                                      advertisement state separately, and this interface does not
                                      state whether they act on this same member; hiding the SSID is
                                      not stated to prevent a station from associating with a known
                                      SSID. */
  BOOL isolation;                /*!< `TRUE` where stations associated with this `BSS` cannot reach
                                      one another through the `AP`. `wifi_getApIsolationEnable()`
                                      and `wifi_setApIsolationEnable()` read and write that state
                                      separately, and this interface does not state whether they act
                                      on this same member. */
  INT mgmtPowerControl;          /*!< Transmit power for management frames.
                                      `wifi_getApManagementFramePowerControl()` documents the same
                                      quantity in dBm, and this member is signed accordingly; this
                                      interface states no accepted range here and does not say
                                      whether that call reads this member. */
  UINT bssMaxSta;               /*!< Largest number of stations that may associate with this `BSS`.
                                     `wifi_getApMaxAssociatedDevices()` and
                                     `wifi_setApMaxAssociatedDevices()` read and write that limit
                                     separately, and this interface states neither an accepted range
                                     nor a value meaning `no limit`. */
  BOOL bssTransitionActivated;    /*!< `TRUE` where the `BSS` has 802.11 BSS transition management
                                       turned on. `wifi_ap_capabilities_t::BSSTransitionImplemented`
                                       reports whether the `AP` implements it at all, and this
                                       interface does not state what happens if the member is set
                                       where that capability is absent. */
  BOOL nbrReportActivated;       /*!< `TRUE` where the `BSS` answers 802.11k neighbour report
                                      requests. `bssTransitionActivated` above is the separate
                                      802.11v setting, and this interface states no dependency
                                      between the two. */

  BOOL rapidReconnectEnable; /**< Whether rapid reconnect is enabled. This field should not be implemented in the HAL. */
  UINT rapidReconnThreshold; /*!< Threshold governing rapid reconnect, which `rapidReconnectEnable`
                                  above turns on. This interface states neither a unit nor an
                                  accepted range for it, and the field is marked as not to be
                                  implemented in the `HAL`, so a caller must not rely on it being
                                  honoured. */
  BOOL vapStatsEnable;      /**< Whether VAP statistics are enabled. This field should not be implemented in the HAL. */

  wifi_vap_security_t security;    /*!< Security mode, encryption and credentials of this `BSS`.
                                        Within `wifi_vap_security_t` it is `mode` that selects which
                                        arm of that structure's union is valid, so a caller must set
                                        it before writing the credentials. */
  wifi_interworking_t interworking; /*!< 802.11u and Hotspot 2.0 configuration of this `BSS`. Its
                                         `interworking.interworkingEnabled` is what says whether the
                                         Interworking element is advertised, and its `anqp` and
                                         `passpoint` members are largely marked as not to be
                                         implemented in the `HAL`. */
  wifi_preassoc_control_t preassoc;  /*!< Thresholds applied before a station associates - RSSI, SNR
                                          and channel utilization - together with the rate sets
                                          advertised. See `wifi_preassoc_control_t`, whose members
                                          are text fields whose comparison direction this interface
                                          does not state. */
  wifi_postassoc_control_t postassoc; /*!< Thresholds applied to an associated station, with the
                                           sampling interval and count that feed them. See
                                           `wifi_postassoc_control_t`, whose comparison directions
                                           are likewise not stated by this interface. */
  BOOL mac_filter_enable;           /*!< `TRUE` where the `BSS` applies its MAC filter list. It is
                                         what makes `mac_filter_mode` below meaningful, and that
                                         member says whether the list admits or excludes the
                                         addresses on it. */
  amenities_network_config_t am_config;  /*!< Amenities configuration of this `BSS`, whose
                                              `npc.speed_tier` is conveyed to the RADIUS server as a
                                              vendor attribute. `connected_building_enabled` below
                                              is the flag for the feature itself, and this interface
                                              does not state what this member holds while that is
                                              `FALSE`. */
  wifi_mac_filter_mode_t mac_filter_mode; /*!< Whether the MAC filter list admits or excludes the
                                               addresses on it, as one of the
                                               `wifi_mac_filter_mode_t` enumerators. It is
                                               meaningful only where `mac_filter_enable` above is
                                               `TRUE`. */

  BOOL sec_changed; /**< Whether security settings have changed. This field should not be implemented in the HAL. */

  wifi_wps_t wps;        /*!< WPS onboarding configuration of this `BSS` - whether it is offered,
                              which methods, and the device PIN - as `wifi_wps_t` describes. */
  BOOL wmm_enabled;     /*!< `TRUE` where Wi-Fi Multimedia is enabled on the `BSS`. `UAPSDEnabled`
                             below is a separate setting, and `wifi_setApWmmEnable()` warns that
                             U-APSD cannot be enabled without WMM. `wifi_getApWmmEnable()` and
                             `wifi_setApWmmEnable()` read and write the `AP`'s WMM state separately,
                             and this interface does not state whether they act on this same member. */
  BOOL UAPSDEnabled;    /*!< `TRUE` where unscheduled automatic power save delivery is enabled.
                             `wifi_setApWmmEnable()` warns that U-APSD cannot be enabled without
                             WMM, so this member is meaningful only where `wmm_enabled` above is
                             set; `wifi_getApWmmUapsdEnable()` and `wifi_setApWmmUapsdEnable()` read
                             and write the state separately. */
  wifi_bitrate_t beaconRate; /*!< Rate the `BSS` transmits beacons at, as a single `wifi_bitrate_t`
                                  value. `beaconRateCtl` below carries a beacon-rate setting in text
                                  form as well, and this interface does not state which of the two
                                  takes effect if they disagree. */
  mac_address_t bssid;    /**< The BSSID. This variable should only be used in the get API. It can't be used to change the interface MAC. */
  UINT wmmNoAck;         /*!< No-acknowledgement policy for the WMM access categories.
                              `wifi_setApWmmOgAckPolicy()` sets that policy one class at a time, and
                              this interface states neither how the classes are packed into this
                              `UINT` nor whether that call reads this member, so a caller should
                              preserve what it read back. */
  UINT wepKeyLength;     /*!< Length of the WEP key in use. This interface states neither the unit -
                              bits or octets - nor an accepted set of lengths, and does not state
                              what the member holds when the security mode is not WEP. */
  BOOL bssHotspot;       /*!< `TRUE` where the `BSS` is operated as a hotspot.
                              `wifi_getApHotspotElement()` and `wifi_pushApHotspotElement()` read
                              and write the hotspot element separately, and this interface states
                              neither whether they act on this same member nor how it relates to the
                              Passpoint settings in `interworking.passpoint`, so a caller must not
                              read either from the other. */
  UINT wpsPushButton;    /*!< Push-button onboarding setting of this `BSS`. This interface states no
                              accepted values for the member and no relation to `wps.methods` above;
                              `wifi_setApWpsButtonPush()` starts a push-button exchange without
                              taking this value. */
  char beaconRateCtl[32]; /*!< Beacon rate expressed as text, in 32 bytes. `beaconRate` above
                               carries the same setting as a `wifi_bitrate_t`, and this interface
                               states neither which of the two takes effect if they disagree, nor
                               the accepted strings, nor whether the value is NUL-terminated - so a
                               caller must bound any read at 32 bytes. */
  BOOL network_initiated_greylist; /*!< `TRUE` where the network may place a client on the greylist
                                        rather than only the local policy doing so.
                                        `wifi_enableGreylistAccessControl()` turns greylist access
                                        control on device-wide, and this interface does not state
                                        whether that call reads this member. */
  BOOL mcast2ucast;              /*!< `TRUE` where group-addressed frames are converted to unicast
                                      before transmission on this `BSS`.
                                      `wifi_radio_capabilities_t::mcast2ucastSupported` reports
                                      whether the radio can do it at all, and this interface does
                                      not state what happens if the member is set where that
                                      capability is absent. */
  BOOL connected_building_enabled; /*!< `TRUE` where the Connected Building feature is enabled on
                                        the `BSS`; `am_config` above carries that feature's own
                                        configuration. This interface declares no call bound to the
                                        member and states no effect for it, so a caller should
                                        preserve what it read back rather than composing a value. */
  BOOL mdu_enabled;   /**< Whether Managed Wifi Phase 2 is enabled. */
  wifi_mld_info_ap_t mld_info;        /*!< Multi-link parameters of this `BSS`. See
                                           `wifi_mld_info_ap_t`, whose `common_info.mld_enable` is
                                           what says whether the rest describes an active multi-link
                                           device. */
  BOOL hostap_mgt_frame_ctrl;        /*!< `TRUE` where management frame handling is left to the
                                          vendor's hostapd control path. This interface states
                                          neither which frames that covers nor a call that reports
                                          the member, so a caller cannot establish the effect
                                          through this interface. */
  BOOL mbo_enabled;                  /*!< `TRUE` where Multiband Operation is enabled on the `BSS`.
                                          This interface declares no call that reports or writes the
                                          member on its own and states no relation to
                                          `bssTransitionActivated` above, so a caller cannot confirm
                                          the setting or its effect through this interface. */
  BOOL   interop_ctrl;               /*!< `TRUE` where the vendor's interoperability workarounds are
                                          applied on this `BSS`. This interface states neither which
                                          behaviours those are nor a call that reports the member,
                                          so a caller cannot establish the effect through this
                                          interface; `interop_tel` below controls only the reporting
                                          of it. */
  BOOL   interop_tel;                /*!< `TRUE` where the interoperability handling reports
                                          telemetry. It controls only the reporting, not the
                                          workarounds themselves, which `interop_ctrl` above
                                          enables, so setting it does not change how the `BSS`
                                          behaves towards a client. */
  UINT    inum_sta;                   /*!< Number of stations the interoperability handling is
                                           configured for. This interface states no accepted range,
                                           no relation to `bssMaxSta` above and no member listing
                                           the stations themselves, so a caller should preserve a
                                           value it read back rather than composing one. */
  UCHAR vendor_elements[WIFI_AP_MAX_VENDOR_IE_LEN]; /**< The vendor elements to be added to beacon/probe response frames. Includes IE ID (0xDD), Length, and Payload */
  USHORT vendor_elements_len;        /**< Length of vendor_elements currently stored since it is not null terminated */
  char interop_info[64];             /*!< Vendor interop descriptor, in 64 bytes. This interface
                                          states neither the accepted values, nor the encoding, nor
                                          whether the value is `NUL`-terminated, so a caller must
                                          bound any read at 64 bytes and must not run an unbounded
                                          string function over it. */
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
    wifi_vap_index_t vap_index;     /*!< Index of the Virtual Access Point this structure describes.
                                         `wifi_vap_index_t` is a `UINT` while the `apIndex`
                                         arguments elsewhere in this interface are `INT`, so a
                                         caller passing this value into one of those calls must
                                         check that it converts. */
    wifi_vap_name_t vap_name;      /*!< Name of the Virtual Access Point, in the 64-byte
                                        `wifi_vap_name_t`. Termination is not stated, so a caller
                                        should bound any read at the declared length;
                                        `repurposed_vap_name` below carries the alternative name. */
    wifi_radio_index_t radio_index; /*!< Radio the Virtual Access Point belongs to, as a
                                         `wifi_radio_index_t`. This interface does not state whether
                                         the value is the upper-layer radio index or a PHY index, so
                                         a caller must not assume it is interchangeable with either. */
    CHAR bridge_name[WIFI_BRIDGE_NAME_LEN]; /*!< Bridge the Virtual Access Point is attached to, in
                                                 the `WIFI_BRIDGE_NAME_LEN` bytes that macro
                                                 declares as 32. This interface states neither the
                                                 encoding nor whether the value is NUL-terminated,
                                                 so a caller must bound any read at 32 bytes. */
    wifi_vap_mode_t vap_mode;      /*!< Role of the Virtual Access Point, as one of the
                                        `wifi_vap_mode_t` enumerators. It is also the selector for
                                        the `u` union below, so it must be set before that member is
                                        written and read before it is read. */
    wifi_vap_name_t repurposed_vap_name; /*!< Alternative Virtual Access Point name, in the 64-byte
                                              `wifi_vap_name_t`. This interface states neither when
                                              it is used in place of `vap_name` nor what it holds
                                              when there is no alternative. */
    CHAR repurposed_bridge_name[WIFI_BRIDGE_NAME_LEN]; /*!< Alternative bridge name, in
                                                            `WIFI_BRIDGE_NAME_LEN` bytes. This
                                                            interface states neither when it is used
                                                            in place of `bridge_name`, nor whether
                                                            the value is NUL-terminated, so a caller
                                                            must bound any read at 32 bytes. */
    union
    {
        wifi_front_haul_bss_t bss_info; /*!< Fronthaul BSS information, valid when `vap_mode`
                        is `wifi_vap_mode_ap`. */
        wifi_back_haul_sta_t sta_info; /*!< Backhaul station information, valid when
                        `vap_mode` is `wifi_vap_mode_sta`. */
    } u; /*!< The role-specific half of the Virtual Access Point description, selected by
                        `vap_mode`: `bss_info` for a fronthaul BSS and `sta_info` for a
                        backhaul station. `get_vap_ssid()` and `get_vap_security_mode()`
                        state the same rule for the members they reach through this union.
                        The arms overlap in storage, so a
                        caller must set `vap_mode` before writing this member and read
                        `vap_mode` before reading it; this interface states no arm for
                        `wifi_vap_mode_monitor`, so a caller must not read either arm in
                        that mode. */
} __attribute__((packed)) wifi_vap_info_t;

/**
 * @brief VAP information map structure.
 */
typedef struct
{
    unsigned int num_vaps;       /*!< Number of leading entries of `vap_array` below that describe a
                                      Virtual Access Point. That array is dimensioned
                                      `MAX_NUM_VAP_PER_RADIO`, so a caller must clamp its read at
                                      that bound as well. */
    wifi_vap_info_t vap_array[MAX_NUM_VAP_PER_RADIO]; /*!< The `VAP`s of one radio, of which the
                                                           leading `num_vaps` entries are populated;
                                                           `MAX_NUM_VAP_PER_RADIO` dimensions the
                                                           array at 8, so a caller must clamp its
                                                           read at that bound as well. Each entry's
                                                           own `vap_mode` selects which arm of its
                                                           union describes it. */
} __attribute__((packed)) wifi_vap_info_map_t;

/**
 * @brief Wi-Fi AP capabilities.
 */
typedef struct
{
    BOOL rtsThresholdSupported; /*!< `TRUE` where the `AP`'s radio honours an RTS/CTS threshold,
                                     which `wifi_radio_operationParam_t::rtsThreshold` carries in
                                     bytes. This interface does not state what the radio does with
                                     that value where the capability is `FALSE`. */
    wifi_security_modes_t securityModesSupported; /*!< Security modes the `AP` supports, as a
                                                       bitmask of `wifi_security_modes_t` values -
                                                       those enumerators are distinct single bits,
                                                       so several are set at once. */
    wifi_onboarding_methods_t methodsSupported; /*!< Onboarding methods the `AP` supports, as a
                                                     bitmask of `wifi_onboarding_methods_t` values -
                                                     those enumerators are distinct single bits from
                                                     0x0001 to 0x2000, so several are set at once. */
    BOOL WMMSupported; /*!< `TRUE` where the `AP` can run Wi-Fi Multimedia, which is the feature
                            `wifi_front_haul_bss_t::wmm_enabled` carries a setting for.
                            `UAPSDSupported` below is the separate U-APSD capability, and this
                            interface does not state that one implies the other. */
    BOOL UAPSDSupported; /*!< `TRUE` where the `AP` can run unscheduled automatic power save
                              delivery, which `wifi_front_haul_bss_t::UAPSDEnabled` turns on.
                              `WMMSupported` above is the separate WMM capability, and U-APSD cannot
                              run without WMM per `wifi_setApWmmEnable()`. */
    BOOL interworkingServiceSupported; /*!< `TRUE` where the `AP` can run the 802.11u interworking
                                            service. This interface does not state what
                                            `wifi_setApInterworkingServiceEnable()` does where the
                                            capability is `FALSE`, so a caller must not read a
                                            successful status from that call as proof that the
                                            service is running. */
    BOOL BSSTransitionImplemented; /*!< `TRUE` where the `AP` implements 802.11 BSS transition
                                        management, which is the feature
                                        `wifi_front_haul_bss_t::bssTransitionActivated` carries a
                                        setting for. The member reports the implementation rather
                                        than the state in force, so a caller must read the two
                                        separately. */
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
 * @post This interface does not specify whether the accessor leaves the structure, or its
 *       SSID members, unmodified; the parameter is not `const`, so a caller must not rely
 *       on the structure being untouched.
 *
 * @return A `const char *` reporting the selected SSID. The declared type is the whole of
 *         what this interface establishes about the result. It does not state the
 *         representation of the bytes, whether they form a `NUL`-terminated string, how
 *         many bytes are readable, how a NULL is to be read - NULL is the sentinel this
 *         interface defines for the result, returned when no SSID can be selected for the VAP, and it is the one
 *         value a caller must test for before any other use - whether the pointer aliases storage inside `vap` or refers somewhere else,
 *         which component owns that storage, or how long it remains valid. A caller can
 *         therefore neither determine the value's length nor bound its lifetime from this
 *         interface, and the conservative consequence is to scan for no terminator, copy
 *         out no bytes, retain the pointer nowhere and release nothing. Where a bounded
 *         read is required, read the union member the `SSID` lives in instead: the
 *         declarations do bound those - `u.bss_info.ssid` is `WIFI_AP_MAX_SSID_LEN` bytes,
 *         which is 33, and `u.sta_info.ssid` is an `ssid_t`, which is 32, with the same
 *         open question about a terminator recorded on `ssid_t` in `wifi_hal_generic.h`.
 *         The `const` qualifier does establish one thing, because it is a property of the
 *         type rather than of the implementation: the characters must not be written
 *         through this pointer.
 *
 * @warning Because none of the representation, extent, ownership, aliasing or lifetime of
 *          the returned pointer is established here, a caller that needs the `SSID` for
 *          anything beyond the immediate expression should take it from the structure
 *          member rather than from this accessor. Obtaining any of those properties for a
 *          given platform is a matter for that platform's implementer.
 *
 * @note The structure this accessor reads is memory the caller already owns, and this
 *       interface declares no allocation for the value it returns; it does not identify the
 *       storage that value refers to either way. See `Memory Model` in
 *       the HAL specification.
 * @note The call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification. That covers the `HAL`'s own state; serialising access to a
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
 * @post This interface does not specify whether the accessor leaves the structure, or its
 *       bridge-name members, unmodified; the parameter is not `const`, so a caller must
 *       not rely on the structure being untouched.
 *
 * @return A `const char *` reporting the bridge name. The declared type is the whole of
 *         what this interface establishes about the result. It does not state the
 *         representation of the bytes, whether they form a `NUL`-terminated string, how
 *         many bytes are readable, how a NULL is to be read - NULL is the sentinel this
 *         interface defines for the result, returned when the VAP has no bridge name, and it is the one
 *         value a caller must test for before any other use - whether the pointer aliases `vap->bridge_name`, `vap->repurposed_bridge_name`
 *         or storage elsewhere, which component owns that storage, or how long it remains
 *         valid. A caller can therefore neither determine the value's length nor bound its
 *         lifetime from this interface, and the conservative consequence is to scan for no
 *         terminator, copy out no bytes, retain the pointer nowhere and release nothing.
 *         Where a bounded read is required, read `bridge_name` directly: the declaration
 *         does bound it at `WIFI_BRIDGE_NAME_LEN` bytes, which is 32, though it states no
 *         terminator inside that width either. The `const` qualifier does establish one
 *         thing, because it is a property of the type rather than of the implementation:
 *         the characters must not be written through this pointer.
 *
 * @warning Because none of the representation, extent, ownership, aliasing or lifetime of
 *          the returned pointer is established here, a caller that needs the bridge name
 *          for anything beyond the immediate expression should take it from the structure
 *          member rather than from this accessor. Obtaining any of those properties for a
 *          given platform is a matter for that platform's implementer.
 *
 * @note The structure this accessor reads is memory the caller already owns, and this
 *       interface declares no allocation for the value it returns; it does not identify the
 *       storage that value refers to either way. See `Memory Model` in
 *       the HAL specification.
 * @note The call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification. Serialising access to a `wifi_vap_info_t` the caller
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
 *                  security configuration. The caller allocates and releases it, and the
 *                  callee writes into it during the call. Whether the implementation
 *                  retains the pointer beyond the call is not specified by this
 *                  interface, so the caller should keep the structure allocated and
 *                  unmoved while the `HAL` remains initialised. Its `u` union holds either
 *                  RADIUS settings or a pre-shared key, selected by `mode`.
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
 *       the HAL specification.
 * @note The call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification. Serialising access to a `wifi_vap_info_t` the caller
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
 *                 `HAL` reads the structure during the call, and whether the
 *                 implementation retains the pointer afterwards is not specified by this
 *                 interface, so the caller should keep it allocated and unmoved while the
 *                 `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the described Virtual Access Points exist on that radio; each one's
 *       arrival at an operational state is reported separately through the registered VAP
 *       status handler. On failure this interface does not specify how much of the
 *       requested change was applied, so a caller should read the affected state back
 *       rather than assume the previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The Virtual Access Points were created.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should read the
 *                          radio back with `wifi_getRadioVapInfoMap()` to establish which
 *                          Virtual Access Points exist, because this interface does not
 *                          state that creation is atomic across the map.
 *
 * @note Success means the Virtual Access Points were created, not that they are up.
 *       Register with `wifi_vapstatus_callback_register()` beforehand, or poll
 *       `wifi_getApStatus()`, to learn when each becomes operational.
 * @note Each `wifi_vap_info_t` selects its union arm through `vap_mode`: `bss_info` for an
 *       Access Point and `sta_info` for a backhaul station. Populating the wrong arm for
 *       the mode is not detected by this interface.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                  `vap_array` were populated. The caller allocates and releases it, and
 *                  the `HAL` writes into it during the call. Whether the implementation
 *                  retains the pointer beyond the call is not specified by this
 *                  interface, so the caller should keep the structure allocated and
 *                  unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds `num_vaps` populated entries, with the remaining
 *       `vap_array` elements left unspecified; on failure the output is left unspecified,
 *       so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The map was retrieved. A `num_vaps` of 0 is a success meaning
 *                          the radio hosts no Virtual Access Points.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note Read `num_vaps` first and only then the corresponding entries of `vap_array`; the
 *       elements beyond it are not defined by this interface.
 * @note This is the call that populates the structure the `get_vap_ssid()`,
 *       `get_vap_bridge_name()` and `get_vap_security_mode()` accessors read.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                      for a personal one. The `HAL` reads the structure during the call,
 *                      and whether the implementation retains the pointer afterwards is
 *                      not specified by this interface, so the caller should keep it
 *                      allocated and unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point enforces the supplied security configuration. On
 *       failure this interface does not specify how much of the requested change was
 *       applied, so a caller should read the affected state back rather than assume the
 *       previous one survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should check the
 *                          supported modes with `wifi_getAPCapabilities()` and read the
 *                          configuration back rather than assuming the update was atomic.
 *
 * @note Populate the union arm that matches `mode`: filling the RADIUS arm for a personal
 *       mode, or the key arm for an enterprise mode, is not detected by this interface.
 * @note `wifi_getAPCapabilities()` reports the modes this Access Point supports through
 *       `securityModesSupported`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The structure carries credentials in its `u` union. A caller that logs or traces
 *          it must redact them. This interface does not establish that the implementation
 *          copies them during the call, and does not state whether it retains the pointer
 *          afterwards, so the point at which erasing the caller's storage becomes safe is
 *          not established either: the caller must keep that storage valid and unmoved
 *          while the `HAL` remains initialised, and must take any erasure guarantee it
 *          needs from the implementation it runs against.
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
 *                       by `mode`. The caller allocates and releases it, and the `HAL`
 *                       writes into it during the call. Whether the implementation
 *                       retains the pointer beyond the call is not specified by this
 *                       interface, so the caller should keep the structure allocated and
 *                       unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the security configuration; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note Read `mode` before the `u` union: it is what selects which arm holds meaningful
 *       data.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The returned structure carries credentials in its `u` union. A caller that logs
 *          or traces it must redact them. This interface does not establish that the
 *          implementation copies them during the call, and does not state whether it
 *          retains the pointer afterwards, so the point at which erasing the caller's
 *          storage becomes safe is not established either: the caller must keep that
 *          storage valid and unmoved while the `HAL` remains initialised, and must take any
 *          erasure guarantee it needs from the implementation it runs against.
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
 *                             releases it, and the `HAL` writes into it during the call.
 *                             Whether the implementation retains the pointer beyond the
 *                             call is not specified by this interface, so the caller
 *                             should keep the structure allocated and unmoved while the
 *                             `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the capability flags; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The capabilities were retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note Use this before configuring an optional feature: `rtsThresholdSupported`,
 *       `WMMSupported`, `UAPSDSupported`, `interworkingServiceSupported` and
 *       `BSSTransitionImplemented` each predict whether the corresponding setter can
 *       succeed, and `securityModesSupported` and `methodsSupported` give the accepted
 *       security modes and onboarding methods.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                        allocates and releases it, and the `HAL` writes into it during
 *                        the call. Whether the implementation retains the pointer beyond
 *                        the call is not specified by this interface, so the caller
 *                        should keep the structure allocated and unmoved while the `HAL`
 *                        remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the WPS configuration; on failure the output is left
 *       unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should validate
 *                          its arguments; a failure that persists across retries should be
 *                          logged and the value treated as unavailable.
 *
 * @note `methods` is a bit set of `wifi_onboarding_methods_t` values, so test individual
 *       bits rather than comparing the whole field.
 * @note `pin` is `WIFI_AP_MAX_WPSPIN_LEN` bytes, which is 9.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                       of `wifi_onboarding_methods_t` values and whose `pin` occupies
 *                       the `WIFI_AP_MAX_WPSPIN_LEN` bytes that member declares, on the
 *                       representation terms recorded there: this interface does not
 *                       state that a terminator is written, so the caller should not rely
 *                       on one. The `HAL` reads it during the call, and whether the
 *                       implementation retains the pointer afterwards is not specified by
 *                       this interface, so the caller should keep the structure allocated
 *                       and unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the Access Point's WPS configuration matches the structure. On failure
 *       this interface does not specify how much of the requested change was applied, so a
 *       caller should read the affected state back rather than assume the previous one
 *       survived.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should check
 *                          `methodsSupported` from `wifi_getAPCapabilities()` and re-check
 *                          the structure against what the `@param` entries state rather
 *                          than retrying unchanged.
 *
 * @note Request only methods that `wifi_getAPCapabilities()` reports in `methodsSupported`.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *                          false when it is not. The caller allocates and releases it,
 *                          and the `HAL` writes into it during the call. Whether the
 *                          implementation retains the pointer beyond the call is not
 *                          specified by this interface, so the caller should keep the
 *                          variable allocated and unmoved while the `HAL` remains
 *                          initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the output holds the libhostapd indication; on failure the output is
 *       left unspecified, so a caller must not read it unless the call succeeded.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The indication was retrieved.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should treat
 *                          libhostapd use as unknown and avoid
 *                          `wifi_updateLibHostApdConfig()`.
 *
 * @note This call takes no Access Point index because the answer is a property of the
 *       platform rather than of one Access Point.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the libhostapd configuration for this Access Point reflects the Access
 *       Point's current settings. On failure this interface does not specify how much of
 *       the configuration was rewritten, and it offers no call that reads the generated
 *       configuration back, so a caller must treat it as being in an unknown state.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration was updated.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should check
 *                          `wifi_getLibhostapd()` first: this interface does not state what
 *                          this call does on a platform that does not use libhostapd.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @see wifi_getLibhostapd
 */
INT wifi_updateLibHostApdConfig(int apIndex);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

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
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 */
typedef INT(* wifi_vapstatus_callback)(INT apIndex, wifi_vapstatus_t status);

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */

/**
 * @brief Installs the caller's handler for VAP up and down transitions.
 *
 * After registration the `HAL` reports each transition of a Virtual Access Point
 * between `wifi_vapstatus_down` and `wifi_vapstatus_up` through the supplied handler,
 * which is how a caller learns that a VAP created by `wifi_createVAP()` has actually
 * come up. This interface does not state whether registering a second handler
 * replaces the first, adds to it or is rejected, so a caller must not depend on any
 * of those outcomes.
 *
 * @param[in] callback  Handler to install, of type `wifi_vapstatus_callback`. The
 *                      `HAL` keeps this function pointer after the call returns,
 *                      since it invokes the handler later, so the function must
 *                      remain callable for as long as notifications are wanted. This
 *                      interface declares no call that removes a handler and states
 *                      no end to a registration, so it does not establish when the
 *                      `HAL` stops using the pointer. The effect of passing NULL is
 *                      not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the handler is installed and is invoked on each subsequent VAP
 *       status change. On failure this interface does not specify whether the handler was
 *       installed, nor whether a previously registered handler is still in place, so a
 *       caller should assume neither.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was installed.
 * @retval WIFI_HAL_ERROR   The handler could not be installed. Which conditions produce
 *                          this code rather than another is not specified by this
 *                          interface; `Internal Error Handling` in
 *                          the HAL specification reserves `WIFI_HAL_NOT_READY` for a
 *                          `HAL` that cannot yet serve a request, so a caller should act
 *                          on the code it actually receives. The caller should log the
 *                          failure and fall back to polling VAP state through
 *                          `wifi_getRadioVapInfoMap()` rather than waiting for a
 *                          notification that may never arrive.
 *
 * @execution callback
 * @sideeffect On success, installs `callback` as the handler the `HAL` invokes on
 *             each VAP status transition, and keeps that function pointer after this
 *             call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous; delivery of `wifi_vapstatus_callback`
 *       is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 *
 * @see wifi_vapstatus_callback
 * @see wifi_vapstatus_t
 * @see wifi_createVAP
 */
INT wifi_vapstatus_callback_register(wifi_vapstatus_callback callback);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

/**
 * @brief Callback function invoked for logging catastrophic failures.
 *
 * This function must not suspend and must not invoke any blocking system calls.
 *
 * @param[in] fmt Format string for the log message.
 *
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note `fmt` is owned by the `HAL` and this interface states no lifetime for it
 *       beyond this call, so the handler must copy whatever it needs before returning
 *       and must neither retain nor release it, per `Asynchronous Notification
 *       Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 */
typedef INT(* wifi_analytics_callback)(CHAR *fmt, ...);

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */

/**
 * @brief Installs the caller's log sink for catastrophic `HAL` failures.
 *
 * After registration the `HAL` routes its catastrophic-failure messages to the
 * supplied handler, which lets the caller's own logging and telemetry pipeline carry
 * them rather than losing them inside the vendor layer. This interface does not
 * state whether registering a second handler replaces the first, adds to it or is
 * rejected, so a caller must not depend on any of those outcomes.
 *
 * @param[in] callback  Handler to install, of type `wifi_analytics_callback`. It takes
 *                      a `printf`-style format string and a variable argument list, so
 *                      the implementation must consume the arguments with the `v`
 *                      family of formatting calls. The `HAL` keeps this function
 *                      pointer after the call returns, since it invokes the handler
 *                      later, so the function must remain callable for as long as
 *                      failure messages are wanted. This interface declares no call
 *                      that removes a handler and states no end to a registration, so
 *                      it does not establish when the `HAL` stops using the pointer.
 *                      The effect of passing NULL is not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. This interface does not specify the outcome of a call
 *      made beforehand: neither the status code nor the effect of the call is
 *      established, so a caller must not rely on either.
 * @post On success the handler is installed and receives subsequent failure messages.
 *       On failure this interface does not specify whether the handler was installed, nor
 *       whether a previously registered handler is still in place, so a caller should
 *       assume neither.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was installed.
 * @retval WIFI_HAL_ERROR   The handler could not be installed. Which conditions produce
 *                          this code rather than another is not specified by this
 *                          interface; `Internal Error Handling` in
 *                          the HAL specification reserves `WIFI_HAL_NOT_READY` for a
 *                          `HAL` that cannot yet serve a request, so a caller should act
 *                          on the code it actually receives. The caller should log the
 *                          failure through its own path and continue; losing this sink
 *                          degrades diagnostics but does not affect Wi-Fi operation.
 *
 * @execution callback
 * @sideeffect On success, installs `callback` as the sink the `HAL` routes its
 *             catastrophic-failure messages to, and keeps that function pointer
 *             after this call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous; invocation of
 *       `wifi_analytics_callback` is asynchronous and may occur on a `HAL` thread.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler, which
 *       is called on failure paths where blocking would compound the problem.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning `fmt` is storage the `HAL` owns and `wifi_analytics_callback` states no lifetime
 *          for it beyond that call; that typedef states nothing at all about any pointer
 *          passed among the variadic arguments. The handler must therefore format or copy
 *          what it needs before returning, and must neither retain nor release `fmt` or any
 *          storage the variadic arguments refer to.
 *
 * @see wifi_analytics_callback
 */
INT wifi_hal_analytics_callback_register(wifi_analytics_callback callback);

/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */

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
 * @returns The status the implementer of the handler returns to the `HAL`. The direction
 *          is inverted relative to the rest of this header: this value travels from the
 *          handler into the `HAL`, not from the `HAL` to a caller.
 * @retval WIFI_HAL_SUCCESS The handler accepted the notification.
 * @retval WIFI_HAL_ERROR   The handler could not process the notification.
 *
 * @execution callback
 * @note The `HAL` may enter this handler on a `HAL`-owned thread, so the handler must
 *       serialize its own access to caller state; see `Threading Model` and
 *       `Asynchronous Notification Model` in the HAL specification.
 * @note This interface does not state whether or how the `HAL` acts on a failure the
 *       handler returns, so a handler must not use the return value to steer the
 *       `HAL`.
 */
typedef INT(* wifi_wpsEvent_callback)(INT apIndex, wifi_wps_ev_t event);

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */

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
 *      Startup` in the HAL specification. This interface does not specify the
 *      outcome of a call made beforehand: neither the status code nor whether a
 *      handler is installed is established, so a caller must not rely on either.
 *      Register before starting a `WPS` session with `wifi_setApWpsButtonPush()`
 *      or `wifi_setApWpsEnrolleePin()`, otherwise the session's outcome is not
 *      reported.
 * @post On success the handler is installed and is invoked for each subsequent `WPS`
 *       event. On failure this interface does not specify whether the handler was
 *       installed, nor whether a previously registered handler is still in place, so a
 *       caller should assume neither.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The handler was installed.
 * @retval WIFI_HAL_ERROR   The handler could not be installed. Which conditions produce
 *                          this code rather than another is not specified by this
 *                          interface; `Internal Error Handling` in
 *                          the HAL specification reserves `WIFI_HAL_NOT_READY` for a
 *                          `HAL` that cannot yet serve a request, so a caller should act
 *                          on the code it actually receives. The caller should log the
 *                          failure and treat `WPS` outcomes as unobservable, since a
 *                          `WPS` session started without a handler completes silently.
 *
 * @execution callback
 * @sideeffect On success, installs `callback` as the handler the `HAL` invokes for
 *             each `WPS` result event, replacing the previously registered handler -
 *             the one registration this interface does state replacement for, in the
 *             description above - and keeps that function pointer after this call
 *             returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous; delivery of `wifi_wpsEvent_callback`
 *       is asynchronous.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
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
