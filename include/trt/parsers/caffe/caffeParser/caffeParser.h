/*
 * Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
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

#ifndef TRT_CAFFE_PARSER_CAFFE_PARSER_H
#define TRT_CAFFE_PARSER_CAFFE_PARSER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include <NvCaffeParser.h>

#include "trt/parsers/caffe/caffeWeightFactory/caffeWeightFactory.h"
#include "trt/parsers/caffe/blobNameToTensor.h"
#include "trt/parsers/caffe/proto/trtcaffe.pb.h"
#include "error_check.h"

namespace nvcaffeparser1 {
    class CaffeParser : public ICaffeParser {
    public:
        const IBlobNameToTensor* parse(const char* deploy,
                                       const char* model,
                                       nvinfer1::INetworkDefinition& network,
                                       nvinfer1::DataType weightType) override;

        const IBlobNameToTensor* parseBuffers(const char* deployBuffer,
                                              size_t deployLength,
                                              const char* modelBuffer,
                                              size_t modelLength,
                                              nvinfer1::INetworkDefinition& network,
                                              nvinfer1::DataType weightType) override;

        void setProtobufBufferSize(size_t size) override {
            mProtobufBufferSize = size;
        }

        void setPluginFactory(nvcaffeparser1::IPluginFactory* factory) override {
            mPluginFactory = factory;
        }

        void setPluginFactoryExt(nvcaffeparser1::IPluginFactoryExt* factory) override {
            mPluginFactory = factory;
            mPluginFactoryIsExt = true;
        }

        void setPluginFactoryV2(nvcaffeparser1::IPluginFactoryV2* factory) override {
            mPluginFactoryV2 = factory;
        }

        void setPluginNamespace(const char* libNamespace) override {
            mPluginNamespace = libNamespace;
        }

        IBinaryProtoBlob* parseBinaryProto(const char* fileName) override;

        void destroy() override {
            delete this;
        }

        void setErrorRecorder(nvinfer1::IErrorRecorder* recorder) override {
            (void)recorder;
            LOG_ASSERT(!"TRT- Not implemented.");
        }

        nvinfer1::IErrorRecorder* getErrorRecorder() const override {
            LOG_ASSERT(!"TRT- Not implemented.");
            return nullptr;
        }

    private:
        ~CaffeParser() override;

        std::vector<nvinfer1::PluginField> parseSliceParam(const trtcaffe::LayerParameter& msg,
                CaffeWeightFactory& weightFactory, BlobNameToTensor& tensors);

        std::vector<nvinfer1::PluginField> parseInstanceNormParam(const trtcaffe::LayerParameter &msg,
                CaffeWeightFactory &weightFactory, BlobNameToTensor &tensors);

        template <typename T>
        T* allocMemory(int size = 1) {
            T* tmpMem = static_cast<T*>(malloc(sizeof(T) * size));
            mTmpAllocs.push_back(tmpMem);
            return tmpMem;
        }

        const IBlobNameToTensor* parse(nvinfer1::INetworkDefinition& network,
                                       nvinfer1::DataType weightType,
                                       bool hasModel);

    private:
        std::shared_ptr<trtcaffe::NetParameter> mDeploy;
        std::shared_ptr<trtcaffe::NetParameter> mModel;
        std::vector<void*> mTmpAllocs;
        BlobNameToTensor* mBlobNameToTensor{nullptr};
        size_t mProtobufBufferSize{INT_MAX};
        nvcaffeparser1::IPluginFactory* mPluginFactory{nullptr};
        nvcaffeparser1::IPluginFactoryV2* mPluginFactoryV2{nullptr};
        bool mPluginFactoryIsExt{false};
        std::vector<nvinfer1::IPluginV2*> mNewPlugins;
        std::unordered_map<std::string, nvinfer1::IPluginCreator*> mPluginRegistry;
        std::string mPluginNamespace = "";
    };
} //namespace nvcaffeparser1
#endif //TRT_CAFFE_PARSER_CAFFE_PARSER_H
