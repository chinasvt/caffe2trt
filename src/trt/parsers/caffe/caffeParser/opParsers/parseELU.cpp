/* Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
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
 */

#include "trt/parsers/caffe/caffeParser/opParsers/opParsers.h"

using namespace nvinfer1;

namespace nvcaffeparser1 {
    ILayer* parseELU(INetworkDefinition& network, const trtcaffe::LayerParameter& msg,
            CaffeWeightFactory& /* weightFactory */, BlobNameToTensor& tensors) {
        if (!checkBlobs(msg, 1, 1))
            return nullptr;

        const trtcaffe::ELUParameter& p = msg.elu_param();

        float alpha = 1.f; // default parameter
        if (p.has_alpha())
            alpha = p.alpha();

        auto newLayer = network.addActivation(*tensors[msg.bottom(0)], ActivationType::kELU);
        newLayer->setAlpha(alpha);
        return newLayer;
    }
} //namespace nvcaffeparser1