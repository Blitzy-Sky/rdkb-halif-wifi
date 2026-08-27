#!/usr/bin/env bash

# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2023 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# * http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *

# In the future this should moved to a fixed verison
HAL_GENERATOR_VERSION=develop

# This will look up the last tag in the git repo, depending on the project this may require modification
PROJECT_VERSION=$(git describe --tags | head -n1)

# Extra Doxygen settings this interface needs, in Doxygen configuration syntax. The
# declarations gated on WIFI_HAL_VERSION_3_PHASE2 - see `Platform or Product
# Customization` in docs/pages/halSpec.md - are dropped by Doxygen's preprocessor
# unless the flag is predefined for the documentation build, and the complementary
# arm in wifi_hal_deprecated.h is published in their place.
DOXYGEN_EXTRA_PARAMS="PREDEFINED = WIFI_HAL_VERSION_3_PHASE2=1"

# The settings above have to reach the configuration Doxygen actually reads, and nothing
# between this script and Doxygen carries them: the pinned generator's Makefile builds by
# running `doxygen Doxyfile.cfg` and never mentions DOXYGEN_EXTRA_PARAMS, its Doxyfile.cfg
# leaves PREDEFINED empty and expands only PROJECT_NAME and PROJECT_VERSION from the
# environment, and the generator is consumed at a pinned revision of another repository, so
# it cannot be edited from here. They are therefore applied by a small wrapper placed ahead
# of Doxygen on PATH: it appends them to the configuration file it is handed and feeds the
# result to the real Doxygen on standard input, which is otherwise exactly what the
# Makefile's recipe does. Remove the wrapper once the generator forwards
# DOXYGEN_EXTRA_PARAMS to Doxygen itself, and pass the settings through it instead.
apply_extra_doxygen_params() {
    local doxygen_bin
    doxygen_bin=$(command -v doxygen) || {
        echo "Doxygen is not found in the PATH:${PATH}, install it with ./build/doxygen_install.sh" >&2
        return 1
    }
    DOXYGEN_WRAPPER_DIR=$(mktemp -d) || return 1
    trap 'rm -rf "${DOXYGEN_WRAPPER_DIR}"' EXIT
    {
        echo '#!/usr/bin/env bash'
        printf 'doxygen_bin=%q\n' "${doxygen_bin}"
        printf 'extra_params=%q\n' "${DOXYGEN_EXTRA_PARAMS}"
        echo '# The generator invokes Doxygen with one configuration file; its Makefile'
        echo '# also probes the version with -v. Only the first form is augmented.'
        echo 'if [ "$#" -ne 1 ] || [ ! -f "$1" ]; then exec "${doxygen_bin}" "$@"; fi'
        echo '{ cat "$1"; printf "%s\n" "${extra_params}"; } | exec "${doxygen_bin}" -'
    } > "${DOXYGEN_WRAPPER_DIR}/doxygen" || return 1
    chmod +x "${DOXYGEN_WRAPPER_DIR}/doxygen" || return 1
    PATH="${DOXYGEN_WRAPPER_DIR}:${PATH}"
    export PATH
}
apply_extra_doxygen_params || exit 1

# Check if the common document configuration is present, if not clone it
if [ -d "./build" ]; then
    make -C ./build PROJECT_NAME="RDK-B Wifi HAL" PROJECT_VERSION=${PROJECT_VERSION} DOXYGEN_EXTRA_PARAMS="${DOXYGEN_EXTRA_PARAMS}"
else
    echo "Cloning Common documentation generation"
    git clone git@github.com:rdkcentral/hal-doxygen.git build
    cd ./build
    git checkout ${HAL_GENERATOR_VERSION}
    cd ..
    ./${0}
fi