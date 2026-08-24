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
    What is new for 2.2.0
      1. Add Country Code support
      2. Add more DCS function
      3. Move RadiusSecret from struct wifi_radius_setting_t to wifi_getApSecurityRadiusServer function
      4. Add wifi_getApSecuritySecondaryRadiusServer
    What is new for 2.2.1
      1. Add wifi_setRadioTrafficStatsMeasure, wifi_setRadioTrafficStatsRadioStatisticsEnable
    What is new for 2.2.2
      1. Add Band Steering HAL
    What is new for 2.3.0
      1. Add AP Beacon Rate control HAL
      2. Add Dynamic Channel Selection (phase 2) HAL
      3. Add Air Time Management HAL
    What is new for 2.4.0
      1. Add data structure and HAL for mesh
    What is new for 2.5.0
      1. Add the Channel switch HAL for mesh
    What is new for 2.6.0
      1. Add the Band steering HAL for mesh
    What is new for 2.7.0
      1. Add HAL for Wifi telemetry
    What is new for 2.8.0
      1. Add HAL for 11w
    What is new for 2.9.0
      1. Add HAL function definitions for 802.11r Fast Transition
    What is new for 2.10.0
      1. Add HAL function definitions for 802.11v BSS Transition Management
    What is new for 2.11.0
      1. Add HAL function definitions for 802.11k Neighbor Request and Response definitions
    What is new for 2.12.0
      1. Add HAL function definitions for 802.11k Beacon Request and Response definitions
    What is new for 2.13.0
      1. Add HAL function definitions for DPP
    What is new for 2.14.0
      1. Add HAL function definitions for steering effectiveness telemetry
    What is new for 2.15.0
      1. Add HAL function definitions for 802.11ax
      2. Add HAL definitions for dfs channel state
      3. Add HAL function definitions for EAP parameters
    What is new for 2.16.0
      1. Modified HAL structure definition for VAP Telemetry
    What is new for 2.17.0
      1. Add HAL function definition for Single Client reporting feature
    What is new for 2.18.0
      1. Add HAL Fuction Definition for Absolute TX-Power retreival
    What is new for 2.19.0
      1. Added zerowait DFS status support
      2. Modified HAL definitions for EAP parameters
      3. Updated comments for ChannelUtilization, ActivityFactor, CarrierSenseThreshold_Exceeded
         and RetransmissionMetirc radio metrics
    What is new for 2.20.0
      1. Add HAL definitions for Multi PSK support
    What is new for 3.0.0
      1. Code refactoring
      2. Add HAL definitions for tri-band and 6GHz support
**********************************************************************/
#ifndef __WIFI_HAL_H__
#define __WIFI_HAL_H__
/**
 * @file wifi_hal.h
 * @brief Aggregates the RDK-B Wi-Fi HAL interface headers into a single entry point.
 *
 * Include this header to obtain the complete Wi-Fi HAL surface in one step. It
 * declares no function or type of its own; every declaration reaches a caller
 * through one of the per-area headers included below. The `Notes:` block above
 * records the interface changes for releases 2.2.0 through 3.0.0, and that record
 * continues in wifi_hal_generic.h, which also declares the interface version macros
 * a caller can compile against.
 *
 * @note Of the per-area headers, only wifi_hal_generic.h and wifi_hal_telemetry.h
 *       compile in isolation. The others use the base types wifi_hal_generic.h
 *       declares, so reach them through this umbrella rather than including one on
 *       its own.
 *
 * @note wifi_hal_emu.h carries a banner naming this module, but no header includes
 *       it, so it contributes nothing to the compile surface. The generated
 *       documentation extracts it regardless, because the generator recurses over
 *       the include directory; it is not part of the documented Wi-Fi HAL contract.
 */
/**
 * @defgroup WIFI_HAL_TYPES Wi-Fi HAL Data Types and Constants
 *
 * The structures, enumerations, typedefs and constants a caller populates or
 * interprets when using the Wi-Fi HAL, collected from every per-area header.
 * Callback function typedefs belong to this group, while the registration functions
 * that install them belong to @ref WIFI_HAL_APIS.
 */
/**
 * @defgroup WIFI_HAL_APIS Wi-Fi HAL APIs
 *
 * The callable Wi-Fi HAL functions, contributed by each per-area header included
 * below. Complete wifi_init() successfully before invoking any other function in
 * this group. The interface is expected to be thread safe and none of its calls
 * should block; the "Initialization and Startup", "Threading Model" and "Blocking
 * calls" sections of docs/pages/halSpec.md state those requirements.
 */
/**
 * Wifi generic API and types definition used across multiples modules.
 * Includes wifi_init(), wifi_factoryReset(), wifi_setLED(), etc.
 */
#include "wifi_hal_generic.h"
/**
 * Wifi Radio specific APIs.
 * Includes API to configure and read configuration, factory-reset and reset of the radio. 
 */
#include "wifi_hal_radio.h"
/**
 * Wifi VAP specific APIs.
 * Includes API to create, configure and read configuration, factory-reset and reset of the VAP. 
 */
#include "wifi_hal_ap.h"
/**
* Wifi STA specific APIs.
* Includes API to create, configure and read configuration the STA VAP. 
*/
#include "wifi_hal_sta.h"
/**
 * Wifi measurement and Telemetry specific APIs.
 * Includes API to read radio and SSID traffic statistics, neighbouring-AP scan
 * results, associated-device diagnostics and VAP and unassociated-station metrics,
 * and to enable the statistics those reads report.
 */
#include "wifi_hal_telemetry.h"
/**
* Wifi client management specific APIs.
* Includes API to configure band steering, associated-device high-watermark
* counters, EAP parameters, 802.11r Fast BSS Transition, and the 802.11v BSS
* Transition and 802.11k Neighbor Report switches.
 */
#include "wifi_hal_client_mgt.h"
/**
* Wifi extender specific APIs.
* Includes API's used by Plume Mesh.
 */
#include "wifi_hal_extender.h"

/**
* Wifi deprecated APIs.
* APIs to be deprecated. Not add new function or structure!
*
* Including the header here puts its declarations on the public compile surface of
* every translation unit that includes wifi_hal.h, and the generated documentation
* extracts them as well. They are nonetheless outside the documented Wi-Fi HAL
* contract: the RDK-B HAL inventory that consumes this repository declares
* wifi_hal_deprecated.h out of scope, so its declarations carry no per-function
* documentation and no test is generated against them.
*
* @warning No replacement is named for any declaration the header marks "Deprecated:
*          used for old RDKB code.", so this interface states no mapping from a
*          deprecated name to a supported one. Take the supported declaration for a
*          functional area from that area's header above - generic, radio, ap, sta,
*          telemetry, client management or extender.
*/
#include "wifi_hal_deprecated.h"

#endif
