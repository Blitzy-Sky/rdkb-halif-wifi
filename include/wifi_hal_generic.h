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

/**********************************************************************
    Notes:

    What is new for 3.0.6

      1. Added new security types wifi_security_key_type_saeext, wifi_security_key_type_sae_saeext
         and wifi_security_key_type_psk_sae_saeext to wifi_security_key_type_t structure in
         wifi_hal_ap.h and wifi_encryption_aes_gcmp256 and wifi_encryption_gcmp256 to
         wifi_encryption_method_t structure in wifi_hal_generic.h file.

    What is new for 3.0.5

      1. Added new variables cli_MLDEnable and cli_MLDAddr to wifi_associated_dev3_t structure
         and added new structures wifi_multi_link_info_t, wifi_multi_link_modes_t and modified
         wifi_multi_link_bands_t to include 5GL and 5GH radios in wifi_hal_generic.h file for wifi7.

    What is new for 3.0.4

      1. Added new structure wifi_radioTemperature_t and hal wifi_hal_getRadioTemperature
         in wifi_hal_radio.h file

    What is new for 3.0.3

      1. Added new structures wifi_mld_common_info_t, wifi_mld_info_sta_t, wifi_mld_info_ap_t
         in wifi_hal_ap.h, wifi_radio_11be_puncturing_info_t in wifi_hal_radio.h and
         wifi_multi_link_bands_t in wifi_hal_generic.h file also WIFI_CHANNELBANDWIDTH_320MHZ
         to structure wifi_channelBandwidth_t and WIFI_80211_VARIANT_BE to structure
         wifi_ieee80211Variant_t in wifi_hal_generic.h file to support WiFi 7 functionality.

    What is new for 3.0.2

      1. Added cli_TxFrames, cli_RxRetries, cli_RxErrors field to wifi_associated_dev3_t
         structure in wifi_hal_generic.h file

    What is new for 3.0.1

      1. Added cli_activeNumSpatialStreams field to wifi_associated_dev3_t structure
         in wifi_hal_generic.h file
      2. Added cli_CapableNumSpatialStreams field to wifi_associated_dev_t structure
         in wifi_hal_ap.h file


**********************************************************************/

#ifndef __WIFI_HAL_GENERIC_H__
#define __WIFI_HAL_GENERIC_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
#ifndef ULLONG
#define ULLONG unsigned long long  /**< Unsigned long long type. */
#endif

#ifndef ULONG
#define ULONG unsigned long    /**< Unsigned long type. */
#endif

#ifndef USHORT
#define USHORT unsigned short   /**< Unsigned short type. */
#endif

#ifndef BOOL
#define BOOL unsigned char     /**< Boolean type. */
#endif

#ifndef CHAR
#define CHAR char          /**< Character type. */
#endif

#ifndef UCHAR
#define UCHAR unsigned char    /**< Unsigned character type. */
#endif

#ifndef INT
#define INT int            /**< Integer type. */
#endif

#ifndef UINT
#define UINT unsigned int       /**< Unsigned integer type. */
#endif

#ifndef TRUE
#define TRUE 1          /**< Boolean true value. */
#endif

#ifndef FALSE
#define FALSE 0         /**< Boolean false value. */
#endif

#ifndef ENABLE
#define ENABLE 1         /**< Enable value. */
#endif

#ifndef RETURN_OK
/**
 * @brief Alias spelling of the success status, with the value `0`.
 *
 * `RETURN_OK` and `WIFI_HAL_SUCCESS` are two names for one outcome: both are defined here
 * with the value `0`, so a caller may test a returned status against either name, or
 * against a literal `0`, and get the same answer. The two spellings are mixed across the
 * interface - `wifi_hal_sta.h`, for instance, documents its returns entirely in the alias
 * spelling while most of the surface uses the `WIFI_HAL_*` spelling - which is why a
 * `@retval` list on a declaration uses one spelling or the other rather than both. The
 * equivalence is what makes those lists interchangeable; see `Internal Error Handling` in
 * `docs/pages/halSpec.md`, which tabulates the whole vocabulary.
 *
 * @note The definition is guarded, so a caller that already defines `RETURN_OK` keeps its
 *       own definition and this header does not override it. A caller that redefines it to
 *       anything other than `0` breaks the equivalence stated above, because the `HAL`
 *       returns the value and not the name.
 *
 * @see WIFI_HAL_SUCCESS
 * @see RETURN_ERR
 */
#define RETURN_OK 0        /*!< Success; numerically identical to `WIFI_HAL_SUCCESS`. */
#endif

#ifndef RETURN_ERR
/**
 * @brief Alias spelling of the generic failure status, with the value `-1`.
 *
 * `RETURN_ERR` and `WIFI_HAL_ERROR` are two names for one outcome: both are defined here
 * with the value `-1`, so a caller may test for either. Neither name separates one cause of
 * failure from another - the conditions a particular function reports it for are stated in
 * that function's own `@retval` list, and the five more specific codes below are the rest
 * of the vocabulary this interface defines.
 *
 * @note The definition is guarded, so a caller's own definition wins, with the same caveat
 *       as `RETURN_OK`: the equivalence holds for the value `-1` and not for the name.
 * @note A caller must not read an out-parameter after a call that reported failure; on
 *       failure the contents are unspecified, per `Caller Responsibilities` under `Memory
 *       Model` in `docs/pages/halSpec.md`.
 *
 * @see WIFI_HAL_ERROR
 * @see RETURN_OK
 */
#define RETURN_ERR -1       /*!< Generic failure; numerically identical to `WIFI_HAL_ERROR`. */
#endif

/**
 * @brief Successful completion of a `HAL` call, with the value `0`.
 *
 * This is the `WIFI_HAL_*` spelling of the success code and is numerically identical to the
 * `RETURN_OK` alias above, so the two are interchangeable in a comparison. It is the only
 * status after which a caller may read the out-parameters a call was given, per `Caller
 * Responsibilities` under `Memory Model` in `docs/pages/halSpec.md`.
 *
 * The six codes below are the failure vocabulary this interface defines. `Internal Error
 * Handling` in `docs/pages/halSpec.md` fixes that vocabulary once; it does not claim that
 * every function returns every code. Which codes a given function can actually return is a
 * per-function fact, stated in that function's own `@retval` list, and a caller should
 * treat a code outside that list as a fault in the implementation rather than as a
 * documented outcome.
 *
 * @see RETURN_OK
 */
#define WIFI_HAL_SUCCESS            0  /*!< The operation completed; out-parameters may be read. Identical to `RETURN_OK`. */
#define WIFI_HAL_ERROR             -1  /*!< The operation failed, without distinguishing why. Each declaration states the conditions it uses this code for, which for most of the surface include an out-of-range index, a `NULL` output pointer, and a vendor layer that could not supply the value. Identical to `RETURN_ERR`. */
#define WIFI_HAL_INTERNAL_ERROR    -2  /*!< The implementation failed internally. The caller's arguments were not the cause, so retrying the identical call is unlikely to help; the caller should log the failure and treat the operation as unavailable. */
#define WIFI_HAL_UNSUPPORTED       -3  /*!< The platform does not implement the operation. This is a capability answer rather than a fault: the caller should treat the feature as absent and not retry, and should use `wifi_getHalCapability()` to establish what the platform supports. */
#define WIFI_HAL_INVALID_ARGUMENTS -4  /*!< An argument was unacceptable, for instance an index outside the declared range or a `NULL` pointer where the call writes a result. The caller should correct the call rather than repeat it. */
#define WIFI_HAL_INVALID_VALUE     -5  /*!< An argument was well formed but its value is not one this interface accepts for that parameter. The caller should choose a value the declaration or the capability report admits. */
#define WIFI_HAL_NOT_READY         -6  /*!< The subsystem cannot yet serve the request, for instance before `wifi_init()` has completed. The caller may retry once the pre-condition holds. */

#ifndef RADIO_INDEX_1
#define RADIO_INDEX_1 1  /**< Radio index 1. */
#define RADIO_INDEX_2 2  /**< Radio index 2. */
#define RADIO_INDEX_3 3  /**< Radio index 3. */

#define AP_INDEX_1 1    /**< Access Point index 1. */
#define AP_INDEX_2 2    /**< Access Point index 2. */
#define AP_INDEX_3 3    /**< Access Point index 3. */
#define AP_INDEX_4 4    /**< Access Point index 4. */
#define AP_INDEX_5 5    /**< Access Point index 5. */
#define AP_INDEX_6 6    /**< Access Point index 6. */
#define AP_INDEX_7 7    /**< Access Point index 7. */
#define AP_INDEX_8 8    /**< Access Point index 8. */
#define AP_INDEX_9 9    /**< Access Point index 9. */
#define AP_INDEX_10 10   /**< Access Point index 10. */
#define AP_INDEX_11 11   /**< Access Point index 11. */
#define AP_INDEX_12 12   /**< Access Point index 12. */
#define AP_INDEX_13 13   /**< Access Point index 13. */
#define AP_INDEX_14 14   /**< Access Point index 14. */
#define AP_INDEX_15 15   /**< Access Point index 15. */
#define AP_INDEX_16 16   /**< Access Point index 16. */
#define AP_INDEX_17 17   /**< Access Point index 17. */
#define AP_INDEX_18 18   /**< Access Point index 18. */
#define AP_INDEX_19 19   /**< Access Point index 19. */
#define AP_INDEX_20 20   /**< Access Point index 20. */
#define AP_INDEX_21 21   /**< Access Point index 21. */
#define AP_INDEX_22 22   /**< Access Point index 22. */
#define AP_INDEX_23 23   /**< Access Point index 23. */
#define AP_INDEX_24 24   /**< Access Point index 24. */
#endif

#ifdef WIFI_HAL_VERSION_3
#define MAX_NUM_RADIOS 3  /**< Maximum number of radios. */
#else
#define MAX_NUM_RADIOS 2  /**< Maximum number of radios. */
#endif

#define MAX_NUM_VAP_PER_RADIO 8  /**< Maximum number of VAPs per radio. */

#define MAC_STR_LEN 18  /**< Length of a MAC address string. */

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6  /**< Length of an Ethernet address. */
#endif /* ETHER_ADDR_LEN */

#define ACL_MACADDR_SIZE 18  /**< Size of an ACL MAC address. */
#define ACL_MACFLT_NUM 16    /**< Number of ACL MAC filters. */
#define MAC_LIST_SIZE ((ACL_MACADDR_SIZE * WL_MACADDR_SIZE) + 2)  /**< Size of a MAC list. */
#define ACL_MAC_ARRAY_MAX 512    /**< Maximum size of an ACL MAC array. */
#define ASSOC_MAC_ARRAY_MAX 1024 /**< Maximum size of an associated MAC array. */
#define SUPPORTED_STANDARDS_RADIO2_4GHZ "b,g,n"  /**< Supported standards for 2.4GHz radio. */
#define SUPPORTED_STANDARDS_RADIO5GHZ "a,n,ac"  /**< Supported standards for 5GHz radio. */
#define RESTORE_CNFG_FILE_NAME "/data/.nvram_restore_cfg.txt"  /**< Restore configuration file name. */
#define NVRAM_LINE_MAX (1024)  /**< Maximum length of an NVRAM line. */

// Defines for HAL version 3.0.6
#define WIFI_HAL_MAJOR_VERSION 3        /**< Wi-Fi HAL major version. */
#define WIFI_HAL_MINOR_VERSION 0        /**< Wi-Fi HAL minor version. */
#define WIFI_HAL_MAINTENANCE_VERSION 6  /**< Wi-Fi HAL maintenance version. */

#define WIFI_HAL_VERSION \
    (WIFI_HAL_MAJOR_VERSION * 1000 + WIFI_HAL_MINOR_VERSION * 10 + WIFI_HAL_MAINTENANCE_VERSION) /**< Wi-Fi HAL version. */

#define MAX_NUM_TWT_SESSION 50    /**< Maximum number of TWT sessions. */
#define MAX_STA_PER_SESSION 100   /**< Maximum number of stations per TWT session. */
#define MAX_BSR 32            /**< Maximum BSR. */
#define MAX_RU_ALLOCATIONS 74   /**< Maximum RU allocations. */

#define CONN_NEW 1 /**< Mode 1: When a client connect or associate message is received by the AP, then the WiFi HAL layer must invoke `wifi_apAssociatedDevice_callback` with `event_type`: `CONN_NEW`. */
#define CONN_RENEW 2 /**< Mode 2: If the AP status for a client is connected or associated and the AP receives a client connect or associate message from this client, `wifi_apAssociatedDevice_callback` needs to be invoked from the HAL layer to notify RDKB with `event_type`: `CONN_RENEW`. */
#define CONN_RECONN_AFTER_INACTIVITY 3 /**< Mode 3: If the AP changes a client's status to "disconnected" due to the AP's client inactivity timeout (RDKB could read this timeout from `wifi_getRadioClientInactivityTimout`) and then the client re-connects or associates back to the same AP, then the WiFi HAL layer must invoke a `wifi_apAssociatedDevice_callback` with `event_type`: `CONN_RECONN_AFTER_INACTIVITY`. */

#define KI1_VER_MASK 0xf8 /**< Key Information 1 (KI1) version mask. */
#define KI1_PW_KEY 0x08 /**< KI1 pairwise key bit. */
#define KI1_INSTALL 0x40 /**< KI1 install bit. */
#define KI1_ACK 0x80 /**< KI1 acknowledgement bit. */
#define KI1_MSG1_BITS (KI1_PW_KEY | KI1_ACK) /**< KI1 bits for message 1. */
#define KI1_MSG3_BITS (KI1_PW_KEY | KI1_INSTALL | KI1_ACK) /**< KI1 bits for message 3. */
#define KI0_MIC 0x01 /**< Key Information 0 (KI0) MIC bit. */
#define KI0_SECURE 0x02 /**< KI0 secure bit. */
#define KI0_ENCR 0x10 /**< KI0 encryption bit. */
#define KI0_MSG3_BITS (KI0_MIC | KI0_SECURE | KI0_ENCR) /**< KI0 bits for message 3. */
#define KI0_MSG4_BITS (KI0_MIC | KI0_SECURE) /**< KI0 bits for message 4. */

#define KEY_MSG_1_OF_4(msg) \
    ((((msg)->key_info[1] & KI1_VER_MASK) == KI1_MSG1_BITS) && ((msg)->key_info[0] == 0)) /**< Checks if a key message is message 1 of 4. */

#define KEY_MSG_2_OF_4(msg) \
    ((((msg)->key_info[1] & KI1_VER_MASK) == KI1_PW_KEY) && ((msg)->key_info[0] == KI0_MIC)) /**< Checks if a key message is message 2 of 4. */

#define KEY_MSG_3_OF_4(msg) \
    ((((msg)->key_info[1] & KI1_VER_MASK) == KI1_MSG3_BITS) && ((msg)->key_info[0] == KI0_MSG3_BITS)) /**< Checks if a key message is message 3 of 4. */

#define KEY_MSG_4_OF_4(msg) \
    ((((msg)->key_info[1] & KI1_VER_MASK) == KI1_PW_KEY) && ((msg)->key_info[0] == KI0_MSG4_BITS)) /**< Checks if a key message is message 4 of 4. */
#define MAX_IE_ELEMENT_LEN 1024 /*!< Size in bytes of the information-element buffer this interface carries for one `BSS`. It is the declared length of the `ie` member of `wifi_bss_info_t`, so it bounds the elements copied out of a probe response or beacon; the number of bytes actually populated is reported separately in `ie_len` and a caller must read no further than that. */
/**
 * @brief Default length of device information fields.
 */
#define DEFAULT_DEVICE_FIELD_LEN 64
/**********************************************************************
                STRUCTURE DEFINITIONS
**********************************************************************/
typedef unsigned char mac_address_t[6]; /*!< Six raw octets of a MAC address. This is not a text
                        form and is not NUL-terminated - `mac_addr_str_t` below carries the
                        printable form, and `bssid_t` is an alias of this type. This interface does
                        not state the octet order, so a caller should move the array as a whole
                        rather than reordering it. */
typedef char mac_addr_str_t[18]; /*!< Printable form of a MAC address, in 18 bytes. Six octets
                                      written as colon-separated pairs occupy 17 characters, so the
                                      width leaves room for a terminator, but this interface states
                                      neither the separator, the digit case nor that a terminator is
                                      written - a caller must bound any read at 18 bytes.
                                      `mac_address_t` above is the raw six-octet form. */
/**
 * @brief The six raw octets of a `BSSID`.
 *
 * `bssid_t` is an alias of `mac_address_t`, so it is the same six-octet array type and the
 * two are interchangeable in an assignment; the distinct name records that the address
 * identifies a `BSS` rather than a station. Being an array type, a `bssid_t` parameter of a
 * function adjusts to a pointer to its first element, so a callee sees the caller's storage
 * and not a copy: the caller allocates and owns the six octets. `Memory Model` in
 * `docs/pages/halSpec.md` establishes who owns that storage but not how long an
 * implementation may hold a pointer to it, so a caller should keep the six octets
 * allocated and unmoved while the `HAL` remains initialised rather than assuming a
 * lifetime that ends with the call. This is
 * the raw form; `mac_addr_str_t` is the printable 18-byte form, and no function in this
 * interface converts between them. The interface states no octet order for either form, so
 * a caller pairing them must follow the platform's own convention.
 *
 * @note An all-zero address is not reserved by this type. Where a declaration gives it a
 *       meaning - `wifi_hal_ap.h` uses it to mean "scan the SSID with probes rather than
 *       target one BSS" - that meaning belongs to the declaration and not to the type.
 *
 * @see mac_address_t
 * @see wifi_bss_info_t
 */
typedef mac_address_t bssid_t;
typedef char ssid_t[32]; /*!< An `SSID` in 32 octets of caller-owned storage, which is the full length IEEE 802.11 allows. This interface does not state whether the 32 octets are a `NUL`-terminated string or 32 significant octets, and `wifi_hal_ap.h` sizes the equivalent field at `WIFI_AP_MAX_SSID_LEN`, which is 33 - so a caller copying into or out of an `ssid_t` must bound the copy at 32 and must not assume a terminator is present. */
/**
 * @brief Index that names one physical radio.
 *
 * The indices this interface defines are `RADIO_INDEX_1` to `RADIO_INDEX_3`, and
 * `MAX_NUM_RADIOS` bounds how many a build admits - three under `WIFI_HAL_VERSION_3` and
 * two otherwise. Which of them a platform actually provisions is a platform fact rather
 * than an interface fact: `wifi_getHalCapability()` reports the radio inventory, and a
 * caller should take the range from that report instead of assuming the whole span is
 * present.
 *
 * The type is unsigned, while much of the surface passes the same index as `INT`; the two
 * forms name the same radio and this interface states no negative index, so a caller
 * converting between them should reject a negative value rather than let it wrap.
 *
 * @see wifi_getHalCapability
 * @see wifi_radio_capabilities_t
 */
typedef UINT wifi_radio_index_t;
/**
 * @brief Compatibility alias for a 32-bit unsigned integer.
 *
 * No declaration in this interface takes or returns `u_int32_t`; it is defined for source
 * compatibility with vendor code that expects the BSD spelling. The name is also provided by
 * `<sys/types.h>` on a glibc-based system, where it is `unsigned int` as it is here, so a
 * translation unit that includes both compiles. Unlike the primitive-type macros at the top
 * of this header - `UINT`, `UCHAR` and their siblings, each wrapped in an `#ifndef` - this
 * typedef is unguarded, so on a platform whose `u_int32_t` is a different underlying type the
 * two definitions conflict and the translation unit fails to compile.
 *
 * @note A caller that wants a width guarantee should use `uint32_t` from `<stdint.h>`
 *       rather than this alias, which guarantees only what `unsigned int` guarantees on the
 *       platform being built for.
 */
typedef unsigned int u_int32_t;
typedef char nas_id_t[49]; /*!< `RADIUS` NAS identifier, in a 49-byte array. This interface states
                                neither the encoding nor whether the value is NUL-terminated, so a
                                caller should bound any read at the declared 49 bytes and must not
                                run an unbounded string function over it. */
typedef unsigned char r0r1_key_t[16]; /*!< Shared key used to protect key transfers between R0 and
                                           R1 key holders, as 16 raw octets. It is not text and
                                           carries no terminator; `r0r1_key_str_t` below is the
                                           33-byte form, which is wide enough for the 32 hexadecimal
                                           digits of the same key plus a terminator, though this
                                           interface does not state that the two are always
                                           populated together. */
typedef char r0r1_key_str_t[33]; /*!< Printable form of an R0/R1 key, in 33 bytes. The 16 octets of
                                      `r0r1_key_t` take 32 characters as hexadecimal, so the width
                                      leaves room for a terminator, but this interface states
                                      neither the digit case nor that a terminator is written; a
                                      caller must bound any read at 33 bytes. */
typedef char wifi_interface_name_t[32]; /*!< Name of a network interface, in 32 bytes. This
                                             interface states neither the encoding nor whether the
                                             value is NUL-terminated, so a caller must bound any
                                             read at 32 bytes and must not run an unbounded string
                                             function over it. */
typedef char wifi_vap_name_t[64]; /*!< Name of a Virtual Access Point, in 64 bytes, on the same
                                       terms as `wifi_interface_name_t` above: no encoding and no
                                       termination are stated, so a caller must bound any read at 64
                                       bytes. */

/**
 * @brief Wi-Fi security mode types.
 */
typedef enum
{
    wifi_security_mode_none = 0x00000001, /**< No security. */
    wifi_security_mode_wep_64 = 0x00000002, /**< WEP 64-bit. */
    wifi_security_mode_wep_128 = 0x00000004, /**< WEP 128-bit. */
    wifi_security_mode_wpa_personal = 0x00000008, /**< WPA Personal. */
    wifi_security_mode_wpa2_personal = 0x00000010, /**< WPA2 Personal. */
    wifi_security_mode_wpa_wpa2_personal = 0x00000020, /**< WPA/WPA2 Personal. */
    wifi_security_mode_wpa_enterprise = 0x00000040, /**< WPA Enterprise. */
    wifi_security_mode_wpa2_enterprise = 0x00000080, /**< WPA2 Enterprise. */
    wifi_security_mode_wpa_wpa2_enterprise = 0x00000100, /**< WPA/WPA2 Enterprise. */
    wifi_security_mode_wpa3_personal = 0x00000200, /**< WPA3 Personal. */
    wifi_security_mode_wpa3_transition = 0x00000400, /**< WPA3 Transition. */
    wifi_security_mode_wpa3_enterprise = 0x00000800, /**< WPA3 Enterprise. */
    wifi_security_mode_enhanced_open = 0x00001000, /**< Enhanced Open. */
    wifi_security_mode_wpa3_compatibility = 0x00002000 /**< WPA3 Personal Compatibility */
} wifi_security_modes_t;

/**
 * @brief Wi-Fi encryption types.
 */
typedef enum
{
    wifi_encryption_none, /**< No encryption. */
    wifi_encryption_tkip = 1, /**< TKIP encryption. */
    wifi_encryption_aes, /**< AES encryption. */
    wifi_encryption_aes_tkip, /**< AES/TKIP encryption. */
    wifi_encryption_aes_gcmp256, /**< AES-GCMP-256 encryption. */
    wifi_encryption_gcmp256, /**< GCMP-256 encryption. */
} wifi_encryption_method_t;

/**
 * @brief Wi-Fi frequency band types.
 */
typedef enum
{
    WIFI_FREQUENCY_2_4_BAND = 0x1, /**< 2.4GHz band. */
    WIFI_FREQUENCY_5_BAND = 0x2, /**< 5GHz band. */
    WIFI_FREQUENCY_5L_BAND = 0x4, /**< 5GHz low band. */
    WIFI_FREQUENCY_5H_BAND = 0x8, /**< 5GHz high band. */
    WIFI_FREQUENCY_6_BAND = 0x10, /**< 6GHz band. */
    WIFI_FREQUENCY_60_BAND = 0x20 /**< 60GHz band. */
} wifi_freq_bands_t;

/**
 * @brief Maximum number of frequency bands.
 */
#define MAX_NUM_FREQ_BAND 4

/**
 * @brief Wi-Fi 802.11 variant types.
 */
typedef enum
{
    WIFI_80211_VARIANT_A = 0x01, /**< 802.11a. */
    WIFI_80211_VARIANT_B = 0x02, /**< 802.11b. */
    WIFI_80211_VARIANT_G = 0x04, /**< 802.11g. */
    WIFI_80211_VARIANT_N = 0x08, /**< 802.11n. */
    WIFI_80211_VARIANT_H = 0x10, /**< 802.11h. */
    WIFI_80211_VARIANT_AC = 0x20, /**< 802.11ac. */
    WIFI_80211_VARIANT_AD = 0x40, /**< 802.11ad. */
    WIFI_80211_VARIANT_AX = 0x80, /**< 802.11ax. */
    WIFI_80211_VARIANT_BE = 0x100, /**< 802.11be. */
    WIFI_80211_VARIANT_BN = 0x200 /**< 802.11bn. */
} wifi_ieee80211Variant_t;

/**
 * @brief Wi-Fi channel bandwidth types.
 */
typedef enum
{
    WIFI_CHANNELBANDWIDTH_20MHZ = 0x1, /**< 20MHz. */
    WIFI_CHANNELBANDWIDTH_40MHZ = 0x2, /**< 40MHz. */
    WIFI_CHANNELBANDWIDTH_80MHZ = 0x4, /**< 80MHz. */
    WIFI_CHANNELBANDWIDTH_160MHZ = 0x8, /**< 160MHz. */
    WIFI_CHANNELBANDWIDTH_80_80MHZ = 0x10, /**< 80+80MHz. */
    WIFI_CHANNELBANDWIDTH_320MHZ = 0x20 /**< 320MHz. */
} wifi_channelBandwidth_t;

/**
 * @brief Wi-Fi supported bitrates.
 */
typedef enum
{
    WIFI_BITRATE_DEFAULT = 0x0001, /**< Used in the set API to default the bitrate configuration. */
    WIFI_BITRATE_1MBPS = 0x0002, /**< 1Mbps. */
    WIFI_BITRATE_2MBPS = 0x0004, /**< 2Mbps. */
    WIFI_BITRATE_5_5MBPS = 0x0008, /**< 5.5Mbps. */
    WIFI_BITRATE_6MBPS = 0x0010, /**< 6Mbps. */
    WIFI_BITRATE_9MBPS = 0x0020, /**< 9Mbps. */
    WIFI_BITRATE_11MBPS = 0x0040, /**< 11Mbps. */
    WIFI_BITRATE_12MBPS = 0x0080, /**< 12Mbps. */
    WIFI_BITRATE_18MBPS = 0x0100, /**< 18Mbps. */
    WIFI_BITRATE_24MBPS = 0x0200, /**< 24Mbps. */
    WIFI_BITRATE_36MBPS = 0x0400, /**< 36Mbps. */
    WIFI_BITRATE_48MBPS = 0x0800, /**< 48Mbps. */
    WIFI_BITRATE_54MBPS = 0x1000 /**< 54Mbps. */
} wifi_bitrate_t;

/**
 * @brief BSS information.
 */
typedef struct
{
    bssid_t bssid; /*!< BSSID of the discovered `BSS`, as the six raw octets of `bssid_t`. It is not
                        a text form; `mac_addr_str_t` is the printable one. */
    ssid_t ssid; /*!< SSID the `BSS` advertises, in the 32-octet `ssid_t`. That type states that
                      termination is not established and that all 32 octets may be significant, so a
                      caller must bound its copy at 32 and must not treat the member as a C string;
                      this structure carries no length member for it. */
    int rssi; /*!< Received signal strength measured for the `BSS`. This interface states no unit
                   and no floor value for it, so a caller must not assume dBm; `noise` and `snr`
                   below are stated no more precisely. */
    unsigned short caps; /*!< Capability Information field the `BSS` carried in its beacon or probe
                              response, as the 16-bit bitmask 802.11 defines for that field. This
                              interface names none of the individual bits, so a caller decodes it
                              against 802.11 rather than against anything declared here. */
    unsigned int beacon_int; /*!< Interval the `BSS` advertises between its beacons. This interface
                                  states no unit for the value - 802.11 carries the field in time
                                  units rather than milliseconds - so a caller must not assume one. */
    unsigned int freq; /*!< Frequency the `BSS` was found on. This interface states no unit for the
                            value, so a caller must not assume MHz; `oper_freq_band` below names the
                            band as a `wifi_freq_bands_t` enumerator. */
    unsigned char ie[MAX_IE_ELEMENT_LEN]; /*!< Information elements copied out of the `BSS`'s beacon
                                               or probe response, of which the leading `ie_len`
                                               bytes are populated. `MAX_IE_ELEMENT_LEN` dimensions
                                               the buffer at 1024 bytes; the content is opaque to
                                               this interface, so a caller decodes it against 802.11
                                               and must read no further than `ie_len`. */
    size_t ie_len; /*!< Number of leading bytes of `ie` above that carry elements. It cannot exceed
                        `MAX_IE_ELEMENT_LEN`, and being a `size_t` it carries no negative sentinel,
                        so a caller should treat any larger value as unusable rather than as a
                        marker. */
    wifi_security_modes_t sec_mode; /*!< Security the `BSS` advertises, drawn from
                                         `wifi_security_modes_t`. The member is declared as a single
                                         enumerator, so a caller must not read it as a bitmask of
                                         several even though that enumeration assigns distinct
                                         single bits; `enc_method` beside it carries the cipher. */
    wifi_encryption_method_t enc_method; /*!< Cipher the `BSS` advertises, as a
                                              `wifi_encryption_method_t` enumerator. It is only
                                              meaningful together with `sec_mode` above, since a
                                              cipher is defined within a security mode; this
                                              interface does not state what the member holds for an
                                              open `BSS`. */
    wifi_freq_bands_t oper_freq_band; /*!< Band the `BSS` is operating in, as one of the
                        `wifi_freq_bands_t` enumerators, which are the whole of the accepted domain. */
    wifi_ieee80211Variant_t supp_standards; /*!< 802.11 variants the `BSS` reports as supported, as
                        a bitmask of `wifi_ieee80211Variant_t` values - those enumerators are
                        distinct single bits, so several may be set at once. */
    wifi_ieee80211Variant_t oper_standards; /*!< 802.11 variants the `BSS` is currently operating
                        with, in the same `wifi_ieee80211Variant_t` bitmask form; this is the
                        operating set rather than the capability set `supp_standards` above carries. */
    wifi_channelBandwidth_t supp_chan_bw; /*!< Channel bandwidths the `BSS` reports as supported,
                                               drawn from `wifi_channelBandwidth_t`. That
                                               enumeration assigns distinct single bits, and this
                                               interface does not state whether the member carries
                                               one enumerator or a bitmask of several here, so a
                                               caller should not assume either form without agreeing
                                               it with the platform implementer. */
    wifi_channelBandwidth_t oper_chan_bw; /*!< Bandwidth the `BSS` is currently using, in the same
                                               `wifi_channelBandwidth_t` domain and with the same
                                               uncertainty over single value against bitmask;
                                               `supp_chan_bw` above is the supported set rather than
                                               the one in use. */
    wifi_bitrate_t basic_rates; /*!< Basic rate set of the `BSS`, as a bitmask of `wifi_bitrate_t`
                                     values - those enumerators are distinct single bits, so several
                                     are set at once. */
    wifi_bitrate_t supp_rates; /*!< Rates the `BSS` reports as supported, in the same
                                    `wifi_bitrate_t` bitmask form; `basic_rates` above carries the
                                    basic subset. */
    unsigned int dtim_period; /*!< Delivery Traffic Indication Message period the `BSS` advertises.
                                   This interface states neither the unit nor an accepted range for
                                   it. */
    unsigned int chan_utilization; /**< Channel utilization.  As per R6 spec, this is valid only if BSS load element is present */
    int noise; /*!< Noise level measured while the `BSS` was observed. This interface states no unit
                    for the value, so a caller must not assume dBm. */
    int snr;  /*!< Signal-to-noise ratio reported for the `BSS`. This interface states no unit for
                   the value and does not state how it is derived from `rssi` and `noise` above, so
                   a caller must not recompute one from the others. */
    BOOL bss_load_element_present; /*!< `TRUE` if the `BSS` carried a BSS Load element, which is
                                        what makes `chan_utilization` and `station_cnt` meaningful -
                                        both members say so themselves. Where it is `FALSE` a caller
                                        must not read either of them. */
    unsigned int station_cnt; /**< Station count. As per R6 spec, this is valid only if BSS load element is present */
} __attribute__((packed)) wifi_bss_info_t;

/**
 * @brief IP family types.
 */
typedef enum
{
    wifi_ip_family_ipv4, /**< IPv4. */
    wifi_ip_family_ipv6 /**< IPv6. */
} wifi_ip_family_t;

/**
 * @brief IP address structure.
 */
typedef struct
{
    wifi_ip_family_t family; /*!< Which arm of the `u` union below carries the address:
                                  `wifi_ip_family_ipv4` selects `u.IPv4addr` and
                                  `wifi_ip_family_ipv6` selects `u.IPv6addr`. A caller must set it
                                  before writing the union and read it before reading the union,
                                  because the other arm is not meaningful. */
    union
    { /* network byte ordered */
        UINT IPv4addr; /**< 32-bit IPv4 address. */
        UCHAR IPv6addr[16]; /**< 128-bit IPv6 address. */
    } u; /*!< The address itself, in network byte order. Exactly one arm is valid:
                        `IPv4addr` when `family` is `wifi_ip_family_ipv4`, and `IPv6addr`
                        when `family` is `wifi_ip_family_ipv6`. A caller must set `family`
                        before writing this member and must read `family` before reading it;
                        the arms overlap in storage, so reading the arm the family does not
                        select reinterprets the other arm's bytes. */
} __attribute__((packed)) ip_addr_t;

/**
 * @brief Wi-Fi enable/disable values.
 */
typedef enum
{
    WIFI_HAL_DISABLE = 0, /**< Disable. */
    WIFI_HAL_ENABLE = 1, /**< Enable. */
} eWifiEnable;

/**
 * @brief Wi-Fi HAL version.
 */
typedef struct
{
    UINT major; /*!< Major component of the version the `HAL` implementation reports through
                     `wifi_hal_capability_t::version`. `WIFI_HAL_MAJOR_VERSION` gives the value this
                     header was written against; this interface states no compatibility rule between
                     the two, so a caller should compare rather than assume. */
    UINT minor; /*!< Minor component of the same reported version, against `WIFI_HAL_MINOR_VERSION`
                     in this header. The maintenance component `WIFI_HAL_MAINTENANCE_VERSION`
                     carries is not part of this structure, so a caller cannot recover it from here. */
} __attribute__((packed)) wifi_hal_version_t;

/**
 * @brief Wi-Fi channel information.
 */
typedef struct
{
    INT channel; /*!< IEEE 802.11 channel number, which is only meaningful together with `band`
                      beside it: the same number denotes different channels in different bands. */
    wifi_freq_bands_t band; /*!< Band the channel in `channel` above belongs to, as one of the
                                 `wifi_freq_bands_t` enumerators. It is what makes that channel
                                 number unambiguous, since the same number denotes different
                                 channels in different bands. */
} __attribute__((packed)) wifi_channel_t;

/**
 * @brief Maximum number of channels.
 */
#define MAX_CHANNELS 64

/**
 * @brief Channel list.
 */
typedef struct
{
    INT num_channels; /**< The number of available channels in `channels_list`. */
    INT channels_list[MAX_CHANNELS]; /*!< Channel numbers, of which the leading `num_channels`
                        entries are populated; this interface says nothing about the entries beyond
                        that count. `MAX_CHANNELS` dimensions the array and so bounds
                        `num_channels`. */
} __attribute__((packed)) wifi_channels_list_t;

/**
 * @brief Wi-Fi channel lists per bandwidth.
 */
typedef struct {
    wifi_channelBandwidth_t chanwidth; /*!< Bandwidth the `channels_list` entries below apply to, as
                                            one of the `wifi_channelBandwidth_t` enumerators. It is
                                            declared as a single enumerator rather than a bitmask,
                                            so one of these structures describes exactly one
                                            bandwidth even though that enumeration assigns distinct
                                            single bits. */
    INT num_channels_list; /*!< Number of leading entries of `channels_list` below that carry a
                                channel block. `MAX_CHANNELS` dimensions that array, so a caller
                                must clamp its read at that bound as well; the member is signed and
                                this interface states no meaning for a negative value. */
    wifi_channels_list_t channels_list[MAX_CHANNELS]; /*!< Channel blocks available at the
                        `chanwidth` above, of which the leading `num_channels_list` entries are
                        populated. Each entry is itself a counted list, so a caller reads that
                        entry's own `num_channels` before its `channels_list`. */
}__attribute__((packed)) wifi_channels_list_per_bandwidth_t;

/**
 * @brief Maximum number of transmit power levels supported.
 */
#define MAXNUMBEROFTRANSMIPOWERSUPPORTED 21

/**
 * @brief Radio transmit power supported list.
 */
typedef struct
{
    UINT transmitPowerSupported[MAXNUMBEROFTRANSMIPOWERSUPPORTED]; /*!< Transmit power levels the
                        radio supports, of which the leading `numberOfElements` entries are valid;
                        `MAXNUMBEROFTRANSMIPOWERSUPPORTED` dimensions the array at 21, so a caller
                        must clamp its read at 21 as well.
                        `wifi_radio_operationParam_t::transmitPower` is documented as a percentage,
                        and this interface does not state that these entries use that same scale. */
    UINT numberOfElements;                                         /**< The number of valid elements in `transmitPowerSupported`. */
} __attribute__((packed)) wifi_radio_trasmitPowerSupported_list_t;

#ifdef WIFI_HAL_RSN_SELECTOR
#undef WIFI_HAL_RSN_SELECTOR
#endif
/**
 * @brief Macro for creating an RSN selector value.
 *
 * @param[in] a First byte of the selector.
 * @param[in] b Second byte of the selector.
 * @param[in] c Third byte of the selector.
 * @param[in] d Fourth byte of the selector.
 */
#define WIFI_HAL_RSN_SELECTOR(a, b, c, d) \
    ((((unsigned int)(a)) << 24) | (((unsigned int)(b)) << 16) | (((unsigned int)(c)) << 8) | (unsigned int)(d))

#define WIFI_HAL_RSN_CIPHER_SUITE_NONE WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 0) /**< RSN cipher suite: None. */
#define WIFI_HAL_RSN_CIPHER_SUITE_TKIP WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 2) /**< RSN cipher suite: TKIP. */
#if 0
#define WIFI_HAL_RSN_CIPHER_SUITE_WRAP WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 3) /**< RSN cipher suite: WRAP. */
#endif
#define WIFI_HAL_RSN_CIPHER_SUITE_CCMP WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 4) /**< RSN cipher suite: CCMP. */
#define WIFI_HAL_RSN_CIPHER_SUITE_AES_128_CMAC WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 6) /**< RSN cipher suite: AES-128-CMAC. */
#define WIFI_HAL_RSN_CIPHER_SUITE_NO_GROUP_ADDRESSED WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 7) /**< RSN cipher suite: No group addressed. */
#define WIFI_HAL_RSN_CIPHER_SUITE_GCMP WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 8) /**< RSN cipher suite: GCMP. */
#define WIFI_HAL_RSN_CIPHER_SUITE_GCMP_256 WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 9) /**< RSN cipher suite: GCMP-256. */
#define WIFI_HAL_RSN_CIPHER_SUITE_CCMP_256 WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 10) /**< RSN cipher suite: CCMP-256. */
#define WIFI_HAL_RSN_CIPHER_SUITE_BIP_GMAC_128 WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 11) /**< RSN cipher suite: BIP-GMAC-128. */
#define WIFI_HAL_RSN_CIPHER_SUITE_BIP_GMAC_256 WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 12) /**< RSN cipher suite: BIP-GMAC-256. */
#define WIFI_HAL_RSN_CIPHER_SUITE_BIP_CMAC_256 WIFI_HAL_RSN_SELECTOR(0x00, 0x0f, 0xac, 13) /**< RSN cipher suite: BIP-CMAC-256. */

#define WIFI_CIPHER_CAPA_ENC_WEP40 0x00000001 /**< Cipher capability: WEP-40 encryption. */
#define WIFI_CIPHER_CAPA_ENC_WEP104 0x00000002 /**< Cipher capability: WEP-104 encryption. */
#define WIFI_CIPHER_CAPA_ENC_TKIP 0x00000004 /**< Cipher capability: TKIP encryption. */
#define WIFI_CIPHER_CAPA_ENC_CCMP 0x00000008 /**< Cipher capability: CCMP encryption. */
#define WIFI_CIPHER_CAPA_ENC_WEP128 0x00000010 /**< Cipher capability: WEP-128 encryption. */
#define WIFI_CIPHER_CAPA_ENC_GCMP 0x00000020 /**< Cipher capability: GCMP encryption. */
#define WIFI_CIPHER_CAPA_ENC_GCMP_256 0x00000040 /**< Cipher capability: GCMP-256 encryption. */
#define WIFI_CIPHER_CAPA_ENC_CCMP_256 0x00000080 /**< Cipher capability: CCMP-256 encryption. */
#define WIFI_CIPHER_CAPA_ENC_BIP 0x00000100 /**< Cipher capability: BIP encryption. */
#define WIFI_CIPHER_CAPA_ENC_BIP_GMAC_128 0x00000200 /**< Cipher capability: BIP-GMAC-128 encryption. */
#define WIFI_CIPHER_CAPA_ENC_BIP_GMAC_256 0x00000400 /**< Cipher capability: BIP-GMAC-256 encryption. */
#define WIFI_CIPHER_CAPA_ENC_BIP_CMAC_256 0x00000800 /**< Cipher capability: BIP-CMAC-256 encryption. */
#define WIFI_CIPHER_CAPA_ENC_GTK_NOT_USED 0x00001000 /**< Cipher capability: GTK not used. */

/**
 * @brief Wi-Fi country code types.
 */
typedef enum {
    wifi_countrycode_AC, /**< ASCENSION ISLAND */
    wifi_countrycode_AD, /**< ANDORRA */
    wifi_countrycode_AE, /**< UNITED ARAB EMIRATES */
    wifi_countrycode_AF, /**< AFGHANISTAN */
    wifi_countrycode_AG, /**< ANTIGUA AND BARBUDA */
    wifi_countrycode_AI, /**< ANGUILLA */
    wifi_countrycode_AL, /**< ALBANIA */
    wifi_countrycode_AM, /**< ARMENIA */
    wifi_countrycode_AN, /**< NETHERLANDS ANTILLES */
    wifi_countrycode_AO, /**< ANGOLA */
    wifi_countrycode_AQ, /**< ANTARCTICA */
    wifi_countrycode_AR, /**< ARGENTINA */
    wifi_countrycode_AS, /**< AMERICAN SAMOA */
    wifi_countrycode_AT, /**< AUSTRIA */
    wifi_countrycode_AU, /**< AUSTRALIA */
    wifi_countrycode_AW, /**< ARUBA */
    wifi_countrycode_AZ, /**< AZERBAIJAN */    
    wifi_countrycode_BA, /**< BOSNIA AND HERZEGOVINA */
    wifi_countrycode_BB, /**< BARBADOS */
    wifi_countrycode_BD, /**< BANGLADESH */
    wifi_countrycode_BE, /**< BELGIUM */
    wifi_countrycode_BF, /**< BURKINA FASO */
    wifi_countrycode_BG, /**< BULGARIA */
    wifi_countrycode_BH, /**< BAHRAIN */
    wifi_countrycode_BI, /**< BURUNDI */
    wifi_countrycode_BJ, /**< BENIN */
    wifi_countrycode_BM, /**< BERMUDA */
    wifi_countrycode_BN, /**< BRUNEI DARUSSALAM */
    wifi_countrycode_BO, /**< BOLIVIA */
    wifi_countrycode_BR, /**< BRAZIL */
    wifi_countrycode_BS, /**< BAHAMAS */
    wifi_countrycode_BT, /**< BHUTAN */
    wifi_countrycode_BV, /**< BOUVET ISLAND */
    wifi_countrycode_BW, /**< BOTSWANA */
    wifi_countrycode_BY, /**< BELARUS */
    wifi_countrycode_BZ, /**< BELIZE */
    wifi_countrycode_CA, /**< CANADA */
    wifi_countrycode_CC, /**< COCOS (KEELING) ISLANDS */
    wifi_countrycode_CD, /**< CONGO, THE DEMOCRATIC REPUBLIC OF THE */
    wifi_countrycode_CF, /**< CENTRAL AFRICAN REPUBLIC */
    wifi_countrycode_CG, /**< CONGO */
    wifi_countrycode_CH, /**< SWITZERLAND */
    wifi_countrycode_CI, /**< COTE D'IVOIRE */
    wifi_countrycode_CK, /**< COOK ISLANDS */
    wifi_countrycode_CL, /**< CHILE */
    wifi_countrycode_CM, /**< CAMEROON */
    wifi_countrycode_CN, /**< CHINA */
    wifi_countrycode_CO, /**< COLOMBIA */
    wifi_countrycode_CP, /**< CLIPPERTON ISLAND */
    wifi_countrycode_CR, /**< COSTA RICA */
    wifi_countrycode_CU, /**< CUBA */
    wifi_countrycode_CV, /**< CAPE VERDE */
    wifi_countrycode_CY, /**< CYPRUS */
    wifi_countrycode_CX, /**< CHRISTMAS ISLAND */
    wifi_countrycode_CZ, /**< CZECH REPUBLIC */
    wifi_countrycode_DE, /**< GERMANY */
    wifi_countrycode_DJ, /**< DJIBOUTI */
    wifi_countrycode_DK, /**< DENMARK */
    wifi_countrycode_DM, /**< DOMINICA */
    wifi_countrycode_DO, /**< DOMINICAN REPUBLIC */
    wifi_countrycode_DZ, /**< ALGERIA */
    wifi_countrycode_EC, /**< ECUADOR */
    wifi_countrycode_EE, /**< ESTONIA */
    wifi_countrycode_EG, /**< EGYPT */
    wifi_countrycode_EH, /**< WESTERN SAHARA */
    wifi_countrycode_ER, /**< ERITREA */
    wifi_countrycode_ES, /**< SPAIN */
    wifi_countrycode_ET, /**< ETHIOPIA */
    wifi_countrycode_FI, /**< FINLAND */
    wifi_countrycode_FJ, /**< FIJI */
    wifi_countrycode_FK, /**< FALKLAND ISLANDS (MALVINAS) */
    wifi_countrycode_FM, /**< MICRONESIA, FEDERATED STATES OF */
    wifi_countrycode_FO, /**< FAROE ISLANDS */
    wifi_countrycode_FR, /**< FRANCE */
    wifi_countrycode_GA, /**< GABON */
    wifi_countrycode_GB, /**< UNITED KINGDOM */
    wifi_countrycode_GD, /**< GRENADA */
    wifi_countrycode_GE, /**< GEORGIA */
    wifi_countrycode_GF, /**< FRENCH GUIANA */
    wifi_countrycode_GG, /**< GUERNSEY */
    wifi_countrycode_GH, /**< GHANA */
    wifi_countrycode_GI, /**< GIBRALTAR */
    wifi_countrycode_GL, /**< GREENLAND */    
    wifi_countrycode_GM, /**< GAMBIA */
    wifi_countrycode_GN, /**< GUINEA */
    wifi_countrycode_GP, /**< GUADELOUPE */
    wifi_countrycode_GQ, /**< EQUATORIAL GUINEA */
    wifi_countrycode_GR, /**< GREECE */
    wifi_countrycode_GS, /**< SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS */
    wifi_countrycode_GT, /**< GUATEMALA */    
    wifi_countrycode_GU, /**< GUAM */
    wifi_countrycode_GW, /**< GUINEA-BISSAU */
    wifi_countrycode_GY, /**< GUYANA */
    wifi_countrycode_HR, /**< CROATIA */
    wifi_countrycode_HT, /**< HAITI */
    wifi_countrycode_HM, /**< HEARD ISLAND AND MCDONALD ISLANDS */
    wifi_countrycode_HN, /**< HONDURAS */
    wifi_countrycode_HK, /**< HONG KONG */
    wifi_countrycode_HU, /**< HUNGARY */
    wifi_countrycode_IS, /**< ICELAND */
    wifi_countrycode_IN, /**< INDIA */
    wifi_countrycode_ID, /**< INDONESIA */
    wifi_countrycode_IR, /**< IRAN, ISLAMIC REPUBLIC OF */
    wifi_countrycode_IQ, /**< IRAQ */
    wifi_countrycode_IE, /**< IRELAND */
    wifi_countrycode_IL, /**< ISRAEL */
    wifi_countrycode_IM, /**< MAN, ISLE OF */
    wifi_countrycode_IT, /**< ITALY */
    wifi_countrycode_IO, /**< BRITISH INDIAN OCEAN TERRITORY */
    wifi_countrycode_JM, /**< JAMAICA */
    wifi_countrycode_JP, /**< JAPAN */
    wifi_countrycode_JE, /**< JERSEY */
    wifi_countrycode_JO, /**< JORDAN */
    wifi_countrycode_KE, /**< KENYA */
    wifi_countrycode_KG, /**< KYRGYZSTAN */
    wifi_countrycode_KH, /**< CAMBODIA */
    wifi_countrycode_KI, /**< KIRIBATI */
    wifi_countrycode_KM, /**< COMOROS */
    wifi_countrycode_KN, /**< SAINT KITTS AND NEVIS */
    wifi_countrycode_KP, /**< KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF */
    wifi_countrycode_KR, /**< KOREA, REPUBLIC OF */
    wifi_countrycode_KW, /**< KUWAIT */
    wifi_countrycode_KY, /**< CAYMAN ISLANDS */
    wifi_countrycode_KZ, /**< KAZAKHSTAN */
    wifi_countrycode_LA, /**< LAO PEOPLE'S DEMOCRATIC REPUBLIC */
    wifi_countrycode_LB, /**< LEBANON */
    wifi_countrycode_LC, /**< SAINT LUCIA */
    wifi_countrycode_LI, /**< LIECHTENSTEIN */
    wifi_countrycode_LK, /**< SRI LANKA */
    wifi_countrycode_LR, /**< LIBERIA */
    wifi_countrycode_LS, /**< LESOTHO */
    wifi_countrycode_LT, /**< LITHUANIA */
    wifi_countrycode_LU, /**< LUXEMBOURG */
    wifi_countrycode_LV, /**< LATVIA */
    wifi_countrycode_LY, /**< LIBYAN ARAB JAMAHIRIYA */
    wifi_countrycode_MA, /**< MOROCCO */
    wifi_countrycode_MC, /**< MONACO */
    wifi_countrycode_MD, /**< MOLDOVA, REPUBLIC OF */
    wifi_countrycode_ME, /**< MONTENEGRO */
    wifi_countrycode_MG, /**< MADAGASCAR */
    wifi_countrycode_MH, /**< MARSHALL ISLANDS */
    wifi_countrycode_MK, /**< MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF */
    wifi_countrycode_ML, /**< MALI */
    wifi_countrycode_MM, /**< MYANMAR */
    wifi_countrycode_MN, /**< MONGOLIA */
    wifi_countrycode_MO, /**< MACAO */
    wifi_countrycode_MQ, /**< MARTINIQUE */
    wifi_countrycode_MR, /**< MAURITANIA */
    wifi_countrycode_MS, /**< MONTSERRAT */
    wifi_countrycode_MT, /**< MALTA */
    wifi_countrycode_MU, /**< MAURITIUS */
    wifi_countrycode_MV, /**< MALDIVES */
    wifi_countrycode_MW, /**< MALAWI */
    wifi_countrycode_MX, /**< MEXICO */
    wifi_countrycode_MY, /**< MALAYSIA */
    wifi_countrycode_MZ, /**< MOZAMBIQUE */
    wifi_countrycode_NA, /**< NAMIBIA */
    wifi_countrycode_NC, /**< NEW CALEDONIA */
    wifi_countrycode_NE, /**< NIGER */
    wifi_countrycode_NF, /**< NORFOLK ISLAND */
    wifi_countrycode_NG, /**< NIGERIA */
    wifi_countrycode_NI, /**< NICARAGUA */
    wifi_countrycode_NL, /**< NETHERLANDS */
    wifi_countrycode_NO, /**< NORWAY */
    wifi_countrycode_NP, /**< NEPAL */    
    wifi_countrycode_NR, /**< NAURU */
    wifi_countrycode_NU, /**< NIUE */    
    wifi_countrycode_NZ, /**< NEW ZEALAND */
    wifi_countrycode_MP, /**< NORTHERN MARIANA ISLANDS */
    wifi_countrycode_OM, /**< OMAN */
    wifi_countrycode_PA, /**< PANAMA */
    wifi_countrycode_PE, /**< PERU */
    wifi_countrycode_PF, /**< FRENCH POLYNESIA */
    wifi_countrycode_PG, /**< PAPUA NEW GUINEA */
    wifi_countrycode_PH, /**< PHILIPPINES */
    wifi_countrycode_PK, /**< PAKISTAN */
    wifi_countrycode_PL, /**< POLAND */
    wifi_countrycode_PM, /**< SAINT PIERRE AND MIQUELON */
    wifi_countrycode_PN, /**< PITCAIRN */
    wifi_countrycode_PR, /**< PUERTO RICO */
    wifi_countrycode_PS, /**< PALESTINIAN TERRITORY, OCCUPIED */
    wifi_countrycode_PT, /**< PORTUGAL */    
    wifi_countrycode_PW, /**< PALAU */
    wifi_countrycode_PY, /**< PARAGUAY */
    wifi_countrycode_QA, /**< QATAR */
    wifi_countrycode_RE, /**< REUNION */
    wifi_countrycode_RO, /**< ROMANIA */
    wifi_countrycode_RS, /**< SERBIA */
    wifi_countrycode_RU, /**< RUSSIAN FEDERATION */
    wifi_countrycode_RW, /**< RWANDA */
    wifi_countrycode_SA, /**< SAUDI ARABIA */
    wifi_countrycode_SB, /**< SOLOMON ISLANDS */
    wifi_countrycode_SD, /**< SUDAN */
    wifi_countrycode_SE, /**< SWEDEN */
    wifi_countrycode_SC, /**< SEYCHELLES */
    wifi_countrycode_SG, /**< SINGAPORE */
    wifi_countrycode_SH, /**< SAINT HELENA */
    wifi_countrycode_SI, /**< SLOVENIA */
    wifi_countrycode_SJ, /**< SVALBARD AND JAN MAYEN */
    wifi_countrycode_SK, /**< SLOVAKIA */
    wifi_countrycode_SL, /**< SIERRA LEONE */
    wifi_countrycode_SM, /**< SAN MARINO */
    wifi_countrycode_SN, /**< SENEGAL */
    wifi_countrycode_SO, /**< SOMALIA */
    wifi_countrycode_SR, /**< SURINAME */
    wifi_countrycode_ST, /**< SAO TOME AND PRINCIPE */
    wifi_countrycode_SV, /**< EL SALVADOR */
    wifi_countrycode_SY, /**< SYRIAN ARAB REPUBLIC */
    wifi_countrycode_SZ, /**< SWAZILAND */
    wifi_countrycode_TA, /**< TRISTAN DA CUNHA */
    wifi_countrycode_TC, /**< TURKS AND CAICOS ISLANDS */
    wifi_countrycode_TD, /**< CHAD */
    wifi_countrycode_TF, /**< FRENCH SOUTHERN TERRITORIES */
    wifi_countrycode_TG, /**< TOGO */
    wifi_countrycode_TH, /**< THAILAND */
    wifi_countrycode_TJ, /**< TAJIKISTAN */
    wifi_countrycode_TK, /**< TOKELAU */
    wifi_countrycode_TL, /**< TIMOR-LESTE (EAST TIMOR) */
    wifi_countrycode_TM, /**< TURKMENISTAN */
    wifi_countrycode_TN, /**< TUNISIA */
    wifi_countrycode_TO, /**< TONGA */
    wifi_countrycode_TR, /**< TURKEY */
    wifi_countrycode_TT, /**< TRINIDAD AND TOBAGO */
    wifi_countrycode_TV, /**< TUVALU */
    wifi_countrycode_TW, /**< TAIWAN, PROVINCE OF CHINA */
    wifi_countrycode_TZ, /**< TANZANIA, UNITED REPUBLIC OF */
    wifi_countrycode_UA, /**< UKRAINE */
    wifi_countrycode_UG, /**< UGANDA */
    wifi_countrycode_UM, /**< UNITED STATES MINOR OUTLYING ISLANDS */
    wifi_countrycode_US, /**< UNITED STATES */
    wifi_countrycode_UY, /**< URUGUAY */
    wifi_countrycode_UZ, /**< UZBEKISTAN */
    wifi_countrycode_VA, /**< HOLY SEE (VATICAN CITY STATE) */
    wifi_countrycode_VC, /**< SAINT VINCENT AND THE GRENADINES */
    wifi_countrycode_VE, /**< VENEZUELA */
    wifi_countrycode_VG, /**< VIRGIN ISLANDS, BRITISH */
    wifi_countrycode_VI, /**< VIRGIN ISLANDS, U.S. */
    wifi_countrycode_VN, /**< VIET NAM */
    wifi_countrycode_VU, /**< VANUATU */
    wifi_countrycode_WF, /**< WALLIS AND FUTUNA */
    wifi_countrycode_WS, /**< SAMOA */
    wifi_countrycode_YE, /**< YEMEN */
    wifi_countrycode_YT, /**< MAYOTTE */
    wifi_countrycode_YU, /**< YUGOSLAVIA */
    wifi_countrycode_ZA, /**< SOUTH AFRICA */
    wifi_countrycode_ZM, /**< ZAMBIA */
    wifi_countrycode_ZW, /**< ZIMBABWE */
    wifi_countrycode_AX, /**< ALAND_ISLANDS */
    wifi_countrycode_BL, /**< SAINT_BARTHELEMY */
    wifi_countrycode_CW, /**< CURACAO */
    wifi_countrycode_MF, /**< SAINT_MARTIN */
    wifi_countrycode_SX, /**< SINT_MAARTEN */
    wifi_countrycode_BQ, /**< BONAIRE_SINT_EUSTATIUS_AND_SABA */
    wifi_countrycode_00, /**< GLOBAL_CC */
    wifi_countrycode_max /**< Max number of country code */
} wifi_countrycode_type_t;

/**
 * @brief Wi-Fi operating environment types.
 */
typedef enum
{
    wifi_operating_env_all, /**< All environments. */
    wifi_operating_env_indoor, /**< Indoor environment. */
    wifi_operating_env_outdoor, /**< Outdoor environment. */
    wifi_operating_env_non_country /**< Non-country environment. */
} wifi_operating_env_t;

/**
 * @brief Wi-Fi radio CSI capabilities.
 */
typedef struct
{
    UINT maxDevices; /**< The maximum number of stations that can be configured to collect the CSI data. Return 0 if CSI is not supported. */
    BOOL soudingFrameSupported; /*!< `TRUE` where the radio can transmit the sounding frames channel
                                     state information collection relies on. The member name
                                     misspells `sounding` as `souding`; the identifier a caller has
                                     to write is `soudingFrameSupported`, and no correctly spelt
                                     member exists in this structure. `maxDevices` above is what
                                     says how many stations may collect at once. */
} __attribute__((packed)) wifi_radio_csi_capabilities_t;

/**
 * @brief Wi-Fi Multi-Link supported bands.
 */
typedef enum
{
    WIFI_BAND_NONE = 0x1, /**< No band. */
    WIFI_BAND_2_5 = 0x2, /**< 2.4GHz band. */
    WIFI_BAND_2_6 = 0x4, /**< 2.4GHz and 6GHz bands. */
    WIFI_BAND_5_6 = 0x8, /**< 5GHz and 6GHz bands. */
    WIFI_BAND_2_5_6 = 0x10, /**< 2.4GHz, 5GHz, and 6GHz bands. */
    WIFI_BAND_2_5L = 0x20, /**< 2.4GHz and 5GHz low bands. */
    WIFI_BAND_2_5H = 0x40, /**< 2.4GHz and 5GHz high bands. */
    WIFI_BAND_5L_5H = 0x80, /**< 5GHz low and high bands. */
    WIFI_BAND_2_5L_5H = 0x100 /**< 2.4GHz, 5GHz low, and 5GHz high bands. */
} wifi_multi_link_bands_t;

/**
 * @brief Wi-Fi 7 supported modes.
 */
typedef enum
{
    STR = 0x1, /**< Single-user Transmit and Receive (STR). */
    NSTR = 0x2, /**< Non-STR. */
    eMLSR = 0x4, /**< Enhanced Multi-Link Single-user Resource (eMLSR). */
    eMLMR = 0x8 /**< Enhanced Multi-Link Multi-user Resource (eMLMR). */
} wifi_multi_link_modes_t;

/**
 * @brief Maximum size of an interface name.
 */
#define MAXIFACENAMESIZE 64

#define HT_MCS_SET_LEN          16  /**< Length in bytes of the HT (802.11n) MCS set field. */
#define VHT_MCS_SET_LEN         8   /**< Length in bytes of the VHT (802.11ac) MCS set field. */
#define HE_MAX_MAC_CAPAB_SIZE    6   /**< Maximum length in bytes of the HE (802.11ax) MAC capabilities field. */
#define HE_MAX_PHY_CAPAB_SIZE    11  /**< Maximum length in bytes of the HE (802.11ax) PHY capabilities field. */
#define HE_MAX_MCS_CAPAB_SIZE    12  /**< Maximum length in bytes of the HE (802.11ax) MCS and NSS set field. */
#define HE_MAX_PPET_CAPAB_SIZE   25  /**< Maximum length in bytes of the HE (802.11ax) PPE thresholds field. */
#define EHT_PHY_CAPAB_LEN        9   /**< Length in bytes of the EHT (802.11be) PHY capabilities field. */
#define EHT_MCS_NSS_CAPAB_LEN    9   /**< Length in bytes of the EHT (802.11be) MCS and NSS capabilities field. */
#define EHT_PPE_THRESH_CAPAB_LEN 62  /**< Maximum length in bytes of the EHT (802.11be) PPE thresholds field. */
#ifndef MAX_CHANNELS_PER_OP_CLASS
#define MAX_CHANNELS_PER_OP_CLASS 70  /**< Maximum channels per operating class. */
#endif
#define MAX_OP_CLASS_ENTRIES      60  /*!< Number of operating-class entries one radio's capability report can carry. It is the declared length of the `op_class_ch_list` member of `wifi_radio_capabilities_t`; the number of entries actually populated is reported in that structure's `num_op_class_entries`, and a caller must read no further than the smaller of the two. */

/**
 * @brief Channel scan impact level. Expected impact on Fronthaul/Backhaul operations during a channel scan.
 *
 * Values are defined by the EasyMesh specification.  The field is 2 bits wide.
 */
typedef enum {
    WIFI_SCAN_IMPACT_NONE           = 0x00, /**< No impact on Fronthaul/Backhaul operations. */
    WIFI_SCAN_IMPACT_REDUCED_STREAMS = 0x01, /**< Reduced number of spatial streams during scan. */
    WIFI_SCAN_IMPACT_TIME_SLICING   = 0x02, /**< Time slicing impairment during scan. */
    WIFI_SCAN_IMPACT_RADIO_UNAVAIL  = 0x03, /**< Radio unavailable for >= 2 seconds during scan. */
} wifi_channel_scan_impact_t;

/**
 * @brief One operating class and the channels it admits on a radio.
 *
 * An instance pairs a global operating class with the channel set that class covers on the
 * radio being described, which is how a caller resolves an operating class to channels
 * without calling the `HAL` again. Instances travel only inside a capability report: the
 * `op_class_ch_list` array of `wifi_radio_capabilities_t` holds up to `MAX_OP_CLASS_ENTRIES`
 * of them, and that structure's `num_op_class_entries` says how many the `HAL` populated.
 * No declared function takes or returns this type on its own.
 *
 * The `HAL` fills every member and the caller only reads them; the storage belongs to
 * whichever structure encloses it, so a caller that keeps an entry beyond the life of the
 * capability report must copy it.
 *
 * @note The structure is packed, so a caller must not take the address of a member and pass
 *       it where an aligned object is required, and must not assume its layout matches an
 *       unpacked structure with the same members.
 *
 * @see wifi_radio_capabilities_t
 * @see MAX_OP_CLASS_ENTRIES
 * @see MAX_CHANNELS_PER_OP_CLASS
 */
typedef struct {
    UCHAR  op_class;                              /*!< Global operating class number as defined in IEEE 802.11-2020 Table E-4. The table's global classes carry the band and bandwidth with them, so the class is what fixes how the `channels` entries below are to be read. */
    UCHAR num_channels;                          /*!< Number of valid channel entries in the channels array. Entries beyond it are unspecified, and a caller must clamp its read at `MAX_CHANNELS_PER_OP_CLASS` as well, because that is the declared length of the array. */
    UCHAR channels[MAX_CHANNELS_PER_OP_CLASS];   /*!< List of channel numbers (or center-frequency indices for 80/160/320 MHz classes) for this operating class. Which of the two readings applies follows from `op_class`, not from this member, and the entries are not stated to be sorted. */
} __attribute__((packed)) op_class_ch_list_t;

/**
 * @brief Wi-Fi radio capabilities.
 */
typedef struct
{
    UINT index; /*!< Index of the radio this capability record describes. `rdk_radio_index` below is
                     documented as the index the upper layer uses, and this interface does not state
                     which of the two the `radioIndex` arguments elsewhere in this interface expect,
                     so a caller must not treat them as interchangeable. */
    UINT rdk_radio_index; /*!< Radio index as the upper layer numbers radios, which is the sense
                               `wifi_interface_name_idex_map_t::rdk_radio_index` is documented in.
                               `index` above is also a radio index, and this interface does not
                               state which of the two the `radioIndex` arguments elsewhere in this
                               interface expect. */
    CHAR ifaceName[MAXIFACENAMESIZE]; /*!< Name of the radio's network interface, in the
                                           `MAXIFACENAMESIZE` bytes that macro declares as 64. This
                                           interface states neither the encoding nor whether the
                                           value is NUL-terminated, so a caller must bound any read
                                           at 64 bytes. */
    UINT numSupportedFreqBand; /*!< Number of leading entries populated in every array in this
                                    structure that is dimensioned `MAX_NUM_FREQ_BAND` - `band`,
                                    `channel_list`, `channelWidth`, `mode`, `maxBitRate`,
                                    `supportedBitRate` and `transmitPowerSupported_list` are indexed
                                    by the same band position - so a caller must not read past this
                                    count in any of them. */
    wifi_freq_bands_t band[MAX_NUM_FREQ_BAND]; /*!< Band of each supported position, of which the
                                                    leading `numSupportedFreqBand` entries are
                                                    populated, each as a `wifi_freq_bands_t`
                                                    enumerator. Every array in this structure
                                                    dimensioned `MAX_NUM_FREQ_BAND` is indexed by
                                                    the same position, so this member is what names
                                                    the band a given position refers to. */
    wifi_channels_list_t channel_list[MAX_NUM_FREQ_BAND]; /*!< Channels supported at each band
                        position, indexed in step with `band` above, of which the leading
                        `numSupportedFreqBand` positions are populated. Each entry is itself a
                        counted list, so a caller reads that entry's own `num_channels` before its
                        `channels`. */
    wifi_channelBandwidth_t channelWidth[MAX_NUM_FREQ_BAND]; /**< The channel bandwidth supported (uses bitmask to return multiple bandwidths) for each frequency band supported. */
    wifi_ieee80211Variant_t mode[MAX_NUM_FREQ_BAND]; /**< The supported modes (uses bitmask to return multiple modes) for each frequency band supported. */
    UINT maxBitRate[MAX_NUM_FREQ_BAND]; /**< The maximum PHY bit rate supported for each frequency band supported. */
    UINT supportedBitRate[MAX_NUM_FREQ_BAND]; /**< The supported data transmit rates in Mbps for each frequency band supported. It uses bitmask to return multiple bitrates and wifi_bitrate_t has the definition of valid values. */
    wifi_radio_trasmitPowerSupported_list_t transmitPowerSupported_list[MAX_NUM_FREQ_BAND]; /*!< Transmit
                        power levels supported at each band position, indexed in step with `band`
                        above, of which the leading `numSupportedFreqBand` positions are populated.
                        Each entry is a counted list in its own right, so a caller reads that
                        entry's own count before its values. */
    BOOL autoChannelSupported; /*!< `TRUE` where the radio can select its own operating channel,
                                    which is the mode
                                    `wifi_radio_operationParam_t::autoChannelEnabled` requests. This
                                    interface does not state what the radio does if that mode is
                                    requested anyway where the capability is `FALSE`, so a caller
                                    must not read `FALSE` as establishing any particular fallback. */
    BOOL DCSSupported; /*!< `TRUE` where the radio supports dynamic channel selection. This
                            interface declares no call bound to the capability and states nothing
                            about how it relates to `autoChannelSupported` above or to
                            `zeroDFSSupported` below, so a caller can use the member only to decide
                            whether to offer the feature at all. */
    BOOL zeroDFSSupported; /**< True if Zero DFS is supported. Zero DFS (also known as Background CAC) allows the Wi-Fi stack to continue operation on the main channel and at the same time run background CAC. */
    wifi_radio_csi_capabilities_t csi; /*!< How much channel state information collection the radio
                                            supports: `wifi_radio_csi_capabilities_t` carries the
                                            maximum number of stations that can be configured to
                                            collect CSI - zero where CSI is not supported at all -
                                            and whether the radio can send sounding frames.
                                            `wifi_enableCSIEngine()` is what enables collection for
                                            a station. */
    UINT cipherSupported; /*!< Ciphers the radio supports. This interface does not establish how
                               they are encoded in the member: it is a plain `UINT` rather than an
                               enumeration type, and it cannot be a bitmask of
                               `wifi_encryption_method_t` values, because those enumerators run
                               consecutively from `wifi_encryption_none` at 0 rather than occupying
                               distinct bits - 0 has no bit to test, and setting the bits of
                               `wifi_encryption_tkip` (1) and `wifi_encryption_aes` (2) together
                               yields 3, which is the separate enumerator
                               `wifi_encryption_aes_tkip`. `wifi_security_modes_t` in this header
                               is the bit-valued enumeration; the cipher one is not. A caller must
                               therefore take the encoding of this member from the implementation
                               it runs against, and must neither mask it against
                               `wifi_encryption_method_t` enumerators nor compare the whole value
                               against one of them. */
    UINT numcountrySupported; /*!< Number of leading entries of `countrySupported` below that carry
                                   a country. That array is dimensioned `wifi_countrycode_max`, so a
                                   caller must clamp its read at that bound as well. */
    wifi_countrycode_type_t countrySupported[wifi_countrycode_max]; /**< The supported country list. It should return the current country code on the first entry. */
    UINT maxNumberVAPs; /*!< Largest number of Virtual Access Points the radio supports.
                             `MAX_NUM_VAP_PER_RADIO` dimensions `wifi_vap_info_map_t::vap_array` at
                             8, and this interface does not state that the reported value cannot
                             exceed that, so a caller must clamp any indexing at the array bound
                             rather than at this member. */
    BOOL mcast2ucastSupported; /*!< `TRUE` where the radio can convert group-addressed frames into
                                    unicast ones before transmitting them, which is the feature
                                    `wifi_front_haul_bss_t::mcast2ucast` turns on per `BSS`. This
                                    interface does not state what that setting does where the
                                    capability is `FALSE`. */
    wifi_multi_link_modes_t mldOperationalCap; /**< Bitmask indicating WiFi 7 supported modes */
    BOOL TIDLinkMapNegotiation; /*!< `TRUE` where the radio can negotiate which traffic identifiers
                                     map to which link of a multi-link device, which is meaningful
                                     only for the 802.11be modes `mldOperationalCap` above reports.
                                     This interface declares no call bound to the capability, so a
                                     caller can use it only to decide whether to offer the feature. */
    USHORT ht_capab; /*!< HT Capability Information field of the radio, as the 16-bit bitmask
                          802.11n defines. This interface names none of the individual bits, so a
                          caller decodes it against 802.11n. */
    UCHAR mcs_set[HT_MCS_SET_LEN]; /*!< HT Supported MCS Set of the radio, as `HT_MCS_SET_LEN` raw
                                        octets of bitmask. Fixed length with no count member: a
                                        supported MCS is indicated by its bit rather than by an
                                        index into the array. */
    UCHAR ampdu_params; /*!< HT A-MPDU Parameters octet, carrying the maximum A-MPDU length exponent
                             and the minimum MPDU start spacing as a bitmask. This interface names
                             neither field's position, so a caller decodes it against 802.11n. */
    UINT vht_capab; /*!< VHT Capabilities Information field of the radio, as the 32-bit bitmask
                         802.11ac defines. This interface names none of the individual bits. */
    UCHAR vht_mcs_set[VHT_MCS_SET_LEN]; /*!< VHT Supported MCS and NSS Set of the radio, as
                                             `VHT_MCS_SET_LEN` raw octets. Fixed length with no
                                             count member, decoded against 802.11ac. */
    BOOL wifi6_supported; /**< Whether WiFi6 (HE) is supported */
    UCHAR he_phy_cap[HE_MAX_PHY_CAPAB_SIZE]; /*!< HE PHY capabilities as the raw 802.11ax octets, in
                                                  the `HE_MAX_PHY_CAPAB_SIZE` bytes that macro
                                                  declares as 11. There is no member reporting how
                                                  many are populated, and `wifi6_supported` above
                                                  reports only whether HE is supported at all, so a
                                                  caller decodes the whole buffer against 802.11ax. */
    UCHAR he_mac_cap[HE_MAX_MAC_CAPAB_SIZE]; /*!< HE MAC capabilities as the raw 802.11ax octets, in
                                                  the `HE_MAX_MAC_CAPAB_SIZE` bytes that macro
                                                  declares as 6, on the same terms as `he_phy_cap`
                                                  above. */
    UCHAR he_mcs_nss_set[HE_MAX_MCS_CAPAB_SIZE]; /*!< HE MCS and NSS support as the raw 802.11ax
                        capability octets, in a buffer dimensioned `HE_MAX_MCS_CAPAB_SIZE`. This
                        interface names none of the individual fields and carries no length member
                        for it, so a caller decodes the buffer against 802.11ax rather than reading
                        a populated extent from this structure. `wifi6_supported` above reports
                        whether the radio supports HE at all; this interface does not state what the
                        buffer holds when that is `FALSE`. */
    UCHAR he_ppet[HE_MAX_PPET_CAPAB_SIZE]; /*!< HE PPE Threshold field as raw 802.11ax octets, in
                                                the `HE_MAX_PPET_CAPAB_SIZE` bytes that macro
                                                declares as 25. The field is variable length in
                                                802.11ax while this buffer is fixed, and this
                                                interface declares no member reporting the populated
                                                extent. */
    USHORT he_6ghz_capa; /*!< HE 6GHz Band Capabilities field, as a 16-bit value whose bits this
                              interface does not name. It is meaningful only for a radio that
                              operates in the 6GHz band, which `band` above reports. */
    BOOL wifi7_supported; /**< Whether WiFi7 (EHT) is supported */
    USHORT eht_mac_cap; /*!< EHT MAC capabilities as the 16-bit field 802.11be defines, whose bits
                             this interface does not name. `wifi7_supported` above reports whether
                             the radio supports EHT at all; this interface does not state what the
                             member holds where it does not. */
    UCHAR eht_phy_cap[EHT_PHY_CAPAB_LEN]; /*!< EHT PHY capabilities as raw 802.11be octets, in the
                                               `EHT_PHY_CAPAB_LEN` bytes that macro declares as 9.
                                               Fixed length with no populated-extent member. */
    UCHAR eht_mcs[EHT_MCS_NSS_CAPAB_LEN]; /*!< EHT MCS and NSS support as raw 802.11be octets, in
                                               the `EHT_MCS_NSS_CAPAB_LEN` bytes that macro declares
                                               as 9, decoded against 802.11be rather than read as an
                                               index. */
    UCHAR eht_ppet[EHT_PPE_THRESH_CAPAB_LEN]; /*!< EHT PPE Threshold field as raw 802.11be octets,
                                                   in the `EHT_PPE_THRESH_CAPAB_LEN` bytes that
                                                   macro declares as 62. The field is variable
                                                   length in 802.11be while this buffer is fixed,
                                                   and no member reports the populated extent. */
    UINT min_scan_interval;    /**< Minimum scan interval in seconds. */
    UINT num_op_class_entries; /*!< Number of leading entries of `op_class_ch_list` below that
                                    describe an operating class. `MAX_OP_CLASS_ENTRIES` dimensions
                                    that array at 60, so a caller must clamp its read at that bound
                                    as well. */
    BOOL boot_only;    /**< True if the radio is capable only of on-boot scans; false if it can perform scans upon request. */
    UCHAR scan_impact; /**< Expected impact on Fronthaul/Backhaul operations during a channel scan. Use wifi_channel_scan_impact_t values. */
    op_class_ch_list_t op_class_ch_list[MAX_OP_CLASS_ENTRIES]; /**< Per-radio table of IEEE 802.11-2020 Table E-4 operating classes and their valid channels, pre-populated by the HAL so that upper layers can resolve operating class channel lists directly from capabilities without needing to call HAL functions, which are not accessible at that layer. */
} __attribute__((packed)) wifi_radio_capabilities_t;

/**
 * @brief Wi-Fi interface property information.
 */
typedef struct
{
    unsigned int phy_index; /*!< Index the platform gives the PHY device itself, as opposed to the
                                 upper-layer radio numbering `rdk_radio_index` below carries. This
                                 interface states no range for it and does not say whether it is
                                 what the `radioIndex` arguments elsewhere in this interface
                                 expect, so a caller must not substitute one for the other. */
    unsigned int rdk_radio_index; /*!< Index the upper layer gives the same radio. This member is
                                       the sense `wifi_radio_capabilities_t::rdk_radio_index` and
                                       `radio_interface_mapping_t::radio_index` are documented
                                       against; the whole purpose of this structure is that the two
                                       indices need not be equal, so a caller must map through it
                                       rather than assuming they are. */
    wifi_interface_name_t interface_name; /*!< Name of the network interface this entry maps, in the
                        32-byte `wifi_interface_name_t`. This interface does not state whether the
                        value is NUL-terminated, so a caller should bound any read at the declared
                        length. */
    wifi_interface_name_t mld_interface_name; /*!< Name of the multi-link device interface this
                        entry belongs to, in the same 32-byte `wifi_interface_name_t`. This
                        interface does not state what the member holds where the interface is not
                        part of an MLD. */
    wifi_interface_name_t bridge_name; /*!< Name of the bridge the interface is attached to, in the
                        same 32-byte `wifi_interface_name_t`. This interface does not state what the
                        member holds where the interface is not bridged. */
    int vlan_id; /*!< VLAN the interface's traffic carries. `int`, and this interface states neither
                      an accepted range nor a value that means untagged. */
    unsigned int index; /*!< Index of this entry. This interface does not state whether it is the
                             `VAP` index that the `apIndex` arguments elsewhere in this interface
                             take, so a caller should identify the `VAP` from `vap_name` beside it
                             rather than from this member. */
    wifi_vap_name_t vap_name; /*!< Name of the `VAP` this entry maps, in the 64-byte
                                   `wifi_vap_name_t`. Termination is not stated, so a caller should
                                   bound any read at the declared length. */
} __attribute__((packed)) wifi_interface_name_idex_map_t;

/**
 * @brief Radio interface mapping.
 */
typedef struct
{
    unsigned int phy_index; /*!< Index of the PHY device, in the sense
                                 `wifi_interface_name_idex_map_t::phy_index` documents - the actual
                                 device index rather than an upper-layer one. */
    unsigned int radio_index; /*!< Radio this PHY is mapped to. This interface does not state
                                   whether the value is the upper-layer index
                                   `wifi_interface_name_idex_map_t::rdk_radio_index` carries or the
                                   `index` of `wifi_radio_capabilities_t`, so a caller must not
                                   treat the three as interchangeable. */
    char radio_name[16]; /*!< Name of the radio, in the 16-byte array this declaration dimensions.
                              This interface states neither the encoding nor whether the value is
                              NUL-terminated, so a caller must bound any read at 16 bytes and must
                              not run an unbounded string function over it. */
    wifi_interface_name_t interface_name; /*!< Name of the interface this radio maps to, in the
                        32-byte `wifi_interface_name_t`. Termination is not stated, so a caller
                        should bound any read at the declared length. */
} __attribute__((packed)) radio_interface_mapping_t;

/**
 * @brief Wi-Fi Multi-Link information.
 */
typedef struct _wifi_multi_link_info_t
{
    wifi_multi_link_bands_t mu_bands; /*!< Bands the platform can combine in a multi-link device, as
                                           a bitmask of `wifi_multi_link_bands_t` values - those
                                           enumerators are distinct single bits, so several are set
                                           at once. */
    wifi_multi_link_modes_t mu_modes; /*!< Multi-link modes the platform supports - STR, non-STR,
                                           eMLSR and eMLMR - as a bitmask of
                                           `wifi_multi_link_modes_t` values, whose enumerators are
                                           distinct single bits.
                                           `wifi_radio_capabilities_t::mldOperationalCap` carries
                                           the same bitmask form per radio. */
} wifi_multi_link_info_t;

/**
 * @brief Wi-Fi platform properties.
 *
 * This structure contains information about the Wi-Fi platform properties.
 */
typedef struct 
{
    UINT numRadios; /*!< Number of radios the platform carries. `radiocap`, `radio_interface_map`
                         and `radio_presence` are each dimensioned `MAX_NUM_RADIOS`; this interface
                         does not state that this count bounds every one of them, so a caller should
                         clamp its reads at `MAX_NUM_RADIOS` as well. */
    wifi_radio_capabilities_t radiocap[MAX_NUM_RADIOS]; /*!< One capability record per radio,
                                                             dimensioned `MAX_NUM_RADIOS`, of which
                                                             `numRadios` above reports how many
                                                             radios the platform carries. Within an
                                                             entry it is that entry's own
                                                             `numSupportedFreqBand` that bounds its
                                                             band-indexed arrays, not `numRadios`. */
    wifi_interface_name_idex_map_t interface_map[(MAX_NUM_RADIOS * MAX_NUM_VAP_PER_RADIO)]; /*!< Interface-to-index
                        mappings for the platform's `VAP`s, dimensioned `MAX_NUM_RADIOS *
                        MAX_NUM_VAP_PER_RADIO`. This interface declares no count member for the
                        array, so how many entries are populated is not established here and a
                        caller has to recognise an unpopulated entry from its contents. */
    radio_interface_mapping_t radio_interface_map[MAX_NUM_RADIOS]; /*!< PHY-to-radio-to-interface
                        mappings, one `radio_interface_mapping_t` per radio, dimensioned
                        `MAX_NUM_RADIOS`. This interface declares no separate count for the array;
                        `numRadios` above reports how many radios the platform carries. */
    BOOL radio_presence[MAX_NUM_RADIOS]; /*!< `TRUE` where the radio's interface is present rather
                                              than in deep sleep. The array is indexed by radio
                                              position in step with `radio_interface_map` above, of
                                              which `numRadios` reports how many positions describe
                                              a radio, so a caller must not read past that count. */
    wifi_multi_link_info_t mu_info; /*!< Bands and modes the platform supports for multi-link
                                         operation, each carried as a bitmask -
                                         `wifi_multi_link_bands_t` for the bands and
                                         `wifi_multi_link_modes_t` for the modes - so several of
                                         either may be set at once. See `wifi_multi_link_info_t`. */
    UINT BssMaxStaAllow; /**< Maximum number of stations supported for the given platform. Gets populated during bring-up. */

    // Device Information related fields
    CHAR manufacturer[DEFAULT_DEVICE_FIELD_LEN]; /*!< Manufacturer name of the device, in the
                                                      `DEFAULT_DEVICE_FIELD_LEN` bytes that macro
                                                      declares as 64. This interface states neither
                                                      the encoding nor whether the value is
                                                      NUL-terminated, so a caller must bound any
                                                      read at 64 bytes and must not run an unbounded
                                                      string function over it. */
    CHAR serialNo[DEFAULT_DEVICE_FIELD_LEN]; /*!< Serial number of the device, in the same 64-byte
                                                  field width. Termination is not stated, so a
                                                  caller must bound any read at 64 bytes; this
                                                  interface states no format for the value, so it
                                                  must not be parsed for structure. */
    CHAR manufacturerModel[DEFAULT_DEVICE_FIELD_LEN]; /*!< Model name the manufacturer gives the
                                                           device, in the same 64-byte field width
                                                           and on the same terms: no encoding and no
                                                           termination are stated, so a caller must
                                                           bound any read at 64 bytes. */
    CHAR software_version[DEFAULT_DEVICE_FIELD_LEN]; /*!< Software version string of the device, in
                                                          the same 64-byte field width, with no
                                                          encoding and no termination stated. It is
                                                          the platform's own version and is
                                                          unrelated to the
                                                          `wifi_hal_capability_t::version` this
                                                          interface reports for the `HAL` itself. */
    mac_address_t cm_mac; /*!< Address of the device's cable modem interface, as the six raw octets
                               of `mac_address_t` rather than text. It identifies the DOCSIS side of
                               the device and is not the address of any Wi-Fi interface;
                               `al_1905_mac` below is a separate address again. */
    mac_address_t al_1905_mac; /*!< Abstraction Layer MAC address of the device, as the six raw
                                    octets of `mac_address_t` rather than text. IEEE Std 1905.1
                                    gives an abstraction-layer entity one `AL MAC` address that
                                    identifies the device across all of its interfaces, which is
                                    what the member name refers to; it has nothing to do with IEEE
                                    802.11v, and it is not the address of any single Wi-Fi
                                    interface. `cm_mac` above is a separate address again. This
                                    interface does not state what the member holds on a device that
                                    runs no abstraction layer. */
    int colocated_mode; /*!< Mode the EasyMesh agent runs in, as the controller configured it. This
                             interface enumerates no values for the member, states no range and
                             gives none meaning `not configured`, so a caller should preserve a
                             value it read back rather than composing one. */
} __attribute__((packed)) wifi_platform_property_t;

/**
 * @brief Wi-Fi HAL capabilities.
 */
typedef struct
{
    wifi_hal_version_t version; /*!< Version the `HAL` implementation reports, as the major and
                                     minor pair `wifi_hal_version_t` carries.
                                     `WIFI_HAL_MAJOR_VERSION` and `WIFI_HAL_MINOR_VERSION` give the
                                     version this header was written against; this interface does
                                     not state that the reported value must equal them, so a caller
                                     should compare rather than assume. The maintenance component
                                     `WIFI_HAL_MAINTENANCE_VERSION` carries is not part of this
                                     structure. */
    wifi_platform_property_t wifi_prop; /**< The platform property that includes the number of radios and supported frequency bands. */
    BOOL BandSteeringSupported; /*!< `TRUE` where the `HAL` can steer a client between bands. The
                                     calls that configure it - `wifi_getBandSteeringEnable()` and
                                     the threshold accessors beside it - are declared in
                                     `wifi_hal_client_mgt.h` rather than here, and this interface
                                     does not state what they do where the capability is `FALSE`. */
} __attribute__((packed)) wifi_hal_capability_t;

/**
 * @brief Wi-Fi TWT agreement type.
 */
typedef enum
{
    wifi_twt_agreement_type_individual, /**< Set an individual TWT session. */
    wifi_twt_agreement_type_broadcast, /**< Set a broadcast TWT session. */
} wifi_twt_agreement_type_t;

/**
 * @brief Wi-Fi TWT operation.
 */
typedef struct
{
    BOOL implicit; /*!< `TRUE` where the session is implicit and `FALSE` where it is explicit. This
                        interface does not define what either mode requires of a station beyond the
                        name, and `trigger_enabled` below states no dependency on this member, so a
                        caller must not read one from the other. */
    BOOL announced; /*!< `TRUE` where the session is announced and `FALSE` where it is unannounced.
                         It is independent of `implicit` above, and this interface defines neither
                         mode further, so a caller must not infer a frame exchange from the member. */
    BOOL trigger_enabled; /*!< `TRUE` where the `AP` sends a trigger frame within the session's
                               service periods, so a station waits for it before transmitting. This
                               interface states no further effect and no dependency on `implicit` or
                               `announced` above, so a caller must not read one from another. */
    UINT flowID; /*!< Identifier of this TWT agreement. This interface states neither an accepted
                      range nor the scope the value is unique within, and does not state how it
                      relates to `wifi_twt_sessions_t::IdTWTsession`, which identifies a session for
                      the `AP`. */
} wifi_twt_operation_t;

/**
 * @brief Wi-Fi TWT individual parameters.
 */
typedef struct
{
    UINT wakeTime_uSec; /*!< Target wake time of the session, in microseconds: the point at which a
                             service period starts, which is what IEEE Std 802.11-2020 calls the
                             Target Wake Time field of the TWT element. It is an instant, not a
                             length, and this interface states neither the epoch nor the clock it is
                             measured against, so a caller must take the time base from the
                             implementation it runs against. */
    UINT wakeInterval_uSec; /*!< Interval between the starts of two consecutive service periods, in
                                 microseconds - the TWT Wake Interval of that element. It is a
                                 spacing between service periods, not the length of one; the length
                                 is `minWakeDuration_uSec` below. */
    UINT minWakeDuration_uSec; /*!< Minimum length of a service period, in microseconds: how long
                                    the station stays awake within each one, which the TWT element
                                    carries as its Nominal Minimum TWT Wake Duration. It is a
                                    duration and not an interval, so it does not bound how often a
                                    service period recurs - `wakeInterval_uSec` above does that.
                                    This interface states no accepted range for it and does not
                                    state whether an implementation may extend a service period
                                    beyond the value. */
    UINT channel; /*!< Channel the individual TWT session operates on. This structure carries no
                       band member, and a channel number alone is ambiguous across bands, so a
                       caller must take the band from the radio the session belongs to. */
} wifi_twt_individual_params_t;

/**
 * @brief Wi-Fi TWT broadcast parameters.
 */
typedef struct
{
    UINT wakeDuration_uSec; /*!< Length of a service period of the broadcast session, in
                                 microseconds: how long a station stays awake within one. It is a
                                 duration and not an instant - this structure declares no target
                                 wake time, unlike `wifi_twt_individual_params_t::wakeTime_uSec` -
                                 and this interface states no accepted range for it. */
    UINT wakeInterval_uSec; /*!< Interval between the starts of two consecutive service periods of
                                 the broadcast session, in microseconds, on the same terms as
                                 `wifi_twt_individual_params_t::wakeInterval_uSec`: a spacing
                                 between service periods rather than the length of one. */
} wifi_twt_broadcast_params_t;

/**
 * @brief Wi-Fi TWT parameters.
 */
typedef struct
{
    wifi_twt_agreement_type_t agreement; /*!< Whether the session is individual or broadcast, as one
                                              of the two `wifi_twt_agreement_type_t` enumerators. It
                                              is what selects which arm of the `params` union below
                                              is valid, so a caller must set it before writing that
                                              member and read it before reading it. */
    wifi_twt_operation_t operation; /*!< How the session operates - implicit against explicit,
                                         announced against unannounced, whether a trigger frame is
                                         sent, and the agreement identifier - as
                                         `wifi_twt_operation_t` describes. */
    union
    {
        wifi_twt_individual_params_t individual; /*!< Wake time, wake interval, minimum wake
                                                      duration and channel of an individual session,
                                                      all three durations in microseconds per
                                                      `wifi_twt_individual_params_t`. It is the arm
                                                      of this union that is valid where `agreement`
                                                      is `wifi_twt_agreement_type_individual`. */
        wifi_twt_broadcast_params_t broadcast; /*!< Wake duration and wake interval of a broadcast
                                                    session, both in microseconds per
                                                    `wifi_twt_broadcast_params_t`. It is the arm of
                                                    this union that is valid where `agreement` is
                                                    `wifi_twt_agreement_type_broadcast`. */
    } params; /*!< Session parameters for the agreement type `agreement` names. Exactly one
                        arm is valid: `individual` when `agreement` is
                        `wifi_twt_agreement_type_individual`, and `broadcast` when it is
                        `wifi_twt_agreement_type_broadcast`. A caller must set `agreement`
                        before writing this member and must read `agreement` before reading
                        it; the arms overlap in storage, so reading the arm the agreement
                        type does not select reinterprets the other arm's bytes. */
    BOOL sessionPaused; /*!< `TRUE` where the session still exists but is not currently delivering
                             its service periods. This interface declares no call that pauses or
                             resumes a session and does not state what the `params` above describe
                             while the member is `TRUE`, so a caller must not read it as a statement
                             that those parameters have changed. */
} wifi_twt_params_t;

/**
 * @brief 802.11ax parameters.
 */
typedef struct
{
    wifi_twt_params_t twt_params; /*!< The one TWT session this 802.11ax parameter block carries.
                                       Its `agreement` member is what selects which arm of its
                                       `params` union is valid, so a caller must read that member
                                       first. */
} wifi_80211ax_params_t;

/**
 * @brief TWT device information.
 */
typedef struct
{
    UINT numTwtSession; /*!< Number of leading entries of `twtParams` below that describe a session
                             the device has joined. That array is dimensioned `MAX_NUM_TWT_SESSION`,
                             so a caller must clamp its read at that bound as well. */
    wifi_twt_params_t twtParams[MAX_NUM_TWT_SESSION]; /*!< Sessions the device has joined, of which
                                                           the leading `numTwtSession` entries above
                                                           are populated; `MAX_NUM_TWT_SESSION`
                                                           dimensions the array at 50, so a caller
                                                           must clamp its read at that bound as
                                                           well. Each entry's own `agreement`
                                                           selects the valid arm of its `params`
                                                           union. */
} wifi_twt_dev_info_t;

/**
 * @brief Wi-Fi TWT sessions.
 */
typedef struct
{
    wifi_twt_params_t twtParameters; /*!< Parameters of this session, whose `agreement` member
                                          selects the valid arm of its `params` union.
                                          `IdTWTsession` below identifies the session for the `AP`,
                                          and `macAddr` with `numDevicesInSession` name the devices
                                          that have joined it. */
    INT IdTWTsession; /*!< Identifier the `AP` uses for this session. This interface states no
                           accepted range for it and does not state how it relates to
                           `wifi_twt_operation_t::flowID`, which identifies the agreement, so a
                           caller must not treat the two as the same value. */
    mac_address_t macAddr[MAX_STA_PER_SESSION]; /*!< Addresses of the devices that have joined the
                                                     session, each as the six raw octets of
                                                     `mac_address_t` rather than text. The leading
                                                     `numDevicesInSession` entries below are
                                                     populated and `MAX_STA_PER_SESSION` dimensions
                                                     the array at 100, so a caller must clamp its
                                                     read at that bound as well. */
    UINT numDevicesInSession; /*!< Number of leading entries of `macAddr` above that name a device.
                                   This interface states no relation between it and the session
                                   parameters, so a caller must not read a device count out of
                                   `twtParameters`. */
} wifi_twt_sessions_t;

/**
 * @brief Wi-Fi RU types.
 */
typedef enum
{
    WIFI_RU_TYPE_26, /**< 26-tone RU. */
    WIFI_RU_TYPE_52, /**< 52-tone RU. */
    WIFI_RU_TYPE_106, /**< 106-tone RU. */
    WIFI_RU_TYPE_242, /**< 242-tone RU. */
    WIFI_RU_TYPE_484, /**< 484-tone RU. */
    WIFI_RU_TYPE_996, /**< 996-tone RU. */
    WIFI_RU_TYPE_2X996, /**< 2x996-tone RU. */
} wifi_ru_type_t;

/**
 * @brief Wi-Fi access category (AC) type.
 */
typedef enum
{
    wifi_access_category_background, /**< Background access category. */
    wifi_access_category_best_effort, /**< Best effort access category. */
    wifi_access_category_video, /**< Video access category. */
    wifi_access_category_voice, /**< Voice access category. */
} wifi_access_category_t;

/**
 * @brief Wi-Fi BSR (Buffer Status Report) information.
 */
typedef struct
{
    wifi_access_category_t access_category; /*!< Access category the buffer status is reported for,
                        as one of the four `wifi_access_category_t` enumerators. That enumeration
                        carries no assigned values, so a caller must compare against the enumerators
                        rather than against numbers. */
    UINT queue_size; /*!< Amount of traffic the station has queued for `access_category`. This
                          interface states neither the unit - octets, frames or an 802.11
                          buffer-status code - nor a range, so a caller must not assume one. */
} wifi_bsr_t;

/**
 * @brief Wi-Fi RU allocation information.
 */
typedef struct
{
    UCHAR subchannels; /*!< Count of subchannels the resource unit spans. This interface states
                            neither the width of a subchannel nor an accepted range for the count;
                            `type` beside it names the RU type as a `wifi_ru_type_t`. */
    wifi_ru_type_t type; /*!< Size of the resource unit, as one of the `wifi_ru_type_t` enumerators
                              - 26-tone through 2x996-tone. That enumeration assigns no values, so a
                              caller must compare against the enumerators rather than against
                              numbers; `subchannels` above is a separate count whose subchannel
                              width this interface does not state. */
} wifi_ru_allocation_t;

/**
 * @brief Wi-Fi downlink MU types.
 */
typedef enum
{
    WIFI_DL_MU_TYPE_NONE, /**< No MU. */
    WIFI_DL_MU_TYPE_OFDMA, /**< OFDMA. */
    WIFI_DL_MU_TYPE_MIMO, /**< MIMO. */
    WIFI_DL_MU_TYPE_OFDMA_MIMO /**< OFDMA and MIMO. */
} wifi_dl_mu_type_t;

/**
 * @brief Uplink multi-user scheme a radio is configured for.
 *
 * `wifi_setUplinkMuType()` and `wifi_getUplinkMuType()` in `wifi_hal_radio.h` write and read
 * this per radio, and `wifi_ul_mu_stats_t` reports the scheme a station's uplink traffic
 * actually used. The uplink set is deliberately smaller than the downlink `wifi_dl_mu_type_t`
 * set: this interface names no uplink MU-MIMO value, so a caller must not construct one by
 * analogy with the downlink type.
 *
 * Whether a radio accepts a value other than `WIFI_UL_MU_TYPE_NONE` is a platform fact this
 * interface does not settle; a caller establishes it from `wifi_getHalCapability()` or from
 * the return of the set call rather than from the presence of the enumerator.
 *
 * @see wifi_dl_mu_type_t
 * @see wifi_ul_mu_stats_t
 */
typedef enum
{
    WIFI_UL_MU_TYPE_NONE, /*!< No uplink multi-user operation: the radio schedules one station at a time on the uplink. */
    WIFI_UL_MU_TYPE_OFDMA, /*!< Uplink OFDMA, in which several stations transmit simultaneously on separate resource units of the same channel. The units allocated to one station are reported in `wifi_ul_mu_stats_t`. */
} wifi_ul_mu_type_t;

/**
 * @brief Downlink multi-user activity reported for one associated station.
 *
 * The `HAL` fills an instance as the `cli_DownlinkMuStats` member of `wifi_associated_dev3_t`,
 * so it reaches a caller through `wifi_getApAssociatedDeviceDiagnosticResult3()` and is read
 * rather than written by the caller. It describes what the scheduler did for that station on
 * the downlink: the multi-user scheme in use, the resource units allocated to it, and the
 * buffer status the station reported.
 *
 * @note Only `cli_AllocatedDownlinkRuNum` bounds a populated length here. This interface
 *       states no count for `cli_BufferStatus`, and no terminator value, so a caller cannot
 *       tell a reported entry from an unwritten one; it should treat the populated length of
 *       that array as unspecified.
 *
 * @see wifi_associated_dev3_t
 * @see wifi_ul_mu_stats_t
 * @see wifi_dl_mu_type_t
 */
typedef struct
{
    wifi_dl_mu_type_t cli_DownlinkMuType; /*!< Downlink multi-user scheme the station's traffic used, as one of the `wifi_dl_mu_type_t` values: none, OFDMA, MU-MIMO, or both together. */
    wifi_bsr_t cli_BufferStatus[MAX_BSR]; /*!< Buffer status reports received from the station, up to `MAX_BSR` of them, each pairing an access category with a queue size. How many entries are populated is not stated by this interface. */
    UCHAR cli_AllocatedDownlinkRuNum; /*!< Number of resource units allocated to the station on the downlink, and the count of valid leading entries in `cli_DownlinkRuAllocations`. A caller must clamp its read at `MAX_RU_ALLOCATIONS` as well, since that is the declared length of the array. */
    wifi_ru_allocation_t cli_DownlinkRuAllocations[MAX_RU_ALLOCATIONS]; /*!< The allocated downlink resource units, each giving its subchannel count and RU type. Entries beyond `cli_AllocatedDownlinkRuNum` are unspecified. */
} wifi_dl_mu_stats_t;

/**
 * @brief Uplink multi-user activity reported for one associated station.
 *
 * The downlink counterpart of `wifi_dl_mu_stats_t`, reaching a caller the same way as the
 * `cli_UplinkMuStats` member of `wifi_associated_dev3_t`, and read rather than written by the
 * caller.
 *
 * @note The first member is spelled `cli_UpinkMuType`, without the `l` of "Uplink". That
 *       spelling is part of the public interface and a caller must use it as declared;
 *       correcting it would break every existing consumer, so it is documented rather than
 *       renamed.
 * @note As in the downlink structure, `cli_AllocatedUplinkRuNum` is the only populated length
 *       stated here; the populated length of `cli_BufferStatus` is unspecified.
 *
 * @see wifi_associated_dev3_t
 * @see wifi_dl_mu_stats_t
 * @see wifi_ul_mu_type_t
 */
typedef struct
{
    wifi_ul_mu_type_t cli_UpinkMuType; /*!< Uplink multi-user scheme the station's traffic used, as one of the `wifi_ul_mu_type_t` values: none or OFDMA. The member name is missing an `l`; see the note on this structure. */
    UCHAR cli_ChannelStateInformation; /*!< A single octet of channel-state information for the uplink. This interface does not state its encoding, its units or its range, so a caller must not interpret it; it is not the channel-state matrix, which travels as `wifi_csi_data_t` through the `cli_CsiData` member of `wifi_associated_dev3_t`. */
    wifi_bsr_t cli_BufferStatus[MAX_BSR]; /*!< Buffer status reports received from the station, up to `MAX_BSR` of them. How many entries are populated is not stated by this interface. */
    UCHAR cli_AllocatedUplinkRuNum; /*!< Number of resource units allocated to the station on the uplink, and the count of valid leading entries in `cli_UplinkRuAllocations`, bounded by `MAX_RU_ALLOCATIONS`. */
    wifi_ru_allocation_t cli_UplinkRuAllocations[MAX_RU_ALLOCATIONS]; /*!< The allocated uplink resource units, each giving its subchannel count and RU type. Entries beyond `cli_AllocatedUplinkRuNum` are unspecified. */
} wifi_ul_mu_stats_t;

/**
 * @brief Kind of radio channel event being reported.
 *
 * The two members distinguish an ordinary change of the channels a radio occupies from a
 * radar detection, which is the event that forces a `DFS` channel to be vacated.
 *
 * No function or handler typedef in the documented surface names this type. The only
 * declaration that does is `wifi_chan_eventCB_t` in `wifi_hal_deprecated.h`, which this
 * repository declares out of scope while `wifi_hal.h` still includes that header - so the
 * type remains part of the compile surface a caller sees, and is documented here for that
 * reason rather than because a current call reports it. A caller that needs channel changes
 * from the current surface reads them with `wifi_getRadioChannelsInUse()`.
 *
 * @see wifi_getRadioChannelsInUse
 */
typedef enum
{
    WIFI_EVENT_CHANNELS_CHANGED, /*!< The set of channels the radio occupies has changed. This interface does not state the new set with the event; a caller reads it back with `wifi_getRadioChannelsInUse()`. */
    WIFI_EVENT_DFS_RADAR_DETECTED /*!< A radar signal was detected on a `DFS` channel. The channel becomes unusable for the regulatory non-occupancy period; see `State-Dependent Behavior` in `docs/pages/halSpec.md`. */
} wifi_chan_eventType_t;

/**
 * @brief Connection state reported for a client VAP.
 *
 * These are the values a caller reads out of `connect_status` in `wifi_station_stats_t`
 * (`wifi_hal_sta.h`) and out of the connection status a station-mode notification carries.
 * They are reported values, not a state machine: this interface states no legal transition
 * between them and no ordering, so a caller must not treat a sequence of reads as a
 * transition trace. See `State Diagram` in `docs/pages/halSpec.md`, which records that the
 * interface declares status enumerations rather than a state model.
 *
 * @see wifi_station_stats_t
 * @see wifi_getStationStats
 */
typedef enum
{
    wifi_connection_status_disabled, /*!< The client `VAP` is administratively disabled, so no connection is attempted. */
    wifi_connection_status_disconnected, /*!< The `VAP` is enabled but not associated. This value does not say why, and does not distinguish "never connected" from "connection lost". */
    wifi_connection_status_connected, /*!< The `VAP` is associated with a `BSS`. Association is not authorisation: this value does not state that key exchange or `RADIUS` authentication has completed. */
    wifi_connection_status_ap_not_found /*!< The target `BSS` was not found, so the association could not be attempted. A caller should re-scan with `wifi_findNetworks()` rather than retry the same target immediately. */
} wifi_connection_status_t;

/**
 * @brief Cause vocabulary for a RADIUS or EAP authentication failure.
 *
 * The two members name the two ways an authentication attempt is refused: the `RADIUS` server
 * rejected the request, or the `EAP` exchange itself failed.
 *
 * No declared signature in this interface names this type. The `RADIUS` and `EAP` failure
 * report in the current surface is the `INT failure_reason` argument of
 * `wifi_radiusEapFailure_callback` in `wifi_hal_ap.h`, whose own documentation names a
 * different enumeration, `wifi_eap_status_code_t`, with different values - so a caller must
 * not assume that argument carries a `radius_eap_failure_code_t` value. Which vocabulary an
 * implementation reports is not established by this interface.
 *
 * @see wifi_radiusEapFailure_callback
 */
typedef enum
{
    RADIUS_ACCESS_REJECT = 1, /*!< The `RADIUS` server answered Access-Reject: the credentials reached the server and were refused. Retrying with the same credentials will be refused again. */
    EAP_FAILURE /*!< The `EAP` exchange failed, which includes a supplicant that stopped responding as well as an explicit EAP-Failure. The value is `2`, since the enumeration starts at `1`. */
} radius_eap_failure_code_t;

/**
 * @brief Cause vocabulary for a RADIUS failover or fallback event.
 *
 * A deployment with more than one `RADIUS` server moves to a secondary server when the primary
 * stops answering, and returns to the primary when it recovers; these three members name the
 * initialization failure and the two transitions.
 *
 * As with `radius_eap_failure_code_t`, no declared signature names this type: the event
 * reaches a caller as the `INT failure_reason` argument of
 * `wifi_radiusFallback_failover_callback` in `wifi_hal_ap.h`, and this interface does not
 * state that the argument carries these values.
 *
 * @see wifi_radiusFallback_failover_callback
 */
typedef enum{
    RADIUS_INIT, /*!< The `RADIUS` client could not be initialized, so no server was contacted. This is a configuration or resource fault rather than a server fault. */
    RADIUS_FAILOVER, /*!< Authentication moved from the primary `RADIUS` server to a secondary one. Authentication continues; the primary is the component that failed. */
    RADIUS_FALLBACK /*!< Authentication returned to the primary `RADIUS` server after a failover. This reports a recovery rather than a failure. */
}radius_fallback_failover_code_t;

#define MAX_NR 4 /**< Maximum number of receive antennas. */
#define MAX_NC 1 /**< Maximum number of transmit antennas. */
#define MAX_SUB_CARRIERS 256 /**< Maximum number of subcarriers. */
#define MAX_PILOTS 26 /**< Maximum number of pilots. */

typedef INT wifi_streams_rssi_t[MAX_NR]; /*!< Per-receive-stream RSSI, dimensioned `MAX_NR` at 4.
                                              This interface states no unit for the values, and how
                                              many entries are populated follows from the capture's
                                              `Nr` rather than from the array bound. */
typedef UINT wifi_carrier_data_t[MAX_NR][MAX_NC]; /*!< Channel state information for one subcarrier,
                                                       indexed `[MAX_NR][MAX_NC]` - receive
                                                       dimension first. `wifi_evm_data_t` below is
                                                       indexed the other way round, so a caller must
                                                       not index the two alike. */
typedef wifi_carrier_data_t wifi_csi_matrix_t[MAX_SUB_CARRIERS]; /*!< Channel state information
                        across subcarriers, dimensioned `MAX_SUB_CARRIERS` at 256. How many of those
                        entries carry data follows from the capture's `num_sc`, `decimation` and
                        `valid_mask` in `wifi_frame_info_t` rather than from this bound, so a caller
                        must not read the array as fully populated. */
typedef UCHAR wifi_evm_data_t[MAX_NC][MAX_NR]; /*!< Error-vector-magnitude data for one pilot,
                        indexed `[MAX_NC][MAX_NR]`. That is the reverse of the `[MAX_NR][MAX_NC]`
                        order `wifi_carrier_data_t` above uses, so a caller must not index the two
                        alike. */
typedef wifi_evm_data_t wifi_evm_matrix_t[MAX_PILOTS]; /*!< EVM data for the pilots of one capture,
                        `MAX_PILOTS` entries of `wifi_evm_data_t`, each a per-pilot EVM value in dB
                        as `wifi_csi_data_t::evm_matrix` records. `MAX_PILOTS` dimensions the array
                        at 26 and this interface does not state how many entries a given capture
                        populates, so a caller should bound its reads at that declared value and
                        take the populated extent from the platform implementer. */

/**
 * @brief This structure holds information about the Wi-Fi frame.
 */
typedef struct _wifi_frame_info
{
    UCHAR bw_mode; /**< Bandwidth mode.
                    * Bit 0-3: 0:20MHz; 1:40MHz; 2:80MHz; 3:160MHz.
                    * Bit 4: 80+80MHz.
                    * Bit 4-7: 0:11n; 1:11ac. */
    UCHAR mcs; /**< Modulation and Coding Scheme (MCS) index, encoded as 11ac numbering. */
    UCHAR Nr; /*!< Receive dimension of this capture: it is what tells a caller how many of the
                   `MAX_NR` rows of each `wifi_carrier_data_t` entry, and how many entries of
                   `nr_rssi`, carry data. The declared bound is 4, and this interface does not state
                   what the unpopulated rows hold. */
    UCHAR Nc; /*!< Transmit dimension of this capture, bounding the `MAX_NC` columns of each
                   `wifi_carrier_data_t` entry, which this header dimensions at 1. `wifi_evm_data_t`
                   is indexed with this dimension first, the reverse of the CSI order, so a caller
                   must not index the two alike. */
    wifi_streams_rssi_t nr_rssi; /*!< RSSI of each receive stream of this frame, of which `Nr` above
                                      are populated; the array is dimensioned `MAX_NR` at 4, so a
                                      caller must clamp its read at that bound as well. This
                                      interface states no unit for the values. */
    USHORT valid_mask; /**< Bit mask that determines which regions of CSI capture (tones) are valid. One bit represents a 20MHz chunk. */
    USHORT phy_bw; /**< VAP bandwidth at the time of capture, indicated as 20, 40, 80, 160. */
    USHORT cap_bw; /*!< Bandwidth of the captured frame. `phy_bw` above is documented as carrying
                        20, 40, 80 or 160 directly, but this interface does not state that this
                        member uses the same scale, so a caller must not read one as the other. */
    UINT num_sc; /*!< Number of subcarriers the capture carries, which with `decimation` and
                      `valid_mask` above is what says how much of the 256-entry `wifi_csi_matrix_t`
                      is populated. This interface states no accepted range for it, so a caller
                      should clamp its reads at the array bound as well. */
    UCHAR decimation; /**< Value to indicate the degree to which the CSI matrix is decimated in terms of the number of subcarriers present. */
    UINT channel; /*!< Primary channel the frame was received on. This structure carries no band
                       member, and the same channel number denotes different channels in different
                       bands, so a caller must take the band from the radio the capture came from. */
    INT cfo; /*!< Centre frequency offset measured while demodulating the frame. This interface
                  states no unit for the value and no sentinel for `no measurement`, so a caller
                  must not assume Hz or parts per million. */
    ULLONG time_stamp; /**< PHY timestamp of CSI capture with at least millisecond resolution.
                        * Ideally, this can be resolved to a standard epoch format with millisecond resolution. */
} wifi_frame_info_t;

/**
 * @brief This structure holds information about the Wi-Fi CSI data.
 */
typedef struct _wifi_csi_data
{
    wifi_frame_info_t frame_info; /*!< PHY and capture parameters of the frame the CSI was taken
                                       from - bandwidth, MCS, antenna and stream counts, subcarrier
                                       count and timestamp. Its `Nr` and `Nc` members are what tell
                                       a caller which part of `csi_matrix` and `evm_matrix` carries
                                       non-zero values. */
    wifi_csi_matrix_t csi_matrix; /*!< Channel state information of the capture, as
                                       `MAX_SUB_CARRIERS` entries of `wifi_carrier_data_t`. Each
                                       such entry is the H matrix for one subcarrier, and this
                                       header declares it `[MAX_NR][MAX_NC]` with `MAX_NR` 4 and
                                       `MAX_NC` 1, so the declared shape is 4 by 1 and not 4 by 4:
                                       a caller has exactly one addressable column here whatever
                                       `frame_info.Nc` reports. `frame_info.Nr` is the number of
                                       rows that carry non-zero values, being the number of receive
                                       antennas, and `frame_info.Nc` is the number of non-zero
                                       columns, being the number of spatial streams in the packet.
                                       This interface does not state what an implementation does
                                       when it has more spatial streams to report than the declared
                                       single column holds, and does not state what the rows
                                       outside `Nr` hold, so a caller must bound every read at the
                                       declared `MAX_NR` and `MAX_NC` and take the populated extent
                                       from the platform implementer. */
    wifi_evm_matrix_t evm_matrix; /*!< Error-vector-magnitude data of the capture, one element per
                                       pilot, each element an EVM value expressed in dB. The array
                                       is declared `wifi_evm_matrix_t`, which this header
                                       dimensions `MAX_PILOTS` at 26, so 26 is the bound a caller
                                       reads against; this interface does not state how many of
                                       those entries a given capture populates. Within an element
                                       the scheme follows `csi_matrix` above - `frame_info.Nr`
                                       non-zero rows and `frame_info.Nc` non-zero columns - but the
                                       element type `wifi_evm_data_t` is indexed
                                       `[MAX_NC][MAX_NR]`, the reverse of the CSI order, so a
                                       caller must not index the two alike. */
} wifi_csi_data_t;

/**
 * @brief Information about an associated device.
 */
typedef struct _wifi_associated_dev3
{
    mac_address_t cli_MACAddress; /*!< Address of the associated device, as the six raw octets of
                                       `mac_address_t` rather than text. `cli_IPAddress` below is
                                       the printable field, and its own comment marks it deprecated
                                       and to be left empty. */
    CHAR cli_IPAddress[64]; /*!< Printable address field of the associated device, in 64 bytes. It
                                 is deprecated and is to be left empty, so a caller must not read an
                                 address out of it; where a caller does read it, no encoding and no
                                 termination are stated, so the read must be bounded at 64 bytes. */
    BOOL cli_AuthenticationState; /*!< `TRUE` where the device has completed authentication with the
                                       `AP`. This interface does not state which exchange that
                                       covers - open authentication, the four-way handshake or an
                                       EAP conversation - so a caller must not read the member as
                                       meaning a particular key is installed. */
    UINT cli_LastDataDownlinkRate; /**< The median PHY rate in Mbps of the most recent 16 unicast data frame transmissions from the access point to the associated device. */
    UINT cli_LastDataUplinkRate; /**< The median PHY rate in Mbps of the most recent 16 unicast data frame transmissions from the associated device to the access point. */
    INT cli_SignalStrength; /**< An indicator of radio signal strength of the uplink from the associated device to the access point, measured in dBm, as an average of the last 100 packets received from the device. */
    UINT cli_Retransmissions; /**< The number of packets that had to be re-transmitted, from the last 100 packets sent to the associated device. Multiple re-transmissions of the same packet count as one. */
    BOOL cli_Active; /**< Whether or not this node is currently present in the WiFi AccessPoint network. */
    CHAR cli_OperatingStandard[64]; /**< Radio standard the associated Wi-Fi client device is operating under. Enumeration of: */
    CHAR cli_OperatingChannelBandwidth[64]; /**< The operating channel bandwidth of the associated device. The channel bandwidth (applicable to 802.11n and 802.11ac specifications only). Enumeration of: */
    INT cli_SNR; /**< A signal-to-noise ratio (SNR) compares the level of the Wi-Fi signal to the level of background noise. Sources of noise can include microwave ovens, cordless phone, bluetooth devices, wireless video cameras, wireless game controllers, fluorescent lights and more. It is measured in decibels (dB). */
    CHAR cli_InterferenceSources[64]; /**< Wi-Fi operates in two frequency ranges (2.4 Ghz and 5 Ghz) which may become crowded with other radio products that operate in the same ranges.
                                         * This parameter reports the probable interference sources that this Wi-Fi access point may be observing. The value of this parameter is a comma-separated list of the following possible sources:
                                         * e.g., MicrowaveOven,CordlessPhone,BluetoothDevices,FluorescentLights,ContinuousWaves,Others. */
    ULONG cli_DataFramesSentAck; /**< The DataFramesSentAck parameter indicates the total number of MSDU frames marked as duplicates and non-duplicates acknowledged.
                                    * The value of this counter may be reset to zero when the CPE is rebooted. Refer to section A.2.3.14 of the CableLabs Wi-Fi MGMT Specification. */
    ULONG cli_DataFramesSentNoAck; /**< The DataFramesSentNoAck parameter indicates the total number of MSDU frames retransmitted out of the interface (i.e., marked as duplicate and non-duplicate)
                                     * and not acknowledged, but does not exclude those defined in the DataFramesLost parameter. The value of this counter may be reset to zero when the CPE is rebooted.
                                     * Refer to section A.2.3.14 of the CableLabs Wi-Fi MGMT Specification. */
    ULONG cli_BytesSent; /**< The total number of bytes transmitted to the client device, including framing characters. */
    ULONG cli_BytesReceived; /**< The total number of bytes received from the client device, including framing characters. */
    INT cli_RSSI; /**< The Received Signal Strength Indicator (RSSI) parameter is the energy observed at the antenna receiver for transmissions from the device averaged over the past 100 packets received from the device. */
    INT cli_MinRSSI; /**< The Minimum Received Signal Strength Indicator (RSSI) parameter is the minimum energy observed at the antenna receiver for past transmissions (100 packets). */
    INT cli_MaxRSSI; /**< The Maximum Received Signal Strength Indicator (RSSI) parameter is the energy observed at the antenna receiver for past transmissions (100 packets). */
    UINT cli_Disassociations; /**< This parameter represents the total number of client disassociations. Reset the parameter every 24hrs or reboot. */
    UINT cli_AuthenticationFailures; /**< This parameter indicates the total number of authentication failures. Reset the parameter every 24hrs or reboot. */
    ULLONG cli_Associations; /**< Stats handle used to determine reconnects; increases for every association (stat delta calculation). */
    ULONG cli_PacketsSent; /*!< Packets sent to this device, counted cumulatively. It counts packets
                                rather than the octets `cli_BytesSent` counts, and the error and
                                retransmission counters below are separate figures rather than
                                subsets a caller may subtract from this one. */
    ULONG cli_PacketsReceived; /*!< Packets received from this device, counted cumulatively, and the
                                    receive-side counterpart of `cli_PacketsSent` above. It counts
                                    packets rather than octets, and this interface states neither
                                    the interval it covers nor what resets it. */
    ULONG cli_ErrorsSent; /**< The total number of outbound packets that could not be transmitted because of errors. These might be due to the number of retransmissions exceeding the retry limit, or from other causes. */
    ULONG cli_RetransCount; /**< The total number of transmitted packets that were retransmissions for each client on the vAP.
                             * Two retransmissions of the same packet result in this counter incrementing by two. Three retransmissions of the same packet result in this counter incrementing by three, and so on. */
    ULONG cli_FailedRetransCount; /**< The number of packets that were not transmitted successfully due to the number of retransmission attempts exceeding an 802.11 retry limit. */
    ULONG cli_RetryCount; /**< The number of packets that were successfully transmitted after one or more retransmissions. */
    ULONG cli_MultipleRetryCount; /**< The number of packets that were successfully transmitted after more than one retransmission. */
    UINT cli_MaxDownlinkRate; /**< The maximum data transmit rate in Mbps for the access point to the associated device. */
    UINT cli_MaxUplinkRate; /**< The maximum data transmit rate in Mbps for the associated device to the access point. */
    wifi_dl_mu_stats_t cli_DownlinkMuStats; /*!< Multi-user downlink counters for this client, as
                                                 `wifi_dl_mu_stats_t` describes. This interface
                                                 states neither the interval those counters cover
                                                 nor when they reset, so a caller should difference
                                                 successive reads rather than treat a single sample
                                                 as a rate. */
    wifi_ul_mu_stats_t cli_UplinkMuStats; /*!< Multi-user uplink counters for this client, as
                                               `wifi_ul_mu_stats_t` describes, on the same terms as
                                               `cli_DownlinkMuStats` above: no interval and no reset
                                               point are stated. */
    wifi_twt_dev_info_t cli_TwtParams; /*!< Target Wake Time sessions this client has joined; the
                                            `numTwtSession` member of `wifi_twt_dev_info_t` bounds
                                            the entries a caller may read. */

    /** To facilitate retrieval of CSI data for a specific associated client, an existing RDK-B Wi-Fi HAL
     * function is being extended. In the current implementation, `wifi_getApAssociatedDeviceDiagnosticResult3`
     * retrieves a variety of statistics and state-specific information for associated clients.
     * The `wifi_associated_dev3_t` data structure is filled by native WLAN drivers for each associated client
     * as and when the function is called by the RDK-B application/process. A new component structure,
     * `wifi_csi_data_t`, is being defined that is part of the `wifi_associated_dev3_t` structure and needs to be
     * allocated and filled for a specific client or list of clients when
     * `wifi_getApAssociatedDeviceDiagnosticResult3` API is called by the RDK-B application/process. In cases when
     * the application needs CSI data, the RDK-B application will call
     * `INT wifi_getApAssociatedDeviceDiagnosticResult3(INT apIndex, wifi_associated_dev3_t **associated_dev_array, UINT *output_array_size)`
     * by allocating the `associated_dev_array` memory for `output_array_size` number of client
     * devices. In other words, `output_array_size` will specify the number of client devices in the array for
     * which CSI data needs to be filled by the driver. The `cli_MACAddress` will specify the client devices in each
     * of `wifi_associated_dev3_t`. The Wi-Fi HAL implementation in such a case MUST allocate memory for
     * `cli_CsiData` and fill in the required fields. The caller in such cases is responsible for deallocating
     * the memory. The `wifi_csi_data_t` is defined above.
     *
     * The member is therefore the one pointer in this structure whose target the `HAL` allocates: it is NULL
     * where the implementation supplied no CSI data for that client, and where it is non-NULL the caller frees
     * it. This interface does not name the allocator that was used, so the matching release function is a
     * platform convention rather than something established here. */
    wifi_csi_data_t *cli_CsiData;

    UINT cli_activeNumSpatialStreams; /**< The number of active spatial streams in the session between the AP and client at the moment of polling. */
    ULLONG cli_TxFrames; /*!< Frames sent to this client, counted cumulatively. This interface
                              states neither the interval the count covers nor what resets it, so a
                              caller should difference successive reads; `cli_RxRetries` below
                              counts on the receive path and is not comparable with this member. */
    ULLONG cli_RxRetries; /*!< Retry count on the receive path for this client. This interface
                               states neither what is counted - frames received carrying the retry
                               bit, or receive attempts the `HAL` repeated - nor the interval the
                               count covers, so a caller must not compare it against `cli_TxFrames`
                               above as though the two were the same kind of quantity. */
    ULLONG cli_RxErrors; /*!< Error count on the receive path for this client. This interface states
                              neither what it counts as an error - CRC failures, decryption failures
                              or discards - nor the interval the count covers, so a caller cannot
                              derive a rate from a single read. */
    BOOL cli_MLDEnable; /*!< Indicates whether the connected client uses a single link or multi-link connections: `FALSE` for a single link, `TRUE` for multi-link. When it is `FALSE` the three members that follow describe no active multi-link session. */
    wifi_multi_link_modes_t cli_MLModeCapa; /*!< Bitmap of the MLD operation modes the client supports, formed from `wifi_multi_link_modes_t` values. It reports the client's capability rather than the mode in use, and this interface does not state a value for a client that supports none. */
    BOOL cli_TIDLinkMapNegotiation; /*!< Indicates whether the client supports TID-to-link mapping negotiation. This is a capability of the client, not a statement that a mapping has been negotiated. */
    mac_address_t cli_MLDAddr; /*!< The MLD MAC address of the connected client, six octets. It is all zeroes for a client that is not Wi-Fi 7 capable, which is the only absence marker this interface defines for the member. */
} wifi_associated_dev3_t;

/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Reads what this platform's Wi-Fi subsystem actually supports.
 *
 * This function returns the features and configuration supported by the HAL. It is the
 * discovery step a caller performs before configuring anything: the report carries the
 * interface version the implementation was built to, the number of radios present and one
 * capability record per radio - bands, channel lists, bandwidths, modes, transmit powers,
 * ciphers and countries - together with the index-to-interface-name maps, the per-radio
 * presence flags, the station limit, the device identity fields and whether band steering is
 * supported. Those are the values that decide which indices and which settings the rest of
 * the interface will accept, which is why `Method Sequencing` in `docs/pages/halSpec.md`
 * places this call before any configuration call, and why the constants this header declares
 * bound the surface rather than describe one platform.
 *
 * @param[out] cap  Pointer to a `wifi_hal_capability_t` structure to store the
 *                  HAL capabilities. One structure is written, not an array. The caller
 *                  allocates and owns the storage, per `Memory Model` in
 *                  `docs/pages/halSpec.md`. This interface does not specify whether the
 *                  implementation retains the pointer beyond the call, so a caller must not
 *                  assume it may free or reuse the storage immediately on return: it should
 *                  keep the object allocated and unmoved while the `HAL` stays initialised,
 *                  or establish the actual lifetime with the platform's implementer. The
 *                  effect of passing `NULL` is not specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup` in
 *      `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results in
 *      undefined behaviour; `State-Dependent Behavior` records that a function may report
 *      `WIFI_HAL_NOT_READY` or fail until initialization has completed.
 * @post On success `cap` holds the platform's capability report and may be read. On failure
 *       its contents are unspecified, including the counts that bound its embedded arrays,
 *       so a caller must not read any part of it, per `Caller Responsibilities` under
 *       `Memory Model`.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The capability report was written to `cap`.
 * @retval WIFI_HAL_ERROR   The capability report was not produced. This interface does not
 *                          enumerate the conditions that lead to this code, so a caller
 *                          cannot infer from it what went wrong; it should treat the
 *                          platform's capabilities as unknown, log the failure, and not fall
 *                          back to assuming the maximum configuration the constants in this
 *                          header allow.
 *
 * @note These are the only two codes this block documents. `Internal Error Handling` in
 *       `docs/pages/halSpec.md` fixes the wider vocabulary the interface uses, and
 *       `WIFI_HAL_SUCCESS` is numerically identical to the `RETURN_OK` alias, so a caller may
 *       compare against either spelling.
 * @note `wifi_hal_capability_t` embeds one `wifi_radio_capabilities_t` per radio and is large
 *       for an automatic object; a caller on a constrained stack should allocate it
 *       elsewhere. It is a packed structure, so a caller must not take the address of a
 *       member and pass it where an aligned object is required.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_hal_capability_t
 * @see wifi_platform_property_t
 * @see wifi_radio_capabilities_t
 * @see wifi_init
 */
INT wifi_getHalCapability(wifi_hal_capability_t *cap);

/**
 * @brief Returns the whole Wi-Fi subsystem to its factory state.
 *
 * This function clears internal variables to implement a factory reset of
 * the Wi-Fi subsystem. It is one of the three teardown calls this interface declares, the
 * other two being `wifi_reset()` and `wifi_down()`, and it is the widest of them: it discards
 * the state the implementation holds rather than restarting it or silencing the radios. It
 * takes no index, so it applies to every radio and every Access Point at once; the per-radio
 * and per-Access-Point equivalents are `wifi_factoryResetRadio()` and `wifi_factoryResetAP()`
 * in `wifi_hal_radio.h` and `wifi_hal_ap.h`.
 *
 * What "factory state" contains is the implementation's own default set. `Persistence Model`
 * in `docs/pages/halSpec.md` places Wi-Fi configuration with the upper layer, so this call is
 * not a way to recover a caller's configuration: a caller that wants a known configuration
 * afterwards reapplies it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup` in
 *      `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime requirements
 *      and, per `Component Runtime Execution Requirements`, likely results in undefined
 *      behaviour.
 * @post On success the implementation's Wi-Fi state is at its factory default and the
 *       initialization pre-condition applies again, so a caller re-initializes with
 *       `wifi_init()` before using the interface further; see `Object Lifecycles` and
 *       `Method Sequencing` in `docs/pages/halSpec.md`. On failure this interface does not
 *       state how much of the reset was carried out, so a caller must treat the subsystem's
 *       state as unknown rather than as unchanged, and re-establish it rather than continue.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The factory reset completed.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. Because the resulting state
 *                          is unspecified, the caller should re-initialize and read the
 *                          configuration back rather than retry blindly.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`. It is
 *       therefore not a completion signal for the underlying work: a caller must not read a
 *       successful return as evidence that every radio has already been re-programmed.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. A caller should still avoid issuing configuration calls
 *       concurrently with a teardown call, because this interface states no ordering between
 *       them and the resulting configuration would depend on which arrived first.
 *
 * @see wifi_init
 * @see wifi_reset
 * @see wifi_down
 */
INT wifi_factoryReset();

/**
 * @brief Lights or extinguishes the Wi-Fi status indicator.
 *
 * The call drives a status indicator rather than the radio behind it: it does not enable or
 * disable transmission, and a caller that wants to bring a radio up or down uses
 * `wifi_setRadioEnable()` in `wifi_hal_radio.h` instead.
 *
 * The declared purpose is the *system* indicator while the signature takes a radio index, and
 * this interface does not reconcile the two: it states neither which physical indicator an
 * index selects, nor whether each radio has an indicator of its own, nor what a platform with
 * a single shared indicator does with the index. A caller should therefore treat the index as
 * naming the radio whose state it wants reflected, and must not infer that distinct indices
 * drive distinct indicators.
 *
 * @param[in] radioIndex Index of the Wi-Fi radio channel. The indices this interface defines
 *                       are `RADIO_INDEX_1` to `RADIO_INDEX_3`, bounded by `MAX_NUM_RADIOS`;
 *                       which of them a platform provisions comes from
 *                       `wifi_getHalCapability()` and is not established here. The parameter
 *                       is `INT` while the interface's own index type
 *                       `wifi_radio_index_t` is unsigned, and no negative index is defined,
 *                       so a caller converting between the two forms should reject a negative
 *                       value rather than pass it on.
 * @param[in] enable     Whether to enable or disable the LED. Pass `TRUE` (equivalently
 *                       `ENABLE`, `1`) to light the indicator and `FALSE` (`0`) to extinguish
 *                       it. `BOOL` is an unsigned char in this interface and no meaning is
 *                       defined for any other value, so a caller should pass only those two.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup` in
 *      `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime requirements
 *      and, per `Component Runtime Execution Requirements`, likely results in undefined
 *      behaviour.
 * @post On success the indicator for `radioIndex` reflects the requested state. This interface
 *       declares no call that reads the indicator back, so the requested state is not
 *       observable through it, and `Persistence Model` in `docs/pages/halSpec.md` leaves
 *       whether the state survives a restart to the upper layer. On failure this interface
 *       does not state whether the indicator changed, so a caller that needs a known state
 *       should reissue the call rather than assume the previous state held.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The indicator state was applied.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should confirm the
 *                          radio inventory with `wifi_getHalCapability()`; a failure that
 *                          persists across retries should be logged and treated as the
 *                          indicator being unavailable, since nothing else in the interface
 *                          depends on it.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`.
 *
 * @see wifi_getHalCapability
 * @see wifi_init
 */
INT wifi_setLED(INT radioIndex, BOOL enable);

/**
 * @brief Initializes all Wi-Fi radios and makes the rest of the interface usable.
 *
 * This is the lifecycle root of the whole Wi-Fi `HAL`. `Initialization and Startup` in
 * `docs/pages/halSpec.md` requires it to be called before any other function in the
 * interface, and every other declaration's `@pre` refers back to it: nothing else in the
 * interface - not a read, not a capability query, not a registration - is defined before this
 * call has completed successfully. `State-Dependent Behavior` records that this is the one
 * state distinction the interface makes uniformly, and that a function may report
 * `WIFI_HAL_NOT_READY` or fail until initialization has completed.
 *
 * What a caller may assume once it has succeeded is bounded and worth stating exactly. The
 * initialization pre-condition of every other function is met, so the interface may be
 * driven; `Process Model` establishes one instance per process whose lifetime is the process
 * lifetime, so no handle is created and none needs to be kept; and the order to proceed in is
 * the one `Method Sequencing` gives - discover with `wifi_getHalCapability()`, then configure
 * a radio with `wifi_setRadioOperatingParameters()`, then create the Virtual Access Points on
 * it with `wifi_createVAP()`, registering any handler before the events it wants. What a
 * caller may **not** assume is that any radio is on the air or carrying traffic: this call
 * initializes radios, and enabling, configuring and creating `VAP`s are separate calls.
 *
 * @pre No call in this interface must precede this one; it is the entry point, and
 *      `Initialization and Startup` states that the kernel boot sequence is expected to have
 *      started the `HAL`'s dependencies. This interface does not state the effect of calling
 *      it a second time without an intervening `wifi_reset()`, `wifi_down()` or
 *      `wifi_factoryReset()`, and `Process Model` admits only one initialized instance, so a
 *      caller must not treat it as idempotent.
 * @post On success every other function's initialization pre-condition is met for the lifetime
 *       of the process, or until one of the three teardown calls is issued, after which the
 *       pre-condition applies again and the interface must be re-initialized. On failure this
 *       interface does not state whether any radio was initialized, so a caller must not
 *       proceed to any other call: the correct response is to report the failure and leave the
 *       interface alone, because a later call's behaviour is undefined rather than merely
 *       unsuccessful.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS Initialization completed and the interface may be used.
 * @retval WIFI_HAL_ERROR   Initialization failed. The caller must not use any other function
 *                          in the interface. Because this interface states no retry policy and
 *                          no timeout - see `Blocking calls` - any retry interval is the
 *                          caller's own design decision, and a failure that persists should be
 *                          logged and treated as the Wi-Fi subsystem being unavailable.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`. A
 *       successful return is therefore not evidence that every radio has finished coming up,
 *       and this interface declares no completion notification for initialization; a caller
 *       that needs to observe readiness reads it back, for instance with
 *       `wifi_getRadioStatus()` in `wifi_hal_radio.h`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. That expectation does not extend to two concurrent
 *       `wifi_init()` calls, whose effect this interface does not state, and `Process Model`
 *       admits a single initialized instance - so a caller should initialize once, from one
 *       thread, before making the interface available to others.
 *
 * @see wifi_getHalCapability
 * @see wifi_reset
 * @see wifi_down
 * @see wifi_factoryReset
 */
INT wifi_init();

/**
 * @brief Resets the Wi-Fi subsystem and the state held for every Access Point.
 *
 * This function resets the Wi-Fi subsystem, including all Access Point
 * variables. It is the middle of the three teardown calls: `wifi_down()` silences the radios,
 * this call discards the running state, and `wifi_factoryReset()` returns the implementation
 * to its defaults. This interface describes the three by what they do and does not state how
 * the resulting states differ beyond those descriptions, so a caller should choose by the
 * description rather than assume a hierarchy between them.
 *
 * It takes no index, so it applies to the whole subsystem; there is no per-radio or
 * per-Access-Point form of this call in the interface. `Module Responsibilities` under
 * `Memory Model` in `docs/pages/halSpec.md` binds the implementation to release the memory it
 * holds, so that nothing leaks across a reset or a re-initialization - which is the property
 * that makes reset-and-re-initialize a usable recovery path for a caller.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup` in
 *      `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime requirements
 *      and, per `Component Runtime Execution Requirements`, likely results in undefined
 *      behaviour.
 * @post On success the subsystem's state, including the variables held for each Access Point,
 *       has been discarded, and the initialization pre-condition applies again: a caller
 *       re-initializes with `wifi_init()` and reapplies the configuration it wants before
 *       using the interface further; see `Object Lifecycles` and `Method Sequencing`. Because
 *       `Persistence Model` places configuration with the upper layer, a caller must not
 *       expect its configuration to be restored by the reset. On failure this interface does
 *       not state how much state was discarded, so the subsystem's state is unknown rather
 *       than unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The subsystem was reset.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should treat the
 *                          subsystem as being in an unknown state, and either re-initialize
 *                          and read the configuration back or escalate, rather than
 *                          continue issuing configuration calls.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, so a
 *       successful return is not evidence that the subsystem has finished restarting.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. A caller should still not issue configuration calls
 *       concurrently with this one: the interface states no ordering between them, so the
 *       state that survives would depend on which arrived first.
 *
 * @see wifi_init
 * @see wifi_down
 * @see wifi_factoryReset
 */
INT wifi_reset();

/**
 * @brief Takes every radio off the air by turning transmit power off.
 *
 * This function turns off transmit power for all radios in the Wi-Fi
 * subsystem. It is the narrowest of the three teardown calls in intent - it stops
 * transmission rather than discarding state - but it applies to the whole subsystem: it takes
 * no index, and the per-radio equivalent is `wifi_setRadioEnable()` in `wifi_hal_radio.h`,
 * which a caller should use to silence one radio and leave the others operating.
 *
 * A caller uses it when Wi-Fi must stop radiating while the process continues to run - during
 * a regulatory or maintenance action, for instance. `Object Lifecycles` in
 * `docs/pages/halSpec.md` nonetheless counts it as a teardown, so the initialization
 * pre-condition applies again afterwards.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup` in
 *      `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime requirements
 *      and, per `Component Runtime Execution Requirements`, likely results in undefined
 *      behaviour.
 * @post On success no radio in the subsystem is transmitting. This interface does not state
 *       whether the configuration a caller applied is retained across the call, and
 *       `Persistence Model` places configuration with the upper layer, so a caller should be
 *       prepared to reapply it; nor does it declare a call that reverses this one, so a caller
 *       returns to service by re-initializing with `wifi_init()` and configuring again, per
 *       `Method Sequencing`. On failure this interface does not state whether any radio was
 *       silenced, so a caller that must guarantee no transmission has to verify it by another
 *       means rather than infer it from the return code.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS Transmit power was turned off for every radio.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. Because a partial effect is
 *                          not excluded, a caller with a regulatory reason for silencing
 *                          the radios must escalate rather than treat the failure as
 *                          leaving the previous state intact.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, so a
 *       successful return is not evidence that every radio has already ceased transmitting.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. This interface states no ordering between this call and a
 *       concurrent configuration or enable call, so a caller should not issue them together.
 *
 * @see wifi_init
 * @see wifi_reset
 * @see wifi_factoryReset
 */
INT wifi_down();

/**
 * @brief Creates the implementation's initial Wi-Fi configuration files.
 *
 * This function creates Wi-Fi configuration files. The format and content
 * of these files are implementation-dependent. The call exists so that a caller can ask the
 * implementation to lay down its own starting configuration - typically on first boot or
 * after a factory reset - without knowing what that configuration is.
 *
 * Everything about the files is left to the implementation: this interface names no path, no
 * file, no format and no encoding, and declares no call that reads them back. A caller
 * therefore cannot inspect, validate or remove them through this interface, and must not build
 * behaviour on their contents. The one path constant this header declares,
 * `RESTORE_CNFG_FILE_NAME`, is not stated to be one of these files: `Persistence Model` in
 * `docs/pages/halSpec.md` records that no function in this interface is stated to read or
 * write it.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup` in
 *      `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime requirements
 *      and, per `Component Runtime Execution Requirements`, likely results in undefined
 *      behaviour.
 * @post On success the implementation's initial configuration files exist. This interface does
 *       not state whether an existing file is overwritten or left as it is, so a caller must
 *       not use this call to reset a configuration it has since changed. On failure this
 *       interface does not state whether some files were created, so the file set may be
 *       incomplete rather than absent.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The initial configuration files were created.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. Since a partially created set
 *                          is not excluded, the caller should log the failure and not
 *                          assume a usable configuration exists.
 *
 * @note This interface states nothing about the location, ownership or permissions of the
 *       files created, so a caller must not rely on them for any security property, and must
 *       not assume that the state it wants is protected because this call succeeded.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`, so a
 *       successful return is not evidence that every file has been written to storage.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. This interface states no ordering between this call and a
 *       concurrent configuration call that might write the same files.
 *
 * @see wifi_createHostApdConfig
 * @see wifi_init
 */
INT wifi_createInitialConfigFiles();

/**
 * @brief Creates the WPA and WPS configuration variables one Access Point needs.
 *
 * This function creates configuration variables needed for WPA/WPS. These
 * variables are implementation-dependent and, in some implementations, are
 * used by `hostapd` when it is started. Together with `wifi_startHostApd()` and
 * `wifi_stopHostApd()` it is the caller's only interface to the optional `hostapd` path
 * described under `Optional Components` in `docs/pages/halSpec.md`; a vendor that reaches the
 * driver by a proprietary route still implements all three, because they are part of the
 * interface, so a successful return does not establish that `hostapd` is in use on the
 * platform. `wifi_getLibhostapd()` in `wifi_hal_ap.h` is the call that answers that question.
 *
 * Ordering matters here: the variables are the input `hostapd` reads when it starts, so this
 * call precedes `wifi_startHostApd()` for the configuration to have effect. The interface does
 * not state that a caller must create the variables for every Access Point before starting,
 * nor what a start does for an Access Point whose variables were never created.
 *
 * @param[in] apIndex      Access Point index, in the range `AP_INDEX_1` to `AP_INDEX_24`.
 *                         Which of those indices a platform provisions comes from
 *                         `wifi_getHalCapability()` and is not established by this interface.
 *                         One Access Point is configured per call, so a caller repeats the
 *                         call per Access Point.
 * @param[in] createWpsCfg Whether to create WPS configuration variables. `TRUE` creates the
 *                         `WPS` variables alongside the `WPA` ones; `FALSE` creates only the
 *                         `WPA` variables. `BOOL` is an unsigned char in this interface and no
 *                         meaning is defined for any other value, so a caller should pass only
 *                         `TRUE` or `FALSE`. This interface does not state whether `FALSE`
 *                         removes `WPS` variables created by an earlier call, so a caller must
 *                         not use it to disable `WPS`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup` in
 *      `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime requirements
 *      and, per `Component Runtime Execution Requirements`, likely results in undefined
 *      behaviour.
 * @post On success the configuration variables for `apIndex` exist and are what a subsequent
 *       `wifi_startHostApd()` uses. This interface does not state whether a `hostapd` already
 *       running picks the change up, so a caller changing the configuration of a running
 *       Access Point should either restart the daemon with `wifi_stopHostApd()` and
 *       `wifi_startHostApd()`, or, on a platform that uses the library form, call
 *       `wifi_updateLibHostApdConfig()`. On failure this interface does not state whether any
 *       variable was written, so the Access Point's configuration is unknown rather than
 *       unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The configuration variables for `apIndex` were created.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should confirm the
 *                          Access Point inventory with `wifi_getHalCapability()` and must
 *                          not start `hostapd` expecting the configuration it asked for.
 *
 * @warning The variables this call creates carry the Access Point's security material, since
 *          that is what a `WPA` or `WPS` configuration consists of. This interface states
 *          nothing about where they are held or who can read them, so a caller must not treat
 *          their creation as evidence that the material is protected, and must not log or
 *          trace their contents if it obtains them by another route.
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. This interface states no ordering between this call and a
 *       concurrent `wifi_startHostApd()`, so a caller should not issue them together.
 *
 * @see wifi_startHostApd
 * @see wifi_stopHostApd
 * @see wifi_getLibhostapd
 * @see wifi_updateLibHostApdConfig
 */
INT wifi_createHostApdConfig(INT apIndex, BOOL createWpsCfg);

/**
 * @brief Starts hostapd against the configuration already created.
 *
 * This function starts `hostapd`, using the variables in the `hostapd`
 * configuration. It takes no index and no configuration argument: the configuration is
 * whatever `wifi_createHostApdConfig()` last created for the Access Points concerned, which is
 * why that call precedes this one. This interface does not state whether one daemon serves
 * every Access Point or one is started per Access Point, so a caller must not infer a
 * per-Access-Point lifecycle from the absence of an index.
 *
 * The call belongs to the optional `hostapd` path described under `Optional Components` in
 * `docs/pages/halSpec.md`. Every implementation declares it, including one that drives the
 * driver by a proprietary route, so a successful return is not evidence that a `hostapd`
 * process exists on the platform.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup` in
 *      `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime requirements
 *      and, per `Component Runtime Execution Requirements`, likely results in undefined
 *      behaviour. For the configuration to take effect, `wifi_createHostApdConfig()` should
 *      have been called for the Access Points concerned; this interface does not state what
 *      this call does for an Access Point whose variables were never created.
 * @post On success the `HAL` has started `hostapd`. Because the call does not block, this
 *       interface provides no signal that the daemon is up and serving associations, and
 *       declares no notification for it, so a caller must not read a successful return as
 *       evidence that any Access Point is on the air; it should confirm operational state
 *       through the Access Point and radio status calls in `wifi_hal_ap.h` and
 *       `wifi_hal_radio.h`. This interface does not state the effect of calling it while
 *       `hostapd` is already running, so a caller should not use it as a restart.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The `HAL` accepted the start request.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. The caller should re-create
 *                          the configuration with `wifi_createHostApdConfig()` before
 *                          retrying; it must not read a persistent failure as proof that
 *                          the `hostapd` path is unavailable on that platform, since this
 *                          interface does not report that separately.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. This interface states no ordering between this call and a
 *       concurrent `wifi_stopHostApd()`, so a caller should serialize the two itself.
 *
 * @see wifi_createHostApdConfig
 * @see wifi_stopHostApd
 * @see wifi_getLibhostapd
 */
INT wifi_startHostApd();

/**
 * @brief Stops hostapd.
 *
 * The counterpart of `wifi_startHostApd()`, and like it, it takes no index: this interface does
 * not state whether the daemon it stops serves one Access Point or all of them, so a caller
 * must treat the effect as subsystem-wide rather than scoped. Stopping the daemon is not the
 * same as taking the radios off the air - `wifi_down()` does that - and this interface does not
 * state what happens to the stations currently associated through it.
 *
 * The call belongs to the optional `hostapd` path described under `Optional Components` in
 * `docs/pages/halSpec.md`, and is declared by every implementation whether or not that path is
 * the one in use.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup` in
 *      `docs/pages/halSpec.md`. A call made beforehand does not meet the runtime requirements
 *      and, per `Component Runtime Execution Requirements`, likely results in undefined
 *      behaviour. This interface does not state the effect of calling it when `hostapd` is not
 *      running, so a caller should not use it as a way to test whether it is.
 * @post On success the `HAL` has stopped `hostapd`. This interface does not state whether the
 *       configuration variables created by `wifi_createHostApdConfig()` survive, so a caller
 *       that intends to restart should re-create them rather than assume they are still there;
 *       nor does it state that the call blocks until the daemon has exited, so a caller must
 *       not read a successful return as evidence that no `hostapd` process remains. On failure
 *       this interface does not state whether the daemon was stopped, so its state is unknown
 *       rather than unchanged.
 *
 * @returns The status of the operation.
 * @retval WIFI_HAL_SUCCESS The `HAL` accepted the stop request.
 * @retval WIFI_HAL_ERROR   The call failed. This interface does not enumerate the
 *                          conditions that lead to this code. A caller that needs the
 *                          daemon down must verify it by another means rather than infer it
 *                          from the return code, and must not read a persistent failure as
 *                          proof of anything beyond this call not having succeeded.
 *
 * @note This call does not block, per `Blocking calls` in `docs/pages/halSpec.md`.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       `docs/pages/halSpec.md`. This interface states no ordering between this call and a
 *       concurrent `wifi_startHostApd()`, so a caller should serialize the two itself.
 *
 * @see wifi_startHostApd
 * @see wifi_createHostApdConfig
 * @see wifi_down
 */
INT wifi_stopHostApd();

/** @} */  //END OF GROUP WIFI_HAL_APIS

#ifdef __cplusplus
}
#endif

#endif
