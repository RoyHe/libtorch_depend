/*******************************************************************************
* Copyright 2021-2022 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef GRAPH_LNORM_HPP
#define GRAPH_LNORM_HPP

#include "dnnl_graph_common.hpp"
#include "lnorm/lnorm.hpp"

namespace benchdnnext {
namespace lnorm {

fill_status_t append_graph_with_block(const ::lnorm::prb_t *prb);
int doit(const ::lnorm::prb_t *prb, res_t *res);

} // namespace lnorm
} // namespace benchdnnext

#endif
