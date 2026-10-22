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
#ifndef __WIFI_HAL_STA_H__
#define __WIFI_HAL_STA_H__

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief Station capabilities.
 *
 * This interface defines the structure with no members, so an instance carries no
 * capability data and exposes no field a caller can read or write.
 * `wifi_getStationCapability()` takes a pointer to it, which means that under the
 * shipped interface that call conveys no capability information back to the caller.
 * What a member set would contain is not specified by this interface.
 *
 * @note A caller must not assume any member, size or alignment for this type, and must
 *       not interpret the bytes of an instance it has passed to the `HAL`. An empty
 *       structure is a compiler extension in C and a standard construct in C++, and the
 *       two do not agree on its size, so a caller must not size, copy or compare an
 *       instance either.
 *
 * @see wifi_getStationCapability
 */
typedef struct
{

} wifi_sta_capability_t;

/**
 * @brief Station statistics.
 *
 * The `HAL` fills an instance of this structure in `wifi_getStationStats()`, where the
 * caller allocates the storage and keeps ownership of it, and passes a pointer to
 * `HAL`-owned storage to a registered `wifi_staConnectionStatus_callback` handler; see
 * `Memory Model` in the HAL specification. Every member is written by the `HAL` and
 * read by the caller.
 *
 * @see wifi_getStationStats
 * @see wifi_staConnectionStatus_callback
 */
typedef struct
{
    UINT vap_index; /*!< Index of the client `VAP` these statistics describe. The
                         indices this interface defines are `AP_INDEX_1` to
                         `AP_INDEX_24` in `wifi_hal_generic.h`. */
    wifi_connection_status_t connect_status; /*!< Connection state reported for the
                         `VAP`, as one of the `wifi_connection_status_t` values in
                         `wifi_hal_generic.h`: disabled, disconnected, connected, or the
                         target `AP` not found. The interface names these as reported
                         values only and does not specify the transitions between
                         them. */
    UINT channel; /*!< IEEE 802.11 channel number the `VAP` is operating on. The band is
                         not carried in this structure; a caller that needs it must take
                         it from the `BSS` information, for example `oper_freq_band` in
                         `wifi_bss_info_t`. */
    UINT channelWidth; /*!< Bandwidth the connection is using. This interface does not
                         state the encoding of this member; the comparable `UINT` width
                         member elsewhere in the interface, `ap_channelWidth` in
                         `wifi_hal_telemetry.h`, carries a `wifi_channelBandwidth_t`
                         code rather than a count of MHz. */
    UINT op_class; /*!< Operating class the connection is using. This interface numbers
                         operating classes as IEEE 802.11-2020 Table E-4 global
                         operating classes; see `op_class_ch_list_t` in
                         `wifi_hal_generic.h`. */
} wifi_station_stats_t;
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Connects a client VAP to a specified BSS.
 *
 * The call hands the `HAL` the identity of one target `BSS` - its `BSSID`, `SSID`, band,
 * security mode and encryption method - and asks it to bring the given client `VAP` onto
 * that `BSS`. A caller can obtain a `BSS` description from `wifi_findNetworks()` rather
 * than constructing one.
 *
 * @param[in] ap_index Index of the client VAP. The indices this interface defines are
 *                     `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`; which of
 *                     them is provisioned as a client `VAP` is platform-dependent and is
 *                     not established by this interface.
 * @param[in] bss      Pointer to a `wifi_bss_info_t` structure containing information
 *                     about the BSS to connect to. One `BSS` is passed, not an array. The
 *                     caller allocates and owns the storage, per `Memory Model` in
 *                     the HAL specification; the `HAL` reads it during the call, and
 *                     whether the implementation retains the pointer afterwards is not
 *                     specified by this interface, so the caller should keep the
 *                     structure allocated and unmoved while the `HAL` remains
 *                     initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the `HAL` has accepted the connect request for `ap_index`. This
 *       interface does not state whether the association is complete when the call
 *       returns, so a caller must not read a successful return as evidence that the `VAP`
 *       is connected: confirm the state with `wifi_getStationStats()`, or observe it
 *       through a registered `wifi_staConnectionStatus_callback` handler. Whether that
 *       handler is the completion signal for this call is not specified by this interface
 *       either.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK The `HAL` accepted the connect request for `ap_index`.
 * @retval RETURN_ERR The call failed. This interface does not enumerate the conditions that
 *                    lead to this code. The caller should validate its arguments and read
 *                    the connection state back before retrying; it must not read a
 *                    persistent failure as proof that the `BSS` is unreachable, since this
 *                    interface does not report that separately.
 *
 * @note `RETURN_OK` and `RETURN_ERR`, defined in `wifi_hal_generic.h`, carry the same
 *       values as the `WIFI_HAL_SUCCESS` and `WIFI_HAL_ERROR` codes the rest of this
 *       interface uses for the same two outcomes. The two codes above are the whole
 *       return-code contract of this call, per `Internal Error Handling` in
 *       the HAL specification.
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 *
 * @see wifi_findNetworks
 * @see wifi_disconnect
 * @see wifi_getStationStats
 */
INT wifi_connect(INT ap_index, wifi_bss_info_t *bss);

/**
 * @brief Disconnects a client VAP.
 *
 * The call asks the `HAL` to drop the `VAP`'s current association, so that the `VAP`
 * stops being a client of the `BSS` it joined. It takes no `BSS` argument: the `VAP`
 * index alone identifies the association to remove.
 *
 * @param[in] ap_index Index of the client VAP. The indices this interface defines are
 *                     `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`; which of
 *                     them is provisioned as a client `VAP` is platform-dependent and is
 *                     not established by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the `HAL` has accepted the disconnect request for `ap_index`. This
 *       interface does not state whether the `VAP` has left the `BSS` by the time the
 *       call returns, so a caller that must observe the result should read
 *       `wifi_getStationStats()` or wait for a registered
 *       `wifi_staConnectionStatus_callback` handler. The interface does not state
 *       whether the `HAL` re-associates the `VAP` afterwards, so a caller that requires a
 *       connection should issue `wifi_connect()`.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK The `HAL` accepted the disconnect request for `ap_index`.
 * @retval RETURN_ERR The call failed. This interface does not enumerate the conditions that
 *                    lead to this code, and it does not separate a `VAP` that was not
 *                    associated in the first place from a genuine failure, so the caller
 *                    should read the connection state with `wifi_getStationStats()` rather
 *                    than inferring it from this code.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 *
 * @see wifi_connect
 * @see wifi_getStationStats
 */
INT wifi_disconnect(INT ap_index);

/**
 * @brief Gets the capabilities of a station.
 *
 * `wifi_sta_capability_t` is defined with no members in this interface, so a successful
 * call conveys no capability data: it reports only that the `HAL` accepted the request
 * for the given index. A caller must not read anything out of `cap`, and must not treat
 * this call as a source of station capability information under the shipped interface.
 *
 * @param[in] ap_index  Index of the client VAP. The indices this interface defines are
 *                      `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`; which of
 *                      them is provisioned as a client `VAP` is platform-dependent and
 *                      is not established by this interface.
 * @param[out] cap      Pointer to a caller-allocated `wifi_sta_capability_t`. The caller
 *                      allocates and owns the storage, per `Memory Model` in
 *                      the HAL specification; the `HAL` writes no member through it,
 *                      because the structure defines none. Whether the implementation
 *                      retains the pointer beyond the call is not specified by this
 *                      interface, so the caller should keep the structure allocated and
 *                      unmoved while the `HAL` remains initialised.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success the `HAL` has accepted the request for `ap_index`; `cap` holds no
 *       capability data, because the structure has no members to write.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK The `HAL` accepted the request for `ap_index`.
 * @retval RETURN_ERR The call failed. This interface does not enumerate the conditions that
 *                    lead to this code. The caller should validate its arguments and log a
 *                    failure that persists across retries; because no data crosses the
 *                    interface here, there is nothing further for the caller to recover.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 *
 * @see wifi_sta_capability_t
 * @see wifi_getStationStats
 */
INT wifi_getStationCapability(INT ap_index, wifi_sta_capability_t *cap);

/**
 * @brief Finds available networks.
 *
 * The call scans on behalf of a client `VAP` and returns the `BSS`s it found, which is
 * how a caller obtains the `wifi_bss_info_t` description that `wifi_connect()` expects.
 * The scan is scoped by the channel and band the caller supplies.
 *
 * @param[in] ap_index  Index of the client VAP. The indices this interface defines are
 *                      `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`; which of
 *                      them is provisioned as a client `VAP` is platform-dependent and
 *                      is not established by this interface.
 * @param[in] channel   Pointer to a `wifi_channel_t` structure containing the channel
 *                      number and band information. The caller allocates and owns it and
 *                      the `HAL` reads it during the call. One channel and band pair is
 *                      passed. Elsewhere in this interface a channel of `0` means every
 *                      channel in the band - see `wifi_scan_params_t` in
 *                      `wifi_hal_ap.h` - but this call does not state that it honours the
 *                      same convention, so a caller should name the channel it wants.
 * @param[out] bss      Pointer to a pointer to an array of `wifi_bss_info_t`
 *                      structures. The array is allocated by the HAL layer and
 *                      should be freed by the caller. That is an explicit exception to
 *                      `Memory Model` in the HAL specification, under which memory the
 *                      `HAL` creates stays `HAL`-owned, and the exception applies only to
 *                      this array. On success the array holds `*num_bss` elements. This
 *                      interface does not name the function the `HAL` allocated it with,
 *                      so the matching release function is a platform convention rather
 *                      than something established here.
 * @param[out] num_bss  Pointer to a variable to store the number of BSSs found. The
 *                      caller allocates and owns the variable. On success it holds the
 *                      number of elements in the returned array; a value of zero means no
 *                      `BSS` was found, and this interface does not state whether `*bss`
 *                      is a usable pointer in that case. On failure the interface does
 *                      not state whether either output was written, so a caller must
 *                      treat both as undefined and must not release `*bss`.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success `*bss` points to an array of `*num_bss` `wifi_bss_info_t` structures
 *       that the caller is responsible for freeing, and each element describes one `BSS`
 *       observed on the requested channel and band. On failure both outputs must be
 *       treated as undefined and neither released.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK The scan completed and the outputs were written; `*num_bss` may be
 *                   zero if no `BSS` was found.
 * @retval RETURN_ERR The call failed. This interface does not enumerate the conditions that
 *                    lead to this code. The caller should validate its arguments, release
 *                    nothing, and retry the scan rather than treating the result as an
 *                    empty network list.
 *
 * @warning The `HAL` allocates a fresh array on each successful call and nothing in this
 *          interface releases a previous one, so a caller that scans repeatedly must free
 *          every array it is given.
 * @note This call does not block, per `Blocking calls` in the HAL specification. The
 *       interface does not state how a scan that takes longer than the call is completed,
 *       so a caller must not assume the returned list reflects a scan performed inside
 *       this call rather than a cached one.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 *
 * @see wifi_connect
 * @see wifi_bss_info_t
 */
INT wifi_findNetworks(INT ap_index, wifi_channel_t *channel, wifi_bss_info_t **bss, UINT *num_bss);

/**
 * @brief Gets station statistics.
 *
 * The call reports the connection state of a client `VAP` together with the channel,
 * bandwidth and operating class it is using. It is the polled counterpart of
 * `wifi_staConnectionStatus_callback`, and the way a caller confirms the outcome of an
 * earlier `wifi_connect()` or `wifi_disconnect()`.
 *
 * @param[in] ap_index Index of the client VAP. The indices this interface defines are
 *                     `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`; which of
 *                     them is provisioned as a client `VAP` is platform-dependent and is
 *                     not established by this interface.
 * @param[out] sta      Pointer to a `wifi_station_stats_t` structure to store the
 *                      station statistics. The caller allocates and owns the storage, per
 *                      `Memory Model` in the HAL specification; the `HAL` writes the
 *                      members during the call. Whether the implementation retains the
 *                      pointer beyond the call is not specified by this interface, so the
 *                      caller should keep the structure allocated and unmoved while the
 *                      `HAL` remains initialised. A single structure is written, not an
 *                      array.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. A call made beforehand does not meet the runtime
 *      requirements and, per `Component Runtime Execution Requirements`, likely results
 *      in undefined behaviour.
 * @post On success every member of `*sta` has been written, and `sta->connect_status`
 *       carries the state the `HAL` currently reports for `ap_index`. On failure the
 *       contents are undefined and must not be read.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK The statistics were retrieved.
 * @retval RETURN_ERR The call failed. This interface does not enumerate the conditions that
 *                    lead to this code. The caller should validate its arguments and treat
 *                    the connection state as unknown rather than as disconnected.
 *
 * @note This call does not block, per `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 *
 * @see wifi_station_stats_t
 * @see wifi_staConnectionStatus_callback_register
 */
INT wifi_getStationStats(INT ap_index, wifi_station_stats_t *sta);
/** @} */  //END OF GROUP WIFI_HAL_APIS

/**
 * @addtogroup WIFI_HAL_TYPES
 * @{
 */
/**
 * @brief Callback function invoked when the station connection status changes.
 *
 * A caller installs a handler of this type with
 * `wifi_staConnectionStatus_callback_register()`. The `HAL` then invokes it on each
 * connection-status change of a client `VAP`, passing the `BSS` the change relates to and
 * the `VAP`'s statistics, whose `connect_status` member carries the state being reported.
 *
 * @param[in] apIndex  Index of the client VAP. The indices this interface defines are
 *                     `AP_INDEX_1` to `AP_INDEX_24` in `wifi_hal_generic.h`.
 * @param[in] bss_dev  Pointer to a `wifi_bss_info_t` structure containing information
 *                     about the BSS. The `HAL` owns this storage and it is valid only for
 *                     the duration of the call; the client is responsible for creating a
 *                     copy of the data it needs afterwards, per `Asynchronous
 *                     Notification Model` in the HAL specification.
 * @param[in] sta      Pointer to a `wifi_station_stats_t` structure containing the
 *                     station statistics. Owned by the `HAL` and valid only for the
 *                     duration of the call, on the same terms as `bss_dev`; its
 *                     `connect_status` member carries the reported state.
 *
 * @returns The status of the operation, returned by the handler to the `HAL`.
 * @retval RETURN_OK The handler accepted the notification.
 * @retval RETURN_ERR The handler could not process the notification.
 *
 * @note This interface does not state how the `HAL` acts on a non-success return, so a
 *       handler must not rely on the notification being retried or re-delivered.
 * @note The handler must not suspend and must not invoke any blocking system calls; it
 *       should do no more than pass the event to a driver event handler task, per
 *       `Blocking calls` in the HAL specification.
 * @note The `HAL` is expected to be thread safe and may enter the handler on a `HAL`
 *       thread, so the handler must serialise its own access to caller state; see
 *       `Threading Model` in the HAL specification.
 *
 * @see wifi_staConnectionStatus_callback_register
 * @see wifi_station_stats_t
 */
typedef INT ( * wifi_staConnectionStatus_callback)(INT apIndex, wifi_bss_info_t *bss_dev, wifi_station_stats_t *sta);
/** @} */  //END OF GROUP WIFI_HAL_TYPES

/**
 * @addtogroup WIFI_HAL_APIS
 * @{
 */
/**
 * @brief Registers a callback function for station connection status changes.
 *
 * After registration the `HAL` reports each connection-status change of a client `VAP`
 * through the supplied handler, so a caller can observe a `VAP` becoming connected or
 * disconnected, or its target `AP` not being found, without polling
 * `wifi_getStationStats()`. the HAL specification names this function under
 * `Asynchronous Notification Model` as the registration point for client connection
 * status.
 *
 * @param[in] callback_proc Pointer to the callback function to register, of type
 *                          `wifi_staConnectionStatus_callback`. The `HAL` keeps this
 *                          function pointer after the call returns, since it invokes
 *                          the handler later, so the function must remain callable for
 *                          as long as notifications are wanted. This interface
 *                          declares no call that removes a handler and states no end
 *                          to a registration, so it does not establish when the `HAL`
 *                          stops using the pointer. The effect of passing NULL is not
 *                          specified by this interface.
 *
 * @pre `wifi_init()` must have completed successfully; see `Initialization and Startup`
 *      in the HAL specification. The effect of registering beforehand is not specified
 *      by this interface.
 * @post The handler is installed and is invoked on each subsequent connection-status
 *       change of a client `VAP`.
 *
 * @execution callback
 * @sideeffect Installs `callback_proc` as the handler the `HAL` invokes on each
 *             connection-status change of a client `VAP`, and keeps that function
 *             pointer after this call returns. This interface states no other effect.
 *
 * @note The registration call itself is synchronous and returns nothing; delivery of
 *       `wifi_staConnectionStatus_callback` is asynchronous.
 * @note This interface defines a single registration point and describes neither a list
 *       of handlers nor a way to unregister. It does not state whether a later
 *       registration replaces an earlier one, adds to it or is rejected, so a caller
 *       should register once and must not assume a handler can be removed.
 * @note This function must not suspend and must not invoke any blocking system calls; see
 *       `Blocking calls` in the HAL specification. The same holds for the handler.
 * @note The `HAL` is expected to be thread safe, per `Threading Model` in
 *       the HAL specification.
 * @warning The `wifi_bss_info_t` and `wifi_station_stats_t` data reaching the handler is
 *          owned by the `HAL` and is valid only for the duration of that call; the client
 *          must copy whatever it needs afterwards, per `Asynchronous Notification Model`
 *          in the HAL specification.
 *
 * @see wifi_staConnectionStatus_callback
 * @see wifi_getStationStats
 */
void wifi_staConnectionStatus_callback_register(wifi_staConnectionStatus_callback callback_proc);
/** @} */  //END OF GROUP WIFI_HAL_APIS

#ifdef __cplusplus
}
#endif

#endif
