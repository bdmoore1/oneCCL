/*
 Copyright 2016-2020 Intel Corporation
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "coll/selection/selection.hpp"

template <>
std::map<ccl_coll_alltoall_algo, std::string>
    ccl_algorithm_selector_helper<ccl_coll_alltoall_algo>::algo_names = {
        std::make_pair(ccl_coll_alltoall_direct, "direct"),
        std::make_pair(ccl_coll_alltoall_naive, "naive"),
        std::make_pair(ccl_coll_alltoall_scatter, "scatter"),
#ifdef CCL_ENABLE_SYCL
        std::make_pair(ccl_coll_alltoall_topo, "topo")
#endif // CCL_ENABLE_SYCL
    };

ccl_algorithm_selector<ccl_coll_alltoall>::ccl_algorithm_selector() {
#if defined(CCL_ENABLE_SYCL) && defined(CCL_ENABLE_ZE)
    insert(main_table, 0, CCL_SELECTION_MAX_COLL_SIZE, ccl_coll_alltoall_topo);
#else // CCL_ENABLE_SYCL && CCL_ENABLE_ZE
    insert(main_table, 0, CCL_SELECTION_MAX_COLL_SIZE, ccl_coll_alltoall_scatter);
    if (ccl::global_data::env().atl_transport == ccl_atl_mpi) {
        insert(main_table, 0, CCL_ALLTOALL_MEDIUM_MSG_SIZE, ccl_coll_alltoall_direct);
    }
#endif // CCL_ENABLE_SYCL && CCL_ENABLE_ZE
    insert(fallback_table, 0, CCL_SELECTION_MAX_COLL_SIZE, ccl_coll_alltoall_scatter);

    // scale-out table by default duplicates the main table
    // TODO: fill the table with algorithms which is suitable for the better scale-out performance.
    // Explanation: when implementing it was a simple scenario that does not contradict with the selection logic.
    // If there are no environemnt variable provided, scale-out path will go through the scaleout_table like it is a main_table
    // and use fallback path if nothing is suitable. Correct default behavior of each algorithm`s scale-out path is another task with discussion
    // and performance measurements.
    scaleout_table = main_table;
}

template <>
bool ccl_algorithm_selector_helper<ccl_coll_alltoall_algo>::can_use(
    ccl_coll_alltoall_algo algo,
    const ccl_selector_param& param,
    const ccl_selection_table_t<ccl_coll_alltoall_algo>& table) {
    bool can_use = true;

    if (algo == ccl_coll_alltoall_topo && !ccl_can_use_topo_algo(param)) {
        can_use = false;
    }
    else if (param.is_vector_buf && algo != ccl_coll_alltoall_scatter &&
             algo != ccl_coll_alltoall_naive && algo != ccl_coll_alltoall_topo) {
        can_use = false;
    }
    else if (algo == ccl_coll_alltoall_direct &&
             (ccl::global_data::env().atl_transport == ccl_atl_ofi)) {
        can_use = false;
    }

    return can_use;
}

CCL_SELECTION_DEFINE_HELPER_METHODS(ccl_coll_alltoall_algo,
                                    ccl_coll_alltoall,
                                    ccl::global_data::env().alltoall_algo_raw,
                                    param.count,
                                    ccl::global_data::env().alltoall_scaleout_algo_raw);
